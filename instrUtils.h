#include "instr.h"
#include <stdio.h>

#ifndef INSTR_UTILS_H
#define INSTR_UTILS_H

void printInstruction(FILE *outFile, instruction *instr);
void printInstructionList(FILE *outFile, instruction *instr);
instruction *readInstruction(FILE *inFile);
instruction *readInstructionList(FILE *inFile);
instruction *lastInstruction(instruction *instr);
void destroyInstructionList(instruction *instr);

#endif
