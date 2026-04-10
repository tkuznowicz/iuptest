/**
 *  Dokumentacja: https://www.tecgraf.puc-rio.br/iup/
 **/
#include <stdio.h>
#include <stdlib.h>
#include <iup.h>
#include "fmgr_win.h"

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

int tree_node_open(Ihandle *ih, const int id) {
  printf("event for %d\n", id);
  char state_name[20];
  sprintf(state_name, "TITLE%d\n", id);
  printf("%s", IupGetAttribute(ih, state_name));
  return IUP_DEFAULT;
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
  IupSetAttribute(toolbar_hb, "GAP", "2");
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

  //Drzewo katalogów
  Ihandle *dir_list = IupTree();
  IupSetHandle("tree", dir_list);
  IupSetCallback(dir_list, "BRANCHOPEN_CB", (Icallback)tree_node_open);
  IupSetAttribute(dir_list, "EXPAND", "VERTICAL");
  IupSetAttribute(dir_list, "ADDROOT", "FALSE");


  // Białe tło pod toolbarem
  Ihandle *frame = IupBackgroundBox(NULL);
  IupSetAttribute(frame, "BGCOLOR", "255 255 255");
  IupSetAttribute(frame, "CANVASBOX", "YES");
  IupSetAttribute(frame, "EXPAND", "YES");


  Ihandle *main_hbox = IupHbox(
    dir_list,
    frame,
    NULL);

  //Kontener z zawartością okna (Vbox - vertical box)
  Ihandle *vbox = IupVbox(
    toolbar_hb,
    address_bar_container,
    main_hbox,
    // frame,
    NULL);

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

  IupSetAttribute(dir_list, "ADDBRANCH-1","/");
  IupSetAttribute(dir_list, "ADDEXPANDED","NO");
  //TODO
  list_directories("C:/", 0, 0, dir_list);
  IupSetAttribute(dir_list, "STATE0","EXPANDED");

  IupMainLoop();

  IupClose();
  return EXIT_SUCCESS;
}