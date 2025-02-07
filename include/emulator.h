#ifndef EMULATOR_H
#define EMULATOR_H

#include <stdint.h>

typedef struct {
  // Registers
  uint8_t V[0xf + 1];
  uint8_t sound;
  uint8_t delay;
  uint8_t memory[0xfff + 1];
  uint16_t opcode;
} Chip8;

Chip8 *Chip8_init();

#endif // EMULATOR_H
