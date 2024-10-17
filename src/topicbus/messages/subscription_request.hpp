#ifndef SQUAWKBUS_TOPICBUS_MESSAGES_SUBSCRIPTION_REQUEST_HPP
#define SQUAWKBUS_TOPICBUS_MESSAGES_SUBSCRIPTION_REQUEST_HPP

#include <format>
#include <memory>
#include <string>

#include "serialization/frame_buffer.hpp"
#include "serialization/frame_buffer_io.hpp"

#include "topicbus/messages/message_type.hpp"
#include "topicbus/messages/message.hpp"

namespace squawkbus::topicbus::messages
{
    using serialization::FrameBuffer;

    struct SubscriptionRequest : public Message
    {
        std::string topic;
        bool is_add;

        SubscriptionRequest()
            : Message(MessageType::SubscriptionRequest)
        {
        }

        SubscriptionRequest(
            const std::string &topic,
            bool is_add)
            : Message(MessageType::SubscriptionRequest),
            topic(topic),
            is_add(is_add)
        {
        }

        void write_body(FrameBuffer &frame) const override
        {
            frame
                << topic
                << is_add;
        }

        void read_body(FrameBuffer &frame) override
        {
            frame
                >> topic
                >> is_add;
        }

        bool equals(const std::shared_ptr<SubscriptionRequest> &other) const
        {
            return
                message_type == other->message_type &&
                topic == other->topic &&
                is_add == other->is_add;
        }

        bool equals(const std::shared_ptr<Message> &other) const override
        {
            return equals(std::static_pointer_cast<SubscriptionRequest>(other));
        }

        std::string to_string() const override
        {
            return std::format(
                "SubscriptionRequest(message_type={},topic=\"{}\",is_add={})",
                messages::to_string(message_type),
                topic,
                (is_add ? "<true>" : "<false")
            );
        }
    };
}

#endif // SQUAWKBUS_TOPICBUS_MESSAGES_SUBSCRIPTION_REQUEST_HPP
