#ifndef SQUAWKBUS_MESSAGES_NOTIFICATION_REQUEST_HPP
#define SQUAWKBUS_MESSAGES_NOTIFICATION_REQUEST_HPP

#include <format>
#include <memory>
#include <string>

#include "serialization/frame_buffer.hpp"
#include "serialization/frame_buffer_io.hpp"

#include "messages/_message_type.hpp"
#include "messages/_message.hpp"

namespace squawkbus::messages
{
  using serialization::FrameBuffer;

  class NotificationRequest : public Message
  {
  public:
    std::string topic_pattern;
    bool is_add;

  public:
    NotificationRequest() noexcept
      : Message(MessageType::NotificationRequest)
    {
    }

    NotificationRequest(
      const std::string &topic_pattern,
      bool is_add) noexcept
      : Message(MessageType::NotificationRequest),
        topic_pattern(topic_pattern),
        is_add(is_add)
    {
    }

    bool operator==(const NotificationRequest &other) const noexcept
    {
      return
        Message::operator==(other) &&
        topic_pattern == other.topic_pattern &&
        is_add == other.is_add;
    }

    bool equals(const Message* other) const noexcept override
    {
      return operator==(*dynamic_cast<const NotificationRequest*>(other));
    }

    std::string str() const override
    {
      return std::format(
        "NotificationRequest(message_type={},topic_pattern=\"{}\",is_add={})",
        messages::to_string(message_type),
        topic_pattern,
        (is_add ? "<true>" : "<false>"));
    }

  protected:

    void serialize_body(FrameBuffer &frame) const override
    {
      frame
        << topic_pattern
        << is_add;
    }

    void deserialize_body(FrameBuffer &frame) override
    {
      frame
        >> topic_pattern
        >> is_add;
    }
  };
}

#endif // SQUAWKBUS_MESSAGES_NOTIFICATION_REQUEST_HPP
