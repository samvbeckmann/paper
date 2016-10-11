#include "machines.h"
#include "word_defs.h"
#include "analyzer.h"
#include "synch_set.h"
#include "parser.h"

static void program_tail_call();
static void program_tail_tail_call();
static void id_list_call();
static void id_list_tail_call();
static void declarations_call();
static void declarations_tail_call();
static void type_call();
static void standard_type_call();
static void sub_declarations_call();
static void sub_declarations_tail_call();
static void sub_declaration_call();
static void sub_declaration_tail_call();
static void sub_declaration_tail_tail_call();
static void sub_head_call();
static void sub_head_tail_call();
static void arguments_call();
static void parameter_list_call();
static void parameter_list_tail_call();
static void compound_statement_call();
static void compound_statement_tail_call();
static void optional_statements_call();
static void statement_list_call();
static void statement_list_tail_call();
static void statement_call();
static void statement_tail_call();
static void variable_call();
static void variable_tail_call();
static void procedure_statement_call();
static void procedure_statement_tail_call();
static void expression_list_call();
static void expression_list_tail_call();
static void expression_call();
static void expression_tail_call();
static void simple_expression_call();
static void simple_expression_tail_call();
static void term_call();
static void term_tail_call();
static void factor_call();
static void factor_tail_call();
static void sign_call();

void program_call()
{
        if (tok.token_type == PROGRAM) {
                match(PROGRAM);
                match(ID);
                match(PAREN_OPEN);
                id_list_call();
                match(PAREN_CLOSE);
                match(SEMI);
                program_tail_call();
        } else {
                synerr("'program'", tok.lexeme);
                enum Derivation dir = program;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void program_tail_call()
{
        switch (tok.token_type) {
        case VAR:
                declarations_call();
                program_tail_tail_call();
                break;
        case PROCEDURE:
                sub_declarations_call();
                compound_statement_call();
                match(DOT);
                break;
        case BEGIN:
                compound_statement_call();
                match(DOT);
                break;
        default:
                synerr("'var', 'procedure', or 'begin'", tok.lexeme);
                enum Derivation dir = program_tail;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void program_tail_tail_call()
{
        switch (tok.token_type) {
        case PROCEDURE:
                sub_declarations_call();
                compound_statement_call();
                match(DOT);
                break;
        case BEGIN:
                compound_statement_call();
                match(DOT);
                break;
        default:
                synerr("'procedure' or 'begin'", tok.lexeme);
                enum Derivation dir = program_tail_tail;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void id_list_call()
{
        if (tok.token_type == ID) {
                match(ID);
                id_list_tail_call();
        } else {
                synerr("'id'", tok.lexeme);
                enum Derivation dir = id_list;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void id_list_tail_call()
{
        switch (tok.token_type) {
        case COMMA:
                match(COMMA);
                match(ID);
                id_list_tail_call();
                break;
        case PAREN_CLOSE:
                break;
        default:
                synerr("',' or ')'", tok.lexeme);
                enum Derivation dir = id_list_tail;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void declarations_call()
{
        if (tok.token_type == VAR) {
                match(VAR);
                match(ID);
                match(COLON);
                type_call();
                match(SEMI);
                declarations_tail_call();
        } else {
                synerr("'var'", tok.lexeme);
                enum Derivation dir = declarations;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void declarations_tail_call()
{
        switch(tok.token_type) {
        case VAR:
                match(VAR);
                match(ID);
                match(COLON);
                type_call();
                match(SEMI);
                declarations_tail_call();
                break;
        case PROCEDURE:
        case BEGIN:
                break;
        default:
                synerr("'var', 'procedure' or 'begin'", tok.lexeme);
                enum Derivation dir = declarations_tail;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void type_call()
{
        switch(tok.token_type) {
        case INTEGER:
        case REAL:
                standard_type_call();
                break;
        case ARRAY:
                match(ARRAY);
                match(BR_OPEN);
                match(NUM);
                match(TWO_DOT);
                match(NUM);
                match(BR_CLOSE);
                match(OF);
                standard_type_call();
                break;
        default:
                synerr("'integer', 'real' or 'array'", tok.lexeme);
                enum Derivation dir = type;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void standard_type_call()
{
        switch(tok.token_type) {
        case INTEGER:
                match(INTEGER);
                break;
        case REAL:
                match(REAL);
                break;
        default:
                synerr("'integer' or 'real'", tok.lexeme);
                enum Derivation dir = standard_type;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void sub_declarations_call()
{
        if (tok.token_type == PROCEDURE) {
                sub_declaration_call();
                match(SEMI);
                sub_declarations_tail_call();
        } else {
                synerr("'procedure'", tok.lexeme);
                enum Derivation dir = sub_declarations;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void sub_declarations_tail_call()
{
        switch(tok.token_type) {
        case PROCEDURE:
                sub_declaration_call();
                match(SEMI);
                sub_declarations_tail_call();
                break;
        case BEGIN:
                break;
        default:
                synerr("'procedure' or 'begin'", tok.lexeme);
                enum Derivation dir = sub_declarations_tail;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void sub_declaration_call()
{
        if (tok.token_type == PROCEDURE) {
                sub_head_call();
                sub_declaration_tail_call();
        } else {
                synerr("'procedure'", tok.lexeme);
                enum Derivation dir = sub_declaration;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void sub_declaration_tail_call()
{
        switch(tok.token_type) {
        case VAR:
                declarations_call();
                sub_declaration_tail_tail_call();
                break;
        case PROCEDURE:
                sub_declarations_call();
                compound_statement_call();
                break;
        case BEGIN:
                compound_statement_call();
                break;
        default:
                synerr("'var', 'procedure', or 'begin'", tok.lexeme);
                enum Derivation dir = sub_declaration_tail;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void sub_declaration_tail_tail_call()
{
        switch(tok.token_type) {
        case PROCEDURE:
                sub_declarations_call();
                compound_statement_call();
                break;
        case BEGIN:
                compound_statement_call();
                break;
        default:
                synerr("'procedure' or 'begin'", tok.lexeme);
                enum Derivation dir = sub_declaration_tail_tail;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void sub_head_call()
{
        if (tok.token_type == PROCEDURE) {
                match(PROCEDURE);
                match(ID);
                sub_head_tail_call();
        } else {
                synerr("'procedure'", tok.lexeme);
                enum Derivation dir = sub_head;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void sub_head_tail_call()
{
        switch(tok.token_type) {
        case PAREN_OPEN:
                arguments_call();
                match(SEMI);
                break;
        case SEMI:
                match(SEMI);
                break;
        default:
                synerr("'(' or ';'", tok.lexeme);
                enum Derivation dir = sub_head_tail;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void arguments_call()
{
        if (tok.token_type == PAREN_OPEN) {
                match(PAREN_OPEN);
                parameter_list_call();
                match(PAREN_CLOSE);
        } else {
                synerr("'procedure'", tok.lexeme);
                enum Derivation dir = sub_head;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void parameter_list_call()
{
        if (tok.token_type == ID) {
                match(ID);
                match(COLON);
                type_call();
                parameter_list_tail_call();
        } else {
                synerr("'id'", tok.lexeme);
                enum Derivation dir = parameter_list;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void parameter_list_tail_call()
{
        switch(tok.token_type) {
        case SEMI:
                match(SEMI);
                match(ID);
                match(COLON);
                type_call();
                parameter_list_tail_call();
                break;
        case PAREN_CLOSE:
                break;
        default:
                synerr("';' or ')'", tok.lexeme);
                enum Derivation dir = parameter_list_tail;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void compound_statement_call()
{
        if (tok.token_type == BEGIN) {
                match(BEGIN);
                compound_statement_tail_call();
        } else {
                synerr("'begin'", tok.lexeme);
                enum Derivation dir = compound_statement;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void compound_statement_tail_call()
{
        switch(tok.token_type) {
        case ID:
        case CALL:
        case BEGIN:
        case IF:
        case WHILE:
                optional_statements_call();
                match(END);
                break;
        case END:
                match(END);
                break;
        default:
                synerr("'id', 'call', 'begin', 'if', 'while', or 'end'", tok.lexeme);
                enum Derivation dir = compound_statement_tail;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void optional_statements_call()
{
        switch(tok.token_type) {
        case ID:
        case CALL:
        case BEGIN:
        case IF:
        case WHILE:
                statement_list_call();
                break;
        default:
                synerr("'id', 'call', 'begin', 'if', or 'while'", tok.lexeme);
                enum Derivation dir = optional_statements;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void statement_list_call()
{
        switch(tok.token_type) {
        case ID:
        case CALL:
        case BEGIN:
        case IF:
        case WHILE:
                statement_call();
                statement_list_tail_call();
                break;
        default:
                synerr("'id', 'call', 'begin', 'if', or 'while'", tok.lexeme);
                enum Derivation dir = statement_list;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void statement_list_tail_call()
{
        switch(tok.token_type) {
        case SEMI:
                match(SEMI);
                statement_call();
                statement_list_tail_call();
                break;
        case END:
                break;
        default:
                synerr("';' or 'end'", tok.lexeme);
                enum Derivation dir = statement_list_tail;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void statement_call()
{
        switch(tok.token_type) {
        case ID:
                variable_call();
                match(ASSIGN);
                expression_call();
                break;
        case CALL:
                procedure_statement_call();
                break;
        case BEGIN:
                compound_statement_call();
        case IF:
                match(IF);
                expression_call();
                match(THEN);
                statement_call();
                statement_tail_call();
                break;
        case WHILE:
                match(WHILE);
                expression_call();
                match(DO);
                statement_call();
                break;
        default:
                synerr("'id', 'call', 'begin', 'if', or 'while'", tok.lexeme);
                enum Derivation dir = statement;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void statement_tail_call()
{
        switch(tok.token_type) {
        case ELSE:
                match(ELSE);
                statement_call();
                break;
        case SEMI:
        case END:
                break;
        default:
                synerr("'else', ';', or 'end'", tok.lexeme);
                enum Derivation dir = statement_tail;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void variable_call()
{
        if (tok.token_type == ID) {
                match(ID);
                variable_tail_call();
        } else {
                synerr("'id'", tok.lexeme);
                enum Derivation dir = variable;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void variable_tail_call()
{
        switch(tok.token_type) {
        case BR_OPEN:
                match(BR_OPEN);
                expression_call();
                match(BR_CLOSE);
                break;
        case ASSIGN:
                break;
        default:
                synerr("'[' or '='", tok.lexeme);
                enum Derivation dir = variable_tail;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void procedure_statement_call()
{
        if (tok.token_type == CALL) {
                match(CALL);
                match(ID);
                procedure_statement_tail_call();
        } else {
                synerr("'call'", tok.lexeme);
                enum Derivation dir = procedure_statement;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void procedure_statement_tail_call()
{
        switch(tok.token_type) {
        case PAREN_OPEN:
                match(PAREN_OPEN);
                expression_list_call();
                match(PAREN_CLOSE);
                break;
        case SEMI:
        case ELSE:
        case END:
                break;
        default:
                synerr("'(', ';', 'else', or 'end'", tok.lexeme);
                enum Derivation dir = procedure_statement_tail;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void expression_list_call()
{
        switch(tok.token_type) {
        case ID:
        case NUM:
        case PAREN_OPEN:
        case NOT:
        case ADDOP:
                expression_call();
                expression_list_tail_call();
                break;
        default:
                synerr("'id', 'num', '(', 'not', '+', or '-'", tok.lexeme);
                enum Derivation dir = expression_list;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void expression_list_tail_call()
{
        switch(tok.token_type) {
        case COMMA:
                match(COMMA);
                expression_call();
                expression_list_tail_call();
                break;
        case PAREN_CLOSE:
                break;
        default:
                synerr("',' or ')'", tok.lexeme);
                enum Derivation dir = expression_list_tail;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void expression_call()
{
        switch(tok.token_type) {
        case ID:
        case NUM:
        case PAREN_OPEN:
        case NOT:
        case ADDOP:
                simple_expression_call();
                expression_tail_call();
                break;
        default:
                synerr("'id', 'num', '(', 'not', '+', or '-'", tok.lexeme);
                enum Derivation dir = expression;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void expression_tail_call()
{
        switch(tok.token_type) {
        case RELOP:
                match(RELOP);
                simple_expression_call();
                break;
        case THEN:
        case DO:
        case BR_CLOSE:
        case COMMA:
        case PAREN_CLOSE:
        case SEMI:
        case ELSE:
        case END:
                break;
        default:
                synerr("'>', '<', '<=' '>=', '<>', '=', 'then', 'do', ']', ',', ')', ';', 'else', or 'end'", tok.lexeme);
                enum Derivation dir = expression_tail;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void simple_expression_call()
{
        switch(tok.token_type) {
        case ID:
        case NUM:
        case PAREN_OPEN:
        case NOT:
                term_call();
                simple_expression_tail_call();
                break;
        case ADDOP:
                sign_call();
                term_call();
                simple_expression_tail_call();
                break;
        default:
                synerr("'id', 'num', '(' 'not', '+', or '-'", tok.lexeme);
                enum Derivation dir = simple_expression;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void simple_expression_tail_call()
{
        switch(tok.token_type) {
        case ADDOP:
                match(ADDOP);
                term_call();
                simple_expression_tail_call();
                break;
        case RELOP:
        case THEN:
        case DO:
        case BR_CLOSE:
        case COMMA:
        case PAREN_CLOSE:
        case SEMI:
        case ELSE:
        case END:
                break;
        default:
                synerr("'+', '-', 'or', '>', '<', '<=' '>=', '<>', '=', 'then', 'do', ']', ',', ')', ';', 'else', or 'end'", tok.lexeme);
                enum Derivation dir = simple_expression_tail;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void term_call()
{
        switch(tok.token_type) {
        case ID:
        case NUM:
        case PAREN_OPEN:
        case NOT:
                factor_call();
                term_tail_call();
                break;
        default:
                synerr("'id', 'num' '(', or 'not'", tok.lexeme);
                enum Derivation dir = term;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void term_tail_call()
{
        switch(tok.token_type) {
        case MULOP:
                match(MULOP);
                factor_call();
                term_tail_call();
                break;
        case ADDOP:
        case RELOP:
        case THEN:
        case DO:
        case BR_CLOSE:
        case COMMA:
        case PAREN_CLOSE:
        case SEMI:
        case ELSE:
        case END:
                break;
        default:
                synerr("'*', '/', 'and', '+', '-', 'or', '>', '<', '<=' '>=', '<>', '=', 'then', 'do', ']', ',', ')', ';', 'else', or 'end'", tok.lexeme);
                enum Derivation dir = term_tail;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void factor_call()
{
        switch(tok.token_type) {
        case ID:
                match(ID);
                factor_tail_call();
                break;
        case NUM:
                match(NUM);
                break;
        case PAREN_OPEN:
                match(PAREN_OPEN);
                expression_call();
                match(PAREN_CLOSE);
                break;
        case NOT:
                match(NOT);
                factor_call();
                break;
        default:
                synerr("'id', 'num' '(', or 'not'", tok.lexeme);
                enum Derivation dir = factor;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void factor_tail_call()
{
        switch(tok.token_type) {
        case BR_OPEN:
                match(BR_OPEN);
                expression_call();
                match(BR_CLOSE);
                break;
        case MULOP:
        case ADDOP:
        case RELOP:
        case THEN:
        case DO:
        case BR_CLOSE:
        case COMMA:
        case PAREN_CLOSE:
        case SEMI:
        case ELSE:
        case END:
                break;
        default:
                synerr("'[', '*', '/', 'and', '+', '-', 'or', '>', '<', '<=' '>=', '<>', '=', 'then', 'do', ']', ',', ')', ';', 'else', or 'end'", tok.lexeme);
                enum Derivation dir = factor_tail;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}

static void sign_call()
{
        if (tok.token_type == ADDOP &&
                        (tok.attribute.attribute == ADD || tok.attribute.attribute == SUB)) {
                match(ADDOP);
        } else {
                synerr("'+' or '-'", tok.lexeme);
                enum Derivation dir = sign;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
        }
}
