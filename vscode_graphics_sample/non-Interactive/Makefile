#simple handwritten makefile for practice
#
vpath %.h ./include ./include/textbookHeader
vpath %.c ./src ./src/textbookSrc

CC       = gcc -Wall -O2
SRC      = main.c $(wildcard ./src/*.c) $(wildcard ./src/textbookSrc/*.c)
OBJ      = $(patsubst %.c, %.o, $(SRC))
INCS     = -I"./include" -I"./include/textbookHeader"
BIN      = test.exe
LDFLAGS  = -static-libgcc -g3 -mwindows -lKernel32
CFLAGS   = $(INCS) -g3

.PHONY: all clean
all: $(BIN) 

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN) $(LDFLAGS)

exceptio.o:
genlib.o:
graphics.o:
random.o:
simpio.o:
strlib.o:
main.o:
# test.o:

clean:
	${RM} $(OBJ) $(BIN)