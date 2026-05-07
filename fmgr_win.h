//
// Created by TmkTmk on 10.04.2026.
//

#ifndef IUPTEST_FMGR_WIN_H
#define IUPTEST_FMGR_WIN_H
#include "iup.h"
#include "fmgr.h"
extern char SEPARATOR[1];

void list_drives(Ihandle *tree);
void list_directories(const char *basePath, int depth, tree_element *parent, Ihandle *tree);
void open_directory_sys(const char *path);
void delete_file(const char *path);
void delete_directory(const char *basePath);
void rename_element(const char *path, char *new_name);
void properties(const char *path);
#endif //IUPTEST_FMGR_WIN_H
