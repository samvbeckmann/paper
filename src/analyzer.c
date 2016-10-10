#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "machines.h"
#include "analyzer.h"
#include "symbols.h"
#include "word_defs.h"

// Class variables
int line;
char *forward;
FILE *sfp;
FILE *lfp;
FILE *tfp;
struct Token tok;

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

        line = 0;

        forward = get_next_line();

        parse();

        fclose(sfp);
        fclose(lfp);
        fclose(tfp);
        fclose(rfp);
}

static char* get_next_line()
{
        static char buff[72];
        fgets(buff, 72, (FILE*) sfp);
        if (feof(sfp)) {
                buff[0] = EOF;
                line++;
        } else {
                fprintf(lfp, "%-10d", ++line);
                fputs(buff, lfp);
        }
        return buff;
}

static void parse()
{
        while (tok.token_type != 34)
                tok = get_token();
        // match(34);
}

void match(int token_type)
{
        if (tok.token_type == token_type) {
                if (token_type != 34)
                        tok = get_token();
        } else {
                // throw synerr
                tok = get_token();
        }
}

void update_tok(struct Token token)
{
        tok = token;
}

static struct Token get_token()
{
        struct Token token = match_token();

        forward = token.forward;

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

        return token;
}

/*
 * Runs a buffer through all of the machines to match a token.
 *
 * Arguments: forward -> Pointer to memory location to begin reading from.
 *
 * Returns: Token that was matched from one of the machines. Some token will
 *          always be matched by the catch-all machine, so this is garunteed.
 */
static struct Token match_token()
{
        forward = ws_machine(forward);

        if (*forward == '\n') {
                forward = get_next_line();
                forward = ws_machine(forward);
        }

        if (*forward == EOF) {
                return make_token("EOF", 34, 0, NULL);
                forward = ws_machine(forward);
        }

        union Optional_Token result;

        result = longreal_machine(forward);
        if (result.nil != NULL)
                return result.token;

        result = real_machine(forward);
        if (result.nil != NULL)
                return result.token;

        result = int_machine(forward);
        if (result.nil != NULL)
                return result.token;

        result = id_res_machine(forward);
        if (result.nil != NULL)
                return result.token;

        result = relop_machine(forward);
        if (result.nil != NULL)
                return result.token;

        return catchall_machine(forward);
}
