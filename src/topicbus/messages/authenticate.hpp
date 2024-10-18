#ifndef SQUAWKBUS_TOPICBUS_MESSAGES_AUTHENTICATE_DATA_HPP
#define SQUAWKBUS_TOPICBUS_MESSAGES_AUTHENTICATE_DATA_HPP

#include <format>
#include <memory>
#include <string>
#include <vector>

#include "serialization/frame_buffer.hpp"
#include "serialization/frame_buffer_io.hpp"
#include "serialization/data_packet.hpp"

#include "topicbus/messages/message_type.hpp"
#include "topicbus/messages/message.hpp"

namespace squawkbus::topicbus::messages
{
  using serialization::FrameBuffer;

  class Authenticate : public Message
  {
  public:
    std::string method;
    std::vector<char> data;

    Authenticate()
      : Message(MessageType::Authenticate)
    {
    }

    Authenticate(
      const std::string &method,
      const std::vector<char> &data)
      : Message(MessageType::Authenticate),
        method(method),
        data(data)
    {
    }

    bool equals(const std::shared_ptr<Authenticate> &other) const
    {
      return
        message_type == other->message_type &&
        method == other->method &&
        data == other->data;
    }

    bool equals(const std::shared_ptr<Message>& other) const override
    {
      return equals(std::static_pointer_cast<Authenticate>(other));
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

#endif // SQUAWKBUS_TOPICBUS_MESSAGES_AUTHENTICATE_DATA_HPP
