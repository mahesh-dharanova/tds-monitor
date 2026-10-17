#include "health.h"
#include <cassert>

int main() {
    HealthMonitor health;
    // Chunk 1 stub always reports Ok. This assertion is expected to be
    // REWRITTEN in Chunk 5 once the real checks land.
    assert(health.check(2.5f, 1000) == HealthStatus::Ok);
    return 0;
}
