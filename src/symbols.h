#ifndef SYMBOLS_H
#define SYMBOLS_H

#include "machines.h"
#include <stdio.h>

struct Symbol {
        char word[11];
        struct Symbol *ptr;
};

struct Reserved_Word {
        char word[11];
        int token_type;
        int attribute;
        struct Reserved_Word *next;
};

extern struct Symbol *global_sym_table;
extern struct Reserved_Word *reserved_word_table;

struct Reserved_Word * initialize_reserved_words(FILE *rfp);
struct Symbol * add_symbol(char word[]);
union Optional_Token check_reserved_words(char word[]);

#endif
