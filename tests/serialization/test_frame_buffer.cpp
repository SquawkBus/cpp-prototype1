#include <vector>

#include "serialization/frame_buffer.hpp"
#include "serialization/frame_buffer_io.hpp"

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

using namespace squawkbus::serialization;

TEST_CASE("Test read/write") {
    FrameBuffer frame;

    auto input = std::vector<char> { 'a', 'a', 'c' };
    
    frame.write(input);
    auto output = frame.read(input.size());
    
    REQUIRE(input == output);
}

TEST_CASE("Test char") {
    FrameBuffer frame;

    frame << 'g';
    char c;
    frame >> c;
    REQUIRE(c == 'g');
}

TEST_CASE("Test bool true") {
    FrameBuffer frame;

    frame << true;
    bool value;
    frame >> value;
    REQUIRE(value);
}

TEST_CASE("Test bool false") {
    FrameBuffer frame;

    frame << false;
    bool value;
    frame >> value;
    REQUIRE_FALSE(value);
}

TEST_CASE("Test bool many") {
    FrameBuffer frame;

    frame << false;
    frame << true;
    frame << false;
    frame << true;

    bool value;

    frame >> value;
    REQUIRE_FALSE(value);

    frame >> value;
    REQUIRE(value);

    frame >> value;
    REQUIRE_FALSE(value);

    frame >> value;
    REQUIRE(value);
}

TEST_CASE("Test uint32_t") {
    FrameBuffer frame;

    std::uint32_t a = 0, b = 1, c = 65001, x;
    frame << a;
    frame << b;
    frame << c;

    frame >> x;
    REQUIRE(x == a);
    frame >> x;
    REQUIRE(x == b);
    frame >> x;
    REQUIRE(x == c);
}