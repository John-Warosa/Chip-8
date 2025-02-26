#ifndef RENDER_H
#define RENDER_H

#include "emulator/emulator.h"
#include <stddef.h>

enum Scene {
  SCENE_CHIP8,
  SCENE_QUIRK_SELECT,
};

void render_init(size_t width, size_t height);

void render(const Emulator *emu);

void change_scene(enum Scene scene);

#endif // RENDER_H
