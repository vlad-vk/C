// coding: cp866
//---------------------------------------------------------------------------
// Основная программа
#include <allegro.h>
#include <allegro/internal/aintern.h>
#include <winalleg.h>
//---------------------------------------------------------------------------
#include "include.h"
#include "define.h"
#include "codus.h"
#include "version.h"
#include "message.h"
//---------------------------------------------------------------------------
#include "dbstr/dbstr_e.h"
#include "drnbw/drnbw_e.h"
#include "anima/anima_e.h"
#include "choic/choic_e.h"
#include "drcom/drcom_e.h"
#include "drtcp/drtcp_e.h"
#include "polzn/polzn_e.h"
#include "vkdbf/vkdbf_e.h"
//-----------------------------------------------------------------------------
#include "15_cvvks.c"
#include "17_ldcfg.c"
#include "20_rdalr.c"
#include "21_rdchb.c"
#include "22_rdcfg.c"
#include "23_rdsav.c"
#include "24_rdprn.c"
#include "25_mdcdb.c"
#include "26_rncmd.c"
#include "30_sgraf.c"
#include "35_wncmd.c"
#include "40_timer.c"
#include "44_ldpcx.c"
#include "46_ldkbd.c"
#include "47_exdrv.c"
#include "48_sdata.c"
#include "50_farfc.c"
#include "52_edite.c"
#include "53_getvl.c"
#include "54_xcval.c"
#include "55_funct.c"
#include "56_vfile.c"
#include "57_runpr.c"
#include "58_smenu.c"
#include "59_debug.c"
#include "60_reprt.c"
#include "61_smmem.c"
#include "62_vlscr.c"
#include "63_trscr.c"
#include "64_gsscr.c"
#include "65_everc.c"
#include "71_fnasm.c"
#include "77_mesou.c"

DWORD WINAPI CodusMyCycle(void);
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
_i Mexit(_i tmode, _i exitcode){
   _i a=0;
   // выход из графического режима и установка текстового
   DPrn( 0,0,"%s\n",SMSG[20]);                      // "D: Begin function Mexit. Release memory graphics occupied."
   DPrn(20,0,"%s\n",SMSG[20]);                      // "D: Begin function Mexit. Release memory graphics occupied."
   Ust_Graph();
   set_gfx_mode(GFX_TEXT, 80, 25, 0, 0);
   CBL[14]=0;
   DPrn( 0,0,"%s\n",SMSG[22]);                      // "D: Write flag end programm."
   // выход из сетевых драйверов
   if (SSTAT> 0){ VKSPStop(); }
   if (TSTAT==1){ DRV_TCP(2); }
   TCP_ExitSRV=1;
   if(thA!=NULL) CloseHandle(thA);
   if(thN!=NULL) CloseHandle(thN);
   if(thM!=NULL) CloseHandle(thM);
   if(thC!=NULL) CloseHandle(thC);
   Sleep(200);
#ifdef WINDOW
   if(TMTI) KillTimer(NULL,TMTI);
   if(TMMS) KillTimer(NULL,TMMS);
#endif
   DPrn( 0,0,"%s\n",SMSG[26]);                      // "D: Close current database files."
   for (a=0;a<MFVW;a++){
       if(VW[a].ffl==1){ dbClose(VW[a].fhd); }
          VW[a].ffl =0;
   }
   DPrn( 0,0,"%s\n",SMSG[27]);                      // "D: Write archive buffers."
   ARCFlush();
   DPrn( 0,0,"%s\n",SMSG[28]);                      // "D: Close database archive & report."
   // закрытие баз данных
   if(ArcFL>0){ dbClose(ARCOut); }
   if(RepFL>0){ dbClose(REPOut); }
   if(UpdFL>0){ dbClose(UPDOut); }
   // запись последних данных
   ReadSave(1);
   ReadIDX (1,0);
   DPrn( 0,0,"%s\n",SMSG[23]);                      // "D: Stop network interfaces."
   // удаление файла семафора
   if(access(FNFlg,0)==0){ unlink(FNFlg); }
   sprintf(SMSG[MMSG],SMSG[29],MDate, MTime);       // "D: Time exit from programm CODUS: %s %s"
   DPrn( 0,0,"%s\n\n",SMSG[MMSG]);
   // закрытие файла записи отладочной информации
   if(DebOut!=NULL){ fclose(DebOut); }
   // выход
   DPrn( 0,0,"%s\n",SMSG[21]);                      // "D: Close graphics library."
   allegro_exit();
   exit (exitcode);
   return(0);
}
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
DWORD WINAPI CodusMyRefresh(void){
    tt_msec(); cn_tick++; if(cn_tick>9){ cn_tick=0; tt_tick(); }
    return(0);
}
//-----------------------------------------------------------------------------



#ifdef WINDOW
//-----------------------------------------------------------------------------
LRESULT CALLBACK myWNDProc(HWND myWND, UINT message, UINT wParam, LONG lParam){
   int   a;
   char *s;
   switch (message) {
        case WM_CREATE:
//              SetTimer(myWND, 1, 5, NULL);
                return 0;
        case WM_TIMER:
//              CodusMyRefresh();
                return 0;
      case WM_KEYDOWN:
                return 1;
      case WM_QUERYNEWPALETTE:
                InvalidateRect(myWND, NULL, 1);
                return 1;
      case WM_PALETTECHANGED:
                if ((HWND)wParam != myWND) {
                   myHDC=GetDC(myWND);
                   InvalidateRect(myWND, NULL, 1);
                   ReleaseDC(myHDC,myWND);
                }
                return 1;
      case WM_PAINT:
                return 0;
      case WM_DESTROY:
                Mexit(0,0);
                return (0);
   }
   return DefWindowProc(myWND, message, wParam, lParam);
}
//-----------------------------------------------------------------------------
#endif



//-----------------------------------------------------------------------------
#ifdef WINDOW
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow){
#else
int main(){
#endif
    _i a=0, b=0, f=0, fh=-1, o=0, k=0, ta=0, vm=0, n=0;
    // не запускать вторую копию программы
    vvkEVENT=OpenEvent(EVENT_ALL_ACCESS,FALSE,"CODUS_EVENT");
    if(vvkEVENT!=NULL){ exit(0); }
    vvkEVENT=CreateEvent(0,0,0,"CODUS_EVENT");
    DPrn(0,0,"Create event CODUS_EVENT.\n");
    // прочитать файл сообщений программы
    if(access(FNMSG,4)==0){ Read_FNMSG(); }
    GetTime(0);
    // открыть файл отладки для записи
    DebOut=fopen(FNDeb,"a+");
    sprintf(SMSG[MMSG],SMSG[2],MDate, MTime);           // "D: Time start programm CODUS: %s %s"
    DPrn(0,0,"\n%s\n",SMSG[MMSG]);
    sprintf(SMSG[MMSG],SMSG[3],codus_version, OSVER);   // "D: Version %.4f %s"
    DPrn(0,0,"%s\n",SMSG[MMSG]);
    HDFlg=open(FNFlg,O_BINARY|O_RDWR|O_CREAT,S_IREAD|S_IWRITE); close(HDFlg);
    DPrn(0,0,"Create flag file.\n");
#ifdef WINDOW
    // определение класса и создание окна программы
    if (!hPrevInstance) {
       wndclass.style         = CS_HREDRAW | CS_VREDRAW;
       wndclass.lpfnWndProc   = myWNDProc;
       wndclass.cbClsExtra    = 0;
       wndclass.cbWndExtra    = 0;
       wndclass.hInstance     = hInstance;
       wndclass.hIcon         = LoadIcon  (hInstance,"CODUS");
       wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
       wndclass.hbrBackground = NULL;
       wndclass.lpszMenuName  = NULL;
       wndclass.lpszClassName = szAppName;
       RegisterClass(&wndclass);
    }
    myWND = CreateWindow(szAppName,  /* window class name       */
               "CODUS",              /* window caption          */
// (WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_THICKFRAME|WS_MINIMIZEBOX|WS_MAXIMIZEBOX),
   (WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX),
               of_X,                 /* initial x position      */
               of_Y,                 /* initial y position      */
               sz_W,                 /* initial x size          */
               sz_H,                 /* initial y size          */
               NULL,                 /* parent window handle    */
               NULL,                 /* window menu handle      */
               hInstance,            /* program instance handle */
               NULL);                /* creation parameters     */
    win_set_window(myWND);
#endif
    //-------------------------------------------------------------------------
    // инициализация библиотеки Allegro
    DPrn(0,0,"%s\n",SMSG[4]);               // "D: System initialization..."
    allegro_init();
    install_keyboard();
    install_mouse();
    install_timer();
    if((os_type==OSTYPE_UNKNOWN)||
       (os_type==OSTYPE_WIN3 )||
       (os_type==OSTYPE_WIN95)||
       (os_type==OSTYPE_WIN98)){
        BIOS_FUNCTIONS=1;
    }
    // установить отображение русских символов
    set_uformat(U_ASCII);    
    three_finger_flag=FALSE;
    //-------------------------------------------------------------------------
    // инициализация массивов баз данных
    dbClearALL(); dbBFClearALL();
    DPrn(0,0,"%s\n",SMSG[0]);
    DPrn(0,0,"Clear database array.\n");
    //-------------------------------------------------------------------------
    // определение имени начального файла конфигурации
    DPrn(0,0,"%s\n",SMSG[5]);                           
    SetFNCfg();
    // чтение начального файла конфигурации
    strcpy(FirstINI,"");
    DPrn(0,0,"Read config file.\n");
    Read_Cfg();
    //-------------------------------------------------------------------------
    // проверка контрольных сумм системных файлов данных
    DPrn(0,0,"Check system files.\n");
    if((CRCheck())>0){ DPrn(0,0,"%s\n",SMSG[6]); Mexit(1,1); }
    // чтение файла кодированных строк
    Read_ESt();
    // Чтение файла базы каналов
    DPrn(0,0,"Read database channel file.\n");
    Read_CHB();
    // чтение файла сообщений
    DPrn(0,0,"Read database alarm file.\n");
    Read_Alarm();
    // восстановление сохраненных установок
    DPrn(0,0,"Read save data file.\n");
    ReadSave(0);
    if (CBL[14]==1){ ExitBAD=1; }; CBL[14]=1;
    ReadSave(1);
    // Чтение файла последней записи изменений каналов
    if(access(FNDLS,6)==0){
       if((fh=open(FNDLS,O_RDWR|O_BINARY))>=0){
          read (fh,&upd_ls,sizeof(upd_ls));
          close(fh);
    }  }
    // Чтение коммандного файла и первый вызов
    DPrn(0,0,"Read command file.\n");
    Read_CMD();
    RunCMD();
    DPrn(0,0,"%s\n",SMSG[0]);               // "D: Ok!"
    //-------------------------------------------------------------------------
    // проверка свободной памяти для запуска программы
    DPrn(13,0,"%s\n",SMSG[7]);              // "D: Check free memory..."
    SumMem();
    DPrn(13,0,"%s\n",SMSG[0]);              // "D: Ok!"
    //-------------------------------------------------------------------------
    // установка графического режима
    DPrn(13,0,"%s\n",SMSG[8]);              // "D: Set graphics mode..."
    GRInitF=1; Set_Graph();
    DPrn(13,0,"%s\n",SMSG[0]);              // "D: Ok!"
    set_display_switch_mode(SWITCH_BACKAMNESIA);    // run programm in background
    //-------------------------------------------------------------------------
    // установить функции таймеров
    DPrn(0,0,"Init and start timers.\n");
    for(a=0;a<10;a++){ LOCK_VARIABLE(HTimer[a]); LOCK_VARIABLE(mTimer[a]); }
    LOCK_FUNCTION(tt_msec);
    LOCK_FUNCTION(tt_tick);
    install_int(tt_msec, 1);
    install_int(tt_tick,10);
    // 'h'-таймер тиков, 'm'-таймер миллисекунд
    //  p1-номер таймера, p2-тип таймера('h'|'m'), p3-количество счета(0-обнуление)
    MTimer(0,'h',0);                        // таймер тиков
    MTimer(1,'h',0);                        // таймер секунд
    MTimer(2,'h',0);                        // таймер обработки сообщений
    MTimer(3,'h',0);                        // таймер вывода строки статуса
    MTimer(4,'h',0);                        // таймер вывода строки сообщений
    MTimer(5,'h',0);                        // таймер обработки переменных
    MTimer(6,'h',0);                        // таймер вывода строк в окнах
    MTimer(7,'h',0);                        // таймер таймаутов COM порта
    MTimer(8,'h',0);                        //
    MTimer(9,'h',0);                        //
    DPrn(13,0,"%s\n",SMSG[0]);
    //-------------------------------------------------------------------------
    // установка начальных значений

    // установка скорости движения мышки (по-умолчанию=2)
    XSpeed=10; YSpeed=10; set_mouse_speed(XSpeed,YSpeed);

    // определение начального файла конфигурации (загрузки)
    strcpy(FNCfg,FNini);                    // считать стартовый .ini файл
    strcpy(PN_fn[0],FNini);                 //
    strcpy(PN_fn[1],FNini);                 //
    strcpy(PN_CC,FNCfg);                    //
    FNCfgLD=1;                              //
    memset(GS.ES,0,SLEN);                   // очистка временной строки ввода
    DPrn(DEBLDI,0,"%s\n",SMSG[14]);         // ""
    //-------------------------------------------------------------------------
    CBCMD=1;             

    //-------------------------------------------------------------------------
    // запустить в фоне процессы обмена и перерисовки
    DPrn(0,0,"Create thread COM port exchange.\n");
    thA=CreateThread(NULL,0,
                (LPTHREAD_START_ROUTINE) &ExchangeCOM, 0, 0,
                (PDWORD) &thIDA);
    DPrn(0,0,"Create thread NetBIOS exchange.\n");
    thN=CreateThread(NULL,0,
                (LPTHREAD_START_ROUTINE) &WS_NB__NetChange, 0, 0,
                (PDWORD) &thIDN);
#ifdef WINDOW
    // WIN32 WINDOW
    DPrn(0,0,"Create thread My Public Cycle.\n");
    thM=CreateThread(NULL,0,
                (LPTHREAD_START_ROUTINE) &CodusMyCycle, 0, 0,
                (PDWORD) &thIDM);
    ShowWindow  (myWND, nCmdShow);
    UpdateWindow(myWND);
    while(GetMessage(&myMSG, NULL, 0, 0)){
        TranslateMessage(&myMSG);
        DispatchMessage (&myMSG);
    }
    return myMSG.wParam;
}
#else
    // WIN32 CONSOLE
    CodusMyCycle(); Mexit(0,1); return(0);
} END_OF_MAIN()
#endif
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
DWORD WINAPI CodusMyCycle(void){
    _i a=0, b=0, f=0, fh=-1, o=0, k=0, ta=0, vm=0, n=0;
//  win_set_window(myWND);
//  TMMS=SetTimer(NULL, 0,  1, (TIMERPROC)tt_msec);
//  TMTI=SetTimer(NULL, 0, 10, (TIMERPROC)tt_tick);
    //-------------------------------------------------------------------------
    DPrn(0,0,"Start: Public cycle...\n");
    for(;;){
        Sleep(SYSLEEP);
        if(TCP_ExitSRV) break;
      //-----------------------------------------------------------------------
      // Если это первый вызов
      if ( CFirst == 0 ){
           GetTime(1);       // получить текущее время и дату
           FARName();        // определить имена файлов архива,отчета тревог
           FAROpen();        // открыть эти файлы
           FUPOpen();        //
           CFirst =1;        // сбросить флаг первого вызова
           CBNUMMSGP[MAXMCH]=1; // записать это сообщение в файл
           rectfill(screen,0,0,XMAX,YMAX,0);  // чистка экрана после заставки
           continue;
      }
      // Читаем файл конфигурации
      if ( FNCfgLD==1  ){
           DPrn(DEBLDI,0,"\n%s\n",SMSG[15]);        // "D: Begin configuration read."
           if( (Read_Cfg()) == 2 ) { continue; }
           DPrn(DEBLDI,0,"%s\n\n",SMSG[16]);        // "D: End configuration read."
      }
      // Если было сообщение об ошибке или другое внеочередное-
      // вывести его в строку статуса
      if(ErrINI>0){
         if(ErrINI> 1){ strcpy(STAT[1].str,ErrSTR); ErrINI=0; ErrCNT=1; };
         if(ErrINI> 0){ ErrINI++; }
      }
      PN_FF=0;
      //-----------------------------------------------------------------------
      // перенесено на обновление экрана для подсчета FPS
      Loops_on_second++;     // увеличить счетчик циклов за секунду
      Loops_on_tick++;       // увеличить счетчик циклов за тик
      First_in_Cycle();      // первая пользовательская функция в цикле
      EveryCyc();            // функции на каждый цикл
      // функции на каждый тик
      if(MTimer(0,'h',1)==2){
         MTimer(0,'h',0);
         EveryTic();
         // это количество "пустых" циклов за тик
         // количество циклов между выполнением функции EveryTic()
         // на быстрых процессорах это значение будет увеличиваться.
         // значение так же зависит от сложности и количества процедур,
         // вызываемых в функции EveryTic().
         // на медленных процессорах это значение будет около нуля или ноль.
         // ноль так же может быть, если функция EveryTic будет выполняться
         // более тика. тогда значение Empty_Loops_on_second будет меньше 18
         Empty_Loops_on_tick=Loops_on_tick-1;
         Loops_on_tick=0;
      }
      // функции на каждую секунду
      if(MTimer(1,'h',100)==2){
         MTimer(1,'h',0);
         EverySec();
         // это значение интересно для контроля выполнения программы на
         // медленных процессорах, когда выполнение общего цикла невелико
         Empty_Loops_on_second=Loops_on_second-1;
         Loops_on_second=0;
         for(n=0;n<10;n++) NewSec[n]=1;
      }
      // последняя функция в цикле
      Last__in_Cycle();
      //-----------------------------------------------------------------------

      //-----------------------------------------------------------------------
      if(GOPK> 0){ SCode=GOPK; GOPKCODE=GOPK; GOPK=0; COPK=0; }
      //-----------------------------------------------------------------------
      // Генерируемый код клавиши после ввода строки
      if (GVCode>0){
          SCode=GVCode; GVCode=0; ECode=0; KCT=0; KAL=0; KSH=0; vkey=1;
      }
      // Проверка сгенерированных функциями кодов клавиш
      if (ECode>0){ SCode=ECode; ECode=0; }
      if (SCode>CPCTRL  && SCode<CPCTRL +120){ KCT=1; SCode=SCode-CPCTRL;  }
      if (SCode>CPALT   && SCode<CPALT  +120){ KAL=1; SCode=SCode-CPALT;   }
      if (SCode>CPSHIFT && SCode<CPSHIFT+120){ KSH=1; SCode=SCode-CPSHIFT; }
      //-----------------------------------------------------------------------

      //-----------------------------------------------------------------------
      // Переключение клавиатуры по Ctrl+SPACE
      if (KCT==1&&SCode==KEY_SPACE){
          // 0-Русский, 1-Английский
          CBL[13]=(CBL[13]==0)?1:0; SCode=0;
      }
      //-----------------------------------------------------------------------

      //-----------------------------------------------------------------------
      // если ожидаем конца ввода строки - другие обработки не делаем
      if (LGET_en==1 || GS.FL==1){
         if((SCode>0)&&(GKey==0)){ GKey=SCode; SCode=0; }
      }
      // если была нажата клавиша- сбросить флаг ожидания нажатия клавиши
      if (SCode>0){ WaitPressKey=0; }
      //-----------------------------------------------------------------------

      //-----------------------------------------------------------------------
      // при управлении системой должна быть нажата клавиша Shift
      if (KSH==1){
          // SH+A - информация об авторе
          if (SCode==KEY_A){
              AUFL=(AUFL==0)?AUFL=5:0; AUCNT=2; AUw=0; AUx=0; SCode=0;
          }
          // SH+M - включение/выключение координат мышки
          if (SCode==KEY_M){ MSCFL=(MSCFL==0)?1:0; }
          // SH+Q - выход из программы
          if (SCode==KEY_Q){ Mexit(0,0); }
          // SH+F - загрузка начального экрана программы
          if (SCode==KEY_F){
              SetFNCfg();
              strcpy(PN_fn[0],FNCfg);
              strcpy(PN_fn[1],FNCfg);
              FNCfgLD=1;
          }
          // переопределение координат вывода элементов
          if (SCode==KEY_Z || SCode==KEY_X || SCode==KEY_N || SCode==KEY_U ||
              SCode==KEY_C || SCode==KEY_B || SCode==KEY_D || SCode==KEY_V ||
              SCode==KEY_UP|| SCode==KEY_DOWN ||
              SCode==KEY_LEFT || SCode==KEY_RIGHT){
              GetXYEL(1);
          }
      }
      // если была нажата клавиша ESC или прошло более минуты ожидания
      // нажатия клавиши- закрыть окно вывода информации выделенного элемента
      if((SCode==KEY_ESC||WaitPressKey>60)&&(XYMode>0)){
          SCode= KEY_ESC; GetXYEL(1);
      }
      // изменение скорости движения мышки
      if (KSH==1&&KCT==1&&(SCode==61||SCode==62||SCode==27||SCode==36)){
          // уменьшение скорости движения мышки "Ctrl Shift -"
          if (SCode==61&XSpeed<200) XSpeed=XSpeed+10; 
          // увеличение скорости движения мышки "Ctrl Shift +"
          if (SCode==62&XSpeed>10 ) XSpeed=XSpeed-10;
          // начальная установка скорости
          if (SCode==27) XSpeed=10;
          YSpeed=XSpeed;
          if (SCode==36){ XSpeed=370; YSpeed=470; }
          // установка скорости
          set_mouse_speed(XSpeed,YSpeed);
      }  
      //-----------------------------------------------------------------------

      //-----------------------------------------------------------------------
      // проверка последней нажатой клавиши и выполнение комманд,
      // определенных для этих клавиш:   k  sc  vw  "cmd nn"
      if (SCode>0){
         KCode=SCode; AUCNT=2; AUFL=0; RKDraw=1;
         if (KCT>0){ OCode=SCode+CPCTRL;  }
         if (KAL>0){ OCode=SCode+CPALT;   }
         if (KSH>0){ OCode=SCode+CPSHIFT; }

//DPrn(0,0,"1: KCode=%d OCode=%d ECode=%d SCode=%d KCT=%d KAL=%d KSH=%d \n",
//             KCode,OCode,ECode,SCode,KCT,KAL,KSH);

         // проверить каждое описание определенных клавиш
         for(a=0,k=0; a<HKBD; a++){
            // если в описании код клавиши совпал с нажатой клавишей
            if((LKBD[a].cod==SCode)&&((vkey==1)||(LKBD[a].flg==1))){
               //--------------------------------------------------------------
               // вызов меню (если не открыто окно вывода)
               if (LKBD[a].run=='m'){
                  // если нажата не ENTER и разрешен ввод
                  if (((atoi(LKBD[a].str))!=95)&&(LockKBD==100)&&HMNU>0){
                     // восстановить экран под предыдущим пунктом меню
                     LMNU[vmnu].bf=1; vm=vmnu;
                     SRMenu(1,vmnu);
                     if (LMNU[vmnu].sf>0){
                         draw_sprite(scrsav,LKNP[ LMNU[vmnu].bn ].sp[0],
                                     LMNU[vmnu].X,LMNU[vmnu].Y);
                     }
                     // определить номер текущего(выбранного) меню
                     o=vmnu;
                     for(b=0;b<HMNU;b++){
                         vmnu=vmnu+atoi(LKBD[a].str);
                         // проверка значения выбранного пункта меню (по кругу)
                         if (vmnu>=HMNU){ vmnu=0; }
                         if (vmnu< 0   ){ vmnu=HMNU-1; }
                         if (LMNU[vmnu].vw>0){ break; }
                     }
                     if (b>HMNU){ vmnu=o; }
                     // проверка доступности выбранного пункта меню
                     if (LMNU[vmnu].k==0){ vmnu=vm; }
                     // установить курсор мышки, если он на пункте меню
                     if (MSdX1>0 && MSdY1>0){
                        position_mouse(LMNU[vmnu].X+(LMNU[vmnu].W*0.01*MSdX1),
                                       LMNU[vmnu].Y+(LMNU[vmnu].H*0.01*MSdY1));
                        LMNU[vmnu].bf=2;
                     }
                     // нарисовать пункт меню
                     SRMenu(0,vmnu);
                     // пояснение  пункта меню - в строку статуса
                     if (LMNU[vmnu].k >0&&ErrCNT==0){
                         strcpy(STAT[1].str,LMNU[vmnu].str); STAT[1].cnt=10;
                     }
                     if (LMNU[vmnu].sf>0&&LMNU[vmnu].vw>0){
                         LMNU[vmnu].bf=2;
                         draw_sprite(scrsav,LKNP[ LMNU[vmnu].bn ].sp[1],
                                     LMNU[vmnu].X,LMNU[vmnu].Y);
                     }
                     //
                     ReDraw=1;
                  }  else  {
                  // если на пункте меню нажата клавиша ENETR
                     SCode =LMNU[vmnu].k;
                     MENUSC=SCode;
                     if (Dism>0){ SCode=0; }
                     a=-1; vkey=1; akey=1; zkey=SCode;
                     // код с клавишей Ctrl
                     if (zkey>CPCTRL && zkey<CPCTRL+120){
                         KCT=1;
                         OCode=SCode; ECode=SCode;
                         SCode=SCode-CPCTRL; KCode=SCode; zkey=SCode;
                     }
                     // код с клавишей Alt
                     if (zkey>CPALT && zkey<CPALT+120){
                         KAL=1;
                         OCode=SCode; ECode=SCode;
                         SCode=SCode-CPALT; KCode=SCode; zkey=SCode;
                     }
                     // код с клавишей Shift
                     if (zkey>CPSHIFT && zkey<CPSHIFT+120){
                         KSH=1;
                         OCode=SCode; ECode=SCode;
                         SCode=SCode-CPSHIFT; zkey=SCode;
                     }
                  }; continue;
               }
            }
            //-----------------------------------------------------------------
            if((LKBD[a].cod==SCode)&&((vkey==1)||(LKBD[a].flg==1))&&
               (KAL==0&&KSH==0&&KCT==0)){
               //--------------------------------------------------------------
               // загрузка изображений по нажатию клавиши
               if (LKBD[a].run=='w'){ KBDLoadPCX(atoi(LKBD[a].str),0); }
               //--------------------------------------------------------------
               // вывод строки в строку статуса
               if (LKBD[a].run=='S'){
                  strcpy(STAT[1].str,LKBD[a].str); STAT[1].cnt=20;
               }
               //--------------------------------------------------------------
               // изменение в текщ записи БД просмотра указанного поля
               if (LKBD[a].run=='B'){
                   FVDBFLDWR(atoi(LKBD[a].str));
               }
               //--------------------------------------------------------------
               // вывод строки по заданным координатам
               if (LKBD[a].run=='s'){
                  if (LockSTR==0){
                      ta=atoi(LKBD[a].str);
                      if(ta>=0&&ta<MSTR){
                          MESOut((MOUT*)&LSTR[ta],3,ta); zkey=SCode;
                      }
                  }
               }
               //--------------------------------------------------------------
               // отмена - закрытие окон
               if (LKBD[a].run== 'q'){
                  Animation(3);
                  if (LockKBD !=100){ KBDLoadPCX(a,1); zkey=0; };
               }
               //--------------------------------------------------------------
               // выполнение внешних программ
               if (LKBD[a].run=='r'){
                  if (HRUN>0){ RunPRG(atoi(LKBD[a].str)); }
               }
               //--------------------------------------------------------------
               // загрузка ini файлов (если не открыто окно вывода)
               if (LKBD[a].run=='i'){
                  if (HINI>0){
//                   if((strcmp(PN_fn[1], SINI[(atoi(LKBD[a].str))].fn))!=0){
                         strcpy(FNCfg   , SINI[(atoi(LKBD[a].str))].fn);
                         strcpy(PN_fn[0], PN_fn[1]);
                         strcpy(PN_fn[1], FNCfg   );
                         FNCfgLD=1;
//                   }
                     break;
                  }
               }
               //--------------------------------------------------------------
               // возврат назад (на предыдущий экран)
               if (LKBD[a].run=='p'){
                     PCode=SCode; strcpy(FNCfg,PN_FC);
//                   strcpy(FNCfg,   PN_fn[0]);
                     strcpy(PN_fn[0],PN_fn[1]);
                     strcpy(PN_fn[1],FNCfg   );
                     FNCfgLD=1;
               }
               //--------------------------------------------------------------
               // Ввод строк данных
               if (LKBD[a].run=='G'){
                  if (HGET>0){ GetVal(atoi(LKBD[a].str),0); };
               }
               //--------------------------------------------------------------
               // Ввод рабочих строк
               if (LKBD[a].run=='W'){
                  if (HSTT>0){ GetWStr(atoi(LKBD[a].str),0); break; };
               }
               //--------------------------------------------------------------
               // Изменение значений каналов
               if (LKBD[a].run=='c'){
                  if (HXCV>0){ XCVal(atoi(LKBD[a].str)); };
               }
               //--------------------------------------------------------------
               // Вывод анимации
               if (LKBD[a].run=='a'){
                   if (HANI>0){
                      b=atoi(LKBD[a].str);
                      f=(_i)Arr_to_Val(LANI[b].na, LANI[b].nf);
                      if (LANI[b].vw<MANI || f>=300000){
                          if (f<300000){
                              LANI[b].vx=LANI[b].vw;
                          }
                          if (akey ==1){ LANI[b].vw=b+MANI*3; akey=0; }
                          else         { LANI[b].vw=a+MANI*2; }
                          Val_to_Arr(LANI[b].na, LANI[b].nf, 1, CBMODNO,'s');
                      }  else  {
                          if (LANI[b].vw==a+MANI*2 && f<300000 ){
                              LANI[b].vw =LANI[b].vx;
                              Val_to_Arr(LANI[b].na, LANI[b].nf, 0, CBMODNO,'s');
                          }
                      }
                   };
               }
               //--------------------------------------------------------------
               // Запись(печать) текущего экрана по нажатию клавиши
               if (LKBD[a].run=='Z'){ SaveScreen(atoi(LKBD[a].str)); }
               //--------------------------------------------------------------
            }
            //-----------------------------------------------------------------

            //-----------------------------------------------------------------
            if (KSH==1){
               // SH+Q - выход из программы
               if (KCode==KEY_Q){ Mexit(0,0); }
            }
            //-----------------------------------------------------------------

            //-----------------------------------------------------------------
            // проверка кодов, если используются расширенные клавиши
            if(LKBD[a].cod==OCode){
                //  запись экрана
                if (LKBD[a].run=='Z'){ SaveScreen(atoi(LKBD[a].str));   }
                //  загрузка окна
                if (LKBD[a].run=='w'){
                    KBDLoadPCX(atoi(LKBD[a].str),0);
                }
                //  ввод строки
                if (LKBD[a].run=='W'){
                  if (HSTT>0){ GetWStr(atoi(LKBD[a].str),0); break; };
                }
                //  запись строки в файл сообщений
                if (LKBD[a].run=='e'){ AddOpMess(atoi(LKBD[a].str));    }
                // Ввод строк данных
                if (LKBD[a].run=='G'){
                    if (HGET>0){ GetVal(atoi(LKBD[a].str),0); }
                }
                // Изменение значений каналов
                if (LKBD[a].run=='c'){
                   if (HXCV>0){ XCVal(atoi(LKBD[a].str)); }
                }
                // отмена - закрытие окон
                if (LKBD[a].run== 'q'){
                   Animation(3); if(LockKBD !=100){ KBDLoadPCX(a,1); zkey=0; }
                }
                // выполнение внешних программ
                if (LKBD[a].run=='r'){
                   if (HRUN>0){ RunPRG(atoi(LKBD[a].str)); }
                }
                // загрузка ini файлов (если не открыто окно вывода)
                if (LKBD[a].run=='i'){
                   if ((HINI>0)/*&&(LockKBD==100)*/){
                      if((strcmp(PN_fn[1], SINI[(atoi(LKBD[a].str))].fn))!=0){
                          strcpy(FNCfg   , SINI[(atoi(LKBD[a].str))].fn);
                          strcpy(PN_fn[0], PN_fn[1]);
                          strcpy(PN_fn[1], FNCfg   );
                          FNCfgLD=1;
                      }
                   }
                }
            }
            //-----------------------------------------------------------------


            //-----------------------------------------------------------------
            // при управлении трендами должна быть нажата клавиша CTRL
            if (KCT==1){
               // CTRL+TAB - переключение между трендами загруженного экрана
               if (KCode==KEY_TAB||KCode==KEY_N){ TRD_Set(1); break; }
               // CTRL+'+',CTRL+'-',CTRL+'*' -изменение кол-ва секунд в тренде
               if (KCode==KEY_PLUS_PAD||KCode==KEY_MINUS_PAD||
                   KCode==KEY_MINUS   ||KCode==KEY_EQUALS   ||
                   KCode==KEY_ASTERISK ){ TRD_Set(2); break; }
               // CTRL+A - изменение режима активного графика [С]-[A]
               if (KCode==KEY_A){ TRD_Set(3); break; }
               // CTRL+<-, CTRL+->, CTRL+Home, CTRL+End -
               // передвижение временного среза
               if (KCode==KEY_LEFT||KCode==KEY_RIGHT||
                   KCode==KEY_HOME||KCode==KEY_END){ TRD_Set(4); break; }
               // CTRL+0...CTRL+1 - включение|выключение выбранного графика
               // и значения временного среза на тренде
               if (KCode>=KEY_1&&KCode<=KEY_9){ TRD_Set(5); break; }
               // CTRL+INS, CTRL+DEL - изменение шага временного среза
               if (KCode==KEY_INSERT||KCode==KEY_DEL||
                   KCode==KEY_0_PAD ||KCode==KEY_DEL_PAD){ TRD_Set(6); break; }
               // CTRL+PgDn, CTRL+PgUp - листание графика в режиме архива
               // на количество секунд(записей) в графике
               if (KCode==KEY_PGDN||KCode==KEY_PGUP){ TRD_Set(7); break; }
               // CTRL+D - ввод даты файла архива для просмотра графика
               if (KCode==KEY_D){ TRD_Set(8); break; }
               // CTRL+T - ввод времени архива
               if (KCode==KEY_T){ TRD_Set(9); break; }
               // CTRL+>, CTRL+< - увеличение,уменьшение диапазона
               // тренда на 10%
               if (KCode==KEY_COMMA||KCode==KEY_STOP){ TRD_Set(10); break; }
               // CTRL+вниз, CTRL+вверх - сдвиг по шкале тренда на 10%
               if (KCode==KEY_UP||KCode==KEY_DOWN){ TRD_Set(10); break; }
               // CTRL+/ - установка начального диапазона тренда
               if (KCode==KEY_SLASH){ TRD_Set(10); break; }
               // CTRL+M - переключение режима сдвига
               if (KCode==KEY_M){ TRD_Set(10); break; }
               // CTRL+\ - установка начального времени отображения тренда
               if (KCode==KEY_BACKSLASH){ TRD_Set(10); break; }
               // CTRL+ENTER - режим передвижения временного среза
               if (KCode==KEY_ENTER){ TRD_Set(11); break; }
            }
            //-----------------------------------------------------------------

            //-----------------------------------------------------------------
            // при управлении просмотром файлов и ползунков д.б.
            // нажата клавиша ALT
            if (KAL==1){
               //--------------------------------------------------------------
               // ALT+N - переключ между окнами просмотра загруж экрана
               if (KCode==KEY_N){ FV_Set(1); break; }
               // ALT+D - ввод даты для просмотра файла отчета
               if (KCode==KEY_D){ FV_Set(2); break; }
               // ALT+G - поиск введенной строки
               if (KCode==KEY_G){ FV_Set(4); break; }
               // ALT+T - обратный поиск введенной строки
               if (KCode==KEY_T){ FV_Set(5); break; }
               // ALT+P - печать текущей страницы активного файла
               if (KCode==KEY_P){ PrintFile(1); break; }
               // ALT+O - печать текущего экрана
               if (KCode==KEY_O){ PrintScrn(1); break; }
               // ALT+S - вкл-выкл флага фильтра по флагу сообщения
               if (KCode==KEY_S){ FSON=(FSON>0)?0:1; break; }
               // ALT+E - вкл-выкл флага фильтра по вводу строки поиска
               if (KCode==KEY_E){ FEON=(FEON>0)?0:1; break; }
               //--------------------------------------------------------------
               // ALT+> - следующий ползунок
               if (KCode== KEY_STOP ){ Polzun(3,1); break; }
               // ALT+< - предыдущий ползунок
               if (KCode== KEY_COMMA){ Polzun(3,2); break; }
               // ALT+стрелка вверх  || -> -увеличить значение ползунка на 1%
               if (KCode== KEY_8_PAD || KCode== KEY_6_PAD){ Polzun(3,3); break; }
               // ALT+стрелка вниз   || <- -уменьшить значение ползунка на 1%
               if (KCode== KEY_2_PAD || KCode== KEY_4_PAD){ Polzun(3,4); break; }
               // ALT++ - увеличить значение ползунка на 10%
               if (KCode== KEY_PLUS_PAD ){ Polzun(3,5); break; }
               // ALT+- - уменьшить значение ползунка на 10%
               if (KCode== KEY_MINUS_PAD){ Polzun(3,6); break; }
               //--------------------------------------------------------------
               // Alt+TAB-свернуть экран на панель
               if (KCode==KEY_TAB){ WndCmd (1); break; }
               //--------------------------------------------------------------
            }
            //-----------------------------------------------------------------
         }  // end for
         //--------------------------------------------------------------------
         //
         LockSTR=(LockKBD==100) ? 0 : 1;
         // передача код последней нажатой клавиши для функций
         LLKey=SCode; TLKey=SCode; FVCode=SCode;
         // обнуление кода последней нажатой клавиши
         SCode=0; KCode=0; ECode=0; OCode=0; KSH=0; KCT=0; KAL=0; vkey=0;
     }
   }
   //--------------------------------------------------------------------------
   Mexit(0,1);
   return (0);
}
//-----------------------------------------------------------------------------
