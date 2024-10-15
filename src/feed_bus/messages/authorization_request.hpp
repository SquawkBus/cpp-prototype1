#ifndef SQUAWKBUS_FEED_BUS_MESSAGES_AUTHORIZATION_REQUEST_HPP
#define SQUAWKBUS_FEED_BUS_MESSAGES_AUTHORIZATION_REQUEST_HPP

#include <memory>
#include <sstream>
#include <string>

#include "serialization/frame_buffer.hpp"
#include "serialization/frame_buffer_io.hpp"

#include "feed_bus/messages/message_type.hpp"
#include "feed_bus/messages/message.hpp"

namespace squawkbus::feed_bus::messages
{
    struct AuthorizationRequest : public Message
    {
        std::string client_id;
        std::string host;
        std::string user;
        std::string feed;
        std::string topic;

        AuthorizationRequest()
            :   Message(MessageType::AuthorizationRequest)
        {
        }

        AuthorizationRequest(
            const std::string &client_id,
            const std::string &host,
            const std::string &user,
            const std::string &feed,
            const std::string &topic)
            :   Message(MessageType::AuthorizationRequest),
                client_id(client_id),
                host(host),
                user(user),
                feed(feed),
                topic(topic)
        {
        }

        void write_body(serialization::FrameBuffer &frame) const override
        {
            frame
                << client_id
                << host
                << user
                << feed
                << topic;
        }

        void read_body(serialization::FrameBuffer &frame) override
        {
            frame
                >> client_id
                >> host
                >> user
                >> feed
                >> topic;
        }

        bool equals(const std::shared_ptr<AuthorizationRequest> &other) const
        {
            return
                message_type == other->message_type &&
                client_id == other->client_id &&
                host == other->host &&
                user == other->user &&
                feed == other->feed &&
                topic == other->topic;
        }

        bool equals(const std::shared_ptr<Message>& other) const override
        {
            return equals(std::static_pointer_cast<AuthorizationRequest>(other));
        }

        std::string to_string() const override
        {
            std::stringstream ss;
            ss
                    << "AuthorizationRequest("
                    << "message_type=" << message_type
                    << ", client_id=\"" << client_id << "\""
                    << ", host=\"" << host << "\""
                    << ", user=\"" << user << "\""
                    << ", feed=\"" << feed << "\""
                    << ", topic=\"" << topic << "\""
                    << ")";
            return ss.str();
        }
    };

}

#endif // SQUAWKBUS_FEED_BUS_MESSAGES_AUTHORIZATION_REQUEST_HPP
