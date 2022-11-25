// coding=cp1251; version=2013020116; title="";
//----------------------------------------------------------------------------
// OPCClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OPCClient.h"
#include "OPCClientDlg.h"
#include "comutil.h"
#include "atlbase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const IID IID_IUnknown          = 
    { 0x00000000, 0x0000, 0x0000, {0xC0,0x00, 0x00,0x00,0x00,0x00,0x00,0x46}};

const IID IID_IOPCServer        = 
    { 0x39c13a4d, 0x011e, 0x11d0, {0x96,0x75, 0x00,0x20,0xaf,0xd8,0xad,0xb3}};
const IID IID_IOPCServerPublicGroups = 
    { 0x39c13a4e, 0x011e, 0x11d0, {0x96,0x75, 0x00,0x20,0xaf,0xd8,0xad,0xb3}};
const IID IID_IOPCBrowseServerAddressSpace = 
    { 0x39c13a4f, 0x011e, 0x11d0, {0x96,0x75, 0x00,0x20,0xaf,0xd8,0xad,0xb3}};

const IID IID_IOPCGroupStateMgt =
    { 0x39c13a50, 0x011e, 0x11d0, {0x96,0x75, 0x00,0x20,0xaf,0xd8,0xad,0xb3}};
const IID IID_IOPCPublicGroupStateMgt = 
    { 0x39c13a51, 0x011e, 0x11d0, {0x96,0x75, 0x00,0x20,0xaf,0xd8,0xad,0xb3}};
const IID IID_IOPCSyncIO        = 
    { 0x39c13a52, 0x011e, 0x11d0, {0x96,0x75, 0x00,0x20,0xaf,0xd8,0xad,0xb3}};
const IID IID_IOPCAsyncIO       = 
    { 0x39c13a53, 0x011e, 0x11d0, {0x96,0x75, 0x00,0x20,0xaf,0xd8,0xad,0xb3}};
const IID IID_IOPCItemMgt       = 
    { 0x39c13a54, 0x011e, 0x11d0, {0x96,0x75, 0x00,0x20,0xaf,0xd8,0xad,0xb3}};
const IID IID_IEnumOPCItemAttributes = 
    { 0x39c13a55, 0x011e, 0x11d0, {0x96,0x75, 0x00,0x20,0xaf,0xd8,0xad,0xb3}};

const IID IID_IOPCDataCallback  = 
    { 0x39c13a70, 0x011e, 0x11d0, {0x96,0x75, 0x00,0x20,0xaf,0xd8,0xad,0xb3}};
const IID IID_IOPCAsyncIO2      = 
    { 0x39c13a71, 0x011e, 0x11d0, {0x96,0x75, 0x00,0x20,0xaf,0xd8,0xad,0xb3}};

const IID IID_IOPCCommon        = 
    { 0xF31DFDE2, 0x07B6, 0x11D2, {0xB2,0xD8, 0x00,0x60,0x08,0x3B,0xA1,0xFB}};

const CLSID IID_Serkernel       = 
    { 0xD5B3AE88, 0x97B7, 0x4D0E, {0xBD,0xAE, 0xEB,0x5C,0x7B,0x88,0x8D,0x60}};


//----------------------------------------------------------------------------
// COPCClientDlg dialog
COPCClientDlg::COPCClientDlg(CWnd* pParent /*=NULL*/)
        : CDialog(COPCClientDlg::IDD, pParent)
{
        //{{AFX_DATA_INIT(COPCClientDlg)
    //}}AFX_DATA_INIT
        // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
        m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(COPCClientDlg, CDialog)
        //{{AFX_MSG_MAP(COPCClientDlg)
        ON_WM_PAINT()
        ON_WM_DESTROY()
        ON_WM_TIMER()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
void COPCClientDlg::OnPaint() 
{
//     if (!IsIconic()){
//           AddLogStr("Окно программы открыто",1);
//     }
     CDialog::OnPaint();
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  Обработка нажатий клавиш в диалоговом окне
BOOL COPCClientDlg::PreTranslateMessage(MSG* pMsg) 
{ 
    // контролировать нажатия клавиш
    if( pMsg->message == WM_KEYDOWN ){ 
        // игнорировать нажатие клавиши ESC
        if(pMsg->wParam == VK_ESCAPE ){ 
            ::TranslateMessage(pMsg); 
            ::DispatchMessage (pMsg); 
            AddLogStr("Нажата клавиша ESC",1);
            return TRUE;
        } 
        // игнорировать нажатие клавиши ENTER
        if(pMsg->wParam == 13 ){ 
            ::TranslateMessage(pMsg); 
            ::DispatchMessage (pMsg); 
            AddLogStr("Нажата клавиша ENTER",1);
            return TRUE;
        }
    } 
    return CDialog::PreTranslateMessage(pMsg); 
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
void COPCClientDlg::SetUserDefaults(void){
     int a=0, b=0;
     DBBH =0;
     // начальная установка значений переменных
     for(a=0; a<ITEMMAX; a++){
         DBMN[a]=0;
         DBBA[a]=0;
         DBFM[a]=0;
         DBBI[a]=0;
         VTCL[a]=0;
         VTSR[a]=0;
         for(b=0; b<5;     b++) DBVT[a][b]=0;
         for(b=0; b<LENFLD;b++) DBBN[a][b]=0;
         for(b=0; b<LENFLD;b++) DBRM[a][b]=0;
         for(b=0; b<LENFLD;b++) DBQA[a][b]=0;
         for(b=0; b<LENFLD;b++) DBTM[a][b]=0;
         for(b=0; b<LENFLD;b++) DBBT[a][b]=0;
         ItemYES[a]=-1;
     }
     for(a=0; a<DBBSZ; a++){
         DBBD[a]=0;
         DBBO[a]=0;
         for(b=0; b<DBBLN-1; b++){ DBBV[a][b]=32; }; DBBV[a][b]=0;
     }
     for(a=0; a<LENSTR; a++){
         HRStat  [a]=0; PEStat  [a]=0; Cnt_Stat[a]=0; CoInStat[a]=0; CoCrStat[a]=0;
         QIBrStat[a]=0; AddGStat[a]=0; QISyStat[a]=0; QICPStat[a]=0; RIT1Stat[a]=0;
         AIT1Stat[a]=0; RIT2Stat[a]=0; AIT2Stat[a]=0; FCPTStat[a]=0; AdvsStat[a]=0;
         ReadStat[a]=0; WritStat[a]=0; TimeStr [a]=0; Tmp1Str [a]=0; LTMRead [a]=0;
         LTMWrit [a]=0; ES_SPAR [a]=0; ES_AIT1 [a]=0; LogSTR  [a]=0; ssLL    [a]=0;
     }
     for(a=0; a<VTSETMAX; a++){ VTOLD[a]=0; VTNEW[a]=0; }; VTCUR =0;
     for(a=0; a<CMDMAX;   a++){ 
         for(b=0; b<CMDMAXF; b++){ CMDIF[a][b]=0; CMDRN[a][b]=0; }
     };  CMDPP=0; CMDNN=0; CMDRR=0;

     HCVR=0;
     m_updRate=1000;
     DestroyFLG=0;
     strcpy(ReadStat,"");
     strcpy(WritStat,"");
     strcpy(LTMRead, "");
     strcpy(LTMWrit, "");

     strcpy(GLMEMNAME,  "GL_MEM_NAME");
     strcpy(OPCSRVADDR, "127.0.0.1");
     strcpy(OPCSRVNAME, "OPCSRVNAME = OPC_Server_Name");
     strcpy(GROUPNAME,  "GROUPNAME");
     strcpy(ACCESSDATA, "S##");
     ADATA_S=1;
     ADATA_A=0;  
     ADATA_U=0;
     WHENCEDATA='D';
     CHECKSERVER=30;
     C_IOCLSCTX=0;
     ADDITEMS2=0;
     ADDITEMSALL=0;
     DBBC=0;
     strcpy(VTSTR,"");
     strcpy(LOGFILE, "OPCClient.log");
     LOGDISKFREE=100000000; 
     LOGMAXFILESZ=20000000;
     LOGMAXFILEDO='B';
     LOGCHECKCNT=0;
     CurLogFileSize=0;
     CurDiskFreeSpace=0;
     Cnt_WRStat=0;
     Cnt_WRSttt=0;
 	 ErrForReboot=0;

     // читать файл конфигурации
     ReadConfigFile();
     // информация о размере лог-файла и места на диске
     CheckLogFileSize();
     return; 
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// Обработка исключительных ситуаций
void i_SIGCHK(int sig){
     signal(sig,SIG_IGN); exit(0);
}
//----------------------------------------------------------------------------
BOOL COPCClientDlg::OnInitDialog()
{
        CDialog::OnInitDialog();
        int a=0, b=0;

        srvdbbEV=OpenEvent  (EVENT_ALL_ACCESS,false,"OPCCLIENTEVRUN");
        if(srvdbbEV!=NULL) {
           MessageBox("Program OPCClient already RUN!","CHECK",MB_OK); exit(0); 
        }
        srvdbbEV=CreateEvent(NULL, false, false,    "OPCCLIENTEVRUN");

        signal(SIGABRT, i_SIGCHK);
        signal(SIGFPE,  i_SIGCHK);
        signal(SIGILL,  i_SIGCHK);
        signal(SIGSEGV, i_SIGCHK);
        signal(SIGTERM, i_SIGCHK);
        signal(SIGINT,  i_SIGCHK);

        Cnt_Initialize=0;
        Cnt_Writ=0;
        Cnt_Call=0;
        Cur_Call=0;
        Not_Call=0;
        InitOPCFL=0;
        InitDefFL=0;
        CurTime=0;
        OneSec=0;
        DBBR=0;
        ErrForRebootC=0;
        LastError=0;
       
        //--------------------------------------------------------------------
        SetUserDefaults ();

        //--------------------------------------------------------------------
		AddLogStr(" ",1);
		AddLogStr(" ",1);
		AddLogStr("Запуск программы.",1);
	    AddLogStr("Информация файла конфигурации:",1);
		  sprintf(Tmp1Str,"OPCServer: %s",OPCSRVNAME);
	    AddLogStr(Tmp1Str,1);
		  sprintf(Tmp1Str,"OPCServer IP Address: %s",OPCSRVADDR);
	    AddLogStr(Tmp1Str,1);
		  sprintf(Tmp1Str,"Через сколько секунд делать переинициализацию, если сервер не отвечает: %d",CHECKSERVER);
	    AddLogStr(Tmp1Str,1);
		  sprintf(Tmp1Str,"Метод доступа к данным: %s  %s",((ADATA_S)?"SyncRead":""),((ADATA_U)?"SubScribe":""));
	    AddLogStr(Tmp1Str,1);
		if(ADATA_S>0){
		  sprintf(Tmp1Str,"Откуда брать данные для синхронного чтения: %s",((WHENCEDATA=='C')?"CACHE":"DEVICE"));
        AddLogStr(Tmp1Str,1);
		}
		  sprintf(Tmp1Str,"Имя области памяти для обмена данными: %s",GLMEMNAME);
	    AddLogStr(Tmp1Str,1);
		  sprintf(Tmp1Str,"Имя LOG-файла: %s",LOGFILE);
	    AddLogStr(Tmp1Str,1);

		AddLogStr("Создание именованной области памяти для обмена данными...",1);
        // Открыть именнованную область памяти для передачи в нее данных от OPC сервера
        hFGCB = CreateFileMapping(
       (HANDLE) 0xffffffff, NULL, PAGE_READWRITE, 0, DBBSZ*DBBLN, GLMEMNAME);
        PGCB=MapViewOfFile(hFGCB, FILE_MAP_ALL_ACCESS,0,0,0);
        if(hFGCB==NULL){ 
		    AddLogStr("ERROR.",0);
            MessageBox("Not created global memory area.","ERROR",MB_OK); 
            return FALSE; 
        }
		AddLogStr("OK.",0);
		//
		AddLogStr("Создание именованной области памяти контроля работы программы...",1);
        PM_hF = CreateFileMapping(
       (HANDLE) 0xffffffff, NULL, PAGE_READWRITE, 0, MAXCELLPROC*DBBLN, PM_GLMEMNAME);
        PM_pF=MapViewOfFile(PM_hF, FILE_MAP_ALL_ACCESS,0,0,0);
        if(PM_hF==NULL){ 
		    AddLogStr("ERROR.",0);
            MessageBox("Not created global memory area fo control proccess.","ERROR",MB_OK); 
            return FALSE; 
        }
		AddLogStr("OK.",0);

        // установить вызов функции таймера
        SetTimer(0, 40, NULL); OneSec=0;

        // начальные установки фонта перед рисованием списка
//      strcpy(CMyListCtrl::FontNM,"Tahoma";
//		CMyListCtrl::FontSZ=12;
//      CMyListCtrl::FontST=FW_NORMAL;
//		CMyListCtrl::FontCL=RGB(0,0,160);

        // начальная установка переменных
        lp_IOPCServer               = NULL;
        lp_IOPCBrowse               = NULL;
        lp_IOPCItemMgt              = NULL;
        lp_IOPCSyncIO               = NULL;
        lp_IConnectionPoint         = NULL;
        lp_IConnectionPointContainer= NULL;

        //--------------------------------------------------------------------
        // Построение окна вывода строк статуса
        myLStat.InsertColumn( 0,"Operation",     LVCFMT_LEFT, 200, -1); 
        myLStat.InsertColumn( 1,"Status HR",     LVCFMT_LEFT, 265, -1); 
        myLStat.InsertColumn( 2,"Description",   LVCFMT_LEFT, 325, -1); 
        for(a=0; a<17; a++){ myLStat.InsertItem(0,""); }

        myLStat.SetItemText ( 0, 0, "OPC Server");
        myLStat.SetItemText ( 1, 0, "IPTest");
        myLStat.SetItemText ( 2, 0, "CoInitialize");
        myLStat.SetItemText ( 3, 0, "CoCreateInctance");
        myLStat.SetItemText ( 4, 0, "QI BrowseServerAddressSpace");
        myLStat.SetItemText ( 5, 0, "AddGroup");
        myLStat.SetItemText ( 6, 0, "QI SyncIO");
        myLStat.SetItemText ( 7, 0, "QI ConnectionPointContainer");
        myLStat.SetItemText ( 8, 0, "RemoveItems 1");
        myLStat.SetItemText ( 9, 0, "AddItems 1");
        myLStat.SetItemText (10, 0, "RemoveItems 2");
        myLStat.SetItemText (11, 0, "AddItems 2");
        myLStat.SetItemText (12, 0, "FindConnection Point");
        myLStat.SetItemText (13, 0, "Advise");
        myLStat.SetItemText (14, 0, "Initialize");
        myLStat.SetItemText (15, 0, "Read");
        myLStat.SetItemText (16, 0, "Write");
        myLStat.SendMessage (WM_PAINT,0,0);
        //--------------------------------------------------------------------
        // Построение окна вывода списка
        myLView.InsertColumn( 0,"Num", LVCFMT_LEFT,  35, -1); 
        myLView.InsertColumn( 1,"ItemsID",      LVCFMT_LEFT,  50, -1); 
        myLView.InsertColumn( 2,"Channel Name", LVCFMT_LEFT, 170, -1); 
        myLView.InsertColumn( 3,"Values",       LVCFMT_LEFT,  80, -1); 
        myLView.InsertColumn( 4,"tCL",          LVCFMT_LEFT,  30, -1); 
        myLView.InsertColumn( 5,"tSR",          LVCFMT_LEFT,  30, -1); 
        myLView.InsertColumn( 6,"tRT",          LVCFMT_LEFT,  30, -1); 
        myLView.InsertColumn( 7,"Count",        LVCFMT_LEFT,  40, -1); 
        myLView.InsertColumn( 8,"Quality",      LVCFMT_LEFT, 155, -1); 
        myLView.InsertColumn( 9,"TimeStamp",    LVCFMT_LEFT, 120, -1); 
        myLView.InsertColumn(10,"Description",  LVCFMT_LEFT, 225, -1); 
        //--------------------------------------------------------------------
        // Окно лога
        myLLogV.InsertColumn( 0,"", LVCFMT_LEFT, 620, -1);
        //--------------------------------------------------------------------
//      CListCtrl:

        return TRUE;  
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
void COPCClientDlg::OnDestroy() 
{
        CDialog::OnDestroy();
  	    AddLogStr("Освобождение памяти:",1);

        if (lp_IConnectionPoint!=NULL){
            AddLogStr("lp_IConnectionPoint->Unadvise(conCookie)...",1);
            lp_IConnectionPoint->Unadvise(conCookie);
            AddLogStr("OK",0);
            AddLogStr("lp_IConnectionPoint->Release()...",1);
            lp_IConnectionPoint->Release();
            AddLogStr("OK",0);
        }
        if (lp_IConnectionPointContainer!=NULL){
            AddLogStr("lp_IConnectionPointContainer->Release()...",1);
            lp_IConnectionPointContainer->Release();
            AddLogStr("OK",0);
        }
        if (lp_IOPCSyncIO   !=NULL){ 
            AddLogStr("lp_IOPCSyncIO->Release()...",1);
            lp_IOPCSyncIO->Release();  
            AddLogStr("OK",0);
        }
        if (lp_IOPCItemMgt  !=NULL){ 
            AddLogStr("lp_IOPCItemMgt->Release()...",1);
            lp_IOPCItemMgt->Release(); 
            AddLogStr("OK",0);
        }
        if (lp_IOPCBrowse   !=NULL){ 
            AddLogStr("lp_IOPCBrowse->Release()...",1);
            lp_IOPCBrowse->Release();
            AddLogStr("OK",0);
        }
        if (lp_IOPCServer   !=NULL){ 
            AddLogStr("lp_IOPCServer->Release()...",1);
            lp_IOPCServer->Release();  
            AddLogStr("OK",0);
        }
            AddLogStr("  ",1);

        if (DestroyFLG==0){ exit(0); }
        return;
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
BEGIN_EVENTSINK_MAP(COPCClientDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(COPCClientDlg)
    //}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// Перезагрузка компьютера
void CompReboot(void) 
{
   HANDLE hToken;
   TOKEN_PRIVILEGES tkp;

   char *name="";    // address of name of computer to shut down
   char *msg ="";    // address of message to display in dialog box
   DWORD time= 0;    // time to display dialog box
   int   force=true; // force applications with unsaved changes flag
   int  reboot=true; // reboot flag

   OpenProcessToken(GetCurrentProcess(),
                     TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
 
   if(!LookupPrivilegeValue(name, SE_SHUTDOWN_NAME,&tkp.Privileges[0].Luid)){
   //  printf("SE_SHUTDOWN_NAME Privilege access denied.\n");
       return;
   }
   tkp.PrivilegeCount =1;
   tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
   AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,(PTOKEN_PRIVILEGES)NULL, 0);
   if(!LookupPrivilegeValue(name,
      SE_REMOTE_SHUTDOWN_NAME,&tkp.Privileges[0].Luid)){
   // printf("SE_REMOTE_SHUTDOWN_NAME Privilege access denied.\n");
      return;
   }
   tkp.PrivilegeCount =1;
   tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
   AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,(PTOKEN_PRIVILEGES)NULL, 0);
   if (InitiateSystemShutdown(name,msg,time,force,reboot)){
   //  printf("%s shutdown Ok\n",name);
   } else {
   //  printf("Can't shutdown %s\n",name);
   }
   return;
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  Функция таймера
void COPCClientDlg::OnTimer(UINT nIDEvent) 
{
     OneSec++;
     if(OneSec>25){
        OneSec=0;
        // вывод на экран значений каналов
        ViewValues();
        // сделать инициализацию и подключение, если еще не делали
        if(InitOPCFL==0){ InitOPCCL(); InitOPCFL=1; }
        // если нет ответов OPC сервера- установить флаг вывода сообщения
        if(Cur_Call==Cnt_Call){
           Not_Call++;
           if(Not_Call>CHECKSERVER){ 
              Not_Call=0; InitOPCFL=0;
                sprintf(Tmp1Str,"Через %d сек нет ответа от сервера.",CHECKSERVER);
              AddLogStr(Tmp1Str,1);
              AddLogStr("Переинициализация...",1);
			  if(ErrForRebootC<1000000) ErrForRebootC++;;
           }
        } else {
           Cur_Call=Cnt_Call;
           Not_Call=0;
        }   
        // синхронное чтение данных
        if(lp_IOPCItemMgt != NULL && ADATA_S>0){
           SyncRead();
        }
        // выполнение отдельных комманд
        for(CMDRR=0; CMDRR<CMDNN; CMDRR++){
            CMDA1=DBBD[(int)CMDIF[CMDRR][0]];
            CMDA2=CMDIF[CMDRR][2];
            if((CMDIF[CMDRR][1]==1 && CMDA1==CMDA2) ||
               (CMDIF[CMDRR][1]==2 && CMDA1!=CMDA2) ||
               (CMDIF[CMDRR][1]==3 && CMDA1> CMDA2) ||
               (CMDIF[CMDRR][1]==4 && CMDA1< CMDA2) ){
                CMDA1=CMDIF[CMDRR][3];
                // если это комманда записи значения
                if((char)CMDRN[(int)CMDA1][0]=='W'){
                   SyncWrite((int)CMDRN[(int)CMDA1][1],CMDRN[(int)CMDA1][2]);
                }
            }
        }
        // проверка размера LOG-файла
        if(LOGCHECKCNT>LOGCHECKPER){
           LOGCHECKCNT=0; CheckLogFileSize();
        }; LOGCHECKCNT++;
        // запись статистики работы программы в лог-файл
        if(Cnt_WRStat==10    || Cnt_WRStat==40    || Cnt_WRStat==60*2  ||
           Cnt_WRStat==60*5  || Cnt_WRStat==60*10 || Cnt_WRStat==60*20 || Cnt_WRStat==60*60){
           WRStat(Cnt_WRStat);
        }
        if(Cnt_WRSttt>=60*60){ WRStat(Cnt_WRSttt); Cnt_WRSttt= 0; }
        if(Cnt_WRStat< 60*61){ Cnt_WRStat++; }
        if(Cnt_WRSttt< 60*61){ Cnt_WRSttt++; }
		if(ErrForReboot>0 && ErrForRebootC>=ErrForReboot){
             sprintf(Tmp1Str,"Количество неответов OPC сервера превысило %d",ErrForReboot);
           AddLogStr(Tmp1Str,1);
           AddLogStr("Перезагрузка компьютера...",1);
           LastError=GetLastError();
           CompReboot();
		}
     }
     myLStat.SendMessage(WM_PAINT,0,0);
     myLView.SendMessage(WM_PAINT,0,0);
     myLLogV.SendMessage(WM_PAINT,0,0);
     CDialog::OnTimer(nIDEvent);
}
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
void COPCClientDlg::DoDataExchange(CDataExchange* pDX) 
{
    // TODO: Add your specialized code here and/or call the base class
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, myLView);   
    DDX_Control(pDX, IDC_STAT,  myLStat);   
    DDX_Control(pDX, IDC_LIST2, myLLogV);   
}
//----------------------------------------------------------------------------
