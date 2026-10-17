#include "storage.h"

// TODO(Chunk 9): #include <EEPROM.h>, read/write at the EEPROM_ADDR_* offsets
// from config.h. Needs a hardware session to confirm values survive a reset.

void storageInit() {}
void storageLoadCalibration() {}
void storageSaveCalibration() {}
void storageSaveTdsFactor(float factor) { (void)factor; }
void storageSaveAlertPpm(float alertPpm) { (void)alertPpm; }
