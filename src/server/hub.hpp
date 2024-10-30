#ifndef SQUAWKBUS_SERVER_HUB_HPP
#define SQUAWKBUS_SERVER_HUB_HPP

#include <map>
#include <memory>
#include <string>

#include "messages/messages.hpp"

#include "publisher_manager.hpp"
#include "subscription_manager.hpp"
#include "notification_manager.hpp"
#include "authorization.hpp"

namespace squawkbus::server
{
  using squawkbus::messages::Message;

  class Interactor;

  class Hub
  {
  private:
    std::optional<std::filesystem::path> authorization_file_;
    std::vector<AuthorizationSpec> cmd_line_authorizations_;

    std::map<std::string, Interactor*> interactors_;
    PublisherManager publisher_manager_;
    SubscriptionManager subscription_manager_;
    NotificationManager notification_manager_;
    AuthorizationManager authorization_manager_;

  public:
    Hub(std::optional<std::filesystem::path> authorization_file, std::vector<AuthorizationSpec> cmd_line_authorizations)
      : authorization_file_(authorization_file),
        cmd_line_authorizations_(cmd_line_authorizations)
    {
    }

    void on_startup();
    void on_message(Interactor* interactor, const Message* message);
    void on_connected(Interactor* interactor);
    void on_disconnected(Interactor* interactor);
  };
}

#endif // SQUAWKBUS_SERVER_HUB_HPP
