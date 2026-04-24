#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "counter/Counter.h"
#include "counter/ICounterListener.h"

// ── Value tests ────────────────────────────────────────────────────────────

TEST(CounterTest, DefaultInitialisesToZero) {
    Counter c;
    EXPECT_EQ(c.value(), 0);
}

TEST(CounterTest, InitialisesToGivenValue) {
    Counter c{5};
    EXPECT_EQ(c.value(), 5);
}

TEST(CounterTest, IncrementIncreasesValueByOne) {
    Counter c;
    c.increment();
    EXPECT_EQ(c.value(), 1);
}

TEST(CounterTest, DecrementDecreasesValueByOne) {
    Counter c;
    c.decrement();
    EXPECT_EQ(c.value(), -1);
}

TEST(CounterTest, ResetReturnsValueToZero) {
    Counter c{10};
    c.reset();
    EXPECT_EQ(c.value(), 0);
}

// ── Listener mock ──────────────────────────────────────────────────────────

class MockCounterListener : public ICounterListener {
public:
    MOCK_METHOD(void, onValueChanged, (int newValue), (override));
};

class CounterListenerTest : public ::testing::Test {
protected:
    MockCounterListener listener;
    Counter counter;

    void SetUp() override {
        counter.setListener(&listener);
    }
};

TEST_F(CounterListenerTest, NotifiesOnIncrement) {
    EXPECT_CALL(listener, onValueChanged(1));
    counter.increment();
}

TEST_F(CounterListenerTest, NotifiesOnDecrement) {
    EXPECT_CALL(listener, onValueChanged(-1));
    counter.decrement();
}

TEST_F(CounterListenerTest, NotifiesOnReset) {
    Counter c{5};
    c.setListener(&listener);
    EXPECT_CALL(listener, onValueChanged(0));
    c.reset();
}

TEST_F(CounterListenerTest, NoCallsWithoutListener) {
    Counter c;
    EXPECT_CALL(listener, onValueChanged(::testing::_)).Times(0);
    c.increment();
    EXPECT_EQ(c.value(), 1);
}
