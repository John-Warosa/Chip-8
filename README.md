# Chip-8

This is an Emulator for the Chip-8 instruction set I am writing.
The goal is to have an emulator that can read binary files and execute them reasonably accurately.

## Test ROMs

I am using some test ROMs I found to test the functionality (https://github.com/Timendus/chip8-test-suite).

## State

Right now, the emulator passes the first 2 test ROMs and implements all instructions up until the draw intruction.
Once the emulator is functional, I will clean up the code (and maybe the functionality) in a version 2.

## Usage

Do not use this if you expect something good. Otherwise, download the repo
and build the project with Cmake in the build directory.
Then run the executable with a binary file.
