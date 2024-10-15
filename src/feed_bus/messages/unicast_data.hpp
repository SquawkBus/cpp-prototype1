#ifndef SQUAWKBUS_FEED_BUS_MESSAGES_UNICAST_DATA
#define SQUAWKBUS_FEED_BUS_MESSAGES_UNICAST_DATA

#include <format>
#include <memory>
#include <string>
#include <vector>

#include "serialization/frame_buffer.hpp"
#include "serialization/frame_buffer_io.hpp"
#include "serialization/data_packet.hpp"

#include "feed_bus/messages/message_type.hpp"
#include "feed_bus/messages/message.hpp"

namespace squawkbus::feed_bus::messages
{
    using serialization::FrameBuffer;
    using serialization::DataPacket;

    struct UnicastData : public Message
    {
        std::string client_id;
        std::string feed;
        std::string topic;
        std::string content_type;
        std::vector<DataPacket> data_packets;

        UnicastData()
            : Message(MessageType::UnicastData)
        {
        }

        UnicastData(
            const std::string &client_id,
            const std::string &feed,
            const std::string &topic,
            const std::string& content_type,
            const std::vector<DataPacket>& data_packets)
            :   Message(MessageType::UnicastData),
                client_id(client_id),
                feed(feed),
                topic(topic),
                content_type(content_type),
                data_packets(data_packets)
        {
        }

        void write_body(FrameBuffer &frame) const override
        {
            frame
                << client_id
                << feed
                << topic
                << content_type
                << data_packets;
        }

        void read_body(FrameBuffer &frame) override
        {
            frame
                >> client_id
                >> feed
                >> topic
                >> content_type
                >> data_packets;
        }

        bool equals(const std::shared_ptr<UnicastData> &other) const
        {
            return
                message_type == other->message_type &&
                client_id == other->client_id &&
                feed == other->feed &&
                topic == other->topic &&
                content_type == other->content_type &&
                data_packets == other->data_packets;
        }

        bool equals(const std::shared_ptr<Message>& other) const override
        {
            return equals(std::static_pointer_cast<UnicastData>(other));
        }

        std::string to_string() const override
        {
            return std::format(
                "UnicastData(message_type={},client_id=\"{}\",feed=\"{}\",topic=\"{}\",content_type=\"{}\",data_packets={})",
                messages::to_string(message_type),
                client_id,
                feed,
                topic,
                content_type,
                ::to_string(data_packets)
            );
        }
    };
}

#endif // SQUAWKBUS_FEED_BUS_MESSAGES_UNICAST_DATA
