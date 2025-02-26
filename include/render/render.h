#ifndef RENDER_H
#define RENDER_H

#include "chip8.h"
// Still need raylib.h for Emulator mainloop
#include "raylib.h"
#include <stddef.h>

enum Scene {
  SCENE_CHIP8,
  SCENE_QUIRK_SELECT,
};

void render_init(size_t width, size_t height);

void render(const Chip8 *chip);

void change_scene(enum Scene scene);

#endif // RENDER_H
