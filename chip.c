#include "chip.h"
#include "registers.h"
#include <stdio.h>
#include <string.h>

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
    printf("File is too large for this computer: %ld\n", size);
    fclose(file);
    return -1;
  }

  fread(&chip->memory[PROGRAMS_START_ADDR], 1, size, file);
  fclose(file);

  return 0;
}

// opcodes look like 0xXYZW where:
//  X - Category
//  Y, Z, W - args (registers, other values, etc)...

void handle_opcode(chip8_t *chip, uint16_t op, SDL_Renderer *renderer) {
  uint8_t X = (op >> 12) & 0xF;
  uint8_t Y = (op >> 8) & 0xF;
  uint8_t Z = (op >> 4) & 0xF;
  uint8_t W = op & 0xF;

  switch (X) {
  case CLS:
    printf("Clear screen\n");
    SDL_RenderClear(renderer);
    break;
  case RET:
    printf("Return to top stack address\n");
    chip->PC = chip->stack[chip->SP];
    chip->SP--;
    return;
  case JP: {
    uint16_t nnn = (Y << 8) | (Z << 4) | W;
    printf("Jump to: %0x04X", nnn);
    chip->PC = nnn;
    return;
  }
  case LD_VX_KK: {
    chip->V[Y] = (Z << 4) | W;
    break;
  }
  case ADD: {
    uint8_t kk = (Z << 4) | W;
    chip->V[Y] = chip->V[Y] + kk;
    break;
  }
  case LOGICAL: {
    switch (W) {
    case OR: {
      chip->V[Y] = chip->V[Y] | chip->V[Z];
      break;
    }
    case AND: {
      chip->V[Y] = chip->V[Y] & chip->V[Z];
      break;
    }
    case XOR: {
      chip->V[Y] = chip->V[Y] ^ chip->V[Z];
      break;
    }
    case LD_VX_VY: {
      chip->V[Y] = chip->V[Z];
      break;
    }
    case ADD_VX_VY: {
      uint16_t sum = chip->V[Y] + chip->V[Z];
      chip->V[Y] = sum & 0xFF;
      chip->V[0xF] = (sum > 0xFF) ? 1 : 0;
      break;
    }
    }
    break;
  }
  }

  // Since it is 2 bytes sized we move forward 2 steps.
  chip->PC += 2;
}

uint16_t chip_step(chip8_t *chip) {
  uint16_t opcode = (chip->memory[chip->PC] << 8) |
                    chip->memory[chip->PC + 1]; // 2 byte opcodes

  return opcode;
}
