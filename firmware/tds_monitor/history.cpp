#include "history.h"

TdsHistory::TdsHistory() {}

void TdsHistory::record(uint32_t nowMs, float ppm) {
    (void)nowMs;
    (void)ppm;
    // TODO(Chunk 6): push into the 5min raw ring buffer; every 60th call,
    // fold into the 24h hourly (1min average) ring buffer.
}

uint16_t TdsHistory::recentCount() const {
    // TODO(Chunk 6): real count once the ring buffer exists.
    return 0;
}

uint16_t TdsHistory::hourlyCount() const {
    // TODO(Chunk 6): real count once the ring buffer exists.
    return 0;
}
