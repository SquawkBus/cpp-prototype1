#ifndef SQUAWKBUS_TOPICBUS_MESSAGES_MULTICAST_DATA_HPP
#define SQUAWKBUS_TOPICBUS_MESSAGES_MULTICAST_DATA_HPP

#include <format>
#include <memory>
#include <string>
#include <vector>

#include "serialization/frame_buffer.hpp"
#include "serialization/frame_buffer_io.hpp"
#include "serialization/data_packet.hpp"

#include "topicbus/messages/message_type.hpp"
#include "topicbus/messages/message.hpp"

namespace squawkbus::topicbus::messages
{
  using serialization::FrameBuffer;
  using serialization::DataPacket;

  class MulticastData : public Message
  {
  public:
    std::string topic;
    std::string content_type;
    std::vector<DataPacket> data_packets;

    MulticastData() noexcept
      : Message(MessageType::MulticastData)
    {
    }

    MulticastData(
      const std::string &topic,
      const std::string &content_type,
      const std::vector<DataPacket> &data_packets) noexcept
      : Message(MessageType::MulticastData),
        topic(topic),
        content_type(content_type),
        data_packets(data_packets)
    {
    }

    bool operator==(const MulticastData &other) const noexcept
    {
      return
        Message::operator==(other) &&
        topic == other.topic &&
        content_type == other.content_type &&
        data_packets == other.data_packets;
    }

    bool equals(const Message* other) const noexcept override
    {
      return operator==(*dynamic_cast<const MulticastData*>(other));
    }

    std::string str() const override
    {
      return std::format(
        "MulticastData(message_type={},topic=\"{}\",content_type=\"{}\",data_packets={})",
        messages::to_string(message_type_),
        topic,
        content_type,
        ::to_string(data_packets));
    }

  protected:

    void serialize_body(FrameBuffer &frame) const override
    {
      frame
        << topic
        << content_type
        << data_packets;
    }

    void deserialize_body(FrameBuffer &frame) override
    {
      frame
        >> topic
        >> content_type
        >> data_packets;
    }
  };
}

#endif // SQUAWKBUS_TOPICBUS_MESSAGES_MULTICAST_DATA_HPP
