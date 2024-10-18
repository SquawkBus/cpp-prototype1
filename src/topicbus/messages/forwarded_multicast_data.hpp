#ifndef SQUAWKBUS_TOPICBUS_MESSAGES_FORWARDED_MULTICAST_DATA_HPP
#define SQUAWKBUS_TOPICBUS_MESSAGES_FORWARDED_MULTICAST_DATA_HPP

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

  class ForwardedMulticastData : public Message
  {
  public:
    std::string user;
    std::string host;
    std::string topic;
    std::string content_type;
    std::vector<DataPacket> data_packets;

    ForwardedMulticastData()
      : Message(MessageType::ForwardedMulticastData)
    {
    }

    ForwardedMulticastData(
      const std::string &user,
      const std::string &host,
      const std::string &topic,
      const std::string& content_type,
      const std::vector<DataPacket>& data_packets)
      : Message(MessageType::ForwardedMulticastData),
        user(user),
        host(host),
        topic(topic),
        content_type(content_type),
        data_packets(data_packets)
    {
    }

    bool equals(const std::shared_ptr<ForwardedMulticastData> &other) const
    {
      return
        message_type == other->message_type &&
        user == other->user &&
        host == other->host &&
        topic == other->topic &&
        content_type == other->content_type &&
        data_packets == other->data_packets;
    }

    bool equals(const std::shared_ptr<Message>& other) const override
    {
      return equals(std::static_pointer_cast<ForwardedMulticastData>(other));
    }

    std::string str() const override
    {
      return std::format(
        "ForwardedMulticastData(message_type={},user=\"{}\",host=\"{}\",topic=\"{}\",content_type=\"{}\",data_packets={})",
        messages::to_string(message_type),
        user,
        host,
        topic,
        content_type,
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
        << content_type
        << data_packets;        
    }

    void deserialize_body(FrameBuffer &frame) override
    {
      frame
        >> user
        >> host
        >> topic
        >> content_type
        >> data_packets;
    }

  };
}

#endif // SQUAWKBUS_TOPICBUS_MESSAGES_FORWARDED_MULTICAST_DATA_HPP
