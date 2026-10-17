#pragma once

// Maps a filtered sensor voltage to conductivity (uS/cm) using a provisional
// default curve (module is generic/unbranded — see AGENTS.md "Hard-won
// facts"), corrected to 25C. Pure C++, no Arduino dependency.
//
// temperatureC is a parameter, not read from a sensor here: v1 passes
// ASSUMED_TEMP_C from config.h; v2 passes a real DS18B20 reading instead.
// This keeps the correction math itself unchanged between v1 and v2.
float conductivityFromVoltage(float filteredVolts, float temperatureC);
