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

  //Przycisk - Lewo
  Ihandle *btn_left = IupButton(NULL, NULL);
  IupSetAttribute(btn_left, "IMAGE", "IUP_ArrowLeft");
  IupSetAttribute(btn_left, "FLAT", "Yes");
  IupSetAttribute(btn_left, "CANFOCUS", "No");
  IupSetAttribute(btn_left, "TIP", "Wstecz");

  //Przycisk - Prawo
  Ihandle *btn_right = IupButton(NULL, NULL);
  IupSetAttribute(btn_right, "IMAGE", "IUP_ArrowRight");
  IupSetAttribute(btn_right, "FLAT", "Yes");
  IupSetAttribute(btn_right, "CANFOCUS", "No");
  IupSetAttribute(btn_right, "TIP", "Do przodu");

  //Przycisk - Wytnij
  Ihandle *btn_cut = IupButton(NULL, NULL);
  IupSetAttribute(btn_cut, "IMAGE", "IUP_EditCut");
  IupSetAttribute(btn_cut, "FLAT", "Yes");
  IupSetAttribute(btn_cut, "CANFOCUS", "No");
  IupSetAttribute(btn_cut, "TIP", "Wytnij");

  //Przycisk - Kopiuj
  Ihandle *btn_copy = IupButton(NULL, NULL);
  IupSetAttribute(btn_copy, "IMAGE", "IUP_EditCopy");
  IupSetAttribute(btn_copy, "FLAT", "Yes");
  IupSetAttribute(btn_copy, "CANFOCUS", "No");
  IupSetAttribute(btn_copy, "TIP", "Kopiuj");

  //Przycisk - Wklej
  Ihandle *btn_paste = IupButton(NULL, NULL);
  IupSetAttribute(btn_paste, "IMAGE", "IUP_EditPaste");
  IupSetAttribute(btn_paste, "FLAT", "Yes");
  IupSetAttribute(btn_paste, "CANFOCUS", "No");
  IupSetAttribute(btn_paste, "TIP", "Wklej");

  //Przycisk - Usuń
  Ihandle *btn_delete = IupButton(NULL, NULL);
  IupSetAttribute(btn_delete, "IMAGE", "IUP_EditErase");
  IupSetAttribute(btn_delete, "FLAT", "Yes");
  IupSetAttribute(btn_delete, "CANFOCUS", "No");
  IupSetAttribute(btn_delete, "TIP", "Usuń");

  //Przycisk - Cofnij
  Ihandle *btn_undo = IupButton(NULL, NULL);
  IupSetAttribute(btn_undo, "IMAGE", "IUP_EditUndo");
  IupSetAttribute(btn_undo, "FLAT", "Yes");
  IupSetAttribute(btn_undo, "CANFOCUS", "No");
  IupSetAttribute(btn_undo, "TIP", "Cofnij");

  //Przycisk - Ponów
  Ihandle *btn_redo = IupButton(NULL, NULL);
  IupSetAttribute(btn_redo, "IMAGE", "IUP_EditRedo");
  IupSetAttribute(btn_redo, "FLAT", "Yes");
  IupSetAttribute(btn_redo, "CANFOCUS", "No");
  IupSetAttribute(btn_redo, "TIP", "Ponów");



  //Sam Toolbar (hbox - horizontal box)
  Ihandle *toolbar_hb = IupHbox(
    btn_left,
    btn_right,
    //Separator
    IupSetAttributes(IupLabel(NULL), "SEPARATOR=VERTICAL"),
    btn_cut,
    btn_copy,
    btn_paste,
    btn_delete,
    //Separator
    IupSetAttributes(IupLabel(NULL), "SEPARATOR=VERTICAL"),
    btn_undo,
    btn_redo,
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