# Test Target
TARGET = arisr_test

# Directories
SRC_DIR = ../source
INC_DIR = ../include
BIN_DIR = ../bin
BUILD_DIR = ../build

# Source files
SRCS = $(filter-out $(SRC_DIR)/main.c, $(wildcard $(SRC_DIR)/*.c)) main.c
OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(notdir $(SRCS)))

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -I$(INC_DIR) -DARISR_PROTO_PARTIAL_FUNCTIONS
VPATH = $(SRC_DIR):.

# Default target
all: $(BIN_DIR)/$(TARGET)

# Link executable
$(BIN_DIR)/$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

# Compilation pattern rule
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Combined directory creation rule
$(BIN_DIR) $(BUILD_DIR):
	mkdir -p $@

# Clean
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)/$(TARGET)

# Run
run: $(BIN_DIR)/$(TARGET)
	./$(BIN_DIR)/$(TARGET)