#include "synch_set.h"

#include "word_defs.h"

/** REVIEW: Documentation
 * [synch description]
 * @param  dir [description]
 * @param  token_type [description]
 * @return [description]
 */
int synch(enum Derivation dir, int token_type)
{
        if (token_type == EOF_TYPE)
                return 1;

        switch(dir) {
        case id_list:
        case id_list_tail:
        case parameter_list:
        case parameter_list_tail:
        case expression_list:
        case expression_list_tail:
                return token_type == PAREN_CLOSE;
        case declarations:
        case declarations_tail:
                return token_type == PROCEDURE || token_type == BEGIN;
        case type:
        case standard_type:
                return token_type == SEMI || token_type == PAREN_CLOSE;
        case sub_declarations:
        case sub_declarations_tail:
                return token_type == BEGIN;
        case sub_declaration:
        case sub_declaration_tail:
        case sub_declaration_tail_tail:
        case arguments:
                return token_type == SEMI;
        case sub_head:
        case sub_head_tail:
                return token_type == VAR
                        || token_type == PROCEDURE
                        || token_type == BEGIN;
        case compound_statement:
        case compound_statement_tail:
                return token_type == DOT
                        || token_type == SEMI
                        || token_type == ELSE
                        || token_type == END;
        case optional_statements:
        case statement_list:
        case statement_list_tail:
                return token_type == END;
        case statement:
        case statement_tail:
        case procedure_statement:
        case procedure_statement_tail:
                return token_type == SEMI
                        || token_type == ELSE
                        || token_type == END;
        case variable:
        case variable_tail:
                return token_type == ASSIGN;
        case expression:
        case expression_tail:
                return token_type == THEN
                        || token_type == DO
                        || token_type == BR_CLOSE
                        || token_type == COMMA
                        || token_type == PAREN_CLOSE
                        || token_type == SEMI
                        || token_type == ELSE
                        || token_type == END;
        case simple_expression:
        case simple_expression_tail:
                return token_type == RELOP
                        || token_type == THEN
                        || token_type == DO
                        || token_type == BR_CLOSE
                        || token_type == COMMA
                        || token_type == PAREN_CLOSE
                        || token_type == SEMI
                        || token_type == ELSE
                        || token_type == END;
        case term:
        case term_tail:
                return token_type == ADDOP
                        || token_type == RELOP
                        || token_type == THEN
                        || token_type == DO
                        || token_type == BR_CLOSE
                        || token_type == COMMA
                        || token_type == PAREN_CLOSE
                        || token_type == SEMI
                        || token_type == ELSE
                        || token_type == END;
        case factor:
        case factor_tail:
                return token_type == MULOP
                        || token_type == ADDOP
                        || token_type == RELOP
                        || token_type == THEN
                        || token_type == DO
                        || token_type == BR_CLOSE
                        || token_type == COMMA
                        || token_type == PAREN_CLOSE
                        || token_type == SEMI
                        || token_type == ELSE
                        || token_type == END;
        case sign:
                return token_type == ID
                        || token_type == NUM
                        || token_type == PAREN_OPEN
                        || token_type == NOT;
        default:
                return 0;
        }
}
