@echo off
gcc main.c fmgr_win.c fmgr_win.h fmgr.c fmgr.h -o bin/iuptest.exe -Llib/iup -Ilib/iup/include -liup -liupimglib
pause