#include "hub.hpp"

#include <stdexcept>
#include <utility>

#include "interactor.hpp"

namespace squawkbus::server
{
  using squawkbus::messages::Message;
  using squawkbus::messages::MessageType;
  using squawkbus::messages::SubscriptionRequest;
  using squawkbus::messages::NotificationRequest;

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

    case MessageType::Authenticate:
    case MessageType::MulticastData:
    case MessageType::UnicastData:
    case MessageType::ForwardedSubscriptionRequest:
    case MessageType::ForwardedMulticastData:
    case MessageType::ForwardedUnicastData:
      throw std::runtime_error("unhandled message");
    }

    std::unreachable();
  }
}