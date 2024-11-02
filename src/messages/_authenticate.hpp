#ifndef SQUAWKBUS_MESSAGES_AUTHENTICATE_DATA_HPP
#define SQUAWKBUS_MESSAGES_AUTHENTICATE_DATA_HPP

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

  class Authenticate : public Message
  {
  public:
    std::string method;
    std::vector<char> data;

  public:
    Authenticate() noexcept
      : Message(MessageType::Authenticate)
    {
    }

    Authenticate(
      const std::string &method,
      const std::vector<char> &data) noexcept
      : Message(MessageType::Authenticate),
        method(method),
        data(data)
    {
    }

    bool operator==(const Authenticate &other) const noexcept
    {
      return
        Message::operator==(other) &&
        method == other.method &&
        data == other.data;
    }

    bool equals(const Message* other) const noexcept override
    {
      return operator==(*dynamic_cast<const Authenticate*>(other));
    }

    std::string str() const override
    {
      return std::format(
        "Authenticate(message_type={},data=\"{}\",data={})",
        messages::to_string(message_type),
        method,
        ::to_string(data));
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

#endif // SQUAWKBUS_MESSAGES_AUTHENTICATE_DATA_HPP
