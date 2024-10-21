#include <format>
#include <utility>

#include "logging/log.hpp"

#include "interactor.hpp"
#include "notify.hpp"
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
      add_subscription(subscriber, message->topic_pattern(), notification_manager);
    else
      remove_subscription(subscriber, message->topic_pattern(), notification_manager);
  }

  void SubscriptionManager::add_subscription(
    Interactor* subscriber,
    const std::string& topic_pattern,
    NotificationManager& notification_manager)
  {
    logging::debug(std::format( "add_subscription: {}", topic_pattern));

    repository_.add_subscription(subscriber, topic_pattern);
  }

  void SubscriptionManager::remove_subscription(
    Interactor* subscriber,
    const std::string& topic_pattern,
    NotificationManager& notification_manager)
  {
    logging::debug(std::format( "remove_subscription: {}", topic_pattern));

    repository_.remove_subscription(subscriber, topic_pattern);
  }

  void SubscriptionManager::on_interactor_closed(Interactor* subscriber)
  {
    repository_.remove_interactor(subscriber);
  }
}
