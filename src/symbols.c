#include "symbols.h"
#include <string.h>
#include <stdlib.h>

#include <stdio.h>

struct Symbol *global_sym_table;

struct Symbol * add_symbol(char word[])
{
        struct Symbol *current = global_sym_table;

        while (current -> ptr != NULL) {
                if (strcmp(current -> word, word) == 0)
                        return current;
                current = current -> ptr;
        }

        current -> ptr = malloc(sizeof(struct Symbol));
        strcpy(current -> ptr -> word, word);
        current -> ptr -> ptr = NULL;

        return current -> ptr;
}

union Optional_Token check_reserved_words(char word[])
{
        // TODO: Implement reserved words
        return null_optional();
}
