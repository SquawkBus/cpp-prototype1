#ifndef SQUAWKBUS_SERVER_HUB_HPP
#define SQUAWKBUS_SERVER_HUB_HPP

#include <memory>

#include "messages/messages.hpp"

#include "publisher_manager.hpp"
#include "subscription_manager.hpp"
#include "notification_manager.hpp"

namespace squawkbus::server
{
  using squawkbus::messages::Message;

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

#endif // SQUAWKBUS_SERVER_HUB_HPP
