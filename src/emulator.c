#include "emulator.h"
#include "instructions.h"
#include "render.h"
#include "romload.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static uint16_t fetch_opcode(Chip8 *chip);
static void get_input(Chip8 *chip);

static uint8_t fonts[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8 *Chip8_init(const char *filename) {
  Chip8 *chip = calloc(1, sizeof(*chip));

  chip->PC = 0x200;
  load_rom(chip, filename);

  int start = 0x50;
  for (int i = 0; i < 16 * 5; ++i) {
    chip->memory[start + i] = fonts[i];
  }

  render_setup();
  srand(time(NULL));
  return chip;
}

#include "raylib.h"
void Emulator_loop(Chip8 *chip) {
  Sound sound = LoadSound("sound.wav");

  while (!WindowShouldClose()) {
    // while (true) {

    if (chip->delay > 0) {
      --chip->delay;
    }
    if (chip->sound > 0) {
      if (!IsSoundPlaying(sound)) {
        PlaySound(sound);
      }

      --chip->sound;
    } else {
      StopSound(sound);
    }

    chip->vblank = true;

    for (int i = 0; i < 660 / 60; ++i) {
      get_input(chip);
      chip->opcode = fetch_opcode(chip);
      execute(chip);
    }

    render(chip);
  }
}

static uint16_t fetch_opcode(Chip8 *chip) {
  uint8_t hiByte = chip->memory[chip->PC];
  uint8_t loByte = chip->memory[chip->PC + 1];
  chip->PC += 2;

  return (hiByte << 8) + loByte;
}

static void get_input(Chip8 *chip) {
  memset(chip->keys, false, sizeof(chip->keys));

  if (IsKeyDown(KEY_ONE)) {
    chip->keys[0x1] = true;
  } else if (IsKeyDown(KEY_TWO)) {
    chip->keys[0x2] = true;
  } else if (IsKeyDown(KEY_THREE)) {
    chip->keys[0x3] = true;
  } else if (IsKeyDown(KEY_FOUR)) {
    chip->keys[0xc] = true;
  } else if (IsKeyDown(KEY_Q)) {
    chip->keys[0x4] = true;
  } else if (IsKeyDown(KEY_W)) {
    chip->keys[0x5] = true;
  } else if (IsKeyDown(KEY_E)) {
    chip->keys[0x6] = true;
  } else if (IsKeyDown(KEY_R)) {
    chip->keys[0xd] = true;
  } else if (IsKeyDown(KEY_A)) {
    chip->keys[0x7] = true;
  } else if (IsKeyDown(KEY_S)) {
    chip->keys[0x8] = true;
  } else if (IsKeyDown(KEY_D)) {
    chip->keys[0x9] = true;
  } else if (IsKeyDown(KEY_F)) {
    chip->keys[0xe] = true;
  } else if (IsKeyDown(KEY_Z)) {
    chip->keys[0xa] = true;
  } else if (IsKeyDown(KEY_X)) {
    chip->keys[0x0] = true;
  } else if (IsKeyDown(KEY_C)) {
    chip->keys[0xb] = true;
  } else if (IsKeyDown(KEY_V)) {
    chip->keys[0xf] = true;
  }
}
