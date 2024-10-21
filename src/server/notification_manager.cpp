#include <format>

#include "logging/log.hpp"

#include "interactor.hpp"
#include "notification_manager.hpp"

namespace logging = squawkbus::logging;

namespace squawkbus::server
{
  void NotificationManager::notify(Interactor* subscriber, const std::string& topic_pattern, bool is_add)
  {
    logging::debug(std::format( "notify: {}", topic_pattern));
  }
}
