#include "hub.hpp"

#include <stdexcept>
#include <utility>

#include "interactor.hpp"

namespace squawkbus::topicbus::server
{
  using squawkbus::topicbus::messages::Message;
  using squawkbus::topicbus::messages::MessageType;
  using squawkbus::topicbus::messages::SubscriptionRequest;

  void Hub::on_message(Interactor* interactor, Message* message)
  {
    switch (message->message_type())
    {
    case MessageType::SubscriptionRequest:
      subscription_manager_.on_subscription(
        interactor,
        dynamic_cast<SubscriptionRequest*>(message));
      return;
      
    case MessageType::Authenticate:
    case MessageType::MulticastData:
    case MessageType::UnicastData:
    case MessageType::ForwardedSubscriptionRequest:
    case MessageType::NotificationRequest:
    case MessageType::ForwardedMulticastData:
    case MessageType::ForwardedUnicastData:
      throw std::runtime_error("unhandled message");
    }

    std::unreachable();
  }
}