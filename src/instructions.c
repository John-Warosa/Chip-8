#include "instructions.h"
#include "emulator.h"
#include "stdlib.h"
#include <stdio.h>
#include <string.h>

#define N(opcode) (opcode & 0x000f)
#define NN(opcode) (opcode & 0x00ff)
#define NNN(opcode) (opcode & 0x0fff)
#define X_REG(opcode) ((opcode & 0x0f00) >> 8)
#define Y_REG(opcode) ((opcode & 0x00f0) >> 4)

void OP_NULL(Chip8 *chip) { (void)chip; }

void OP_00E0(Chip8 *chip) { memset(chip->pixels, 0, sizeof(chip->pixels)); }

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

  chip->V[0xf] = 0;
  chip->V[x] |= chip->V[y];
}

void OP_8XY2(Chip8 *chip) {
  uint8_t x = X_REG(chip->opcode);
  uint8_t y = Y_REG(chip->opcode);

  chip->V[0xf] = 0;
  chip->V[x] &= chip->V[y];
}

void OP_8XY3(Chip8 *chip) {
  uint8_t x = X_REG(chip->opcode);
  uint8_t y = Y_REG(chip->opcode);

  chip->V[0xf] = 0;
  chip->V[x] ^= chip->V[y];
}

void OP_8XY4(Chip8 *chip) {
  uint8_t x = X_REG(chip->opcode);
  uint8_t y = Y_REG(chip->opcode);
  uint8_t flag = (chip->V[x] + chip->V[y]) > 0xff;

  chip->V[x] += chip->V[y];
  chip->V[0xf] = flag;
}

void OP_8XY5(Chip8 *chip) {
  uint8_t x = X_REG(chip->opcode);
  uint8_t y = Y_REG(chip->opcode);
  uint8_t flag = (chip->V[x] >= chip->V[y]);

  chip->V[x] -= chip->V[y];
  chip->V[0xf] = flag;
}

void OP_8XY6(Chip8 *chip) {
  uint8_t x = X_REG(chip->opcode);
  uint8_t y = Y_REG(chip->opcode);
  uint8_t flag = (chip->V[y] & 1u);

  chip->V[x] = chip->V[y] >> 1;
  chip->V[0xf] = flag;
  // uint8_t x = X_REG(chip->opcode);
  // uint8_t flag = (chip->V[x] & 1u);

  // chip->V[x] >>= 1;
  // chip->V[0xf] = flag;
}

void OP_8XY7(Chip8 *chip) {
  uint8_t x = X_REG(chip->opcode);
  uint8_t y = Y_REG(chip->opcode);
  uint8_t flag = (chip->V[y] >= chip->V[x]);

  chip->V[x] = chip->V[y] - chip->V[x];
  chip->V[0xf] = flag;
}

void OP_8XYE(Chip8 *chip) {
  uint8_t x = X_REG(chip->opcode);
  uint8_t y = Y_REG(chip->opcode);
  uint8_t flag = ((chip->V[y] >> 7) & 1u);

  chip->V[x] = chip->V[y] << 1;
  chip->V[0xf] = flag;
  // uint8_t x = X_REG(chip->opcode);
  // uint8_t flag = ((chip->V[x] >> 7) & 1u);

  // chip->V[x] <<= 1;
  // chip->V[0xf] = flag;
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
  uint8_t randInt = rand();

  chip->V[x] = randInt & NN(chip->opcode);
}

void OP_DXYN(Chip8 *chip) {
  uint8_t x = X_REG(chip->opcode);
  uint8_t y = Y_REG(chip->opcode);
  uint8_t height = N(chip->opcode);

  uint8_t xPos = chip->V[x] % 64;
  uint8_t yPos = chip->V[y] % 32;

  chip->V[0xf] = 0;

  for (int row = 0; row < height; ++row) {
    uint8_t byte = chip->memory[chip->I + row];

    for (int col = 0; col < 8; ++col) {
      bool memPixel = (byte >> (7 - col)) & 1u;
      bool scrPixel = chip->pixels[yPos + row][xPos + col];
      chip->pixels[yPos + row][xPos + col] ^= memPixel;

      if ((chip->V[0xf] == 0) && memPixel && scrPixel) {
        chip->V[0xf] = 1;
      }
    }
  }
}

void OP_EX9E(Chip8 *chip) {
  uint8_t x = X_REG(chip->opcode);
  uint8_t index = N(chip->V[x]);

  if (chip->keys[index]) {
    chip->PC += 2;
  }
}

void OP_EXA1(Chip8 *chip) {
  uint8_t x = X_REG(chip->opcode);
  uint8_t index = N(chip->V[x]);

  if (!chip->keys[index]) {
    chip->PC += 2;
  }
}

void OP_FX07(Chip8 *chip) {
  uint8_t x = X_REG(chip->opcode);

  chip->V[x] = chip->delay;
}

void OP_FX0A(Chip8 *chip) {
  static uint8_t lastKeys[16];

  uint8_t x = X_REG(chip->opcode);

  // If no key is pressed, result will be NULL
  uint8_t *pressedKeyLast = memchr(lastKeys, 1, 16);
  uint8_t *pressedKeyNow = memchr(chip->keys, 1, 16);

  // for (int i = 0; i < 16; ++i) {
  //   printf("chip->key[%d]: %d, lastKeys[%d]: %d\n", i, chip->keys[i], i,
  //          lastKeys[i]);
  // }

  // Need to have non-NULL ptr and NULL ptr
  // to represent the key being released
  if (pressedKeyLast && !pressedKeyNow) {
    chip->V[x] = (uint8_t)(pressedKeyLast - lastKeys);
    memset(lastKeys, 0, 16);
    return;
  }

  memcpy(lastKeys, chip->keys, 16);
  chip->PC -= 2;

  // for (int i = 0; i < 16; ++i) {
  //   if (chip->keys[i]) {
  //     chip->V[x] = i;
  //     return;
  //   }
  // }

  // chip->PC -= 2;
}

void OP_FX15(Chip8 *chip) {
  uint8_t x = X_REG(chip->opcode);

  chip->delay = chip->V[x];
}

void OP_FX18(Chip8 *chip) {
  uint8_t x = X_REG(chip->opcode);

  chip->sound = chip->V[x];
}

void OP_FX1E(Chip8 *chip) {
  uint8_t x = X_REG(chip->opcode);

  chip->I = NNN(chip->I + chip->V[x]);
}

void OP_FX29(Chip8 *chip) {
  uint8_t x = X_REG(chip->opcode);
  uint8_t num = N(chip->V[x]);

  chip->I = 0x050 + 5 * num;
}

void OP_FX33(Chip8 *chip) {
  uint8_t x = X_REG(chip->opcode);
  uint8_t val = chip->V[x];

  chip->memory[chip->I] = (val / 100) % 10;
  chip->memory[chip->I + 1] = (val / 10) % 10;
  chip->memory[chip->I + 2] = val % 10;
}

void OP_FX55(Chip8 *chip) {
  uint8_t x = X_REG(chip->opcode);

  for (int i = 0; i <= x; ++i) {
    chip->memory[chip->I++] = chip->V[i];
    // chip->memory[chip->I + i] = chip->V[i];
  }
}

void OP_FX65(Chip8 *chip) {
  uint8_t x = X_REG(chip->opcode);

  for (int i = 0; i <= x; ++i) {
    chip->V[i] = chip->memory[chip->I++];
  }
}

void execute(Chip8 *chip) {
  switch (chip->opcode >> 12) {

  case 0x0:
    switch (NNN(chip->opcode)) {
    case 0x0e0:
      OP_00E0(chip);
      break;
    case 0x0ee:
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
    if (N(chip->opcode) == 0)
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
    if (N(chip->opcode) == 0)
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
      OP_EXA1(chip);
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
