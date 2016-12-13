#ifndef RESERVED_WORDS_H
#define RESERVED_WORDS_H

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
