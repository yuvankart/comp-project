#include "semantic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Symbol* symbol_table = NULL;

void semantic_analysis(ASTNode* ast) {
    // Traverse AST and build symbol table
    switch(ast->type) {
        case NODE_FUNCTION:
            // Add function to symbol table
            Symbol* sym = malloc(sizeof(Symbol));
            strcpy(sym->name, ast->data.function.name);
            sym->type = SYM_FUNC;
            sym->next = symbol_table;
            symbol_table = sym;
            break;
            
        // ... Handle other node types ...
    }
}

void print_symbol_table() {
    Symbol* current = symbol_table;
    while(current != NULL) {
        printf("Symbol: %s (%s)\n", 
              current->name, 
              current->type == SYM_VAR ? "var" : "func");
        current = current->next;
    }
}
