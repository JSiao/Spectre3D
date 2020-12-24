# INCLUDE_FILES=-I/usr/include
INC=./inc
SRC=./src
BIN=./bin
SRCS:=$(wildcard $(SRC)/*.c)
OBJS:=$(patsubst $(SRC)/%.c, $(BIN)/%.o, $(SRCS))
CC=x86_64-w64-mingw32-gcc 
CFLAGS=--std=c11 -I$(INC) -g
LFLAGS=-mwindows -lkernel32 
EXE=$(BIN)/spectre.exe

all: $(EXE)

$(EXE): $(OBJS) | $(BIN)
	$(CC) $(LFLAGS) $^ -o $@
	rm $(OBJS)

$(BIN)/%.o: $(SRC)/%.c 
	$(CC) $(CFLAGS) -c $< -o $@

run: $(EXE)
	$<

clean:
	rm $(OBJS) $(EXE)
