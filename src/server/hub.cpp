#include <format>
#include <stdexcept>
#include <utility>

#include "logging/log.hpp"

#include "hub.hpp"
#include "interactor.hpp"

namespace squawkbus::server
{
  namespace logging = squawkbus::logging;

  using squawkbus::messages::Message;
  using squawkbus::messages::MessageType;
  using squawkbus::messages::SubscriptionRequest;
  using squawkbus::messages::NotificationRequest;
  using squawkbus::messages::UnicastData;
  using squawkbus::messages::MulticastData;

  void Hub::on_connected(Interactor* interactor)
  {
    logging::debug(std::format("adding interactor {}", interactor->id()));

    interactors_.insert({interactor->id(), interactor});
  }

  void Hub::on_disconnected(Interactor* interactor)
  {
    logging::debug(std::format("removing interactor {}", interactor->id()));

    subscription_manager_.on_interactor_closed(interactor);
    notification_manager_.on_interactor_closed(interactor);
    publisher_manager_.on_interactor_closed(interactor);

    interactors_.erase(interactor->id());
  }

  void Hub::on_message(Interactor* interactor, Message* message)
  {
    switch (message->message_type())
    {
    case MessageType::SubscriptionRequest:
      subscription_manager_.on_subscription(
        interactor,
        dynamic_cast<SubscriptionRequest*>(message),
        notification_manager_);
      return;

    case MessageType::NotificationRequest:
      notification_manager_.on_listen(
        interactor,
        dynamic_cast<NotificationRequest*>(message),
        subscription_manager_);
      return;

    case MessageType::UnicastData:
      publisher_manager_.on_send_unicast(
        interactor,
        dynamic_cast<UnicastData*>(message),
        interactors_
      );
      return;

    case MessageType::MulticastData:
      publisher_manager_.on_send_multicast(
        interactor,
        dynamic_cast<MulticastData*>(message),
        subscription_manager_
      );
      return;

    case MessageType::Authenticate:
    case MessageType::ForwardedSubscriptionRequest:
    case MessageType::ForwardedMulticastData:
    case MessageType::ForwardedUnicastData:
      throw std::runtime_error("unhandled message");
    }

    std::unreachable();
  }
}