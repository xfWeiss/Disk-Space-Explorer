#include "parser.h"

argv_t argv_parse(int argc, char* argv[])
{
    argv_t new_argv = {NULL, NULL};
    // нет аргументов
    if (argc < 2)
        return new_argv;

    if (argc > 3) {
        fprintf(stderr, "Usage: %s [option] [dir_path]\n", argv[0]);
        exit(1);
    }
    // первый аргумент это опция
    if (argv[1][0] == '-') {
        // опция '-h' и '-m'
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-m") == 0) {
            new_argv.opt = argv[1];

            if (argc == 3) {
                fprintf(stderr,
                        "Option '%s' does not allow specify path\n",
                        argv[1]);
                exit(1);
            }
            // опция '-a'
        } else if (strcmp(argv[1], "-a") == 0) {
            new_argv.opt = argv[1];
            // 2-ой аргумент это путь
            if (argc == 3) {
                new_argv.dir = argv[2];
            }
        } else {
            fprintf(stderr, "Unknown option: '%s'\n", argv[1]);
            exit(1);
        }
    } else {
        // был передан только 1-ый аргумент и это путь
        new_argv.dir = argv[1];
    }
    // проверяем директорию
    if (new_argv.dir != NULL) {
        struct stat st;
        if (stat(new_argv.dir, &st) != 0 || !S_ISDIR(st.st_mode)) {
            fprintf(stderr,
                    "Path %s is unknown or inaccessible\n",
                    new_argv.dir);
            exit(1);
        }
    }

    return new_argv;
}
