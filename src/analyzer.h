#ifndef ANALYZER_H
#define ANALYZER_H

#include "machines.h"

/*
 * Constant array of error code strings. Used for reporting error in a human
 * readable format in the listing file.
 */
const char * const error_codes[] = {
                "Unrecognized Sym:",
                "Extra Long ID:",
                "Extra Long Integer:",
                "Extra Long Real:",
                "Leading Zeroes:" };

/*
 * Compiles the given Pascal file.
 * Creates two files in the directory of the given file:
 *      - .listing file which displays the source with line numbers and errors.
 *      - .tokens file which has a line for each token in the source.
 *
 * Arguments: src -> path to source file.
 */
static void compile_file(char src[]);

/*
 * Adds all tokens for the line into the token file.
 * Reports lexical errors to the listing file.
 *
 * Arguments: line -> line number that is currently being read.
 *            buff -> char array that contins a line of the source file.
 *            tfp -> Pointer to the token file that tokens are written to.
 *            lfp -> Pointer to the listing file, where errors are written.
 */
static void generate_tokens(int line, char buff[], FILE *tfp, FILE *lfp);

/*
 * Runs a buffer through all of the machines to match a token.
 *
 * Arguments: forward -> Pointer to memory location to begin reading from.
 *
 * Returns: Token that was matched from one of the machines. Some token will
 *          always be matched by the catch-all machine, so this is garunteed.
 */
static struct Token match_token(char *forward);

#endif
