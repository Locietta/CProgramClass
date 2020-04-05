#simple handwritten makefile for practice
#
vpath %.h ./include ./include/textbookLib
vpath %.c ./src

CC       = gcc -Wall -O2
SRC      = main.c $(wildcard ./src/*.c)
OBJ      = $(patsubst %.c, %.o, $(SRC))
INCS     = -I"./include" -I"./include/textbookLib"
BIN      = test.exe
LDFLAGS  = -static-libgcc -g3
CFLAGS   = $(INCS) -g3

.PHONY: all clean
all: $(BIN) 

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN) $(LDFLAGS)
exceptio.o:
genlib.o:
random.o:
simpio.o: 
strlib.o: 
queue.o: 
main.o: 

clean:
	${RM} $(OBJ) $(BIN)