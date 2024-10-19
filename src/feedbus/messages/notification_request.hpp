#ifndef SQUAWKBUS_FEEDBUS_MESSAGES_NOTIFICATION_REQUEST_HPP
#define SQUAWKBUS_FEEDBUS_MESSAGES_NOTIFICATION_REQUEST_HPP

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

  class NotificationRequest : public Message
  {
  private:
    std::string feed_;
    bool is_add_;

  public:
    NotificationRequest() noexcept
      : Message(MessageType::NotificationRequest)
    {
    }

    NotificationRequest(
      const std::string &feed,
      bool is_add) noexcept
      : Message(MessageType::NotificationRequest),
        feed_(feed),
        is_add_(is_add)
    {
    }

    const std::string& feed() const noexcept { return feed_; }
    bool is_add() const noexcept { return is_add_; }

    bool operator==(const NotificationRequest& other) const noexcept
    {
      return Message::operator==(other) &&
        feed_ == other.feed_ &&
        is_add_ == other.is_add_;
    }

    bool equals(const std::shared_ptr<Message> &other) const noexcept override
    {
      return operator==(*std::static_pointer_cast<NotificationRequest>(other));
    }

    std::string str() const override
    {
      return std::format(
        "NotificationRequest(message_type={},feed=\"{}\",is_add={})",
        messages::to_string(message_type_),
        feed_,
        (is_add_ ? "<true>" : "<false>"));
    }

  protected:

    void serialize_body(FrameBuffer &frame) const override
    {
      frame
        << feed_
        << is_add_;
    }

    void deserialize_body(FrameBuffer &frame) override
    {
      frame
        >> feed_
        >> is_add_;
    }
  };
}

#endif // SQUAWKBUS_FEEDBUS_MESSAGES_NOTIFICATION_REQUEST_HPP
