#include "filter.h"
#include <algorithm>

namespace {
float median5(const float *v) {
    float sorted[5] = { v[0], v[1], v[2], v[3], v[4] };
    std::sort(sorted, sorted + 5);
    return sorted[2];
}
}

TdsFilter::TdsFilter()
    : head_(0), count_(0), emaInitialized_(false), emaValue_(0.0f) {
    for (uint8_t i = 0; i < FILTER_WINDOW_SIZE; i++) buffer_[i] = 0.0f;
}

float TdsFilter::update(float rawSample) {
    buffer_[head_] = rawSample;
    head_ = (head_ + 1) % FILTER_WINDOW_SIZE;
    if (count_ < FILTER_WINDOW_SIZE) count_++;

    // Not enough samples yet for a real median: pass the raw sample through.
    const float medianVal = (count_ < FILTER_WINDOW_SIZE) ? rawSample : median5(buffer_);

    if (!emaInitialized_) {
        emaValue_ = medianVal;
        emaInitialized_ = true;
    } else {
        emaValue_ = FILTER_SMOOTHING * medianVal + (1.0f - FILTER_SMOOTHING) * emaValue_;
    }
    return emaValue_;
}
