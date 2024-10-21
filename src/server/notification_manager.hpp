#ifndef SQUAWKBUS_SERVER_NOTIFICATION_MANAGER_HPP
#define SQUAWKBUS_SERVER_NOTIFICATION_MANAGER_HPP

#include <string>
namespace squawkbus::server
{
  class Interactor;

  class NotificationManager
  {
  public:
    void notify(Interactor* subscriber, const std::string& topic_pattern, bool is_add);
  };
}

#endif // SQUAWKBUS_SERVER_NOTIFICATION_MANAGER_HPP
