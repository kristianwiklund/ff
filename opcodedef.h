#ifndef OPCODEDEF_H
#define OPCODEDEF_H

#define LOADIM  0x00
#define LOAD    0x01
#define LOADA   0x02
#define LOADS   0x03
#define TEST    0x04
#define POP     0x05
#define PUSH    0x06
#define BEQ     0x07
#define BLT     0x08
#define BGT     0x09
#define BLE     0x0a
#define BGE     0x0b
#define LOADST  0x0c
#define LOADAT  0x0d
#define STORE   0x0e
#define STOREA  0x0f
#define STORES  0x10
#define STOREST 0x11
#define STOREAT 0x12
#define PD      0x13
#define PA      0x14
#define PT      0x15
#define DP      0x16
#define AP      0x17
#define TP      0x18
#define CALL    0x19
#define CALLC   0x1a
#define TTA     0x1b
#define TTT     0x1c
#define TFA     0x1d
#define TFT     0x1e
#define ADT     0x1f
#define SBT     0x20
#define CMP     0x21
#define RTS     0x22
#define BNE     0x23
#define JMP     0x24
#define CLR     0x25
#define NOP     0x26
#define BRA     0x27

#define NROPCODES       0x28

#endif OPCODEDEF_H
