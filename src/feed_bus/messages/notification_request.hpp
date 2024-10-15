#ifndef SQUAWKBUS_FEED_BUS_MESSAGES_NOTIFICATION_REQUEST_HPP
#define SQUAWKBUS_FEED_BUS_MESSAGES_NOTIFICATION_REQUEST_HPP

#include <memory>
#include <sstream>
#include <string>

#include "serialization/frame_buffer.hpp"
#include "serialization/frame_buffer_io.hpp"

#include "feed_bus/messages/message_type.hpp"
#include "feed_bus/messages/message.hpp"

namespace squawkbus::feed_bus::messages
{
    struct NotificationRequest : public Message
    {
        std::string feed;
        bool is_add;

        NotificationRequest()
            : Message(MessageType::NotificationRequest)
        {
        }

        NotificationRequest(
            const std::string &feed,
            bool is_add)
            : Message(MessageType::NotificationRequest),
            feed(feed),
            is_add(is_add)
        {
        }

        void write_body(serialization::FrameBuffer &frame) const override
        {
            frame
                << feed
                << is_add;
        }

        void read_body(serialization::FrameBuffer &frame) override
        {
            frame
                >> feed
                >> is_add;
        }

        bool equals(const std::shared_ptr<NotificationRequest> &other) const
        {
            return
                message_type == other->message_type &&
                feed == other->feed &&
                is_add == other->is_add;
        }

        bool equals(const std::shared_ptr<Message> &other) const override
        {
            return equals(std::static_pointer_cast<NotificationRequest>(other));
        }

        std::string to_string() const override
        {
            std::stringstream ss;
            ss
                << "NotificationRequest("
                << "message_type=" << message_type
                << ", feed=\"" << feed << "\""
                << ", is_add=" << (is_add ? "<true>" : "<false>")
                << ")";
            return ss.str();
        }
    };
}

#endif // SQUAWKBUS_FEED_BUS_MESSAGES_NOTIFICATION_REQUEST_HPP
