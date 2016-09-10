#include "symbols.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct Symbol *global_sym_table;
struct Reserved_Word *reserved_word_table;

/*
 * Adds a symbol to the symbol table if it is not already present. If the symbol
 * is already present, returns a pointer to that Symbol.
 *
 * Arguments: word -> literal symbol to be added to the table.
 *
 * Returns: A pointer to the symbol in the table.
 */
struct Symbol * add_symbol(char word[])
{
        struct Symbol *current = global_sym_table;

        while (current -> ptr != NULL) {
                if (strcmp(current -> word, word) == 0)
                        return current;
                current = current -> ptr;
        }

        current -> ptr = malloc(sizeof(struct Symbol));
        strcpy(current -> word, word);
        current -> ptr -> ptr = NULL;

        return current;
}

/*
 * Adds a reserved word to the reserved word table.
 *
 * Arguments: word -> Literal of the word to be added.
 *            type -> Token type associated with the reserved word.
 *            attr -> Token attribute associated with the reserved word.
 *
 * Returns: A pointer to the reserved word added to the table.
 */
struct Reserved_Word * add_reserved_word(char word[], int type, int attr)
{
        struct Reserved_Word *current = reserved_word_table;

        while (current -> next != NULL) {
                current = current -> next;
        }

        current -> next = malloc(sizeof(struct Reserved_Word));
        strcpy(current -> word, word);
        current -> token_type = type;
        current -> attribute = attr;
        current -> next -> next = NULL;

        return current -> next;
}

/*
 * Checks if a given word is in the reserved word table.
 *
 * Arguments: word -> Literal of the word to be checked.
 *
 * Returns: The token associated with the reserved word. If no reserved word is
 *          found, returns a null Optional_Token.
 */
union Optional_Token check_reserved_words(char word[])
{
        struct Reserved_Word *current = reserved_word_table;

        do {
                if (strcmp(current -> word, word) == 0) {
                        return make_optional(word,
                                        current -> token_type,
                                        current -> attribute,
                                        NULL);
                }
                current = current -> next;
        } while (current -> next != NULL);

        return null_optional();
}

/*
 * Initializes the reserved word table from the RESERVED_WORDS file.
 *
 * Arguments: rfp -> Pointer to the reserved word file.
 *
 * Returns: A pointer to the reserved word table.
 */
struct Reserved_Word * initialize_reserved_words(FILE *rfp)
{
        reserved_word_table = malloc(sizeof(struct Reserved_Word));
        reserved_word_table -> next = NULL;
        char buff[80];
        fgets(buff, 80, rfp);

        while(!feof(rfp)) {
                if (buff[0] != '\n') {
                        char word[11];
                        int type;
                        int attr;

                        sscanf(buff, "%s %d %d", word, &type, &attr);
                        add_reserved_word(word, type, attr);
                }
                fgets(buff, 80, rfp);
        }
        return reserved_word_table;
}
