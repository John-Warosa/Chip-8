#include "render.h"
#include "raylib.h"

#define DEBUG

void render_setup() {
  InitWindow(64 * 15 + 350, 32 * 15, "Chip-8 Emulator");
  InitAudioDevice();
  SetTargetFPS(60);
}

void render(Chip8 *chip) {
  BeginDrawing();
  ClearBackground(BLACK);

  for (int row = 0; row < 32; ++row) {
    for (int col = 0; col < 64; ++col) {
      if (chip->pixels[row][col]) {
        DrawRectangle(col * 15, row * 15, 14, 14, WHITE);
      }
    }
  }

#ifdef DEBUG
  if (IsKeyPressed(KEY_ENTER)) {
    SetTargetFPS(60);
  } else if (IsKeyPressed(KEY_SPACE)) {
    SetTargetFPS(10);
  } else if (IsKeyDown(KEY_SPACE)) {
    SetTargetFPS(2);
  }
  DrawFPS(64 * 15 + 10, 10);
  const char *debug = TextFormat("FPS: %d\n\n"
                                 "current opcode: %04x\n",
                                 GetFPS(), chip->opcode);
  const char *registers = TextFormat("Registers");
  DrawText(debug, 64 * 15 + 10, 10, 15, WHITE);

#endif

  EndDrawing();
}
