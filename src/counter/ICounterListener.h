#pragma once

class ICounterListener {
public:
    virtual ~ICounterListener() = default;
    virtual void onValueChanged(int newValue) = 0;
};
