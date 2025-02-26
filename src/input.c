#include "input.h"
#include "raylib.h"

#define SET_KEY(key, index) (key | 1u << index)

enum Action get_action(void) {
  if (!IsKeyDown(KEY_LEFT_SUPER))
    return NO_ACTION;

  switch (GetKeyPressed()) {
  case KEY_ESCAPE:
    return EMU_EXIT;
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
  u16 keys = 0;

  if (IsKeyDown(KEY_ONE)) {
    keys = SET_KEY(keys, 0x1);
  } else if (IsKeyDown(KEY_TWO)) {
    keys = SET_KEY(keys, 0x2);
  } else if (IsKeyDown(KEY_THREE)) {
    keys = SET_KEY(keys, 0x3);
  } else if (IsKeyDown(KEY_FOUR)) {
    keys = SET_KEY(keys, 0xc);
  } else if (IsKeyDown(KEY_Q)) {
    keys = SET_KEY(keys, 0x4);
  } else if (IsKeyDown(KEY_W)) {
    keys = SET_KEY(keys, 0x5);
  } else if (IsKeyDown(KEY_E)) {
    keys = SET_KEY(keys, 0x6);
  } else if (IsKeyDown(KEY_R)) {
    keys = SET_KEY(keys, 0xd);
  } else if (IsKeyDown(KEY_A)) {
    keys = SET_KEY(keys, 0x7);
  } else if (IsKeyDown(KEY_S)) {
    keys = SET_KEY(keys, 0x8);
  } else if (IsKeyDown(KEY_D)) {
    keys = SET_KEY(keys, 0x9);
  } else if (IsKeyDown(KEY_F)) {
    keys = SET_KEY(keys, 0xe);
  } else if (IsKeyDown(KEY_Z)) {
    keys = SET_KEY(keys, 0xa);
  } else if (IsKeyDown(KEY_X)) {
    keys = SET_KEY(keys, 0x0);
  } else if (IsKeyDown(KEY_C)) {
    keys = SET_KEY(keys, 0xb);
  } else if (IsKeyDown(KEY_V)) {
    keys = SET_KEY(keys, 0xf);
  }

  return keys;
}
