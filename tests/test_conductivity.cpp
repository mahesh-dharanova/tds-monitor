#include "conductivity.h"
#include <cassert>

int main() {
    // Chunk 1 stub always returns 0. This assertion is expected to be
    // REWRITTEN in Chunk 3 once the real curve + temp correction land.
    assert(conductivityFromVoltage(1.5f, 25.0f) == 0.0f);
    return 0;
}
