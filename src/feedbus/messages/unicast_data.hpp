#ifndef SQUAWKBUS_FEEDBUS_MESSAGES_UNICAST_DATA
#define SQUAWKBUS_FEEDBUS_MESSAGES_UNICAST_DATA

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

  class UnicastData : public Message
  {
  private:
    std::string client_id_;
    std::string feed_;
    std::string topic_;
    std::string content_type_;
    std::vector<DataPacket> data_packets_;

  public:
    UnicastData() noexcept
      : Message(MessageType::UnicastData)
    {
    }

    UnicastData(
      const std::string &client_id,
      const std::string &feed,
      const std::string &topic,
      const std::string& content_type,
      const std::vector<DataPacket>& data_packets) noexcept
      : Message(MessageType::UnicastData),
        client_id_(client_id),
        feed_(feed),
        topic_(topic),
        content_type_(content_type),
        data_packets_(data_packets)
    {
    }

    const std::string& client_id() const noexcept { return client_id_; }
    const std::string& feed() const noexcept { return feed_; }
    const std::string& topic() const noexcept { return topic_; }
    const std::string& content_type() const noexcept { return content_type_; }
    const std::vector<DataPacket>& data_packets() const noexcept { return data_packets_; }

    bool operator==(const UnicastData& other) const noexcept
    {
      return Message::operator==(other) &&
        client_id_ == other.client_id_ &&
        feed_ == other.feed_ &&
        topic_ == other.topic_ &&
        content_type_ == other.content_type_ &&
        data_packets_ == other.data_packets_;
    }

    bool equals(const std::shared_ptr<Message>& other) const noexcept override
    {
      return operator==(*std::static_pointer_cast<UnicastData>(other));
    }

    std::string str() const override
    {
      return std::format(
        "UnicastData(message_type={},client_id=\"{}\",feed=\"{}\",topic=\"{}\",content_type=\"{}\",data_packets={})",
        messages::to_string(message_type_),
        client_id_,
        feed_,
        topic_,
        content_type_,
        ::to_string(data_packets_));
    }

  protected:

    void serialize_body(FrameBuffer &frame) const override
    {
      frame
        << client_id_
        << feed_
        << topic_
        << content_type_
        << data_packets_;
    }

    void deserialize_body(FrameBuffer &frame) override
    {
      frame
        >> client_id_
        >> feed_
        >> topic_
        >> content_type_
        >> data_packets_;
    }
  };
}

#endif // SQUAWKBUS_FEEDBUS_MESSAGES_UNICAST_DATA
