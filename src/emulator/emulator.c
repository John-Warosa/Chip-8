#include "emulator/emulator.h"
#include "chip8.h"
#include "chip8_constants.h"
#include "emulator/emulator_constants.h"
#include "input.h"
#include "render/render.h"
#include "timer.h"
#include <string.h>

#define ENOUGH_STEPS(counter, steps) ((counter) % (steps) == 0)
#define TOGGLE_QUIRK(quirks, index) ((quirks) ^ (1u << (index)))

static void action_handler(Emulator *emu, enum Action action);

void Emulator_init(Emulator *emu, const char *filename) {
  emu->quirks = DEFAULT_QUIRKS;
  emu->running = true;

  Chip8_init(&emu->chip, filename);
  strncpy(emu->filename, filename, MAX_FILE_LENGTH);

  render_init(CHIP8_SCR_WIDTH, CHIP8_SCR_HEIGHT);
}

void Emulator_loop(Emulator *emu) {
  milli lastStepEnd = 0;
  milli nextStepStart = 0;
  size_t counter = 0;

  // TODO: Replace WindowShouldClose with something else
  while (emu->running) {
    nextStepStart = ms_time();

    if (nextStepStart - lastStepEnd < STEP_TIME) {
      continue;
    }

    if (!emu->pause) {
      Chip8_step(&emu->chip, emu->quirks,
                 ENOUGH_STEPS(counter, STEPS_PER_FRAME));
    }

    if (ENOUGH_STEPS(counter, STEPS_PER_FRAME)) {
      render(emu);
      emu->chip.vblank = true;
    }

    action_handler(emu, get_action());

    ++counter;
    lastStepEnd = ms_time();
  }
}

static void action_handler(Emulator *emu, enum Action action) {
  switch (action) {
  case EMU_EXIT:
    emu->running = false;
    break;
  case CHIP8_RESTART:
    Chip8_restart(&emu->chip, emu->filename);
    emu->pause = false;
    break;
  case CHIP8_PAUSE:
    emu->pause = !emu->pause;
    break;
  case TOGGLE_QUIRK_0:
    emu->quirks = TOGGLE_QUIRK(emu->quirks, 0);
    break;
  case TOGGLE_QUIRK_1:
    emu->quirks = TOGGLE_QUIRK(emu->quirks, 1);
    break;
  case TOGGLE_QUIRK_2:
    emu->quirks = TOGGLE_QUIRK(emu->quirks, 2);
    break;
  case TOGGLE_QUIRK_3:
    emu->quirks = TOGGLE_QUIRK(emu->quirks, 3);
    break;
  case TOGGLE_QUIRK_4:
    emu->quirks = TOGGLE_QUIRK(emu->quirks, 4);
    break;
  case TOGGLE_QUIRK_5:
    emu->quirks = TOGGLE_QUIRK(emu->quirks, 5);
    break;
  default:
    break;
  }
}
