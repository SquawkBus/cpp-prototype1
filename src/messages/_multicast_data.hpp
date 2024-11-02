#ifndef SQUAWKBUS_MESSAGES_MULTICAST_DATA_HPP
#define SQUAWKBUS_MESSAGES_MULTICAST_DATA_HPP

#include <format>
#include <memory>
#include <string>
#include <vector>

#include "serialization/frame_buffer.hpp"
#include "serialization/frame_buffer_io.hpp"

#include "messages/_data_packet.hpp"
#include "messages/_message_type.hpp"
#include "messages/_message.hpp"

namespace squawkbus::messages
{
  using serialization::FrameBuffer;

  class MulticastData : public Message
  {
  public:
    std::string topic;
    std::vector<DataPacket> data_packets;

  public:
    MulticastData() noexcept
      : Message(MessageType::MulticastData)
    {
    }

    MulticastData(
      const std::string &topic,
      const std::vector<DataPacket> &data_packets) noexcept
      : Message(MessageType::MulticastData),
        topic(topic),
        data_packets(data_packets)
    {
    }

    bool operator==(const MulticastData &other) const noexcept
    {
      return
        Message::operator==(other) &&
        topic == other.topic &&
        data_packets == other.data_packets;
    }

    bool equals(const Message* other) const noexcept override
    {
      return operator==(*dynamic_cast<const MulticastData*>(other));
    }

    std::string str() const override
    {
      return std::format(
        "MulticastData(message_type={},topic=\"{}\",data_packets={})",
        messages::to_string(message_type),
        topic,
        ::to_string(data_packets));
    }

  protected:

    void serialize_body(FrameBuffer &frame) const override
    {
      frame
        << topic
        << data_packets;
    }

    void deserialize_body(FrameBuffer &frame) override
    {
      frame
        >> topic
        >> data_packets;
    }
  };
}

#endif // SQUAWKBUS_MESSAGES_MULTICAST_DATA_HPP
