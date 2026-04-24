#include <catch2/catch_test_macros.hpp>
#include "counter/Counter.h"

TEST_CASE("Counter default-initialises to zero", "[counter]") {
    Counter c;
    REQUIRE(c.value() == 0);
}

TEST_CASE("Counter initialises to given value", "[counter]") {
    Counter c{5};
    REQUIRE(c.value() == 5);
}

TEST_CASE("increment increases value by one", "[counter]") {
    Counter c;
    c.increment();
    REQUIRE(c.value() == 1);
}

TEST_CASE("decrement decreases value by one", "[counter]") {
    Counter c;
    c.decrement();
    REQUIRE(c.value() == -1);
}

TEST_CASE("reset returns value to zero", "[counter]") {
    Counter c{10};
    c.reset();
    REQUIRE(c.value() == 0);
}
