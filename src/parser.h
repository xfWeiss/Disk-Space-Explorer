#ifndef PARSER_H
#define PARSER_H

#include "global.h"

typedef struct {
    char* opt;
    char* dir;
} argv_t;

argv_t argv_parse(int argc, char* argv[]);

#endif
