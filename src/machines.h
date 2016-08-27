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

#endif
