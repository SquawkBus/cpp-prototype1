#ifndef SQUAWKBUS_FEEDBUS_MESSAGES_AUTHORIZATION_REQUEST_HPP
#define SQUAWKBUS_FEEDBUS_MESSAGES_AUTHORIZATION_REQUEST_HPP

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

        void write_body(FrameBuffer &frame) const override
        {
            frame
                << client_id
                << host
                << user
                << feed
                << topic;
        }

        void read_body(FrameBuffer &frame) override
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
            return std::format(
                "message_type={},client_id=\"{}\",host=\"{}\",user=\"{}\",feed=\"{}\",topic=\"{}\")",
                messages::to_string(message_type),
                client_id,
                host,
                user,
                feed,
                topic
            );
        }
    };
}

#endif // SQUAWKBUS_FEEDBUS_MESSAGES_AUTHORIZATION_REQUEST_HPP
