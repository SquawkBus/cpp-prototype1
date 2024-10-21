#ifndef SQUAWKBUS_TOPICBUS_SERVER_HUB_HPP
#define SQUAWKBUS_TOPICBUS_SERVER_HUB_HPP

#include <memory>

#include "topicbus/messages/message.hpp"

#include "publish.hpp"
#include "subscription_manager.hpp"
#include "notify.hpp"

namespace squawkbus::topicbus::server
{
  using squawkbus::topicbus::messages::Message;

  class Interactor;

  class Hub
  {
  private:
    PublisherManager publisher_manager_;
    SubscriptionManager subscription_manager_;
    NotificationManager notification_manager_;

  public:
    void on_message(Interactor* interactor, Message* message);
  };
}

#endif // SQUAWKBUS_TOPICBUS_SERVER_HUB_HPP
