#ifndef MACHINES_H
#define MACHINES_H

union Optional_Token {
        void *nil;
        struct Token {
                char lexeme[20];
                int token_type;
                union Attribute {
                        int attribute;
                        int *ptr;
                } attribute;
        } token;
};

union Optional_Token relop_machine(int *forward, int *back);
union Optional_Token longreal_machine(int *forward, int *back);
union Optional_Token real_machine(int *forward, int *back);
union Optional_Token int_machine(int *forward, int *back);
union Optional_Token id_res(int *forward, int *back);
union Optional_Token ws_machine(int *forward, int *back);
struct Token catchall_machine(int *forward, int *back);

#endif
