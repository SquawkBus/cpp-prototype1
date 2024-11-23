#ifndef SQUAWKBUS_MESSAGES_AUTHENTICATION_REQUEST_HPP
#define SQUAWKBUS_MESSAGES_AUTHENTICATION_REQUEST_HPP

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

  class AuthenticationRequest : public Message
  {
  public:
    std::string method;
    std::string data;

  public:
    AuthenticationRequest() noexcept
      : Message(MessageType::AuthenticationRequest)
    {
    }

    AuthenticationRequest(
      const std::string &method,
      const std::string &data) noexcept
      : Message(MessageType::AuthenticationRequest),
        method(method),
        data(data)
    {
    }

    bool operator==(const AuthenticationRequest &other) const noexcept
    {
      return
        Message::operator==(other) &&
        method == other.method &&
        data == other.data;
    }

    bool equals(const Message* other) const noexcept override
    {
      return operator==(*dynamic_cast<const AuthenticationRequest*>(other));
    }

    std::string str() const override
    {
      return std::format(
        "AuthenticationRequest(message_type={},method=\"{}\",data={})",
        messages::to_string(message_type),
        method,
        data);
    }

    protected:

      void serialize_body(FrameBuffer &frame) const override
      {
        frame
          << method
          << data;
      }

      void deserialize_body(FrameBuffer &frame) override
      {
        frame
          >> method
          >> data;
      }
  };
}

#endif // SQUAWKBUS_MESSAGES_AUTHENTICATION_REQUEST_HPP
