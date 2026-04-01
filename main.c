/**
 *  Dokumentacja: https://www.tecgraf.puc-rio.br/iup/
 **/
#include <stdio.h>
#include <stdlib.h>
#include <iup.h>

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
  Ihandle *item_open = IupItem("Otwórz...", NULL);
  Ihandle *item_saveas = IupItem("Zapisz jako...", NULL);
  Ihandle *item_exit = IupItem("Wyjdź", NULL);

  //Zawartość rozwijanej listy
  Ihandle *file_menu = IupMenu(
    item_open,
    item_saveas,
    //Separator
    IupSeparator(),
    item_exit,
    NULL);

  //Sam przycisk rozwijający menu
  Ihandle *sub_menu_file = IupSubmenu("Plik", file_menu);


  // 1.2 Edycja (Pusty)

  //Zawartość rozwijanej listy
  Ihandle *edit_menu = IupMenu(NULL);
  //Przycisk
  Ihandle *sub_menu_edit = IupSubmenu("Edycja", edit_menu);


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


  //Pasek adresu
  Ihandle *address_bar = IupList(NULL);
  IupSetAttribute(address_bar, "EDITBOX", "YES");
  IupSetAttribute(address_bar, "DROPDOWN", "YES");
  IupSetAttribute(address_bar, "EXPAND", "HORIZONTAL");


  //Kontener na pasek adresu
  Ihandle *address_bar_container = IupHbox(address_bar_label, address_bar, NULL);
  IupSetAttribute(address_bar_container, "MARGIN", "5x");
  IupSetAttribute(address_bar_container, "GAP", "10");

  //Białe tło pod toolbarem
  Ihandle *frame = IupBackgroundBox(NULL);
  IupSetAttribute(frame, "BGCOLOR", "255 255 255");
  IupSetAttribute(frame, "CANVASBOX", "YES");

  //Kontener z zawartością okna (Vbox - vertical box)
  Ihandle *vbox = IupVbox(
    toolbar_hb,
    address_bar_container,
    frame,
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

  IupMainLoop();

  IupClose();
  return EXIT_SUCCESS;
}