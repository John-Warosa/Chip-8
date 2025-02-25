#include "romload.h"
#include <stdio.h>

void load_rom(u8 ram[], size_t start, const char *filename) {
  FILE *file = fopen(filename, "r");

  int byte;
  uint16_t index = start;

  while ((byte = fgetc(file)) != EOF) {
    ram[index++] = byte;
  }
}
