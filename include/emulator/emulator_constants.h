#ifndef EMULATOR_CONSTANTS_H
#define EMULATOR_CONSTANTS_H

#include "timer.h"

enum {
  STEPS_PER_SECOND = 660,
  STEPS_PER_FRAME = 11,
  DEFAULT_QUIRKS = 0b00001111,
  MAX_FILE_LENGTH = 1024,
};

// Indices for the quirks in the quirks bitarray
// Used for instructions to check whether a quirk is set
enum Quirks {
  QUIRK_RESET = 0,
  QUIRK_MEMORY = 1,
  QUIRK_WAIT = 2,
  QUIRK_CLIPPING = 3,
  QUIRK_SHIFTING = 4,
  QUIRK_JUMPING = 5,
};

static const milli STEP_TIME = 1000. / STEPS_PER_SECOND;

#endif // EMULATOR_CONSTANTS_H
