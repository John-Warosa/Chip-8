#ifndef EMULATOR_H
#define EMULATOR_H

#include "chip8.h"
#include "emulator/emulator_constants.h"
#include "types.h"
#include <stdbool.h>

typedef struct {
  Chip8 chip;
  u16 quirks;
  char filename[MAX_FILE_LENGTH];
  bool running;
} Emulator;

void Emulator_init(Emulator *emu, const char *filename);

void Emulator_loop(Emulator *emu);

#endif // EMULATOR_H
