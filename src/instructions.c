#include "instructions.h"
#include "emulator.h"
#include <string.h>

#define N(opcode) (opcode & 0x000f)
#define NN(opcode) (opcode & 0x00ff)
#define NNN(opcode) (opcode & 0x0fff)
#define X_REG(opcode) ((opcode & 0x0f00) >> 8)
#define Y_REG(opcode) ((opcode & 0x00f0) >> 4)

typedef void (*instruction)(Chip8 *chip);

// NOP used for invalid opcodes
static void OP_NULL(Chip8 *chip) { (void)chip; }

// Clear the display
// Set pixel buffer to 0
static void OP_00E0(Chip8 *chip) { memset(chip->pixels, 0, 32); }

// Return from subroutine
// Set PC to top of stack, decrement SP
static void OP_00EE(Chip8 *chip) {
  chip->PC = chip->stack[chip->SP];
  --chip->SP;
}

// Jump to location NNN
// Set PC to NNN
static void OP_1NNN(Chip8 *chip) { chip->PC = NNN(chip->opcode); }

// Call subroutine at NNN
// Increment SP, put PC on top of stack, set PC to NNN
static void OP_2NNN(Chip8 *chip) {
  ++chip->SP;
  chip->stack[chip->SP] = chip->PC;
  chip->PC = NNN(chip->opcode);
}

// Skip instruction if Vx = NN
// Increment PC by 2 if equal
static void OP_3XNN(Chip8 *chip) {
  uint8_t xReg = X_REG(chip->opcode);
  uint8_t val = NN(chip->opcode);

  if (chip->V[xReg] == val) {
    chip->PC += 2;
  }
}

// Skip instruction if Vx != NN
// Increment PC by 2 if not equal
static void OP_4XNN(Chip8 *chip) {
  uint8_t xReg = X_REG(chip->opcode);
  uint8_t val = NN(chip->opcode);

  if (chip->V[xReg] != val) {
    chip->PC += 2;
  }
}

// Skip instruction if Vx == Vy
// Increment PC by 2 if equal
static void OP_5XY0(Chip8 *chip) {
  uint8_t xReg = X_REG(chip->opcode);
  uint8_t yReg = Y_REG(chip->opcode);

  if (chip->V[xReg] == chip->V[yReg]) {
    chip->PC += 2;
  }
}

// Set Vx = NN
static void OP_6XNN(Chip8 *chip) {
  uint8_t xReg = X_REG(chip->opcode);
  uint8_t val = NN(chip->opcode);

  chip->V[xReg] = val;
}

// Set Vx to Vx + NN
static void OP_7XNN(Chip8 *chip) {
  uint8_t xReg = X_REG(chip->opcode);
  uint8_t val = NN(chip->opcode);

  chip->V[xReg] += val;
}

// Set I = NNN
static void OP_ANNN(Chip8 *chip) { chip->I = NNN(chip->opcode); }

// Draw sprite at (Vx, Vy)
// Get N bytes from memory location I
// Sprites are 8 pixels wide, N pixels tall
// Pixels in buffer get XOR-ed with sprite pixels
static void OP_DXYN(Chip8 *chip) {}

static instruction instructionTable[0x10] = {
    [0x1] = &OP_1NNN, [0x2] = &OP_2NNN, [0x3] = &OP_3XNN, [0x4] = &OP_4XNN,
    [0x5] = &OP_5XY0, [0x6] = &OP_6XNN, [0x7] = &OP_7XNN,
};

static instruction instructionTable0[0x10] = {
    [0x0 ... 0xf] = &OP_NULL,
    [0x0] = &OP_00E0,
    [0xe] = &OP_00EE,
};

static instruction instructionTable8[0x100] = {
    [0x00 ... 0xff] = &OP_NULL,
};

static instruction instructionTableE[0x10] = {
    [0x0 ... 0xf] = &OP_NULL,
};

static instruction instructionTableF[0x10] = {
    [0x0 ... 0xf] = &OP_NULL,
};
