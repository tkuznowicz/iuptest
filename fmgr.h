//
// Created by TmkTmk on 2026-04-28.
//

#ifndef IUPTEST_FMGR_H
#define IUPTEST_FMGR_H

//Element w drzewie
struct tree_element {
    int is_directory;
    int is_special;
    char name[1024],path[32768];
};

#endif //IUPTEST_FMGR_H
