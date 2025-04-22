#ifndef OUTPUT_H
#define OUTPUT_H

#include "global.h"

#include "converter.h"
#include "dir.h"
#include "list.h"

void display_root_path(Listdir* ldir);
int display_percentage(Listnode* n, int y, int x, Listdir* ldir);
int movement(Listdir* ldir, wchar_t c, Map** map);
void display_size(Listnode* n, const int y, const int x);
void display_listnode(Listdir* ldir);
int move_right(Listdir* t_ldir, Map** map);
int move_left(Listdir* t_ldir, Map** map);
void display_delim(Listdir* ldir);
int display_listdir(Listdir* ldir);

#endif
