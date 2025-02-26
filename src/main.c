#include "emulator/emulator.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "Failed to run emulator. Need to provide a file to run.\n"
                    "Use ./chip8 <filename> to properly run the program.");
    exit(1);
  } else if (argc > 2) {
    fprintf(stderr,
            "Failed to run emulator. Please provide only one file to run.\n"
            "Use ./chip8 <filename> to properly run the program.\n"
            "Support for multiple files may be added in the future.");
  }

  static Emulator emu;

  Emulator_init(&emu, argv[1]);
  Emulator_loop(&emu);
}
