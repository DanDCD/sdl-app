#include "Counter.h"

Counter::Counter(int initial) : m_value(initial) {}

void Counter::increment() { ++m_value; }
void Counter::decrement() { --m_value; }
void Counter::reset()     { m_value = 0; }
int  Counter::value() const { return m_value; }
