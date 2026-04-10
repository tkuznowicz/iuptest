//
// Created by TmkTmk on 10.04.2026.
//
#include "fmgr_win.h"
#include <iup.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <windows.h>

void list_drives(void) {

}

void list_directories(const char *basePath, const int root, const int depth, Ihandle *tree) {
    struct dirent *entry;
    DIR *dir = opendir(basePath);

    if (dir == NULL) {
        perror("Unable to open directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        char *test = malloc(1025);

        snprintf(test, 1024, "%s\\%s", basePath, entry->d_name);
        struct stat statbuf;
        if (stat(test, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
            IupSetAttributeId(tree, "ADDBRANCH", depth, entry->d_name);
            IupTreeSetUserId(tree, IupGetInt(tree, "LASTADDNODE"), test);
            IupSetAttributeId(tree, "ADDLEAF", depth+1, "<empty>");
        } else {
            IupSetAttributeId(tree, "ADDLEAF", depth, entry->d_name);
        }
    }

    closedir(dir);
}