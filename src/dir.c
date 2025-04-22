#include "dir.h"

// получаем пути других каталогов
char* change_path(char* path, char* name)
{
    char* mod_path = malloc(strlen(path) + strlen(name) + 2);
    if (mod_path == NULL)
        return NULL;
    memcpy(mod_path, path, strlen(path) + 1);
    mod_path[strlen(path)] = '/';
    memcpy(&mod_path[strlen(path) + 1], name, strlen(name) + 1);
    return mod_path;
};

// определяет текущий рабочий каталог
int absolute_root_path(char** path)
{
    if (*path != NULL)
        return 1;
    *path = getcwd(*path, 0);
    return 0;
};

int fill_listnode(DIR* dir, Listdir* ldir, int flag_hidden_dir)
{
    struct dirent* entry;
    struct stat file_stat;

    // проходим по каталогу и заполняем его содержимое в структуру
    while ((entry = readdir(dir)) != NULL) {
        if (flag_hidden_dir == NO_OPT || flag_hidden_dir == OPT_DIR
            || flag_hidden_dir == OPT_M)
            if (entry->d_name[0] == '.')
                continue;

        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
            continue;

        // получаем информацию о файле или каталоге
        char* full_path = change_path(ldir->path_dir, entry->d_name);
        if (full_path == NULL)
            return 1;
        if (stat(full_path, &file_stat) == 0) {
            ldir->node = listnode_add(ldir->node, entry->d_name, entry->d_type);
            ldir->node->byte = file_stat.st_size; // сохраняем размер файла
        }
        free(full_path);
    }
    return 0;
};

// получение байтов каталога и запись в структуру
int count_bytes_dir(Listdir* ldir)
{
    // доходим до конца связного списка
    while (ldir->next != NULL)
        ldir = ldir->next;

    // с конца связного списка идем к началу списка
    // вместе с этим заполняем поле byte_dir
    while (ldir != NULL) {
        size_t count_byte = 0;
        // проходим по содержимому каталога и подсчитываем байты
        for (Listnode* node = ldir->node; node != NULL; node = node->next) {
            if (node->type == DT_DIR) {
                char* path_find_dir = change_path(ldir->path_dir, node->name);
                if (path_find_dir == NULL)
                    return 1;

                for (Listdir* t_ldir = ldir;; t_ldir = t_ldir->next) {
                    if (strcmp(t_ldir->path_dir, path_find_dir) == 0) {
                        node->byte = t_ldir->byte_dir;
                        count_byte += t_ldir->byte_dir;
                        break;
                    }
                }
                free(path_find_dir);
            } else
                count_byte += node->byte;
        }
        ldir->byte_dir = count_byte;
        ldir = ldir->prev;
    }
    return 0;
};

void swap(Listnode* node1, Listnode* node2)
{
    char* node1_name = node1->name;
    char node1_size_type = node1->size_type;
    int node1_type = node1->type;
    size_t node1_byte = node1->byte;

    node1->name = node2->name;
    node1->size_type = node2->size_type;
    node1->type = node2->type;
    node1->byte = node2->byte;
    node2->name = node1_name;
    node2->size_type = node1_size_type;
    node2->type = node1_type;
    node2->byte = node1_byte;
};

void sort_items(Listnode* node, int dir_type)
{
    int byte = node->byte;
    Listnode* replace_node = NULL;

    for (Listnode* n = node; n != NULL; n = n->next) {
        if (dir_type == DT_DIR) {
            if (n->type == DT_DIR && byte < n->byte) {
                byte = n->byte;
                replace_node = n;
            }
        } else {
            if (n->type != DT_DIR && byte < n->byte) {
                byte = n->byte;
                replace_node = n;
            }
        }
    }
    if (replace_node != NULL)
        swap(node, replace_node);
};

// сортировка элементов по не возврастанию в каждом Listdir
void sort_items_listnode(Listdir* ldir)
{
    for (Listdir* t_ldir = ldir; t_ldir != NULL; t_ldir = t_ldir->next) {
        // сортируем каталоги и объекты по категориям
        for (Listnode* node = t_ldir->node; node != NULL; node = node->next) {
            if (node->type != DT_DIR)
                for (Listnode* n = node; n != NULL; n = n->next)
                    if (n->type == DT_DIR) {
                        swap(node, n);
                        break;
                    }
        }
        // сортируем элементы каждой категории по байтам
        for (Listnode* node = t_ldir->node; node != NULL; node = node->next) {
            if (node->type == DT_DIR) {
                sort_items(node, node->type);
                continue;
            }
            sort_items(node, node->type);
        }
    }
};

// заполнение списка каталогов. ldir должен быть инициализирован
int fill_listdir(Listdir* ldir, int flag_hidden_dir, argv_t argv)
{
    DIR* dir;
    char* path = NULL;

    if (flag_hidden_dir == NO_OPT || flag_hidden_dir == OPT_A) {
        if (absolute_root_path(&path))
            return 1;
    } else if (flag_hidden_dir == OPT_DIR || flag_hidden_dir == OPT_DIR_A)
        path = argv.dir;
    else if (flag_hidden_dir == OPT_M) {
        path = getenv("HOME");
        if (path == NULL)
            return 1;
    }

    ldir->path_dir = path;

    // заполняем в структуру содержимое корневого пути
    dir = opendir(path);
    if (!dir)
        return 1;
    if (fill_listnode(dir, ldir, flag_hidden_dir))
        return 1;
    closedir(dir);
    // увеличение связных списков и добавление каталогов с их содержимых
    Listdir* t_ldir = ldir;
    for (Listdir* mod_ldir = NULL; t_ldir != NULL; t_ldir = t_ldir->next) {
        Listnode* n = t_ldir->node;
        for (char* mod_path = NULL; n != NULL; n = n->next) {
            if (n->type == DT_DIR) {
                if (strcmp(n->name, ".") == 0 || strcmp(n->name, "..") == 0)
                    continue;

                mod_path = change_path(t_ldir->path_dir, n->name);
                if (mod_path == NULL)
                    return 1;
                mod_ldir = listdir_create(mod_path);
                listdir_add(t_ldir, mod_ldir);

                dir = opendir(mod_ldir->path_dir);
                if (fill_listnode(dir, mod_ldir, flag_hidden_dir))
                    return 1;
                closedir(dir);
            }
        }
    }
    if (count_bytes_dir(ldir))
        return 1;
    sort_items_listnode(ldir);
    return 0;
};
