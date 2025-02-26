#ifndef CHIP8_H
#define CHIP8_H

/*===================================================================

Chip-8

For more information on the Chip-8, see Cowgod's reference
and the guides by Austin Morlan and Tobias V. Langhoff:
  - https://github.com/trapexit/chip-8_documentation
  - https://tobiasvl.github.io/blog/write-a-chip-8-emulator/
  - https://austinmorlan.com/posts/chip8_emulator/

The Chip-8 has 6 quirks that can change a programs behaviour.
Those are:
  - 0: Vf reset
  - 1: Memory
  - 2: Display wait
  - 3: Clipping
  - 4: Shifting
  - 5: Jumping

For detailed descriptions of the quirks, see test 5 of Timendus'
testing suite:
  - https://github.com/Timendus/chip8-test-suite?tab=readme-ov-file

The default behaviour will have all quirks conform to
test 5 - quirks by Timendus. This means quirks 0 - 3 are turned
on and quirks 4 and 5 are off.
Note that having all quirks conform breaks test 3 - corax.
Enabling or diabling quirks may break other tests (for example,
disabling all quirks seems to break the visuals of test 6 - keypad)

===================================================================*/

#include "types.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct {
  // Registers
  u8 V[16];
  u8 sound;
  u8 delay;
  u16 I;

  // Pointers
  u8 SP;
  u16 PC;

  // Memory + stack
  u8 ram[0xfff + 1];
  u16 stack[16];

  // Keypad (bitarray)
  // Keys 0 - 16 are represented as bits in one u16
  u16 keys;

  // Pixel buffer (bool array)
  // If vblank is true, sprites can be drawn to the display
  bool pixels[32][64];
  bool vblank;

  // opcode, only uses lower 3 nibbles
  u16 opcode;

  // flags for quirks, bits 0 - 5 will be used as laid out above
  u8 quirks;
} Chip8;

// Allocate emulator on the heap and initialise it
// Returned pointer may be NULL
Chip8 *Chip8_init(const char *filename);

void Chip8_loop(Chip8 *chip);

// Store Chip8 info in the provided buffer
// Returns 1 if the buffer size was too small, 0 otherwise
int Chip8_write_info(char *buf, size_t bufsize, const Chip8 *chip);

#endif // CHIP8_H
