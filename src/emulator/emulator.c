#include "emulator/emulator.h"
#include "chip8.h"
#include "chip8_constants.h"
#include "emulator/emulator_constants.h"
#include "raylib.h"
#include "render/render.h"
#include "timer.h"
#include <string.h>

#define ENOUGH_STEPS(counter, steps) ((counter) % (steps) == 0)

void Emulator_init(Emulator *emu, const char *filename) {
  emu->quirks = DEFAULT_QUIRKS;

  Chip8_init(&emu->chip, filename);
  strncpy(emu->filename, filename, MAX_FILE_LENGTH);

  render_init(CHIP8_SCR_WIDTH, CHIP8_SCR_HEIGHT);
}

void Emulator_loop(Emulator *emu) {
  milli lastStepEnd = 0;
  milli nextStepStart = 0;
  size_t counter = 0;

  // TODO: Replace WindowShouldClose with something else
  while (!WindowShouldClose()) {
    nextStepStart = ms_time();

    if (nextStepStart - lastStepEnd < STEP_TIME) {
      continue;
    }

    Chip8_step(&emu->chip, emu->quirks, ENOUGH_STEPS(counter, STEPS_PER_FRAME));

    if (ENOUGH_STEPS(counter, STEPS_PER_FRAME)) {
      render(&emu->chip);
    }

    ++counter;
    lastStepEnd = ms_time();
  }
}
