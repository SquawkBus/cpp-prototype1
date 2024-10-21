#ifndef SQUAWKBUS_SERVER_HUB_HPP
#define SQUAWKBUS_SERVER_HUB_HPP

#include <map>
#include <memory>
#include <string>

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
    std::map<std::string, Interactor*> interactors_;
    PublisherManager publisher_manager_;
    SubscriptionManager subscription_manager_;
    NotificationManager notification_manager_;

  public:
    void on_message(Interactor* interactor, const Message* message);
    void on_connected(Interactor* interactor);
    void on_disconnected(Interactor* interactor);
  };
}

#endif // SQUAWKBUS_SERVER_HUB_HPP
