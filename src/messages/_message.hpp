#ifndef SQUAWKBUS_MESSAGES_MESSAGE_HPP
#define SQUAWKBUS_MESSAGES_MESSAGE_HPP

#include <memory>
#include <string>
#include <utility>

#include "messages/_message_type.hpp"

namespace squawkbus::messages
{
  class Message
  {
  protected:
    MessageType message_type_;

  public:
    Message(MessageType message_type) noexcept
      : message_type_(message_type)
    {
    }
    virtual ~Message() noexcept
    {
    }

    MessageType message_type() const noexcept { return message_type_; }

    bool operator==(const Message& other) const noexcept
    {
      return message_type_ == other.message_type_;
    }

    serialization::FrameBuffer serialize() const
    {
      serialization::FrameBuffer frame;
      frame << message_type_;
      serialize_body(frame);
      return frame;
    }
    virtual bool equals(const Message* other) const noexcept = 0;
    virtual std::string str() const = 0;

    static std::shared_ptr<Message> deserialize(serialization::FrameBuffer& frame)
    {
      MessageType message_type;
      frame >> message_type;
      auto message = make_shared(message_type);
      message->deserialize_body(frame);
      return message;
    }
    static std::shared_ptr<Message> make_shared(MessageType message_type);

  protected:
    virtual void serialize_body(serialization::FrameBuffer& frame) const = 0;
    virtual void deserialize_body(serialization::FrameBuffer& frame) = 0;
  };
}

#endif // SQUAWKBUS_MESSAGES_MESSAGE_HPP
