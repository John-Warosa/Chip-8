#ifndef EMULATOR_H
#define EMULATOR_H

#include <stdint.h>

typedef struct {
  // Registers
  // V0 to Vf, sound and delay timers
  uint8_t V[16];
  uint8_t sound;
  uint8_t delay;
  uint16_t I;

  // Pointers
  // Stack pointer, Program counter
  uint8_t SP;
  uint16_t PC;

  // Memory
  // RAM (4KB), Stack (16 addresses)
  uint8_t memory[0x1000];
  uint16_t stack[16];

  // Keyboard input (16 keys)
  uint16_t keyboard;

  // pixel buffer (32 lines of 64 pixels)
  uint64_t pixels[32];

  // current opcode used
  uint16_t opcode;
} Chip8;

Chip8 *Chip8_init(const char *filename);

void Emulator_loop(Chip8 *chip);

#endif // EMULATOR_H
