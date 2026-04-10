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

char SEPARATOR[1] = {'\\'};

void list_drives(void) {

}

void list_directories(const char *basePath, const int depth, Ihandle *tree) {
    struct dirent *entry;
    DIR *dir = opendir(basePath);
    struct stat statbuf;
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
        snprintf(path, 1024, "%s%s%s", basePath, SEPARATOR, entry->d_name);
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

void delete_file(const char* path) {
    if (remove(path) == 0) {
        printf("Successfully deleted file of path: %s\n", path);
    } else {
        fprintf(stderr, "Could not delete file of path %s: %s\n", path, strerror(errno));
    }
}

void delete_directory(const char* basePath) {
    struct dirent *entry;
    DIR *dir = opendir(basePath);
    struct stat statbuf;
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
        snprintf(path, 1024, "%s%s%s", basePath, SEPARATOR, entry->d_name);
        //Sprawdzamy, czy podana Ĺ›cieĹĽka to katalog
        if (stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
            delete_directory(path);
        } else {
            delete_file(path);
        }
    }
    closedir(dir);
    if (rmdir(basePath) == 0) {
        printf("Successfully deleted directory of path: %s\n", basePath);
    } else {
        fprintf(stderr, "Could not delete directory of path %s: %s\n", basePath, strerror(errno));
    }}