#ifndef SQUAWKBUS_MESSAGES_AUTHENTICATION_RESPONSE_HPP
#define SQUAWKBUS_MESSAGES_AUTHENTICATION_RESPONSE_HPP

#include <format>
#include <memory>
#include <string>
#include <vector>

#include "serialization/frame_buffer.hpp"
#include "serialization/frame_buffer_io.hpp"

#include "messages/_data_packet.hpp"
#include "messages/_message_type.hpp"
#include "messages/_message.hpp"

namespace squawkbus::messages
{
  using serialization::FrameBuffer;

  class AuthenticationResponse : public Message
  {
  public:
    bool is_authenticated;

  public:
    AuthenticationResponse() noexcept
      : Message(MessageType::AuthenticationResponse)
    {
    }

    AuthenticationResponse(bool is_authenticated) noexcept
      : Message(MessageType::AuthenticationResponse),
        is_authenticated(is_authenticated)
    {
    }

    bool operator==(const AuthenticationResponse &other) const noexcept
    {
      return
        Message::operator==(other) &&
        is_authenticated == other.is_authenticated;
    }

    bool equals(const Message* other) const noexcept override
    {
      return operator==(*dynamic_cast<const AuthenticationResponse*>(other));
    }

    std::string str() const override
    {
      return std::format(
        "AuthenticationResponse(message_type={},is_authenticated={})",
        messages::to_string(message_type),
        is_authenticated);
    }

    protected:

      void serialize_body(FrameBuffer &frame) const override
      {
        frame
          << is_authenticated;
      }

      void deserialize_body(FrameBuffer &frame) override
      {
        frame
          >> is_authenticated;
      }
  };
}

#endif // SQUAWKBUS_MESSAGES_AUTHENTICATION_RESPONSE_HPP
