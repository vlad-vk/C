#!/bin/bash

echo "Compiling programm..."

OS=win

if [ "OS" == "lin" ] ; then
# For Linux, the mingw-64 package must be installed:
GCC=i686-w64-mingw32-gcc
else
# For Windows mingw:
GCC=mingw32-gcc
fi

PRG="test"
LIB="-lnetapi32 -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lole32 \
     -ldinput -lddraw -ldxguid -lwinmm -ldsound -lwsock32 \
     -lmsvcrt ./vkdbf.a"

$GCC test.c   -DWIN32 -DOGW ${LIB} -o test.exe
$GCC test01.c -DWIN32 -DOGW ${LIB} -o test01.exe
$GCC testbf.c -DWIN32 -DOGW ${LIB} -o testbf.exe
$GCC testdl.c -DWIN32 -DOGW ${LIB} -o testdl.exe

read -p "Press ENTER to continue..." key
