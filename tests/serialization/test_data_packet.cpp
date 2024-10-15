#define CATCH_CONFIG_MAIN

#include <iostream>
#include <sstream>

#include "serialization/data_packet.hpp"
#include "serialization/frame_buffer.hpp"

#include "catch2/catch.hpp"

using namespace squawkbus::serialization;

TEST_CASE("smoke test") {
    auto dp = DataPacket(
        std::set<std::int32_t> { 1, 2},
        std::vector<char> { 'H', 'e', 'l', 'l', 'o' }
    );
    auto user_entitlements_pass = std::set<std::int32_t> { 1, 2, 3 };
    auto user_entitlements_fail = std::set<std::int32_t> { 1 };
    REQUIRE(dp.is_authorized(user_entitlements_pass));
    REQUIRE_FALSE(dp.is_authorized(user_entitlements_fail));
}

TEST_CASE("frames") {
    FrameBuffer frame;

    auto dp1 = DataPacket(
        std::set<std::int32_t> { 1, 2 },
        std::vector<char> { 'H', 'e', 'l', 'l', 'o' }
    );
    auto dp2 = DataPacket(
        std::set<std::int32_t> { 3, 4, 5 },
        std::vector<char> { 'W', 'o', 'r', 'l', 'd' }
    );
    std::vector<DataPacket> data_packets_in, data_packets_out;
    data_packets_in.push_back(dp1);
    data_packets_in.push_back(dp2);

    frame << data_packets_in;
    frame >> data_packets_out;
    REQUIRE(data_packets_in == data_packets_out);
}