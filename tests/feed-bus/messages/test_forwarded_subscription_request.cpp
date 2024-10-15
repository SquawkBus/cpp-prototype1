#include <memory>

#include "serialization/frame_buffer.hpp"
#include "feed_bus/messages/message.hpp"

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

using namespace squawkbus::serialization;
using namespace squawkbus::feed_bus::messages;

TEST_CASE("smoke test") {
    FrameBuffer frame;

    std::shared_ptr<Message> m0 = std::make_shared<ForwardedSubscriptionRequest>(
        "host-1.example.com",
        "tom.jones",
        "client-1",
        "PUB-1",
        "TOPIC-1",
        true
    );
    m0->write(frame);

    auto m1 = Message::read(frame);

    REQUIRE(m0 == m1);
}