#include <format>
#include <memory>

#include "logging/log.hpp"

#include "interactor.hpp"
#include "notification_manager.hpp"

namespace logging = squawkbus::logging;

namespace squawkbus::server
{
  using squawkbus::messages::NotificationRequest;
  using squawkbus::messages::ForwardedSubscriptionRequest;

  void NotificationManager::on_listen(Interactor* listener, NotificationRequest* message)
  {
    logging::debug(
      std::format(
        "on_subscription: {} ({})",
        message->topic_pattern(),
        (message->is_add() ? "<true>" : "<false>")));

    if (message->is_add())
      add_listener(listener, message->topic_pattern());
    else
      remove_listener(listener, message->topic_pattern());
  }

  void NotificationManager::add_listener(Interactor* listener, const std::string& topic_pattern)
  {
    repository_.add_listener(listener, topic_pattern);

  }

  void NotificationManager::remove_listener(Interactor* listener, const std::string& topic_pattern)
  {
    repository_.remove_listener(listener, topic_pattern);
  }

  void NotificationManager::on_interactor_closed(Interactor* listener)
  {
    repository_.remove_interactor(listener);
  }

  void NotificationManager::notify(
    Interactor* subscriber,
    const std::string& topic_pattern,
    bool is_add)
  {
    logging::debug(std::format( "notify: {}", topic_pattern));

    auto message = std::make_shared<ForwardedSubscriptionRequest>(
      subscriber->user(),
      subscriber->host(),
      subscriber->id(),
      topic_pattern,
      is_add);

    auto listeners = repository_.find_listeners(topic_pattern);
    for (auto listener : listeners)
    {
      listener->send(message);
    }
  }
}
