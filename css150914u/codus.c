// coding: cp866
//---------------------------------------------------------------------------
// �᭮���� �ணࠬ��
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
   // ��室 �� ����᪮�� ०��� � ��⠭���� ⥪�⮢���
   DPrn( 0,0,"%s\n",SMSG[20]);                      // "D: Begin function Mexit. Release memory graphics occupied."
   DPrn(20,0,"%s\n",SMSG[20]);                      // "D: Begin function Mexit. Release memory graphics occupied."
   Ust_Graph();
   set_gfx_mode(GFX_TEXT, 80, 25, 0, 0);
   CBL[14]=0;
   DPrn( 0,0,"%s\n",SMSG[22]);                      // "D: Write flag end programm."
   // ��室 �� �⥢�� �ࠩ��஢
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
   // �����⨥ ��� ������
   if(ArcFL>0){ dbClose(ARCOut); }
   if(RepFL>0){ dbClose(REPOut); }
   if(UpdFL>0){ dbClose(UPDOut); }
   // ������ ��᫥���� ������
   ReadSave(1);
   ReadIDX (1,0);
   DPrn( 0,0,"%s\n",SMSG[23]);                      // "D: Stop network interfaces."
   // 㤠����� 䠩�� ᥬ���
   if(access(FNFlg,0)==0){ unlink(FNFlg); }
   sprintf(SMSG[MMSG],SMSG[29],MDate, MTime);       // "D: Time exit from programm CODUS: %s %s"
   DPrn( 0,0,"%s\n\n",SMSG[MMSG]);
   // �����⨥ 䠩�� ����� �⫠��筮� ���ଠ樨
   if(DebOut!=NULL){ fclose(DebOut); }
   // ��室
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
    // �� ����᪠�� ����� ����� �ணࠬ��
    vvkEVENT=OpenEvent(EVENT_ALL_ACCESS,FALSE,"CODUS_EVENT");
    if(vvkEVENT!=NULL){ exit(0); }
    vvkEVENT=CreateEvent(0,0,0,"CODUS_EVENT");
    DPrn(0,0,"Create event CODUS_EVENT.\n");
    // ������ 䠩� ᮮ�饭�� �ணࠬ��
    if(access(FNMSG,4)==0){ Read_FNMSG(); }
    GetTime(0);
    // ������ 䠩� �⫠��� ��� �����
    DebOut=fopen(FNDeb,"a+");
    sprintf(SMSG[MMSG],SMSG[2],MDate, MTime);           // "D: Time start programm CODUS: %s %s"
    DPrn(0,0,"\n%s\n",SMSG[MMSG]);
    sprintf(SMSG[MMSG],SMSG[3],codus_version, OSVER);   // "D: Version %.4f %s"
    DPrn(0,0,"%s\n",SMSG[MMSG]);
    HDFlg=open(FNFlg,O_BINARY|O_RDWR|O_CREAT,S_IREAD|S_IWRITE); close(HDFlg);
    DPrn(0,0,"Create flag file.\n");
#ifdef WINDOW
    // ��।������ ����� � ᮧ����� ���� �ணࠬ��
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
    // ���樠������ ������⥪� Allegro
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
    // ��⠭����� �⮡ࠦ���� ���᪨� ᨬ�����
    set_uformat(U_ASCII);    
    three_finger_flag=FALSE;
    //-------------------------------------------------------------------------
    // ���樠������ ���ᨢ�� ��� ������
    dbClearALL(); dbBFClearALL();
    DPrn(0,0,"%s\n",SMSG[0]);
    DPrn(0,0,"Clear database array.\n");
    //-------------------------------------------------------------------------
    // ��।������ ����� ��砫쭮�� 䠩�� ���䨣��樨
    DPrn(0,0,"%s\n",SMSG[5]);                           
    SetFNCfg();
    // �⥭�� ��砫쭮�� 䠩�� ���䨣��樨
    strcpy(FirstINI,"");
    DPrn(0,0,"Read config file.\n");
    Read_Cfg();
    //-------------------------------------------------------------------------
    // �஢�ઠ ����஫��� �㬬 ��⥬��� 䠩��� ������
    DPrn(0,0,"Check system files.\n");
    if((CRCheck())>0){ DPrn(0,0,"%s\n",SMSG[6]); Mexit(1,1); }
    // �⥭�� 䠩�� ����஢����� ��ப
    Read_ESt();
    // �⥭�� 䠩�� ���� �������
    DPrn(0,0,"Read database channel file.\n");
    Read_CHB();
    // �⥭�� 䠩�� ᮮ�饭��
    DPrn(0,0,"Read database alarm file.\n");
    Read_Alarm();
    // ����⠭������� ��࠭����� ��⠭����
    DPrn(0,0,"Read save data file.\n");
    ReadSave(0);
    if (CBL[14]==1){ ExitBAD=1; }; CBL[14]=1;
    ReadSave(1);
    // �⥭�� 䠩�� ��᫥���� ����� ��������� �������
    if(access(FNDLS,6)==0){
       if((fh=open(FNDLS,O_RDWR|O_BINARY))>=0){
          read (fh,&upd_ls,sizeof(upd_ls));
          close(fh);
    }  }
    // �⥭�� ����������� 䠩�� � ���� �맮�
    DPrn(0,0,"Read command file.\n");
    Read_CMD();
    RunCMD();
    DPrn(0,0,"%s\n",SMSG[0]);               // "D: Ok!"
    //-------------------------------------------------------------------------
    // �஢�ઠ ᢮������ ����� ��� ����᪠ �ணࠬ��
    DPrn(13,0,"%s\n",SMSG[7]);              // "D: Check free memory..."
    SumMem();
    DPrn(13,0,"%s\n",SMSG[0]);              // "D: Ok!"
    //-------------------------------------------------------------------------
    // ��⠭���� ����᪮�� ०���
    DPrn(13,0,"%s\n",SMSG[8]);              // "D: Set graphics mode..."
    GRInitF=1; Set_Graph();
    DPrn(13,0,"%s\n",SMSG[0]);              // "D: Ok!"
    set_display_switch_mode(SWITCH_BACKAMNESIA);    // run programm in background
    //-------------------------------------------------------------------------
    // ��⠭����� �㭪樨 ⠩��஢
    DPrn(0,0,"Init and start timers.\n");
    for(a=0;a<10;a++){ LOCK_VARIABLE(HTimer[a]); LOCK_VARIABLE(mTimer[a]); }
    LOCK_FUNCTION(tt_msec);
    LOCK_FUNCTION(tt_tick);
    install_int(tt_msec, 1);
    install_int(tt_tick,10);
    // 'h'-⠩��� ⨪��, 'm'-⠩��� �����ᥪ㭤
    //  p1-����� ⠩���, p2-⨯ ⠩���('h'|'m'), p3-������⢮ ���(0-���㫥���)
    MTimer(0,'h',0);                        // ⠩��� ⨪��
    MTimer(1,'h',0);                        // ⠩��� ᥪ㭤
    MTimer(2,'h',0);                        // ⠩��� ��ࠡ�⪨ ᮮ�饭��
    MTimer(3,'h',0);                        // ⠩��� �뢮�� ��ப� �����
    MTimer(4,'h',0);                        // ⠩��� �뢮�� ��ப� ᮮ�饭��
    MTimer(5,'h',0);                        // ⠩��� ��ࠡ�⪨ ��६�����
    MTimer(6,'h',0);                        // ⠩��� �뢮�� ��ப � �����
    MTimer(7,'h',0);                        // ⠩��� ⠩���⮢ COM ����
    MTimer(8,'h',0);                        //
    MTimer(9,'h',0);                        //
    DPrn(13,0,"%s\n",SMSG[0]);
    //-------------------------------------------------------------------------
    // ��⠭���� ��砫��� ���祭��

    // ��⠭���� ᪮��� �������� ��誨 (��-㬮�砭��=2)
    XSpeed=10; YSpeed=10; set_mouse_speed(XSpeed,YSpeed);

    // ��।������ ��砫쭮�� 䠩�� ���䨣��樨 (����㧪�)
    strcpy(FNCfg,FNini);                    // ����� ���⮢� .ini 䠩�
    strcpy(PN_fn[0],FNini);                 //
    strcpy(PN_fn[1],FNini);                 //
    strcpy(PN_CC,FNCfg);                    //
    FNCfgLD=1;                              //
    memset(GS.ES,0,SLEN);                   // ���⪠ �६����� ��ப� �����
    DPrn(DEBLDI,0,"%s\n",SMSG[14]);         // ""
    //-------------------------------------------------------------------------
    CBCMD=1;             

    //-------------------------------------------------------------------------
    // �������� � 䮭� ������ ������ � ����ᮢ��
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
      // �᫨ �� ���� �맮�
      if ( CFirst == 0 ){
           GetTime(1);       // ������� ⥪�饥 �६� � ����
           FARName();        // ��।����� ����� 䠩��� ��娢�,���� �ॢ��
           FAROpen();        // ������ �� 䠩��
           FUPOpen();        //
           CFirst =1;        // ����� 䫠� ��ࢮ�� �맮��
           CBNUMMSGP[MAXMCH]=1; // ������� �� ᮮ�饭�� � 䠩�
           rectfill(screen,0,0,XMAX,YMAX,0);  // ��⪠ �࠭� ��᫥ ���⠢��
           continue;
      }
      // ��⠥� 䠩� ���䨣��樨
      if ( FNCfgLD==1  ){
           DPrn(DEBLDI,0,"\n%s\n",SMSG[15]);        // "D: Begin configuration read."
           if( (Read_Cfg()) == 2 ) { continue; }
           DPrn(DEBLDI,0,"%s\n\n",SMSG[16]);        // "D: End configuration read."
      }
      // �᫨ �뫮 ᮮ�饭�� �� �訡�� ��� ��㣮� �����।���-
      // �뢥�� ��� � ��ப� �����
      if(ErrINI>0){
         if(ErrINI> 1){ strcpy(STAT[1].str,ErrSTR); ErrINI=0; ErrCNT=1; };
         if(ErrINI> 0){ ErrINI++; }
      }
      PN_FF=0;
      //-----------------------------------------------------------------------
      // ��७�ᥭ� �� ���������� �࠭� ��� ������ FPS
      Loops_on_second++;     // 㢥����� ���稪 横��� �� ᥪ㭤�
      Loops_on_tick++;       // 㢥����� ���稪 横��� �� ⨪
      First_in_Cycle();      // ��ࢠ� ���짮��⥫�᪠� �㭪�� � 横��
      EveryCyc();            // �㭪樨 �� ����� 横�
      // �㭪樨 �� ����� ⨪
      if(MTimer(0,'h',1)==2){
         MTimer(0,'h',0);
         EveryTic();
         // �� ������⢮ "������" 横��� �� ⨪
         // ������⢮ 横��� ����� �믮������� �㭪樨 EveryTic()
         // �� ������� ������� �� ���祭�� �㤥� 㢥��稢�����.
         // ���祭�� ⠪ �� ������ �� ᫮����� � ������⢠ ��楤��,
         // ��뢠���� � �㭪樨 EveryTic().
         // �� ��������� ������� �� ���祭�� �㤥� ����� ��� ��� ����.
         // ���� ⠪ �� ����� ����, �᫨ �㭪�� EveryTic �㤥� �믮�������
         // ����� ⨪�. ⮣�� ���祭�� Empty_Loops_on_second �㤥� ����� 18
         Empty_Loops_on_tick=Loops_on_tick-1;
         Loops_on_tick=0;
      }
      // �㭪樨 �� ������ ᥪ㭤�
      if(MTimer(1,'h',100)==2){
         MTimer(1,'h',0);
         EverySec();
         // �� ���祭�� ����᭮ ��� ����஫� �믮������ �ணࠬ�� ��
         // ��������� �������, ����� �믮������ ��饣� 横�� ��������
         Empty_Loops_on_second=Loops_on_second-1;
         Loops_on_second=0;
         for(n=0;n<10;n++) NewSec[n]=1;
      }
      // ��᫥���� �㭪�� � 横��
      Last__in_Cycle();
      //-----------------------------------------------------------------------

      //-----------------------------------------------------------------------
      if(GOPK> 0){ SCode=GOPK; GOPKCODE=GOPK; GOPK=0; COPK=0; }
      //-----------------------------------------------------------------------
      // ������㥬� ��� ������ ��᫥ ����� ��ப�
      if (GVCode>0){
          SCode=GVCode; GVCode=0; ECode=0; KCT=0; KAL=0; KSH=0; vkey=1;
      }
      // �஢�ઠ ᣥ���஢����� �㭪�ﬨ ����� ������
      if (ECode>0){ SCode=ECode; ECode=0; }
      if (SCode>CPCTRL  && SCode<CPCTRL +120){ KCT=1; SCode=SCode-CPCTRL;  }
      if (SCode>CPALT   && SCode<CPALT  +120){ KAL=1; SCode=SCode-CPALT;   }
      if (SCode>CPSHIFT && SCode<CPSHIFT+120){ KSH=1; SCode=SCode-CPSHIFT; }
      //-----------------------------------------------------------------------

      //-----------------------------------------------------------------------
      // ��४��祭�� ���������� �� Ctrl+SPACE
      if (KCT==1&&SCode==KEY_SPACE){
          // 0-���᪨�, 1-������᪨�
          CBL[13]=(CBL[13]==0)?1:0; SCode=0;
      }
      //-----------------------------------------------------------------------

      //-----------------------------------------------------------------------
      // �᫨ ������� ���� ����� ��ப� - ��㣨� ��ࠡ�⪨ �� ������
      if (LGET_en==1 || GS.FL==1){
         if((SCode>0)&&(GKey==0)){ GKey=SCode; SCode=0; }
      }
      // �᫨ �뫠 ����� ������- ����� 䫠� �������� ������ ������
      if (SCode>0){ WaitPressKey=0; }
      //-----------------------------------------------------------------------

      //-----------------------------------------------------------------------
      // �� �ࠢ����� ��⥬�� ������ ���� ����� ������ Shift
      if (KSH==1){
          // SH+A - ���ଠ�� �� ����
          if (SCode==KEY_A){
              AUFL=(AUFL==0)?AUFL=5:0; AUCNT=2; AUw=0; AUx=0; SCode=0;
          }
          // SH+M - ����祭��/�몫�祭�� ���न��� ��誨
          if (SCode==KEY_M){ MSCFL=(MSCFL==0)?1:0; }
          // SH+Q - ��室 �� �ணࠬ��
          if (SCode==KEY_Q){ Mexit(0,0); }
          // SH+F - ����㧪� ��砫쭮�� �࠭� �ணࠬ��
          if (SCode==KEY_F){
              SetFNCfg();
              strcpy(PN_fn[0],FNCfg);
              strcpy(PN_fn[1],FNCfg);
              FNCfgLD=1;
          }
          // ��८�।������ ���न��� �뢮�� ����⮢
          if (SCode==KEY_Z || SCode==KEY_X || SCode==KEY_N || SCode==KEY_U ||
              SCode==KEY_C || SCode==KEY_B || SCode==KEY_D || SCode==KEY_V ||
              SCode==KEY_UP|| SCode==KEY_DOWN ||
              SCode==KEY_LEFT || SCode==KEY_RIGHT){
              GetXYEL(1);
          }
      }
      // �᫨ �뫠 ����� ������ ESC ��� ��諮 ����� ������ ��������
      // ������ ������- ������� ���� �뢮�� ���ଠ樨 �뤥������� �����
      if((SCode==KEY_ESC||WaitPressKey>60)&&(XYMode>0)){
          SCode= KEY_ESC; GetXYEL(1);
      }
      // ��������� ᪮��� �������� ��誨
      if (KSH==1&&KCT==1&&(SCode==61||SCode==62||SCode==27||SCode==36)){
          // 㬥��襭�� ᪮��� �������� ��誨 "Ctrl Shift -"
          if (SCode==61&XSpeed<200) XSpeed=XSpeed+10; 
          // 㢥��祭�� ᪮��� �������� ��誨 "Ctrl Shift +"
          if (SCode==62&XSpeed>10 ) XSpeed=XSpeed-10;
          // ��砫쭠� ��⠭���� ᪮���
          if (SCode==27) XSpeed=10;
          YSpeed=XSpeed;
          if (SCode==36){ XSpeed=370; YSpeed=470; }
          // ��⠭���� ᪮���
          set_mouse_speed(XSpeed,YSpeed);
      }  
      //-----------------------------------------------------------------------

      //-----------------------------------------------------------------------
      // �஢�ઠ ��᫥���� ����⮩ ������ � �믮������ �������,
      // ��।������� ��� ��� ������:   k  sc  vw  "cmd nn"
      if (SCode>0){
         KCode=SCode; AUCNT=2; AUFL=0; RKDraw=1;
         if (KCT>0){ OCode=SCode+CPCTRL;  }
         if (KAL>0){ OCode=SCode+CPALT;   }
         if (KSH>0){ OCode=SCode+CPSHIFT; }

//DPrn(0,0,"1: KCode=%d OCode=%d ECode=%d SCode=%d KCT=%d KAL=%d KSH=%d \n",
//             KCode,OCode,ECode,SCode,KCT,KAL,KSH);

         // �஢���� ������ ���ᠭ�� ��।������� ������
         for(a=0,k=0; a<HKBD; a++){
            // �᫨ � ���ᠭ�� ��� ������ ᮢ��� � ����⮩ �����襩
            if((LKBD[a].cod==SCode)&&((vkey==1)||(LKBD[a].flg==1))){
               //--------------------------------------------------------------
               // �맮� ���� (�᫨ �� ����� ���� �뢮��)
               if (LKBD[a].run=='m'){
                  // �᫨ ����� �� ENTER � ࠧ�襭 ����
                  if (((atoi(LKBD[a].str))!=95)&&(LockKBD==100)&&HMNU>0){
                     // ����⠭����� �࠭ ��� �।��騬 �㭪⮬ ����
                     LMNU[vmnu].bf=1; vm=vmnu;
                     SRMenu(1,vmnu);
                     if (LMNU[vmnu].sf>0){
                         draw_sprite(scrsav,LKNP[ LMNU[vmnu].bn ].sp[0],
                                     LMNU[vmnu].X,LMNU[vmnu].Y);
                     }
                     // ��।����� ����� ⥪�饣�(��࠭����) ����
                     o=vmnu;
                     for(b=0;b<HMNU;b++){
                         vmnu=vmnu+atoi(LKBD[a].str);
                         // �஢�ઠ ���祭�� ��࠭���� �㭪� ���� (�� ����)
                         if (vmnu>=HMNU){ vmnu=0; }
                         if (vmnu< 0   ){ vmnu=HMNU-1; }
                         if (LMNU[vmnu].vw>0){ break; }
                     }
                     if (b>HMNU){ vmnu=o; }
                     // �஢�ઠ ����㯭��� ��࠭���� �㭪� ����
                     if (LMNU[vmnu].k==0){ vmnu=vm; }
                     // ��⠭����� ����� ��誨, �᫨ �� �� �㭪� ����
                     if (MSdX1>0 && MSdY1>0){
                        position_mouse(LMNU[vmnu].X+(LMNU[vmnu].W*0.01*MSdX1),
                                       LMNU[vmnu].Y+(LMNU[vmnu].H*0.01*MSdY1));
                        LMNU[vmnu].bf=2;
                     }
                     // ���ᮢ��� �㭪� ����
                     SRMenu(0,vmnu);
                     // ���᭥���  �㭪� ���� - � ��ப� �����
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
                  // �᫨ �� �㭪� ���� ����� ������ ENETR
                     SCode =LMNU[vmnu].k;
                     MENUSC=SCode;
                     if (Dism>0){ SCode=0; }
                     a=-1; vkey=1; akey=1; zkey=SCode;
                     // ��� � �����襩 Ctrl
                     if (zkey>CPCTRL && zkey<CPCTRL+120){
                         KCT=1;
                         OCode=SCode; ECode=SCode;
                         SCode=SCode-CPCTRL; KCode=SCode; zkey=SCode;
                     }
                     // ��� � �����襩 Alt
                     if (zkey>CPALT && zkey<CPALT+120){
                         KAL=1;
                         OCode=SCode; ECode=SCode;
                         SCode=SCode-CPALT; KCode=SCode; zkey=SCode;
                     }
                     // ��� � �����襩 Shift
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
               // ����㧪� ����ࠦ���� �� ������ ������
               if (LKBD[a].run=='w'){ KBDLoadPCX(atoi(LKBD[a].str),0); }
               //--------------------------------------------------------------
               // �뢮� ��ப� � ��ப� �����
               if (LKBD[a].run=='S'){
                  strcpy(STAT[1].str,LKBD[a].str); STAT[1].cnt=20;
               }
               //--------------------------------------------------------------
               // ��������� � ⥪� ����� �� ��ᬮ�� 㪠������� ����
               if (LKBD[a].run=='B'){
                   FVDBFLDWR(atoi(LKBD[a].str));
               }
               //--------------------------------------------------------------
               // �뢮� ��ப� �� ������� ���न��⠬
               if (LKBD[a].run=='s'){
                  if (LockSTR==0){
                      ta=atoi(LKBD[a].str);
                      if(ta>=0&&ta<MSTR){
                          MESOut((MOUT*)&LSTR[ta],3,ta); zkey=SCode;
                      }
                  }
               }
               //--------------------------------------------------------------
               // �⬥�� - �����⨥ ����
               if (LKBD[a].run== 'q'){
                  Animation(3);
                  if (LockKBD !=100){ KBDLoadPCX(a,1); zkey=0; };
               }
               //--------------------------------------------------------------
               // �믮������ ���譨� �ணࠬ�
               if (LKBD[a].run=='r'){
                  if (HRUN>0){ RunPRG(atoi(LKBD[a].str)); }
               }
               //--------------------------------------------------------------
               // ����㧪� ini 䠩��� (�᫨ �� ����� ���� �뢮��)
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
               // ������ ����� (�� �।��騩 �࠭)
               if (LKBD[a].run=='p'){
                     PCode=SCode; strcpy(FNCfg,PN_FC);
//                   strcpy(FNCfg,   PN_fn[0]);
                     strcpy(PN_fn[0],PN_fn[1]);
                     strcpy(PN_fn[1],FNCfg   );
                     FNCfgLD=1;
               }
               //--------------------------------------------------------------
               // ���� ��ப ������
               if (LKBD[a].run=='G'){
                  if (HGET>0){ GetVal(atoi(LKBD[a].str),0); };
               }
               //--------------------------------------------------------------
               // ���� ࠡ��� ��ப
               if (LKBD[a].run=='W'){
                  if (HSTT>0){ GetWStr(atoi(LKBD[a].str),0); break; };
               }
               //--------------------------------------------------------------
               // ��������� ���祭�� �������
               if (LKBD[a].run=='c'){
                  if (HXCV>0){ XCVal(atoi(LKBD[a].str)); };
               }
               //--------------------------------------------------------------
               // �뢮� �����樨
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
               // ������(�����) ⥪�饣� �࠭� �� ������ ������
               if (LKBD[a].run=='Z'){ SaveScreen(atoi(LKBD[a].str)); }
               //--------------------------------------------------------------
            }
            //-----------------------------------------------------------------

            //-----------------------------------------------------------------
            if (KSH==1){
               // SH+Q - ��室 �� �ணࠬ��
               if (KCode==KEY_Q){ Mexit(0,0); }
            }
            //-----------------------------------------------------------------

            //-----------------------------------------------------------------
            // �஢�ઠ �����, �᫨ �ᯮ������� ���७�� ������
            if(LKBD[a].cod==OCode){
                //  ������ �࠭�
                if (LKBD[a].run=='Z'){ SaveScreen(atoi(LKBD[a].str));   }
                //  ����㧪� ����
                if (LKBD[a].run=='w'){
                    KBDLoadPCX(atoi(LKBD[a].str),0);
                }
                //  ���� ��ப�
                if (LKBD[a].run=='W'){
                  if (HSTT>0){ GetWStr(atoi(LKBD[a].str),0); break; };
                }
                //  ������ ��ப� � 䠩� ᮮ�饭��
                if (LKBD[a].run=='e'){ AddOpMess(atoi(LKBD[a].str));    }
                // ���� ��ப ������
                if (LKBD[a].run=='G'){
                    if (HGET>0){ GetVal(atoi(LKBD[a].str),0); }
                }
                // ��������� ���祭�� �������
                if (LKBD[a].run=='c'){
                   if (HXCV>0){ XCVal(atoi(LKBD[a].str)); }
                }
                // �⬥�� - �����⨥ ����
                if (LKBD[a].run== 'q'){
                   Animation(3); if(LockKBD !=100){ KBDLoadPCX(a,1); zkey=0; }
                }
                // �믮������ ���譨� �ணࠬ�
                if (LKBD[a].run=='r'){
                   if (HRUN>0){ RunPRG(atoi(LKBD[a].str)); }
                }
                // ����㧪� ini 䠩��� (�᫨ �� ����� ���� �뢮��)
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
            // �� �ࠢ����� �७���� ������ ���� ����� ������ CTRL
            if (KCT==1){
               // CTRL+TAB - ��४��祭�� ����� �७���� ����㦥����� �࠭�
               if (KCode==KEY_TAB||KCode==KEY_N){ TRD_Set(1); break; }
               // CTRL+'+',CTRL+'-',CTRL+'*' -��������� ���-�� ᥪ㭤 � �७��
               if (KCode==KEY_PLUS_PAD||KCode==KEY_MINUS_PAD||
                   KCode==KEY_MINUS   ||KCode==KEY_EQUALS   ||
                   KCode==KEY_ASTERISK ){ TRD_Set(2); break; }
               // CTRL+A - ��������� ०��� ��⨢���� ��䨪� [�]-[A]
               if (KCode==KEY_A){ TRD_Set(3); break; }
               // CTRL+<-, CTRL+->, CTRL+Home, CTRL+End -
               // ��।������� �६������ �१�
               if (KCode==KEY_LEFT||KCode==KEY_RIGHT||
                   KCode==KEY_HOME||KCode==KEY_END){ TRD_Set(4); break; }
               // CTRL+0...CTRL+1 - ����祭��|�몫�祭�� ��࠭���� ��䨪�
               // � ���祭�� �६������ �१� �� �७��
               if (KCode>=KEY_1&&KCode<=KEY_9){ TRD_Set(5); break; }
               // CTRL+INS, CTRL+DEL - ��������� 蠣� �६������ �१�
               if (KCode==KEY_INSERT||KCode==KEY_DEL||
                   KCode==KEY_0_PAD ||KCode==KEY_DEL_PAD){ TRD_Set(6); break; }
               // CTRL+PgDn, CTRL+PgUp - ���⠭�� ��䨪� � ०��� ��娢�
               // �� ������⢮ ᥪ㭤(����ᥩ) � ��䨪�
               if (KCode==KEY_PGDN||KCode==KEY_PGUP){ TRD_Set(7); break; }
               // CTRL+D - ���� ���� 䠩�� ��娢� ��� ��ᬮ�� ��䨪�
               if (KCode==KEY_D){ TRD_Set(8); break; }
               // CTRL+T - ���� �६��� ��娢�
               if (KCode==KEY_T){ TRD_Set(9); break; }
               // CTRL+>, CTRL+< - 㢥��祭��,㬥��襭�� ���������
               // �७�� �� 10%
               if (KCode==KEY_COMMA||KCode==KEY_STOP){ TRD_Set(10); break; }
               // CTRL+����, CTRL+����� - ᤢ�� �� 誠�� �७�� �� 10%
               if (KCode==KEY_UP||KCode==KEY_DOWN){ TRD_Set(10); break; }
               // CTRL+/ - ��⠭���� ��砫쭮�� ��������� �७��
               if (KCode==KEY_SLASH){ TRD_Set(10); break; }
               // CTRL+M - ��४��祭�� ०��� ᤢ���
               if (KCode==KEY_M){ TRD_Set(10); break; }
               // CTRL+\ - ��⠭���� ��砫쭮�� �६��� �⮡ࠦ���� �७��
               if (KCode==KEY_BACKSLASH){ TRD_Set(10); break; }
               // CTRL+ENTER - ०�� ��।������� �६������ �१�
               if (KCode==KEY_ENTER){ TRD_Set(11); break; }
            }
            //-----------------------------------------------------------------

            //-----------------------------------------------------------------
            // �� �ࠢ����� ��ᬮ�஬ 䠩��� � ����㭪�� �.�.
            // ����� ������ ALT
            if (KAL==1){
               //--------------------------------------------------------------
               // ALT+N - ��४��� ����� ������ ��ᬮ�� ����� �࠭�
               if (KCode==KEY_N){ FV_Set(1); break; }
               // ALT+D - ���� ���� ��� ��ᬮ�� 䠩�� ����
               if (KCode==KEY_D){ FV_Set(2); break; }
               // ALT+G - ���� ��������� ��ப�
               if (KCode==KEY_G){ FV_Set(4); break; }
               // ALT+T - ����� ���� ��������� ��ப�
               if (KCode==KEY_T){ FV_Set(5); break; }
               // ALT+P - ����� ⥪�饩 ��࠭��� ��⨢���� 䠩��
               if (KCode==KEY_P){ PrintFile(1); break; }
               // ALT+O - ����� ⥪�饣� �࠭�
               if (KCode==KEY_O){ PrintScrn(1); break; }
               // ALT+S - ���-�몫 䫠�� 䨫��� �� 䫠�� ᮮ�饭��
               if (KCode==KEY_S){ FSON=(FSON>0)?0:1; break; }
               // ALT+E - ���-�몫 䫠�� 䨫��� �� ����� ��ப� ���᪠
               if (KCode==KEY_E){ FEON=(FEON>0)?0:1; break; }
               //--------------------------------------------------------------
               // ALT+> - ᫥���騩 ����㭮�
               if (KCode== KEY_STOP ){ Polzun(3,1); break; }
               // ALT+< - �।��騩 ����㭮�
               if (KCode== KEY_COMMA){ Polzun(3,2); break; }
               // ALT+��५�� �����  || -> -㢥����� ���祭�� ����㭪� �� 1%
               if (KCode== KEY_8_PAD || KCode== KEY_6_PAD){ Polzun(3,3); break; }
               // ALT+��५�� ����   || <- -㬥����� ���祭�� ����㭪� �� 1%
               if (KCode== KEY_2_PAD || KCode== KEY_4_PAD){ Polzun(3,4); break; }
               // ALT++ - 㢥����� ���祭�� ����㭪� �� 10%
               if (KCode== KEY_PLUS_PAD ){ Polzun(3,5); break; }
               // ALT+- - 㬥����� ���祭�� ����㭪� �� 10%
               if (KCode== KEY_MINUS_PAD){ Polzun(3,6); break; }
               //--------------------------------------------------------------
               // Alt+TAB-ᢥ���� �࠭ �� ������
               if (KCode==KEY_TAB){ WndCmd (1); break; }
               //--------------------------------------------------------------
            }
            //-----------------------------------------------------------------
         }  // end for
         //--------------------------------------------------------------------
         //
         LockSTR=(LockKBD==100) ? 0 : 1;
         // ��।�� ��� ��᫥���� ����⮩ ������ ��� �㭪権
         LLKey=SCode; TLKey=SCode; FVCode=SCode;
         // ���㫥��� ���� ��᫥���� ����⮩ ������
         SCode=0; KCode=0; ECode=0; OCode=0; KSH=0; KCT=0; KAL=0; vkey=0;
     }
   }
   //--------------------------------------------------------------------------
   Mexit(0,1);
   return (0);
}
//-----------------------------------------------------------------------------
