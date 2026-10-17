#pragma once
#include <cstdint>

// Named constants shared by the pure logic modules and sensor.cpp/storage.cpp/network.cpp.
// No magic numbers scattered in module bodies — change the behavior here.

// ADC
constexpr uint8_t ADC_RESOLUTION_BITS = 14;    // Uno R4 WiFi's max; confirm in Chunk 2
constexpr float   ADC_VREF_VOLTS      = 5.0f;  // module and board both run at 5V

// Timing
constexpr uint32_t REPORT_INTERVAL_MS = 1000;
constexpr uint32_t SAMPLE_WINDOW_US   = 200000UL; // 200 ms cancels 50/60 Hz mains flicker

// Filter (Chunk 2)
constexpr uint8_t FILTER_WINDOW_SIZE  = 5;
constexpr float   FILTER_SMOOTHING    = 0.4f;     // 0..1, higher = faster response

// Conductivity / temperature (Chunk 3)
// v1 has no temperature probe — this stands in for a real DS18B20 reading until v2.
// conductivity's correction function takes a temperature value as a parameter so v2 only
// needs to supply a measured value here instead of this constant.
constexpr float   ASSUMED_TEMP_C      = 25.0f;
constexpr float   TEMP_COEFF_PER_C    = 0.02f;    // conductivity changes ~2% per degC

// Calibration (Chunk 4)
constexpr float   DEFAULT_TDS_FACTOR  = 0.5f;     // ppm = conductivity * factor, user-tunable

// Health (Chunk 5)
constexpr float    HEALTH_MIN_VOLTS   = 0.0f;
constexpr float    HEALTH_MAX_VOLTS   = ADC_VREF_VOLTS;
constexpr uint32_t  HEALTH_WARMUP_MS  = 500;

// History (Chunk 6)
constexpr uint16_t HISTORY_RECENT_N = 300;   // last 5 min of 1s readings
constexpr uint16_t HISTORY_HOURLY_N = 1440;  // last 24h of 1min averages

// Storage (Chunk 9) — EEPROM byte offsets, one named constant per stored field
constexpr uint16_t EEPROM_ADDR_CAL_POINTS = 0;
constexpr uint16_t EEPROM_ADDR_TDS_FACTOR = 64;
constexpr uint16_t EEPROM_ADDR_ALERT_PPM  = 68;

// Alert (Chunk 9/10)
constexpr float DEFAULT_ALERT_PPM = 500.0f; // IS 10500:2012 acceptable limit
