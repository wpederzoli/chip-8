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

int load_rom(chip8_t* chip, const char* path) {
  FILE* file = fopen(path, "rb");
  if(file == NULL) {
    printf("Cannot read file: %s", path);
    return -1;
  }

  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  rewind(file);

  if(size > ROM_MAX_S) {
    printf("File is too large for this computer: %ld", size);
    fclose(file);
    return -1;
  }

  fread(&chip->memory[PROGRAMS_START_ADDR], 1, size, file) ;
  fclose(file);
  
  return 0;
}
