#ifndef SQUAWKBUS_FEEDBUS_MESSAGES_AUTHORIZATION_RESPONSE_HPP
#define SQUAWKBUS_FEEDBUS_MESSAGES_AUTHORIZATION_RESPONSE_HPP

#include <cstdint>
#include <format>
#include <memory>
#include <set>
#include <string>

#include "utils/utils.hpp"

#include "serialization/frame_buffer.hpp"

#include "feedbus/messages/message_type.hpp"
#include "feedbus/messages/message.hpp"

namespace squawkbus::feedbus::messages
{
    using serialization::FrameBuffer;

    struct AuthorizationResponse : public Message
    {
        std::string client_id;
        std::string feed;
        std::string topic;
        bool is_authorization_required;
        std::set<std::int32_t> entitlements;

        AuthorizationResponse()
            :   Message(MessageType::AuthorizationResponse)
        {
        }

        AuthorizationResponse(
            const std::string &client_id,
            const std::string &feed,
            const std::string &topic,
            bool is_authorization_required,
            const std::set<std::int32_t> &entitlements)
            :   Message(MessageType::AuthorizationResponse),
                client_id(client_id),
                feed(feed),
                topic(topic),
                is_authorization_required(is_authorization_required),
                entitlements(entitlements)
        {
        }

        void write_body(FrameBuffer &frame) const override
        {
            frame
                << client_id
                << feed
                << topic
                << is_authorization_required
                << entitlements;
        }

        void read_body(FrameBuffer &frame) override
        {
            frame
                >> client_id
                >> feed
                >> topic
                >> is_authorization_required
                >> entitlements;
        }

        bool equals(const std::shared_ptr<AuthorizationResponse> &other) const
        {
            return
                message_type == other->message_type &&
                client_id == other->client_id &&
                feed == other->feed && 
                topic == other->topic &&
                is_authorization_required == other->is_authorization_required &&
                entitlements == other->entitlements;
        }

        bool equals(const std::shared_ptr<Message>& other) const override
        {
            return equals(std::static_pointer_cast<AuthorizationResponse>(other));
        }

        std::string to_string() const override
        {
            return std::format(
                "AuthorizationResponse(message_type={},client_id=\"{}\",feed=\"{}\",topic=\"{}\",is_authorization_required={},entitlements={})",
                messages::to_string(message_type),
                client_id,
                feed,
                topic,
                (is_authorization_required ? "<true>" : "<false>"),
                ::to_string(entitlements)
            );
        }
    };
}

#endif // SQUAWKBUS_FEEDBUS_MESSAGES_AUTHORIZATION_RESPONSE_HPP
