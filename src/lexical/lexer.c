#include "lexer.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>

// Initialize the lexer with source code
void init_lexer(Lexer* lexer, const char* source) {
    lexer->source = source;
    lexer->current_pos = 0;
    lexer->line = 1;
}

static Token handle_number(Lexer* lexer, char first_char) {
    Token token = {TOK_INT_LIT, "", lexer->line};
    int i = 0, is_float = 0;
    token.lexeme[i++] = first_char;

    while (isdigit(lexer->source[lexer->current_pos]) ||
           (lexer->source[lexer->current_pos] == '.' && !is_float)) {
        if (lexer->source[lexer->current_pos] == '.') {
            is_float = 1;
            token.type = TOK_FLOAT_LIT;
        }
        token.lexeme[i++] = lexer->source[lexer->current_pos++];
        if (i >= (int)sizeof(token.lexeme) - 1) break;
    }

    token.lexeme[i] = '\0';
    return token;
}

static Token handle_char_literal(Lexer* lexer) {
    Token token = {TOK_CHAR_LIT, "", lexer->line};
    int i = 0;
    token.lexeme[i++] = '\'';

    char c = lexer->source[lexer->current_pos++];
    token.lexeme[i++] = c;

    if (lexer->source[lexer->current_pos] == '\'') {
        token.lexeme[i++] = '\'';
        lexer->current_pos++;
    } else {
        token.type = TOK_ERROR;
    }

    token.lexeme[i] = '\0';
    return token;
}

static Token handle_string_literal(Lexer* lexer) {
    Token token = {TOK_STR_LIT, "", lexer->line};
    int i = 0;
    token.lexeme[i++] = '"';

    while (lexer->source[lexer->current_pos] != '"' &&
           lexer->source[lexer->current_pos] != '\0') {
        token.lexeme[i++] = lexer->source[lexer->current_pos++];
        if (i >= (int)sizeof(token.lexeme) - 2) break;
    }

    if (lexer->source[lexer->current_pos] == '"') {
        token.lexeme[i++] = '"';
        lexer->current_pos++;
    } else {
        token.type = TOK_ERROR;
    }

    token.lexeme[i] = '\0';
    return token;
}

Token get_next_token(Lexer* lexer) {
    Token token = {TOK_ERROR, "", lexer->line};

    while (isspace(lexer->source[lexer->current_pos])) {
        if (lexer->source[lexer->current_pos] == '\n')
            lexer->line++;
        lexer->current_pos++;
    }

    char c = lexer->source[lexer->current_pos];
    if (c == '\0') {
        token.type = TOK_EOF;
        return token;
    }

    lexer->current_pos++;

    // Identifiers and keywords
    if (isalpha(c) || c == '_') {
        int i = 0;
        token.lexeme[i++] = c;

        while (isalnum(lexer->source[lexer->current_pos]) || lexer->source[lexer->current_pos] == '_') {
            token.lexeme[i++] = lexer->source[lexer->current_pos++];
            if (i >= (int)sizeof(token.lexeme) - 1) break;
        }

        token.lexeme[i] = '\0';

        if (strcmp(token.lexeme, "int") == 0) token.type = TOK_INT;
        else if (strcmp(token.lexeme, "float") == 0) token.type = TOK_FLOAT;
        else if (strcmp(token.lexeme, "char") == 0) token.type = TOK_CHAR;
        else if (strcmp(token.lexeme, "void") == 0) token.type = TOK_VOID;
        else if (strcmp(token.lexeme, "if") == 0) token.type = TOK_IF;
        else if (strcmp(token.lexeme, "else") == 0) token.type = TOK_ELSE;
        else if (strcmp(token.lexeme, "while") == 0) token.type = TOK_WHILE;
        else if (strcmp(token.lexeme, "for") == 0) token.type = TOK_FOR;
        else if (strcmp(token.lexeme, "do") == 0) token.type = TOK_DO;
        else if (strcmp(token.lexeme, "return") == 0) token.type = TOK_RETURN;
        else if (strcmp(token.lexeme, "printf") == 0) token.type = TOK_PRINTF;
        else if (strcmp(token.lexeme, "scanf") == 0) token.type = TOK_SCANF;
        else token.type = TOK_IDENT;

        return token;
    }

    if (isdigit(c)) return handle_number(lexer, c);
    if (c == '\'') return handle_char_literal(lexer);
    if (c == '"') return handle_string_literal(lexer);

    switch (c) {
        case '+':
            if (lexer->source[lexer->current_pos] == '+') {
                lexer->current_pos++;
                return (Token){TOK_INC, "++", lexer->line};
            }
            if (lexer->source[lexer->current_pos] == '=') {
                lexer->current_pos++;
                return (Token){TOK_PLUS_EQ, "+=", lexer->line};
            }
            return (Token){TOK_PLUS, "+", lexer->line};

        case '-':
            if (lexer->source[lexer->current_pos] == '=') {
                lexer->current_pos++;
                return (Token){TOK_MINUS_EQ, "-=", lexer->line};
            }
            return (Token){TOK_MINUS, "-", lexer->line};

        case '*': return (Token){TOK_MUL, "*", lexer->line};
        case '/': return (Token){TOK_DIV, "/", lexer->line};
        case '%': return (Token){TOK_MOD, "%", lexer->line};

        case '=':
            if (lexer->source[lexer->current_pos] == '=') {
                lexer->current_pos++;
                return (Token){TOK_EQ, "==", lexer->line};
            }
            return (Token){TOK_ASSIGN, "=", lexer->line};

        case '!':
            if (lexer->source[lexer->current_pos] == '=') {
                lexer->current_pos++;
                return (Token){TOK_NEQ, "!=", lexer->line};
            }
            return (Token){TOK_NOT, "!", lexer->line};

        case '&':
            if (lexer->source[lexer->current_pos] == '&') {
                lexer->current_pos++;
                return (Token){TOK_AND, "&&", lexer->line};
            }
            break;

        case '|':
            if (lexer->source[lexer->current_pos] == '|') {
                lexer->current_pos++;
                return (Token){TOK_OR, "||", lexer->line};
            }
            break;

        case '<':
            if (lexer->source[lexer->current_pos] == '=') {
                lexer->current_pos++;
                return (Token){TOK_LEQ, "<=", lexer->line};
            }
            return (Token){TOK_LT, "<", lexer->line};

        case '>':
            if (lexer->source[lexer->current_pos] == '=') {
                lexer->current_pos++;
                return (Token){TOK_GEQ, ">=", lexer->line};
            }
            return (Token){TOK_GT, ">", lexer->line};

        case '{': return (Token){TOK_LBRACE, "{", lexer->line};
        case '}': return (Token){TOK_RBRACE, "}", lexer->line};
        case '(': return (Token){TOK_LPAREN, "(", lexer->line};
        case ')': return (Token){TOK_RPAREN, ")", lexer->line};
        case ';': return (Token){TOK_SEMICOLON, ";", lexer->line};
        case ',': return (Token){TOK_COMMA, ",", lexer->line};

        default:
            snprintf(token.lexeme, sizeof(token.lexeme), "Unexpected character: %c", c);
            token.type = TOK_ERROR;
            break;
    }

    return token;
}
