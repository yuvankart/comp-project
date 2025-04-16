#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include "../lexical/tokens.h"
#include "../lexical/lexer.h"

static Lexer* lexer;
static Token currentToken;
static Token nextToken;

// Initialize parser with first two tokens
static void init_parser() {
    currentToken = get_next_token(lexer);
    nextToken = get_next_token(lexer);
}

static Token peek() {
    return currentToken;
}

static Token peek_next() {
    return nextToken;
}

static Token advance() {
    Token prev = currentToken;
    currentToken = nextToken;
    nextToken = get_next_token(lexer);
    return prev;
}

static int match(TokenType type) {
    if (peek().type == type) {
        advance();
        return 1;
    }
    return 0;
}

static void error(const char* message) {
    fprintf(stderr, "Syntax error at line %d: %s\n", peek().line, message);
    exit(1);
}

static void expect(TokenType type, const char* message) {
    if (!match(type)) error(message);
}

static void statement();
static void expression();
static void block();
static void declaration();
static void for_loop();
static void while_loop();
static void if_statement();
static void light_declaration();  // NEW for `for` loops

static void parse_program() {
    while (peek().type != TOK_EOF) {
        statement();
    }
}

static void statement() {
    switch (peek().type) {
        case TOK_INT:
        case TOK_FLOAT:
        case TOK_CHAR:
        case TOK_VOID:
            declaration();
            break;
        case TOK_IF:
            if_statement();
            break;
        case TOK_WHILE:
            while_loop();
            break;
        case TOK_FOR:
            for_loop();
            break;
        case TOK_RETURN:
            advance();
            expression();
            expect(TOK_SEMICOLON, "Expected ';' after return");
            break;
        case TOK_LBRACE:
            block();
            break;
        case TOK_IDENT:
            advance();
            if (match(TOK_ASSIGN)) {
                expression();
                expect(TOK_SEMICOLON, "Expected ';' after assignment");
            } else {
                error("Expected assignment operator after identifier");
            }
            break;
        default:
            error("Unknown or unsupported statement");
    }
}

static void declaration() {
    advance(); // consume type

    expect(TOK_IDENT, "Expected identifier after type");

    if (match(TOK_LPAREN)) {
        // Function definition
        expect(TOK_RPAREN, "Expected ')' after function parameters");
        block();  // parse the body
        return;
    }

    if (match(TOK_ASSIGN)) {
        expression();
    }

    expect(TOK_SEMICOLON, "Expected ';' after declaration");
}

// Lighter version for use in `for` loop initializer
static void light_declaration() {
    advance(); // type
    expect(TOK_IDENT, "Expected identifier after type");
    if (match(TOK_ASSIGN)) {
        expression();
    }
    // Don't expect semicolon here — the for-loop does that
}

static void if_statement() {
    advance(); // 'if'
    expect(TOK_LPAREN, "Expected '(' after if");
    expression();
    expect(TOK_RPAREN, "Expected ')' after if condition");
    statement();
    if (match(TOK_ELSE)) {
        statement();
    }
}

static void while_loop() {
    advance(); // 'while'
    expect(TOK_LPAREN, "Expected '(' after while");
    expression();
    expect(TOK_RPAREN, "Expected ')' after while condition");
    statement();
}

static void for_loop() {
    advance(); // 'for'
    expect(TOK_LPAREN, "Expected '(' after for");

    // init
    if (peek().type == TOK_INT || peek().type == TOK_FLOAT || peek().type == TOK_CHAR || peek().type == TOK_VOID) {
        light_declaration();  // fixed
    } else if (peek().type != TOK_SEMICOLON) {
        expression();
    }
    expect(TOK_SEMICOLON, "Expected ';' after initializer");

    // condition
    if (peek().type != TOK_SEMICOLON) {
        expression();
    }
    expect(TOK_SEMICOLON, "Expected ';' after condition");

    // increment
    if (peek().type != TOK_RPAREN) {
        expression();
    }
    expect(TOK_RPAREN, "Expected ')' after for");

    statement();
}

static void block() {
    expect(TOK_LBRACE, "Expected '{'");
    while (peek().type != TOK_RBRACE && peek().type != TOK_EOF) {
        statement();
    }
    expect(TOK_RBRACE, "Expected '}'");
}

static void factor() {
    if (match(TOK_LPAREN)) {
        expression();
        expect(TOK_RPAREN, "Expected ')'");
    } else if (match(TOK_IDENT)) {
        // Support post-increment like i++
        if (peek().type == TOK_INC) {
            advance();  // consume ++
        }
        return;
    } else if (
        match(TOK_INT_LIT) ||
        match(TOK_FLOAT_LIT) ||
        match(TOK_CHAR_LIT) ||
        match(TOK_STR_LIT)
    ) {
        return;
    } else {
        error("Expected expression factor");
    }
}


static void term() {
    factor();
    while (peek().type == TOK_MUL || peek().type == TOK_DIV || peek().type == TOK_MOD) {
        advance();
        factor();
    }
}

static void expression() {
    term();
    while (peek().type == TOK_PLUS || peek().type == TOK_MINUS) {
        advance();
        term();
    }
}


void parse(Lexer* l) {
    lexer = l;
    init_parser();
    parse_program();
    printf("Parsing completed successfully.\n");
}
