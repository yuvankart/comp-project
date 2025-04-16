#include <stdio.h>
#include <stdlib.h>
#include "../src/lexical/lexer.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source_file>\n", argv[0]);
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (!file) {
        perror("Failed to open input file");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long len = ftell(file);
    rewind(file);

    char* source = malloc(len + 1);
    fread(source, 1, len, file);
    source[len] = '\0';
    fclose(file);

    Lexer lexer;
    init_lexer(&lexer, source);

    Token tok;
    do {
        tok = get_next_token(&lexer);
        printf("Token: %-20s Lexeme: %-15s Line: %d\n",
            (tok.type == TOK_EOF) ? "TOK_EOF" :
            (tok.type == TOK_IDENT) ? "TOK_IDENT" :
            (tok.type == TOK_INT_LIT) ? "TOK_INT_LIT" :
            (tok.type == TOK_FLOAT_LIT) ? "TOK_FLOAT_LIT" :
            (tok.type == TOK_IF) ? "TOK_IF" :
            (tok.type == TOK_ELSE) ? "TOK_ELSE" :
            (tok.type == TOK_PLUS) ? "TOK_PLUS" :
            (tok.type == TOK_ASSIGN) ? "TOK_ASSIGN" :
            (tok.type == TOK_SEMICOLON) ? "TOK_SEMICOLON" :
            "OTHER",
            tok.lexeme,
            tok.line);
    } while (tok.type != TOK_EOF);

    free(source);
    return 0;
}
