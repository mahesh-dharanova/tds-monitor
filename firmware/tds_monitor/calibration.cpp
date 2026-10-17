#include "calibration.h"

TdsEstimate calibrationEstimate(float conductivityUScm) {
    (void)conductivityUScm;
    // TODO(Chunk 4): apply calibration points + DEFAULT_TDS_FACTOR.
    // Stub stays uncalibrated, as it must until real points exist.
    return TdsEstimate{ 0.0f, false };
}

bool calibrationAddPoint(float conductivityUScm, float referenceUScm, bool readingWasSteady) {
    (void)conductivityUScm;
    (void)referenceUScm;
    (void)readingWasSteady;
    // TODO(Chunk 4): store the point if steady and it fits existing points.
    return false;
}
