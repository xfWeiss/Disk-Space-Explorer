#include "help.h"

void help_draw()
{
    initscr();
    curs_set(0);
    noecho();
    // создаём рамку граф.окна для вывода текста
    box(stdscr, ACS_VLINE, ACS_HLINE);

    mvprintw(2, 2, "Guide to using the dse app");
    mvprintw(4, 2, "Valid options:");
    mvprintw(6, 4, "-h\t- displays manual");
    mvprintw(7, 4, "-m\t- scan disk space usage from the starting directory");
    mvprintw(
            8,
            4,
            "-a [dir_path]\t- scan disk space usage, including hidden objects");
    mvprintw(10, 2, "Usage:");
    mvprintw(12, 4, "./dse [option] [dir_path]");
    mvprintw(14, 2, "Press 'q' to exit");
    refresh();

    char ch;
    while ((ch = getch()) != 'q') {
    }

    endwin();
};
