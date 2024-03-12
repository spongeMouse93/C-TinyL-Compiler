#ifndef INSTR_H
#define INSTR_H

typedef enum { LOAD, LOADI, STORE, ADD, SUB, MUL, OR, AND, READ, WRITE } opCode;

typedef struct instructionInfo instruction;

struct instructionInfo {
  opCode code;
  int field1;
  int field2;
  int field3;
  instruction *prev;
  instruction *next;
  char critical;
};

#endif
