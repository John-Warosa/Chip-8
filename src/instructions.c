#include "instructions.h"
#include "emulator.h"
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
  uint8_t xReg = X_REG(chip->opcode);
  uint8_t val = NN(chip->opcode);

  if (chip->V[xReg] == val) {
    chip->PC += 2;
  }
}

void OP_4XNN(Chip8 *chip) {
  uint8_t xReg = X_REG(chip->opcode);
  uint8_t val = NN(chip->opcode);

  if (chip->V[xReg] != val) {
    chip->PC += 2;
  }
}

void OP_5XY0(Chip8 *chip) {
  uint8_t xReg = X_REG(chip->opcode);
  uint8_t yReg = Y_REG(chip->opcode);

  if (chip->V[xReg] == chip->V[yReg]) {
    chip->PC += 2;
  }
}

void OP_6XNN(Chip8 *chip) {
  uint8_t xReg = X_REG(chip->opcode);
  uint8_t val = NN(chip->opcode);

  chip->V[xReg] = val;
}

void OP_7XNN(Chip8 *chip) {
  uint8_t xReg = X_REG(chip->opcode);
  uint8_t val = NN(chip->opcode);

  chip->V[xReg] += val;
}

void OP_ANNN(Chip8 *chip) { chip->I = NNN(chip->opcode); }

void OP_DXYN(Chip8 *chip) {}

void execute(Chip8 *chip) {}
