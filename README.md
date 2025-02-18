# Chip-8

This is an Emulator for the Chip-8 instruction set I am writing.
The goal is to have an emulator that can read binary files and execute them reasonably accurately.

## Test ROMs

I am using some test ROMs I found to test the functionality (https://github.com/Timendus/chip8-test-suite).

## State

The emulator now supports all instructions except witing for keyboard input. Test ROMs 1 through 4 work fine now.
So logic and arithmetic are properly implemented.
Once the emulator is functional, I will clean up the code (and maybe the functionality) in a version 2.

## Usage

Do not use this if you expect something good. Otherwise, download the repo
and build the project with Cmake in the build directory.
Then run the executable with a binary file.
