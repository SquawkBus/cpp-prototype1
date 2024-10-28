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
  private:
    std::string client_id_;
    std::string topic_;
    std::vector<DataPacket> data_packets_;

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
        client_id_(client_id),
        topic_(topic),
        data_packets_(data_packets)
    {
    }

    const std::string& client_id() const noexcept { return client_id_; }
    const std::string& topic() const noexcept { return topic_; }
    const std::vector<DataPacket>& data_packets() const noexcept { return data_packets_; }

    bool operator==(const UnicastData &other) const noexcept
    {
      return
        Message::operator==(other) &&
        client_id_ == other.client_id_ &&
        topic_ == other.topic_ &&
        data_packets_ == other.data_packets_;
    }

    bool equals(const Message* other) const noexcept override
    {
      return operator==(*dynamic_cast<const UnicastData*>(other));
    }

    std::string str() const override
    {
      return std::format(
        "UnicastData(message_type={},client_id=\"{}\",topic=\"{}\",data_packets={})",
        messages::to_string(message_type_),
        client_id_,
        topic_,
        ::to_string(data_packets_));
    }

  protected:

    void serialize_body(FrameBuffer &frame) const override
    {
      frame
        << client_id_
        << topic_
        << data_packets_;
    }

    void deserialize_body(FrameBuffer &frame) override
    {
      frame
        >> client_id_
        >> topic_
        >> data_packets_;
    }
  };
}

#endif // SQUAWKBUS_MESSAGES_UNICAST_DATA
