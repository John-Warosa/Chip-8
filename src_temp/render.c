#include "render.h"
#include "raylib.h"
#include <stdio.h>

#define DEBUG

void render_setup() {
  InitWindow(64 * 15 + 450, 32 * 15, "Chip-8 Emulator");
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

  const char *debug = TextFormat(
      "current opcode: %04x\n"
      "Registers:\n"
      "V0: %02x\n"
      "V1: %02x\n"
      "V2: %02x\n"
      "V3: %02x\n"
      "V4: %02x\n"
      "V5: %02x\n"
      "V6: %02x\n"
      "V7: %02x\n"
      "V8: %02x\n"
      "V9: %02x\n"
      "Va: %02x\n"
      "Vb: %02x\n"
      "Vc: %02x\n"
      "Vd: %02x\n"
      "Ve: %02x\n"
      "Vf: %02x\n",
      chip->opcode, chip->V[0x0], chip->V[0x1], chip->V[0x2], chip->V[0x3],
      chip->V[0x4], chip->V[0x5], chip->V[0x6], chip->V[0x7], chip->V[0x8],
      chip->V[0x9], chip->V[0xa], chip->V[0xb], chip->V[0xc], chip->V[0xd],
      chip->V[0xe], chip->V[0xf]);

  const char *keypad = TextFormat(
      "Keypad: \n"
      "1: %d\t2: %d\t3: %d\t4: %d\n\n"
      "q: %d\tw: %d\te: %d\tr: %d\n\n"
      "a: %d\ts: %d\td: %d\tf: %d\n\n"
      "y: %d\tx: %d\tc: %d\tv: %d\n\n",
      chip->keys[0x1], chip->keys[0x2], chip->keys[0x3], chip->keys[0xc],
      chip->keys[0x4], chip->keys[0x5], chip->keys[0x6], chip->keys[0xd],
      chip->keys[0x7], chip->keys[0x8], chip->keys[0x9], chip->keys[0xe],
      chip->keys[0xa], chip->keys[0x0], chip->keys[0xb], chip->keys[0xf]);

  DrawText(debug, 64 * 15 + 10, 30, 15, WHITE);
  DrawText(keypad, 64 * 15 + 200, 100, 15, WHITE);

#endif

  EndDrawing();
}
