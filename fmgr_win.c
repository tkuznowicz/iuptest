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
#include <winbase.h>

//Windowsowy separator do katalogĂłw
char SEPARATOR[1] = {'\\'};

//Funkcja do listowania drzewa od korzenia, w Windowsie to jest listowanie wszystkich dyskĂłw
void list_drives(Ihandle *tree) {
    //Usuwamy caĹ‚e drzewo
    IupSetAttribute(tree, "DELNODE", "ALL");
    //Dodajemy korzeĹ„
    IupSetAttribute(tree, "ADDBRANCH-1","Komputer");
    //Specjalny atrybut, jako ĹĽe korzeĹ„ to nie katalog i nie moĹĽna na nim wykonywaÄ‡ niektĂłrych operacji
    tree_element *main = create_tree_element(NULL, 1, 1, "", "!");

    const int last_added_id = IupGetInt(tree, "LASTADDNODE");
    IupTreeSetUserId(tree, last_added_id, main);

    //Ikona korzenia
    IupSetAttributeId(tree, "IMAGE", last_added_id, "COMPUTER");
    IupSetAttributeId(tree, "IMAGEEXPANDED", last_added_id, "COMPUTER");
    IupSetAttribute(tree, "ADDEXPANDED","NO");

    wchar_t LogicalDrives[MAX_PATH] = {0};
    const DWORD r = GetLogicalDriveStringsW(MAX_PATH, LogicalDrives);

    if (r == 0) {
        fprintf(stderr, "Unable to list drives.");
        return;
    }

    if (r <= MAX_PATH) {
        wchar_t *SingleDrive = LogicalDrives;

        while (*SingleDrive) {
            char* drive_str = malloc(8);
            wcstombs(drive_str, SingleDrive, 8);

            IupSetAttribute(tree, "ADDBRANCH",drive_str);

            tree_element *drive = create_tree_element(main, 1, 1, drive_str, drive_str);
            int last_added = IupGetInt(tree, "LASTADDNODE");
            IupTreeSetUserId(tree, last_added, drive);
            //Ikona dysku
            IupSetAttributeId(tree, "IMAGE", last_added, "DISK");
            IupSetAttributeId(tree, "IMAGEEXPANDED", last_added, "DISK");
            IupSetAttribute(tree, "ADDEXPANDED","NO");

            list_directories(drive_str, 1, drive, tree);
            SingleDrive += wcslen(SingleDrive) + 1;
        }
    }
}

void list_directories(const char *basePath, const int depth, tree_element *parent, Ihandle *tree) {
    struct dirent *entry;
    //Otwieramy katalog
    DIR *dir = opendir(basePath);
    struct stat statbuf;
    if (dir == NULL) {
        fprintf(stderr, "Unable to open directory %s: %s\n", basePath, strerror(errno));
        return;
    }

    //Wczytujemy wszystkie pliki
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        char *path = malloc(32769);

        //Formatujemy nowÄ… Ĺ›cieĹĽkÄ™
        snprintf(path, 1024, "%s%s%s", basePath, SEPARATOR, entry->d_name);
        //Sprawdzamy, czy podana Ĺ›cieĹĽka to katalog
        if (stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
            //Dodajemy nowÄ… gaĹ‚Ä…Ĺş (katalog)
            IupSetAttributeId(tree, "ADDBRANCH", depth, entry->d_name);
            //Ustawiamy Ĺ›cieĹĽkÄ™ katalogu jako jego id
            tree_element *directory = create_tree_element(parent, 1, 0, entry->d_name, path);

            //dbg
            // fprintf(stdout, "Creating branch of %s, %s, %s\n", directory->path, directory->name, path);

            IupTreeSetUserId(tree, IupGetInt(tree, "LASTADDNODE"), directory);
            //Placeholder, ĹĽeby gaĹ‚Ä…Ĺş mogĹ‚a siÄ™ rozwinÄ…Ä‡
            IupSetAttributeId(tree, "ADDLEAF", IupGetInt(tree, "LASTADDNODE"), "<empty>");
        } else {
            //Dodajemy liĹ›Ä‡ (plik)
            IupSetAttributeId(tree, "ADDLEAF", depth, entry->d_name);
            tree_element *file = create_tree_element(parent, 0, 0, entry->d_name, path);
            IupTreeSetUserId(tree,  IupGetInt(tree, "LASTADDNODE"), file);
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

void properties(const char* path)
{
    WIN32_FILE_ATTRIBUTE_DATA *attr = malloc(sizeof(WIN32_FILE_ATTRIBUTE_DATA));
    GetFileAttributesEx(path, GetFileExInfoStandard, attr);

    SYSTEMTIME *time = malloc(sizeof(SYSTEMTIME));

    //Creation time
    FileTimeToSystemTime(&attr->ftCreationTime, time);

    char *date_c = malloc(64);
    snprintf(date_c, 1024, "%hu:%hu:%hu %hu-%hu-%hu", time->wHour, time->wMinute, time->wSecond, time->wYear, time->wMonth, time->wDay);
    IupSetAttribute(IupGetHandle("prop_date_created"), "TITLE", date_c);

    //Modification time
    FileTimeToSystemTime(&attr->ftLastWriteTime, time);

    char *date_m = malloc(64);
    snprintf(date_m, 1024, "%hu:%hu:%hu %hu-%hu-%hu", time->wHour, time->wMinute, time->wSecond, time->wYear, time->wMonth, time->wDay);
    IupSetAttribute(IupGetHandle("prop_date_modified"), "TITLE", date_m);

    //Last access time
    FileTimeToSystemTime(&attr->ftLastAccessTime, time);

    char *date_a = malloc(64);
    snprintf(date_a, 1024, "%hu:%hu:%hu %hu-%hu-%hu", time->wHour, time->wMinute, time->wSecond, time->wYear, time->wMonth, time->wDay);
    IupSetAttribute(IupGetHandle("prop_date_last_accessed"), "TITLE", date_a);


    //Size
    char *l = malloc(sizeof(long long));
    ULONGLONG file_size = ((ULONGLONG)(attr->nFileSizeHigh) <<
                      sizeof(attr->nFileSizeLow) *8) |
                     attr->nFileSizeLow;
    snprintf(l, sizeof(long long), "%llu", file_size);

    IupSetAttribute(IupGetHandle("prop_size"), "TITLE", l);

}

void delete_directory(const char* basePath) {
    struct dirent *entry;
    DIR *dir = opendir(basePath);
    struct stat statbuf;
    if (dir == NULL) {
        fprintf(stderr, "Unable to open directory %s: %s\n", basePath, strerror(errno));
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
    }
}

void rename_element(const char* path, char* new_name) {
    if (rename(path, new_name) == 0) {
        printf("Successfully renamed %s to %s\n", path, new_name);
    } else {
        fprintf(stderr, "Could not rename %s to %s: %s\n", path, new_name, strerror(errno));
    }
}