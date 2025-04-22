#ifndef PATH_H
#define PATH_H

#include "global.h"

#include "list.h"
#include "opt.h"
#include "parser.h"

int absolute_root_path(char** path);
int count_bytes_dir(Listdir* ldir);
int fill_listdir(Listdir* ldir, int flag_hidden_dir, argv_t argv);
int fill_listnode(DIR* dir, Listdir* ldir, int flag_hidden_dir);
void swap(Listnode* node1, Listnode* node2);
void sort_items(Listnode* node, int dir_type);
void sort_items_listnode(Listdir* ldir);
char* change_path(char* path, char* name);

#endif
