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

- Reconnect A0 to the TDS module's AOUT (currently jumpered to 5V for the ADC test below) to
  resume normal sensor readings. Once confirmed, Chunk 2 is complete.
- `conductivity.cpp` (Chunk 3) and `calibration.cpp` (Chunk 4) can proceed once the probe is
  in distilled water for the single calibration point (see decision below) — no second
  reference standard needed for v1.

## Open questions / blockers

- **ADC-resolution sanity check is inconclusive, not failed.** A0 jumpered straight to 5V
  read a steady ~0.96V (raw~3132-3146) instead of the expected ~5V, across two captures
  including after reseating. Arduino's own docs confirm 14-bit/5V-reference is correct
  defaults for this board, so the firmware math isn't in question — most likely a pin
  mix-up on the jumper, not confirmed without a multimeter. See `docs/TEST_LOG.md`. Does not
  block Chunk 2: the real dry-air/tap-water sensor readings are internally consistent.
- v1 calibration is **single-point** (distilled water as a near-zero/offset reference),
  confirmed by the user — a second known-value standard (e.g. 1413 uS/cm) is deferred to v2
  alongside the DS18B20. See `architecture.md`'s key-decisions table.
- No datasheet for the generic TDS module — the Claude-Chat draft's curve was for a
  different module (DFRobot SEN0244) it was never told about; `conductivity.cpp` still needs
  real curve constants derived from the dry-air/tap-water/distilled-water observations.
- WiFi credentials not yet provided — `secrets.h` doesn't exist until Chunk 10; only
  `secrets.h.example` is committed.
