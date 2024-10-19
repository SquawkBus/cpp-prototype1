#ifndef SQUAWKBUS_TOPICBUS_MESSAGES_UNICAST_DATA
#define SQUAWKBUS_TOPICBUS_MESSAGES_UNICAST_DATA

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

  class UnicastData : public Message
  {
  public:
    std::string client_id;
    std::string topic;
    std::string content_type;
    std::vector<DataPacket> data_packets;

    UnicastData() noexcept
      : Message(MessageType::UnicastData)
    {
    }

    UnicastData(
      const std::string &client_id,
      const std::string &topic,
      const std::string& content_type,
      const std::vector<DataPacket>& data_packets) noexcept
      : Message(MessageType::UnicastData),
        client_id(client_id),
        topic(topic),
        content_type(content_type),
        data_packets(data_packets)
    {
    }

    bool operator==(const UnicastData &other) const noexcept
    {
      return
        Message::operator==(other) &&
        client_id == other.client_id &&
        topic == other.topic &&
        content_type == other.content_type &&
        data_packets == other.data_packets;
    }

    bool equals(const Message* other) const noexcept override
    {
      return operator==(*dynamic_cast<const UnicastData*>(other));
    }

    std::string str() const override
    {
      return std::format(
        "UnicastData(message_type={},client_id=\"{}\",topic=\"{}\",content_type=\"{}\",data_packets={})",
        messages::to_string(message_type_),
        client_id,
        topic,
        content_type,
        ::to_string(data_packets));
    }

  protected:

    void serialize_body(FrameBuffer &frame) const override
    {
      frame
        << client_id
        << topic
        << content_type
        << data_packets;
    }

    void deserialize_body(FrameBuffer &frame) override
    {
      frame
        >> client_id
        >> topic
        >> content_type
        >> data_packets;
    }
  };
}

#endif // SQUAWKBUS_TOPICBUS_MESSAGES_UNICAST_DATA
