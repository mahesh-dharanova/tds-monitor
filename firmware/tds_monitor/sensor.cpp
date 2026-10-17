#include "sensor.h"

void sensorInit() {
    // TODO(Chunk 2): analogReadResolution(ADC_RESOLUTION_BITS), pinMode(A0, INPUT).
}

SensorReading sensorRead() {
    // TODO(Chunk 2): windowed analogRead() average over SAMPLE_WINDOW_US, needs the
    // physical TDS module wired and the board's real ADC resolution confirmed.
    return SensorReading{ 0, 0.0f };
}
