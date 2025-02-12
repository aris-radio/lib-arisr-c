# Load version information from the .version file
VERSION_FILE = .version
LIB_NAME = $(shell grep '^LibraryName=' $(VERSION_FILE) | cut -d'=' -f2)
MAJOR = $(shell grep '^Major=' $(VERSION_FILE) | cut -d'=' -f2)
MINOR = $(shell grep '^Minor=' $(VERSION_FILE) | cut -d'=' -f2)
PATCH = $(shell grep '^Patch=' $(VERSION_FILE) | cut -d'=' -f2)
VERSION = $(MAJOR).$(MINOR).$(PATCH)
LIB_VERSIONED = $(LIB_NAME).$(VERSION)
LIB_MAJOR = $(LIB_NAME).$(MAJOR)

# Directories
SRC_DIR = source
INC_DIR = include
BIN_DIR = bin
BUILD_DIR = build

# Source files and object files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -fPIC -I$(INC_DIR) -DARISR_PROTO_PARTIAL_FUNCTIONS
LDFLAGS = -shared

# Default target
all: $(BIN_DIR)/$(LIB_VERSIONED) symlinks

# Build the shared library with versioning
$(BIN_DIR)/$(LIB_VERSIONED): $(OBJS) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ -o $@

# Create symbolic links for the shared library
symlinks: $(BIN_DIR)/$(LIB_VERSIONED)
	ln -sf $(LIB_VERSIONED) $(BIN_DIR)/$(LIB_MAJOR)
	ln -sf $(LIB_MAJOR) $(BIN_DIR)/$(LIB_NAME)

# Compile object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure necessary directories exist
$(BIN_DIR) $(BUILD_DIR):
	mkdir -p $@

# Clean generated files
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)/$(LIB_NAME) $(BIN_DIR)/$(LIB_MAJOR) $(BIN_DIR)/$(LIB_VERSIONED)
