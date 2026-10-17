#pragma once
#include <cstdint>
#include "config.h"

// Ring buffers: HISTORY_RECENT_N seconds of raw readings, HISTORY_HOURLY_N
// minutes of 1-minute averages. Pure C++, no Arduino dependency — RAM-only,
// resets on power cut (no persistence; see storage.* for what IS persisted).
class TdsHistory {
public:
    TdsHistory();

    // Call once per report cycle (REPORT_INTERVAL_MS) with the current ppm.
    void record(uint32_t nowMs, float ppm);

    uint16_t recentCount() const;
    uint16_t hourlyCount() const;
};
