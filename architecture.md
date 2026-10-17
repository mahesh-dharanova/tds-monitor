# Architecture

Design and structure. Edit only on a real design decision.

## Overview

Single Arduino Uno R4 WiFi sensor node. `tds_monitor.ino` is an orchestrator only; all logic
lives in single-concern modules under `firmware/tds_monitor/`. The split exists so everything
that doesn't strictly need the Arduino runtime (`filter`, `conductivity`, `calibration`,
`health`, `history`, `reporting`) can be unit-tested on the host with plain `g++` — no
emulator, no new toolchain dependency. Only `sensor.*` (analog read), `storage.*` (EEPROM),
and `network.*` (WiFiS3) touch hardware libraries and can only be verified on the physical
board.

```
sensor.cpp  --raw ADC, volts-->  filter.cpp  --smoothed volts-->  conductivity.cpp
  (200ms window,                 (spike/median                   (volts -> conductivity,
   Arduino.h only)                rejection, pure)                 provisional default curve
                                                                    + fixed assumed-temp
                                                                    correction, pure)
                                                                           |
                                                                           v
                                        health.cpp <---- independent signal ----  calibration.cpp
                                   (range/stuck/dry-probe,                  (2-point fit, TDS
                                    pure, never merged                      factor, ppm,
                                    with calibration)                       calibrated:false
                                                                             until real points)
                                                                           |
                                                                           v
                                    history.cpp (24h / 5min ring buffers, pure)
                                                                           |
                                                                           v
                                reporting.cpp (JSON builder, pure) --> storage.cpp (EEPROM,
                                                                        Arduino-only) +
                                                                        network.cpp (WiFiS3
                                                                        HTTP server + dashboard,
                                                                        Arduino-only) +
                                                                        Serial.println (USB)
```

## Components

- **`sensor`** — reads the TDS module's analog pin, converts raw ADC counts to volts using
  the board's actual ADC resolution and reference voltage. The only reading-side module with
  hardware dependence.
- **`filter`** — windowed average (cancels AC mains flicker) + spike rejection (e.g. a
  bubble). Pure function of a value stream.
- **`conductivity`** — maps filtered voltage to conductivity using a provisional default
  curve (module is generic/unbranded — curve constants are placeholders until Chunk 2
  observes real behavior), corrected to 25°C using a fixed assumed-temperature constant from
  `config.h` (v1 has no temp probe). Pure.
- **`calibration`** — applies the user's 2-point calibration and the TDS factor to turn
  conductivity into a ppm estimate. Every reading is tagged `"calibrated":false` until the
  user has supplied real calibration points; rejects calibration points taken from an
  unsteady reading or that don't fit the existing points. Pure.
- **`health`** — plausibility checks on the (filtered) voltage: out-of-range, stuck value,
  still warming up, probe likely dry/disconnected. Independent of calibration state by
  design (house rule, see below).
- **`history`** — ring buffers: 5 minutes of 1-second readings, 24 hours of 1-minute
  averages. Pure.
- **`reporting`** — builds the JSON report (serial line and HTTP response body) into a fixed
  buffer. Pure formatting, no I/O.
- **`storage`** — EEPROM persistence of calibration points, TDS factor, and alert level.
  Hardware-dependent.
- **`network`** — WiFiS3 HTTP server: serves the dashboard and a JSON API (current reading,
  history, calibrate, settings), mirrors the same commands over USB serial. Reads WiFi
  credentials from `secrets.h` (gitignored). Hardware-dependent.

## Key decisions

| Decision | Why | Date |
|---|---|---|
| Sensor node owns the full pipeline including WiFi/dashboard in v1 | User's explicit choice: v1 deliverable is the full dashboard, not a serial-only phase like turbidity-monitor | 2026-10-17 |
| `health` and `calibration` are separate modules/signals | House rule in `Embedded/CLAUDE.md`: never merge or infer one from the other | 2026-10-17 |
| v1 ships explicit `calibrated:false` default instead of a guessed calibration | Module is generic/unbranded with no datasheet; a silently-wrong ppm number is worse than an honestly-flagged default one | 2026-10-17 |
| No temperature probe in v1; fixed assumed-temperature constant in `config.h` | User's explicit instruction — DS18B20 deferred to v2 once v1 works; `conductivity`'s correction function takes a temperature value as a parameter so v2 only swaps the input source | 2026-10-17 |
| WiFi credentials in gitignored `secrets.h`, not the `.ino` | The Claude-Chat-generated draft hardcoded credentials in a committed file — violates the no-secrets-in-code rule | 2026-10-17 |
| Host-side unit tests via plain `g++`, no framework | `filter`/`conductivity`/`calibration`/`health`/`history`/`reporting` have zero Arduino dependency; g++ is already installed; avoids adding PlatformIO/Unity for no benefit | 2026-10-17 |
| Board: Arduino Uno R4 WiFi, FQBN `arduino:renesas_uno:unor4wifi` | User's hardware, same as turbidity-monitor | 2026-10-17 |

## Open hardware questions (resolve during Chunk 2, don't assume)

- Exact ADC resolution the Uno R4 WiFi actually returns via `analogReadResolution()`
  (datasheet claims up to 14-bit; turbidity-monitor's finding should carry over since it's
  the same board, but confirm).
- The generic TDS module's actual dry (in-air) and reference-solution voltage output — no
  datasheet exists, so the default curve in `conductivity.cpp` is a placeholder until this
  session happens.
- Whether the module needs a settling/warm-up delay after power-on (affects `HEALTH_WARMUP_MS`
  in `config.h`).
