# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

# Directories
SRC_DIR = src
BUILD_DIR = build
LEXICAL_DIR = $(SRC_DIR)/lexical
SYNTAX_DIR = $(SRC_DIR)/syntax
TESTS_DIR = tests
EXAMPLES_DIR = examples

# Executable name
BIN = mycc

# Source files for the lexical and syntax phases
LEXICAL_SRCS = $(wildcard $(LEXICAL_DIR)/*.c)
SYNTAX_SRCS = $(wildcard $(SYNTAX_DIR)/*.c)
MAIN_SRC = $(SRC_DIR)/main.c

# Object files for the lexical and syntax phases
LEXICAL_OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(LEXICAL_SRCS))
SYNTAX_OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SYNTAX_SRCS))
MAIN_OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(MAIN_SRC))

# Test files
TEST_LEXER_SRC = $(TESTS_DIR)/test_lexer.c

# Default target: build the compiler
all: dirs $(BIN)

# Create necessary directories
dirs:
	mkdir -p $(BUILD_DIR)/lexical
	mkdir -p $(BUILD_DIR)/syntax

# Build the compiler executable
$(BIN): $(LEXICAL_OBJS) $(SYNTAX_OBJS) $(MAIN_OBJ)
	$(CC) $(CFLAGS) $^ -o $@

# Compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(BIN) lexer_test

# Run the compiler with a sample program
run: $(BIN)
	./$(BIN) $(EXAMPLES_DIR)/sample_program.txt

# Test the lexical analyzer
test_lexer:
	$(CC) $(CFLAGS) $(TEST_LEXER_SRC) $(LEXICAL_SRCS) -o lexer_test
	./lexer_test

.PHONY: all clean run test_lexer dirs
