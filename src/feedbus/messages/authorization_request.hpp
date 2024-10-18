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

  class AuthorizationRequest : public Message
  {
  public:
    std::string client_id;
    std::string host;
    std::string user;
    std::string feed;
    std::string topic;

    AuthorizationRequest()
      : Message(MessageType::AuthorizationRequest)
    {
    }

    AuthorizationRequest(
      const std::string &client_id,
      const std::string &host,
      const std::string &user,
      const std::string &feed,
      const std::string &topic)
      : Message(MessageType::AuthorizationRequest),
        client_id(client_id),
        host(host),
        user(user),
        feed(feed),
        topic(topic)
    {
    }

    bool operator==(const AuthorizationRequest& other) const noexcept
    {
      return Message::operator==(other) &&
        client_id == other.client_id &&
        host == other.host &&
        user == other.user &&
        feed == other.feed &&
        topic == other.topic;
    }

    bool equals(const std::shared_ptr<Message>& other) const override
    {
      return operator==(*std::static_pointer_cast<AuthorizationRequest>(other));
    }

    std::string str() const override
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

  protected:

    void serialize_body(FrameBuffer &frame) const override
    {
      frame
        << client_id
        << host
        << user
        << feed
        << topic;
    }

    void deserialize_body(FrameBuffer &frame) override
    {
      frame
        >> client_id
        >> host
        >> user
        >> feed
        >> topic;
    }

  };
}

#endif // SQUAWKBUS_FEEDBUS_MESSAGES_AUTHORIZATION_REQUEST_HPP
