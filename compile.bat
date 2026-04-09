@echo off
gcc main.c -o filemanager.exe -Llib/iup -Ilib/iup/include -liup -liupimglib
pause