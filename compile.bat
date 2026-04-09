@echo off
gcc main.c -o bin/iuptest.exe -Llib/iup -Ilib/iup/include -liup -liupimglib
pause