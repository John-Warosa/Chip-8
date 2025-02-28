#include "chip8_constants.h"
#include "emulator/emulator_constants.h"
#include "instructions.h"
#include "types.h"
#include <stdlib.h>
#include <string.h>

#define N(opcode) ((opcode) & 0x000f)
#define NN(opcode) ((opcode) & 0x00ff)
#define NNN(opcode) ((opcode) & 0x0fff)
#define X_REG(opcode) (((opcode) & 0x0f00) >> 8)
#define Y_REG(opcode) (((opcode) & 0x00f0) >> 4)

#define CHECK_BIT(num, index) ((num) & (1u << (index)))
#define IS_QUIRK_ACTIVE(quirks, index) ((quirks) & (1u << index))

void execute_instruction(Chip8 *chip, u16 quirks) {
  switch (chip->opcode & 0xf000) {

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

  case 0x5000: {
    if (chip->opcode & 0x000f)
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

      if (IS_QUIRK_ACTIVE(quirks, QUIRK_RESET)) {
        chip->V[0xf] = 0;
      }
    } break;

    case 0x8002: {
      u8 x = X_REG(chip->opcode);
      u8 y = Y_REG(chip->opcode);

      chip->V[x] &= chip->V[y];

      if (IS_QUIRK_ACTIVE(quirks, QUIRK_RESET)) {
        chip->V[0xf] = 0;
      }
    } break;

    case 0x8003: {
      u8 x = X_REG(chip->opcode);
      u8 y = Y_REG(chip->opcode);

      chip->V[x] ^= chip->V[y];

      if (IS_QUIRK_ACTIVE(quirks, QUIRK_RESET)) {
        chip->V[0xf] = 0;
      }
    } break;
    case 0x8004: {
      u8 x = X_REG(chip->opcode);
      u8 y = Y_REG(chip->opcode);
      u8 flag = (chip->V[x] + chip->V[y]) > 0xff;

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

      // TODO: refactor 8xy6 and 8xye to be cleaner
      u8 x = X_REG(chip->opcode);
      u8 y = Y_REG(chip->opcode);
      u8 flag;

      if (IS_QUIRK_ACTIVE(quirks, QUIRK_SHIFTING)) {
        flag = (CHECK_BIT(chip->V[x], 0));
        chip->V[x] = chip->V[x] >> 1;
      } else {
        flag = (chip->V[y] & 1u);
        chip->V[x] = chip->V[y] >> 1;
      }

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
      u8 y = Y_REG(chip->opcode);
      u8 flag;

      if (IS_QUIRK_ACTIVE(quirks, QUIRK_SHIFTING)) {
        flag = ((chip->V[x] >> 7) & 1u);
        chip->V[x] = chip->V[x] << 1;
      } else {
        flag = ((chip->V[y] >> 7) & 1u);
        chip->V[x] = chip->V[y] << 1;
      }

      chip->V[0xf] = flag;
    } break;
    }
    break;

  case 0x9000: {
    if (chip->opcode & 0x000f)
      break;

    u8 x = X_REG(chip->opcode);
    u8 y = Y_REG(chip->opcode);

    if (chip->V[x] != chip->V[y]) {
      chip->PC += 2;
    }
  } break;

  case 0xa000:
    chip->I = NNN(chip->opcode);
    break;

  case 0xb000: {
    u8 x = IS_QUIRK_ACTIVE(quirks, QUIRK_JUMPING) ? X_REG(chip->opcode) : 0;
    u16 addr = NNN(chip->opcode) + chip->V[x];

    chip->PC = NNN(addr);
  } break;

  case 0xc000: {
    u8 x = X_REG(chip->opcode);
    u8 randInt = rand();

    chip->V[x] = randInt & NN(chip->opcode);
  } break;

  case 0xd000: {
    if (!chip->vblank) {
      chip->PC -= 2;
      return;
    }

    u8 x = X_REG(chip->opcode);
    u8 y = Y_REG(chip->opcode);
    u8 height = N(chip->opcode);

    u8 xPos = chip->V[x] % 64;
    u8 yPos = chip->V[y] % 32;

    chip->V[0xf] = 0;

    if (IS_QUIRK_ACTIVE(quirks, QUIRK_WAIT)) {
      chip->vblank = false;
    }

    for (int row = 0; row < height; ++row) {
      u8 byte = chip->ram[chip->I + row];

      for (int col = 0; col < 8; ++col) {
        // Prevent overflow in clipping mode
        if (IS_QUIRK_ACTIVE(quirks, QUIRK_CLIPPING) &&
            (yPos + row >= 32 || xPos + col >= 64)) {
          break;
        }

        bool memPixel = (byte >> (7 - col)) & 1u;
        bool scrPixel = chip->pixels[(yPos + row) % 32][(xPos + col) % 64];

        chip->pixels[(yPos + row) % 32][(xPos + col) % 64] ^= memPixel;
        if ((chip->V[0xf] == 0) && memPixel && scrPixel) {
          chip->V[0xf] = 1;
        }
      }
    }
  } break;

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

  case 0xf000:
    switch (chip->opcode & 0xf0ff) {

    case 0xf007: {
      u8 x = X_REG(chip->opcode);

      chip->V[x] = chip->delay;
    } break;

    case 0xf00a: {
      static uint16_t lastKeys;
      u8 x = X_REG(chip->opcode);

      if (!lastKeys || chip->keys) {
        lastKeys = chip->keys;
        chip->PC -= 2;
        return;
      }

      for (int i = 0; i < 16; ++i) {
        if (CHECK_BIT(lastKeys, i)) {
          chip->V[x] = i;
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

      chip->I = CHIP8_FONT_START + 5 * num;
    } break;

    case 0xf033: {
      u8 x = X_REG(chip->opcode);
      u8 val = chip->V[x];

      chip->ram[chip->I] = (val / 100) % 10;
      chip->ram[chip->I + 1] = (val / 10) % 10;
      chip->ram[chip->I + 2] = val % 10;
    } break;

    case 0xf055: {
      u8 x = X_REG(chip->opcode);

      if (IS_QUIRK_ACTIVE(quirks, QUIRK_MEMORY)) {
        for (size_t i = 0; i <= x; ++i) {
          chip->ram[chip->I++] = chip->V[i];
        }
      } else {
        for (size_t i = 0; i <= x; ++i) {
          chip->ram[chip->I + i] = chip->V[i];
        }
      }
    } break;

    case 0xf065: {
      u8 x = X_REG(chip->opcode);

      if (IS_QUIRK_ACTIVE(quirks, QUIRK_MEMORY)) {

        for (size_t i = 0; i <= x; ++i) {
          chip->V[i] = chip->ram[chip->I++];
        }
      } else {
        for (size_t i = 0; i <= x; ++i) {
          chip->V[i] = chip->ram[chip->I + i];
        }
      }
    } break;
    }
    break;

  default:
    break;
  }
}
