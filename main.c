/**
 *  Dokumentacja: https://www.tecgraf.puc-rio.br/iup/
 **/
#include <stdio.h>
#include <stdlib.h>
#include <iup.h>

/**
 * Funkcja obs³uguj¹ca klikniêcie "WyjdŸ" z menu "Plik"
 **/
int item_exit_action_cb(void) {
  //Zamyka okno
  return IUP_CLOSE;
}

/**
 * Funkcja obs³uguj¹ca klikniêcie "O programie" z menu "Pomoc"
 **/
int item_about_action_cb(void) {
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
  //W ogóle z jakiegoœ powodu windows nie obs³uguje polskich znaków w tych gui, dopóki nie zmienisz encodingu z utf8 na win1250 xd
  //temu siê trzeba przyjrzeæ i ogarn¹æ to jakoœ
  Ihandle *item_open = IupItem("Otwórz...", NULL);
  Ihandle *item_saveas = IupItem("Zapisz jako...", NULL);
  Ihandle *item_exit = IupItem("WyjdŸ", NULL);

  //Zawartoœæ rozwijanej listy
  Ihandle *file_menu = IupMenu(
    item_open,
    item_saveas,
    //Separator
    IupSeparator(),
    item_exit,
    NULL);

  //Sam przycisk rozwijaj¹cy menu
  Ihandle *sub_menu_file = IupSubmenu("Plik", file_menu);


  // 1.2 Edycja (Pusty)

  //Zawartoœæ rozwijanej listy
  Ihandle *edit_menu = IupMenu(NULL);
  //Przycisk
  Ihandle *sub_menu_edit = IupSubmenu("Edycja", edit_menu);


  // 1.3 Widok (Pusty)

  //Zawartoœæ rozwijanej listy
  Ihandle *view_menu = IupMenu(NULL);
  //Przycisk
  Ihandle *sub_menu_view = IupSubmenu("Widok", view_menu);


  // 1.4 Pomoc
  //Elementy menu Pomoc
  Ihandle *item_about = IupItem("O programie...", NULL);

  //Zawartoœæ rozwijanej listy
  Ihandle *help_menu = IupMenu(
    item_about,
    NULL);
  //Przycisk
  Ihandle *sub_menu_help = IupSubmenu("Pomoc", help_menu);

  //Pasek menu z zawartoœci¹
  Ihandle *menu = IupMenu(
    sub_menu_file,
    sub_menu_edit,
    sub_menu_view,
    sub_menu_help,
    NULL);

  //Wywo³ywanie funkcji po naciœniêciu odpowiedniego elementu menu
  IupSetCallback(item_exit, "ACTION", (Icallback)item_exit_action_cb);
  IupSetCallback(item_about, "ACTION", (Icallback)item_about_action_cb);


  // 2. Toolbar z ikonami

  //Przycisk - Lewo
  Ihandle *btn_left = IupButton(NULL, NULL);
  IupSetAttribute(btn_left, "IMAGE", "IUP_ArrowLeft");
  IupSetAttribute(btn_left, "FLAT", "Yes");
  IupSetAttribute(btn_left, "CANFOCUS", "No");

  //Przycisk - Prawo
  Ihandle *btn_right = IupButton(NULL, NULL);
  IupSetAttribute(btn_right, "IMAGE", "IUP_ArrowRight");
  IupSetAttribute(btn_right, "FLAT", "Yes");
  IupSetAttribute(btn_right, "CANFOCUS", "No");


  //Sam Toolbar (hbox - horizontal box)
  Ihandle *toolbar_hb = IupHbox(
    btn_left,
    //Separator
    IupSetAttributes(IupLabel(NULL), "SEPARATOR=VERTICAL"),
    btn_right,
    NULL);
  //Margines i odstêp
  IupSetAttribute(toolbar_hb, "MARGIN", "5x5");
  IupSetAttribute(toolbar_hb, "GAP", "2");
  IupSetAttribute(toolbar_hb, "EXPAND", "NO");

  //Bia³e t³o pod toolbarem
  Ihandle *frame = IupBackgroundBox(NULL);
  IupSetAttribute(frame, "BGCOLOR", "255 255 255");
  IupSetAttribute(frame, "CANVASBOX", "YES");

  //Kontener z zawartoœci¹ okna (Vbox - vertical box)
  Ihandle *vbox = IupVbox(
    toolbar_hb,
    frame,
    NULL);

  //G³ówne okno
  Ihandle *dlg = IupDialog(vbox);
  //Podpiêcie menu do okna
  IupSetAttributeHandle(dlg, "MENU", menu);
  //Tytu³ okna
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