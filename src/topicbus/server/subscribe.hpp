#ifndef SQUAWKBUS_TOPIC_BUS_SERVER_SUBSCRIBE_HPP
#define SQUAWKBUS_TOPIC_BUS_SERVER_SUBSCRIBE_HPP

#include <memory>

#include "topicbus/messages/message.hpp"

namespace squawkbus::topicbus::server
{
  using squawkbus::topicbus::messages::SubscriptionRequest;

  class SubscriptionManager
  {
  public:
    void on_subscription(std::shared_ptr<SubscriptionRequest> message);
  };
}

#endif // SQUAWKBUS_TOPIC_BUS_SERVER_SUBSCRIBE_HPP
