#ifndef OPT_H
#define OPT_H

#include "global.h"

#include "help.h"
#include "parser.h"

#define OPT_A 0
#define NO_OPT 1
#define OPT_M 2
#define OPT_DIR 3
#define OPT_DIR_A 4

int selection_option(argv_t new_argv);

#endif
