#include <format>

#include "logging/log.hpp"

#include "interactor.hpp"
#include "notification_manager.hpp"

namespace logging = squawkbus::logging;

namespace squawkbus::server
{
  void NotificationManager::on_listen(Interactor* listener, NotificationRequest* message)
  {
    logging::debug(
      std::format(
        "on_subscription: {} ({})",
        message->topic_pattern(),
        (message->is_add() ? "<true>" : "<false>")));

    if (message->is_add())
      repository_.add_listener(listener, message->topic_pattern());
    else
      repository_.remove_listener(listener, message->topic_pattern());
  }

  void NotificationManager::on_interactor_closed(Interactor* listener)
  {
    repository_.remove_interactor(listener);
  }

  void NotificationManager::notify(Interactor* subscriber, const std::string& topic_pattern, bool is_add)
  {
    logging::debug(std::format( "notify: {}", topic_pattern));
  }
}
