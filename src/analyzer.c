#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "machines.h"
#include "analyzer.h"
#include "symbols.h"

int main(int argc, char *argv[])
{
        for(int i = 1; i < argc; i++) {
                compile_file(argv[i]);
        }
}

/*
 * Compiles the given Pascal file.
 * Creates two files in the directory of the given file:
 *      - .listing file which displays the source with line numbers and errors.
 *      - .tokens file which has a line for each token in the source.
 *
 * Arguments: src -> path to source file.
 */
static void compile_file(char src[])
{
        global_sym_table = malloc(sizeof(struct Symbol));
        global_sym_table -> ptr = NULL;

        FILE *sfp;
        FILE *lfp;
        FILE *tfp;
        FILE *rfp;

        char noext[40];
        strcpy(noext, src);
        *(strrchr(noext, '.') + 1) = '\0';

        char lfname[50];
        strcpy(lfname, noext);
        strcat(lfname, "listing");

        char tkname[50];
        strcpy(tkname, noext);
        strcat(tkname, "tokens");

        sfp = fopen(src, "r");
        lfp = fopen(lfname, "w");
        tfp = fopen(tkname, "w");
        rfp = fopen("RESERVED_WORDS", "r");

        if (sfp == NULL) {
                fprintf(stderr, "Source file \"%s\" does not exist.\n", src);
                return;
        } else if (rfp == NULL) {
                fprintf(stderr, "RESERVED_WORDS file not found.\n");
                return;
        }

        initialize_reserved_words(rfp);

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
        fclose(rfp);
}

/*
 * Adds all tokens for the line into the token file.
 * Reports lexical errors to the listing file.
 *
 * Arguments: line -> line number that is currently being read.
 *            buff -> char array that contins a line of the source file.
 *            tfp -> Pointer to the token file that tokens are written to.
 *            lfp -> Pointer to the listing file, where errors are written.
 */
static void generate_tokens(int line, char buff[], FILE *tfp, FILE *lfp)
{
        char *forward = buff;
        char *back = buff;

        while (*forward != '\n') {
                forward = ws_machine(forward, back);
                back = forward;

                struct Token token = match_token(forward, back);
                if (token.is_id) {
                        fprintf(tfp, "%4d\t%-20s\t%-2d\t%-p\n",
                                        line,
                                        token.lexeme,
                                        token.token_type,
                                        token.attribute.ptr);
                } else {
                        fprintf(tfp, "%4d\t%-20s\t%-2d\t%-d\n",
                                        line,
                                        token.lexeme,
                                        token.token_type,
                                        token.attribute.attribute);
                }

                if (token.token_type == 99) {
                        fprintf(lfp, "LEXERR:   %-20s%s\n",
                                error_codes[token.attribute.attribute- 1],
                                token.lexeme);
                }

                forward = token.forward;
                back = forward;
        }
}

/*
 * Runs a buffer through all of the machines to match a token.
 *
 * Arguments: forward -> Pointer to memory location to begin reading from.
 *
 * Returns: Token that was matched from one of the machines. Some token will
 *          always be matched by the catch-all machine, so this is garunteed.
 */
static struct Token match_token(char *forward, char *back) // TODO: Remove back.
{
        union Optional_Token result;

        result = longreal_machine(forward, back);
        if (result.nil != NULL)
                return result.token;

        result = real_machine(forward, back);
        if (result.nil != NULL)
                return result.token;

        result = int_machine(forward, back);
        if (result.nil != NULL)
                return result.token;

        result = id_res_machine(forward);
        if (result.nil != NULL)
                return result.token;

        result = relop_machine(forward, back);
        if (result.nil != NULL)
                return result.token;

        return catchall_machine(forward, back);
}
