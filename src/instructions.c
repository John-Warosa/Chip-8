#include "instructions.h"
#include "chip8_constants.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*===================================================================

Opcodes

Each opcode is a 2 Byte unsigned integer of the form N0_N1_N2_N3,
where Nx represents the nibbles of the opcode (nibble = 4 bits)
The nibbles are used as follows:

  - N0 represents the (type of) instruction
    N0 may be unique or be the same for different instructions
    The following numbers are not unique:
      * 0
      * 8
      * e
      * f

  - the remaining 3 nibbles usually appear in the following forms:
      * XYK - The instruction uses registers X and Y
      * XNN - The instructuon uses register X and value NN
      * NNN - The instruction uses the address NNN

For more information on the instructions and different opcodes,
see Cowgod's reference manual

Below are some macros to determine the most commonly required
nibbles for decoding the opcodes

===================================================================*/

#define N(opcode) ((opcode) & 0x000f)
#define NN(opcode) ((opcode) & 0x00ff)
#define NNN(opcode) ((opcode) & 0x0fff)
#define X_REG(opcode) (((opcode) & 0x0f00) >> 8)
#define Y_REG(opcode) (((opcode) & 0x00f0) >> 4)
#define CHECK_BIT(num, index) ((num) & (1u << (index)))

void execute_instruction(Chip8 *chip) {
  switch (chip->opcode & 0xf000) {

  // Two valid opcodes: 00E0 and 00EE
  case 0x0000:
    switch (chip->opcode & 0xffff) {

    case 0x00e0:
      memset(chip->pixels, 0, sizeof(chip->pixels));
      break;

    case 0x00ee:
      chip->PC = chip->stack[chip->SP];
      --chip->SP;
      break;
    }
    break;

  case 0x1000:
    chip->PC = NNN(chip->opcode);
    break;

  case 0x2000:
    ++chip->SP;
    chip->stack[chip->SP] = chip->PC;
    chip->PC = NNN(chip->opcode);
    break;

  case 0x3000: {
    u8 x = X_REG(chip->opcode);
    u8 val = NN(chip->opcode);

    if (chip->V[x] == val) {
      chip->PC += 2;
    }
  } break;

  case 0x4000: {
    u8 x = X_REG(chip->opcode);
    u8 val = NN(chip->opcode);

    if (chip->V[x] != val) {
      chip->PC += 2;
    }
  } break;

  // Opcode is 5XY0, so need to check last nibble
  case 0x5000: {
    if (N(chip->opcode) != 0)
      break;

    u8 x = X_REG(chip->opcode);
    u8 y = Y_REG(chip->opcode);

    if (chip->V[x] == chip->V[y]) {
      chip->PC += 2;
    }
  } break;

  case 0x6000: {
    u8 x = X_REG(chip->opcode);
    u8 val = NN(chip->opcode);

    chip->V[x] = val;
  } break;

  case 0x7000: {
    u8 x = X_REG(chip->opcode);
    u8 val = NN(chip->opcode);

    chip->V[x] += val;
  } break;

  // Valid opcodes:
  // 8XY0, 8XY1, 8XY2, 8XY3, 8XY4,
  // 8XY5, 8XY6, 8XY7, 8XYE
  case 0x8000:
    switch (chip->opcode & 0xf00f) {

    case 0x8000: {
      u8 x = X_REG(chip->opcode);
      u8 y = Y_REG(chip->opcode);

      chip->V[x] = chip->V[y];
    } break;

    case 0x8001: {
      u8 x = X_REG(chip->opcode);
      u8 y = Y_REG(chip->opcode);

      chip->V[x] |= chip->V[y];
    } break;

    case 0x8002: {
      u8 x = X_REG(chip->opcode);
      u8 y = Y_REG(chip->opcode);

      chip->V[x] &= chip->V[y];
    } break;

    case 0x8003: {
      u8 x = X_REG(chip->opcode);
      u8 y = Y_REG(chip->opcode);

      chip->V[x] ^= chip->V[y];
    } break;

    case 0x8004: {
      u8 x = X_REG(chip->opcode);
      u8 y = Y_REG(chip->opcode);
      u8 flag = ((chip->V[x] + chip->V[y]) > 0xff);

      chip->V[x] += chip->V[y];
      chip->V[0xf] = flag;
    } break;

    case 0x8005: {
      u8 x = X_REG(chip->opcode);
      u8 y = Y_REG(chip->opcode);
      u8 flag = (chip->V[x] >= chip->V[y]);

      chip->V[x] -= chip->V[y];
      chip->V[0xf] = flag;
    } break;

    case 0x8006: {
      u8 x = X_REG(chip->opcode);
      // u8 y = Y_REG(chip->opcode);
      // u8 flag = (chip->V[y] & 1u);
      u8 flag = (chip->V[x] & 1u);

      // chip->V[x] = chip->V[y] >> 1;
      chip->V[x] >>= 1;
      chip->V[0xf] = flag;
    } break;

    case 0x8007: {
      u8 x = X_REG(chip->opcode);
      u8 y = Y_REG(chip->opcode);
      u8 flag = (chip->V[y] >= chip->V[x]);

      chip->V[x] = chip->V[y] - chip->V[x];
      chip->V[0xf] = flag;
    } break;

    case 0x800e: {
      u8 x = X_REG(chip->opcode);
      // u8 y = Y_REG(chip->opcode);
      // u8 flag = ((chip->V[y] >> 7) & 1u);
      u8 flag = ((chip->V[x] >> 7) & 1u);

      // chip->V[x] = chip->V[y] << 1;
      chip->V[x] <<= 1;
      chip->V[0xf] = flag;
    } break;
    }
    break;

  // Opcode is 9XY0, so need to check last nibble
  case 0x9000: {
    if (N(chip->opcode) != 0)
      break;

    u8 x = X_REG(chip->opcode);
    u8 y = X_REG(chip->opcode);

    if (chip->V[x] != chip->V[y]) {
      chip->PC += 2;
    }
  } break;

  case 0xa000: {
    chip->I = NNN(chip->opcode);
  } break;

  case 0xb000: {
    u8 x = X_REG(chip->opcode);
    u16 addr = NNN(chip->opcode) + chip->V[x];

    chip->PC = NNN(addr);
  } break;

  case 0xc000: {
    u8 x = X_REG(chip->opcode);
    u8 randInt = rand();

    chip->V[x] = randInt & NN(chip->opcode);
  } break;

  case 0xd000: {
    // TODO: clean up maybe?
    u8 x = X_REG(chip->opcode);
    u8 y = Y_REG(chip->opcode);
    u8 height = N(chip->opcode);

    u8 xPos = chip->V[x] % 64;
    u8 yPos = chip->V[y] % 32;

    chip->V[0xf] = 0;

    for (int row = 0; row < height; ++row) {
      u8 byte = chip->ram[chip->I + row];

      for (int col = 0; col < 8; ++col) {
        // if (yPos + row >= 32 || xPos + col >= 64) {
        //   break;
        // }
        // if (xPos + col >= 64)
        //   break;

        bool memPixel = (byte >> (7 - col)) & 1u;
        bool scrPixel = chip->pixels[(yPos + row) % 32][(xPos + col) % 64];

        chip->pixels[(yPos + row) % 32][(xPos + col) % 64] ^= memPixel;
        if ((chip->V[0xf] == 0) && memPixel && scrPixel) {
          chip->V[0xf] = 1;
        }
      }
    }
  } break;

  // valid opcodes: EX9E nd EXA1
  case 0xe000:
    switch (chip->opcode & 0xf0ff) {

    case 0xe09e: {
      u8 x = X_REG(chip->opcode);
      u8 index = N(chip->V[x]);

      if (CHECK_BIT(chip->keys, index)) {
        chip->PC += 2;
      }
    } break;

    case 0xe0a1: {
      u8 x = X_REG(chip->opcode);
      u8 index = N(chip->V[x]);

      if (!CHECK_BIT(chip->keys, index)) {
        chip->PC += 2;
      }
    } break;
    }
    break;

  // valid opcodes:
  // FX07, FX0A, FX15, FX18, FX1E,
  // FX29, FX33, FX55, FX65
  case 0xf000:
    switch (chip->opcode & 0xf0ff) {

    case 0xf007: {
      u8 x = X_REG(chip->opcode);

      chip->V[x] = chip->delay;
    } break;

    case 0xf00a: {
      static u16 lastKeys;

      if (!lastKeys || chip->keys) {
        chip->PC -= 2;
        lastKeys = chip->keys;

        return;
      }

      u8 x = X_REG(chip->opcode);

      for (size_t i = 0; i < 16; ++i) {
        if (CHECK_BIT(lastKeys, i)) {
          chip->V[x] = i;
          lastKeys = chip->keys;

          return;
        }
      }
    } break;

    case 0xf015: {
      u8 x = X_REG(chip->opcode);

      chip->delay = chip->V[x];
    } break;

    case 0xf018: {
      u8 x = X_REG(chip->opcode);

      chip->sound = chip->V[x];
    } break;

    case 0xf01e: {
      u8 x = X_REG(chip->opcode);

      chip->I = NNN(chip->I + chip->V[x]);
    } break;

    case 0xf029: {
      u8 x = X_REG(chip->opcode);
      u8 num = N(chip->V[x]);

      // TODO: make 5 into enum value
      chip->I = CHIP8_FONT_START + 5 * num;
    } break;

    case 0xf033: {
      u8 x = X_REG(chip->opcode);
      u8 val = chip->V[x];

      chip->ram[chip->I] = (val / 100) % 10;    // Hundreds digit
      chip->ram[chip->I + 1] = (val / 10) % 10; // Tens digit
      chip->ram[chip->I + 2] = val % 10;        // Ones digit
    } break;

    case 0xf055: {
      u8 x = X_REG(chip->opcode);

      for (int i = 0; i <= x; ++i) {
        chip->ram[chip->I + i] = chip->V[i];
      }
    } break;

    case 0xf065: {
      u8 x = X_REG(chip->opcode);

      for (int i = 0; i <= x; ++i) {
        chip->V[i] = chip->ram[chip->I++];
      }
    } break;
    }
    break;

  default:
    break;
  }
}
