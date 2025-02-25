#include "chip8.h"
#include "chip8_constants.h"
#include "raylib.h"
#include "render.h"
#include "timer.h"
#include <stdlib.h>

static void load_font(u8 ram[]);

// Font set that gets loade into RAM
// Each number is a sprite 8 pixels wide and 5 pixels tall
static const u8 fontset[FONT_SIZE] = {
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

Chip8 *Chip8_init(void) {
  Chip8 *chip = calloc(1, sizeof(*chip));

  if (!chip) {
    return chip;
  }

  chip->PC = PROGRAM_START;
  load_font(chip->ram);

  return chip;
}

static void load_font(u8 ram[]) {
  for (size_t i = 0; i < FONT_SIZE; ++i) {
    ram[FONT_START + i] = fontset[i];
  }
}

void Chip8_loop(Chip8 *chip) {
  milli lastStepEnd = 0;
  milli nextStepStart = 0;

  while (!WindowShouldClose()) {
    nextStepStart = ms_time();

    if (nextStepStart - lastStepEnd < STEP_TIME) {
      continue;
    }

    if (chip->delay) {
      --chip->delay;
    }

    if (chip->sound) {
      --chip->sound;
    }

    // TODO: get input, get opcode, execute, render

    lastStepEnd = ms_time();
  }
}

int write_chip8_info(char *buf, size_t bufsize, const Chip8 *chip);
