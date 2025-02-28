#include "render/render.h"
#include "chip8.h"
#include "raylib.h"
#include "render/render_constants.h"

#define DRAW_FONT_TEXT(str, xPos, yPos)                                        \
  DrawTextEx(rc.font, str, (Vector2){xPos, yPos}, 20, 1, RAYWHITE)

static void render_screen(const Chip8 *chip);
static void render_chip_info(const Chip8 *chip);
static void play_sound(u8 soundTimer);

static struct RenderContext {
  size_t width;
  size_t height;

  enum Scene scene;
  Font font;
  Sound sound;
} rc;

void render_init(size_t width, size_t height) {
  InitWindow(width * SCALE + INFO_WIDTH, height * SCALE,
             "A better Chip-8 emulator");
  InitAudioDevice();

  rc = (struct RenderContext){.width = width * SCALE,
                              .height = height * SCALE,
                              .scene = SCENE_CHIP8,
                              .font = LoadFont(fontname),
                              .sound = LoadSound(soundname)};
}

void change_scene(enum Scene scene) { rc.scene = scene; }

void render(const Emulator *emu) {
  play_sound(emu->chip.sound);

  BeginDrawing();

  ClearBackground(DARKGRAY);

  DrawRectangle(0, 0, rc.width, rc.height, BLACK);
  render_screen(&emu->chip);

  // for (int i = 0; i < 6; ++i) {
  //   DRAW_FONT_TEXT(
  //       TextFormat("Quirk %d: %d", i, emu->quirks & (1u << i) ? 1 : 0),
  //       rc.width + 200, 20 + 20 * i);
  // }

  // render_chip_info(&emu->chip);
  DRAW_FONT_TEXT(Chip8_memory_view(&emu->chip), rc.width + MEMORY_POS_X,
                 MEMORY_POS_Y);

  // DrawFPS(20, 20);

  EndDrawing();
}

static void play_sound(u8 soundTimer) {
  if (soundTimer) {
    if (!IsSoundPlaying(rc.sound)) {
      PlaySound(rc.sound);
    }
  } else {
    StopSound(rc.sound);
  }
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
