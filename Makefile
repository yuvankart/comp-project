# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I./src -I./include

# Directories
LEXICAL_DIR = src/lexical
SYNTAX_DIR = src/syntax
SEMANTIC_DIR = src/semantic
SRC_DIR = src
TEST_DIR = tests

# Source files
LEXICAL_SRC = $(LEXICAL_DIR)/lexer.c
SYNTAX_SRC = $(SYNTAX_DIR)/parser.c
SEMANTIC_SRC = $(SEMANTIC_DIR)/semantic.c $(SEMANTIC_DIR)/symbol_table.c
MAIN_SRC = $(SRC_DIR)/main.c

SRC_FILES = $(LEXICAL_SRC) $(SYNTAX_SRC) $(SEMANTIC_SRC) $(MAIN_SRC)

# Output
EXEC = compiler

# Test executables (optional)
TEST_LEXER = test_lexer
TEST_PARSER = test_parser
TEST_SEMANTIC = test_semantic

.PHONY: all clean test

# Build full compiler
all: $(EXEC)

$(EXEC): $(SRC_FILES)
	$(CC) $(CFLAGS) $(SRC_FILES) -o $(EXEC)

# Build and run tests
test: $(TEST_LEXER) $(TEST_PARSER) $(TEST_SEMANTIC)

$(TEST_LEXER): $(TEST_DIR)/test_lexer.c $(LEXICAL_SRC)
	$(CC) $(CFLAGS) $^ -o $@
	./$@

$(TEST_PARSER): $(TEST_DIR)/test_parser.c $(LEXICAL_SRC) $(SYNTAX_SRC)
	$(CC) $(CFLAGS) $^ -o $@
	./$@

$(TEST_SEMANTIC): $(TEST_DIR)/test_semantic.c $(SEMANTIC_SRC)
	$(CC) $(CFLAGS) $^ -o $@
	./$@

# Clean build artifacts
clean:
	rm -f $(EXEC) $(TEST_LEXER) $(TEST_PARSER) $(TEST_SEMANTIC)
