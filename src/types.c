#include "types.h"
#include "analyzer.h"
#include <stdio.h>

enum Type make_param(enum Type input)
{
        switch(input) {
        case INT:
        case PP_INT:
                return PP_INT;
        case REAL_TYPE:
        case PP_REAL:
                return PP_REAL;
        case AINT:
        case PP_AINT:
                return PP_AINT;
        case AREAL:
        case PP_AREAL:
                return PP_AREAL;
        default:
                fprintf(lfp, "SEMERR:   Unsuitable type for parameter.\n");
                return input;
        }
}

const char* get_type_name(enum Type type)
{
   switch (type)
   {
      case INT: return "INT";
      case REAL_TYPE: return "REAL";
      case AINT: return "AINT";
      case AREAL: return "AREAL";
      case BOOL: return "BOOL";
      case PG_NAME: return "PGM_NAME";
      case PG_PARM: return "PGM_PARAM";
      case PROC: return "PROCEDURE";
      case PP_INT: return "PP_INT";
      case PP_REAL: return "PP_REAL";
      case PP_AINT: return "PP_AINT";
      case PP_AREAL: return "PP_AREAL";
      default: return "UNKNOWN TYPE";
   }
}

int num_type_agreement(enum Type first, enum Type second)
{
        return integer_agreement(first, second) || real_agreement(first, second);
}

int integer_agreement(enum Type first, enum Type second)
{
        return (first == INT || first == PP_INT)
                && (second == INT || second == PP_INT);
}

int real_agreement(enum Type first, enum Type second)
{
        return (first == REAL_TYPE || first == PP_REAL)
                && (second == REAL_TYPE || second == PP_REAL);
}

int verify_param(enum Type input, enum Type expected)
{
        return make_param(input) == expected;
}
