#pragma once

class ICounterListener;

class Counter {
public:
    Counter() = default;
    explicit Counter(int initial);

    void setListener(ICounterListener* listener);

    void increment();
    void decrement();
    void reset();
    int value() const;

private:
    void notify();

    int m_value = 0;
    ICounterListener* m_listener = nullptr;
};
