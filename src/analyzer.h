#ifndef ANALYZER_H
#define ANALYZER_H

#include "machines.h"

const char * const error_codes[] = {
                "Unrecognized Symbol:",
                "Extra Long ID:",
                "Extra Long Integer:",
                "Extra Long Real:",
                "Leading Zeroes:" };

static void create_listing(char src[]);
static void generate_tokens(int line, char buff[], FILE *tfp, FILE *lfp);
static struct Token match_token(char *forward, char *back);

#endif
