#ifndef TYPES_H
#define TYPES_H

enum Type
{
        INT,
        REAL_TYPE,
        AINT,
        AREAL,
        BOOL,
        PG_NAME,
        PG_PARM,
        PROC,
        ERR,
        PP_INT,
        PP_REAL,
        PP_AINT,
        PP_AREAL
};

enum Type make_param(enum Type input);

const char* get_type_name(enum Type type);

int num_type_agreement(enum Type first, enum Type second);

int integer_agreement(enum Type first, enum Type second);

int real_agreement(enum Type first, enum Type second);

int verify_param(enum Type input, enum Type expected);

#endif
