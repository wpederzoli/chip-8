#include "chip.h"
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_stdinc.h"
#include <stdlib.h>

chip8_t *init_chip() {
  chip8_t *chip = malloc(sizeof(chip8_t));
  if (chip == NULL) {
    printf("Failed to allocate memory for the chip\n");
    return NULL;
  }

  memset(chip->memory, 0, RAM_S);
  memset(chip->V, 0, REGISTERS_S);
  chip->I = 0;
  chip->SP = 0;

  chip->PC = PROGRAMS_START_ADDR;
  return chip;
}

int load_rom(chip8_t *chip, const char *path) {
  FILE *file = fopen(path, "rb");
  if (file == NULL) {
    printf("Cannot read file: %s", path);
    return -1;
  }

  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  rewind(file);

  if (size > ROM_MAX_S) {
    printf("File is too large for this computer: %ld", size);
    fclose(file);
    return -1;
  }

  fread(&chip->memory[PROGRAMS_START_ADDR], 1, size, file);
  fclose(file);

  return 0;
}

void chip_step(chip8_t *chip) {
  uint16_t opcode = (chip->memory[chip->PC] << 8) |
                    chip->memory[chip->PC + 1]; // 2 byte opcodes
  // opcodes look like 0xXYZW where:
  //  X - Category
  //  Y, Z, W - args (registers, other values, etc)...

  uint8_t X = (opcode >> 12) & 0xF;
  uint8_t Y = (opcode >> 8) & 0xF;
  uint8_t Z = (opcode >> 4) & 0xF;
  uint8_t W = opcode & 0xF;

  // log for now
  printf("PC: 0x%04X | Opcode: 0x%04X (X: 0x%X, Y: 0x%X, Z: 0x%X, W: 0x%X)\n",
         chip->PC, opcode, X, Y, Z, W);

  // Since it is 2 bytes sized we move forward 2 steps.
  chip->PC += 2;
}
