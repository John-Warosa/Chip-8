#include "chip8.h"
#include "raylib.h"
#include "render.h"

int main(int argc, char **argv) {
  Chip8 *chip = Chip8_init();
  render_init(64, 32, 60);
  while (!WindowShouldClose()) {
    render(chip);
  }
}
