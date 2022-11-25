#!/bin/bash

echo "Compiling programm..."

OS=win

if [ "OS" == "lin" ] ; then
# For Linux, the mingw-64 package must be installed:
GCC=i686-w64-mingw32-gcc
AR=i686-w64-mingw32-ar
else
# For Windows mingw:
GCC=mingw32-gcc
AR=mingw32-gcc-ar
fi

PRG="vkdbf"
LIB="-lnetapi32 -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lole32 \
     -ldinput -lddraw -ldxguid -lwinmm -ldsound -lwsock32 \
     -lmsvcrt"

$GCC -c vkdbf.c -DWIN32 -DOGW

$AR -r -c -s ${PRG}.a *.o

read -p "Press ENTER to continue..." key
