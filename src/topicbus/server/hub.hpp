#ifndef SQUAWKBUS_TOPICBUS_SERVER_HUB_HPP
#define SQUAWKBUS_TOPICBUS_SERVER_HUB_HPP

#include <memory>

#include "topicbus/messages/message.hpp"

#include "subscribe.hpp"

namespace squawkbus::topicbus::server
{
  using squawkbus::topicbus::messages::Message;

  class Hub
  {
  private:
    SubscriptionManager subscription_manager_;

  public:
    void on_message(std::shared_ptr<Message> message);
  };
}

#endif // SQUAWKBUS_TOPICBUS_SERVER_HUB_HPP
