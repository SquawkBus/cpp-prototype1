#ifndef SQUAWKBUS_MESSAGES_MESSAGE_INLINE_HPP
#define SQUAWKBUS_MESSAGES_MESSAGE_INLINE_HPP

#include <memory>
#include <string>
#include <utility>

#include "messages/_data_packet.hpp"
#include "messages/_message.hpp"
#include "messages/_authenticate.hpp"
#include "messages/_forwarded_multicast_data.hpp"
#include "messages/_forwarded_subscription_request.hpp"
#include "messages/_forwarded_unicast_data.hpp"
#include "messages/_multicast_data.hpp"
#include "messages/_notification_request.hpp"
#include "messages/_subscription_request.hpp"
#include "messages/_unicast_data.hpp"

namespace squawkbus::messages
{
  inline std::shared_ptr<Message> Message::make_shared(MessageType message_type)
  {
    switch (message_type)
    {
    case MessageType::Authenticate:
      return std::make_shared<Authenticate>();

    case MessageType::MulticastData:
      return std::make_shared<MulticastData>();

    case MessageType::UnicastData:
      return std::make_shared<UnicastData>();

    case MessageType::ForwardedSubscriptionRequest:
      return std::make_shared<ForwardedSubscriptionRequest>();

    case MessageType::NotificationRequest:
      return std::make_shared<NotificationRequest>();

    case MessageType::SubscriptionRequest:
      return std::make_shared<SubscriptionRequest>();

    case MessageType::ForwardedMulticastData:
      return std::make_shared<ForwardedMulticastData>();

    case MessageType::ForwardedUnicastData:
      return std::make_shared<ForwardedUnicastData>();
            
    default:
      throw "invalid";
    }
  }

  inline bool operator == (const std::shared_ptr<Message>& lhs, const std::shared_ptr<Message>& rhs)
  {
    return lhs->equals(rhs.get());
  }

  inline std::ostream& operator << (std::ostream& os, const std::shared_ptr<Message>& message)
  {
    return os << message->str();
  }
}

#endif // SQUAWKBUS_MESSAGES_MESSAGE_INLINE_HPP
