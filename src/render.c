#include "render.h"
#include "raylib.h"
#include "render_constants.h"

static struct {
  size_t width;
  size_t height;
  Font font;
} RenderContext;

void render_init(size_t width, size_t height) {
  InitWindow(width * SCALE + INFO_WIDTH, height * SCALE,
             "A better Chip-8 emulator");
  InitAudioDevice();

  RenderContext.width = width * SCALE;
  RenderContext.height = height * SCALE;
  RenderContext.font = LoadFont(fontname);
}

void render(const Chip8 *chip) {
  BeginDrawing();

  ClearBackground(DARKGRAY);

  DrawRectangle(0, 0, RenderContext.width, RenderContext.height, BLACK);
  for (int row = 0; row < 32; ++row) {
    for (int col = 0; col < 64; ++col) {
      if (chip->pixels[row][col]) {
        DrawRectangle(col * SCALE, row * SCALE, SCALE - 1, SCALE - 1, WHITE);
      }
    }
  }

  DrawFPS(20, 20);

  EndDrawing();
}
