//
// Created by TmkTmk on 2026-04-28.
//
#include "fmgr.h"

#include <stdlib.h>
#include <string.h>


tree_element *create_directory(tree_element *parent, const int is_special, const char *name, const char *path) {
    tree_element *ret = malloc(sizeof(struct tree_element));
    ret->name = strdup(name);
    ret->parent = parent;
    ret->is_directory = 1;
    ret->is_special = is_special;
    ret->path = strdup(path);
    ret->file_id = 0;
    return ret;
}
//todo do refactora
tree_element *create_file(tree_element *parent, const char *name, const char *path, const int file_id) {
    tree_element *ret = malloc(sizeof(struct tree_element));
    ret->name = strdup(name);
    ret->parent = parent;
    ret->is_directory = 0;
    ret->is_special = 0;
    ret->path = strdup(path);
    ret->file_id = file_id;
    return ret;
}
