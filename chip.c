#include "chip.h"
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_log.h"
#include "SDL3/SDL_stdinc.h"
#include <stdlib.h>

chip8_t* init_chip() {
  chip8_t* chip = malloc(sizeof(chip8_t));
  if(chip == NULL) {
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
