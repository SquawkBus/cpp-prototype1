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
      repository_.add_subscription(subscriber, message->topic_pattern());
    else
      repository_.remove_subscription(subscriber, message->topic_pattern());

    notification_manager.notify(subscriber, message->topic_pattern(), message->is_add());
  }

  void SubscriptionManager::on_interactor_closed(Interactor* subscriber)
  {
    repository_.remove_interactor(subscriber);
  }

  std::vector<std::pair<std::string, std::vector<Interactor*>>> SubscriptionManager::find_matching_subscriptions(const std::regex& regex) const
  {
    return repository_.find_matching_subscriptions(regex);

  }
}
