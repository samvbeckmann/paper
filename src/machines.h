#ifndef MACHINES_H
#define MACHINES_H

struct Token {
        char lexeme[20];
        int token_type;
        union Attribute {
                int attribute;
                struct Symbol *ptr;
        } attribute;
        char *forward;
};

union Optional_Token {
        void *nil;
        struct Token token;
};

struct Symbol {
        char word[11];
        struct Symbol *ptr;
};

union Optional_Token relop_machine(char *forward, char *back);
union Optional_Token longreal_machine(char *forward, char *back);
union Optional_Token real_machine(char *forward, char *back);
union Optional_Token int_machine(char *forward, char *back);
union Optional_Token id_res_machine(char *forward, struct Symbol *sym_table);
char * ws_machine(char *forward, char *back);
struct Token catchall_machine(char *forward, char *back);

#endif
