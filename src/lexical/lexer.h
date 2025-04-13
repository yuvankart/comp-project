#ifndef LEXER_H
#define LEXER_H

#include "tokens.h"

// Lexer structure to store the input source and current state
typedef struct {
    const char* source;  // Input source code as a string
    int current_pos;     // Current character position in the input string
    int line;            // Current line number (for error reporting)
} Lexer;

// Initialize the lexer with source code input
void init_lexer(Lexer* lexer, const char* source);

// Get the next token from the input
Token get_next_token(Lexer* lexer);

#endif // LEXER_H
