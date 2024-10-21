#include <cstdint>
#include <set>
#include <memory>
#include <vector>

#include "serialization/frame_buffer.hpp"
#include "serialization/data_packet.hpp"
#include "messages/message.hpp"

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

using namespace squawkbus::serialization;
using namespace squawkbus::messages;

TEST_CASE("roundtrip")
{
    std::shared_ptr<Message> m0 = std::make_shared<UnicastData>(
        "client-1",
        "TOPIC-1",
        "text/plain",
        std::vector<DataPacket>{
            DataPacket(
                std::set<std::int32_t>{1, 2},
                std::vector<char>{'H', 'e', 'l', 'l', 'o'}),
            DataPacket(
                std::set<std::int32_t>{3, 4, 5},
                std::vector<char>{'W', 'o', 'r', 'l', 'd'})});
    auto frame = m0->serialize();

    auto m1 = Message::deserialize(frame);

    REQUIRE(m0 == m1);
}