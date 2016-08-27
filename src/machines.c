#include "machines.h"
#include "word_defs.h"
#include <string.h>

static Optional_Token make_optional(char lexeme[], int token_type, int attr) {
        struct Token token;
        strcpy(token.lexeme, lexeme);
        token.token_type = token_type;
        token.attribute.attribute = attr;
        union Optional_Token op_token;
        op_token.token = token;
        return op_token;
}

static Optional_Token null_optional() {
        Optional_Token op_token;
        op_token.nil = NULL;
        return op_token;
}

union Optional_Token relop_machine(int *forward, int *back)
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
