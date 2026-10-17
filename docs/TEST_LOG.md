# Test Log

Dated hardware-in-loop verification entries. Never edit a past entry — append a new one.

## 2026-10-17 — Chunk 1 scaffold

- `scripts/compile.sh` succeeds against `arduino:renesas_uno:unor4wifi` with the empty
  orchestrator (`Serial.begin()` only) and all stub modules compiled alongside it.
- `tests/run_tests.sh` passes (asserts stub placeholder behavior only — not real logic).
- No hardware connected yet; no sensor wiring performed.

## 2026-10-17 — Chunk 2, in progress: first power to the module

- Module VCC -> 5V, GND -> GND wired on the Uno R4 WiFi. Module's blue power-indicator LED
  illuminates immediately — confirms the 5V supply path is correct. This is a power-good
  indicator only; it says nothing about the analog output yet.
- AOUT -> A0 not wired yet. No analog readings taken yet, so the module's dry/reference-
  solution voltage and the board's real ADC resolution remain unconfirmed (see
  "Open hardware questions" in `architecture.md`).
- `sensor.cpp` and `filter.cpp` implemented for real in this chunk (windowed ADC average +
  median-of-5/EMA smoothing). `filter.cpp` is host-tested and passing
  (`tests/run_tests.sh`). `sensor.cpp` only compiles clean so far
  (`scripts/compile.sh`) — not yet run on hardware, since AOUT isn't connected.
- Next: wire AOUT -> A0, upload, and record the dry-air raw/voltage reading here before
  touching the probe to any reference solution.
