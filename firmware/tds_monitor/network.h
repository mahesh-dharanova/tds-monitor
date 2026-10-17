#pragma once

// WiFiS3 HTTP server: serves the dashboard and a JSON API (current reading,
// history, calibrate, settings), mirrors the same commands over USB serial.
// Arduino-only (touches WiFiS3.h) — see AGENTS.md module boundary.
// Reads credentials from secrets.h (gitignored; see secrets.h.example).

void networkInit();
void networkPoll();
