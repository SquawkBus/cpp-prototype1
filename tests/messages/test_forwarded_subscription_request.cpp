#include <memory>

#include "serialization/frame_buffer.hpp"
#include "messages/message.hpp"

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

using namespace squawkbus::serialization;
using namespace squawkbus::messages;

TEST_CASE("smoke test")
{
    std::shared_ptr<Message> m0 = std::make_shared<ForwardedSubscriptionRequest>(
        "host-1.example.com",
        "tom.jones",
        "client-1",
        "TOPIC-1",
        true
    );
    auto frame = m0->serialize();

    auto m1 = Message::deserialize(frame);

    REQUIRE(m0 == m1);
}