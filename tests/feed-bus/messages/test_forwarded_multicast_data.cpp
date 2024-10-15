#include <cstdint>
#include <set>
#include <memory>
#include <vector>

#include "serialization/frame_buffer.hpp"
#include "serialization/data_packet.hpp"
#include "feed_bus/messages/message.hpp"

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

using namespace squawkbus::serialization;
using namespace squawkbus::feed_bus::messages;

TEST_CASE("roundtrip")
{
    FrameBuffer frame;

    std::shared_ptr<Message> m0 = std::make_shared<ForwardedMulticastData>(
        "client-1",
        "host-1.example.com",
        "PUB-1",
        "TOPIC-1",
        "text/plain",
        std::vector<DataPacket>{
            DataPacket(
                std::set<std::int32_t>{1, 2},
                std::vector<char>{'H', 'e', 'l', 'l', 'o'}),
            DataPacket(
                std::set<std::int32_t>{3, 4, 5},
                std::vector<char>{'W', 'o', 'r', 'l', 'd'})});
    m0->write(frame);

    auto m1 = Message::read(frame);

    REQUIRE(m0 == m1);
}