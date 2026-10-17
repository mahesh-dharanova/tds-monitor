# AGENTS.md

Rules for any agent working in this repo (Claude Code, Codex, OpenCode with a local model,
others). Work here may switch agents mid-task: everything needed to continue lives in this
repo, never in an agent's private memory. `CLAUDE.md` in this repo just points here.

tds-monitor is a single Arduino Uno R4 WiFi sensor node (generic analog 2-needle TDS
module). v1 scope: filtered, health-checked, calibration-flagged TDS readings, served over
serial JSON **and** a WiFi dashboard/HTTP API, with EEPROM-persisted calibration — but no
real temperature probe (fixed assumed-temperature constant; a DS18B20 is v2 only). See
`plan.md` for the full roadmap and `architecture.md` for why the code is split the way it is.

## If you are a small model (1.5B to 3B, 16k context)

- One chunk per session. Read only `current_state.md` and the one module you're touching.
- Copy an existing module's pattern (e.g. a new pure-logic module: copy `filter.h`/`filter.cpp`
  and its test) instead of inventing a new shape.
- Unsure? Stop and say exactly what you do not understand. Never guess a fix.

## Where the truth lives

- Progress: `git log --oneline -15`, then `current_state.md`.
- Roadmap: `plan.md` (stable; only changes on a real scope decision).
- Design/module boundaries: `architecture.md` (stable; only changes on a real design decision).
- Hardware-in-loop results: `docs/TEST_LOG.md` (dated entries, never overwritten, written the
  same session the data was captured — not reconstructed later from memory).
- Do not create HANDOFF or status files. Continuity is `current_state.md` + the commits.

## Documentation model (three tiers — see `~/Dev/CLAUDE.md`'s workspace conventions)

1. **This repo** — the engineering source of truth. `docs/TEST_LOG.md` gets a dated entry
   the same session as every hardware trial (readings, observations, pass/fail), whether or
   not it's the outcome hoped for. `current_state.md` updates in the same commit as every
   verified chunk. Nothing here waits for the project to finish.
2. **`mahesh-dharanova/publications`** (middle tier, informal) — once a chunk produces real
   experimental data worth a write-up (starting around Chunk 2's hardware session), draft it
   there under a new topic folder, in the three house styles (`templates/dharanova-strict`,
   `templates/academic-imrad`, `templates/dharanova-narrative`). See that repo's own
   `README.md`/`templates/README.md`.
3. **`dharanova-code/publications`** (official) — only the `dharanova-strict` draft moves
   here, only after v1 is fully tested and demoed, per that repo's `PUBLISHING_GUIDELINES.md`.
   Never fabricate a number; every figure traces to a file in `data/` or is labelled
   calculated/simulated/datasheet.

## Module boundary (the one rule that matters most here)

`firmware/tds_monitor/tds_monitor.ino` is an orchestrator ONLY: `setup()`/`loop()` calling
into modules, no logic of its own. Every module is one concern:

- `sensor.*`, `storage.*`, `network.*` — the only modules allowed to touch `Arduino.h` /
  hardware libraries (`analogRead()`, `EEPROM`, `WiFiS3`).
- `filter.*`, `conductivity.*`, `calibration.*`, `health.*`, `history.*`, `reporting.*` —
  plain C++, no `Arduino.h`, so they compile and run as real unit tests on the host with
  `g++` (`tests/run_tests.sh`).
- `health` (is the sensor behaving plausibly?) and `calibration` (do we trust the ppm
  mapping?) are separate signals. Never merge them or infer one from the other — a sensor
  can be healthy but uncalibrated, or calibrated but currently out of range (e.g. dry probe).

## Commands (repo root)

- Host unit tests (fast, no hardware): `tests/run_tests.sh`
- Compile for the board (no upload): `scripts/compile.sh`
- Upload (board connected): `scripts/upload.sh /dev/ttyACMx`
- Monitor: `arduino-cli monitor -p /dev/ttyACMx --config baudrate=115200`

## Chunk protocol

1. Say the goal and how you will verify it, in two lines.
2. Bug: write the failing test first. Feature: its test comes in the same chunk.
3. Smallest change that works, in the style of the surrounding code. No drive-by edits.
4. Verify: `tests/run_tests.sh` for pure modules; `scripts/compile.sh` always; hardware
   session + `docs/TEST_LOG.md` entry for anything touching `sensor.*`, `storage.*`,
   `network.*`, or the `.ino`.
5. Update `current_state.md` in the same commit: what's done, what's next.
6. Commit after every verified chunk (message = what changed, how verified, what's open).
   Pushing needs the user's explicit go-ahead the first time a remote exists.

## Never

- Secrets in files. WiFi credentials live in `firmware/tds_monitor/secrets.h`, which is
  gitignored — `secrets.h.example` is the committed template. Never hardcode SSID/password
  in the `.ino` or any committed file.
- Hardcoded machine values (paths, usernames). Pin numbers, EEPROM addresses, and board
  constants belong in `config.h` as named constants, not scattered literals.
- Claiming a calibrated reading without real reference data. Output is always tagged
  `"calibrated":false` until the user supplies real calibration points through the
  calibration flow (dashboard or serial command).
- Weakening or deleting a test to get green. If a host test fails, the module is wrong.
- `git push`, `reset`, `rebase`, force anything, without the user's explicit approval.

## Too big or risky for a small model: stop and say so

Any change to the conductivity curve's shape (not just its constants), ADC resolution
assumptions, anything that would make `sensor.*` do more than read and convert, anything
touching WiFi credential handling, and anything needing hardware you don't have physical
access to verify.

## Hard-won facts

- The TDS module is a **generic, unbranded** 2-needle analog probe — not the DFRobot SEN0244
  an earlier AI-generated draft (`~/Downloads/TDS.zip`) assumed without being told the real
  hardware. Its voltage→conductivity curve is unverified; `conductivity.cpp` uses a
  provisional default curve explicitly flagged as unverified until Chunk 2's hardware
  session observes the module's real behavior (see `docs/HARDWARE_WIRING.md`).
- Uno R4 WiFi's exact usable ADC resolution was unverified at project start (datasheet
  claims up to 14-bit); confirmed value goes in `architecture.md` once Chunk 2 is done.
- v1 has no temperature probe. `conductivity.cpp` takes an assumed-temperature constant
  (`config.h`) instead of a measurement — the function signature is written so v2 only needs
  to swap that constant for a real DS18B20 reading, not change the correction math.
