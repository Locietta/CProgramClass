#simple handwritten makefile for practice
#
vpath %.h ./include ./include/textbookHeader
vpath %.c ./src ./src/textbookSrc

CC       = gcc -Wall -O2
SRC      = main.c $(wildcard ./src/*.c) 
OBJ      = $(patsubst %.c, %.o, $(SRC))
INCS     = -I"./include" 
BIN      = test.exe
LDFLAGS  = -static-libgcc -g3
CFLAGS   = $(INCS) -g3

.PHONY: all clean
all: $(BIN) 

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN) $(LDFLAGS)

linkedList.o: 
main.o: 
# test.o:

clean:
	${RM} $(OBJ) $(BIN)