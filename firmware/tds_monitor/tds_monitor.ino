/*
  tds-monitor — v1 sensor node + dashboard
  Board:  Arduino Uno R4 WiFi
  Sensor: generic analog 2-needle TDS module (no datasheet — see AGENTS.md)

  This file is an orchestrator only: setup()/loop() wire the modules together.
  All logic lives in sensor.*, filter.*, conductivity.*, calibration.*,
  health.*, history.*, reporting.*, storage.*, network.* (see architecture.md).
  Full pipeline wiring lands in Chunk 8 (see plan.md); network/dashboard in
  Chunk 10.

  Chunk 1 (current): scaffold only. All modules are stubs.
*/

#include "config.h"
#include "sensor.h"

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
  Serial.print(F("{\"ms\":"));
  Serial.print(now);
  Serial.print(F(",\"raw\":"));
  Serial.print(reading.rawAdc);
  Serial.print(F(",\"v\":"));
  Serial.print(reading.volts, 3);
  Serial.println(F("}"));
}
