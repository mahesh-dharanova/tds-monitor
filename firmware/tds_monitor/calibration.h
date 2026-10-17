#pragma once

// Applies the user's 2-point calibration and TDS factor to turn conductivity
// into a ppm estimate. v1 has no reference data until the user supplies real
// calibration points, so `calibrated` starts false — see AGENTS.md "Never:
// claiming a calibrated reading without real reference data" and
// docs/CALIBRATION_PROCEDURE.md.
struct TdsEstimate {
    float ppm;
    bool  calibrated;
};

TdsEstimate calibrationEstimate(float conductivityUScm);

// Adds one calibration point (conductivity reading against a known reference
// standard, e.g. 1413 uS/cm). Returns false and leaves calibration unchanged
// if the reading wasn't steady or doesn't fit the existing points.
bool calibrationAddPoint(float conductivityUScm, float referenceUScm, bool readingWasSteady);
