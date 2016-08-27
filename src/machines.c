#include "machines.h"
#include "word_defs.h"
#include <string.h>
#include <ctype.h>

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
        return wrap_token(token);
}

static union Optional_Token null_optional() {
        union Optional_Token op_token;
        op_token.nil = NULL;
        return op_token;
}

static union OptionalToken wrap_token(struct Token token)
{
        union Optional_Token op_token;
        op_token.token = token;
        return op_token;
}

static struct Symbol * add_symbol(char word[], struct Symbol *sym_table)
{
        if (*sym_table == NULL) {
                struct Symbol symbol;
                strcpy(symbol.word, word);
                symbol.ptr = NULL;
                return &symbol;
        } else if (strcmp(sym_table -> word, word) == 0) {
                return sym_table -> ptr;
        } else {
                return add_symbol(word, sym_tableptr);
        }
}

static union Optional_Token check_reserved_words(char word[])
{
        // TODO: Implement reserved words
        return null_optional();
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

union Optional_Token id_res_machine(char *forward, struct Symbol *sym_table)
{
        char word[11];
        int i = 0;
        char value = *forward++;
        while (isalnum(value)) {
                if (i < 10)
                        word[i] = value;
                value = *forward++;
                i++;
        }
        forward--;
        word[min(10, i)] = '\0';

        if (i == 0)
                return null_optional();
        else if (i > 10)
                return make_optional(word, 99, 2, forward);

        union Optional_Token res = check_reserved_words(word);
        if (res.nil != NULL) {
                return res;
        } else {
                struct Symbol *sym_ptr = add_symbol(word, sym_table);
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
