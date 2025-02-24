#include "romload.h"
#include <stdio.h>

void load_rom(Chip8 *chip, const char *filename) {
  FILE *file = fopen(filename, "r");

  int byte;
  uint16_t index = chip->PC;

  while ((byte = fgetc(file)) != EOF) {
    chip->memory[index++] = byte;
  }
}
