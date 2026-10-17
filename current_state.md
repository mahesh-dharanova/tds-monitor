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

## Next

- Chunk 2: wire the generic TDS module to the board, implement `sensor.cpp`/`filter.cpp`,
  confirm real ADC resolution on hardware, log a dry/reference-solution session in
  `docs/TEST_LOG.md`. Needs the physical board + module in hand.

## Open questions / blockers

- No datasheet for the generic TDS module — the Claude-Chat draft's curve was for a
  different module (DFRobot SEN0244) it was never told about; Chunk 2 must observe the real
  module's voltage behavior before `conductivity.cpp` gets real constants.
- No conductivity calibration reference solutions yet — v1 stays in `calibrated:false` mode
  until the user supplies real points.
- WiFi credentials not yet provided — `secrets.h` doesn't exist until Chunk 10; only
  `secrets.h.example` is committed.
