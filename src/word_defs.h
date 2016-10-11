#ifndef WORD_DEFS_H
#define WORD_DEFS_H

// token types
#define PROGRAM 10
#define FUNCTION 11
#define PROCEDURE 12
#define BEGIN 13
#define END 14
#define IF 15
#define THEN 16
#define ELSE 17
#define WHILE 18
#define DO 19
#define NOT 20
#define ARRAY 21
#define OF 22
#define VAR 23
#define EOF_TYPE 24
#define CALL 25

#define SEMI 30
#define COMMA 31
#define PAREN_OPEN 32
#define PAREN_CLOSE 33
#define BR_OPEN 34
#define BR_CLOSE 35
#define COLON 36
#define ASSIGN 37
#define DOT 38
#define TWO_DOT 39

#define NUM 40
#define ID 50
#define MULOP 60
#define ADDOP 70
#define RELOP 80
#define STANDARD_TYPE 90
#define LEXERR 99

// Addops
#define ADD 1
#define SUB 2
#define OR 3

// Mulops
#define MULT 1
#define DIVIDE 2
#define DIV 3
#define MOD 4
#define AND 5

// Relops
#define LT 1
#define GT 2
#define LT_EQ 3
#define GT_EQ 4
#define EQ 5
#define NEQ 6

// Standard types
#define INTEGER 1
#define REAL 2
#define LONG_REAL 3

// Error Codes
#define UNRECOG_SYM 1
#define EXTRA_LONG_ID 2
#define EXTRA_LONG_INT 3
#define EXTRA_LONG_REAL 4
#define LEADING_ZEROES 5

#endif
