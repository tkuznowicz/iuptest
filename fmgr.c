//
// Created by TmkTmk on 2026-04-28.
//
#include "fmgr.h"

#include <stdlib.h>
#include <string.h>


tree_element *create_tree_element(tree_element *parent, const int is_directory, const int is_special, const char *name, const char *path) {
    tree_element *ret = malloc(sizeof(struct tree_element));
    ret->name = strdup(name);
    ret->parent = parent;
    ret->is_directory = is_directory;
    ret->is_special = is_special;
    ret->path = strdup(path);
    return ret;
}
