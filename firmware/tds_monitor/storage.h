#pragma once
#include <cstdint>

// EEPROM persistence of calibration points, TDS factor, and alert level.
// Arduino-only (touches EEPROM.h) — see AGENTS.md module boundary.

void storageInit();
void storageLoadCalibration();
void storageSaveCalibration();
void storageSaveTdsFactor(float factor);
void storageSaveAlertPpm(float alertPpm);
