#include "filter.h"

TdsFilter::TdsFilter() {}

float TdsFilter::update(float rawSample) {
    // TODO(Chunk 2): median-of-FILTER_WINDOW_SIZE + exponential smoothing.
    // Stub passes the raw sample through unchanged.
    return rawSample;
}
