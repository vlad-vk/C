#!/bin/bash

GCC=i586-mingw32msvc-gcc
GCC=mingw32-gcc

MD="-DWIN32 -DWINDOW"
MD="-DWIN32"

WD="-Wl,--subsystem,windows"
WD=""

DB=" -O2 -funroll-loops -ffast-math -fomit-frame-pointer"

RC="0_res/codus.res"

LF="-lalleg -lkernel32 -luser32 -lgdi32 -lcomdlg32 -lole32 -ldinput \
    -lddraw -ldxguid -lwinmm -ldsound -lwsock32 -lnetapi32 -lwinspool "

if test -f 'codus.dae'; then rm codus.dae;    fi
if test -f 'codus.dae'; then crcfl codus.dae; fi
if test -f 'codus.o';   then rm codus.o;      fi

$GCC ${DB} ${MD} -c -o anima/anima.o   anima/anima.c
$GCC ${DB} ${MD} -c -o choic/choic.o   choic/choic.c
$GCC ${DB} ${MD} -c -o dbstr/dbstr.o   dbstr/dbstr.c
$GCC ${DB} ${MD} -c -o drcom/isrgw.o   drcom/isrgw.s
$GCC ${DB} ${MD} -c -o drcom/drcomgw.o drcom/drcomgw.c
$GCC ${DB} ${MD} -c -o drnbw/nbfngw.o  drnbw/nbfngw.c
$GCC ${DB} ${MD} -c -o drnbw/nbwsgw.o  drnbw/nbwsgw.c
$GCC ${DB} ${MD} -c -o drtcp/drtcpgw.o drtcp/drtcpgw.c
$GCC ${DB} ${MD} -c -o polzn/polzn.o   polzn/polzn.c
$GCC ${DB} ${MD} -c -o vkdbf/vkdbf.o   vkdbf/vkdbf.c

$GCC ${DB} ${MD} -DWIN32 -o codusgw.exe codus.c \
anima/anima.o choic/choic.o dbstr/dbstr.o drcom/isrgw.o drcom/drcomgw.o \
drnbw/nbfngw.o drnbw/nbwsgw.o drtcp/drtcpgw.o polzn/polzn.o vkdbf/vkdbf.o \
${WD} ${RC} ${LF}

[ -f ../codus/codus.deb ] && rm -f ../codus/codus.deb

if test -f 'codusgw.exe'; then  mv  codusgw.exe ../codus/codusgw.exe; fi

date
echo -n "Press ENTER for contitue..."; read k;
