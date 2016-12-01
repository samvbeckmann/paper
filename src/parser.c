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
static enum Type variable_call();
static enum Type variable_tail_call(enum Type inherited);
static void procedure_statement_call();
static void procedure_statement_tail_call();
static void expression_list_call();
static void expression_list_tail_call();
static enum Type expression_call();
static enum Type expression_tail_call(enum Type inherited);
static enum Type simple_expression_call();
static enum Type simple_expression_tail_call(enum Type inherited);
static enum Type term_call();
static enum Type term_tail_call(enum Type inherited);
static enum Type factor_call();
static enum Type factor_tail_call(enum Type inherited);
static void sign_call();


enum Type {
        INT, REAL_TYPE, AINT, AREAL, BOOL, ERR
};

static void noop() {}

static enum Type get_type(char *lexeme)
{
        // TODO: write function
        return INT;
}

void program_call()
{
        if (tok.token_type == PROGRAM) {
                match(PROGRAM);
                struct Token id_tok = tok;
                match(ID);
                check_add_scope(tok.lexeme); // TODO 
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
        case STANDARD_TYPE:
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
        case STANDARD_TYPE:
                match(STANDARD_TYPE);
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
                break;
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

static enum Type variable_call()
{
        if (tok.token_type == ID) {
                struct Token id_tok = tok;
                match(ID);
                return variable_tail_call(get_type(id_tok.lexeme));
        } else {
                synerr("'id'", tok.lexeme);
                enum Derivation dir = variable;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
                return ERR;
        }
}

static enum Type variable_tail_call(enum Type inherited)
{
        switch(tok.token_type) {
        case BR_OPEN:
                match(BR_OPEN);
                enum Type exp_type = expression_call();
                match(BR_CLOSE);
                if (exp_type == INT && inherited == AINT) {
                        return INT;
                } else if (exp_type == INT && inherited == AREAL) {
                        return REAL_TYPE;
                } else if (exp_type != INT || exp_type != REAL_TYPE) {
                        // TODO: Print semantic error
                        return ERR;
                } else if (inherited != AINT || inherited != AREAL) {
                        // TODO: Print semantic error
                        return ERR;
                } else {
                        return ERR;
                }
        case ASSIGN:
                return inherited;
        default:
                synerr("'[' or '='", tok.lexeme);
                enum Derivation dir = variable_tail;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
                return ERR;
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

static enum Type expression_call()
{
        enum Type exp_type;

        switch(tok.token_type) {
        case ID:
        case NUM:
        case PAREN_OPEN:
        case NOT:
        case ADDOP:
                exp_type = simple_expression_call();
                return expression_tail_call(exp_type);
        default:
                synerr("'id', 'num', '(', 'not', '+', or '-'", tok.lexeme);
                enum Derivation dir = expression;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
                return ERR;
        }
}

static enum Type expression_tail_call(enum Type inherited)
{
        switch(tok.token_type) {
        case RELOP:
                match(RELOP);
                enum Type exp_type = simple_expression_call();
                if (exp_type == inherited) {
                        return exp_type;
                } else {
                        // TODO: Print semantic error
                        return ERR;
                }
        case THEN:
        case DO:
        case BR_CLOSE:
        case COMMA:
        case PAREN_CLOSE:
        case SEMI:
        case ELSE:
        case END:
                return inherited;
        default:
                synerr("'>', '<', '<=' '>=', '<>', '=', 'then', 'do', ']', ',', ')', ';', 'else', or 'end'", tok.lexeme);
                enum Derivation dir = expression_tail;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
                return ERR;
        }
}

static enum Type simple_expression_call()
{
        enum Type t_type;
        switch(tok.token_type) {
        case ID:
        case NUM:
        case PAREN_OPEN:
        case NOT:
                t_type = term_call();
                return simple_expression_tail_call(t_type);
        case ADDOP:
                sign_call();
                t_type = term_call();
                return simple_expression_tail_call(t_type);
        default:
                synerr("'id', 'num', '(' 'not', '+', or '-'", tok.lexeme);
                enum Derivation dir = simple_expression;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
                return ERR;
        }
}

static enum Type simple_expression_tail_call(enum Type inherited)
{
        switch(tok.token_type) {
        case ADDOP:
                match(ADDOP);
                enum Type t_type = term_call();
                enum Type tail_type ;
                if (t_type == inherited) {
                        tail_type = t_type;
                } else {
                        // TODO: Print semantic error
                        tail_type = ERR;
                }
                return simple_expression_tail_call(tail_type);
        case RELOP:
        case THEN:
        case DO:
        case BR_CLOSE:
        case COMMA:
        case PAREN_CLOSE:
        case SEMI:
        case ELSE:
        case END:
                return inherited;
        default:
                synerr("'+', '-', 'or', '>', '<', '<=' '>=', '<>', '=', 'then', 'do', ']', ',', ')', ';', 'else', or 'end'", tok.lexeme);
                enum Derivation dir = simple_expression_tail;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
                return ERR;
        }
}

static enum Type term_call()
{
        enum Type fac_type;
        switch(tok.token_type) {
        case ID:
        case NUM:
        case PAREN_OPEN:
        case NOT:
                fac_type = factor_call();
                return term_tail_call(fac_type);
        default:
                synerr("'id', 'num' '(', or 'not'", tok.lexeme);
                enum Derivation dir = term;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
                return ERR;
        }
}

static enum Type term_tail_call(enum Type inherited)
{
        switch(tok.token_type) {
        case MULOP:
                match(MULOP);
                enum Type fac_type = factor_call();
                enum Type term_in;
                if (fac_type == inherited) {
                        term_in = fac_type;
                } else {
                        // TODO: Print semantic error
                        term_in = ERR;
                }
                return term_tail_call(term_in);
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
                return inherited;
        default:
                synerr("'*', '/', 'and', '+', '-', 'or', '>', '<', '<=' '>=', '<>', '=', 'then', 'do', ']', ',', ')', ';', 'else', or 'end'", tok.lexeme);
                enum Derivation dir = term_tail;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
                return ERR;
        }
}

static enum Type factor_call()
{
        struct Token id_tok;
        enum Type num_type;
        switch(tok.token_type) {
        case ID:
                id_tok = tok;
                match(ID);
                enum Type lex_type = get_type(id_tok.lexeme);
                // TODO: Verify id is in scope
                return factor_tail_call(lex_type);
        case NUM:
                num_type = tok.attribute.attribute == 1 ? INT : REAL_TYPE;
                match(NUM);
                return num_type;
        case PAREN_OPEN:
                match(PAREN_OPEN);
                enum Type exp_type = expression_call();
                match(PAREN_CLOSE);
                return exp_type;
        case NOT:
                match(NOT);
                enum Type fac_type = factor_call();
                if (fac_type == BOOL) {
                        return fac_type;
                } else if (fac_type == ERR) {
                        return fac_type;
                } else {
                        // TODO: Print semantic error
                        return ERR;
                }
        default:
                synerr("'id', 'num' '(', or 'not'", tok.lexeme);
                enum Derivation dir = factor;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
                return ERR;
        }
}

static enum Type factor_tail_call(enum Type inherited)
{
        switch(tok.token_type) {
        case BR_OPEN:
                match(BR_OPEN);
                enum Type exp_type = expression_call();
                match(BR_CLOSE);
                if (exp_type == INT && inherited == AINT) {
                        return INT;
                } else if (exp_type == REAL_TYPE && inherited == AREAL) {
                        return REAL_TYPE;
                } else if (exp_type != INT || exp_type != ERR) {
                        // TODO: Print semantic error
                        return ERR;
                } else if (inherited != AINT || inherited != AREAL) {
                        // TODO: Print semantic error
                        return ERR;
                } else {
                        return ERR;
                }
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
                return inherited;
        default:
                synerr("'[', '*', '/', 'and', '+', '-', 'or', '>', '<', '<=' '>=', '<>', '=', 'then', 'do', ']', ',', ')', ';', 'else', or 'end'", tok.lexeme);
                enum Derivation dir = factor_tail;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
                return ERR; // NOTE: Not sure about this
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
