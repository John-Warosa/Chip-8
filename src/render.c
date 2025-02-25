#include "render.h"
#include "raylib.h"
#include "render_constants.h"

static struct RC {
  size_t width;
  size_t height;
  Font font;
} RenderContext;

static void render_screen(const Chip8 *chip);
static void render_chip_info(const Chip8 *chip);

void render_init(size_t width, size_t height) {
  InitWindow(width * SCALE + INFO_WIDTH, height * SCALE,
             "A better Chip-8 emulator");
  InitAudioDevice();

  RenderContext = (struct RC){.width = width * SCALE,
                              .height = height * SCALE,
                              .font = LoadFont(fontname)};
}

void render(const Chip8 *chip) {
  BeginDrawing();

  ClearBackground(DARKGRAY);

  DrawRectangle(0, 0, RenderContext.width, RenderContext.height, BLACK);
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
  DrawText(info, RenderContext.width + 20, 0, 20, RAYWHITE);
  // static char chipInfo[1024];
  // chipInfo = TextFormat("Opcode: %d", chip->opcode);
}
