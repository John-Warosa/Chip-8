#include "render.h"
#include "raylib.h"

void render_setup() {
  InitWindow(64 * 15, 32 * 15, "Chip-8 Emulator");
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
  const char *debug = TextFormat("Opcode: %04x\n"
                                 "V0: %02x\n"
                                 "V1: %02x\n"
                                 "I: %03x\n",
                                 chip->opcode, chip->V[0], chip->V[1], chip->I);
  DrawText(debug, 700, 10, 15, WHITE);

#endif

  EndDrawing();
}
