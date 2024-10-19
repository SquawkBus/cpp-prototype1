#ifndef SQUAWKBUS_FEEDBUS_MESSAGES_MULTICAST_DATA_HPP
#define SQUAWKBUS_FEEDBUS_MESSAGES_MULTICAST_DATA_HPP

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

  class MulticastData : public Message
  {
  private:
    std::string feed_;
    std::string topic_;
    std::string content_type_;
    std::vector<DataPacket> data_packets_;

  public:
    MulticastData() noexcept
      : Message(MessageType::MulticastData)
    {
    }

    MulticastData(
      const std::string &feed,
      const std::string &topic,
      const std::string &content_type,
      const std::vector<DataPacket> &data_packets) noexcept
      : Message(MessageType::MulticastData),
        feed_(feed),
        topic_(topic),
        content_type_(content_type),
        data_packets_(data_packets)
    {
    }

    const std::string& feed() const noexcept { return feed_; }
    const std::string& topic() const noexcept { return topic_; }
    const std::string& content_type() const noexcept { return content_type_; }
    const std::vector<DataPacket>& data_packets() const noexcept { return data_packets_; }

    bool operator==(const MulticastData& other) const noexcept
    {
      return Message::operator==(other) &&
        feed_ == other.feed_ &&
        topic_ == other.topic_ &&
        content_type_ == other.content_type_ &&
        data_packets_ == other.data_packets_;
    }

    bool equals(const std::shared_ptr<Message>& other) const noexcept override
    {
      return operator==(*std::static_pointer_cast<MulticastData>(other));
    }

    std::string str() const override
    {
      return std::format(
        "MulticastData(message_type={},feed=\"{}\",topic=\"{}\",content_type=\"{}\",data_packets={})",
        messages::to_string(message_type_),
        feed_,
        topic_,
        content_type_,
        ::to_string(data_packets_));
    }

  protected:

    void serialize_body(FrameBuffer &frame) const override
    {
      frame
        << feed_
        << topic_
        << content_type_
        << data_packets_;
    }

    void deserialize_body(FrameBuffer &frame) override
    {
      frame
        >> feed_
        >> topic_
        >> content_type_
        >> data_packets_;
    }
  };
}

#endif // SQUAWKBUS_FEEDBUS_MESSAGES_MULTICAST_DATA_HPP
