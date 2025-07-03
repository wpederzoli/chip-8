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
  memset(chip->gtx, 0, sizeof(chip->gtx));

  chip->I = 0;
  chip->SP = 0;
  chip->DT = 0;

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
    memset(chip->gtx, 0, sizeof(chip->gtx));
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
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
    printf("LD_Vx_KK\n");
    chip->V[Y] = (Z << 4) | W;
    break;
  }
  case ADD: {
    uint8_t kk = (Z << 4) | W;
    chip->V[Y] = chip->V[Y] + kk;
    break;
  }
  case DRW: {
    printf("DRAW - X: %d, Y: %d, Rows: %d, I: 0x%04X\n", chip->V[Y], chip->V[Z],
           W, chip->I);

    uint8_t x = chip->V[Y] % 64;
    uint8_t y = chip->V[Z] % 32;
    chip->V[0xF] = 0;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (uint8_t row = 0; row < W; row++) {
      uint8_t sprite_byte = chip->memory[chip->I + row];
      for (uint8_t col = 0; col < 8; col++) {
        if ((sprite_byte >> (7 - col)) & 1) {
          printf("sprite draw\n");
          uint8_t curr_x = (x + col) % 64;
          uint8_t curr_y = (y + row) % 32;

          if (chip->gtx[curr_y][curr_x]) {
            chip->V[0xF] = 1;
          }
          chip->gtx[curr_y][curr_x] ^= 1;

          if (chip->gtx[curr_y][curr_x]) {
            SDL_FRect rect = {curr_x * 10.f, curr_y * 10.f, 10.f, 10.f};
            SDL_RenderFillRect(renderer, &rect);
          }
        }
      }
    }
    break;
  }
  case LD_I_ADDR: {
    uint16_t addr = (Y << 8) | (Z << 4) | W;
    chip->I = addr;
    break;
  }
  case SE: {
    uint8_t kk = (Z << 4) | W;
    if (chip->V[Y] == kk) {
      chip->PC += 2;
    }
    break;
  }
  case LD_VX_DT: {
    chip->V[Y] = chip->DT;
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
    default:
      break;
    }
    break;
  }
  default:
    break;
  }

  // Since it is 2 bytes sized we move forward 2 steps.
  chip->PC += 2;
}

uint16_t chip_step(chip8_t *chip) {
  if (chip->DT > 0) {
    chip->DT--;
  }

  uint16_t opcode = (chip->memory[chip->PC] << 8) |
                    chip->memory[chip->PC + 1]; // 2 byte opcodes
  printf("PC: %04X | Opcode: %04X\n", chip->PC, opcode);

  return opcode;
}
