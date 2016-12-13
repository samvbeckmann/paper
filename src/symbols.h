#ifndef SYMBOLS_H
#define SYMBOLS_H

#include "machines.h"
#include "types.h"
#include <stdio.h>

/**
 * A Symbol for an ID in the symbol table.
 * REVIEW: Documentaton
 * Fields: word -> Literal of the lexeme symbol.
 *         ptr -> Pointer to the next symbol in the table.
 */
struct Symbol {
        char word[11];
        enum Type type;
        int offset;
        int num_parms;
        struct Symbol *content;
        struct Symbol *previous;
        struct Symbol *next;
};

/**
 * REVIEW: Documentation
 */
struct SymbolStack {
        struct Symbol *symbol;
        struct SymbolStack *previous;
};

// REVIEW: Documentation
extern struct Symbol *forward_eye;

// REVIEW: Documentation
extern struct Symbol *eye;

/*
 * Rerved word table. Pointer fo first item in the linked list.
 */
extern struct Reserved_Word *reserved_word_table;

struct Symbol * check_add_green_node(char lex[], enum Type type);

void check_add_blue_node(char lex[], enum Type type, int offset);

void pop_scope_stack();

void enter_num_params(int counter);

enum Type get_type(char lex[]);

struct Symbol * get_proc_pointer(char lexeme[]);

void print_symbol_table(FILE *out);

#endif
