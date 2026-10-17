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

- Chunk 2 (sensor + filter): fully wired (VCC/GND/AOUT). `sensor.cpp` and `filter.cpp` are
  real and hardware-verified across two conditions: dry-air baseline (`raw=21, v=0.006V`)
  and plain tap water (`raw≈330, v≈0.101V`) — clear, steady separation between the two.
  `filter.cpp` also host-tested (`tests/run_tests.sh` 6/6). Tap water is not a calibration
  reference (unknown TDS), so this doesn't feed `conductivity.cpp`/`calibration.cpp` yet.
  See `docs/TEST_LOG.md` for the full dated entries.

## Next

- A direct ADC-resolution sanity check (A0 jumpered to 5V) — `ADC_RESOLUTION_BITS=14` is
  requested but not independently confirmed yet. Once logged, Chunk 2 is complete.
- A real conductivity reference standard (e.g. 1413 uS/cm solution) is needed before
  `conductivity.cpp` (Chunk 3) can get real curve constants or `calibration.cpp` (Chunk 4)
  can do a real fit — not yet on hand.

## Open questions / blockers

- No datasheet for the generic TDS module — the Claude-Chat draft's curve was for a
  different module (DFRobot SEN0244) it was never told about; the pending AOUT hardware
  session must observe the real module's voltage behavior before `conductivity.cpp` gets
  real constants.
- No conductivity calibration reference solutions yet — v1 stays in `calibrated:false` mode
  until the user supplies real points.
- WiFi credentials not yet provided — `secrets.h` doesn't exist until Chunk 10; only
  `secrets.h.example` is committed.
