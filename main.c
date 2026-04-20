/**
 *  Dokumentacja: https://www.tecgraf.puc-rio.br/iup/
 **/
#include <stdio.h>
#include <stdlib.h>
#include <iup.h>
#include <string.h>
#include "fmgr_win.h"

char current_path[1024] = "";
int selected_node = -1;

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

void reload_directory(const int id) {
  Ihandle *tree = IupGetHandle("tree");
  //Usuwamy placeholder
  IupSetAttributeId(tree, "DELNODE", id, "CHILDREN");
  //Dodajemy podkatalogi
  list_directories(IupTreeGetUserId(tree, id), id, tree);
  IupSetAttributeId(tree, "STATE", id, "EXPANDED");
}


int tree_branch_opened(Ihandle *tree, const int id) {
  reload_directory(id);
  return IUP_DEFAULT;
}

int tree_branch_clicked(Ihandle *tree, const int id) {
  IupSetAttribute(IupGetHandle("address_bar"), "VALUE", IupTreeGetUserId(tree, id));
  return IUP_DEFAULT;
}

int tree_branch_right_clicked(Ihandle *tree, const int id) {
  IupPopup(IupGetHandle("popup_tree_branch"), IUP_MOUSEPOS, IUP_MOUSEPOS);
  return IUP_DEFAULT;
}

int tree_leaf_right_clicked(Ihandle *tree, const int id) {
  IupPopup(IupGetHandle("popup_tree_leaf"), IUP_MOUSEPOS, IUP_MOUSEPOS);
  return IUP_DEFAULT;
}

int tree_node_right_clicked(Ihandle *tree, const int id) {
  const int is_branch = strcmp(IupGetAttributeId(tree, "KIND", id), "LEAF");
  IupSetAttributeId(tree, "MARKED", id, "YES");
  selected_node = id;
  return is_branch ? tree_branch_right_clicked(tree, id) : tree_leaf_right_clicked(tree, id);
}

int popup_tree_branch_open_click(void) {
  if (selected_node != -1) {
    open_directory_sys(IupTreeGetUserId(IupGetHandle("tree"), selected_node));
  } else {
    fprintf(stderr, "Opening directory from right-click while no directory selected.\n");
  }
  return IUP_DEFAULT;
}

int popup_tree_branch_expand_click(void) {
  if (selected_node != -1) {
    //Trzeba teraz manualnie wczytać dzieci
    tree_branch_opened(IupGetHandle("tree"), selected_node);
    IupSetAttributeId(IupGetHandle("tree"), "STATE", selected_node, "EXPANDED");
  } else {
    fprintf(stderr, "Expanding directory from right-click while no directory selected.\n");
  }
  return IUP_DEFAULT;
}

int popup_tree_branch_collapse_click(void) {
  if (selected_node != -1) {
    IupSetAttributeId(IupGetHandle("tree"), "STATE", selected_node, "COLLAPSED");
  } else {
    fprintf(stderr, "Collapsing directory from right-click while no directory selected.\n");
  }
  return IUP_DEFAULT;
}

int popup_tree_branch_delete_click(void) {
  if (selected_node != -1) {
    delete_directory(IupTreeGetUserId(IupGetHandle("tree"), selected_node));
    const int parent = IupGetIntId(IupGetHandle("tree"), "PARENT", selected_node);
    reload_directory(parent);
  } else {
    fprintf(stderr, "Deleting directory from right-click while no directory selected.\n");
  }
  return IUP_DEFAULT;
}

int popup_tree_leaf_delete_click(void) {
  if (selected_node != -1) {
    Ihandle *tree = IupGetHandle("tree");
    //Scieżka do folderu (rodzica)
    char file_path[2048];
    const int parent = IupGetIntId(tree, "PARENT", selected_node);
    const char* parent_path = IupTreeGetUserId(tree, parent);
    strcpy(file_path, parent_path);
    strcat(file_path, SEPARATOR);
    strcat(file_path, IupGetAttributeId(tree, "TITLE", selected_node));
    delete_file(file_path);
    tree_branch_opened(tree, parent);
  } else {
    fprintf(stderr, "Deleting file from right-click while no file selected.\n");
  }
  return IUP_DEFAULT;
}

int popup_tree_node_rename_click(void) {
  if (selected_node != -1) {
    IupSetInt(IupGetHandle("tree"), "RENAME", selected_node);
  } else {
    fprintf(stderr, "Renaming element from right-click while no node selected.\n");
  }
  return IUP_DEFAULT;
}

void build_tree(void) {
  Ihandle* dir_list = IupGetHandle("tree");

  //Listowanie wszystkich dysków
  list_drives(dir_list);

  IupSetAttribute(dir_list, "STATE0","EXPANDED");
}

int action_refresh(void) {
  build_tree();
  return IUP_DEFAULT;
}

int tree_branch_renamed(Ihandle *tree, const int id, const char *new_title) {
  char old_path[2048];
  char new_path[2048];
  const int parent = IupGetIntId(tree, "PARENT", id);
  const char* parent_path = IupTreeGetUserId(tree, parent);
  strcpy(old_path, parent_path);
  strcat(old_path, SEPARATOR);
  strcpy(new_path, old_path);
  strcat(old_path, IupGetAttributeId(tree, "TITLE", id));
  strcat(new_path, new_title);
  rename_element(old_path, new_path);
  reload_directory(parent);
  return IUP_DEFAULT;
}

int tree_leaf_renamed(Ihandle *tree, const int id, const char *new_title) {
  char old_path[2048];
  char new_path[2048];
  const int parent = IupGetIntId(tree, "PARENT", id);
  const char* parent_path = IupTreeGetUserId(tree, parent);
  strcpy(old_path, parent_path);
  strcat(old_path, SEPARATOR);
  strcpy(new_path, old_path);
  strcat(old_path, IupGetAttributeId(tree, "TITLE", id));
  strcat(new_path, new_title);
  rename_element(old_path, new_path);
  reload_directory(parent);
  return IUP_DEFAULT;
}

int tree_node_renamed(Ihandle *tree, const int id, char *new_title) {
  const int is_branch = strcmp(IupGetAttributeId(tree, "KIND", id), "LEAF");
  return is_branch ? tree_branch_renamed(tree, id, new_title) : tree_leaf_renamed(tree, id, new_title);
}

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
  //Inicjalizacja Iup i IupImgLib (do ikon)
  IupOpen(&argc, &argv);
  IupImageLibOpen();

  // 1. Tworzenie menu

  // 1.1 Plik
  //Elementy menu Plik
  //W ogóle z jakiegoś powodu windows nie obsługuje polskich znaków w tych gui, dopóki nie zmienisz encodingu z utf8 na win1250 xd
  //temu się trzeba przyjrzeć i ogarnąć to jakoś
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

  //Sam Toolbar (hbox - horizontal box)
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

  //Kontener z zawartością okna (Vbox - vertical box)
  Ihandle *vbox = IupVbox(
    toolbar_hb,
    address_bar_container,
    main_hbox,
    // frame,
    NULL);


  //TEST - menu do drzewa katalogow
  Ihandle *popup_tree_branch_expand = IupItem("Rozwiń", NULL);
  Ihandle *popup_tree_branch_collapse = IupItem("Zwiń", NULL);
  Ihandle *popup_tree_branch_open = IupItem("Otwórz katalog", NULL);
  Ihandle *popup_tree_branch_rename = IupItem("Zmień nazwę", NULL);
  Ihandle *popup_tree_branch_delete = IupItem("Usuń", NULL);
  Ihandle *popup_tree_branch_properties = IupItem("Właściwości", NULL);

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


  //
  Ihandle *popup_tree_leaf_open = IupItem("Otwórz plik", NULL);
  Ihandle *popup_tree_leaf_rename = IupItem("Zmień nazwę", NULL);
  Ihandle *popup_tree_leaf_delete = IupItem("Usuń", NULL);
  Ihandle *popup_tree_leaf_properties = IupItem("Właściwości", NULL);

  Ihandle *popup_tree_leaf = IupMenu(
    popup_tree_leaf_open,
    popup_tree_leaf_rename,
    popup_tree_leaf_delete,
    IupSeparator(),
    popup_tree_leaf_properties,
    NULL);
  IupSetHandle("popup_tree_leaf", popup_tree_leaf);
  IupSetCallback(popup_tree_leaf_delete, "ACTION", (Icallback)popup_tree_leaf_delete_click);
  IupSetCallback(popup_tree_leaf_rename, "ACTION", (Icallback)popup_tree_node_rename_click);



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