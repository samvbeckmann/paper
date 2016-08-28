#include "symbols.h"
#include <string.h>
#include <stdlib.h>

#include <stdio.h>

// struct Symbol *global_sym_table;

struct Symbol * add_symbol(char word[], struct Symbol *sym_table)
{
        if (sym_table -> ptr == NULL) {
                // struct Symbol symbol;
                strcpy(sym_table -> word, word);
                sym_table -> ptr = malloc(sizeof(struct Symbol));
                // printf("Added \"%s\" : %p\n", word, &symbol);
                return sym_table;
        } else if (strcmp(sym_table -> word, word) == 0) {
                // printf("ALREADY IN TABLE\n");
                return sym_table;
        } else {
                // printf("Looking for \"%s\" at %p, next entry: %p\n",
                //                 word,
                //                 sym_table,
                //                 sym_table -> ptr);
                // printf("CHECKING NEXT ENTRY...\n");
                // getchar();
                return add_symbol(word, sym_table -> ptr);
        }
}

union Optional_Token check_reserved_words(char word[])
{
        // TODO: Implement reserved words
        return null_optional();
}
