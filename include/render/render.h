#ifndef RENDER_H
#define RENDER_H

#include "chip8.h"
#include "raylib.h"
#include <stddef.h>

void render_init(size_t width, size_t height);

void render(const Chip8 *chip);

extern Sound sound;

#endif // RENDER_H
