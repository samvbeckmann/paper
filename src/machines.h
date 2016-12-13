#ifndef MACHINES_H
#define MACHINES_H

#include "tokens.h"

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
