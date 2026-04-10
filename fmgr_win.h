//
// Created by TmkTmk on 10.04.2026.
//

#ifndef IUPTEST_FMGR_WIN_H
#define IUPTEST_FMGR_WIN_H
#include "iup.h"

void list_drives(void);
void list_directories(const char *basePath, int root, int depth, Ihandle *tree);
#endif //IUPTEST_FMGR_WIN_H
