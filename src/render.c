#include "render.h"
#include "raylib.h"

void render_setup() {
  InitWindow(1280, 320, "Chip-8 Emulator");
  SetTargetFPS(10);
}

void render(Chip8 *chip) {
  BeginDrawing();
  ClearBackground(BLACK);

  for (int row = 0; row < 32; ++row) {
    for (int col = 0; col < 128; ++col) {
      if (chip->pixels[row] & ((__uint128_t)1 << (123 - col))) {
        DrawRectangle(col * 10, row * 10, 10, 10, RAYWHITE);
      }
    }
  }
  EndDrawing();
}
