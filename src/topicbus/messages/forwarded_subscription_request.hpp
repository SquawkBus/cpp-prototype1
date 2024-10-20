#ifndef SQUAWKBUS_TOPICBUS_MESSAGES_FORWARDED_SUBSCRIPTION_REQUEST_HPP
#define SQUAWKBUS_TOPICBUS_MESSAGES_FORWARDED_SUBSCRIPTION_REQUEST_HPP

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

  class ForwardedSubscriptionRequest : public Message
  {
  private:
    std::string user_;
    std::string host_;
    std::string client_id_;
    std::string topic_pattern_;
    bool is_add_;

  public:
    ForwardedSubscriptionRequest() noexcept
      : Message(MessageType::ForwardedSubscriptionRequest)
    {
    }

    ForwardedSubscriptionRequest(
      const std::string &user,
      const std::string &host,
      const std::string &client_id,
      const std::string &topic_pattern,
      bool is_add) noexcept
      : Message(MessageType::ForwardedSubscriptionRequest),
        user_(user),
        host_(host),
        client_id_(client_id),
        topic_pattern_(topic_pattern),
        is_add_(is_add)
    {
    }

    const std::string& user() const noexcept { return user_; }
    const std::string& host() const noexcept { return host_; }
    const std::string& client_id() const noexcept { return client_id_; }
    const std::string& topic_pattern() const noexcept { return topic_pattern_; }
    bool is_add() const noexcept { return is_add_; }

    bool operator==(const ForwardedSubscriptionRequest &other) const noexcept
    {
      return Message::operator==(other) &&
        user_ == other.user_ &&
        host_ == other.host_ &&
        client_id_ == other.client_id_ &&
        topic_pattern_ == other.topic_pattern_ &&
        is_add_ == other.is_add_;
    }

    bool equals(const Message* other) const noexcept override
    {
      return operator==(*dynamic_cast<const ForwardedSubscriptionRequest*>(other));
    }

    std::string str() const override
    {
      return std::format(
        "ForwardedSubscriptionRequest(message_type={},user=\"{}\",host=\"{}\",client_id=\"{}\",topic=\"{}\",is_add={})",
        messages::to_string(message_type_),
        user_,
        host_,
        client_id_,
        topic_pattern_,
        (is_add_ ? "<true>" : "<false>"));
    }

  protected:

    void serialize_body(FrameBuffer &frame) const override
    {
      frame
        << user_
        << host_
        << client_id_
        << topic_pattern_
        << is_add_;
    }

    void deserialize_body(FrameBuffer &frame) override
    {
      frame
        >> user_
        >> host_
        >> client_id_
        >> topic_pattern_
        >> is_add_;
    }
  };
}

#endif // SQUAWKBUS_TOPICBUS_MESSAGES_FORWARDED_SUBSCRIPTION_REQUEST_HPP
