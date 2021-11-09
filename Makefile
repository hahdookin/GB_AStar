SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

EXE := $(BIN_DIR)/main.gb
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CC := c:\gbdk\bin\lcc

CFLAGS := -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG

.PHONY: all clean

all: $(EXE)
	@((1))

$(EXE): $(OBJ) | $(BIN_DIR)
	@$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	@rm $(OBJ_DIR)/*

# all: main.o Common.o
# 	@c:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -o main.gb main.o Common.o

# main.o: main.c
# 	c:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o main.o main.c

# Common.o: Common.c
# 	c:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG -c -o Common.o Common.c
