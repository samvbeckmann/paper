#ifndef SYNCH_SET_H
#define SYNCH_SET_H

/**
 * REVIEW: Documentation
 */
enum Derivation
{
        program,
        program_tail,
        program_tail_tail,
        id_list,
        id_list_tail,
        declarations,
        declarations_tail,
        type,
        standard_type,
        sub_declarations,
        sub_declarations_tail,
        sub_declaration,
        sub_declaration_tail,
        sub_declaration_tail_tail,
        sub_head,
        sub_head_tail,
        arguments,
        parameter_list,
        parameter_list_tail,
        compound_statement,
        compound_statement_tail,
        optional_statements,
        statement_list,
        statement_list_tail,
        statement,
        statement_tail,
        variable,
        variable_tail,
        procedure_statement,
        procedure_statement_tail,
        expression_list,
        expression_list_tail,
        expression,
        expression_tail,
        simple_expression,
        simple_expression_tail,
        term,
        term_tail,
        factor,
        factor_tail,
        sign
};

int synch(enum Derivation dir, int token_type);

#endif
