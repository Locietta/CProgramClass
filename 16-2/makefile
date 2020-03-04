#simple handwritten makefile for practice
vpath %.h ./include
vpath %.c ./lib

CC       = gcc -Wall -O2
SRC      = main.c $(wildcard ./lib/*.c)
OBJ      = $(patsubst %.c, %.o, $(SRC))
LIBS     = -static-libgcc -g3
INCS     = -I"./include"
BIN      = test.exe
CFLAGS   = $(INCS) -g3
RM       = rm -f

.PHONY: all all-before all-after clean clean-custom
all: all-before $(BIN) all-after
clean: clean-custom
	${RM} $(OBJ) $(BIN)
$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN) $(LIBS)
	
%.o : %.c
	$(CC) -c $^ -o $@ $(CFLAGS)
main.o: main.c
exceptio.o: exceptio.c
genlib.o: genlib.c
random.o: random.c
simpio.o: simpio.c
strlib.o: strlib.c
