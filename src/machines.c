#include "machines.h"
#include "word_defs.h"
#include <string.h>

static union Optional_Token make_optional(char lexeme[], int type, int attr) {
        struct Token token;
        strcpy(token.lexeme, lexeme);
        token.token_type = type;
        token.attribute.attribute = attr;
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
                        back = forward;
                        return make_optional("<>", RELOP, NEQ);
                case '=':
                        back = forward;
                        return make_optional("<=", RELOP, LT_EQ);
                default:
                        forward--;
                        back = forward;
                        return make_optional("<", RELOP, LT);
                }
        case '>':
                value = *forward++;
                if (value == '=') {
                        back = forward;
                        return make_optional(">=", RELOP, GT_EQ);
                } else {
                        forward--;
                        back = forward;
                        return make_optional(">", RELOP, GT);
                }
        case '=':
                back = forward;
                return make_optional("=", RELOP, EQ);
        default:
                forward--;
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

void ws_machine(char *forward, char *back)
{
        char value;
        do {
                value = *forward++;
        } while (value == ' ' || value == '\t');
        forward--;
}

struct Token catchall_machine(char *forward, char *back)
{
        struct Token token;
        return token;
}
