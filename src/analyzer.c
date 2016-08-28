#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "machines.h"
#include "analyzer.h"
#include "symbols.h"

int main(int argc, char *argv[])
{
        for(int i = 1; i < argc; i++) {
                create_listing(argv[i]);
                // TODO Check if file exists.
        }
}

static void create_listing(char src[])
{

        global_sym_table = malloc(sizeof(struct Symbol));

        FILE *sfp;
        FILE *lfp;
        FILE *tfp;

        char noext[40];
        strcpy(noext, src);
        strcpy(noext, src);
        noext[strlen(noext) - 3] = '\0';

        char lfname[50];
        strcpy(lfname, noext);
        strcat(lfname, "listing");

        char tkname[50];
        strcpy(tkname, noext);
        strcat(tkname, "tokens");

        sfp = fopen(src, "r");
        lfp = fopen(lfname, "w");
        tfp = fopen(tkname, "w");

        char buff[72];
        int line = 0;
        fgets(buff, 72, (FILE*) sfp);
        while(!feof(sfp)) {
                fprintf(lfp, "%-10d", ++line);
                fputs(buff, lfp);
                generate_tokens(line, buff, tfp, lfp);
                fgets(buff, 72, (FILE*) sfp);
        }

        fclose(sfp);
        fclose(lfp);
        fclose(tfp);
}

/*
 * Adds all tokens for the line into the token file.
 * Reports lexical errors to the listing file.
 */
static void generate_tokens(int line, char buff[], FILE *tfp, FILE *lfp)
{
        char *forward = buff;
        char *back = buff;

        while (*forward != '\n') {
                forward = ws_machine(forward, back);
                back = forward;

                struct Token token = match_token(forward, back);
                fprintf(tfp, "%d,%s,%d,%d\n",
                                line,
                                token.lexeme,
                                token.token_type,
                                token.attribute.attribute);

                forward = token.forward;
                back = forward;
        }
}

static struct Token match_token(char *forward, char *back)
{
        union Optional_Token result;

        result = longreal_machine(forward, back);
        if (result.nil != NULL) {
                return result.token;
        }

        result = real_machine(forward, back);
        if (result.nil != NULL) {
                return result.token;
        }

        result = int_machine(forward, back);
        if (result.nil != NULL) {
                return result.token;
        }

        result = id_res_machine(forward);
        if (result.nil != NULL) {
                return result.token;
        }

        result = relop_machine(forward, back);
        if (result.nil != NULL) {
                return result.token;
        }

        return catchall_machine(forward, back);
}
