/**
 *  Dokumentacja: https://www.tecgraf.puc-rio.br/iup/
 **/
#include <stdio.h>
#include <stdlib.h>
#include <iup.h>
#include <string.h>
#include <windows.h>
#include "fmgr_win.h"
#include "fmgr.h"

char current_path[1024] = "";
tree_element *selected_node = NULL;

/**
 * Funkcja obsługująca kliknięcie "Wyjdź" z menu "Plik"
 **/
int item_exit_click(void) {
  //Zamyka okno
  return IUP_CLOSE;
}

/**
 * Funkcja obsługująca kliknięcie "O programie" z menu "Pomoc"
 **/
int item_about_click(void) {
  // Otwiera nowe okno dialogowe
  // https://www.tecgraf.puc-rio.br/iup/en/dlg/iupmessage.html
  IupMessage("O programie", "File Manager\n\nAutorzy:\n   Mateusz Pietrzykowski\n   Tomasz Kuznowicz");
  return IUP_DEFAULT;
}

void reload_directory(tree_element *dir) {
  Ihandle *tree = IupGetHandle("tree");
  const int id = IupTreeGetId(tree, dir);

  //Poprawka na reloadowanie głównego folderu
  const char *basePath = dir->path;

  if (strcmp(basePath, "!") == 0) {
    //Budujemy od nowa listę dysków
    list_drives(tree);
  } else {
    //Usuwamy placeholder
    IupSetAttributeId(tree, "DELNODE", id, "CHILDREN");
    //Dodajemy podkatalogi
    list_directories(basePath, id, dir, tree);
  }
  IupSetAttributeId(tree, "STATE", id, "EXPANDED");
}

int item_exit_click_dialog(Ihandle *handle) {
  //TODO
  IupDestroy(IupGetParent(IupGetParent(handle)));
  return IUP_DEFAULT;
}


void open_dialog_cut(void) {
  Ihandle *quit_bt = IupButton("Anuluj", NULL);
  Ihandle *save_bt = IupButton("Zapisz", NULL);
  Ihandle *path_old = IupText(NULL);
  Ihandle *path_new = IupText(NULL);
  Ihandle *path_old_label = IupLabel("Ścieżka");
  Ihandle *path_new_label = IupLabel("Ścieżka docelowa");
  IupSetCallback(quit_bt, "ACTION", item_exit_click_dialog);

  Ihandle *vbox = IupVbox(
    path_old_label,
    path_old,
    path_new_label,
    path_new,
    save_bt,
    quit_bt,
    NULL);
  IupSetAttribute(vbox, "MARGIN", "10x10");
  IupSetAttribute(vbox, "GAP", "5");

  Ihandle *dialog = IupDialog(vbox);
  IupSetAttribute(dialog, "RESIZE", "NO");
  IupSetAttribute(dialog, "TITLE", "Wycinanie");
  IupSetAttributeHandle(dialog, "DEFAULTESC", quit_bt);
  IupShow(dialog);
}

void open_dialog_properties(void) {
  Ihandle *label_name = IupLabel("Nazwa pliku: ");
  Ihandle *val_name = IupLabel(selected_node->name);

  // Ihandle *box_name = IupHbox(label_name, val_name, NULL);

  Ihandle *label_path = IupLabel("Ścieżka: ");
  Ihandle *val_path = IupLabel(selected_node->path);

  // Ihandle *box_path = IupHbox(label_path, val_path, NULL);

  Ihandle *label_date_created = IupLabel("Data utworzenia: ");
  Ihandle *val_date_created = IupLabel("");

  // Ihandle *box_date_created = IupHbox(label_date_created, val_date_created, NULL);

  Ihandle *label_date_modified = IupLabel("Data modyfikacji: ");
  Ihandle *val_date_modified = IupLabel("");

  // Ihandle *box_date_modified = IupHbox(label_date_modified, val_date_modified, NULL);

  Ihandle *label_date_last_used = IupLabel("Data ostatniego użycia: ");
  Ihandle *val_date_last_used = IupLabel("");

  // Ihandle *box_date_last_used = IupHbox(label_date_last_used, val_date_last_used, NULL);

  Ihandle *label_size = IupLabel("Rozmiar: ");
  Ihandle *val_size = IupLabel("");

  // Ihandle *box_size = IupHbox(label_size, val_size, NULL);

  IupSetHandle("prop_date_created", val_date_created);
  IupSetHandle("prop_date_modified", val_date_modified);
  IupSetHandle("prop_date_last_accessed", val_date_last_used);
  IupSetHandle("prop_size", val_size);

  Ihandle *btn_close = IupButton("Zamknij", NULL);
  IupSetCallback(btn_close, "ACTION", item_exit_click_dialog);

  Ihandle *vbox = IupGridBox(
    label_name, val_name,
    label_path, val_path,
    label_date_created, val_date_created,
    label_date_modified, val_date_modified,
    label_date_last_used, val_date_last_used,
    label_size, val_size,
    btn_close, NULL);

  IupSetAttribute(vbox, "NMARGIN", "10x10");
  IupSetAttribute(vbox, "ORIENTATION", "HORIZONTAL");
  IupSetAttribute(vbox, "ALIGNMENTCOL", "ARIGHT");
  IupSetAttribute(vbox, "NORMALIZESIZE", "BOTH");
  IupSetAttribute(vbox, "EXPANDCHILDREN", "YES");
  IupSetAttribute(vbox, "HOMOGENEOUSCOL", "YES");
  IupSetAttribute(vbox, "SIZELIN", "-1");
  IupSetAttribute(vbox, "NUMDIV", "2");

  Ihandle *dialog = IupDialog(vbox);
  IupSetAttribute(dialog, "TITLE", "Właściwości");
  IupSetAttribute(dialog, "RESIZE", "NO");

  properties(selected_node->path);


  IupShow(dialog);
}

int tree_branch_opened(Ihandle *tree, const int id) {
  reload_directory(IupTreeGetUserId(tree, id));
  return IUP_DEFAULT;
}

int tree_branch_clicked(Ihandle *tree, const int id) {
  const tree_element *dir = IupTreeGetUserId(tree, id);
  IupSetAttribute(IupGetHandle("address_bar"), "VALUE", dir->path);
  return IUP_DEFAULT;
}

int tree_node_right_clicked(Ihandle *tree, const int id) {
  IupSetAttributeId(tree, "MARKED", id, "YES");
  selected_node = IupTreeGetUserId(tree, id);

  if (selected_node->is_directory) {
    const int is_root = strcmp(selected_node->path, "!") == 0;
    const int is_special = is_root || selected_node->is_special;
    IupSetAttribute(IupGetHandle("popup_tree_branch_open"), "ACTIVE", is_root?"NO":"YES");
    IupSetAttribute(IupGetHandle("popup_tree_branch_rename"), "ACTIVE", is_special?"NO":"YES");
    IupSetAttribute(IupGetHandle("popup_tree_branch_delete"), "ACTIVE", is_special?"NO":"YES");
    IupPopup(IupGetHandle("popup_tree_branch"), IUP_MOUSEPOS, IUP_MOUSEPOS);
  } else {
    IupPopup(IupGetHandle("popup_tree_leaf"), IUP_MOUSEPOS, IUP_MOUSEPOS);
  }
  return IUP_DEFAULT;
}

int popup_tree_branch_open_click(void) {
  if (selected_node != NULL) {
    open_directory_sys(selected_node->path);
  } else {
    fprintf(stderr, "Opening directory from right-click while no directory selected.\n");
  }
  return IUP_DEFAULT;
}

int popup_tree_branch_expand_click(void) {
  if (selected_node != NULL) {
    //Trzeba teraz manualnie wczytać dzieci
    const int id = IupTreeGetId(IupGetHandle("tree"), selected_node);
    tree_branch_opened(IupGetHandle("tree"), id);
    IupSetAttributeId(IupGetHandle("tree"), "STATE", id, "EXPANDED");
  } else {
    fprintf(stderr, "Expanding directory from right-click while no directory selected.\n");
  }
  return IUP_DEFAULT;
}

int popup_tree_branch_collapse_click(void) {
  if (selected_node != NULL) {
    IupSetAttributeId(IupGetHandle("tree"), "STATE", IupTreeGetId(IupGetHandle("tree"), selected_node), "COLLAPSED");
  } else {
    fprintf(stderr, "Collapsing directory from right-click while no directory selected.\n");
  }
  return IUP_DEFAULT;
}

int popup_tree_branch_delete_click(void) {
  if (selected_node != NULL) {
    delete_directory(selected_node->path);
    reload_directory(selected_node->parent);
  } else {
    fprintf(stderr, "Deleting directory from right-click while no directory selected.\n");
  }
  return IUP_DEFAULT;
}

int popup_tree_leaf_delete_click(void) {
  if (selected_node != NULL) {
    Ihandle *tree = IupGetHandle("tree");

    //Ścieżka do folderu (rodzica)
    const char *file_path = selected_node->path;
    const int parent = IupGetIntId(tree, "PARENT", IupTreeGetId(IupGetHandle("tree"), selected_node));

    delete_file(file_path);
    tree_branch_opened(tree, parent);
  } else {
    fprintf(stderr, "Deleting file from right-click while no file selected.\n");
  }
  return IUP_DEFAULT;
}

int popup_tree_node_rename_click(void) {
  if (selected_node != NULL) {
    IupSetInt(IupGetHandle("tree"), "RENAME", IupTreeGetId(IupGetHandle("tree"), selected_node));
  } else {
    fprintf(stderr, "Renaming element from right-click while no node selected.\n");
  }
  return IUP_DEFAULT;
}

void build_tree(void) {
  Ihandle* tree = IupGetHandle("tree");
  //Listowanie wszystkich dysków
  list_drives(tree);
  IupSetAttribute(tree, "STATE0","EXPANDED");
}

int action_refresh(void) {
  build_tree();
  return IUP_DEFAULT;
}

int tree_node_renamed(Ihandle *tree, const int id, const char *new_title) {
  tree_element *branch = IupTreeGetUserId(tree, id);
  const char *old_path = branch->path;

  tree_element *parent = branch->parent;
  const char* parent_path = parent->path;

  char new_path[32768];
  strcpy(new_path, parent_path);
  strcpy(new_path, SEPARATOR);
  strcat(new_path, new_title);

  rename_element(old_path, new_path);
  reload_directory(parent);
  return IUP_DEFAULT;}

/**
 * Funkcja pomocnicza do tworzenia przycisków
 * @param icon
 * @param tip
 * @return
 */
Ihandle* create_button(char icon[], char tip[]) {
  Ihandle *btn = IupButton(NULL, NULL);
  IupSetAttribute(btn, "IMAGE", icon);
  IupSetAttribute(btn, "FLAT", "Yes");
  IupSetAttribute(btn, "CANFOCUS", "No");
  IupSetAttribute(btn, "TIP", tip);
  return btn;
}

int main(int argc, char **argv) {
  //Inicjalizacja Iup i iupimglib (do ikon)
  IupOpen(&argc, &argv);
  IupImageLibOpen();

  // 1. Tworzenie menu

  // 1.1 Plik
  //Elementy menu Plik
  //W ogóle z jakiegoś powodu windows nie obsługuje polskich znaków w tych gui, dopóki nie zmienisz kodowania z utf8 na win1250.
  //Temu się trzeba przyjrzeć i ogarnąć to jakoś
  Ihandle *item_settings = IupItem("Ustawienia", NULL);
  Ihandle *item_exit = IupItem("Wyjdź", NULL);

  //Zawartość rozwijanej listy
  Ihandle *file_menu = IupMenu(
    item_settings,
    IupSeparator(),
    item_exit,
    NULL);

  //Sam przycisk rozwijający menu
  Ihandle *sub_menu_file = IupSubmenu("Plik", file_menu);


  // 1.2 Edycja (Pusty)
  Ihandle *item_cut = IupItem("Wytnij", NULL);
  Ihandle *item_copy = IupItem("Kopiuj", NULL);
  Ihandle *item_paste = IupItem("Wklej", NULL);
  Ihandle *item_delete = IupItem("Usuń", NULL);

  Ihandle *item_undo = IupItem("Cofnij", NULL);
  Ihandle *item_redo = IupItem("Ponów", NULL);

  Ihandle *item_new_folder = IupItem("Nowy plik...", NULL);
  Ihandle *item_new_file = IupItem("Nowy folder...", NULL);

  Ihandle *item_properties = IupItem("Właściwości...", NULL);
  //Zawartość rozwijanej listy
  Ihandle *edit_menu = IupMenu(item_undo, item_redo, IupSeparator(), item_cut, item_copy, item_paste, item_delete, IupSeparator(),
    item_new_folder, item_new_file, IupSeparator(), item_properties, NULL);
  //Przycisk
  Ihandle *sub_menu_edit = IupSubmenu("Edycja", edit_menu);


  Ihandle *item_back = IupItem("Wstecz", NULL);
  Ihandle *item_forward = IupItem("Do przodu", NULL);
  Ihandle *item_up = IupItem("Do góry", NULL);
  Ihandle *item_home = IupItem("Katalog domowy", NULL);
  Ihandle *item_refresh = IupItem("Odśwież", NULL);
  IupSetCallback(item_refresh, "ACTION", (Icallback)action_refresh);

  Ihandle *nav_menu = IupMenu(item_back, item_forward, item_up, item_home, item_refresh, NULL);
  Ihandle *sub_menu_nav = IupSubmenu("Nawigacja", nav_menu);


  // 1.3 Widok (Pusty)

  //Zawartość rozwijanej listy
  Ihandle *view_menu = IupMenu(NULL);
  //Przycisk
  Ihandle *sub_menu_view = IupSubmenu("Widok", view_menu);


  // 1.4 Pomoc
  //Elementy menu Pomoc
  Ihandle *item_about = IupItem("O programie...", NULL);

  //Zawartość rozwijanej listy
  Ihandle *help_menu = IupMenu(
    item_about,
    NULL);
  //Przycisk
  Ihandle *sub_menu_help = IupSubmenu("Pomoc", help_menu);

  //Pasek menu z zawartością
  Ihandle *menu = IupMenu(
    sub_menu_file,
    sub_menu_edit,
    sub_menu_nav,
    sub_menu_view,
    sub_menu_help,
    NULL);

  //Wywoływanie funkcji po naciśnięciu odpowiedniego elementu menu
  IupSetCallback(item_exit, "ACTION", (Icallback)item_exit_click);
  IupSetCallback(item_about, "ACTION", (Icallback)item_about_click);


  // 2. Toolbar z ikonami

  //Przyciski
  Ihandle *btn_left = create_button("IUP_ArrowLeft", "Wstecz");
  Ihandle *btn_right = create_button("IUP_ArrowRight", "Do przodu");
  Ihandle *btn_up = create_button("IUP_ArrowUp", "Do góry");
  Ihandle *btn_home = create_button("IUP_NavigateHome", "Katalog domowy");
  Ihandle *btn_refresh = create_button("IUP_NavigateRefresh", "Odśwież");
  IupSetCallback(btn_refresh, "ACTION", (Icallback)action_refresh);
  Ihandle *btn_cut = create_button("IUP_EditCut", "Wytnij...");
  Ihandle *btn_copy = create_button("IUP_EditCopy", "Kopiuj...");
  Ihandle *btn_paste = create_button("IUP_EditPaste", "Wklej...");
  Ihandle *btn_delete = create_button("IUP_EditErase", "Usuń...");
  Ihandle *btn_properties = create_button("IUP_FileProperties", "Właściwości...");
  Ihandle *btn_new_directory = create_button("IUP_FileClose", "Utwórz katalog...");
  Ihandle *btn_new_file = create_button("IUP_FileNew", "Utwórz plik...");
  Ihandle *btn_undo = create_button("IUP_EditUndo", "Cofnij");
  Ihandle *btn_redo = create_button("IUP_EditRedo", "Powtórz");
  Ihandle *btn_sort = create_button("IUP_ToolsSortAscend", "Sortowanie...");
  Ihandle *btn_settings = create_button("IUP_ToolsSettings", "Ustawienia");
  Ihandle *btn_help = create_button("IUP_MessageHelp", "Pomoc");

  //Sam Toolbar (hbox — horizontal box)
  Ihandle *toolbar_hb = IupHbox(
    btn_left,
    btn_right,
    btn_up,
    btn_home,
    btn_refresh,
    //Separator
    IupSetAttributes(IupLabel(NULL), "SEPARATOR=VERTICAL"),
    btn_cut,
    btn_copy,
    btn_paste,
    btn_delete,
    //Separator
    IupSetAttributes(IupLabel(NULL), "SEPARATOR=VERTICAL"),
    btn_properties,
    btn_new_directory,
    btn_new_file,
    //Separator
    IupSetAttributes(IupLabel(NULL), "SEPARATOR=VERTICAL"),
    btn_undo,
    btn_redo,
    //Separator
    IupSetAttributes(IupLabel(NULL), "SEPARATOR=VERTICAL"),
    btn_sort,
    //Separator
    IupSetAttributes(IupLabel(NULL), "SEPARATOR=VERTICAL"),
    btn_settings,
    btn_help,
    NULL);
  //Margines i odstęp
  IupSetAttribute(toolbar_hb, "MARGIN", "5x5");
  IupSetAttribute(toolbar_hb, "GAP", "5");
  IupSetAttribute(toolbar_hb, "EXPAND", "NO");

  //Label dla paska adresu
  Ihandle *address_bar_label = IupLabel("Adres:");
  IupSetAttribute(address_bar_label, "ALIGNMENT", "ACENTER:ACENTER");
  IupSetAttribute(address_bar_label, "EXPAND", "VERTICALFREE");
  IupSetAttribute(address_bar_label, "PADDING", "10");


  //Pasek adresu
  Ihandle *address_bar = IupList(NULL);
  IupSetAttribute(address_bar, "EDITBOX", "YES");
  IupSetAttribute(address_bar, "DROPDOWN", "YES");
  IupSetAttribute(address_bar, "EXPAND", "HORIZONTAL");
  IupSetHandle("address_bar", address_bar);

  //Szukajka
  Ihandle *search_bar = IupText(NULL);
  IupSetAttribute(search_bar, "TIP", "Szukaj...");
  IupSetAttribute(search_bar, "CUEBANNER", "Szukaj...");
  IupSetAttribute(search_bar, "VISIBLECOLUMNS", "16");
  IupSetAttribute(search_bar, "EXPAND", "VERTICALFREE");

  //Przycisk do szukajki
  Ihandle *btn_search = IupButton("Szukaj", NULL);

  //Kontener na pasek adresu
  Ihandle *address_bar_container = IupHbox(address_bar_label, address_bar, search_bar, btn_search, NULL);
  IupSetAttribute(address_bar_container, "MARGIN", "5x");
  IupSetAttribute(address_bar_container, "GAP", "5");

  //Drzewo katalogów
  Ihandle *dir_list = IupTree();
  IupSetHandle("tree", dir_list);
  IupSetCallback(dir_list, "BRANCHOPEN_CB", (Icallback)tree_branch_opened);
  IupSetCallback(dir_list, "EXECUTEBRANCH_CB", (Icallback)tree_branch_clicked);
  IupSetCallback(dir_list, "RIGHTCLICK_CB", (Icallback)tree_node_right_clicked);
  IupSetCallback(dir_list, "RENAME_CB", (Icallback)tree_node_renamed);
  IupSetAttribute(dir_list, "EXPAND", "VERTICAL");
  IupSetAttribute(dir_list, "ADDROOT", "NO");
  IupSetAttribute(dir_list, "SHOWRENAME", "YES");
  //Ikona plikow
  IupSetAttribute(dir_list, "IMAGELEAF", "IMGPAPER");




  // Białe tło pod toolbarem
  Ihandle *frame = IupBackgroundBox(NULL);
  IupSetAttribute(frame, "BGCOLOR", "255 255 255");
  IupSetAttribute(frame, "CANVASBOX", "YES");
  IupSetAttribute(frame, "EXPAND", "YES");

  //StatusBar
  Ihandle *status_bar = IupLabel("test statusbara");
  IupSetAttribute(status_bar, "EXPAND", "HORIZONTAL");
  IupSetAttribute(status_bar, "PADDING", "10x5");

  //Kontener do okna plikow
  Ihandle *file_list = IupVbox(
    frame,
    status_bar);


  Ihandle *main_hbox = IupHbox(
    dir_list,
    file_list,
    NULL);

  //Kontener z zawartością okna (Vbox — vertical box)
  Ihandle *vbox = IupVbox(
    toolbar_hb,
    address_bar_container,
    main_hbox,
    // frame,
    NULL);


  //Menu kontekstowe przy prawym kliknieciu katalogu w drzewie katalogu
  Ihandle *popup_tree_branch_expand = IupItem("Rozwiń", NULL);
  Ihandle *popup_tree_branch_collapse = IupItem("Zwiń", NULL);
  Ihandle *popup_tree_branch_open = IupItem("Otwórz katalog", NULL);
  Ihandle *popup_tree_branch_rename = IupItem("Zmień nazwę", NULL);
  Ihandle *popup_tree_branch_delete = IupItem("Usuń", NULL);
  Ihandle *popup_tree_branch_properties = IupItem("Właściwości", NULL);

  IupSetHandle("popup_tree_branch_open", popup_tree_branch_open);
  IupSetHandle("popup_tree_branch_rename", popup_tree_branch_rename);
  IupSetHandle("popup_tree_branch_delete", popup_tree_branch_delete);

  Ihandle *popup_tree_branch = IupMenu(
    popup_tree_branch_expand,
    popup_tree_branch_collapse,
    IupSeparator(),
    popup_tree_branch_open,
    popup_tree_branch_rename,
    popup_tree_branch_delete,
    IupSeparator(),
    popup_tree_branch_properties,
    NULL);
  IupSetHandle("popup_tree_branch", popup_tree_branch);
  IupSetCallback(popup_tree_branch_open, "ACTION", (Icallback)popup_tree_branch_open_click);
  IupSetCallback(popup_tree_branch_expand, "ACTION", (Icallback)popup_tree_branch_expand_click);
  IupSetCallback(popup_tree_branch_collapse, "ACTION", (Icallback)popup_tree_branch_collapse_click);
  IupSetCallback(popup_tree_branch_delete, "ACTION", (Icallback)popup_tree_branch_delete_click);
  IupSetCallback(popup_tree_branch_rename, "ACTION", (Icallback)popup_tree_node_rename_click);
  IupSetCallback(popup_tree_branch_properties, "ACTION", (Icallback)open_dialog_properties);


  //Menu kontekstowe przy prawym kliknieciu pliku w drzewie katalogu
  Ihandle *popup_tree_leaf_open = IupItem("Otwórz plik", NULL);
  Ihandle *popup_tree_leaf_rename = IupItem("Zmień nazwę", NULL);
  Ihandle *popup_tree_leaf_cut = IupItem("Wytnij", NULL);
  Ihandle *popup_tree_leaf_copy = IupItem("Kopiuj", NULL);
  Ihandle *popup_tree_leaf_delete = IupItem("Usuń", NULL);
  Ihandle *popup_tree_leaf_properties = IupItem("Właściwości", NULL);

  Ihandle *popup_tree_leaf = IupMenu(
    popup_tree_leaf_open,
    popup_tree_leaf_rename,
    popup_tree_leaf_cut,
    popup_tree_leaf_copy,
    popup_tree_leaf_delete,
    IupSeparator(),
    popup_tree_leaf_properties,
    NULL);
  IupSetHandle("popup_tree_leaf", popup_tree_leaf);
  IupSetCallback(popup_tree_leaf_delete, "ACTION", (Icallback)popup_tree_leaf_delete_click);
  IupSetCallback(popup_tree_leaf_rename, "ACTION", (Icallback)popup_tree_node_rename_click);
  IupSetCallback(popup_tree_leaf_cut, "ACTION", (Icallback)open_dialog_cut);
  IupSetCallback(popup_tree_leaf_properties, "ACTION", (Icallback)open_dialog_properties);

  //Główne okno
  Ihandle *dlg = IupDialog(vbox);
  //Podpięcie menu do okna
  IupSetAttributeHandle(dlg, "MENU", menu);
  //Tytuł okna
  IupSetAttribute(dlg, "TITLE", "File Explorer");
  //Rozmiar okna
  IupSetAttribute(dlg, "SIZE", "HALFxHALF");

  IupSetAttributeHandle(NULL, "PARENTDIALOG", dlg);

  //Pozycja okna
  IupShowXY(dlg, IUP_CENTERPARENT, IUP_CENTERPARENT);
  //Rozmiar okna
  IupSetAttribute(dlg, "USERSIZE", NULL);

  build_tree();

  IupMainLoop();
  IupClose();
  return EXIT_SUCCESS;
}