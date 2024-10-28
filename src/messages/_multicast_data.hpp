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
  private:
    std::string topic_;
    std::vector<DataPacket> data_packets_;

  public:
    MulticastData() noexcept
      : Message(MessageType::MulticastData)
    {
    }

    MulticastData(
      const std::string &topic,
      const std::vector<DataPacket> &data_packets) noexcept
      : Message(MessageType::MulticastData),
        topic_(topic),
        data_packets_(data_packets)
    {
    }

    const std::string& topic() const noexcept { return topic_; }
    const std::vector<DataPacket>& data_packets() const noexcept { return data_packets_; }

    bool operator==(const MulticastData &other) const noexcept
    {
      return
        Message::operator==(other) &&
        topic_ == other.topic_ &&
        data_packets_ == other.data_packets_;
    }

    bool equals(const Message* other) const noexcept override
    {
      return operator==(*dynamic_cast<const MulticastData*>(other));
    }

    std::string str() const override
    {
      return std::format(
        "MulticastData(message_type={},topic=\"{}\",data_packets={})",
        messages::to_string(message_type_),
        topic_,
        ::to_string(data_packets_));
    }

  protected:

    void serialize_body(FrameBuffer &frame) const override
    {
      frame
        << topic_
        << data_packets_;
    }

    void deserialize_body(FrameBuffer &frame) override
    {
      frame
        >> topic_
        >> data_packets_;
    }
  };
}

#endif // SQUAWKBUS_MESSAGES_MULTICAST_DATA_HPP
