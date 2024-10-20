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

    auto i_interator_subscription = interactor_subscriptions_.find(subscriber);
    if (i_interator_subscription == interactor_subscriptions_.end())
    {
      // new interactor subscription.
      interactor_subscriptions_.insert({ { subscriber, { {topic} } } });
    }
    else
    {
      // add this topic.
      i_interator_subscription->second.insert(topic);
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

    auto i_interactor_subscription = interactor_subscriptions_.find(subscriber);
    if (i_interactor_subscription == interactor_subscriptions_.end())
    {
      return;
    }
    
    i_interactor_subscription->second.erase(topic);
    if (i_interactor_subscription->second.empty())
    {
      // remove a subscriber with no subscriptions.
      interactor_subscriptions_.erase(subscriber);
    }
  }
}
