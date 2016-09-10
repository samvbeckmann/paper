#ifndef MACHINES_H
#define MACHINES_H

/*
 * A token is the basic unit the Pascal interpretation.
 *
 * Fields: lexeme -> The literal from source that is this token.
 *         is_id -> 1 if this token represents an id, otherwise 0.
 *         token_type -> integer that represents this token's type.
 *         Attribute.attribute -> Integer that represents the type's attribute.
 *         Attribute.ptr -> Pointer to a symbol in the symbol table.
 *                          Used if this token is an id.
 *         forward -> Pointer to next position in buffer after lexeme.
 *                    Used to update the forward pointer, then discarded.
 */
struct Token {
        char lexeme[20];
        int is_id;
        int token_type;
        union Attribute {
                int attribute;
                struct Symbol *ptr;
        } attribute;
        char *forward;
};

/*
 * An Optional_Token is either a token or null.
 * Used as a return type for machines that may not match a token.
 *
 * Fields: nil -> Void pointer if the Optional_Token is nil.
 *         token -> Token if Optional_Token is not null.
 */
union Optional_Token {
        void *nil;
        struct Token token;
};

/*
 * Factory for Optional_Tokens.
 * Takes in needed parameters for a token, and makes an Optional_Token with
 * those parameters. Abstracts the creation of Optional_Token structs.
 *
 * Arguments: lexeme -> Literal of matched lexeme.
 *            type -> Integer representation of token's type.
 *            attr -> Integer representation of token's attribute.
 *            forward -> Pointer to the char after this lexeme ended in buffer.
 *
 * Returns: An Optional_Token with the given parameters. Not a null optional.
 */
union Optional_Token make_optional(char lexeme[], int type, int attr, char *forward);

/*
 * Factory for Tokens.
 * Takes in needed parameters for a token, and makes an Optional_Token with
 * those paratmers. Abstracts the creation of Token structs.
 *
 * Arguments: lexeme -> Literal of matched lexeme.
 *            type -> Integer representation of token's type.
 *            attr -> Integer representation of token's attribute.
 *            forward -> Pointer to the char after this lexeme ended in buffer.
 *
 * Returns: A Token with the given parameters. This does not create an id token.
 */
struct Token make_token(char lexeme[], int type, int attr, char *forward);

/*
 * Creates an Optional_Token which is nil.
 * Used as standard factory of nil Optional_Token structs.
 *
 * Returns: Optional_Token with "nil" as the token.
 */
union Optional_Token null_optional();

/*
 * Wraps a token as an Optional_Token, so that it can be returned as such.
 *
 * Arguments: token -> Token that is to be wrapped.
 *
 * Returns: Optional_Token that contains the paramter "token"
 */
union Optional_Token wrap_token(struct Token token);

/*
 * Machine that matches whitespace.
 *
 * Arguments: forward -> Pointer to memory location to begin reading from.

 * Returns: Pointer to first non-whitespace character matched.
 */
char * ws_machine(char *forward);

/*
 * Machine that reads real numbers containing an exponent, or "Long Reals".
 *
 * A long real consists of 1-5 digits, a decimal point, 1-5 digits, "E",
 * an optional sign (+|-), and 1-2 digits.
 *
 * Arguments: forward -> Pointer to memory location to begin reading from.
 *
 * Returns: an Optional_Token representing the matched long real, or a nil
 *          Optional_Token if no long real is matched.
 */
union Optional_Token longreal_machine(char *forward);

/*
 * Machine that reads real numbers.
 *
 * A real number consists of 1-5 digits, a decimal point, and 1-5 digits.
 *
 * Arguments: forward -> Pointer to memory location to begin reading from.
 *
 * Returns: An Optional_Token representing the matched real, or a nil
 *          Optional_Token if no real number is matched.
 */
union Optional_Token real_machine(char *forward);

/*
 * Machine that reads integers.
 *
 * An integer consists of 1-10 digits with no leading zeroes.
 *
 * Arguments: forward -> Pointer to memory location to begin reading from.
 *
 * Returns: An Optional_Token representing the matched integer, or a nil
 *          Optional_Token if no integer is matched.
 */
union Optional_Token int_machine(char *forward);

/*
 * Machine that matches ids and reserved words.
 *
 * An ID consists of a letter, followed by 0-9 digits or letters.
 * If the matched string is equivalent to a reserved word, returns the token
 * that represents the reserved word.
 * Otherwise, adds the ID to the symbol table if it is not already there,
 * and returns an Optional_Token containg the matched ID and a reference
 * to it in the symbol table.
 *
 * Arguments: forward -> Pointer to memory location to begin reading from.
 *
 * Returns: A LEXERR Optional_Token if an error is encountered, or a a nil
 *          Optional_Token if no id or reserved word is matched.
 */
union Optional_Token id_res_machine(char *forward);

/*
 * Machine that matches relational operators, or "Relops".
 *
 * Valid relops: '<', '>', '==', '<=', '>=', '<>'.
 *
 * Arguments: forward -> Pointer to memory location to begin reading from.
 *
 * Returns: An Optional_Token representing the matched relop, or a nil
 *          Optional_Token if no relop is matched.
 */
union Optional_Token relop_machine(char *forward);

/*
 * Machine that caches all other tokens not matched by a previous machine.
 *
 * If no valid token is matched by this machine, it returns a LEXERR for an
 * unrecognized symbol. This garuntees this machine will always return a token.
 *
 * Arguments: forward -> Pointer to memory location to begin reading from.
 *
 * Returns: Token either containing a valid token, attribute pair, or a LEXERR
 *          token if no valid token is matched.
 */
struct Token catchall_machine(char *forward);

#endif
