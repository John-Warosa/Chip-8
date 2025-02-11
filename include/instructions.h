#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "emulator.h"

void execute(Chip8 *chip);

// NOP
// NOP used for invalid opcodes
void OP_NULL(Chip8 *chip);

// CLS
// Clear the display
// Set pixel buffer to 0
void OP_00E0(Chip8 *chip);

// RET
// Return from subroutine
// Set PC to top of stack, decrement SP
void OP_00EE(Chip8 *chip);

// JP NNN
// Jump to location NNN
// Set PC to NNN
void OP_1NNN(Chip8 *chip);

// CALL NNN
// Call subroutine at NNN
// Increment SP, put PC on top of stack, set PC to NNN
void OP_2NNN(Chip8 *chip);

// SE Vx, NN
// Skip instruction if Vx == NN
// Increment PC by 2 if equal
void OP_3XNN(Chip8 *chip);

// SNE Vx, NN
// Skip instruction if Vx != NN
// Increment PC by 2 if not equal
void OP_4XNN(Chip8 *chip);

// SE Vx, Vy
// Skip instruction if Vx == Vy
// Increment PC by 2 if equal
void OP_5XY0(Chip8 *chip);

// LD Vx, NN
// Set Vx = NN
void OP_6XNN(Chip8 *chip);

// ADD Vx, NN
// Set Vx to Vx + NN
void OP_7XNN(Chip8 *chip);

// LD Vx, Vy
// Set Vx to Vy
void OP_8XY0(Chip8 *chip);

// OR Vx, Vy
// Set Vx = Vx | Vy
void OP_8XY1(Chip8 *chip);

// AND Vx, Vy
// Set Vx = Vx & Vy
void OP_8XY2(Chip8 *chip);

// XOR Vx, Vy
// Set Vx = Vx ^ Vy
void OP_8XY3(Chip8 *chip);

// ADD Vx, Vy
// Set Vx = Vx + Vy, Vf = 1 if carry else 0
void OP_8XY4(Chip8 *chip);

// SUB Vx, Vy
// Set Vx = Vx - Vy, Vf = 1 if Vx > Vy else 0
void OP_8XY5(Chip8 *chip);

// SHR Vx (SHR Vx, Vy on old systems)
// Shift Vx 1 to the right, Vf = least significant bit of Vx
void OP_8XY6(Chip8 *chip);

// SUBN Vx, Vy
// Set Vx = Vy - Vx, Vf = 1 if Vy > Vx else 0
void OP_8XY7(Chip8 *chip);

// SHL Vx (SHL Vx, Vy on old systems)
// Shift Vx 1 to the left, Vf = most significant bit of Vx
void OP_8XYE(Chip8 *chip);

// SNE Vx, Vy
// Skip instruction if Vx != Vy
// Increment PC by 2 if not equal
void OP_9XY0(Chip8 *chip);

// LD I, NNN
// Set I = NNN
void OP_ANNN(Chip8 *chip);

// JP V0, NNN
// Jump to NNN + V0
void OP_BNNN(Chip8 *chip);

// RND Vx, NN
// Set Vx = rand() & NN
// rand() is between 0x0 and 0xff
void OP_CXNN(Chip8 *chip);

// Draw sprite at (Vx, Vy)
// Get N bytes from memory location I
// Sprites are 8 pixels wide, N pixels tall
// Pixels in buffer get XOR-ed with sprite pixels
void OP_DXYN(Chip8 *chip);

// SKP Vx
// Skip instruction if key with value Vx is pressed
void OP_EX9E(Chip8 *chip);

// SKNP Vx
// Skip instructionif key with value Vx is not pressed
void OP_EXA1(Chip8 *chip);

//

#endif // INSTRUCTIONS_H
