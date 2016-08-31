#include "machines.h"
#include "word_defs.h"
#include "symbols.h"
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

extern inline int min(int a, int b);

union Optional_Token make_optional(
                        char lexeme[],
                        int type,
                        int attr,
                        char *forward) {
        return wrap_token(make_token(lexeme, type, attr, forward));
}

struct Token make_token(char lexeme[], int type, int attr, char *forward) {
        struct Token token;
        strcpy(token.lexeme, lexeme);
        token.token_type = type;
        token.is_id = 0;
        token.attribute.attribute = attr;
        token.forward = forward;
        return token;
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

static char * read_digits(char *forward) {
        char * buff = malloc(30);
        int i = 0;
        char value = *forward++;
        while (isdigit(value)) {
                buff[i] = value;
                value = *forward++;
                i++;
        }
        buff[i] = '\0';
        return buff;
}

union Optional_Token longreal_machine(char *forward, char *back)
{
        char real_lit[30];
        bool extra_long = false;
        bool lead_zeros = false;

        char * first_part = read_digits(forward);
        int len = strlen(first_part);
        forward += len;
        strcpy(real_lit, first_part);

        if (len == 0)
                return null_optional();
        else if (len > 5)
                extra_long = true;
        else if (first_part[0] == '0' && len != 1)
                lead_zeros = true;

        char value = *forward++;
        if (value != '.')
                return null_optional();
        strncat(real_lit, &value, 1);

        char *second_part = read_digits(forward);
        len = strlen(second_part);
        forward += len;
        strcat(real_lit, second_part);

        if (len == 0)
                return null_optional();
        else if (len > 5)
                extra_long = true;
        else if (second_part[0] == '0' && len != 1)
                lead_zeros = true;

        value = *forward++;
        if (value != 'E')
                return null_optional();
        strncat(real_lit, &value, 1);

        value = *forward++;
        if (value == '-' || value == '+')
                strncat(real_lit, &value, 1);
        else
                forward--;

        char *exponent = read_digits(forward);
        len = strlen(exponent);
        forward += len;
        strcat(real_lit, exponent);

        if (len == 0)
                return null_optional();
        else if (len > 2)
                extra_long = true;
        else if (exponent[0] == '0')
                lead_zeros = true;

        if (extra_long)
                return make_optional(real_lit, LEXERR, EXTRA_LONG_REAL, forward);
        else if (lead_zeros)
                return make_optional(real_lit, LEXERR, LEADING_ZEROES, forward);
        else
                return make_optional(real_lit, NUM, LONG_REAL, forward);

        return null_optional();
}

union Optional_Token real_machine(char *forward, char *back)
{
        char real_lit[30];
        bool extra_long = false;
        bool lead_zeros = false;

        char * first_part = read_digits(forward);
        int len = strlen(first_part);
        forward += len;
        strcpy(real_lit, first_part);

        if (len == 0)
                return null_optional();
        else if (len > 5)
                extra_long = true;
        else if (first_part[0] == '0' && len != 1)
                lead_zeros = true;

        char value = *forward++;
        if (value != '.')
                return null_optional();
        strncat(real_lit, &value, 1);

        char *second_part = read_digits(forward);
        len = strlen(second_part);
        forward += len;
        strcat(real_lit, second_part);

        if (len == 0)
                return null_optional();
        else if (len > 5)
                extra_long = true;
        else if (second_part[0] == '0' && len != 1)
                lead_zeros = true;

        if (extra_long)
                return make_optional(real_lit, LEXERR, EXTRA_LONG_REAL, forward);
        else if (lead_zeros)
                return make_optional(real_lit, LEXERR, LEADING_ZEROES, forward);
        else
                return make_optional(real_lit, NUM, REAL, forward);
}

union Optional_Token int_machine(char *forward, char *back)
{
        char *digits = read_digits(forward);
        int len = strlen(digits);
        forward += len;

        if (len == 0)
                return null_optional();
        else if (digits[0] == '0' && len != 1)
                return make_optional(digits, LEXERR, LEADING_ZEROES, forward);
        else if (len > 10)
                return make_optional(digits, LEXERR, EXTRA_LONG_INT, forward);
        else
                return make_optional(digits, NUM, INTEGER, forward);
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
                return make_optional(word, LEXERR, EXTRA_LONG_ID, forward);

        union Optional_Token res = check_reserved_words(word);
        if (res.nil != NULL) {
                res.token.forward = forward;
                return res;
        } else {
                struct Symbol *sym_ptr = add_symbol(word);
                struct Token token;
                strcpy(token.lexeme, word);
                token.token_type = ID;
                token.is_id = 1;
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
        char value = *forward++;

        switch (value) {
        case '+':
                return make_token("+", ADDOP, ADD, forward);
        case '-':
                return make_token("-", ADDOP, SUB, forward);
        case '*':
                return make_token("*", MULOP, MULT, forward);
        case '/':
                return make_token("/", MULOP, DIVIDE, forward);
        case ';':
                return make_token(";", SEMI, 0, forward);
        case ',':
                return make_token(",", COMMA, 0, forward);
        case '(':
                return make_token("(", PAREN_OPEN, 0, forward);
        case ')':
                return make_token(")", PAREN_CLOSE, 0, forward);
        case '[':
                return make_token("[", BR_OPEN, 0, forward);
        case ']':
                return make_token("]", BR_CLOSE, 0, forward);
        case ':':
                value = *forward++;
                if (value == '=') {
                        return make_token(":=", ASSIGN, 0, forward);
                } else {
                        forward--;
                        return make_token(":", COLON, 0, forward);
                }
        case '.':
                value = *forward++;
                if (value == '.') {
                        return make_token("..", TWO_DOT, 0, forward);
                } else {
                        forward--;
                        return make_token(".", DOT, 0, forward);
                }
        default:
                return make_token(&value, LEXERR, UNRECOG_SYM, forward);
        }
}
