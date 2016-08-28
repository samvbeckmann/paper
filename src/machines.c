#include "machines.h"
#include "word_defs.h"
#include "symbols.h"
#include <string.h>
#include <ctype.h>

extern inline int min(int a, int b);

union Optional_Token make_optional(
                        char lexeme[],
                        int type,
                        int attr,
                        char *forward) {
        struct Token token;
        strcpy(token.lexeme, lexeme);
        token.token_type = type;
        token.attribute.attribute = attr;
        token.forward = forward;
        return wrap_token(token);
}

union Optional_Token null_optional() {
        union Optional_Token op_token;
        op_token.nil = NULL;
        return op_token;
}

union Optional_Token wrap_token(struct Token token)
{
        union Optional_Token op_token;
        op_token.token = token;
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
        char int_lit[30];
        int i = 0;
        char value = *forward++;
        while (isdigit(value)) {
                int_lit[i] = value;
                value = *forward++;
                i++;
        }
        forward--;
        int_lit[i] = '\0';

        if (i == 0)
                return null_optional();
        else if (int_list[0] == '0' && i != 1)
                return make_optional(int_lit, 99, 4, forward);
        else if (i > 10)
                return make_optional(int_lit, 99, 3, forward);
        else
                return make_optional(int_lit, 90, 1, forward);
}

union Optional_Token id_res_machine(char *forward)
{
        char word[30];
        int i = 0;
        char value = *forward++;
        while (isalnum(value)) {
                word[i] = value;
                value = *forward++;
                i++;
        }
        forward--;
        word[i] = '\0';

        if (i == 0)
                return null_optional();
        else if (i > 10)
                return make_optional(word, 99, 2, forward);

        union Optional_Token res = check_reserved_words(word);
        if (res.nil != NULL) {
                res.token.forward = forward;
                return res;
        } else {
                struct Symbol *sym_ptr = add_symbol(word);
                struct Token token;
                strcpy(token.lexeme, word);
                token.token_type = 50;
                token.attribute.ptr = sym_ptr;
                token.forward = forward;
                return wrap_token(token);
        }
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
