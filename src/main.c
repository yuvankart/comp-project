#include <stdio.h>
#include <stdlib.h>  // Include this for malloc() and free()
#include "lexical/lexer.h"
#include "syntax/parser.h"  // Include the parser header

int main(int argc, char** argv) {
    // Check for input file argument
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input-file>\n", argv[0]);
        return 1;
    }

    // Open the input file
    FILE* file = fopen(argv[1], "r");
    if (!file) {
        perror("Failed to open file");
        return 1;
    }

    // Read the entire file into a buffer
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char* buffer = malloc(size + 1);  // Allocate memory for the source code
    if (!buffer) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        return 1;
    }

    fread(buffer, 1, size, file);
    buffer[size] = '\0';  // Null-terminate the string
    fclose(file);

    // Initialize the lexer
    Lexer lexer;
    init_lexer(&lexer, buffer);

    // Parse the program using the parser
    printf("Parsing Program...\n");
    ASTNode* program = parse_program(&lexer);  // Call parser to parse tokens

    if (!program) {
        fprintf(stderr, "Error: Failed to parse program\n");
        free(buffer);
        return 1;
    }

    printf("Parsing complete! Abstract Syntax Tree successfully built.\n");

    // Free resources
    free_ast(program);  // Free the AST
    free(buffer);       // Free allocated memory

    return 0;
}
