#ifndef SYMBOLS_H
#define SYMBOLS_H

#include "machines.h"

struct Symbol {
        char word[11];
        struct Symbol *ptr;
};

// extern struct Symbol *global_sym_table;

struct Symbol * add_symbol(char word[], struct Symbol *sym_table);
union Optional_Token check_reserved_words(char word[]);

#endif
