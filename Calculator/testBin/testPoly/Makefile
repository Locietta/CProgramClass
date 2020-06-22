# simple handwritten makefile for practice

# vpath %.h ./include ./include/ADT ./include/textbookHeader
# vpath %.c ./src ./src/CDT ./src/textbookSrc # ./testBin

CC       = gcc -Wall -O2 # -Werror
SRC      = $(wildcard ./*.c) 
OBJ      = $(patsubst %.c, %.o, $(SRC))
# INCS     = -I"./include" -I"./include/ADT" -I"./include/textbookHeader"
BIN      = test.exe
LDFLAGS  = -static-libgcc -g3 # -mwindows
CFLAGS   = $(INCS) -g3

.PHONY: all clean
all: $(BIN) 

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN) $(LDFLAGS)

# (.o: .c)



clean:
	${RM} $(OBJ) $(BIN)