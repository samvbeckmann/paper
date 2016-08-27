#ifndef MACHINES_H
#define MACHINES_H

struct Token {
        char lexeme[20];
        int token_type;
        union Attribute {
                int attribute;
                int *ptr;
        } attribute;
        char *forward;
};

union Optional_Token {
        void *nil;
        struct Token token;
};

union Optional_Token relop_machine(char *forward, char *back);
union Optional_Token longreal_machine(char *forward, char *back);
union Optional_Token real_machine(char *forward, char *back);
union Optional_Token int_machine(char *forward, char *back);
union Optional_Token id_res_machine(char *forward, char *back);
char * ws_machine(char *forward, char *back);
struct Token catchall_machine(char *forward, char *back);

#endif
