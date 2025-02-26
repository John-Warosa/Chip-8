#include "input.h"
#include "raylib.h"

#define EMU_KEY KEY_LEFT_CONTROL
#define SET_KEY(index) (0 | 1u << index)

enum Action get_action(void) {
  if (IsKeyPressed(KEY_ESCAPE))
    return EMU_EXIT;

  if (!IsKeyDown(EMU_KEY))
    return NO_ACTION;

  switch (GetKeyPressed()) {
  case KEY_R:
    return CHIP8_RESTART;
  case KEY_P:
    return CHIP8_PAUSE;
  case KEY_ONE:
    return TOGGLE_QUIRK_0;
  case KEY_TWO:
    return TOGGLE_QUIRK_1;
  case KEY_THREE:
    return TOGGLE_QUIRK_2;
  case KEY_FOUR:
    return TOGGLE_QUIRK_3;
  case KEY_FIVE:
    return TOGGLE_QUIRK_4;
  case KEY_SIX:
    return TOGGLE_QUIRK_5;
  default:
    return NO_ACTION;
  }
}

u16 get_keys(void) {
  // Ignore Chip-8 input if emulator key is pressed
  if (IsKeyDown(EMU_KEY))
    return 0;

  if (IsKeyDown(KEY_ONE)) {
    return SET_KEY(0x1);
  } else if (IsKeyDown(KEY_TWO)) {
    return SET_KEY(0x2);
  } else if (IsKeyDown(KEY_THREE)) {
    return SET_KEY(0x3);
  } else if (IsKeyDown(KEY_FOUR)) {
    return SET_KEY(0xc);
  } else if (IsKeyDown(KEY_Q)) {
    return SET_KEY(0x4);
  } else if (IsKeyDown(KEY_W)) {
    return SET_KEY(0x5);
  } else if (IsKeyDown(KEY_E)) {
    return SET_KEY(0x6);
  } else if (IsKeyDown(KEY_R)) {
    return SET_KEY(0xd);
  } else if (IsKeyDown(KEY_A)) {
    return SET_KEY(0x7);
  } else if (IsKeyDown(KEY_S)) {
    return SET_KEY(0x8);
  } else if (IsKeyDown(KEY_D)) {
    return SET_KEY(0x9);
  } else if (IsKeyDown(KEY_F)) {
    return SET_KEY(0xe);
  } else if (IsKeyDown(KEY_Z)) {
    return SET_KEY(0xa);
  } else if (IsKeyDown(KEY_X)) {
    return SET_KEY(0x0);
  } else if (IsKeyDown(KEY_C)) {
    return SET_KEY(0xb);
  } else if (IsKeyDown(KEY_V)) {
    return SET_KEY(0xf);
  }

  return 0;
}
