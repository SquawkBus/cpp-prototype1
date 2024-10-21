#include <cstdint>
#include <set>
#include <memory>
#include <vector>

#include "serialization/frame_buffer.hpp"
#include "messages/messages.hpp"

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

using namespace squawkbus::serialization;
using namespace squawkbus::messages;

TEST_CASE("smoke test")
{
    std::shared_ptr<Message> m0 = std::make_shared<Authenticate>(
        "PLAIN",
        std::vector<char>{'f', 'r', 'e', 'd'}
    );
    auto frame = m0->serialize();

    auto m1 = Message::deserialize(frame);

    REQUIRE(m0 == m1);
}