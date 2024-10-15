#include <memory>

#include "serialization/frame_buffer.hpp"
#include "feed_bus/messages/message.hpp"

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

TEST_CASE("smoke test") {
    squawkbus::serialization::FrameBuffer frame;

    std::shared_ptr<squawkbus::feed_bus::messages::Message> m0 = std::make_shared<squawkbus::feed_bus::messages::AuthorizationRequest>(
        "client-1",
        "host-1.example.com",
        "tom.jones",
        "PUB-1",
        "TOPIC-1"
    );
    m0->write(frame);

    auto m1 = squawkbus::feed_bus::messages::Message::read(frame);

    REQUIRE(m0 == m1);
}