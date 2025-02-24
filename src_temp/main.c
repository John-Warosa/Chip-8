#include "emulator.h"
#include <stdio.h>

int main(int argc, char **argv) {
  if (argc < 2) {
    puts("Need to specify a file");
    return 1;
  } else if (argc > 2) {
    puts("Too many arguments. Specify only one");
    return 1;
  }

  Chip8 *chip = Chip8_init(argv[1]);
  Emulator_loop(chip);

  return 0;
}
