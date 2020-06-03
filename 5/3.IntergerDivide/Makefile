# simple handwritten makefile for practice

CC       = gcc -Wall -O2 -Werror
SRC      = $(wildcard ./*.c ) 
OBJ      = $(patsubst %.c, %.o, $(SRC))
BIN      = test.exe
LDFLAGS  = -static-libgcc -g3 
CFLAGS   = $(INCS) -g3

.PHONY: all clean
all: $(BIN) 

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN) $(LDFLAGS)

clean:
	${RM} $(OBJ) $(BIN)