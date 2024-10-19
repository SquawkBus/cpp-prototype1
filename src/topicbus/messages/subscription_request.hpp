#ifndef SQUAWKBUS_TOPICBUS_MESSAGES_SUBSCRIPTION_REQUEST_HPP
#define SQUAWKBUS_TOPICBUS_MESSAGES_SUBSCRIPTION_REQUEST_HPP

#include <format>
#include <memory>
#include <string>

#include "serialization/frame_buffer.hpp"
#include "serialization/frame_buffer_io.hpp"

#include "topicbus/messages/message_type.hpp"
#include "topicbus/messages/message.hpp"

namespace squawkbus::topicbus::messages
{
  using serialization::FrameBuffer;

  class SubscriptionRequest : public Message
  {
  public:
    std::string topic;
    bool is_add;

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

    bool equals(const std::shared_ptr<Message> &other) const noexcept override
    {
      return operator==(*std::static_pointer_cast<SubscriptionRequest>(other));
    }

    std::string str() const override
    {
      return std::format(
        "SubscriptionRequest(message_type={},topic=\"{}\",is_add={})",
        messages::to_string(message_type_),
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

#endif // SQUAWKBUS_TOPICBUS_MESSAGES_SUBSCRIPTION_REQUEST_HPP
