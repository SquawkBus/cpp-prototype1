#include <memory>
#include <vector>

#include "messages/messages.hpp"

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

using squawkbus::messages::Message;
using squawkbus::messages::Authenticate;

TEST_CASE("smoke test")
{
    std::shared_ptr<Message> m0 = std::make_shared<Authenticate>(
        "PLAIN",
        std::vector {'f', 'r', 'e', 'd'}
    );
    
    auto frame = m0->serialize();

    auto m1 = Message::deserialize(frame);

    REQUIRE(m0 == m1);
}