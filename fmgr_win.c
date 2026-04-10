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

void list_directories(const char *basePath, const int depth, Ihandle *tree) {
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
        char *path = malloc(1025);

        //Formatujemy nowÄ… Ĺ›cieĹĽkÄ™
        snprintf(path, 1024, "%s\\%s", basePath, entry->d_name);
        struct stat statbuf;
        //Sprawdzamy, czy podana Ĺ›cieĹĽka to katalog
        if (stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
            //Dodajemy nowÄ… gaĹ‚Ä…Ĺş (katalog)
            IupSetAttributeId(tree, "ADDBRANCH", depth, entry->d_name);
            //Ustawiamy Ĺ›cieĹĽkÄ™ katalogu jako jego id
            IupTreeSetUserId(tree, IupGetInt(tree, "LASTADDNODE"), path);
            //Placeholder, ĹĽeby gaĹ‚Ä…Ĺş mogĹ‚a siÄ™ rozwinÄ…Ä‡
            IupSetAttributeId(tree, "ADDLEAF", IupGetInt(tree, "LASTADDNODE"), "<empty>");
        } else {
            //Dodajemy liĹ›Ä‡ (plik)
            IupSetAttributeId(tree, "ADDLEAF", depth, entry->d_name);
        }
    }
    closedir(dir);
}

void open_directory_sys(const char* path) {
    char str[2048];
    strcpy(str, "start \"\" \"");
    strcat(str, path);
    strcat(str, "\"");
    system(str);
}