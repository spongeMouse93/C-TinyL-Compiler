#include "instrUtils.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REG_NUM 1000

int main(int argc, char *argv[]) {
  FILE *inFile;
  instruction *head, *instr;
  int memory[6];
  int registerFile[MAX_REG_NUM];
  int input, instrCounter = 0; /* counts number of executed instructions */
  if (argc != 2) {
    fprintf(stderr, "Use of command:\n  run <RISC code file>\n");
    exit(EXIT_FAILURE);
  }
  memset(memory, '\0', sizeof(memory));
  inFile = fopen(argv[1], "r");
  if (!inFile) {
    ERROR("Cannot open input file \"%s\"\n", argv[1]);
    exit(EXIT_FAILURE);
  }
  head = readInstructionList(inFile);
  fclose(inFile);
  instr = head;
  while (instr) {
    switch (instr->code) {
    case LOAD:
      registerFile[instr->field1] = memory[instr->field2 - 'a'];
      break;
    case LOADI:
      registerFile[instr->field1] = instr->field2;
      break;
    case STORE:
      memory[instr->field1 - 'a'] = registerFile[instr->field2];
      break;
    case ADD:
      registerFile[instr->field1] = registerFile[instr->field2] + registerFile[instr->field3];
      break;
    case SUB:
      registerFile[instr->field1] = registerFile[instr->field2] - registerFile[instr->field3];
      break;
    case MUL:
      registerFile[instr->field1] = registerFile[instr->field2] * registerFile[instr->field3];
      break;
    case AND:
      registerFile[instr->field1] = registerFile[instr->field2] & registerFile[instr->field3];
      break;
    case OR:
      registerFile[instr->field1] = registerFile[instr->field2] | registerFile[instr->field3];
      break;
    case READ:
      printf("tinyL>> enter value for \"%c\": ", instr->field1);
      scanf("%d", &input);
      memory[instr->field1 - 'a'] = input;
      break;
    case WRITE:
      printf("tinyL>> %c = %d\n", instr->field1, memory[instr->field1 - 'a']);
      break;
    default:
      ERROR("Illegal instructions\n");
    }
    instrCounter++;
    instr = instr->next;
  }
  destroyInstructionList(head);
  return EXIT_SUCCESS;
}
