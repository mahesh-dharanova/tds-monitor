# Current State

Updated once per chunk, right after it is built and verified.
This is a pointer into git history for the next agent, not a duplicate of it.

## Done

- Chunk 1 (scaffold): repo structure, `AGENTS.md`/`README.md`/`plan.md`/`architecture.md`,
  stub modules (`sensor`, `filter`, `conductivity`, `calibration`, `health`, `history`,
  `reporting`, `storage`, `network`) with signatures only, `tests/run_tests.sh` harness,
  `scripts/compile.sh`/`upload.sh`, doc stubs, `secrets.h.example`, dashboard source
  (`dashboard.html`, `make_gz_header.py`) copied in from the Claude-Chat draft but not yet
  wired up. Verified: `scripts/compile.sh` compiles the empty orchestrator against
  `arduino:renesas_uno:unor4wifi`; `tests/run_tests.sh` runs (no real assertions yet since
  modules are stubs).

## In progress

- Chunk 2 (sensor + filter): module's VCC/GND wired, blue power LED confirms the 5V supply
  path. AOUT -> A0 not wired yet — no analog readings taken. `filter.cpp` is implemented for
  real (median-of-5 + EMA) and host-tested (`tests/run_tests.sh` 6/6). `sensor.cpp` is
  implemented for real (windowed ADC average) and compiles (`scripts/compile.sh`), but is
  **not yet hardware-verified** — that needs AOUT wired and a real upload+monitor session.
  See `docs/TEST_LOG.md` for the dated entries.

## Next

- Wire AOUT -> A0, upload, and record the dry-air (and ideally reference-solution) raw/volts
  reading in `docs/TEST_LOG.md`. This also confirms the Uno R4 WiFi's real ADC resolution —
  once done, Chunk 2 is complete and `conductivity.cpp` (Chunk 3) can get real curve
  constants instead of the current placeholder.

## Open questions / blockers

- No datasheet for the generic TDS module — the Claude-Chat draft's curve was for a
  different module (DFRobot SEN0244) it was never told about; the pending AOUT hardware
  session must observe the real module's voltage behavior before `conductivity.cpp` gets
  real constants.
- No conductivity calibration reference solutions yet — v1 stays in `calibrated:false` mode
  until the user supplies real points.
- WiFi credentials not yet provided — `secrets.h` doesn't exist until Chunk 10; only
  `secrets.h.example` is committed.
