# Hardware Wiring

## Board

Arduino Uno R4 WiFi (`arduino:renesas_uno:unor4wifi`). ADC reference 5V.

## Sensor

Generic, unbranded analog 2-needle TDS module. No manufacturer datasheet — an earlier
AI-generated draft (`~/Downloads/TDS.zip`) assumed a DFRobot SEN0244 without being told the
real hardware; that assumption does not apply here. Wiring below follows the common form
factor for this class of module (3.3–5V in, analog voltage out, 3-pin VCC/GND/AOUT) until
confirmed against the physical module in Chunk 2.

| Module pin | Uno R4 WiFi pin | Notes |
|---|---|---|
| VCC | 5V | |
| GND | GND | |
| AOUT (analog) | A0 | |

No temperature probe in v1 (see `config.h`'s `ASSUMED_TEMP_C`). A DS18B20 (data pin + 4.7kΩ
pull-up to 5V) is v2 only.

## Logic-level check

Module and board both run their analog I/O at 5V — no level-shifting needed as currently
understood. Flag immediately if the physical module turns out to be 3.3V-only (per
`Embedded/CLAUDE.md`'s standing rule to proactively flag voltage mismatches).

## To confirm in Chunk 2

- [ ] Physical pin labels and connector type on the module match this table.
- [ ] Observed dry (in-air) voltage and reference-solution (e.g. 1413 uS/cm standard) voltage,
      recorded in `TEST_LOG.md` — this is what `conductivity.cpp`'s default curve gets built
      from, since no datasheet exists.
- [ ] Actual ADC resolution returned by `analogReadResolution()` on this board.
- [ ] Whether the module needs a settling/warm-up delay after power-on.
