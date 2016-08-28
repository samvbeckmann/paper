#ifndef ANALYZER_H
#define ANALYZER_H

#include "machines.h"

static void create_listing(char src[]);
static void generate_tokens(int line, char buff[], FILE *tfp, FILE *lfp, struct Symbol *sym_table);
static struct Token match_token(char *forward, char *back, struct Symbol *sym_table);

#endif
