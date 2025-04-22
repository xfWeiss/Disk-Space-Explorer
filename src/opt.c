#include "opt.h"

int selection_option(argv_t new_argv)
{
    if (new_argv.opt != NULL) {
        // опция '-h'
        if (strcmp(new_argv.opt, "-h") == 0) {
            help_draw();
            exit(EXIT_SUCCESS);
            // опция '-m'
        } else if (strcmp(new_argv.opt, "-m") == 0) {
            return OPT_M;
            // опция '-a'
        } else if (strcmp(new_argv.opt, "-a") == 0) {
            // '-a' передана с dir_path
            if (new_argv.dir != NULL)
                return OPT_DIR_A;
            // '-a' передана без dir_path
            else
                return OPT_A;
        }
        // указан только путь dir_path
    } else if (new_argv.dir != NULL)
        return OPT_DIR;

    return NO_OPT;
};
