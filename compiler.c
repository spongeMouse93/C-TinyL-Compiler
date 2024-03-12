#include "instr.h"
#include "instrUtils.h"
#include "utils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_BUFFER_SIZE 500
#define EMPTY_FIELD 0xFFFFF
#define token *buffer

/* GLOBALS */
static char *buffer = NULL;  /* read buffer */
static int regnum = 1;       /* for next free virtual register number */
static FILE *outFile = NULL; /* output of code generation */

/* Utilities */
static void codeGen(opCode opcode, int field1, int field2, int field3);
static inline void nextToken();
static inline int nextReg();
static inline int isDigit(char c);
static inline int toDigit(char c);
static inline int isIdentifier(char c);
static char *readInput(FILE *f);

/* Routines for recursive descending parser LL(1) */
static void program();
static void stmtList();
static void morestmts();
static void stmt();
static void assign();
static void read();
static void print();
static int expr();
static int variable();
static int digit();

/*************************************************************************/
/* Definitions for recursive descending parser LL(1)                     */
/*************************************************************************/
static int digit() {
  int reg;
  if (!isDigit(token)) {
    ERROR("Expected digit\n");
    exit(EXIT_FAILURE);
  }
  reg = nextReg();
  codeGen(LOADI, reg, toDigit(token), EMPTY_FIELD);
  nextToken();
  return reg;
}

static int variable() {
  if (!isIdentifier(token)) {
    ERROR("Expected variable\n");
    exit(EXIT_FAILURE);
  }
  int reg = nextReg();
  codeGen(LOAD, reg, token, EMPTY_FIELD);
  return reg;
}

static int expr() {
  int reg, left_reg, right_reg;

  switch (token) {
  case '+':
    nextToken();
    left_reg = expr();
    right_reg = expr();
    reg = nextReg();
    codeGen(ADD, reg, left_reg, right_reg);
    return reg;
  case '-':
    nextToken();
    left_reg = expr();
    right_reg = expr();
    reg = nextReg();
    codeGen(SUB, reg, left_reg, right_reg);
    return reg;
  case '*':
    nextToken();
    left_reg = expr();
    right_reg = expr();
    reg = nextReg();
    codeGen(MUL, reg, left_reg, right_reg);
    return reg;
  case 'a':
  case 'b':
  case 'c':
  case 'd':
  case 'e':
    return variable();
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    return digit();
  default:
    ERROR("Symbol %c unknown\n", token);
    exit(EXIT_FAILURE);
  }
}

static void assign() {
  char var;
  if (isIdentifier(token)) {
    var = token;
    printf("\nVariable: %c\n", var);
    nextToken();
    if (token == '=') {
      nextToken();
      int a = expr();
      codeGen(STORE, var, a, EMPTY_FIELD);
    }
  } else
    ERROR("Assign error: %c unknown\n", token);
}

static void read() {
  if (token == '?') {
    nextToken();
    codeGen(READ, token, EMPTY_FIELD, EMPTY_FIELD);
  }
}

static void print() {
  if (token == '%') {
    nextToken();
    if (isIdentifier(token) == 1) {
      codeGen(WRITE, token, EMPTY_FIELD, EMPTY_FIELD);
      nextToken();
    } else
      ERROR("Error.  Current input symbol is %c\n", token);
  }
}

static void stmt() {
  switch (token) {
  case 'a':
  case 'b':
  case 'c':
  case 'd':
  case 'e':
    assign();
    break;
  case '?':
    read();
    nextToken();
    break;
  case '%':
    print();
    break;
  }
}

static void morestmts() {
  if (token == ';') {
    nextToken();
    stmtList();
  }
}

static void stmtList() {
  stmt();
  morestmts();
}

static void program() {
  /* YOUR CODE GOES HERE */
  stmtList();
  expr();
  if (token != '!') {
    ERROR("Program error.  Current input symbol is %c\n", token);
    exit(EXIT_FAILURE);
  }
}

/*************************************************************************/
/* Utility definitions                                                   */
/*************************************************************************/
static void codeGen(opCode opcode, int field1, int field2, int field3) {
  instruction instr;
  if (!outFile) {
    ERROR("File error\n");
    exit(EXIT_FAILURE);
  }
  instr.code = opcode;
  instr.field1 = field1;
  instr.field2 = field2;
  instr.field3 = field3;
  printInstruction(outFile, &instr);
}

static inline void nextToken() {
  if (*buffer == '\0') {
    ERROR("End of program input\n");
    exit(EXIT_FAILURE);
  }
  printf("%c ", *buffer);
  if (*buffer == ';')
    printf("\n");
  buffer++;
  if (*buffer == '\0') {
    ERROR("End of program input\n");
    exit(EXIT_FAILURE);
  }
  if (*buffer == '!')
    printf(".\n");
}

static inline int nextReg() { return regnum++; }

static inline int isDigit(char c) {
  if (c >= '0' && c <= '9')
    return 1;
  return 0;
}

static inline int toDigit(char c) {
  if (isDigit(c))
    return c - '0';
  WARNING("Non-digit passed to %s, returning zero\n", __func__);
  return 0;
}

static inline int isIdentifier(char c) {
  if (c >= 'a' && c <= 'e')
    return 1;
  return 0;
}

static char *readInput(FILE *f) {
  size_t size, i;
  char *b;
  int c;

  for (b = NULL, size = 0, i = 0;;) {
    if (i >= size) {
      size = (size == 0) ? MAX_BUFFER_SIZE : size * 2;
      b = (char *)realloc(b, size * sizeof(char));
      if (!b) {
        ERROR("Realloc failed\n");
        exit(EXIT_FAILURE);
      }
    }
    c = fgetc(f);
    if (EOF == c) {
      b[i] = '\0';
      break;
    }
    if (isspace(c))
      continue;
    b[i] = c;
    i++;
  }
  return b;
}

/*************************************************************************/
/* Main function                                                         */
/*************************************************************************/

int main(int argc, char *argv[]) {
  const char *outfilename = "tinyL.out";
  char *input;
  FILE *inFile;
  printf("------------------------------------------------\n");
  printf("CS314 compiler for tinyL\n");
  printf("------------------------------------------------\n");
  if (argc != 2) {
    ERROR("Use of command:\n  compile <tinyL file>\n");
    exit(EXIT_FAILURE);
  }
  inFile = fopen(argv[1], "r");
  if (!inFile) {
    ERROR("Cannot open input file \"%s\"\n", argv[1]);
    exit(EXIT_FAILURE);
  }
  outFile = fopen(outfilename, "w");
  if (!outFile) {
    ERROR("Cannot open output file \"%s\"\n", outfilename);
    exit(EXIT_FAILURE);
  }
  input = readInput(inFile);
  buffer = input;
  program();
  printf("\nCode written to file \"%s\".\n\n", outfilename);
  free(input);
  fclose(inFile);
  fclose(outFile);
  return EXIT_SUCCESS;
}
