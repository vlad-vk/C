//---------------------------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <io.h>
#include <conio.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <process.h>
#include <mem.h>
#include <malloc.h>
#include <math.h>
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>

#ifdef   OWC
   #define   COMPILER    "OWC"
   #include <i86.h>
   #include <env.h>
   #include <direct.h>
#endif

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
