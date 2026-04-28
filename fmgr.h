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
} tree_element;
tree_element *create_tree_element(tree_element *parent, const int is_directory, const int is_special, const char *name, const char *path);
#endif //IUPTEST_FMGR_H
