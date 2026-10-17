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

## Done (cont.)

- Chunk 2 (sensor + filter): complete. Fully wired (VCC/GND/AOUT), `sensor.cpp` and
  `filter.cpp` real and hardware-verified across five conditions, lowest to highest: dry air
  (`v=0.006V`) < RO water (`v=0.079V`) < canned/bottled drinking water (`v=0.101V`, initially
  misdescribed as "tap water") < distilled water, the confirmed v1 calibration point
  (`v≈0.51V`, reproduced twice) < fresh confirmed municipal tap water (`v=0.721V`).
  `filter.cpp` also host-tested (`tests/run_tests.sh` 6/6). RO/tap/dry-air/canned-water all
  nest in the physically expected order; distilled water is the one value out of place (see
  open question below). See `docs/TEST_LOG.md` for full dated entries.

## Next

- **Paused by user decision (2026-10-17):** buying a fresh distilled water bottle and
  re-testing with it, planned for Monday (next occurrence after 2026-10-17). Chunk 3 is
  intentionally on hold until then — the current distilled-water reading (~0.51V,
  reproduced twice with the same bottle) is repeatable but still anomalously high (see open
  question below), so a fresh bottle is worth checking before it becomes the permanent v1
  calibration constant baked into `conductivity.cpp`.
- Once the fresh-bottle reading is in: Chunk 3 (`conductivity.cpp` real curve constants from
  the dry-air/distilled/tap dataset), then Chunk 4 (`calibration.cpp` applies the single
  calibration point as a near-zero/offset reference per the locked-in v1 design decision).

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
- **Distilled water's reading (~0.51V, reproduced twice: 0.508V and 0.516V) is out of place
  in the full dataset**, sitting above RO water (0.079V) and the canned-drinking-water
  sample (0.101V) when purer water should read lower, not higher. RO/tap/dry-air all nest
  correctly relative to each other, and the distilled-water reading itself is highly
  repeatable (~2% apart across two separate dips) — so this is a real property of the
  sample/container, not a sensor glitch or a one-off contamination fluke. Root cause (sample
  purity, container residue) not investigated. Repeatability is good enough for a v1
  single-point reference regardless; the "near-zero" assumption just doesn't hold for this
  particular bottle.
- No datasheet for the generic TDS module — the Claude-Chat draft's curve was for a
  different module (DFRobot SEN0244) it was never told about; `conductivity.cpp` still needs
  real curve constants derived from the dry-air/tap-water/distilled-water observations.
- WiFi credentials not yet provided — `secrets.h` doesn't exist until Chunk 10; only
  `secrets.h.example` is committed.
