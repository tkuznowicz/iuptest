//
// Created by TmkTmk on 2026-04-28.
//

#ifndef IUPTEST_FMGR_H
#define IUPTEST_FMGR_H

//Element w drzewie
typedef struct tree_element {
    //Rodzic elementu drzewa (katalog wyżej)
    struct tree_element *parent;
    int is_directory;
    //Niektóre foldery są "specjalne" i nie można na nich wykonywać pewnych operacji (Komputer, dyski)
    int is_special;
    //Nazwa elementu i pełna ścieżka wraz z jego nazwą
    char *name;
    char *path;
    int file_id;
} tree_element;
tree_element *create_directory(tree_element *parent, int is_special, const char *name, const char *path);
tree_element *create_file(tree_element *parent, const char *name, const char *path, int file_id);
#endif //IUPTEST_FMGR_H
