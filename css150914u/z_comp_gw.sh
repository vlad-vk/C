#!/bin/bash

PRG=codusgw

GCC=i686-w64-mingw32-gcc

#GCC=mingw32-gcc

MD="-DWIN32"
MD="-DWIN32 -DWINDOW -I./allegro/include"

WD="-Wl,--subsystem,windows"
WD=""

DB="-O2 -funroll-loops -ffast-math -fomit-frame-pointer"
DB=""

RC="0_res/codus.res"

LF="-lkernel32 -luser32 -lgdi32 -lcomdlg32 -lole32 -ldinput \
    -lddraw -ldxguid -lwinmm -ldsound -lwsock32 -lnetapi32 -lwinspool \
    ./allegro/lib/liballeg.a"

$GCC ${DB} ${MD} -c -o codus.o codus.c
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

$GCC -o ${PRG}.exe codus.o \
anima/anima.o choic/choic.o dbstr/dbstr.o drcom/isrgw.o drcom/drcomgw.o \
drnbw/nbfngw.o drnbw/nbwsgw.o drtcp/drtcpgw.o polzn/polzn.o vkdbf/vkdbf.o \
${WD} ${RC} ${LF}

date
read -p "Press ENTER for contitue..." k;
