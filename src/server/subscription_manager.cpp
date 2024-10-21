#include <format>
#include <utility>

#include "logging/log.hpp"

#include "interactor.hpp"
#include "notification_manager.hpp"
#include "subscription_manager.hpp"

namespace logging = squawkbus::logging;

namespace squawkbus::server
{
  using squawkbus::messages::SubscriptionRequest;

  void SubscriptionManager::on_subscription(
    Interactor* subscriber,
    SubscriptionRequest* message,
    NotificationManager& notification_manager)
  {
    logging::debug(
      std::format(
        "on_subscription: {} ({})",
        message->topic_pattern(),
        (message->is_add() ? "<true>" : "<false>")));

    if (message->is_add())
      add_subscription(subscriber, message->topic_pattern());
    else
      remove_subscription(subscriber, message->topic_pattern());

    notification_manager.notify(subscriber, message->topic_pattern(), message->is_add());
  }

  void SubscriptionManager::on_interactor_closed(Interactor* subscriber)
  {
    remove_interactor(subscriber);
  }

  void SubscriptionManager::add_subscription(
    Interactor* subscriber,
    const std::string& topic_pattern)
  {
    logging::debug(std::format( "add_subscription: {}", topic_pattern));

    // Try to find the topic pattern.
    auto i_subscribers = subscriptions_.find(topic_pattern);
    if (i_subscribers == subscriptions_.end())
    {
      // New topic pattern.
      subscriptions_.insert( { {topic_pattern, { {subscriber, 1} }} });
      regex_cache_.insert({ {topic_pattern, std::regex(topic_pattern)} });
    }
    else
    {
      // Existing topic pattern.
      auto subscribers = i_subscribers->second;
      auto i_subscriber = subscribers.find(subscriber);
      if (i_subscriber == subscribers.end())
      {
        // new subscriber for topic pattern.
        subscribers.insert({ {subscriber,1 } });
      }
      else
      {
        // increment the subscription count.
        i_subscriber->second += 1;
      }
    }

    // Handle the reverse lookup.
    auto i_subscriber_topic_patterns = subscriber_topic_patterns_.find(subscriber);
    if (i_subscriber_topic_patterns == subscriber_topic_patterns_.end())
    {
      // A new subscriber.
      subscriber_topic_patterns_.insert({ {subscriber, { {topic_pattern} }} });
    }
    else
    {
      // Add this topic to the subscriber.
      i_subscriber_topic_patterns->second.insert(topic_pattern);
    }
  }

  void SubscriptionManager::remove_subscription(
    Interactor* subscriber,
    const std::string& topic_pattern)
  {
    logging::debug(std::format( "remove_subscription: {}", topic_pattern));

    auto i_subscriptions = subscriptions_.find(topic_pattern);
    if (i_subscriptions == subscriptions_.end())
    {
      // The pattern has no subscribers.
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
      // The topic pattern no longer has subscribers.
      subscriptions_.erase(topic_pattern);
      regex_cache_.erase(topic_pattern);
    }

    auto i_subscriber_topic_patterns = subscriber_topic_patterns_.find(subscriber);
    if (i_subscriber_topic_patterns == subscriber_topic_patterns_.end())
    {
      // This would be a program logic error.
      return;
    }

    // Remove the topic pattern from the subscriber.
    i_subscriber_topic_patterns->second.erase(topic_pattern);
    if (i_subscriber_topic_patterns->second.empty())
    {
      // Remove a subscriber with no subscriptions.
      subscriber_topic_patterns_.erase(subscriber);
    }
  }

  void SubscriptionManager::remove_interactor(Interactor* subscriber)
  {
    auto i_subscriber_topic_patterns = subscriber_topic_patterns_.find(subscriber);
    if (i_subscriber_topic_patterns == subscriber_topic_patterns_.end())
    {
      // The subscriber has no subscriptions.
      return;
    }

    // Go through each of the subscriptions removing the subscriber.
    for (auto& topic_pattern : i_subscriber_topic_patterns->second)
    {
      auto i_subscriptions = subscriptions_.find(topic_pattern);
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

      subscriptions_.erase(topic_pattern);
      regex_cache_.erase(topic_pattern);
    }

    subscriber_topic_patterns_.erase(subscriber);
  }

  std::set<Interactor*> SubscriptionManager::find_subscribers(const std::string& topic) const
  {
    auto subscribers = std::set<Interactor*> {};

    // Go through each of the topic patterns checking for a match.
    for (auto& [topic_pattern, regex] : regex_cache_)
    {
      if (!std::regex_match(topic, regex))
      {
        // No match.
        continue;
      }

      auto i_subscriptions = subscriptions_.find(topic_pattern);
      if (i_subscriptions == subscriptions_.end())
      {
        // A match is found with no subscribers.
        // Should never happen.
        continue;
      }

      // Add the interactors that have subscribed to this topic pattern.
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

    for (auto& [topic_pattern, subscriptions] : subscriptions_)
    {
      if (std::regex_match(topic_pattern, regex))
      {
        auto interactors = std::vector<Interactor*>{};
        for (auto& [interactor, _count] : subscriptions)
        {
          interactors.push_back(interactor);
        }
        matching_subscriptions.push_back({topic_pattern, interactors});
      }
    }

    return matching_subscriptions;
  }

}
