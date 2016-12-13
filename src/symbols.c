#include "symbols.h"
#include "analyzer.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct Reserved_Word *reserved_word_table;
struct Symbol *eye;
struct Symbol *forward_eye;
static struct SymbolStack *scope_stack;

static int is_green_node(struct Symbol node);
static void print_symbol_line(FILE *out, int num_levels, struct Symbol *current);
static void print_bars(FILE *out, int num);
static void print_temp_line(FILE *out, int num);

struct Symbol * check_add_green_node(char lex[], enum Type type)
{
        struct Symbol *current = eye;
        if (current != NULL) {
                while (current -> previous != NULL) {
                        if (is_green_node(*current)) {
                                if (strcmp(lex, current -> word) == 0) {
                                        fprintf(lfp, "SEMERR:   Reuse of scope id '%s'\n", lex);
                                        return NULL;
                                }
                        }
                        current = current -> previous;
                }
        }


        // No name conflicts
        strcpy(forward_eye -> word, lex);
        forward_eye -> type = type;
        forward_eye -> offset = 0;
        forward_eye -> previous = eye;
        forward_eye -> next = malloc(sizeof(struct Symbol));
        forward_eye -> content = malloc(sizeof(struct Symbol));

        eye = forward_eye;
        forward_eye = eye -> content;

        // Add scope to stack
        struct SymbolStack *push = malloc(sizeof(struct SymbolStack));
        push -> symbol = eye;
        push -> previous = scope_stack;
        scope_stack = push;

        return eye;
}

void check_add_blue_node(char lex[], enum Type type, int offset)
{
        struct Symbol *current = eye;
        while(!is_green_node(*current)) {
                if (strcmp(lex, current -> word) == 0) {
                        fprintf(lfp, "SEMERR:   Reuse of id '%s'\n", lex);
                        return;
                } else {
                        current = current -> previous;
                }
        }

        strcpy(forward_eye -> word, lex);
        forward_eye -> type = type;
        forward_eye -> offset = offset;
        forward_eye -> previous = eye;
        forward_eye -> next = malloc(sizeof(struct Symbol));

        eye = forward_eye;
        forward_eye = eye -> next;
}

void pop_scope_stack()
{
        eye = scope_stack -> symbol;
        forward_eye = eye -> next;
        scope_stack = scope_stack -> previous;
}

void enter_num_params(int counter)
{
        scope_stack -> symbol -> num_parms = counter;
}

static int is_green_node(struct Symbol node)
{
        return node.type == PROC || node.type == PG_NAME;
}

enum Type get_type(char lex[])
{
        struct Symbol *current = eye;
        while(current -> previous != NULL) {
                if (strcmp(current -> word, lex) == 0)
                        return current -> type;
                else
                        current = current -> previous;
        }
        fprintf(lfp, "SEMERR:   Use of undeclared identifier: '%s'\n", lex);
        return ERR;
}

struct Symbol * get_proc_pointer(char lexeme[])
{

        struct Symbol *current = eye;

        while (current -> previous != NULL) {
                if (is_green_node(*current) && strcmp(current -> word, lexeme) == 0)
                        return current;
                current = current -> previous;
        }

        fprintf(lfp, "SEMERR:   Did not find pointer in stack.\n");
        return NULL;
}

void print_symbol_table(FILE *out)
{
        struct Symbol *current = eye;
        while(current -> previous != NULL) {
                current = current -> previous;
        }
        print_symbol_line(out, 0, current);
}

static void print_symbol_line(FILE *out, int num_levels, struct Symbol *current)
{

        if (is_green_node(*current)) {
                print_bars(out, num_levels);
                fprintf(out, "* SCOPE: {id: %s, type: %s, num-params: %d}\n", current -> word, get_type_name(current -> type), current -> num_parms);
                print_temp_line(out, num_levels + 1);
                print_symbol_line(out, num_levels + 1, current -> content);
                print_symbol_line(out, num_levels, current -> next);
        } else if (current -> next != NULL) {
                print_bars(out, num_levels);
                fprintf(out, "* VAR: {id: %s, type: %s, offset: %d}\n", current -> word, get_type_name(current -> type), current -> offset);
                print_symbol_line(out, num_levels, current -> next);
        }
}

static void print_bars(FILE *out, int num)
{
        for (int i = 0; i < num; i++)
                fprintf(out, "| ");
}

static void print_temp_line(FILE *out, int num)
{
        if (num > 0)
                fprintf(out, "|");

        for (int i = 1; i < num; i++)
                fprintf(out, " |");

        fprintf(out, "\\\n");
}
