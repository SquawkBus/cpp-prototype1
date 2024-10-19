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

  class AuthorizationResponse : public Message
  {
  private:
    std::string client_id_;
    std::string feed_;
    std::string topic_;
    bool is_authorization_required_;
    std::set<std::int32_t> entitlements_;

  public:
    AuthorizationResponse() noexcept
      : Message(MessageType::AuthorizationResponse)
    {
    }

    AuthorizationResponse(
      const std::string &client_id,
      const std::string &feed,
      const std::string &topic,
      bool is_authorization_required,
      const std::set<std::int32_t> &entitlements) noexcept
      : Message(MessageType::AuthorizationResponse),
        client_id_(client_id),
        feed_(feed),
        topic_(topic),
        is_authorization_required_(is_authorization_required),
        entitlements_(entitlements)
    {
    }

    const std::string& client_id() const noexcept {return client_id_; }
    const std::string& feed() const noexcept {return feed_; }
    const std::string& topic() const noexcept {return topic_; }
    bool is_authorization_required() const noexcept { return is_authorization_required_; }
    const std::set<std::int32_t>& entitlements() const noexcept { return entitlements_; }

    bool operator==(const AuthorizationResponse& other) const noexcept
    {
      return Message::operator==(other) &&
        client_id_ == other.client_id_ &&
        feed_ == other.feed_ && 
        topic_ == other.topic_ &&
        is_authorization_required_ == other.is_authorization_required_ &&
        entitlements_ == other.entitlements_;
    }

    bool equals(const std::shared_ptr<Message>& other) const noexcept override
    {
      return operator==(*std::static_pointer_cast<AuthorizationResponse>(other));
    }

    std::string str() const override
    {
      return std::format(
        "AuthorizationResponse(message_type={},client_id=\"{}\",feed=\"{}\",topic=\"{}\",is_authorization_required={},entitlements={})",
        messages::to_string(message_type_),
        client_id_,
        feed_,
        topic_,
        (is_authorization_required_ ? "<true>" : "<false>"),
        ::to_string(entitlements_)
      );
    }

  protected:

    void serialize_body(FrameBuffer &frame) const override
    {
      frame
        << client_id_
        << feed_
        << topic_
        << is_authorization_required_
        << entitlements_;
    }

    void deserialize_body(FrameBuffer &frame) override
    {
      frame
        >> client_id_
        >> feed_
        >> topic_
        >> is_authorization_required_
        >> entitlements_;
    }
  };
}

#endif // SQUAWKBUS_FEEDBUS_MESSAGES_AUTHORIZATION_RESPONSE_HPP
