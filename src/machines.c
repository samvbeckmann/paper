#include "machines.h"
#include "word_defs.h"
#include "symbols.h"
#include "tokens.h"
#include "reserved_words.h"
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

static char * read_digits(char *forward);

/*
 * Reads a series of digits until a non-digit character is read, returning a
 * buffer of read digits.
 *
 * Arguments: forward -> Pointer to where begin reading.
 *
 * Returns: char pointer to buffer or read digits.
 */
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

/*
 * Machine that matches whitespace.
 *
 * Arguments: forward -> Pointer to memory location to begin reading from.

 * Returns: Pointer to first non-whitespace character matched.
 */
char * ws_machine(char *forward)
{
        char value;
        do {
                value = *forward++;
        } while (value == ' ' || value == '\t');
        forward--;

        return forward;
}

/*
 * Machine that reads real numbers containing an exponent, or "Long Reals".
 *
 * A long real consists of 1-5 digits, a decimal point, 1-5 digits, "E",
 * an optional sign (+|-), and 1-2 digits.
 *
 * Arguments: forward -> Pointer to memory location to begin reading from.
 *
 * Returns: an Optional_Token representing the matched long real, or a nil
 *          Optional_Token if no long real is matched.
 */
union Optional_Token longreal_machine(char *forward)
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

/*
 * Machine that reads real numbers.
 *
 * A real number consists of 1-5 digits, a decimal point, and 1-5 digits.
 *
 * Arguments: forward -> Pointer to memory location to begin reading from.
 *
 * Returns: An Optional_Token representing the matched real, or a nil
 *          Optional_Token if no real number is matched.
 */
union Optional_Token real_machine(char *forward)
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

/*
 * Machine that reads integers.
 *
 * An integer consists of 1-10 digits with no leading zeroes.
 *
 * Arguments: forward -> Pointer to memory location to begin reading from.
 *
 * Returns: An Optional_Token representing the matched integer, or a nil
 *          Optional_Token if no integer is matched.
 */
union Optional_Token int_machine(char *forward)
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

/*
 * Machine that matches ids and reserved words.
 *
 * An ID consists of a letter, followed by 0-9 digits or letters.
 * If the matched string is equivalent to a reserved word, returns the token
 * that represents the reserved word.
 * Otherwise, adds the ID to the symbol table if it is not already there,
 * and returns an Optional_Token containing the matched ID and a reference
 * to it in the symbol table.
 *
 * Arguments: forward -> Pointer to memory location to begin reading from.
 *
 * Returns: A LEXERR Optional_Token if an error is encountered, or a a nil
 *          Optional_Token if no id or reserved word is matched.
 */
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
                return wrap_token(make_token(word, ID, 0, forward));
        }
}

/*
 * Machine that matches relational operators, or "Relops".
 *
 * Valid relops: '<', '>', '==', '<=', '>=', '<>'.
 *
 * Arguments: forward -> Pointer to memory location to begin reading from.
 *
 * Returns: An Optional_Token representing the matched relop, or a nil
 *          Optional_Token if no relop is matched.
 */
union Optional_Token relop_machine(char *forward)
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

/*
 * Machine that caches all other tokens not matched by a previous machine.
 *
 * If no valid token is matched by this machine, it returns a LEXERR for an
 * unrecognized symbol. This garuntees this machine will always return a token.
 *
 * Arguments: forward -> Pointer to memory location to begin reading from.
 *
 * Returns: Token either containing a valid token, attribute pair, or a LEXERR
 *          token if no valid token is matched.
 */
struct Token catchall_machine(char *forward)
{
        char value = *forward++;
        char lexeme[2];

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
                lexeme[0] = value;
                lexeme[1] = '\0';
                return make_token(lexeme, LEXERR, UNRECOG_SYM, forward);
        }
}
