#include "../src/lexical/lexer.h"
#include <stdio.h>

void test_lexer() {
    const char* code =
        "int main() {\n"
        "   int x = 5;\n"
        "   float y = x + 3.14;\n"
        "   while(x > 0) x--;\n"
        "}";

    Lexer lexer;
    init_lexer(&lexer, code);

    Token token;
    
    printf("Lexical Analysis Output:\n");
    
    do {
        token = get_next_token(&lexer);
        
        printf("Line %2d: %-15s => %s\n",
               token.line,
               token.lexeme,
               token.type == TOK_ERROR ? "ERROR" : "VALID");
        
    } while(token.type != TOK_EOF && token.type != TOK_ERROR);
}

int main() {
    test_lexer();
    return 0;
}
