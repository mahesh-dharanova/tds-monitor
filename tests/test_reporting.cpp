#include "reporting.h"
#include <cassert>
#include <cstring>

int main() {
    char buf[256];
    const SensorReading reading{ 0, 0.0f };
    const TdsEstimate estimate{ 0.0f, false };
    // Chunk 1 stub writes an empty string and returns 0. This assertion is
    // expected to be REWRITTEN in Chunk 7 once the real JSON builder lands.
    const size_t n = formatReport(buf, sizeof(buf), 1000, reading, 0.0f,
                                   HealthStatus::Ok, estimate);
    assert(n == 0);
    assert(strlen(buf) == 0);
    return 0;
}
