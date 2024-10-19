#include "subscribe.hpp"

#include <format>

#include "logging/log.hpp"

namespace logging = squawkbus::logging;

namespace squawkbus::topicbus::server
{
  using squawkbus::topicbus::messages::SubscriptionRequest;

  void SubscriptionManager::on_subscription(std::shared_ptr<SubscriptionRequest> message)
  {
    logging::debug(
      std::format(
        "on_subscription: {} ({})",
        message->topic(),
        (message->is_add() ? "<true>" : "<false>")));
  }
}
