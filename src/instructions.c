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

  uint8_t xPos = chip->V[x] % 64;
  uint8_t yPos = chip->V[y] % 32;

  for (int i = 0; i < rows; ++i) {
    chip->pixels[yPos + i] ^= chip->memory[chip->I + i] << (63 - 7 - xPos);
  }

  // TODO: Add Vf functionality
}

void OP_EX9E(Chip8 *chip) {}

void OP_EXA1(Chip8 *chip) {}

void OP_FX07(Chip8 *chip) {}

void OP_FX0A(Chip8 *chip) {}

void OP_FX15(Chip8 *chip) {}

void OP_FX18(Chip8 *chip) {}

void OP_FX1E(Chip8 *chip) {}

void OP_FX29(Chip8 *chip) {}

void OP_FX33(Chip8 *chip) {}

void OP_FX55(Chip8 *chip) {}

void OP_FX65(Chip8 *chip) {}

void execute(Chip8 *chip) {
  switch (chip->opcode >> 12) {

  case 0x0:
    switch (N(chip->opcode)) {
    case 0x0:
      OP_00E0(chip);
      break;
    case 0xe:
      OP_00EE(chip);
      break;
    default:
      break;
    }
    break;

  case 0x1:
    OP_1NNN(chip);
    break;

  case 0x2:
    OP_2NNN(chip);
    break;

  case 0x3:
    OP_3XNN(chip);
    break;

  case 0x4:
    OP_4XNN(chip);
    break;

  case 0x5:
    OP_5XY0(chip);
    break;

  case 0x6:
    OP_6XNN(chip);
    break;

  case 0x7:
    OP_7XNN(chip);
    break;

  case 0x8:
    switch (N(chip->opcode)) {
    case 0x0:
      OP_8XY0(chip);
      break;
    case 0x1:
      OP_8XY1(chip);
      break;
    case 0x2:
      OP_8XY2(chip);
      break;
    case 0x3:
      OP_8XY3(chip);
      break;
    case 0x4:
      OP_8XY4(chip);
      break;
    case 0x5:
      OP_8XY5(chip);
      break;
    case 0x6:
      OP_8XY6(chip);
      break;
    case 0x7:
      OP_8XY7(chip);
      break;
    case 0xe:
      OP_8XYE(chip);
      break;
    default:
      break;
    }
    break;

  case 0x9:
    OP_9XY0(chip);
    break;

  case 0xa:
    OP_ANNN(chip);
    break;

  case 0xb:
    OP_BNNN(chip);
    break;

  case 0xc:
    OP_CXNN(chip);
    break;

  case 0xd:
    OP_DXYN(chip);
    break;

  case 0xe:
    switch (N(chip->opcode)) {
    case 0x1:
      OP_EX9E(chip);
      break;
    case 0xe:
      OP_EX9E(chip);
      break;
    default:
      break;
    }
    break;

  case 0xf:
    switch (NN(chip->opcode)) {
    case 0x07:
      OP_FX07(chip);
      break;
    case 0x0a:
      OP_FX0A(chip);
      break;
    case 0x15:
      OP_FX15(chip);
      break;
    case 0x18:
      OP_FX18(chip);
      break;
    case 0x1e:
      OP_FX1E(chip);
      break;
    case 0x29:
      OP_FX29(chip);
      break;
    case 0x33:
      OP_FX33(chip);
      break;
    case 0x55:
      OP_FX55(chip);
      break;
    case 0x65:
      OP_FX65(chip);
      break;
    default:
      break;
    }
    break;

  default:
    break;
  }
}
