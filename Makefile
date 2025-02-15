# Load version information from the .version file
VERSION_FILE = .version
LIB_NAME = $(shell grep '^LibraryName=' $(VERSION_FILE) | cut -d'=' -f2)
MAJOR = $(shell grep '^Major=' $(VERSION_FILE) | cut -d'=' -f2)
MINOR = $(shell grep '^Minor=' $(VERSION_FILE) | cut -d'=' -f2)
PATCH = $(shell grep '^Patch=' $(VERSION_FILE) | cut -d'=' -f2)

VERSION = $(MAJOR).$(MINOR).$(PATCH)
LIB_SHARED = $(LIB_NAME).so
LIB_STATIC = $(LIB_NAME).a
LIB_WIN_SHARED = $(LIB_NAME).dll
LIB_WIN_STATIC = $(LIB_NAME).lib
LIB_MAC_SHARED = $(LIB_NAME).dylib

# Directories
SRC_DIR = source
INC_DIR = include
BUILD_DIR = build
BIN_DIR = bin
VERSION_DIR = version

LINUX_DIRNAME = Linux-x86_64
WIN_DIRNAME = Windows-x86_64
MAC_DIRNAME = MacOS-x86_64
ARM_DIRNAME = ARMCortexM

LINUX_DIR = $(BIN_DIR)/$(LINUX_DIRNAME)
WIN_DIR = $(BIN_DIR)/$(WIN_DIRNAME)
MAC_DIR = $(BIN_DIR)/$(MAC_DIRNAME)
ARM_DIR = $(BIN_DIR)/$(ARM_DIRNAME)

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -fPIC -I$(INC_DIR)
LDFLAGS_SHARED = -shared
AR = ar
ARFLAGS = rcs

# Cross-compilers
CC_WIN = x86_64-w64-mingw32-gcc
CC_MAC = o64-clang
CC_ARM = arm-none-eabi-gcc
AR_ARM = arm-none-eabi-ar

# ARM settings
ARM_ARCH = cortex-m3

# Object files
OBJS_LINUX = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/$(LINUX_DIRNAME)/%.o, $(SRCS))
OBJS_WIN = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/$(WIN_DIRNAME)/%.o, $(SRCS))
OBJS_MAC = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/$(MAC_DIRNAME)/%.o, $(SRCS))
OBJS_ARM = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/$(ARM_DIRNAME)/%.o, $(SRCS))

# Build all platforms
default: all

# all: linux windows mac arm
all: linux windows arm

# Installation directory for header files
INSTALL_DIR ?= /usr/local/include/$(LIB_NAME)

# Rule to install header files
install: | $(INSTALL_DIR)
	cp -r $(INC_DIR)/*.h $(INSTALL_DIR)/

# Ensure the installation directory exists
$(INSTALL_DIR):
	mkdir -p $(INSTALL_DIR)

# Rule to uninstall (optional)
uninstall:
	rm -rf $(INSTALL_DIR)

# Build Linux version
linux: $(LINUX_DIR)/$(LIB_SHARED) $(LINUX_DIR)/$(LIB_STATIC)

$(LINUX_DIR)/$(LIB_SHARED): $(OBJS_LINUX) | $(LINUX_DIR)
	$(CC) $(LDFLAGS_SHARED) $^ -o $@

$(LINUX_DIR)/$(LIB_STATIC): $(OBJS_LINUX) | $(LINUX_DIR)
	$(AR) $(ARFLAGS) $@ $^

$(BUILD_DIR)/$(LINUX_DIRNAME)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)/$(LINUX_DIRNAME)
	$(CC) $(CFLAGS) -c $< -o $@

# Build Windows version
windows: $(WIN_DIR)/$(LIB_WIN_SHARED) $(WIN_DIR)/$(LIB_WIN_STATIC)

$(WIN_DIR)/$(LIB_WIN_SHARED): $(OBJS_WIN) | $(WIN_DIR)
	$(CC_WIN) -shared $^ -o $@

$(WIN_DIR)/$(LIB_WIN_STATIC): $(OBJS_WIN) | $(WIN_DIR)
	$(AR) $(ARFLAGS) $@ $^

$(BUILD_DIR)/$(WIN_DIRNAME)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)/$(WIN_DIRNAME)
	$(CC_WIN) $(CFLAGS) -c $< -o $@

# Build macOS version
mac: $(MAC_DIR)/$(LIB_MAC_SHARED)

$(MAC_DIR)/$(LIB_MAC_SHARED): $(OBJS_MAC) | $(MAC_DIR)
	$(CC_MAC) -dynamiclib -o $@ $^ -install_name @rpath/$(LIB_MAC_SHARED)

$(MAC_DIR)/$(LIB_STATIC): $(OBJS_MAC) | $(MAC_DIR)
	$(AR) $(ARFLAGS) $@ $^

$(BUILD_DIR)/$(MAC_DIRNAME)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)/$(MAC_DIRNAME)
	$(CC_MAC) $(CFLAGS) -c $< -o $@

# Build ARM version
arm: $(ARM_DIR)/$(LIB_STATIC)

$(ARM_DIR)/$(LIB_STATIC): $(OBJS_ARM) | $(ARM_DIR)
	$(AR_ARM) $(ARFLAGS) $@ $^

$(BUILD_DIR)/$(ARM_DIRNAME)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)/$(ARM_DIRNAME)
	$(CC_ARM) -mcpu=$(ARM_ARCH) $(CFLAGS) -c $< -o $@

# Ensure directories exist
$(LINUX_DIR) $(WIN_DIR) $(MAC_DIR) $(ARM_DIR) $(BUILD_DIR)/$(LINUX_DIRNAME) $(BUILD_DIR)/$(WIN_DIRNAME) $(BUILD_DIR)/$(MAC_DIRNAME) $(BUILD_DIR)/$(ARM_DIRNAME):
	mkdir -p $@


BUILD_OUTPUT = $(VERSION_DIR)/$(LIB_NAME)-$(VERSION)
ARCHIVE = $(VERSION_DIR)/$(LIB_NAME)-$(VERSION).tar.gz

# Generate version product
generate: | $(BUILD_OUTPUT)/include $(BUILD_OUTPUT)/lib
	cp -r $(INC_DIR)/*.h $(BUILD_OUTPUT)/include/
	cp -r $(BIN_DIR)/* $(BUILD_OUTPUT)/lib/
	echo $(VERSION) > $(BUILD_OUTPUT)/version
	sync && tar -czf $(ARCHIVE) $(BUILD_OUTPUT)
	sha256sum $(ARCHIVE) > $(ARCHIVE).sha256
	sha1sum $(ARCHIVE) > $(ARCHIVE).sha1
	if command -v gpg >/dev/null 2>&1; then gpg --detach-sign --armor -o $(ARCHIVE).asc $(ARCHIVE); fi

# Ensure necessary directories exist
$(BUILD_OUTPUT)/include $(BUILD_OUTPUT)/lib:
	mkdir -p $@



# Clean generated files
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR) $(VERSION_DIR)



