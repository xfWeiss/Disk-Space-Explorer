#include "output.h"

#define SIZE_BUF 17

// отображение абсолютного корневого пути
void display_root_path(Listdir* ldir)
{
    const int length_path = strlen(ldir->path_dir);
    wchar_t buf[length_path + 1];

    swprintf(buf, length_path + 1, L"%s", ldir->path_dir);
    mvaddstr(0, 0, "path: ");
    mvaddwstr(0, strlen("path: "), buf);
};

// отображение процентного соотношения
int display_percentage(Listnode* n, int y, int x, Listdir* ldir)
{
    // сдвиг по x
    const int shift = x + 3;
    const float percentage = ((float)n->byte / ldir->byte_dir) * 100;

    mvprintw(y, shift, "%5.2f%c", percentage, '%');
    // обходим отображение процентного соотношения
    return shift + 6;
};

void display_size(Listnode* n, const int y, const int x)
{
    const int shift = x + 4;
    size_t byte = converter(n);

    mvprintw(y, shift, "%ld %c", byte, n->size_type);
};

// выводит ступени для открытых каталогов
void display_transition(int x, int y)
{
    for (int i = 0; i < x; i++)
        mvaddstr(y, i, "-");
};

// отображение открытых каталогов
int display_next_listnode(Listdir* ldir, char* path, int y, int x)
{
    while (strcmp(ldir->path_dir, path) != 0) {
        ldir = ldir->next;
        if (ldir == NULL) {
            listdir_free(ldir);
            exit(1);
        }
    }
    for (Listnode* node = ldir->node; node != NULL; node = node->next, y++) {
        display_transition(x, y);

        wchar_t buf[SIZE_BUF];

        swprintf(buf, SIZE_BUF, L"%16s", node->name);
        mvaddwstr(y, x, buf);

        const int current_x = display_percentage(node, y, SIZE_BUF + x, ldir);

        display_size(node, y, current_x);
        if (node->type == DT_DIR)
            mvprintw(y, current_x + 14, "%c", node->state);

        if (node->type == DT_DIR && node->state == '@') {
            char* path = change_path(ldir->path_dir, node->name);
            y = display_next_listnode(ldir, path, y + 1, x + 4);
            free(path);
        }
    }
    return y - 1;
};

// отображение начального каталога
void display_listnode(Listdir* ldir)
{
    Listnode* node = ldir->node;

    while (node->prev != NULL)
        node = node->prev;

    for (int y = 2; node != NULL; node = node->next, y++) {
        wchar_t buf[SIZE_BUF];

        swprintf(buf, SIZE_BUF, L"%16s", node->name);
        mvaddwstr(y, 0, buf);

        const int x = display_percentage(node, y, SIZE_BUF, ldir);

        display_size(node, y, x);
        if (node->type == DT_DIR)
            mvprintw(y, x + 14, "%c", node->state);

        if (node->type == DT_DIR && node->state == '@') {
            char* path = change_path(ldir->path_dir, node->name);
            y = display_next_listnode(ldir, path, y + 1, 4);
            free(path);
        }
    }
};

// отображение разделителей
void display_delim(Listdir* ldir)
{
    const int cols = getmaxx(stdscr);

    for (int i = 0; i < cols; i++)
        mvaddch(1, i, '-');
};

int move_right(Listdir* t_ldir, Map** map)
{
    char* path = change_path((*map)->path_dir, (*map)->node->name);
    if (path == NULL)
        return 1;

    while (strcmp(t_ldir->path_dir, path) != 0)
        t_ldir = t_ldir->next;
    free(path);

    map_add_node(*map, t_ldir);
    (*map)->node->state = '@';

    return 0;
};

int move_left(Listdir* t_ldir, Map** map)
{
    char* path = change_path((*map)->path_dir, (*map)->node->name);
    if (path == NULL)
        return 1;

    while (strcmp(t_ldir->path_dir, path) != 0)
        t_ldir = t_ldir->next;
    free(path);

    Listnode* node = t_ldir->node;
    while (node != NULL) {
        if (node->state == '@')
            return 0;
        node = node->next;
    }
    map_delete_node(*map);
    (*map)->node->state = '#';

    return 0;
};

// управление клавишами up, down, left, right
int movement(Listdir* ldir, wchar_t c, Map** map)
{
    static int x = 0, y = 2;
    Listdir* t_ldir = ldir;

    move(y, x);
    refresh();

    if (c == KEY_UP && (*map)->prev != NULL) {
        y--;
        *map = (*map)->prev;

    } else if (c == KEY_DOWN && (*map)->next != NULL) {
        y++;
        *map = (*map)->next;

    } else if (c == KEY_RIGHT && (*map)->node->type == DT_DIR) {
        if ((*map)->node->state == '#')
            if (move_right(t_ldir, map))
                return 1;

    } else if (c == KEY_LEFT && (*map)->node->type == DT_DIR) {
        if ((*map)->node->state == '@')
            if (move_left(t_ldir, map))
                return 1;
    }
    display_listnode(ldir);
    move(y, x);

    return 0;
};

// отображение структуры listdir
int display_listdir(Listdir* ldir)
{
    wchar_t c = 0;
    Map* map = map_create(ldir->node, ldir->path_dir);
    Listnode* node = ldir->node;

    node = node->next;
    // заполняем карту корневыми элементами
    while (node != NULL) {
        Map* map1 = map_create(node, ldir->path_dir);
        map_add(map, map1);
        node = node->next;
    }

    initscr();
    scrollok(stdscr, TRUE);
    noecho();
    keypad(stdscr, TRUE);
    curs_set(1);

    setlocale(LC_ALL, "ru_RU.UTF-8");

    do {
        clear();
        display_root_path(ldir);
        display_delim(ldir);
        if (movement(ldir, c, &map))
            return 1;
        refresh();
    } while ((c = getch()) != 'q');

    map_free(map);
    endwin();

    return 0;
};
