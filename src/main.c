#include "chip8.h"
#include "render.h"

int main(int argc, char **argv) {
  render_init(64, 32);
  Chip8 *chip = Chip8_init(argv[1]);

  Chip8_loop(chip);
}
