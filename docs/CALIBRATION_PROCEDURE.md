# Calibration Procedure

Not started — no conductivity reference solutions on hand yet (see `current_state.md`).

## When reference solutions are available

1. Put the probe in a conductivity standard (e.g. 1413 uS/cm) and let the reading settle.
2. Enter the known reference value via the dashboard or serial calibrate command (Chunk 4).
3. `calibrationAddPoint()` refuses the point if the reading wasn't steady, or if it doesn't
   fit the earlier points — repeat the measurement if rejected.
4. Repeat with a second reference point at a different concentration for a 2-point fit.
5. Update `tests/test_calibration.cpp` to assert against the fitted points, and confirm
   `calibrated` flips to `true` only once the fit is verified against a held-out sample (not
   one of the fitting points).
6. Record the fit and verification result in `TEST_LOG.md`.
