#include "instructions.h"
#include "emulator.h"
#include "stdlib.h"
#include <string.h>

#define N(opcode) (opcode & 0x000f)
#define NN(opcode) (opcode & 0x00ff)
#define NNN(opcode) (opcode & 0x0fff)
#define X_REG(opcode) ((opcode & 0x0f00) >> 8)
#define Y_REG(opcode) ((opcode & 0x00f0) >> 4)
#define TABLE_INDEX(opcode) ((opcode & 0xf000) >> 12)

void OP_NULL(Chip8 *chip) { (void)chip; }

void OP_00E0(Chip8 *chip) { memset(chip->pixels, 0, 32); }

void OP_00EE(Chip8 *chip) {
  chip->PC = chip->stack[chip->SP];
  --chip->SP;
}

void OP_1NNN(Chip8 *chip) { chip->PC = NNN(chip->opcode); }

void OP_2NNN(Chip8 *chip) {
  ++chip->SP;
  chip->stack[chip->SP] = chip->PC;
  chip->PC = NNN(chip->opcode);
}

void OP_3XNN(Chip8 *chip) {
  uint8_t x = X_REG(chip->opcode);
  uint8_t val = NN(chip->opcode);

  if (chip->V[x] == val) {
    chip->PC += 2;
  }
}

void OP_4XNN(Chip8 *chip) {
  uint8_t x = X_REG(chip->opcode);
  uint8_t val = NN(chip->opcode);

  if (chip->V[x] != val) {
    chip->PC += 2;
  }
}

void OP_5XY0(Chip8 *chip) {
  uint8_t x = X_REG(chip->opcode);
  uint8_t y = Y_REG(chip->opcode);

  if (chip->V[x] == chip->V[y]) {
    chip->PC += 2;
  }
}

void OP_6XNN(Chip8 *chip) {
  uint8_t x = X_REG(chip->opcode);
  uint8_t val = NN(chip->opcode);

  chip->V[x] = val;
}

void OP_7XNN(Chip8 *chip) {
  uint8_t x = X_REG(chip->opcode);
  uint8_t val = NN(chip->opcode);

  chip->V[x] += val;
}

void OP_8XY0(Chip8 *chip) {
  uint8_t x = X_REG(chip->opcode);
  uint8_t y = Y_REG(chip->opcode);

  chip->V[x] = chip->V[y];
}

void OP_8XY1(Chip8 *chip) {
  uint8_t x = X_REG(chip->opcode);
  uint8_t y = Y_REG(chip->opcode);

  chip->V[x] |= chip->V[y];
}

void OP_8XY2(Chip8 *chip) {
  uint8_t x = X_REG(chip->opcode);
  uint8_t y = Y_REG(chip->opcode);

  chip->V[x] &= chip->V[y];
}

void OP_8XY3(Chip8 *chip) {
  uint8_t x = X_REG(chip->opcode);
  uint8_t y = Y_REG(chip->opcode);

  chip->V[x] ^= chip->V[y];
}

void OP_8XY4(Chip8 *chip) {
  uint8_t x = X_REG(chip->opcode);
  uint8_t y = Y_REG(chip->opcode);
  uint16_t sum = chip->V[x] + chip->V[y];

  chip->V[0xf] = (sum > 0xff);
  chip->V[x] = (uint8_t)sum;
}

void OP_8XY5(Chip8 *chip) {
  uint8_t x = X_REG(chip->opcode);
  uint8_t y = Y_REG(chip->opcode);

  chip->V[0xf] = (chip->V[x] > chip->V[y]);
  chip->V[x] -= chip->V[y];
}

void OP_8XY6(Chip8 *chip) {
  uint8_t x = X_REG(chip->opcode);

  chip->V[0xf] = (chip->V[x] & 1);
  chip->V[x] >>= 1;
}

void OP_8XY7(Chip8 *chip) {
  uint8_t x = X_REG(chip->opcode);
  uint8_t y = Y_REG(chip->opcode);

  chip->V[0xf] = (chip->V[y] > chip->V[x]);
  chip->V[x] = chip->V[y] - chip->V[x];
}

void OP_8XYE(Chip8 *chip) {
  uint8_t x = X_REG(chip->opcode);

  chip->V[0xf] = ((chip->V[x] >> 7) & 1);
  chip->V[x] <<= 1;
}

void OP_9XY0(Chip8 *chip) {
  uint8_t x = X_REG(chip->opcode);
  uint8_t y = Y_REG(chip->opcode);

  if (chip->V[x] != chip->V[y]) {
    chip->PC += 2;
  }
}

void OP_ANNN(Chip8 *chip) { chip->I = NNN(chip->opcode); }

void OP_BNNN(Chip8 *chip) {
  uint16_t address = NNN(chip->opcode) + chip->V[0x0];

  chip->PC = NNN(address);
}

void OP_CXNN(Chip8 *chip) {
  uint8_t x = X_REG(chip->opcode);
  uint8_t randInt = rand() % 0xff;

  chip->V[x] = randInt & NN(chip->opcode);
}

void OP_DXYN(Chip8 *chip) {
  uint8_t x = X_REG(chip->opcode);
  uint8_t y = Y_REG(chip->opcode);
  uint8_t rows = N(chip->opcode);

  for (int i = 0; i < rows; ++i) {
    chip->pixels[y + i] = 1;
  }
}

void execute(Chip8 *chip) {}
