#include <format>

#include "logging/log.hpp"

#include "subscribe.hpp"
#include "interactor.hpp"

namespace logging = squawkbus::logging;

namespace squawkbus::topicbus::server
{
  using squawkbus::topicbus::messages::SubscriptionRequest;

  void SubscriptionManager::on_subscription(
    Interactor* interactor,
    SubscriptionRequest* message)
  {
    logging::debug(
      std::format(
        "on_subscription: {} ({})",
        message->topic(),
        (message->is_add() ? "<true>" : "<false>")));

    if (message->is_add())
      add_subscription(interactor, message->topic());
    else
      remove_subscription(interactor, message->topic());
  }

  void SubscriptionManager::add_subscription(
    Interactor* interactor,
    const std::string& topic)
  {
    logging::debug(std::format( "add_subscription: {}", topic));
  }

  void SubscriptionManager::remove_subscription(
    Interactor* interactor,
    const std::string& topic)
  {
    logging::debug(std::format( "add_subscription: {}", topic));
  }
}
