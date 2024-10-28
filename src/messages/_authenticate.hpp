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
  private:
    std::string method_;
    std::vector<char> data_;

  public:
    Authenticate() noexcept
      : Message(MessageType::Authenticate)
    {
    }

    Authenticate(
      const std::string &method,
      const std::vector<char> &data) noexcept
      : Message(MessageType::Authenticate),
        method_(method),
        data_(data)
    {
    }

    const std::string& method() const noexcept { return method_; }
    const std::vector<char>& data() const noexcept { return data_; }

    bool operator==(const Authenticate &other) const noexcept
    {
      return
        Message::operator==(other) &&
        method_ == other.method_ &&
        data_ == other.data_;
    }

    bool equals(const Message* other) const noexcept override
    {
      return operator==(*dynamic_cast<const Authenticate*>(other));
    }

    std::string str() const override
    {
      return std::format(
        "Authenticate(message_type={},data=\"{}\",data={})",
        messages::to_string(message_type_),
        method_,
        ::to_string(data_));
    }

    protected:

      void serialize_body(FrameBuffer &frame) const override
      {
        frame
          << method_
          << data_;
      }

      void deserialize_body(FrameBuffer &frame) override
      {
        frame
          >> method_
          >> data_;
      }
  };
}

#endif // SQUAWKBUS_MESSAGES_AUTHENTICATE_DATA_HPP
