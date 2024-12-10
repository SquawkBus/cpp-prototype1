#include <format>
#include <utility>

#include "logging/log.hpp"

#include "interactor.hpp"
#include "notification_manager.hpp"
#include "subscription_manager.hpp"

namespace squawkbus::server
{
  namespace
  {
    auto log = logging::logger("squawkbus");
  }

  using squawkbus::messages::SubscriptionRequest;

  void SubscriptionManager::on_subscription(
    Interactor* subscriber,
    const SubscriptionRequest& request,
    const NotificationManager& notification_manager)
  {
    if (request.is_add)
      add_subscription(subscriber, request.topic, notification_manager);
    else
      remove_subscription(subscriber, request.topic, notification_manager);
  }

  void SubscriptionManager::on_interactor_closed(
    Interactor* subscriber,
    const NotificationManager& notification_manager)
  {
    remove_interactor(subscriber, notification_manager);
  }

  void SubscriptionManager::add_subscription(
    Interactor* subscriber,
    const std::string& topic,
    const NotificationManager& notification_manager)
  {
    log.debug(
      std::format(
        "Subscribing {} to \"{}\".",
        subscriber->str(),
        topic));

    // Try to find the topic pattern.
    auto i_subscribers = subscriptions_.find(topic);
    if (i_subscribers == subscriptions_.end())
    {
      // New topic.
      subscriptions_.insert( { {topic, { {subscriber, 1} }} });
      // Only notify for new subscriptions.
      notification_manager.notify(subscriber, topic, true);
    }
    else
    {
      // Existing topic.
      auto& subscribers = i_subscribers->second;
      auto i_subscriber = subscribers.find(subscriber);
      if (i_subscriber == subscribers.end())
      {
        // new subscriber for topic.
        subscribers.insert({ {subscriber, 1} });
        // Only notify for new subscriptions.
        notification_manager.notify(subscriber, topic, true);
      }
      else
      {
        // increment the subscription count.
        i_subscriber->second += 1;
      }
    }

    // Handle the reverse lookup.
    auto i_subscriber_topics = subscriber_topics_.find(subscriber);
    if (i_subscriber_topics == subscriber_topics_.end())
    {
      // A new subscriber.
      subscriber_topics_.insert({ {subscriber, { {topic} }} });
    }
    else
    {
      // Add this topic to the subscriber.
      i_subscriber_topics->second.insert(topic);
    }
  }

  void SubscriptionManager::remove_subscription(
    Interactor* subscriber,
    const std::string& topic,
    const NotificationManager& notification_manager)
  {
    log.debug(
      std::format("Unsubscribing {} from \"{}\".",
      subscriber->str(),
      topic));

    auto i_subscriptions = subscriptions_.find(topic);
    if (i_subscriptions == subscriptions_.end())
    {
      // The topic has no subscribers.
      return;
    }

    auto subscribers = i_subscriptions->second;
    auto i_subscriber = subscribers.find(subscriber);
    if (i_subscriber == subscribers.end())
    {
      return;
    }

    // Decrement subscription count.
    i_subscriber->second -= 1;
    if (i_subscriber->second > 0)
    {
      // The subscriber still monitors this topic.
      return;
    }

    // The subscriber no longer monitors this topic so remove it.
    subscribers.erase(i_subscriber);
    if (subscribers.empty())
    {
      // The topic no longer has subscribers.
      subscriptions_.erase(topic);
      // Only notify for final un-subscriptions.
      notification_manager.notify(subscriber, topic, false);
    }

    auto i_subscriber_topics = subscriber_topics_.find(subscriber);
    if (i_subscriber_topics == subscriber_topics_.end())
    {
      // This would be a program logic error.
      return;
    }

    // Remove the topic from the subscriber.
    i_subscriber_topics->second.erase(topic);
    if (i_subscriber_topics->second.empty())
    {
      // Remove a subscriber with no subscriptions.
      subscriber_topics_.erase(subscriber);
    }
  }

  void SubscriptionManager::remove_interactor(
    Interactor* subscriber,
    const NotificationManager& notification_manager)
  {
    auto i_subscriber_topics = subscriber_topics_.find(subscriber);
    if (i_subscriber_topics == subscriber_topics_.end())
    {
      // The subscriber has no subscriptions.
      return;
    }

    // Go through each of the subscriptions removing the subscriber.
    for (auto& topic : i_subscriber_topics->second)
    {
      auto i_subscriptions = subscriptions_.find(topic);
      if (i_subscriptions == subscriptions_.end())
      {
        // This should never happen, and would be a logic error.
        continue;
      }

      i_subscriptions->second.erase(subscriber);
      if (!i_subscriptions->second.empty())
      {
        // The topic pattern has other subscribers.
        continue;
      }

      subscriptions_.erase(topic);

      notification_manager.notify(subscriber, topic, false);
    }

    subscriber_topics_.erase(subscriber);
  }

  std::set<Interactor*> SubscriptionManager::find_subscribers(const std::string& topic) const
  {
    auto subscribers = std::set<Interactor*> {};

    auto i_subscriptions = subscriptions_.find(topic);
    if (i_subscriptions != subscriptions_.end())
    {
      for (auto& [subscriber, count] : i_subscriptions->second)
      {
        subscribers.insert(subscriber);
      }
    }
    
    return subscribers;
  }

  std::vector<std::pair<std::string, std::vector<Interactor*>>> SubscriptionManager::find_matching_subscriptions(const std::regex& regex) const
  {
    std::vector<std::pair<std::string, std::vector<Interactor*>>> matching_subscriptions;

    for (auto& [topic, subscriptions] : subscriptions_)
    {
      if (std::regex_match(topic, regex))
      {
        auto interactors = std::vector<Interactor*>{};
        for (auto& [interactor, _count] : subscriptions)
        {
          interactors.push_back(interactor);
        }
        matching_subscriptions.push_back({topic, interactors});
      }
    }

    return matching_subscriptions;
  }

}
