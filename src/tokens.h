#ifndef TOKENS_H
#define TOKENS_H

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

#endif
