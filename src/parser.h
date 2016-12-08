#ifndef PARSER_H
#define PARSER_H

#include "types.h"

struct Decoration {
        enum Type type;
        int width;
};

void program_call();

#endif
