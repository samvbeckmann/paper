#ifndef ANALYZER_H
#define ANALYZER_H

#include "machines.h"
#include <stdio.h>

extern struct Token tok;
extern FILE *lfp;

void match(int token_type);

void update_tok(struct Token token);

struct Token get_token();

void synerr(char* expc, char* rec);

struct Token get_token();


#endif
