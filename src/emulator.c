#include "emulator.h"
#include <stdlib.h>

Chip8 *Chip8_init() {
  Chip8 *chip = calloc(1, sizeof(*chip));

  chip->PC = 0x200;

  return chip;
}
