#include "render/render.h"
#include "raylib.h"
#include "render/render_constants.h"

static void render_screen(const Chip8 *chip);
static void render_chip_info(const Chip8 *chip);

static struct RenderContext {
  size_t width;
  size_t height;

  enum Scene scene;
  Font font;
} rc;

Sound sound;

void render_init(size_t width, size_t height) {
  InitWindow(width * SCALE + INFO_WIDTH, height * SCALE,
             "A better Chip-8 emulator");
  InitAudioDevice();

  sound = LoadSound(soundname);
  rc = (struct RenderContext){.width = width * SCALE,
                              .height = height * SCALE,
                              .scene = SCENE_CHIP8,
                              .font = LoadFont(fontname)};
}

void change_scene(enum Scene scene) { rc.scene = scene; }

void render(const Chip8 *chip) {
  BeginDrawing();

  ClearBackground(DARKGRAY);

  DrawRectangle(0, 0, rc.width, rc.height, BLACK);
  render_screen(chip);
  render_chip_info(chip);

  DrawFPS(20, 20);

  EndDrawing();
}

static void render_screen(const Chip8 *chip) {
  for (int row = 0; row < 32; ++row) {
    for (int col = 0; col < 64; ++col) {
      if (chip->pixels[row][col]) {
        DrawRectangle(col * SCALE, row * SCALE, SCALE - 1, SCALE - 1, WHITE);
      }
    }
  }
}

static void render_chip_info(const Chip8 *chip) {
  const char *info = TextFormat("op: %04x\n"
                                "pc: %03x\n",
                                chip->opcode, chip->PC);
  DrawText(info, rc.width + 20, 0, 20, RAYWHITE);
  for (int i = 0; i < 16; ++i) {
    DrawText(TextFormat("V%x: %02x", i, chip->V[i]), rc.width + 20, 50 + 20 * i,
             20, RAYWHITE);
  }
  // static char chipInfo[1024];
  // chipInfo = TextFormat("Opcode: %d", chip->opcode);
}
