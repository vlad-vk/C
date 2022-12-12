//---------------------------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include <conio.h>
#include <direct.h>
#include <process.h>
#include <mem.h>

#include <errno.h>
#include <time.h>
#include <string.h>
#include <signal.h>
#include <setjmp.h>
#include <malloc.h>
#include <math.h>
#include <float.h>
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>

#include  <windef.h>
#include  <nb30.h>

//---------------------------------------------------------------------------
typedef FILE           vktFL;
typedef unsigned char  vktuc;
typedef unsigned long  vktul;
typedef unsigned int   vktui;
typedef unsigned short vktus;
typedef unsigned       vkt_u;
typedef double         vkt_d;
typedef float          vkt_f;
typedef int            vkt_i;
typedef short          vkt_s;
typedef char           vkt_c;
typedef long           vkt_l;
typedef volatile int   vktvi;
typedef void           vktvd;
typedef void           vkt_v;
//---------------------------------------------------------------------------
typedef unsigned char  uc;
typedef unsigned long  ul;
typedef unsigned int   ui;
typedef double         _d;
typedef float          _f;
typedef int            _i;
typedef short          _s;
typedef char           _c;
typedef long           _l;
typedef volatile int   vi;
typedef unsigned       us;
typedef void           vd;
//---------------------------------------------------------------------------
#define Init_NB        WS_NB__Init
#define vktTB          dbTable
#define vktFT          fieldType
//---------------------------------------------------------------------------
#define vktWRA         O_BINARY | O_WRONLY | O_APPEND
#define vktWRO         O_BINARY | O_WRONLY | O_CREAT
#define vktWRT         O_BINARY | O_WRONLY | O_CREAT|O_TRUNC
#define vktRDO         O_BINARY | O_RDONLY
#define vktRDW         O_BINARY | O_RDWR
#define vktex          extern
#define ex             extern
#define vktst          static
#define vkprn          printf
//---------------------------------------------------------------------------
