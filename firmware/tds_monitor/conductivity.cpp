#include "conductivity.h"

float conductivityFromVoltage(float filteredVolts, float temperatureC) {
    (void)filteredVolts;
    (void)temperatureC;
    // TODO(Chunk 3): provisional default curve + TEMP_COEFF_PER_C correction,
    // needs Chunk 2's hardware session to observe the generic module's real
    // dry/reference-solution voltage behavior first (no datasheet exists).
    return 0.0f;
}
