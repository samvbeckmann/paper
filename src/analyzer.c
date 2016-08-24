#include <stdio.h>
#include <string.h>

static void create_listing(char src[]);

int main(int argc, char *argv[])
{
        // create_listing(argv[1]);
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

        printf("%s\n", src);
        printf("%s\n", lfname);

        do {
                fgets(buff, 72, (FILE*) sfp);
                fputs(buff, lfp);
        } while (!feof(sfp));

        fclose(sfp);
        fclose(lfp);
}
