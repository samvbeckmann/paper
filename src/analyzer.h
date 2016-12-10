#ifndef ANALYZER_H
#define ANALYZER_H

#include "machines.h"
#include <stdio.h>

extern struct Token tok;
extern FILE *lfp;

/*
 * Compiles the given Pascal file.
 * Creates two files in the directory of the given file:
 *      - .listing file which displays the source with line numbers and errors.
 *      - .tokens file which has a line for each token in the source.
 *
 * Arguments: src -> path to source file.
 */
static void compile_file(char src[]);

static char* get_next_line();
static char* type_str(int token_type);
static void parse();
void match(int token_type);
void update_tok(struct Token token);
struct Token get_token();
void synerr(char* expc, char* rec);

/*
 * Runs a buffer through all of the machines to match a token.
 *
 * Arguments: forward -> Pointer to memory location to begin reading from.
 *
 * Returns: Token that was matched from one of the machines. Some token will
 *          always be matched by the catch-all machine, so this is garunteed.
 */
static struct Token match_token();

struct Token get_token();


#endif
