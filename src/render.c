#include "render.h"
#include "raylib.h"
#include "render_constants.h"

static struct {
  size_t width;
  size_t height;
  size_t fps;
  Font font;
} RenderContext;

void render_init(size_t width, size_t height, size_t fps) {
  InitWindow(width * SCALE + INFO_WIDTH, height * SCALE,
             "A better Chip-8 emulator");
  InitAudioDevice();

  SetTargetFPS(fps);

  RenderContext.width = width * SCALE;
  RenderContext.height = height * SCALE;
  RenderContext.fps = fps;
  RenderContext.font = LoadFont(fontname);
}

void render(const Chip8 *chip) {
  BeginDrawing();

  ClearBackground(DARKGRAY);
  DrawRectangle(0, 0, RenderContext.width, RenderContext.height, BLACK);

  EndDrawing();
}
