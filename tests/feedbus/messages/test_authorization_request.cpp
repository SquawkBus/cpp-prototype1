#include <cstdint>
#include <set>
#include <memory>

#include "serialization/frame_buffer.hpp"
#include "feedbus/messages/message.hpp"

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

using namespace squawkbus::serialization;
using namespace squawkbus::feedbus::messages;

TEST_CASE("smoke test") {
    FrameBuffer frame;

    std::shared_ptr<Message> m0 = std::make_shared<AuthorizationRequest>(
        "client-1",
        "host-1.example.com",
        "tom.jones",
        "PUB-1",
        "TOPIC-1"
    );
    m0->write(frame);

    auto m1 = Message::read(frame);

    REQUIRE(m0 == m1);
}