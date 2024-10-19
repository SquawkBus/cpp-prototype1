#ifndef SQUAWKBUS_FEEDBUS_MESSAGES_FORWARDED_MULTICAST_DATA_HPP
#define SQUAWKBUS_FEEDBUS_MESSAGES_FORWARDED_MULTICAST_DATA_HPP

#include <format>
#include <memory>
#include <string>
#include <vector>

#include "serialization/frame_buffer.hpp"
#include "serialization/frame_buffer_io.hpp"
#include "serialization/data_packet.hpp"

#include "feedbus/messages/message_type.hpp"
#include "feedbus/messages/message.hpp"

namespace squawkbus::feedbus::messages
{
  using serialization::FrameBuffer;
  using serialization::DataPacket;

  class ForwardedMulticastData : public Message
  {
  private:
    std::string user_;
    std::string host_;
    std::string feed_;
    std::string topic_;
    std::string content_type_;
    std::vector<DataPacket> data_packets_;

  public:
    ForwardedMulticastData() noexcept
      : Message(MessageType::ForwardedMulticastData)
    {
    }

    ForwardedMulticastData(
      const std::string &user,
      const std::string &host,
      const std::string &feed,
      const std::string &topic,
      const std::string& content_type,
      const std::vector<DataPacket>& data_packets) noexcept
      : Message(MessageType::ForwardedMulticastData),
        user_(user),
        host_(host),
        feed_(feed),
        topic_(topic),
        content_type_(content_type),
        data_packets_(data_packets)
    {
    }

    const std::string& user() const noexcept { return user_; }
    const std::string& host() const noexcept { return host_; }
    const std::string& feed() const noexcept { return feed_; }
    const std::string& topic() const noexcept { return topic_; }
    const std::string& content_type() const noexcept { return content_type_; }
    const std::vector<DataPacket>& data_packets() const noexcept { return data_packets_; }

    bool operator==(const ForwardedMulticastData& other) const noexcept
    {
      return Message::operator==(other) &&
        user_ == other.user_ &&
        host_ == other.host_ &&
        feed_ == other.feed_ &&
        topic_ == other.topic_ &&
        content_type_ == other.content_type_ &&
        data_packets_ == other.data_packets_;
    }

    bool equals(const std::shared_ptr<Message>& other) const noexcept override
    {
      return operator==(*std::static_pointer_cast<ForwardedMulticastData>(other));
    }

    std::string str() const override
    {
      return std::format(
        "ForwardedMulticastData(message_type={},user=\"{}\",host=\"{}\",feed=\"{}\",topic=\"{}\",content_type=\"{}\",data_packets={})",
        messages::to_string(message_type_),
        user_,
        host_,
        feed_,
        topic_,
        content_type_,
        ::to_string(data_packets_)
      );
    }
  protected:

    void serialize_body(FrameBuffer &frame) const override
    {
      frame
        << user_
        << host_
        << feed_
        << topic_
        << content_type_
        << data_packets_;        
    }

    void deserialize_body(FrameBuffer &frame) override
    {
      frame
        >> user_
        >> host_
        >> feed_
        >> topic_
        >> content_type_
        >> data_packets_;
    }
  };
}

#endif // SQUAWKBUS_FEEDBUS_MESSAGES_FORWARDED_MULTICAST_DATA_HPP
