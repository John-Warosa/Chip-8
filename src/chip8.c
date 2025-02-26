#include "chip8.h"
#include "chip8_constants.h"
#include "input.h"
#include "instructions.h"
#include "romload.h"
#include <string.h>

static void load_font(u8 ram[]);
static void update_timers(Chip8 *chip);
static u16 get_opcode(const u8 ram[], u16 PC);

// Font set that gets loade into RAM
// Each number is a sprite 8 pixels wide and 5 pixels tall
static const u8 fontset[CHIP8_FONT_SIZE] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void Chip8_init(Chip8 *chip, const char *filename) {
  chip->PC = CHIP8_PROGRAM_START;

  load_font(chip->ram);
  load_rom(chip->ram, CHIP8_PROGRAM_START, filename);
}

static void load_font(u8 ram[]) {
  for (size_t i = 0; i < CHIP8_FONT_SIZE; ++i) {
    ram[CHIP8_FONT_START + i] = fontset[i];
  }
}

void Chip8_restart(Chip8 *chip, const char *filename) {
  memset(chip, 0, sizeof(*chip));
  load_font(chip->ram);
  load_rom(chip->ram, CHIP8_PROGRAM_START, filename);

  chip->PC = CHIP8_PROGRAM_START;
  chip->vblank = true;
}

void Chip8_step(Chip8 *chip, u16 quirks, bool updateTimers) {
  if (updateTimers) {
    update_timers(chip);
  }

  chip->keys = get_keys();

  chip->opcode = get_opcode(chip->ram, chip->PC);
  chip->PC += 2;

  execute_instruction(chip, quirks);
}

static void update_timers(Chip8 *chip) {
  if (chip->delay) {
    --chip->delay;
  }

  if (chip->sound) {
    --chip->sound;
  }
}

static u16 get_opcode(const u8 ram[], u16 PC) {
  return (ram[PC] << 8) + ram[PC + 1];
}

int write_chip8_info(char *buf, size_t bufsize, const Chip8 *chip);
