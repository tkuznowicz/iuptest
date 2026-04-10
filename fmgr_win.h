//
// Created by TmkTmk on 10.04.2026.
//

#ifndef IUPTEST_FMGR_WIN_H
#define IUPTEST_FMGR_WIN_H
#include "iup.h"

void list_drives(void);
void list_directories(const char *basePath, int depth, Ihandle *tree);
void open_directory_sys(const char *path);
#endif //IUPTEST_FMGR_WIN_H
