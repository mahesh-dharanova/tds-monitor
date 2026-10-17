# Calibration Procedure

Not started — no conductivity reference solutions on hand yet (see `current_state.md`).

## Sourcing a reference standard (researched 2026-10-17, not yet purchased)

Distilled/deionized water is **not** a calibration standard by itself — its real conductivity
is near-zero but not a defined value (typically ~0.5–5 uS/cm depending on CO2 absorption and
purity), so it's only useful as a rough near-zero sanity check, the same role the dry-air
reading already plays (see `TEST_LOG.md`). A real calibration point needs a solution with a
manufacturer-stated, traceable conductivity or TDS value.

Two common families, both workable with `calibration.cpp`'s 2-point fit:

- **Conductivity standards (uS/cm, usually KCl-based)** — 84 / 1413 / 12880 uS/cm are the
  common values; 1413 uS/cm is the general-purpose one and lands close to our v1 alert
  threshold (500 ppm per IS 10500:2012, roughly 700 ppm at a 0.5 NaCl factor).
- **TDS standards (ppm, usually NaCl-based)** — e.g. HM Digital's 342 ppm (~700 uS,
  drinking-water range) and 1000 ppm (beverage/aquarium range) solutions. The conversion
  factor must match what `calibration.cpp` ends up using (NaCl vs. KCl vs. "442" give
  different ppm for the same real conductivity) — pick one family and stay consistent.

India-specific sellers found by search (verify stock/price before ordering, not confirmed
by hand):

- **Rakiro Biotech** (Navi Mumbai) — 1413 uS/cm standard, 100 ml and 500 ml bottles with a
  test report; sales@rakiro.net, +91-022-47804040.
- **Labfriend / Bishops Scientific** — Water-i.d. 1413 uS/cm, pack of 25×20ml sachets,
  listed "Available Soon" at last check.
- **HiMedia (India site)** — 1413 uS/cm standard (SKU R071) listed but shown out of stock.

No Indian seller for the HM Digital 342 ppm NaCl solution turned up in this search; check
HM Digital's own distributor list or Amazon.in directly if that specific product/factor is
wanted.

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
