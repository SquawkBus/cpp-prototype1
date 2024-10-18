#ifndef SQUAWKBUS_FEEDBUS_MESSAGES_MESSAGE_HPP
#define SQUAWKBUS_FEEDBUS_MESSAGES_MESSAGE_HPP

#include <memory>
#include <string>
#include <utility>

#include "feedbus/messages/message_type.hpp"

namespace squawkbus::feedbus::messages
{
  struct Message
  {
    MessageType message_type;

    Message(MessageType message_type)
      : message_type(message_type)
    {
    }
    virtual ~Message()
    {
    }

    serialization::FrameBuffer serialize() const
    {
      serialization::FrameBuffer frame;
      frame << message_type;
      serialize_body(frame);
      return frame;
    }
    virtual bool equals(const std::shared_ptr<Message>& other) const = 0;
    virtual std::string str() const = 0;

    static std::shared_ptr<Message> deserialize(serialization::FrameBuffer& frame)
    {
      MessageType message_type;
      frame >> message_type;
      auto message = make_shared(message_type);
      message->deserialize_body(frame);
      return message;
    }
    static std::shared_ptr<Message> make_shared(MessageType message_type);

  protected:
    virtual void serialize_body(serialization::FrameBuffer& frame) const = 0;
    virtual void deserialize_body(serialization::FrameBuffer& frame) = 0;
  };
}

#include "feedbus/messages/authorization_request.hpp"
#include "feedbus/messages/authorization_response.hpp"
#include "feedbus/messages/forwarded_multicast_data.hpp"
#include "feedbus/messages/forwarded_subscription_request.hpp"
#include "feedbus/messages/forwarded_unicast_data.hpp"
#include "feedbus/messages/multicast_data.hpp"
#include "feedbus/messages/notification_request.hpp"
#include "feedbus/messages/subscription_request.hpp"
#include "feedbus/messages/unicast_data.hpp"

namespace squawkbus::feedbus::messages
{
  inline std::shared_ptr<Message> Message::make_shared(MessageType message_type)
  {
    switch (message_type)
    {
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

    case MessageType::AuthorizationRequest:
      return std::make_shared<AuthorizationRequest>();

    case MessageType::AuthorizationResponse:
      return std::make_shared<AuthorizationResponse>();

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
    return lhs->equals(rhs);
  }

  inline std::ostream& operator << (std::ostream& os, const std::shared_ptr<Message>& message)
  {
    return os << message->str();
  }
}

#endif // SQUAWKBUS_FEEDBUS_MESSAGES_MESSAGE_HPP
