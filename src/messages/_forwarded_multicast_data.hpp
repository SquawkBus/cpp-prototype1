#ifndef SQUAWKBUS_MESSAGES_FORWARDED_MULTICAST_DATA_HPP
#define SQUAWKBUS_MESSAGES_FORWARDED_MULTICAST_DATA_HPP

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

  class ForwardedMulticastData : public Message
  {
  public:
    std::string user;
    std::string host;
    std::string topic;
    std::vector<DataPacket> data_packets;

  public:
    ForwardedMulticastData() noexcept
      : Message(MessageType::ForwardedMulticastData)
    {
    }

    ForwardedMulticastData(
      const std::string &user,
      const std::string &host,
      const std::string &topic,
      const std::vector<DataPacket>& data_packets) noexcept
      : Message(MessageType::ForwardedMulticastData),
        user(user),
        host(host),
        topic(topic),
        data_packets(data_packets)
    {
    }

    bool operator==(const ForwardedMulticastData &other) const noexcept
    {
      return Message::operator==(other) &&
        user == other.user &&
        host == other.host &&
        topic == other.topic &&
        data_packets == other.data_packets;
    }

    bool equals(const Message* other) const noexcept override
    {
      return operator==(*dynamic_cast<const ForwardedMulticastData*>(other));
    }

    std::string str() const override
    {
      return std::format(
        "ForwardedMulticastData(message_type={},user=\"{}\",host=\"{}\",topic=\"{}\",data_packets={})",
        messages::to_string(message_type),
        user,
        host,
        topic,
        ::to_string(data_packets)
      );
    }

  protected:

    void serialize_body(FrameBuffer &frame) const override
    {
      frame
        << user
        << host
        << topic
        << data_packets;        
    }

    void deserialize_body(FrameBuffer &frame) override
    {
      frame
        >> user
        >> host
        >> topic
        >> data_packets;
    }

  };
}

#endif // SQUAWKBUS_MESSAGES_FORWARDED_MULTICAST_DATA_HPP
