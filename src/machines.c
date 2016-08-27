#include "machines.h"
#include "word_defs.h"
#include <string.h>

#include <stdio.h>

static union Optional_Token make_optional(
                        char lexeme[],
                        int type,
                        int attr,
                        char *forward) {
        struct Token token;
        strcpy(token.lexeme, lexeme);
        token.token_type = type;
        token.attribute.attribute = attr;
        token.forward = forward;
        union Optional_Token op_token;
        op_token.token = token;
        return op_token;
}

static union Optional_Token null_optional() {
        union Optional_Token op_token;
        op_token.nil = NULL;
        return op_token;
}

union Optional_Token relop_machine(char *forward, char *back)
{
        char value = *forward++;
        switch (value) {
        case '<':
                value = *forward++;
                switch (value) {
                case '>':
                        return make_optional("<>", RELOP, NEQ, forward);
                case '=':
                        return make_optional("<=", RELOP, LT_EQ, forward);
                default:
                        forward--;
                        return make_optional("<", RELOP, LT, forward);
                }
        case '>':
                value = *forward++;
                if (value == '=') {
                        return make_optional(">=", RELOP, GT_EQ, forward);
                } else {
                        forward--;
                        return make_optional(">", RELOP, GT, forward);
                }
        case '=':
                return make_optional("=", RELOP, EQ, forward);
        default:
                return null_optional();
        }
}

union Optional_Token longreal_machine(char *forward, char *back)
{
        return null_optional();
}

union Optional_Token real_machine(char *forward, char *back)
{
        return null_optional();
}

union Optional_Token int_machine(char *forward, char *back)
{
        return null_optional();
}

union Optional_Token id_res_machine(char *forward, char *back)
{
        return null_optional();
}

char * ws_machine(char *forward, char *back)
{
        char value;
        do {
                value = *forward++;
        } while (value == ' ' || value == '\t');
        forward--;

        return forward;
}

struct Token catchall_machine(char *forward, char *back)
{
        struct Token token;
        return token;
}
