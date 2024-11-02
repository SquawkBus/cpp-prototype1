#ifndef SQUAWKBUS_MESSAGES_SUBSCRIPTION_REQUEST_HPP
#define SQUAWKBUS_MESSAGES_SUBSCRIPTION_REQUEST_HPP

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

  class SubscriptionRequest : public Message
  {
  public:
    std::string topic;
    bool is_add;

  public:
    SubscriptionRequest() noexcept
      : Message(MessageType::SubscriptionRequest)
    {
    }

    SubscriptionRequest(
      const std::string &topic,
      bool is_add) noexcept
      : Message(MessageType::SubscriptionRequest),
        topic(topic),
        is_add(is_add)
    {
    }

    bool operator==(const SubscriptionRequest &other) const noexcept
    {
      return
        Message::operator==(other) &&
        topic == other.topic &&
        is_add == other.is_add;
    }

    bool equals(const Message* other) const noexcept override
    {
      return operator==(*dynamic_cast<const SubscriptionRequest*>(other));
    }

    std::string str() const override
    {
      return std::format(
        "SubscriptionRequest(message_type={},topic=\"{}\",is_add={})",
        messages::to_string(message_type),
        topic,
        (is_add ? "<true>" : "<false"));
    }

  protected:

    void serialize_body(FrameBuffer &frame) const override
    {
      frame
        << topic
        << is_add;
    }

    void deserialize_body(FrameBuffer &frame) override
    {
      frame
        >> topic
        >> is_add;
    }
  };
}

#endif // SQUAWKBUS_MESSAGES_SUBSCRIPTION_REQUEST_HPP
