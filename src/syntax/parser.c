#include "parser.h"
#include <stdio.h>  // Include this for fprintf and stderr
#include <stdlib.h>
#include <string.h>

// Static variable for storing the current token
static Token current_token;

// Function to advance to the next token
static void advance(Lexer* lexer) {
    current_token = get_next_token(lexer);
}


ASTNode* parse_if_else(Lexer* lexer) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_IF_ELSE;

    // Expect "if"
    if (current_token.type != TOK_IF) {
        fprintf(stderr, "Error: Expected 'if' at line %d\n", current_token.line);
        free(node);
        return NULL;
    }
    advance(lexer);  // Skip "if"

    // Expect "("
    if (current_token.type != TOK_LPAREN) {
        fprintf(stderr, "Error: Expected '(' after 'if' at line %d\n", current_token.line);
        free(node);
        return NULL;
    }
    advance(lexer);  // Skip "("

    // Parse condition
    node->data.if_else.expr = parse_expr(lexer);

    // Expect ")"
    if (current_token.type != TOK_RPAREN) {
        fprintf(stderr, "Error: Expected ')' after condition at line %d\n", current_token.line);
        free(node);
        return NULL;
    }
    advance(lexer);  // Skip ")"

    // Parse "if" body
    node->data.if_else.body = parse_statement(lexer);

    // Check for optional "else"
    if (current_token.type == TOK_ELSE) {
        advance(lexer);  // Skip "else"
        node->data.if_else.else_body = parse_statement(lexer);
    } else {
        node->data.if_else.else_body = NULL;
    }

    return node;
}



// Parsing functions (examples provided earlier)
ASTNode* parse_expr(Lexer* lexer) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = NODE_EXPR;

    if (current_token.type == TOK_IF) {
        return parse_if_else(lexer);
    }

    if (current_token.type == TOK_INT_LIT || current_token.type == TOK_IDENT) {
        node->data.expr.token = current_token;
        advance(lexer);
        return node;
    }

    if (current_token.type == TOK_LPAREN) {
        advance(lexer);  // Skip '('
        node = parse_expr(lexer);
        if (current_token.type != TOK_RPAREN) {
            fprintf(stderr, "Error: Expected ')' at line %d\n", current_token.line);
            free(node);
            return NULL;
        }
        advance(lexer);  // Skip ')'
        return node;
    }

    fprintf(stderr, "Error: Invalid expression at line %d\n", current_token.line);
    free(node);
    return NULL;
}

ASTNode* parse_statement(Lexer* lexer) {
    if (current_token.type == TOK_INT) {
        // Parse a declaration
        ASTNode* node = malloc(sizeof(ASTNode));
        node->type = NODE_DECLARATION;
        advance(lexer);  // Skip 'int'

        if (current_token.type != TOK_IDENT) {
            fprintf(stderr, "Error: Expected identifier at line %d\n", current_token.line);
            free(node);
            return NULL;
        }

        node->data.statement.token = current_token;  // Store identifier
        advance(lexer);  // Skip identifier

        if (current_token.type == TOK_ASSIGN) {
            advance(lexer);  // Skip '='
            node->data.statement.expr = parse_expr(lexer);
        }

        if (current_token.type != TOK_SEMICOLON) {
            fprintf(stderr, "Error: Expected ';' at line %d\n", current_token.line);
            free(node);
            return NULL;
        }
        advance(lexer);  // Skip ';'
        
        return node;
    } else if (current_token.type == TOK_WHILE) {
        // Parse a while loop
        ASTNode* node = malloc(sizeof(ASTNode));
        node->type = NODE_LOOP;
        advance(lexer);  // Skip 'while'

        if (current_token.type != TOK_LPAREN) {
            fprintf(stderr, "Error: Expected '(' at line %d\n", current_token.line);
            free(node);
            return NULL;
        }

        advance(lexer);  // Skip '('
        node->data.statement.expr = parse_expr(lexer);

        if (current_token.type != TOK_RPAREN) {
            fprintf(stderr, "Error: Expected ')' at line %d\n", current_token.line);
            free(node);
            return NULL;
        }

        advance(lexer);  // Skip ')'
        node->data.statement.expr->next = parse_statement(lexer);  // Parse loop body

        return node;
    } else if (current_token.type == TOK_FOR) {
        // Parse a for loop
        ASTNode* node = malloc(sizeof(ASTNode));
        node->type = NODE_LOOP;
        advance(lexer);  // Skip 'for'

        if (current_token.type != TOK_LPAREN) {
            fprintf(stderr, "Error: Expected '(' at line %d\n", current_token.line);
            free(node);
            return NULL;
        }

        advance(lexer);  // Skip '('

        // Parse initialization
        if (current_token.type == TOK_INT) {
            node->data.statement.expr = parse_statement(lexer);
        }

        if (current_token.type != TOK_SEMICOLON) {
            fprintf(stderr, "Error: Expected ';' at line %d\n", current_token.line);
            free(node);
            return NULL;
        }
        
        advance(lexer);  // Skip ';'

        // Parse condition
        node->data.statement.expr->next = parse_expr(lexer);

        if (current_token.type != TOK_SEMICOLON) {
            fprintf(stderr, "Error: Expected ';' at line %d\n", current_token.line);
            free(node);
            return NULL;
        }

        advance(lexer);  // Skip ';'

        // Parse increment
        node->data.statement.expr->next->next = parse_statement(lexer);

        if (current_token.type != TOK_RPAREN) {
            fprintf(stderr, "Error: Expected ')' at line %d\n", current_token.line);
            free(node);
            return NULL;
        }

        advance(lexer);  // Skip ')'
        
        node->data.statement.expr->next->next->next = parse_statement(lexer);  // Parse loop body

        return node;
    } else if (current_token.type == TOK_RETURN) {
        ASTNode* node = malloc(sizeof(ASTNode));
        node->type = NODE_RETURN;

        advance(lexer);  // Skip 'return'
        
        node->data.statement.expr = parse_expr(lexer);

        if (current_token.type != TOK_SEMICOLON) {
            fprintf(stderr, "Error: Expected ';' at line %d\n", current_token.line);
            free(node);
            return NULL;
        }
        
        advance(lexer);  // Skip ';'
        
        return node;
    }

    fprintf(stderr, "Error: Invalid statement at line %d\n", current_token.line);
    return NULL;
}



ASTNode* parse_program(Lexer* lexer) {
    ASTNode* program = malloc(sizeof(ASTNode));
    program->type = NODE_PROGRAM;

    advance(lexer);

    while (current_token.type != TOK_EOF) {
        if (current_token.type == TOK_INT) {  // Parse a function declaration
            ASTNode* func = malloc(sizeof(ASTNode));
            func->type = NODE_FUNCTION;

            advance(lexer);  // Skip 'int'

            if (current_token.type != TOK_IDENT) {
                fprintf(stderr, "Error: Expected function name at line %d\n", current_token.line);
                free(func);
                continue;
            }

            strcpy(func->data.function.name, current_token.lexeme);

            advance(lexer);  // Skip function name

            if (current_token.type != TOK_LPAREN) {
                fprintf(stderr, "Error: Expected '(' at line %d\n", current_token.line);
                free(func);
                continue;
            }

            advance(lexer);  // Skip '('

            if (current_token.type != TOK_RPAREN) {
                fprintf(stderr, "Error: Expected ')' at line %d\n", current_token.line);
                free(func);
                continue;
            }

            advance(lexer);  // Skip ')'

            func->data.function.body = parse_statement(lexer);

            program->next = func;  // Add function to program list
        }
        
        advance(lexer);  // Consume next token
    }

    return program;
}

void free_ast(ASTNode* node) {
    if (!node) return;

    free_ast(node->next);

    switch (node->type) {
        case NODE_PROGRAM:
        case NODE_FUNCTION:
            free_ast(node->data.function.body);
            break;

        case NODE_EXPR:
            free_ast(node->data.expr.left);
            free_ast(node->data.expr.right);
            break;

        default:
            break;
    }

    free(node);
}
