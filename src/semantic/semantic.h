#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "parser.h"

typedef struct Symbol {
    char name[256];
    enum { SYM_VAR, SYM_FUNC } type;
    int line;
    struct Symbol* next;
} Symbol;

void semantic_analysis(ASTNode* ast);
void print_symbol_table();

#endif
