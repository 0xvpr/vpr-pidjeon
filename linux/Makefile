TARGET=payload_injector

CC=gcc
LD=gcc
CFLAGS=-std=c99 -Wall -Wextra -O2 -s -DRELEASE
LDFLAGS=

BIN=bin

SRC=src
OBJ=build
32OBJ=$(OBJ)/x86
64OBJ=$(OBJ)/x64
SRCS=$(wildcard $(SRC)/*.c)
32OBJS=$(patsubst $(SRC)/%.c, $(32OBJ)/%.o, $(SRCS))
64OBJS=$(patsubst $(SRC)/%.c, $(64OBJ)/%.o, $(SRCS))

INCLUDE=include
INCLUDES=$(addprefix -I, $(INCLUDE))

all: $(TARGET)

$(TARGET): $(32OBJ) $(32OBJS) $(64OBJ) $(64OBJS) $(OBJ) $(BIN)
	$(LD) $(32OBJS) $(LDFLAGS) -o $(BIN)/$(TARGET)x86
	$(LD) $(64OBJS) $(LDFLAGS) -o $(BIN)/$(TARGET)x64

$(32OBJS): $(32OBJ)/%.o : $(SRC)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $^ -o $@

$(64OBJS): $(64OBJ)/%.o : $(SRC)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $^ -o $@

$(32OBJ):
	mkdir -p $@

$(64OBJ):
	mkdir -p $@

$(BIN):
	mkdir -p $@

clean:
	rm -fr $(OBJ)/*
	rm -fr $(BIN)/$(TARGET)*
