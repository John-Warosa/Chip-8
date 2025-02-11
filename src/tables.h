#ifndef TABLE_H
#define TABLE_H

#include "emulator.h"

typedef void (*instruction)(Chip8 *chip);

extern instruction instructionTable[];

void set_instruction_table();

#endif // TABLE_H
