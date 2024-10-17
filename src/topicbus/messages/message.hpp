#ifndef SQUAWKBUS_TOPICBUS_MESSAGES_MESSAGE_HPP
#define SQUAWKBUS_TOPICBUS_MESSAGES_MESSAGE_HPP

#include <memory>
#include <string>
#include <utility>

#include "topicbus/messages/message_type.hpp"

namespace squawkbus::topicbus::messages
{
    struct Message
    {
        MessageType message_type;

        Message(MessageType message_type)
            :   message_type(message_type)
        {
        }

        void write(serialization::FrameBuffer& frame) const
        {
            frame << message_type;
            write_body(frame);
        }
        virtual void write_body(serialization::FrameBuffer& frame) const = 0;
        virtual bool equals(const std::shared_ptr<Message>& other) const = 0;
        virtual std::string to_string() const = 0;

        static std::shared_ptr<Message> read(serialization::FrameBuffer& frame)
        {
            MessageType message_type;
            frame >> message_type;
            auto message = make_shared(message_type);
            message->read_body(frame);
            return message;
        }
        static std::shared_ptr<Message> make_shared(MessageType message_type);
        virtual void read_body(serialization::FrameBuffer& frame) = 0;
    };
}

#include "topicbus/messages/authenticate.hpp"
#include "topicbus/messages/forwarded_multicast_data.hpp"
#include "topicbus/messages/forwarded_subscription_request.hpp"
#include "topicbus/messages/forwarded_unicast_data.hpp"
#include "topicbus/messages/multicast_data.hpp"
#include "topicbus/messages/notification_request.hpp"
#include "topicbus/messages/subscription_request.hpp"
#include "topicbus/messages/unicast_data.hpp"

namespace squawkbus::topicbus::messages
{
    std::shared_ptr<Message> Message::make_shared(MessageType message_type)
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

    bool operator == (const std::shared_ptr<Message>& lhs, const std::shared_ptr<Message>& rhs)
    {
        return lhs->equals(rhs);
    }

    std::ostream& operator << (std::ostream& os, const std::shared_ptr<Message>& message)
    {
        return os << message->to_string();
    }
}

#endif // SQUAWKBUS_TOPICBUS_MESSAGES_MESSAGE_HPP
