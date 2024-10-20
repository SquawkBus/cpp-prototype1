#include <format>
#include <utility>

#include "logging/log.hpp"

#include "subscribe.hpp"
#include "interactor.hpp"

namespace logging = squawkbus::logging;

namespace squawkbus::topicbus::server
{
  using squawkbus::topicbus::messages::SubscriptionRequest;

  void SubscriptionManager::on_subscription(
    Interactor* subscriber,
    SubscriptionRequest* message)
  {
    logging::debug(
      std::format(
        "on_subscription: {} ({})",
        message->topic(),
        (message->is_add() ? "<true>" : "<false>")));

    if (message->is_add())
      add_subscription(subscriber, message->topic());
    else
      remove_subscription(subscriber, message->topic());
  }

  void SubscriptionManager::add_subscription(
    Interactor* subscriber,
    const std::string& topic)
  {
    logging::debug(std::format( "add_subscription: {}", topic));

    auto i_subscription = subscriptions_.find(topic);
    if (i_subscription == subscriptions_.end())
    {
      subscriptions_.insert( { { topic, { {subscriber, 1} } } });
      regex_cache_.insert({ { topic, std::regex(topic) } });
    }
    else
    {
      auto topic_subscribers = i_subscription->second;
      auto i_topic_subscriber = topic_subscribers.find(subscriber);
      if (i_topic_subscriber == topic_subscribers.end())
      {
        // new subscriber
        topic_subscribers.insert({ { subscriber, 1 } });
      }
      else
      {
        // increment the subscription count.
        i_topic_subscriber->second += 1;
      }
    }

    auto i_subscriber_topics = subscriber_topics_.find(subscriber);
    if (i_subscriber_topics == subscriber_topics_.end())
    {
      // A new subscriber.
      subscriber_topics_.insert({ { subscriber, { {topic} } } });
    }
    else
    {
      // Add this topic to the subscriber.
      i_subscriber_topics->second.insert(topic);
    }
  }

  void SubscriptionManager::remove_subscription(
    Interactor* subscriber,
    const std::string& topic)
  {
    logging::debug(std::format( "remove_subscription: {}", topic));

    auto i_subscription = subscriptions_.find(topic);
    if (i_subscription == subscriptions_.end())
    {
      return;
    }

    auto subscribers = i_subscription->second;
    auto i_subscriber = subscribers.find(subscriber);
    if (i_subscriber == subscribers.end())
    {
      return;
    }

    // decrement subscriber count.
    i_subscriber->second -= 1;
    if (i_subscriber->second > 0)
    {
      // The subscriber still monitors this topic?
      return;
    }

    // The subscriber doesn't monitor this topic so remove subscriber info.
    subscribers.erase(i_subscriber);
    if (subscribers.empty())
    {
      // the topic no longer has subscribers.
      subscriptions_.erase(topic);
      regex_cache_.erase(topic);
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

  void SubscriptionManager::on_interactor_closed(Interactor* subscriber)
  {
    auto i_subscriber_topics = subscriber_topics_.find(subscriber);
    if (i_subscriber_topics == subscriber_topics_.end())
    {
      return;
    }

    for (auto& topic : i_subscriber_topics->second)
    {
      auto i_subscriptions = subscriptions_.find(topic);
      if (i_subscriptions == subscriptions_.end())
      {
        continue;
      }

      i_subscriptions->second.erase(subscriber);
      if (!i_subscriptions->second.empty())
      {
        continue;
      }

      subscriptions_.erase(topic);
      regex_cache_.erase(topic);
    }

    subscriber_topics_.erase(subscriber);
  }

  std::set<Interactor*> SubscriptionManager::find_subscribers(const std::string& topic) const
  {
    auto subscribers = std::set<Interactor*> {};

    for (auto& [pattern, regex] : regex_cache_)
    {
      if (!std::regex_match(topic, regex))
      {
        continue;
      }

      auto i_subscribers = subscriptions_.find(pattern);
      if (i_subscribers == subscriptions_.end())
      {
        // should never happen.
        continue;
      }

      for (auto& [subscriber, count] : i_subscribers->second)
      {
        subscribers.insert(subscriber);
      }
    }

    return subscribers;
  }

}
