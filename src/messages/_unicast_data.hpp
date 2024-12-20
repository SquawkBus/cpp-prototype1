#ifndef SQUAWKBUS_MESSAGES_UNICAST_DATA
#define SQUAWKBUS_MESSAGES_UNICAST_DATA

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

  class UnicastData : public Message
  {
  public:
    std::string client_id;
    std::string topic;
    std::vector<DataPacket> data_packets;

  public:
    UnicastData() noexcept
      : Message(MessageType::UnicastData)
    {
    }

    UnicastData(
      const std::string &client_id,
      const std::string &topic,
      const std::vector<DataPacket>& data_packets) noexcept
      : Message(MessageType::UnicastData),
        client_id(client_id),
        topic(topic),
        data_packets(data_packets)
    {
    }

    bool operator==(const UnicastData &other) const noexcept
    {
      return
        Message::operator==(other) &&
        client_id == other.client_id &&
        topic == other.topic &&
        data_packets == other.data_packets;
    }

    bool equals(const Message* other) const noexcept override
    {
      return operator==(*dynamic_cast<const UnicastData*>(other));
    }

    std::string str() const override
    {
      return std::format(
        "UnicastData(message_type={},client_id=\"{}\",topic=\"{}\",data_packets={})",
        messages::to_string(message_type),
        client_id,
        topic,
        ::to_string(data_packets));
    }

  protected:

    void serialize_body(FrameBuffer &frame) const override
    {
      frame
        << client_id
        << topic
        << data_packets;
    }

    void deserialize_body(FrameBuffer &frame) override
    {
      frame
        >> client_id
        >> topic
        >> data_packets;
    }
  };
}

#endif // SQUAWKBUS_MESSAGES_UNICAST_DATA
