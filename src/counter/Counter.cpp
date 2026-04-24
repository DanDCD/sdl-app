#include "Counter.h"
#include "ICounterListener.h"

Counter::Counter(int initial) : m_value(initial) {}

void Counter::setListener(ICounterListener* listener) { m_listener = listener; }

void Counter::increment() { ++m_value; notify(); }
void Counter::decrement() { --m_value; notify(); }
void Counter::reset()     { m_value = 0; notify(); }
int  Counter::value() const { return m_value; }

void Counter::notify() {
    if (m_listener) m_listener->onValueChanged(m_value);
}
