#include "filter.h"
#include <cassert>
#include <cmath>

namespace {
bool approxEqual(float a, float b, float eps = 1e-4f) { return std::fabs(a - b) < eps; }
}

int main() {
    // First call: fewer than FILTER_WINDOW_SIZE samples, so the median stage
    // passes the raw value through and the EMA initializes to it exactly.
    TdsFilter f1;
    assert(approxEqual(f1.update(1.23f), 1.23f));

    // Steady input converges to itself once the window fills.
    TdsFilter f2;
    float out = 0.0f;
    for (int i = 0; i < 20; i++) out = f2.update(2.0f);
    assert(approxEqual(out, 2.0f));

    // A single spike among steady samples (e.g. a bubble) gets rejected by
    // the median-of-5 stage, not just smoothed.
    TdsFilter f3;
    for (int i = 0; i < 10; i++) f3.update(1.0f); // settle
    const float spikeOut = f3.update(9.0f);
    assert(approxEqual(spikeOut, 1.0f, 0.05f));

    return 0;
}
