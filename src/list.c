#include "list.h"

#include "dir.h"

Listnode* listnode_create(char* name, int type)
{
    Listnode* p = malloc(sizeof(Listnode));
    char* name_cpy = malloc(strlen(name) + 1);
    memcpy(name_cpy, name, strlen(name) + 1);

    if (p != NULL) {
        p->name = name_cpy;
        p->type = type;
        p->state = '#';
        p->size_type = 'b';
        p->next = NULL;
        p->prev = NULL;
    }

    return p;
};

Listnode* listnode_add(Listnode* list, char* name, int type)
{
    Listnode* newnode = listnode_create(name, type);
    if (newnode == NULL)
        return list;
    newnode->next = list;

    if (list != NULL) {
        list->prev = newnode;
    }
    return newnode;
};

Listdir* listdir_create(char* path)
{
    Listdir* p = malloc(sizeof(Listdir));

    if (p != NULL) {
        p->path_dir = path;
        p->size_type = 'b';
        p->next = NULL;
        p->node = NULL;
        p->prev = NULL;
    }

    return p;
};

void listdir_add(Listdir* ldir, Listdir* newdir)
{
    while (ldir->next != NULL) {
        if (strcmp(ldir->path_dir, newdir->path_dir) == 0)
            return;
        ldir = ldir->next;
    }
    ldir->next = newdir;
    newdir->prev = ldir;
};

/* освобождение памяти для всей структуры listdir */
void listdir_free(Listdir* ldir)
{
    Listdir* p = NULL;
    Listnode* n = NULL;

    while (ldir->prev != NULL)
        ldir = ldir->prev;

    if (ldir != NULL)
        ldir = ldir->next;

    for (; ldir != NULL; ldir = ldir->next) {
        if (p != NULL) {
            free(p->path_dir);
            free(p);
        }

        for (; ldir->node != NULL; ldir->node = ldir->node->next) {
            if (n != NULL) {
                free(n->name);
                free(n);
            }
            n = ldir->node;
        }
        p = ldir;
    }
    free(p);
};

Map* map_create(Listnode* node, char* path_dir)
{
    Map* map = malloc(sizeof(Map));
    if (map == NULL)
        return NULL;

    if (map != NULL) {
        map->path_dir = path_dir;
        map->node = node;
        map->next = NULL;
        map->prev = NULL;
    }
    return map;
};

void map_add(Map* map, Map* new_map)
{
    while (map->next != NULL)
        map = map->next;
    map->next = new_map;
    new_map->prev = map;
};

// добавление элементов открытого каталога в карту
// map указывает на открытый каталог
int map_add_node(Map* map, Listdir* ldir)
{
    Map* map_cpy = map;
    Map* map_end = map->next;
    Map* new_map = NULL;

    Listnode* node = ldir->node;

    while (node != NULL) {
        new_map = map_create(node, ldir->path_dir);
        if (new_map == NULL)
            return 1;
        map_cpy->next = new_map;
        new_map->next = map_end;
        new_map->prev = map_cpy;
        map_cpy = map_cpy->next;
        node = node->next;
    }
    if (map_end != NULL && new_map != NULL)
        map_end->prev = new_map;

    return 0;
};

void map_delete_node(Map* map)
{
    Map* map_cpy = map->next;
    char* path = change_path(map->path_dir, map->node->name);

    while (map_cpy != NULL && strcmp(map_cpy->path_dir, path) == 0) {
        if (map_cpy->node->state == '@')
            map_delete_node(map_cpy);
        Map* map_delete = map_cpy;
        map_cpy = map_cpy->next;
        free(map_delete);
    }

    map->next = map_cpy;
    if (map_cpy != NULL)
        map_cpy->prev = map;
    free(path);
};

void map_free(Map* map)
{
    while (map != NULL && map->prev != NULL)
        map = map->prev;

    while (map != NULL) {
        Map* map_delete = map;
        map = map->next;
        free(map_delete);
    }
}
