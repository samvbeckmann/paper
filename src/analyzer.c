#include <stdio.h>
#include <string.h>

static void create_listing(char src[]);
static void generate_tokens(int line, char buff[], FILE *tfp, FILE *lfp);

int main(int argc, char *argv[])
{
        for(int i = 1; i < argc; i++) {
                create_listing(argv[i]);
                // TODO Check if file exists.
        }
}

static void create_listing(char src[])
{
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
 * Reports lexical erros to the listing file.
 */
static void generate_tokens(int line, char buff[], FILE *tfp, FILE *lfp)
{
        int forward = 0;
        int back = 0;


}
