# Chip-8

This is an Emulator for the Chip-8 instruction set I am writing.
The goal is to have an emulator that can read binary files and execute them reasonably accurately.

## Test ROMs

I am using some test ROMs I found to test the functionality (https://github.com/Timendus/chip8-test-suite).
I also included some game ROMs I found somewhere that work.

## State

I consider this emulator complete (at least version 1). All test ROMs for the Chip-8 specifically pass,
except for test 4-Flags due to the Vf reset quirk.

## Future

The next version of this emulator will feature a much better user interface, including a memory viewer and the ability to pause the emulation.
I will also implement the option to toggle the quirks in case you need certain behaviour

## Usage

Do not use this if you expect something good. And definitely do not look at the code, it's horrific and probably very inefficient.

### Dependencies

If you do want to try out this project, you will need CMake, raylib and (probably) a compiler that supports C23
Simply install CMake and raylib with hombrew, then build the project.
Since I only sparsely use the new C23 features, I might 'downgrade' to C11 or C99 for the next version.

### Building

Otherwise, download the repo
and build the project with Cmake in a build directory.
    `mkdir build
    cd build
    cmake ..
    cmake --build .`
Then run the application with a binary file (usually ending in .ch8)
    `./chip8 filename.ch8`
