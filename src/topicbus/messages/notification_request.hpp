#ifndef SQUAWKBUS_TOPICBUS_MESSAGES_NOTIFICATION_REQUEST_HPP
#define SQUAWKBUS_TOPICBUS_MESSAGES_NOTIFICATION_REQUEST_HPP

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

    struct NotificationRequest : public Message
    {
        std::string topic;
        bool is_add;

        NotificationRequest()
            : Message(MessageType::NotificationRequest)
        {
        }

        NotificationRequest(
            const std::string &topic,
            bool is_add)
            : Message(MessageType::NotificationRequest),
            topic(topic),
            is_add(is_add)
        {
        }

        bool equals(const std::shared_ptr<NotificationRequest> &other) const
        {
            return
                message_type == other->message_type &&
                topic == other->topic &&
                is_add == other->is_add;
        }

        bool equals(const std::shared_ptr<Message> &other) const override
        {
            return equals(std::static_pointer_cast<NotificationRequest>(other));
        }

        std::string str() const override
        {
            return std::format(
                "NotificationRequest(message_type={},topic=\"{}\",is_add={})",
                messages::to_string(message_type),
                topic,
                (is_add ? "<true>" : "<false>")
            );
        }
    protected:

        void serialize_body(FrameBuffer &frame) const override
        {
            frame
                << topic
                << is_add;
        }

        void deserialize_body(FrameBuffer &frame) override
        {
            frame
                >> topic
                >> is_add;
        }
    };
}

#endif // SQUAWKBUS_TOPICBUS_MESSAGES_NOTIFICATION_REQUEST_HPP
