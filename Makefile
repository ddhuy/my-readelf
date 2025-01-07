# define utilities
RM = @rm -rf
MKDIR = @mkdir -p

# define compiler
CC = gcc
CXX = g++
AS = as
LD = ld
OC = objcopy
OD = objdump
OS = size

# compiling flags & linking flags
INCLUDES = -I./ -I./lib
CFLAGS   = -Wall -g -O2
LDFLAGS  = 

# source directories & files
SRC_DIR = source
SRC_DIRS = $(SRC_DIR) $(SRC_DIR)/lib
SRCS = $(foreach sdir,$(SRC_DIRS),$(wildcard $(sdir)/*.c))

# output directories & files
BIN_DIR = build
BIN_DIRS = $(patsubst $(SRC_DIR)/%,$(BIN_DIR)/%,$(SRC_DIRS))
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%.o,$(SRCS))

# application name
TARGET = $(BIN_DIR)/my-readelf

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ $(LDFLAGS) -o $@
	$(OS) $@
	@echo ""

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	$(MKDIR) -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@echo ""

.PHONY: clean
clean:
	$(RM) $(BIN_DIR)
