CCFLAGS = #-ggdb -Wall

all: compile run

compile: compiler.c instrUtils.c instrUtils.h utils.c utils.h
	gcc $(CCFLAGS) compiler.c instrUtils.c utils.c -o compile

run: interpreter.c instrUtils.c instrUtils.h utils.c utils.h
	gcc $(CCFLAGS) interpreter.c instrUtils.c utils.c utils.h -o run

# this will reformat your code according to the linux guidelines.
# be careful when using this command!
pretty: compiler.c instrUtils.c instrUtils.h utils.c utils.h
	indent -linux compiler.c
	indent -linux instr.h
	indent -linux instrUtils.c instrUtils.h
	indent -linux utils.c utils.h
	indent -linux interpreter.c

clean:
	rm -rf compile run tinyL.out
