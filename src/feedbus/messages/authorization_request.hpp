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
  private:
    std::string client_id_;
    std::string host_;
    std::string user_;
    std::string feed_;
    std::string topic_;

  public:
    AuthorizationRequest() noexcept
      : Message(MessageType::AuthorizationRequest)
    {
    }

    AuthorizationRequest(
      const std::string &client_id,
      const std::string &host,
      const std::string &user,
      const std::string &feed,
      const std::string &topic) noexcept
      : Message(MessageType::AuthorizationRequest),
        client_id_(client_id),
        host_(host),
        user_(user),
        feed_(feed),
        topic_(topic)
    {
    }

    const std::string& client_id() const noexcept { return client_id_; }
    const std::string& host() const noexcept { return host_; }
    const std::string& user() const noexcept { return user_; }
    const std::string& feed() const noexcept { return feed_; }
    const std::string& topic() const noexcept { return topic_; }

    bool operator==(const AuthorizationRequest& other) const noexcept
    {
      return Message::operator==(other) &&
        client_id_ == other.client_id_ &&
        host_ == other.host_ &&
        user_ == other.user_ &&
        feed_ == other.feed_ &&
        topic_ == other.topic_;
    }

    bool equals(const std::shared_ptr<Message>& other) const noexcept override
    {
      return operator==(*std::static_pointer_cast<AuthorizationRequest>(other));
    }

    std::string str() const override
    {
      return std::format(
        "message_type={},client_id=\"{}\",host=\"{}\",user=\"{}\",feed=\"{}\",topic=\"{}\")",
        messages::to_string(message_type_),
        client_id_,
        host_,
        user_,
        feed_,
        topic_
      );
    }

  protected:

    void serialize_body(FrameBuffer &frame) const override
    {
      frame
        << client_id_
        << host_
        << user_
        << feed_
        << topic_;
    }

    void deserialize_body(FrameBuffer &frame) override
    {
      frame
        >> client_id_
        >> host_
        >> user_
        >> feed_
        >> topic_;
    }

  };
}

#endif // SQUAWKBUS_FEEDBUS_MESSAGES_AUTHORIZATION_REQUEST_HPP
