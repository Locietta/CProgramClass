#simple handwritten makefile for practice
#
vpath %.h ./include ./include/textbookHeader
vpath %.c ./src ./src/textbookSrc

CC       = gcc -Wall -O2 # -Werror
SRC      = main.c $(wildcard ./src/*.c) $(wildcard ./src/textbookSrc/*.c)
OBJ      = $(patsubst %.c, %.o, $(SRC))
INCS     = -I"./include" -I"./include/textbookHeader"
BIN      = test.exe
LDFLAGS  = -static-libgcc -g3 -mwindows 
CFLAGS   = $(INCS) -g3

.PHONY: all clean
all: $(BIN) 

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN) $(LDFLAGS)

clean:
	${RM} $(OBJ) $(BIN)