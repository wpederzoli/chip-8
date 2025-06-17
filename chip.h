#ifndef CHIP_H
#define CHIP_H

#define RAM_S 4096 // 4KB
#define REGISTERS_S 16
#define STACK_S 16

#define CHIP_START_ADDR 0x000
#define CHIP_END_ADDR 0x1FF

#define PROGRAMS_START_ADDR 0x200

typedef struct {
  unsigned char memory[RAM_S];
  unsigned char V[REGISTERS_S]; // Registers V0 - VF
  unsigned short I; //Register to store memory addresses 
  unsigned short PC; //Program Counter
  unsigned char SP; //Stack pointer
  unsigned short stack[STACK_S]; 
} chip8_t;

chip8_t* init_chip();

#endif
