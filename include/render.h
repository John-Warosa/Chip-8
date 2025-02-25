#ifndef RENDER_H
#define RENDER_H

#include "chip8.h"
#include <stddef.h>

void render_init(size_t width, size_t height);

void render(const Chip8 *chip);

#endif // RENDER_H
