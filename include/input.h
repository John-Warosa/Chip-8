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
  NO_ACTION,      // No key pressed
  EMU_EXIT,       // Ctrl + Esc
  CHIP8_PAUSE,    // Ctrl + p
  CHIP8_RESTART,  // Ctrl + r
  TOGGLE_QUIRK_0, // Ctrl + 1
  TOGGLE_QUIRK_1, // Ctrl + 2
  TOGGLE_QUIRK_2, // Ctrl + 3
  TOGGLE_QUIRK_3, // Ctrl + 4
  TOGGLE_QUIRK_4, // Ctrl + 5
  TOGGLE_QUIRK_5, // Ctrl + 6
};

enum Action get_action(void);

u16 get_keys(void);

#endif // INPUT_H
