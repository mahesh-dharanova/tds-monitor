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

## 2026-10-17 — Chunk 2: first real reading, dry air baseline

- AOUT -> A0 wired. `scripts/upload.sh /dev/ttyACM0` succeeded; monitored at 115200 baud.
- Probe confirmed dry (open air, needles untouched) at capture time.
- Reading, steady across 6+ consecutive seconds:
  ```
  {"ms":13000,"raw":21,"v":0.006,"v_filtered":0.006}
  ```
  `raw` computed against the requested 14-bit resolution (`ADC_RESOLUTION_BITS=14` in
  `config.h`, max count 16383); `v = raw * 5.0 / 16383`, matches the printed value.
- Dry-air baseline is ~0 V and rock steady, as expected for an open (non-conducting) probe
  gap. `filter.cpp`'s median+EMA output equals the raw value here since the signal isn't
  noisy enough to exercise the smoothing yet.
- Still open: a reference-solution (or tap-water) reading, to see the module respond to an
  actual conductive path; and a direct ADC-resolution sanity check (e.g. A0 jumpered to 5V)
  — `ADC_RESOLUTION_BITS=14` is requested but not yet independently confirmed against this
  board's real behavior.

## 2026-10-17 — Chunk 2: plain tap water

- Probe fully submerged in plain tap water (source/composition otherwise unknown — not a
  calibration-grade reference, just a real conductive sample).
- Reading, steady over 12+ consecutive seconds:
  ```
  {"ms":681000,"raw":331,"v":0.101,"v_filtered":0.101}
  ```
  (climbed smoothly from raw=329/v=0.100 to raw=331/v=0.101 over the window — consistent
  with settling, not noise.)
- Clear separation from the dry-air baseline (raw=21, v=0.006V): the module visibly responds
  to a real conductive path. `sensor.cpp` + `filter.cpp` are now confirmed working on
  hardware across two distinct conditions.
- **Not a calibration point** — tap water's actual TDS/conductivity is unknown, so this
  can't feed `conductivity.cpp`'s curve constants (Chunk 3) or `calibration.cpp`'s fit
  (Chunk 4). Those need a real reference standard (e.g. a 1413 uS/cm solution) — not yet on
  hand.
- Still open: the A0-jumpered-to-5V ADC-resolution sanity check.

## 2026-10-17 — Chunk 2: ADC-resolution sanity check, inconclusive

- A0 jumpered directly to 5V (TDS module's AOUT lead left floating in air, VCC/GND
  unchanged). Confirmed via Arduino's own docs that the default analog reference on this
  board is 5V and 14-bit mode is genuinely 0-16383 (`ADC_RESOLUTION_BITS=14` in `config.h`
  is the right call per spec) — [Arduino UNO R4 WiFi ADC Resolution](https://docs.arduino.cc/tutorials/uno-r4-wifi/adc-resolution),
  [Cheat Sheet](https://docs.arduino.cc/tutorials/uno-r4-wifi/cheat-sheet).
- Expected: `raw` near 16383 (~5V). Got instead, rock steady across two separate captures
  (including after reseating the jumper): `raw=3132-3146` (~0.956-0.960V).
- A reading this steady argues against a loose/floating connection (those are typically
  noisy) — more likely a pin mix-up (e.g. 5V vs. a neighboring pin) than a bad contact, but
  not confirmed. No multimeter available to verify the physical voltage at A0 independently.
- **Left unresolved rather than guessed at.** Does not block Chunk 2's main result: the real
  dry-air (raw=21) and tap-water (raw≈330) sensor readings are internally consistent and
  already validate `sensor.cpp`/`filter.cpp` on hardware. Revisit with a multimeter if one
  becomes available.
- A0 needs to be reconnected to the TDS module's AOUT to resume normal sensor readings
  (not yet confirmed done as of this entry).
