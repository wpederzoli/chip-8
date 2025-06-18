#ifndef CHIP_H
#define CHIP_H

// 4KB RAM
#define RAM_S 4096
#define REGISTERS_S 16
#define STACK_S 16

#define CHIP_START_ADDR 0x000
#define CHIP_END_ADDR 0x1FF

#define PROGRAMS_START_ADDR 0x200

#define ROM_MAX_S (0xFFF - 0x200 + 1)

#include <stdlib.h>

typedef struct {
  uint8_t memory[RAM_S];
  uint8_t V[REGISTERS_S]; // Registers V0 - VF
  uint16_t I;             // Register to store memory addresses
  uint16_t PC;            // Program Counter
  uint8_t SP;             // Stack pointer
  uint16_t stack[STACK_S];
} chip8_t;

chip8_t *init_chip();
int load_rom(chip8_t *chip, const char *path);
void chip_step(chip8_t *chip);

#endif
