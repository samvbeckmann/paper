#include <stdlib.h>
#include "machines.h"
#include "word_defs.h"
#include "analyzer.h"
#include "synch_set.h"
#include "parser.h"
#include "types.h"
#include "symbols.h"

static void program_tail_call();
static void program_tail_tail_call();
static void id_list_call();
static void id_list_tail_call();
static void declarations_call();
static void declarations_tail_call();
static struct Decoration type_call();
static struct Decoration standard_type_call();
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
static struct Decoration variable_call();
static struct Decoration variable_tail_call(struct Decoration inherited);
static void procedure_statement_call();
static void procedure_statement_tail_call();
static void expression_list_call();
static void expression_list_tail_call();
static struct Decoration expression_call();
static struct Decoration expression_tail_call(struct Decoration inherited);
static struct Decoration simple_expression_call();
static struct Decoration simple_expression_tail_call(struct Decoration inherited);
static struct Decoration term_call();
static struct Decoration term_tail_call(struct Decoration inherited);
static struct Decoration factor_call();
static struct Decoration factor_tail_call(struct Decoration inherited);
static void sign_call();

static int offset;
static int counter;

/**
 * Creates a proper Decoration struct from a given type.
 * @param  in_type Type to assign the type field of the Decoration struct
 * @return A new Decoration that contains the type of the input
 */
static struct Decoration make_type_decoration(enum Type in_type)
{
        struct Decoration *dec = malloc(sizeof(struct Decoration));
        dec -> type = in_type;
        return *dec;
}

static struct Decoration make_decoration(enum Type in_type, int in_width)
{
        struct Decoration *dec = malloc(sizeof(struct Decoration));
        dec -> type = in_type;
        dec -> width = in_width;
        return *dec;
}

/**
 * Initializes the recursive decent parser.
 * Precondition: The first token of the source file is loaded into "tok"
 */
void program_call()
{
        if (tok.token_type == PROGRAM) {
                match(PROGRAM);
                struct Token id_tok = tok;
                match(ID);
                check_add_green_node(id_tok.lexeme, PG_NAME);
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
        offset = 0;
        counter = 0;
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
                struct Token id_tok = tok;
                match(ID);
                check_add_blue_node(id_tok.lexeme, PG_PARM, 0);
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
                struct Token id_tok = tok;
                match(ID);
                check_add_blue_node(id_tok.lexeme, PG_PARM, 0);
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
                struct Token id_tok = tok;
                match(ID);
                match(COLON);
                struct Decoration type_dec = type_call();
                check_add_blue_node(id_tok.lexeme, type_dec.type, offset);
                offset += type_dec.width;
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
                struct Token id_tok = tok;
                match(ID);
                match(COLON);
                struct Decoration type_dec = type_call();
                check_add_blue_node(id_tok.lexeme, type_dec.type, offset);
                offset += type_dec.width;
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

static struct Decoration type_call()
{
        int arrayLen;
        int ok = 0;
        switch(tok.token_type) {
        case STANDARD_TYPE:
                return standard_type_call();
        case ARRAY: // REVIEW: Not sure about the logic of array type processing
                match(ARRAY);
                match(BR_OPEN);
                struct Token num1 = tok;
                match(NUM);
                match(TWO_DOT);
                struct Token num2 = tok;
                match(NUM);
                match(BR_CLOSE);
                if (num1.token_type == NUM && num2.token_type == NUM) {
                        if (num1.token_type == REAL || num2.token_type == REAL) {
                                // TODO: Print semantic error
                        } else if (num1.token_type == INTEGER && num2.token_type == INTEGER) {
                                arrayLen = atoi(num2.lexeme) - atoi(num1.lexeme) + 1;
                                ok = 1;
                        } else {
                                // TODO: Unexpected error
                        }
                } else if (num1.token_type != LEXERR && num2.token_type != LEXERR) {
                        // TODO: Print semantic error
                }
                match(OF);
                struct Decoration std_type = standard_type_call();
                if (ok) {
                        int width = arrayLen * std_type.width;
                        if (std_type.type == INT)
                                return make_decoration(AINT, width);
                        else if (std_type.type == REAL)
                                return make_decoration(AREAL, width);
                        else {
                                // TODO: Print semantic error
                                return make_type_decoration(ERR);
                        }
                } else {
                        return make_type_decoration(ERR);
                }
        default:
                synerr("'integer', 'real' or 'array'", tok.lexeme);
                enum Derivation dir = type;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
                return make_type_decoration(ERR);
        }
}

static struct Decoration standard_type_call()
{
        int attribute;
        switch(tok.token_type) {
        case STANDARD_TYPE:
                attribute = tok.attribute.attribute;
                match(STANDARD_TYPE);
                if (attribute == 1) {
                        return make_decoration(INT, 4);
                } else {
                        return make_decoration(REAL, 8);
                }
        default:
                synerr("'integer' or 'real'", tok.lexeme);
                enum Derivation dir = standard_type;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
                return make_type_decoration(ERR);
        }
}

static void sub_declarations_call()
{
        if (tok.token_type == PROCEDURE) {
                sub_declaration_call();
                pop_scope_stack();
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
                pop_scope_stack();
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
                enter_num_params(counter);
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
                offset = 0;
                counter = 0;
                match(PROCEDURE);
                struct Token id_tok = tok;
                match(ID);
                check_add_green_node(id_tok.lexeme, PROC);
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
                struct Token id_tok = tok;
                match(ID);
                match(COLON);
                struct Decoration type = type_call();
                check_add_blue_node(id_tok.lexeme, make_param(type.type), offset);
                offset = offset + type.width;
                counter++;
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
                struct Token id_tok = tok;
                match(ID);
                match(COLON);
                struct Decoration type = type_call();
                check_add_blue_node(id_tok.lexeme, make_param(type.type), offset);
                offset = offset + type.width;
                counter++;
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

static struct Decoration variable_call()
{
        if (tok.token_type == ID) {
                struct Token id_tok = tok;
                match(ID);
                enum Type type = get_type(id_tok.lexeme);
                return variable_tail_call(make_type_decoration(type));
        } else {
                synerr("'id'", tok.lexeme);
                enum Derivation dir = variable;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
                return make_type_decoration(ERR);
        }
}

static struct Decoration variable_tail_call(struct Decoration inherited)
{
        switch(tok.token_type) {
        case BR_OPEN:
                match(BR_OPEN);
                struct Decoration exp_dec = expression_call();
                enum Type exp_type = exp_dec.type;
                match(BR_CLOSE);
                if (exp_type == INT && inherited.type == AINT) {
                        return make_type_decoration(INT);
                } else if (exp_type == INT && inherited.type == AREAL) {
                        return make_type_decoration(REAL_TYPE);
                } else if (exp_type != INT || exp_type != REAL_TYPE) {
                        // TODO: Print semantic error
                        return make_type_decoration(ERR);
                } else if (inherited.type != AINT || inherited.type != AREAL) {
                        // TODO: Print semantic error
                        return make_type_decoration(ERR);
                } else {
                        return make_type_decoration(ERR);
                }
        case ASSIGN:
                return inherited;
        default:
                synerr("'[' or '='", tok.lexeme);
                enum Derivation dir = variable_tail;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
                return make_type_decoration(ERR);
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

static struct Decoration expression_call()
{
        struct Decoration exp_type;

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
                return make_type_decoration(ERR);
        }
}

static struct Decoration expression_tail_call(struct Decoration inherited)
{
        switch(tok.token_type) {
        case RELOP:
                match(RELOP);
                struct Decoration exp_type = simple_expression_call();
                if (exp_type.type == inherited.type) {
                        return exp_type;
                } else {
                        // TODO: Print semantic error
                        return make_type_decoration(ERR);
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
                return make_type_decoration(ERR);
        }
}

static struct Decoration simple_expression_call()
{
        struct Decoration t_type;
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
                return make_type_decoration(ERR);
        }
}

static struct Decoration simple_expression_tail_call(struct Decoration inherited)
{
        switch(tok.token_type) {
        case ADDOP:
                match(ADDOP);
                struct Decoration t_type = term_call();
                struct Decoration tail_type;
                if (t_type.type == inherited.type) {
                        tail_type = t_type;
                } else {
                        // TODO: Print semantic error
                        tail_type = make_type_decoration(ERR);
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
                return make_type_decoration(ERR);
        }
}

static struct Decoration term_call()
{
        struct Decoration fac_type;
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
                return make_type_decoration(ERR);
        }
}

static struct Decoration term_tail_call(struct Decoration inherited)
{
        switch(tok.token_type) {
        case MULOP:
                match(MULOP);
                struct Decoration fac_type = factor_call();
                struct Decoration term_in;
                if (fac_type.type == inherited.type) {
                        term_in = fac_type;
                } else {
                        // TODO: Print semantic error
                        term_in = make_type_decoration(ERR);
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
                return make_type_decoration(ERR);
        }
}

static struct Decoration factor_call()
{
        struct Token id_tok;
        struct Decoration num_type;
        switch(tok.token_type) {
        case ID:
                id_tok = tok;
                match(ID);
                enum Type lex_type = get_type(id_tok.lexeme);
                return factor_tail_call(make_type_decoration(lex_type));
        case NUM:
                if (tok.attribute.attribute == 1)
                        num_type = make_type_decoration(INT);
                else {
                        num_type = make_type_decoration(REAL_TYPE);
                }
                match(NUM);
                return num_type;
        case PAREN_OPEN:
                match(PAREN_OPEN);
                struct Decoration exp_type = expression_call();
                match(PAREN_CLOSE);
                return exp_type;
        case NOT:
                match(NOT);
                struct Decoration fac_type = factor_call();
                if (fac_type.type == BOOL) {
                        return fac_type;
                } else if (fac_type.type == ERR) {
                        return fac_type;
                } else {
                        // TODO: Print semantic error
                        return make_type_decoration(ERR);
                }
        default:
                synerr("'id', 'num' '(', or 'not'", tok.lexeme);
                enum Derivation dir = factor;
                while (!synch(dir, tok.token_type))
                        tok = get_token();
                return make_type_decoration(ERR);
        }
}

static struct Decoration factor_tail_call(struct Decoration inherited)
{
        switch(tok.token_type) {
        case BR_OPEN:
                match(BR_OPEN);
                struct Decoration exp_dec = expression_call();
                enum Type exp_type = exp_dec.type;
                match(BR_CLOSE);
                if (exp_type == INT && inherited.type == AINT) {
                        return make_type_decoration(INT);
                } else if (exp_type == REAL_TYPE && inherited.type == AREAL) {
                        return make_type_decoration(REAL_TYPE);
                } else if (exp_type != INT || exp_type != ERR) {
                        // TODO: Print semantic error
                        return make_type_decoration(ERR);
                } else if (inherited.type != AINT || inherited.type != AREAL) {
                        // TODO: Print semantic error
                        return make_type_decoration(ERR);
                } else {
                        return make_type_decoration(ERR);
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
                return make_type_decoration(ERR); // NOTE: Not sure about this
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
