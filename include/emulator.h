#ifndef EMULATOR_H
#define EMULATOR_H

#include "chip8.h"

typedef struct {
  Chip8 chip;
  const char *filename;
} Emulator;

#endif // EMULATOR_H
