#ifndef SYMBOLS_H
#define SYMBOLS_H

#include "machines.h"
#include <stdio.h>

/*
 * A Symbol for an ID in the symbol table.
 *
 * Fields: word -> Literal of the lexeme symbol.
 *         ptr -> Pointer to the next symbol in the table.
 */
struct Symbol {
        char word[11];
        struct Symbol *ptr;
};

/*
 * Contains a reserved word from the reserved word table.
 *
 * Fields: word -> Literal of the reserved word.
 *         token_type -> Integer of token type assoicated with the word.
 *         attribute -> Integer of attribute associated with the word.
 *         next -> Pointer to the next reserved word in the table.
 */
struct Reserved_Word {
        char word[11];
        int token_type;
        int attribute;
        struct Reserved_Word *next;
};

/*
 * Global symbol table. Pointer to first item in the linked list.
 */
extern struct Symbol *global_sym_table;

/*
 * Rerved word table. Pointer fo first item in the linked list.
 */
extern struct Reserved_Word *reserved_word_table;

/*
 * Adds a symbol to the symbol table if it is not already present. If the symbol
 * is already present, returns a pointer to that Symbol.
 *
 * Arguments: word -> literal symbol to be added to the table.
 *
 * Returns: A pointer to the symbol in the table.
 */
struct Symbol * add_symbol(char word[]);

/*
 * Checks if a given word is in the reserved word table.
 *
 * Arguments: word -> Literal of the word to be checked.
 *
 * Returns: The token associated with the reserved word. If no reserved word is
 *          found, returns a null Optional_Token.
 */
union Optional_Token check_reserved_words(char word[]);

/*
 * Initializes the reserved word table from the RESERVED_WORDS file.
 *
 * Arguments: rfp -> Pointer to the reserved word file.
 *
 * Returns: A pointer to the reserved word table.
 */
struct Reserved_Word * initialize_reserved_words(FILE *rfp);

#endif
