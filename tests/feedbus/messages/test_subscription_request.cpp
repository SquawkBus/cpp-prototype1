#include <memory>

#include "serialization/frame_buffer.hpp"
#include "feedbus/messages/message.hpp"

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

using namespace squawkbus::serialization;
using namespace squawkbus::feedbus::messages;

TEST_CASE("smoke test")
{
    std::shared_ptr<Message> m0 = std::make_shared<SubscriptionRequest>(
        "PUB-1",
        "TOPIC-1",
        true
    );
    auto frame = m0->serialize();

    auto m1 = Message::deserialize(frame);

    REQUIRE(m0 == m1);
}