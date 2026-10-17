# tds-monitor

Water TDS (Total Dissolved Solids) sensor node: Arduino Uno R4 WiFi + a generic analog
2-needle TDS module.

## Overview

v1 reads the TDS module, filters it, runs health checks, applies a 2-point calibration and
a fixed assumed-temperature correction, keeps history, and serves a dashboard over WiFi (and
over USB as a fallback) — one JSON report per second plus an HTTP API. No real temperature
probe in v1; that's v2 (see `plan.md`).

The module is unbranded/generic, not the DFRobot SEN0244 an earlier AI-generated draft
assumed — its voltage→conductivity curve is unverified until Chunk 2's hardware session.

## Goals

- v1: modular, tested Arduino Uno R4 WiFi sensor node — filtered, health-checked,
  calibration-flagged TDS readings over serial, EEPROM-persisted calibration, dashboard +
  JSON API over WiFi and USB.
- v2: add a DS18B20 temperature probe, replace the fixed assumed-temperature constant with a
  real measurement.

## Technology

- Arduino Uno R4 WiFi (`arduino:renesas_uno:unor4wifi`), Arduino core, C++.
- `arduino-cli` for compile/upload.
- Plain `g++` for host-side unit tests of the pure logic modules (no framework dependency).

## Setup

See `docs/HARDWARE_WIRING.md` for wiring and `scripts/compile.sh` / `scripts/upload.sh` for
the build workflow. Copy `firmware/tds_monitor/secrets.h.example` to
`firmware/tds_monitor/secrets.h` and fill in your WiFi credentials before Chunk 8 (network)
lands — `secrets.h` is gitignored and never committed.

## Usage

Upload `firmware/tds_monitor/tds_monitor.ino`, open a serial monitor at 115200 baud, or
browse to the dashboard address it prints once WiFi lands (Chunk 8).

## Development

Read `AGENTS.md` first — it's the authoritative, agent-agnostic rulebook for this repo.

## Testing

`tests/run_tests.sh` runs the host-side unit tests for the pure logic modules (`filter`,
`conductivity`, `calibration`, `health`, `history`, `reporting`). Hardware-in-loop
verification is recorded in `docs/TEST_LOG.md`.

## Configuration

No secrets in git. `firmware/tds_monitor/secrets.h` (WiFi SSID/password, Chunk 8) is
gitignored; `secrets.h.example` is the committed template.

## Documentation

- `plan.md` — roadmap and chunks.
- `architecture.md` — module boundaries and design decisions.
- `current_state.md` — what's done, what's next (updated per verified chunk).
- `docs/` — hardware wiring, calibration procedure, test log.
