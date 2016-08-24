#include <stdio.h>
#include <string.h>

static void create_listing(char src[]);

int main(int argc, char *argv[])
{
        for(int i = 1; i < argc; i++) {
                create_listing(argv[i]);
        }
}

static void create_listing(char src[])
{
        FILE *sfp;
        FILE *lfp;

        char lfname[50] = {0};
        strcat(lfname, src);
        strcat(lfname, ".listing");

        char buff[72];
        sfp = fopen(src, "r");
        lfp = fopen(lfname, "w");

        int line = 1;
        fgets(buff, 72, (FILE*) sfp);
        while(!feof(sfp)) {
                fprintf(lfp, "%-5d", line++);
                fputs(buff, lfp);
                fgets(buff, 72, (FILE*) sfp);
        }

        fclose(sfp);
        fclose(lfp);
}
