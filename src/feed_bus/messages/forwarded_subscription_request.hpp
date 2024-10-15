#ifndef SQUAWKBUS_FEED_BUS_MESSAGES_FORWARDED_SUBSCRIPTION_REQUEST_HPP
#define SQUAWKBUS_FEED_BUS_MESSAGES_FORWARDED_SUBSCRIPTION_REQUEST_HPP

#include <memory>
#include <sstream>
#include <string>

#include "serialization/frame_buffer.hpp"
#include "serialization/frame_buffer_io.hpp"

#include "feed_bus/messages/message_type.hpp"
#include "feed_bus/messages/message.hpp"

namespace squawkbus::feed_bus::messages
{
    struct ForwardedSubscriptionRequest : public Message
    {
        std::string user;
        std::string host;
        std::string client_id;
        std::string feed;
        std::string topic;
        bool is_add;

        ForwardedSubscriptionRequest()
            : Message(MessageType::ForwardedSubscriptionRequest)
        {
        }

        ForwardedSubscriptionRequest(
            const std::string &user,
            const std::string &host,
            const std::string &client_id,
            const std::string &feed,
            const std::string &topic,
            bool is_add)
            : Message(MessageType::ForwardedSubscriptionRequest),
            user(user),
            host(host),
            client_id(client_id),
            feed(feed),
            topic(topic),
            is_add(is_add)
        {
        }

        void write_body(serialization::FrameBuffer &frame) const override
        {
            frame
                << user
                << host
                << client_id
                << feed
                << topic
                << is_add;
        }

        void read_body(serialization::FrameBuffer &frame) override
        {
            frame
                >> user
                >> host
                >> client_id
                >> feed
                >> topic
                >> is_add;
        }

        bool equals(const std::shared_ptr<ForwardedSubscriptionRequest> &other) const
        {
            return
                message_type == other->message_type &&
                user == other->user &&
                host == other->host &&
                client_id == other->client_id &&
                feed == other->feed &&
                topic == other->topic &&
                is_add == other->is_add;
        }

        bool equals(const std::shared_ptr<Message> &other) const override
        {
            return equals(std::static_pointer_cast<ForwardedSubscriptionRequest>(other));
        }

        std::string to_string() const override
        {
            std::stringstream ss;
            ss
                << "ForwardedSubscriptionRequest("
                << "message_type=" << message_type
                << ", user=\"" << user << "\""
                << ", host=\"" << host << "\""
                << ", client_id=\"" << client_id << "\""
                << ", feed=\"" << feed << "\""
                << ", topic=\"" << topic << "\""
                << ", is_add=" << (is_add ? "<true>" : "<false>")
                << ")";
            return ss.str();
        }
    };
}

#endif // SQUAWKBUS_FEED_BUS_MESSAGES_FORWARDED_SUBSCRIPTION_REQUEST_HPP
