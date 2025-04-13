#ifndef PARSER_H
#define PARSER_H

#include "../lexical/lexer.h"  // Include lexer.h to define Lexer

typedef enum {
    NODE_PROGRAM,
    NODE_FUNCTION,
    NODE_STATEMENT,
    NODE_DECLARATION,
    NODE_ASSIGNMENT,
    NODE_LOOP,
    NODE_RETURN,
    NODE_EXPR,
    NODE_IF_ELSE  // Add this for if-else statements
} NodeType;


typedef struct ASTNode {
    NodeType type;
    union {
        struct {  // For if-else statements
            struct ASTNode* expr;       // Condition
            struct ASTNode* body;       // If body
            struct ASTNode* else_body;  // Else body (optional)
        } if_else;

        struct {  // For functions
            char name[256];
            struct ASTNode* body;
        } function;

        struct {  // For statements
            Token token;
            struct ASTNode* expr;
        } statement;

        struct {  // For expressions
            Token token;
            struct ASTNode* left;
            struct ASTNode* right;
        } expr;
    } data;
    struct ASTNode* next;  // Pointer to next node (for lists)
} ASTNode;

// Function prototypes for parsing
ASTNode* parse_program(Lexer* lexer);
ASTNode* parse_statement(Lexer* lexer);
ASTNode* parse_expr(Lexer* lexer);



#endif // PARSER_H
