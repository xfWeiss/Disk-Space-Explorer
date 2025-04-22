#ifndef LIST_H
#define LIST_H

#include "global.h"

typedef struct Listnode Listnode;
typedef struct Listdir Listdir;
typedef struct Map Map;

typedef struct Listnode {
    char size_type;
    char state;
    int type;
    size_t byte;
    char* name;
    Listnode* next;
    Listnode* prev;
} Listnode;

typedef struct Listdir {
    char size_type;
    size_t byte_dir;
    char* path_dir;
    Listnode* node;
    Listdir* next;
    Listdir* prev;
} Listdir;

typedef struct Map {
    char* path_dir;
    Listnode* node;
    Map* prev;
    Map* next;
} Map;

Listnode* listnode_create(char* name, int type);
Listnode* listnode_add(Listnode* list, char* name, int type);
Listdir* listdir_create(char* path);
void listdir_add(Listdir* ldir, Listdir* newdir);
void listdir_free(Listdir* ldir);
Map* map_create(Listnode* node, char* path_dir);
void map_add(Map* map1, Map* map2);
int map_add_node(Map* map, Listdir* ldir);
void map_delete_node(Map* map);
void map_free(Map* map);

#endif
