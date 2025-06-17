#ifndef CHIP_H
#define CHIP_H

//4KB RAM
#define RAM_S 4096 
#define REGISTERS_S 16
#define STACK_S 16

#define CHIP_START_ADDR 0x000
#define CHIP_END_ADDR 0x1FF

#define PROGRAMS_START_ADDR 0x200

#define ROM_MAX_S (0xFFF - 0x200 + 1)

typedef struct {
  unsigned char memory[RAM_S];
  unsigned char V[REGISTERS_S]; // Registers V0 - VF
  unsigned short I; //Register to store memory addresses 
  unsigned short PC; //Program Counter
  unsigned char SP; //Stack pointer
  unsigned short stack[STACK_S]; 
} chip8_t;

chip8_t* init_chip();
int load_rom(chip8_t* chip, const char* path);
void chip_step(chip8_t* chip);

#endif
