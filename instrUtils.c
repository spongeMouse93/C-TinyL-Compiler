#include "instrUtils.h"
#include "instr.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printInstruction(FILE *outFile, instruction *instr) {
  if (!outFile) {
    ERROR("File error\n");
    exit(EXIT_FAILURE);
  }
  if (instr) {
    switch (instr->code) {
    case LOAD:
      fprintf(outFile, "LOAD r%d %c\n", instr->field1, instr->field2);
      break;
    case LOADI:
      fprintf(outFile, "LOADI r%d #%d\n", instr->field1, instr->field2);
      break;
    case STORE:
      fprintf(outFile, "STORE %c r%d\n", instr->field1, instr->field2);
      break;
    case ADD:
      fprintf(outFile, "ADD r%d r%d r%d\n", instr->field1, instr->field2, instr->field3);
      break;
    case SUB:
      fprintf(outFile, "SUB r%d r%d r%d\n", instr->field1, instr->field2, instr->field3);
      break;
    case MUL:
      fprintf(outFile, "MUL r%d r%d r%d\n", instr->field1, instr->field2, instr->field3);
      break;
    case READ:
      fprintf(outFile, "READ %c\n", instr->field1);
      break;
    case WRITE:
      fprintf(outFile, "WRITE %c\n", instr->field1);
      break;
    case AND:
      fprintf(outFile, "AND r%d r%d r%d\n", instr->field1, instr->field2, instr->field3);
      break;
    case OR:
      fprintf(outFile, "OR r%d r%d r%d\n", instr->field1, instr->field2, instr->field3);
      break;
    default:
      ERROR("Illegal instructions\n");
    }
  }
}

void printInstructionList(FILE *outFile, instruction *instr) {
  if (!outFile) {
    ERROR("File error\n");
    exit(EXIT_FAILURE);
  }
  if (!instr) {
    ERROR("No instructions\n");
    exit(EXIT_FAILURE);
  }
  while (instr) {
    printInstruction(outFile, instr);
    instr = instr->next;
  }
}

instruction *readInstruction(FILE *inFile) {
  static char instrBuffer[100];
  instruction *instr = NULL;
  char dummy;
  if (!inFile) {
    ERROR("File error\n");
    exit(EXIT_FAILURE);
  }
  instr = (instruction *)calloc(1, sizeof(instruction));
  if (!instr) {
    ERROR("Calloc failed\n");
    exit(EXIT_FAILURE);
  }
  instr->prev = NULL;
  instr->next = NULL;
  fscanf(inFile, "%99s", instrBuffer);
  if (strnlen(instrBuffer, sizeof(instrBuffer)) == 0) {
    free(instr);
    return NULL;
  }
  if (!strcmp(instrBuffer, "LOAD")) {
    instr->code = LOAD;
    fscanf(inFile, "%s", instrBuffer);
    sscanf(instrBuffer, "%c%d", &dummy, &(instr->field1));
    fscanf(inFile, "%s", instrBuffer);
    instr->field2 = *instrBuffer;
  } else if (!strcmp(instrBuffer, "LOADI")) {
    instr->code = LOADI;
    fscanf(inFile, "%s", instrBuffer);
    sscanf(instrBuffer, "%c%d", &dummy, &(instr->field1));
    fscanf(inFile, "%s", instrBuffer);
    sscanf(instrBuffer, "%c%d", &dummy, &(instr->field2));
  } else if (!strcmp(instrBuffer, "STORE")) {
    instr->code = STORE;
    fscanf(inFile, "%s", instrBuffer);
    instr->field1 = *instrBuffer;
    fscanf(inFile, "%s", instrBuffer);
    sscanf(instrBuffer, "%c%d", &dummy, &(instr->field2));
  } else if (!strcmp(instrBuffer, "ADD")) {
    instr->code = ADD;
    fscanf(inFile, "%s", instrBuffer);
    sscanf(instrBuffer, "%c%d", &dummy, &(instr->field1));
    fscanf(inFile, "%s", instrBuffer);
    sscanf(instrBuffer, "%c%d", &dummy, &(instr->field2));
    fscanf(inFile, "%s", instrBuffer);
    sscanf(instrBuffer, "%c%d", &dummy, &(instr->field3));
  } else if (!strcmp(instrBuffer, "SUB")) {
    instr->code = SUB;
    fscanf(inFile, "%s", instrBuffer);
    sscanf(instrBuffer, "%c%d", &dummy, &(instr->field1));
    fscanf(inFile, "%s", instrBuffer);
    sscanf(instrBuffer, "%c%d", &dummy, &(instr->field2));
    fscanf(inFile, "%s", instrBuffer);
    sscanf(instrBuffer, "%c%d", &dummy, &(instr->field3));
  } else if (!strcmp(instrBuffer, "MUL")) {
    instr->code = MUL;
    fscanf(inFile, "%s", instrBuffer);
    sscanf(instrBuffer, "%c%d", &dummy, &(instr->field1));
    fscanf(inFile, "%s", instrBuffer);
    sscanf(instrBuffer, "%c%d", &dummy, &(instr->field2));
    fscanf(inFile, "%s", instrBuffer);
    sscanf(instrBuffer, "%c%d", &dummy, &(instr->field3));
  } else if (!strcmp(instrBuffer, "AND")) {
    instr->code = AND;
    fscanf(inFile, "%s", instrBuffer);
    sscanf(instrBuffer, "%c%d", &dummy, &(instr->field1));
    fscanf(inFile, "%s", instrBuffer);
    sscanf(instrBuffer, "%c%d", &dummy, &(instr->field2));
    fscanf(inFile, "%s", instrBuffer);
    sscanf(instrBuffer, "%c%d", &dummy, &(instr->field3));
  } else if (!strcmp(instrBuffer, "OR")) {
    instr->code = OR;
    fscanf(inFile, "%s", instrBuffer);
    sscanf(instrBuffer, "%c%d", &dummy, &(instr->field1));
    fscanf(inFile, "%s", instrBuffer);
    sscanf(instrBuffer, "%c%d", &dummy, &(instr->field2));
    fscanf(inFile, "%s", instrBuffer);
    sscanf(instrBuffer, "%c%d", &dummy, &(instr->field3));
  } else if (!strcmp(instrBuffer, "READ")) {
    instr->code = READ;
    fscanf(inFile, "%s", instrBuffer);
    instr->field1 = *instrBuffer;
  } else if (!strcmp(instrBuffer, "WRITE")) {
    instr->code = WRITE;
    fscanf(inFile, "%s", instrBuffer);
    instr->field1 = *instrBuffer;
  } else {
    free(instr);
    return NULL;
  }
  return instr;
}

instruction *readInstructionList(FILE *inFile) {
  instruction *instr, *head, *tail;
  if (!inFile) {
    ERROR("File error\n");
    exit(EXIT_FAILURE);
  }
  head = tail = NULL;
  while ((instr = readInstruction(inFile))) {
    if (!head) {
      head = tail = instr;
      continue;
    }
    instr->prev = tail;
    instr->next = NULL;
    tail->next = instr;
    tail = instr;
  }
  return head;
}

instruction *lastInstruction(instruction *instr) {
  if (!instr) {
    ERROR("No instructions\n");
    exit(EXIT_FAILURE);
  }
  while (instr->next)
    instr = instr->next;
  return instr;
}

void destroyInstructionList(instruction *instr) {
  instruction *i;
  if (!instr)
    return;
  while (instr) {
    i = instr;
    instr = instr->next;
    free(i);
  }
}
