#include <memory>
#include <vector>

#include "messages/messages.hpp"

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

using squawkbus::messages::Message;
using squawkbus::messages::ForwardedUnicastData;
using squawkbus::messages::DataPacket;

TEST_CASE("roundtrip")
{
    std::shared_ptr<Message> m0 = std::make_shared<ForwardedUnicastData>(
        "user-1",
        "host-1.example.com",
        "client-1",
        "TOPIC-1",
        std::vector<DataPacket>{
            DataPacket(
                1,
                "text/plain",
                std::vector {'H', 'e', 'l', 'l', 'o'}),
            DataPacket(
                2,
                "text/plain",
                std::vector {'W', 'o', 'r', 'l', 'd'})});

    auto frame = m0->serialize();

    auto m1 = Message::deserialize(frame);

    REQUIRE(m0 == m1);
}