/*
  tds-monitor — v1 sensor node + dashboard
  Board:  Arduino Uno R4 WiFi
  Sensor: generic analog 2-needle TDS module (no datasheet — see AGENTS.md)

  This file is an orchestrator only: setup()/loop() wire the modules together.
  All logic lives in sensor.*, filter.*, conductivity.*, calibration.*,
  health.*, history.*, reporting.*, storage.*, network.* (see architecture.md).
  Full pipeline wiring (conductivity -> calibration -> health -> history ->
  reporting) lands later per plan.md; this chunk only wires sensor -> filter
  directly to serial, so both can be verified against real hardware.

  Chunk 2 (current): sensor.* and filter.* are real. Remaining modules are
  still stubs.
*/

#include "config.h"
#include "sensor.h"
#include "filter.h"

TdsFilter filter;

void setup() {
  Serial.begin(115200);
  sensorInit();
}

void loop() {
  static uint32_t nextReportMs = 0;
  const uint32_t now = millis();
  if (now < nextReportMs) return;
  nextReportMs = now + REPORT_INTERVAL_MS;

  const SensorReading reading = sensorRead();
  const float filteredVolts = filter.update(reading.volts);
  Serial.print(F("{\"ms\":"));
  Serial.print(now);
  Serial.print(F(",\"raw\":"));
  Serial.print(reading.rawAdc);
  Serial.print(F(",\"v\":"));
  Serial.print(reading.volts, 3);
  Serial.print(F(",\"v_filtered\":"));
  Serial.print(filteredVolts, 3);
  Serial.println(F("}"));
}
