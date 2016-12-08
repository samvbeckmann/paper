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
        OK,
        UNDEC,
        ERR,
        PP_INT,
        PP_REAL,
        PP_AINT,
        PP_AREAL
};

enum Type make_param(enum Type input);

#endif
