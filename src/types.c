#include "types.h"
#include <stdio.h>

enum Type make_param(enum Type input)
{
        switch(input) {
        case INT:
                return PP_INT;
        case REAL_TYPE:
                return PP_REAL;
        case AINT:
                return PP_AINT;
        case AREAL:
                return PP_AREAL;
        default:
                printf("SEM ERR:   Unsuitable type for parameter.\n");
                return input;
        }
}
