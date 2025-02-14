#include "render.h"
#include "raylib.h"

void render_setup() {
  InitWindow(640, 320, "Chip-8 Emulator");
  SetTargetFPS(10);
}

void render(Chip8 *chip) {
  BeginDrawing();
  ClearBackground(BLACK);

  for (int row = 0; row < 32; ++row) {
    for (int col = 0; col < 128; ++col) {
      if (chip->pixels[row] & ((uint64_t)1 << (63 - col))) {
        DrawRectangle(col * 10, row * 10, 10, 10, RAYWHITE);
      }
    }
  }
  EndDrawing();
}
