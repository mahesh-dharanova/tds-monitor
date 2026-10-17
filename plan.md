# Plan

Roadmap broken into chunks. Edit only on a real scope decision.

## Goals

- v1: a modular, tested Arduino Uno R4 WiFi sensor node that reports filtered,
  health-checked, calibration-flagged TDS readings from a generic 2-needle module, over
  serial JSON and a WiFi dashboard/HTTP API, with EEPROM-persisted calibration. No
  temperature probe.
- v2: add a DS18B20 temperature probe, replace the fixed assumed-temperature constant with a
  real measurement.

## v1 (current)

1. [x] Scaffold — repo structure, docs, stub modules, git init.
   Verified: `scripts/compile.sh` succeeds on the empty orchestrator.
2. [ ] Sensor + filter modules — wire the generic TDS module (5V/GND/A0), implement
   `sensor.cpp` (raw ADC read + voltage, 200ms window) and `filter.cpp` (spike rejection).
   First real contact with the module: observe its dry and reference-solution voltage swing,
   since no datasheet exists. Confirm the Uno R4 WiFi's real ADC resolution on hardware.
   Verified: `tests/run_tests.sh` covers filter edge cases; upload + monitor, dry vs
   reference-solution readings logged in `docs/TEST_LOG.md`.
3. [ ] Conductivity module — voltage→conductivity using a provisional default curve
   (explicitly flagged unverified), fixed assumed-temperature correction (constant from
   `config.h`, swap point for v2's DS18B20).
   Verified: unit tests against the default curve's known points; a test asserting the
   temperature-correction math at the assumed constant.
4. [ ] Calibration module — 2-point calibration fit + TDS factor → ppm, always flagged
   `calibrated:false` until the user supplies real points; rejects points taken from an
   unsteady reading or that don't fit existing points.
   Verified: unit tests for the fit math, the rejection rules, and the uncalibrated-by-default
   flag.
5. [ ] Health module — range / stuck-value / warm-up / dry-probe checks, independent of
   calibration.
   Verified: one unit test per fault condition.
6. [ ] History module — 5-minute (1s) and 24-hour (1min avg) ring buffers.
   Verified: unit tests for wraparound and averaging.
7. [ ] Reporting module — JSON line builder (serial + HTTP body).
   Verified: unit tests on formatting and edge values.
8. [ ] Orchestrator wiring, serial-only — `.ino` calls sensor → filter → conductivity →
   calibration → health → history → reporting → `Serial.println`, once per second. No
   network yet — validates the whole pipeline on real hardware before adding networking
   complexity on top of still-unverified assumptions.
   Verified: compile + upload + monitor, dry and in-water session logged.
9. [ ] Storage module — EEPROM persistence of calibration points, TDS factor, alert level.
   Verified: unit test for the pure struct-packing logic where feasible; hardware session
   confirms values survive a reset.
10. [ ] Network module + dashboard — WiFiS3 HTTP server, `secrets.h`-based WiFi credentials,
    serves the dashboard (adapted from the Claude-Chat draft in `~/Downloads/TDS.zip`, with
    temperature UI elements stripped) and a JSON API, plus the same commands mirrored over
    USB serial.
    Verified: hardware session — dashboard reachable over WiFi and over USB, calibration /
    alert / history round-trip through the UI, logged in `docs/TEST_LOG.md`.
11. [ ] Integration pass — full host test suite + fresh end-to-end hardware session together,
    `current_state.md` matches reality, tag v1.

## v2 (not started)

- Add a DS18B20 temperature probe (wiring: data pin + 4.7kΩ pull-up, per the original
  Claude-Chat draft's diagram).
- Replace `conductivity.cpp`'s fixed assumed-temperature constant with a real measurement.
- Re-verify against the host tests and a fresh hardware session.
