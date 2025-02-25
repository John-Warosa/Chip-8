#include "chip8.h"
#include "chip8_constants.h"
#include "input.h"
#include "instructions.h"
#include "raylib.h"
#include "render.h"
#include "romload.h"
#include "timer.h"
#include <stdlib.h>

static void load_font(u8 ram[]);
static u16 get_opcode(const u8 ram[], u16 PC);

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

Chip8 *Chip8_init(const char *filename) {
  Chip8 *chip = calloc(1, sizeof(*chip));

  if (!chip) {
    return chip;
  }

  chip->PC = PROGRAM_START;
  load_font(chip->ram);
  load_rom(chip->ram, PROGRAM_START, filename);

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
  size_t counter = 0;

  while (!WindowShouldClose()) {
    nextStepStart = ms_time();

    if (nextStepStart - lastStepEnd < STEP_TIME) {
      continue;
    }

    chip->keys = get_keys();

    chip->opcode = get_opcode(chip->ram, chip->PC);
    chip->PC += 2;

    execute_instruction(chip);

    if (counter % STEPS_PER_FRAME == 0) {
      if (chip->delay) {
        --chip->delay;
      }

      if (chip->sound) {
        if (!IsSoundPlaying(sound)) {
          PlaySound(sound);
        }
        --chip->sound;
      } else {
        StopSound(sound);
      }

      render(chip);
    }

    ++counter;
    lastStepEnd = ms_time();
  }
}

static u16 get_opcode(const u8 ram[], u16 PC) {
  return (ram[PC] << 8) + ram[PC + 1];
}

int write_chip8_info(char *buf, size_t bufsize, const Chip8 *chip);
