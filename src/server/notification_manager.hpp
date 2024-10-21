#ifndef SQUAWKBUS_SERVER_NOTIFICATION_MANAGER_HPP
#define SQUAWKBUS_SERVER_NOTIFICATION_MANAGER_HPP

#include <string>

#include "messages/messages.hpp"

#include "notification_repository.hpp"

namespace squawkbus::server
{
  using squawkbus::messages::NotificationRequest;

  class Interactor;

  class NotificationManager
  {
  private:
    NotificationRepository repository_;

  public:
    void on_listen(Interactor* listener, NotificationRequest* message);
    void on_interactor_closed(Interactor* listener);
    void notify(Interactor* subscriber, const std::string& topic_pattern, bool is_add);

  private:
    void add_listener(Interactor* listener, const std::string& topic_pattern);
    void remove_listener(Interactor* listener, const std::string& topic_pattern);
  };
}

#endif // SQUAWKBUS_SERVER_NOTIFICATION_MANAGER_HPP
