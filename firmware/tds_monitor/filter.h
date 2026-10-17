#pragma once
#include <cstdint>
#include "config.h"

// Smooths a stream of raw sensor voltages: a short median window rejects
// single-sample spikes (e.g. a bubble), then exponential smoothing shapes
// the response. Pure C++, no Arduino dependency — unit-tested on the host.
class TdsFilter {
public:
    TdsFilter();

    // Feed one new raw sample, get back the filtered value.
    float update(float rawSample);

private:
    float   buffer_[FILTER_WINDOW_SIZE];
    uint8_t head_;
    uint8_t count_;
    bool    emaInitialized_;
    float   emaValue_;
};
