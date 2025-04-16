#ifndef TOKENS_H  
#define TOKENS_H  

typedef enum {  
    // Keywords  
    TOK_INT, TOK_FLOAT, TOK_CHAR, TOK_VOID,  
    TOK_IF, TOK_ELSE, TOK_WHILE, TOK_FOR, TOK_DO, TOK_RETURN, 
    TOK_PRINTF, TOK_SCANF,  

    // Operators  
    TOK_PLUS, TOK_MINUS, TOK_MUL, TOK_DIV, TOK_MOD,  
    TOK_EQ, TOK_NEQ, TOK_LT, TOK_GT, TOK_LEQ, TOK_GEQ,  
    TOK_AND, TOK_OR, TOK_NOT,  
    TOK_ASSIGN, TOK_PLUS_EQ, TOK_MINUS_EQ, TOK_INC, // added TOK_INC  

    // Literals  
    TOK_INT_LIT, TOK_FLOAT_LIT, TOK_CHAR_LIT, TOK_STR_LIT,  

    // Identifiers  
    TOK_IDENT,  

    // Delimiters  
    TOK_LBRACE, TOK_RBRACE, TOK_LPAREN, TOK_RPAREN,  
    TOK_SEMICOLON, TOK_COMMA,  

    // Special  
    TOK_EOF, TOK_ERROR  
} TokenType;  

typedef struct {  
    TokenType type;  
    char lexeme[256];  // Token text  
    int line;          // Source line number  
} Token;  

#endif  
