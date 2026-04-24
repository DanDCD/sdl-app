#pragma once

class Counter {
public:
    Counter() = default;
    explicit Counter(int initial);

    void increment();
    void decrement();
    void reset();
    int value() const;

private:
    int m_value = 0;
};
