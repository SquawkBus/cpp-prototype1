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
  private:
    std::string topic_;
    bool is_add_;

  public:
    SubscriptionRequest() noexcept
      : Message(MessageType::SubscriptionRequest)
    {
    }

    SubscriptionRequest(
      const std::string &topic,
      bool is_add) noexcept
      : Message(MessageType::SubscriptionRequest),
        topic_(topic),
        is_add_(is_add)
    {
    }

    const std::string& topic() const noexcept { return topic_; }
    bool is_add() const noexcept { return is_add_; }

    bool operator==(const SubscriptionRequest &other) const noexcept
    {
      return
        Message::operator==(other) &&
        topic_ == other.topic_ &&
        is_add_ == other.is_add_;
    }

    bool equals(const Message* other) const noexcept override
    {
      return operator==(*dynamic_cast<const SubscriptionRequest*>(other));
    }

    std::string str() const override
    {
      return std::format(
        "SubscriptionRequest(message_type={},topic=\"{}\",is_add={})",
        messages::to_string(message_type_),
        topic_,
        (is_add_ ? "<true>" : "<false"));
    }

  protected:

    void serialize_body(FrameBuffer &frame) const override
    {
      frame
        << topic_
        << is_add_;
    }

    void deserialize_body(FrameBuffer &frame) override
    {
      frame
        >> topic_
        >> is_add_;
    }
  };
}

#endif // SQUAWKBUS_MESSAGES_SUBSCRIPTION_REQUEST_HPP
