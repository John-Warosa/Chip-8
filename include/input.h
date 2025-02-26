#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <types.h>

/*===================================================================

Input handler

There are two relevant types of input:
  - Chip-8 input
  - Emulator input

Chip-8 input is comprised of 16 keys, typically mapped to the
left-most side of the keyboard (1 - 4, q - r, a - f and y - v).
These keys are used to interact with ROMs

Emulator input is used to interact with the emulator to perform
actions such as selecting quirks or restarting the Chip-8.
The input is modelled by actions.
All actions and corresponding key bindings are given below
in enum Action.

===================================================================*/

enum Action {
  NO_ACTION,     // No key pressed
  EMU_EXIT,      // Esc
  CHIP8_RESTART, // Ctrl + r
};

enum Action get_action(void);

u16 get_keys(void);

#endif // INPUT_H
