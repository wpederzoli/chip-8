#ifndef REGISTERS_H
#define REGISTERS_H

// Clear screen
#define CLS 0x00E0
// Return to address at top of the stack
#define RET 0x00EE
// Jump to nnn address
#define JP 0x1
//Set Vx = kk
#define LD_VX_KK 0x6
//Set Vx = Vy
#define LD_VX_VY 0x0
//Set Vx = Vx + kk
#define ADD 0x7
//Logical ops
#define LOGICAL 0x8 
//Set Vx = Vx OR Vy
#define OR 0x1
//Set Vx = Vx & Vy
#define AND 0x2
//Set Vx = Vx XOR Vy
#define XOR 0x3
//Set Vx = Vx + Vy, set VF = carry.
#define ADD_VX_VY 0x4
//Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
#define DRW 0xD
//Skip next instruction iv Vx == kk
#define SE 0x3
//Set Vx to DT
#define LD_VX_DT 0xF
//Set I = nnn
#define LD_I_ADDR 0xA

#endif
