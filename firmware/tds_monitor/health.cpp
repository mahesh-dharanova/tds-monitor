#include "health.h"

HealthMonitor::HealthMonitor() {}

HealthStatus HealthMonitor::check(float filteredVolts, uint32_t nowMs) {
    (void)filteredVolts;
    (void)nowMs;
    // TODO(Chunk 5): range / stuck-value / warm-up / dry-probe checks.
    return HealthStatus::Ok;
}
