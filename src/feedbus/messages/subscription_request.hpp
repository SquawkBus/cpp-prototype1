#ifndef SQUAWKBUS_FEEDBUS_MESSAGES_SUBSCRIPTION_REQUEST_HPP
#define SQUAWKBUS_FEEDBUS_MESSAGES_SUBSCRIPTION_REQUEST_HPP

#include <format>
#include <memory>
#include <string>

#include "serialization/frame_buffer.hpp"
#include "serialization/frame_buffer_io.hpp"

#include "feedbus/messages/message_type.hpp"
#include "feedbus/messages/message.hpp"

namespace squawkbus::feedbus::messages
{
  using serialization::FrameBuffer;

  class SubscriptionRequest : public Message
  {
  private:
    std::string feed_;
    std::string topic_;
    bool is_add_;

  public:
    SubscriptionRequest() noexcept
      : Message(MessageType::SubscriptionRequest)
    {
    }

    SubscriptionRequest(
      const std::string &feed,
      const std::string &topic,
      bool is_add) noexcept
      : Message(MessageType::SubscriptionRequest),
        feed_(feed),
        topic_(topic),
        is_add_(is_add)
    {
    }

    const std::string& feed() const noexcept {return feed_; }
    const std::string& topic() const noexcept {return topic_; }
    bool is_add() const noexcept { return is_add_; }

    bool operator==(const SubscriptionRequest& other) const noexcept
    {
      return Message::operator==(other) &&
        feed_ == other.feed_ &&
        topic_ == other.topic_ &&
        is_add_ == other.is_add_;
    }

    bool equals(const std::shared_ptr<Message> &other) const noexcept override
    {
      return operator==(*std::static_pointer_cast<SubscriptionRequest>(other));
    }

    std::string str() const override
    {
      return std::format(
        "SubscriptionRequest(message_type={},feed=\"{}\",topic=\"{}\",is_add={})",
        messages::to_string(message_type_),
        feed_,
        topic_,
        (is_add_ ? "<true>" : "<false"));
    }

  protected:

    void serialize_body(FrameBuffer &frame) const override
    {
      frame
        << feed_
        << topic_
        << is_add_;
    }

    void deserialize_body(FrameBuffer &frame) override
    {
      frame
        >> feed_
        >> topic_
        >> is_add_;
    }
  };
}

#endif // SQUAWKBUS_FEEDBUS_MESSAGES_SUBSCRIPTION_REQUEST_HPP
