#ifndef MACHINES_H
#define MACHINES_H

struct Token {
        char lexeme[20];
        int is_id;
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

union Optional_Token make_optional(char lexeme[], int type, int attr, char *forward);
struct Token make_token(char lexeme[], int type, int attr, char *forward);
union Optional_Token null_optional();
union Optional_Token wrap_token(struct Token token);

union Optional_Token relop_machine(char *forward, char *back);
union Optional_Token longreal_machine(char *forward, char *back);
union Optional_Token real_machine(char *forward, char *back);
union Optional_Token int_machine(char *forward, char *back);
union Optional_Token id_res_machine(char *forward);
char * ws_machine(char *forward, char *back);
struct Token catchall_machine(char *forward, char *back);

#endif
