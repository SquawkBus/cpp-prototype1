#ifndef SQUAWKBUS_FEEDBUS_MESSAGES_SUBSCRIPTION_REQUEST_HPP
#define SQUAWKBUS_FEEDBUS_MESSAGES_SUBSCRIPTION_REQUEST_HPP

#include <format>
#include <memory>
#include <string>

#include "serialization/frame_buffer.hpp"
#include "serialization/frame_buffer_io.hpp"

#include "feedbus/messages/message_type.hpp"
#include "feedbus/messages/message.hpp"

namespace squawkbus::feedbus::messages
{
    using serialization::FrameBuffer;

    struct SubscriptionRequest : public Message
    {
        std::string feed;
        std::string topic;
        bool is_add;

        SubscriptionRequest()
            : Message(MessageType::SubscriptionRequest)
        {
        }

        SubscriptionRequest(
            const std::string &feed,
            const std::string &topic,
            bool is_add)
            : Message(MessageType::SubscriptionRequest),
            feed(feed),
            topic(topic),
            is_add(is_add)
        {
        }

        bool equals(const std::shared_ptr<SubscriptionRequest> &other) const
        {
            return
                message_type == other->message_type &&
                feed == other->feed &&
                topic == other->topic &&
                is_add == other->is_add;
        }

        bool equals(const std::shared_ptr<Message> &other) const override
        {
            return equals(std::static_pointer_cast<SubscriptionRequest>(other));
        }

        std::string str() const override
        {
            return std::format(
                "SubscriptionRequest(message_type={},feed=\"{}\",topic=\"{}\",is_add={})",
                messages::to_string(message_type),
                feed,
                topic,
                (is_add ? "<true>" : "<false")
            );
        }
    protected:

        void serialize_body(FrameBuffer &frame) const override
        {
            frame
                << feed
                << topic
                << is_add;
        }

        void deserialize_body(FrameBuffer &frame) override
        {
            frame
                >> feed
                >> topic
                >> is_add;
        }

    };
}

#endif // SQUAWKBUS_FEEDBUS_MESSAGES_SUBSCRIPTION_REQUEST_HPP
