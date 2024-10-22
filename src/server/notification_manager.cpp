#include <format>
#include <memory>

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

  using squawkbus::messages::NotificationRequest;
  using squawkbus::messages::ForwardedSubscriptionRequest;

  void NotificationManager::on_listen(Interactor* listener, const NotificationRequest& request, const SubscriptionManager& subscription_manager)
  {
    log.debug(
      std::format(
        "on_subscription: {} ({})",
        request.topic_pattern(),
        (request.is_add() ? "<true>" : "<false>")));

    if (request.is_add())
      add_listener(listener, request.topic_pattern(), subscription_manager);
    else
      remove_listener(listener, request.topic_pattern(), subscription_manager);
  }

  void NotificationManager::add_listener(Interactor* listener, const std::string& topic_pattern, const SubscriptionManager& subscription_manager)
  {
    log.debug(std::format( "add_notification: {}", topic_pattern));

    // Try to find the topic pattern.
    auto i_notifications = notifications_.find(topic_pattern);
    if (i_notifications == notifications_.end())
    {
      // New topic pattern.
      notifications_.insert( { {topic_pattern, { {listener, 1} }} });
      regex_cache_.insert({ {topic_pattern, std::regex(topic_pattern)} });
    }
    else
    {
      // Existing topic pattern.
      auto listeners = i_notifications->second;
      auto i_listener = listeners.find(listener);
      if (i_listener == listeners.end())
      {
        // new listener for subscriptions to a topic pattern.
        listeners.insert({ {listener,1 } });
      }
      else
      {
        // increment the notification count.
        i_listener->second += 1;
      }
    }

    // Handle the reverse lookup.
    auto i_listener_topic_patterns = listener_topic_patterns_.find(listener);
    if (i_listener_topic_patterns == listener_topic_patterns_.end())
    {
      // A new listener.
      listener_topic_patterns_.insert({ {listener, { {topic_pattern} }} });
    }
    else
    {
      // Add this topic to the listener.
      i_listener_topic_patterns->second.insert(topic_pattern);
    }

    // Notify listener of existing subscriptions.
    auto i_regex = regex_cache_.find(topic_pattern);
    if (i_regex == regex_cache_.end())
    {
      // Should never happen.
      return;
    }
    auto matching_subscriptions = subscription_manager.find_matching_subscriptions(i_regex->second);
    for (auto& [topic_pattern, subscribers] : matching_subscriptions)
    {
      for (auto subscriber : subscribers)
      {
        auto message = std::make_shared<ForwardedSubscriptionRequest>(
          subscriber->user(),
          subscriber->host(),
          subscriber->id(),
          topic_pattern,
          true);

        listener->send(message);
      }
    }

  }

  void NotificationManager::remove_listener(Interactor* listener, const std::string& topic_pattern, const SubscriptionManager& subscription_manager)
  {
    log.debug(std::format( "remove_notification: {}", topic_pattern));

    auto i_notifications = notifications_.find(topic_pattern);
    if (i_notifications == notifications_.end())
    {
      // The pattern has no listeners.
      return;
    }

    auto listeners = i_notifications->second;
    auto i_listener = listeners.find(listener);
    if (i_listener == listeners.end())
    {
      return;
    }

    // Decrement notification count.
    i_listener->second -= 1;
    if (i_listener->second > 0)
    {
      // The listener still monitors this topic.
      return;
    }

    // The subscriber no longer monitors this topic so remove it.
    listeners.erase(i_listener);
    if (listeners.empty())
    {
      // The topic pattern no longer has listeners.
      notifications_.erase(topic_pattern);
      regex_cache_.erase(topic_pattern);
    }

    auto i_listener_topic_patterns = listener_topic_patterns_.find(listener);
    if (i_listener_topic_patterns == listener_topic_patterns_.end())
    {
      // This would be a program logic error.
      return;
    }

    // Remove the topic pattern from the listener.
    i_listener_topic_patterns->second.erase(topic_pattern);
    if (i_listener_topic_patterns->second.empty())
    {
      // Remove a listener with no notifications.
      listener_topic_patterns_.erase(listener);
    }
  }

  void NotificationManager::on_interactor_closed(Interactor* listener)
  {
    auto i_listener_topic_patterns = listener_topic_patterns_.find(listener);
    if (i_listener_topic_patterns == listener_topic_patterns_.end())
    {
      // The listener has no notifications.
      return;
    }

    // Go through each of the notifications removing the listener.
    for (auto& topic_pattern : i_listener_topic_patterns->second)
    {
      auto i_notifications = notifications_.find(topic_pattern);
      if (i_notifications == notifications_.end())
      {
        // This should never happen, and would be a logic error.
        continue;
      }

      i_notifications->second.erase(listener);
      if (!i_notifications->second.empty())
      {
        // The topic pattern has other listeners.
        continue;
      }

      notifications_.erase(topic_pattern);
      regex_cache_.erase(topic_pattern);
    }

    listener_topic_patterns_.erase(listener);
  }

  void NotificationManager::notify(
    Interactor* subscriber,
    const std::string& topic_pattern,
    bool is_add) const
  {
    auto listeners = find_listeners(topic_pattern);
    if (listeners.empty())
    {
      return;
    }

    auto message = std::make_shared<ForwardedSubscriptionRequest>(
      subscriber->user(),
      subscriber->host(),
      subscriber->id(),
      topic_pattern,
      is_add);

    for (auto listener : listeners)
    {
      log.debug(
        std::format(
          "notifying subscription from {} on \"{}\" to {}",
          subscriber->str(),
          topic_pattern,
          listener->str()));
          
      listener->send(message);
    }
  }

  std::set<Interactor*> NotificationManager::find_listeners(const std::string& topic) const
  {
    auto listeners = std::set<Interactor*> {};

    // Go through each of the topic patterns checking for a match.
    for (auto& [topic_pattern, regex] : regex_cache_)
    {
      if (!std::regex_match(topic, regex))
      {
        // No match.
        continue;
      }

      auto i_notifications = notifications_.find(topic_pattern);
      if (i_notifications == notifications_.end())
      {
        // A match is found with no listeners.
        // Should never happen.
        continue;
      }

      // Add the interactors that are listening to this topic pattern.
      for (auto& [subscriber, count] : i_notifications->second)
      {
        listeners.insert(subscriber);
      }
    }

    return listeners;
  }

}
