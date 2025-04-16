#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexical/lexer.h"
#include "syntax/parser.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source_file.c>\n", argv[0]);
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);

    char* source = (char*)malloc(length + 1);
    if (!source) {
        perror("Memory allocation failed");
        fclose(file);
        return 1;
    }

    fread(source, 1, length, file);
    source[length] = '\0';
    fclose(file);

    Lexer lexer;
    init_lexer(&lexer, source);

    parse(&lexer);  // Start parsing

    free(source);
    return 0;
}
