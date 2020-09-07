#simple handwritten makefile for practice

vpath %.h ./include 
vpath %.c ./src 

CC       = gcc -Wall -Wextra -O2
SRC      = main.c $(wildcard ./src/*.c) 
OBJ      = $(patsubst %.c, %.o, $(SRC))
TEST_SRC = test.c $(wildcard ./src/*.c)
TEST_OBJ = $(patsubst %.c, %.o, $(TEST_SRC))
INCS     = -I"./include" 
BIN      = main.exe
TEST_BIN = test.exe
LDFLAGS  = -static-libgcc -g3
CFLAGS   = $(INCS) -g3

.PHONY: all clean test
all: $(BIN) 
test: $(TEST_BIN)
	./test

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN) $(LDFLAGS)

$(TEST_BIN): $(TEST_OBJ)
	$(CC) $(TEST_OBJ) -o $(TEST_BIN) $(LDFLAGS)

clean:
	${RM} $(OBJ) $(BIN) $(TEST_OBJ) $(TEST_BIN)