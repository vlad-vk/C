//----------------------------------------------------------------------------
// OPCClientDlg.h
//

#include "stdlib.h"
#include "stdio.h"
#include <string.h>
#include <fcntl.h>
#include <io.h>
#include <direct.h>
#include <signal.h>

#include <winsock2.h>
#include <ws2tcpip.h>

#include "backclass.h"
#include "opcda.h"

#include "MyListCtrl.h"

#if !defined(AFX_OPCClientDLG_H__D481FC4C_2EA8_48DC_B154_BFD40584B2A0__INCLUDED_)
#define AFX_OPCClientDLG_H__D481FC4C_2EA8_48DC_B154_BFD40584B2A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DBBSZ         2000   // кол-во ячеек в массиве каналов
#define DBBLN           10   // длинна имен и строковых значений каналов
#define RDBUFSZ     100000   // размер буфера чтения файла конфигураци
#define LENFLD         100   // макс длинна одного поля считанной строки
#define LENSTR         500   // длинна считанной строки
#define REMFLD           7   // номер поля комментария (см. cfgf.h)
#define ITEMMAX        500   // макс количество переменных OPC запроса
#define NOTDEF        7777   // неопределенный флаг
#define VT_MYMANTIME    97   // тип TIME для значений устанавливаемых вручную
#define VTSETMAX        10   // макс кол-во преобразований типов
#define CMDMAX          50   // макс кол-во комманд
#define CMDMAXF          5   // макс кол-во полей в комманде
#define LOGCHECKPER     60   // кол-во сек для проверки размера LOG-файла 
#define MAXCELLPROC     10   // макс кол ячеек контролируемых процессов

//----------------------------------------------------------------------------
class COPCClientDlg : 
                        public CDialog,
                        public IOPCDataCallback
{
//----------------------------------------------------------------------------
// Construction
public:

        OPCHANDLE                       phServer[ITEMMAX];
        DWORD                           ref;
        DWORD                           conCookie;
        HRESULT                         HRFlag;
        HRESULT                         PEFlag;
        HRESULT                         hrL;
        HRESULT *                       pErrorm1;
        HRESULT *                       pErrorm;
        HRESULT *                       pErrorl;
        OPCITEMRESULT *                 pItemResult1;
        OPCITEMRESULT *                 pItemResult;
        OPCITEMSTATE  *                 pItemStateL;

        IConnectionPointContainer *     lp_IConnectionPointContainer;
        IConnectionPoint *              lp_IConnectionPoint;
        IOPCDataCallback *              lp_IOPCDataCallback;
        IOPCBrowseServerAddressSpace *  lp_IOPCBrowse;
        IOPCItemMgt *                   lp_IOPCItemMgt;
        IOPCServer  *                   lp_IOPCServer;
        IOPCSyncIO  *                   lp_IOPCSyncIO;

        COPCClientDlg(CWnd* pParent = NULL);

char    HRStat  [LENSTR];      // Статус последней операции 
char    PEStat  [LENSTR];      //     
char    Cnt_Stat[LENSTR];
char    CoInStat[LENSTR];
char    CoCrStat[LENSTR];
char    QIBrStat[LENSTR];
char    AddGStat[LENSTR];
char    QISyStat[LENSTR];
char    QICPStat[LENSTR];
char    RIT1Stat[LENSTR];
char    AIT1Stat[LENSTR];
char    RIT2Stat[LENSTR];
char    AIT2Stat[LENSTR];
char    FCPTStat[LENSTR];
char    AdvsStat[LENSTR];
char    ReadStat[LENSTR];
char    WritStat[LENSTR];

int     Cnt_Writ;              // счетчик записи
int     Cnt_Initialize;        // счетчик переинициализаций
char    TimeStr[LENSTR];       // временная строка
char    Tmp1Str[LENSTR];       // дополнительная строка для временных преобразований
char    LTMRead[LENSTR];       // время последнего чтения
char    LTMWrit[LENSTR];       // время последней  записи

char    ES_SPAR[LENSTR];       // строка парамметров сервера
char    ES_AIT1[LENSTR];       // номера каналов не добавленных в 1ой попытке

HANDLE  hFGCB;                 // номер именованного файла памяти
HANDLE  PM_hF;                 // номер именованного файла памяти
LPVOID  PGCB;                  // указатель на данные именованной области
LPVOID  PM_pF;                 // указатель на данные именованной области
char    PM_s[10];              // переменная передачи значения в область памяти контроля
int     PM_CPCNUM;             // номер ячейки в области контроля работы программы
int     PM_i;                  // счетчик контроля работы программы

HANDLE	srvdbbEV;              // указатель запуска программы

long    Cnt_Call;              // счетчик вызывов обмена
long    Cur_Call;
long    Not_Call;

int     InitOPCFL;             // флаг инициализации клиента OPC
int     InitDefFL;             // флаг инициализации массива ItemDef

OPCITEMDEF ItemDef[ITEMMAX];   // структура описания тегов (NA)
int        ItemYES[ITEMMAX];   // флаг присутствия переменной на сервере

//----------------------------------------------------------------------------
// NA
char    READBUF[RDBUFSZ*2];    // буфер чтения файла конфигурации
char    READBUI[RDBUFSZ];      // буфер чтения файла конфигурации
char    STRBUF [LENSTR];       // буфер текущей строки
char    CURFLD [LENFLD];       // буфер текущего поля
char    CURCMD [LENFLD];       // буфер текущей комманды
int		DestroyFLG;            // флаг выхода из программы
//----------------------------------------------------------------------------
char    GLMEMNAME   [LENFLD];  // имя области памяти для обмена
char    PM_GLMEMNAME[LENFLD];  // имя области памяти для контроля работы программы

char    OPCSRVADDR  [LENFLD];  // IP  адрес OPC сервера
char    OPCSRVNAME  [LENFLD];  // имя OPC сервера
char    GROUPNAME   [LENFLD];  // имя добавляемой группы
char    ACCESSDATA  [LENFLD];  // метод доступа к данным [S]ync,[A]sync,s[U]bscr
char    ADATA_S;               // флаги методов доступа к данным
char    ADATA_A;  
char    ADATA_U;
char    WHENCEDATA;            // откуда брать данные при чтении (device,cache)
int     CHECKSERVER;           // период проверки работы сервера (сек)
int     C_IOCLSCTX;            // местоположение DCOM задачи
int     ADDITEMS2;             // флаг повторного добавления тэгов
int     ADDITEMSALL;           // флаг добавления тэгов поодному или всех сразу
int     VTOLD[VTSETMAX];       // переопределение типов переменных каналов(тэгов)
int     VTNEW[VTSETMAX];
int     VTCUR;                 // текущий элемент преобразования
double  CMDIF[CMDMAX][CMDMAXF];// массив условий для выполнения отдельных комманд
double  CMDRN[CMDMAX][CMDMAXF];// отдельно выполняемые комманды
double  CMDPP,CMDA1,CMDA2;     // переменная для преобразования комманд
int     CMDNN,CMDRR;           // текущий номер комманды сравнения
double  LOGDISKFREE;           // минимальный размер диска для записи лог-файла
double  LOGMAXFILESZ;          // макс допустимый размер лог-файла
int     LOGMAXFILEDO;          // действие при макс размере лог-файла
int     LOGCHECKCNT;           // счетчик секунд контроля размера лог-файла
double  CurLogFileSize;        // текущий размер лог-файла
double  CurDiskFreeSpace;      // текущий размер свободного места на диске
struct  _diskfree_t  gDSKSP;   // структура информации о диске
int     FileHD;                // системный номер открываемого файла
int     Cnt_WRStat;            // счетчик секунд для записи статистики работы
int     Cnt_WRSttt;            // счетчик секунд для записи статистики работы
int     ErrForReboot;          // кол-во ошибок OPC сервера для перезагрузки(задание)
int     ErrForRebootC;         // кол-во ошибок OPC сервера для перезагрузки(счетчик)
int     LastError;
//----------------------------------------------------------------------------
double  DBBD[DBBSZ];           // значения каналов в формате double
char    DBBV[DBBSZ][DBBLN];    // значения в формате char "%9.2f\0" для передачи
int     DBBH;                  // количество переменных определенных для запроса
int     DBBC;                  // идентификатор  текущей переменной
int     DBBI[ITEMMAX];         // номера идентификаторов переменных
int     DBMN[ITEMMAX];         // номера переменных для ручного преобразования
int     DBBO[DBBSZ];           // номера обратной связи с массивом DBBI[]

char    DBBT[ITEMMAX][LENFLD]; // типы переменных (признак сохранения в архив)
char    DBBN[ITEMMAX][LENFLD]; // имена переменных
char    DBRM[ITEMMAX][LENFLD]; // описания переменных (комментарии)
char    DBTM[ITEMMAX][LENFLD]; // время последнего изменения переменной TimeStamp
char    DBQA[ITEMMAX][LENFLD]; // флаг QUALITY последнего чтения
char    DBVT[ITEMMAX][5];      // типы данных OPC (VT_I2,VT_R4,...)
int     DBFM[ITEMMAX];         // массив признака изменения переменной
int     DBBA[ITEMMAX];         // количество ответов по каждой переменной
int     VTCL[ITEMMAX];         // тип данных переменных, запрашиваемый клиентом
int     VTSR[ITEMMAX];         // тип данных переменных, определенный на сервере
char    VTSTR[LENFLD];         // строка типа данных для функции GetVTSTR()
char    QASTR[LENFLD];         // строка достоверности данных для функции GetQASTR()
int     DBBR;                  // реальное количество переменных
int     HCVR;				   // количество переменных считанных при текущем
							   // последнем чтении
char    tuDBBS[LENFLD];        // строки преобразования значений
char    tsDBBS[LENFLD];
//----------------------------------------------------------------------------
DWORD   m_updRate;             // время обновления значений группы
//----------------------------------------------------------------------------
CMyListCtrl  myLView;               // переменная класса CListCtrl (вывод списка)
CMyListCtrl  myLStat;               // переменная класса CListCtrl (вывод статуса)
CMyListCtrl  myLLogV;               // переменная класса CListBox  (вывод лога)

char         LogSTR [LENSTR];       // текущая строка, записываемая в лог
char         LOGFILE[LENSTR];       // имя LOG-файла
char         MOVFILE[LENSTR];       // имя LOG-файла

time_t       CurTime;               // переменная текущего времени
struct       tm * mTM;              // структура для преобразования времени
SYSTEMTIME   mSTM;                  // структура для преобразования времени

int          OneSec;                // флаг одной секунды для функции таймера
char         ssLL[LENSTR];
FILE         *ff;
//----------------------------------------------------------------------------
// вывод на экран
int          va, vb, vi, vx, vy, vv;
float        vf;
char         vvtcl[5], vvtsr[5], vcc[20];
//----------------------------------------------------------------------------
// SyncWrite
int			 swi, swa, swx, swz;
VARIANT      swVarVALUE[2];
//----------------------------------------------------------------------------
// SyncRead
int          sra, sri, srz, srx, srCnt_One;
float        srf;
CString      srtmpstr;
CTime        srtimestamp;
//----------------------------------------------------------------------------
// OnDataChange
CString      odtmpstr;
CTime        odtimestamp;
//----------------------------------------------------------------------------
// InitOPC
int          ioa, iok, ioi, iol, iox, iov, ioz;
char         iotmpWstr[200], iotmpWstr1[200], iocc[20];
HRESULT      iohr;
OLECHAR      iob [200];
OLECHAR      iosg[200];
BSTR         ioOGRP;
float        prDeadBand;             // [in]  AddGroup
DWORD        revUpdRate;             // [out] AddGroup
OPCHANDLE    m_hServerGroup;         // [out] AddGroup
MULTI_QI     iomqi[1];
COSERVERINFO iosin;
CLSID        ioclsid;
DWORD        ioclsctx;
//----------------------------------------------------------------------------
void SetUserDefaults(void);
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
// Получить строку типа данных
void GetVTSTR(int vt){
	 strcpy(VTSTR,"##");
	 switch (vt){
	     case VT_EMPTY:           strcpy(VTSTR,"EM"); break;;   // 0
	     case VT_NULL:            strcpy(VTSTR,"NL"); break;;   // 1
	     case VT_I2:              strcpy(VTSTR,"I2"); break;;   // 2
	     case VT_I4:              strcpy(VTSTR,"I4"); break;;   // 3
	     case VT_R4:              strcpy(VTSTR,"R4"); break;;   // 4
	     case VT_R8:              strcpy(VTSTR,"R8"); break;;   // 5
	     case VT_CY:              strcpy(VTSTR,"CY"); break;;   // 6
	     case VT_DATE:            strcpy(VTSTR,"DT"); break;;   // 7  
	     case VT_BSTR:            strcpy(VTSTR,"BS"); break;;   // 8
	     case VT_DISPATCH:        strcpy(VTSTR,"DP"); break;;   // 9
	     case VT_ERROR:           strcpy(VTSTR,"ER"); break;;   // 10
	     case VT_BOOL:            strcpy(VTSTR,"BL"); break;;   // 11
	     case VT_VARIANT:         strcpy(VTSTR,"VR"); break;;   // 12
	     case VT_UNKNOWN:         strcpy(VTSTR,"UN"); break;;   // 13
	     case VT_DECIMAL:         strcpy(VTSTR,"DC"); break;;   // 14
	     case VT_I1:              strcpy(VTSTR,"I1"); break;;   // 16
	     case VT_UI1:             strcpy(VTSTR,"U1"); break;;   // 17
	     case VT_UI2:             strcpy(VTSTR,"U2"); break;;   // 18
	     case VT_UI4:             strcpy(VTSTR,"U4"); break;;   // 19
	     case VT_I8:              strcpy(VTSTR,"I8"); break;;   // 20
	     case VT_UI8:             strcpy(VTSTR,"U8"); break;;   // 21
	     case VT_INT:             strcpy(VTSTR,"IN"); break;;   // 22
	     case VT_UINT:            strcpy(VTSTR,"UI"); break;;   // 23
	     case VT_VOID:            strcpy(VTSTR,"VD"); break;;   // 24
	     case VT_HRESULT:         strcpy(VTSTR,"HR"); break;;   // 25
	     case VT_PTR:             strcpy(VTSTR,"PT"); break;;   // 26
		 case VT_SAFEARRAY:       strcpy(VTSTR,"SA"); break;;   // 27
		 case VT_CARRAY:          strcpy(VTSTR,"CA"); break;;   // 28
		 case VT_USERDEFINED:     strcpy(VTSTR,"UD"); break;;   // 29
		 case VT_LPSTR:           strcpy(VTSTR,"LS"); break;;   // 30
		 case VT_LPWSTR:          strcpy(VTSTR,"LW"); break;;   // 31
		 case VT_FILETIME:        strcpy(VTSTR,"FT"); break;;   // 64
		 case VT_BLOB:            strcpy(VTSTR,"BB"); break;;   // 65
		 case VT_STREAM:          strcpy(VTSTR,"SR"); break;;   // 66
		 case VT_STORAGE:         strcpy(VTSTR,"SG"); break;;   // 67
		 case VT_STREAMED_OBJECT: strcpy(VTSTR,"SO"); break;;   // 68
		 case VT_STORED_OBJECT:   strcpy(VTSTR,"RO"); break;;   // 69
		 case VT_BLOB_OBJECT:     strcpy(VTSTR,"BO"); break;;   // 70
		 case VT_CF:              strcpy(VTSTR,"CF"); break;;   // 71
		 case VT_CLSID:           strcpy(VTSTR,"CS"); break;;   // 72
		 case VT_MYMANTIME:       strcpy(VTSTR,"MT"); break;;   // 97
		 case VT_VECTOR:          strcpy(VTSTR,"VR"); break;;   // 0x1000
		 case VT_ARRAY:           strcpy(VTSTR,"AR"); break;;   // 0x2000
		 case VT_BYREF:           strcpy(VTSTR,"BR"); break;;   // 0x4000
		 case VT_RESERVED:        strcpy(VTSTR,"RS"); break;;   // 0x8000
		 case VT_ILLEGAL:         strcpy(VTSTR,"IL"); break;;   // 0xffff
		 case VT_ILLEGALMASKED:   strcpy(VTSTR,"IM"); break;;   // 0xfff
     }
     return;
}
//----------------------------------------------------------------------------
// Получить строку достоверности данных
void GetQASTR(int qa){
	 strcpy(QASTR,"##");
	 switch (qa){
         //
         case OPC_QUALITY_BAD: {
              strcpy(QASTR,"B:00H: BAD"); break;;
         }
         case OPC_QUALITY_UNCERTAIN: {
              strcpy(QASTR,"U:40H: UNCERTAIN"); break;;
         }
         case OPC_QUALITY_GOOD: {
              strcpy(QASTR,"G:C0H: GOOD"); break;;
         }
         // STATUS_MASK Values for Quality = BAD
         case OPC_QUALITY_CONFIG_ERROR: {
              strcpy(QASTR,"B:04H: CONFIG_ERROR"); break;;
         }
         case OPC_QUALITY_NOT_CONNECTED: {
              strcpy(QASTR,"B:08H: NOT_CONNECTED"); break;;
         }
         case OPC_QUALITY_DEVICE_FAILURE: {
              strcpy(QASTR,"B:0CH: DEVICE_FAILURE"); break;;
         }
         case OPC_QUALITY_SENSOR_FAILURE: {
              strcpy(QASTR,"B:10H: SENSOR_FAILURE"); break;;
         }
         case OPC_QUALITY_LAST_KNOWN: {
              strcpy(QASTR,"B:14H: LAST_KNOWN"); break;;
         }
         case OPC_QUALITY_COMM_FAILURE: {
              strcpy(QASTR,"B:18H: COMM_FAILURE"); break;;
         }
         case OPC_QUALITY_OUT_OF_SERVICE: {
              strcpy(QASTR,"B:1C: OUT_OF_SERVICE"); break;;
         }
         // STATUS_MASK Values for Quality = UNCERTAIN
         case OPC_QUALITY_LAST_USABLE: {
              strcpy(QASTR,"U:44H: LAST_USABLE"); break;;
         }
         case OPC_QUALITY_SENSOR_CAL: {
              strcpy(QASTR,"U:50H: SENSOR_CAL"); break;;
         }
         case OPC_QUALITY_EGU_EXCEEDED: {
              strcpy(QASTR,"U:54H: EGU_EXCEEDED"); break;;
         }
         case OPC_QUALITY_SUB_NORMAL: {
              strcpy(QASTR,"U:58H: SUB_NORMAL"); break;;
         }
         // STATUS_MASK Values for Quality = GOOD
         case OPC_QUALITY_LOCAL_OVERRIDE: {
              strcpy(QASTR,"G:D8H: LOCAL_OVERRIDE"); break;;
         }
     }
     return;
}
//----------------------------------------------------------------------------
// Получить статус последнего кода возврата hr
void GetHRFlag(void){
	 sprintf(HRStat,"Not defined error or not used");
     switch (HRFlag) {
		case  E_FAIL:        sprintf(HRStat,"The operation failed"); break;
		case  E_OUTOFMEMORY: sprintf(HRStat,"Not enough memory"); break;
		case  E_INVALIDARG:  sprintf(HRStat,"An argument to the function was invalid (e.g.dwCount=0)"); break;
		case  S_OK:          sprintf(HRStat,"The operation succeeded"); break; 
		case  S_FALSE:       sprintf(HRStat,"The operation completed with one or more errors"); break;
	 }
	 return;
}	
//----------------------------------------------------------------------------
// Получить статус последнего кода возврата pErrorm
void GetPEFlag(void){
	 sprintf(PEStat,"Not defined error or not used");
     switch (PEFlag) {
		case  S_OK:          sprintf(PEStat,"The function was successful  "); break; 
		case  E_FAIL:        sprintf(PEStat,"The function was unsuccessful"); break;
	 }
	 return;
}	
//----------------------------------------------------------------------------
// Получить текущее время в строку TimeStr
void GetCurTime(void){
     CurTime =time(NULL);
     mTM=localtime(&CurTime);
	 sprintf(TimeStr,"%02d.%02d.%04d  %02d:%02d:%02d ",
		              mTM->tm_mday,mTM->tm_mon+1,mTM->tm_year+1900,mTM->tm_hour,mTM->tm_min,mTM->tm_sec);
     return;
}
//----------------------------------------------------------------------------
// Получить число из заданного диапазона бит
int GetBitVal(USHORT val, int lo, int hi){
	int a=0, b=0, c=0;
	if(lo>hi){ hi=lo; }
	if(hi>15){ return(0); }
    for(a=hi; a>=lo; a--){ b=(val >> a) & 1; c=c |(b<<(a-lo)); }
    return(c);
}
//----------------------------------------------------------------------------
// Преобразует символ % в строке текущего поля в пробел
void ConvertCurField(){
     int a=0, l=strlen(CURFLD);
	 for(a=0; a<l; a++){
         if(CURFLD[a]=='%'){ CURFLD[a]=' '; }
	 }
}
//----------------------------------------------------------------------------
// Чтение файла конфигурации
int  ReadConfigFile(void){
     int  hf=0, hb=0, hi=0, a=0, c=0, b=0, f=0, r=0, bs=0, i=0;
	 UINT cs=0;

     if(( hf = open("OPCClient.cfg", O_RDONLY | O_BINARY)) <0){
          MessageBox("Error read config file OPCClient.cfg","ERROR",MB_OK); 
          return FALSE;
     }
     hb=read(hf, READBUF, RDBUFSZ); READBUF[hb]=0; 
     close(hf);
     strcpy(PM_GLMEMNAME,"PM_GLMEMNAME");
	 PM_CPCNUM=MAXCELLPROC-1;

     //-----------------------------------------------------------------------
     // обработка считанного буфера
     for(a=0; a<hb; a++){
         // если это конец поля
         cs= READBUF[a];
         // начальные пробелы не учитывать
         if (cs<=32&&bs==0){ continue; }
         else              { bs=1;     }
         // если текущий символ разделитель поля
         if((cs==32||cs==10||cs==0||cs=='=')&&(c==0)&&(r==0)){
            CURFLD[b]=0; c=1;
            //----------------------------------------------------------------
            // если это поле комманды - запомнить его
            if(f==0){ strcpy(CURCMD,CURFLD); }
            //----------------------------------------------------------------
            // если это поле 1-го парамметра
            if(f==1){ 
               // Имя области памяти для передачи данных другим программам
               if(stricmp(CURCMD,"GLMEMNAME")==0){
                  if(strlen(CURFLD)>0){ strcpy(GLMEMNAME,CURFLD); }
                  else                { strcpy(GLMEMNAME,"NTLDSRV_GET_DATA"); }
               }
               // Имя области памяти для контроля работы программы
               if(stricmp(CURCMD,"CPROCGMEM")==0){
                  if(strlen(CURFLD)>0){ strcpy(PM_GLMEMNAME,CURFLD); }
               }
               // номер ячейки в области памяти контроля программ
               if(stricmp(CURCMD,"CPCNUMBER")==0){
				  i=atoi(CURFLD);
                  if(i>0 && i<MAXCELLPROC){ PM_CPCNUM=i; }
               }
               // IP адрес OPC сервера
               if(stricmp(CURCMD,"OPCSRVADDR")==0){
                  if(strlen(CURFLD)>0){ strcpy(OPCSRVADDR,CURFLD); }
                  else                { strcpy(OPCSRVADDR,"127.0.0.1"); }
               }
               // Имя OPC сервера
               if(stricmp(CURCMD,"OPCSRVNAME")==0){
                  if(strlen(CURFLD)>0){ strcpy(OPCSRVNAME,CURFLD); }
                  else                { strcpy(OPCSRVNAME,"NOT_DEFINED_OPC_SERVER_NAME"); }
               }
               // Местоположение DCOM задачи
               if(stricmp(CURCMD,"IOCLSCTX")==0){
                   if(CURFLD[0]=='L'){ C_IOCLSCTX=0; }
                   if(CURFLD[0]=='R'){ C_IOCLSCTX=1; }
                   if(CURFLD[0]=='A'){ C_IOCLSCTX=2; }
               }
               // Имя добавляемой группы данных
               if(stricmp(CURCMD,"GROUPNAME")==0){
                  if(strlen(CURFLD)>0){ strcpy(GROUPNAME,CURFLD); }
                  else                { strcpy(GROUPNAME,"myOPCGROUP"); }
               }
               // Метод доступа к данным
               if(stricmp(CURCMD,"ACCESSDATA")==0){
                  if(strlen(CURFLD)>0){ strcpy(ACCESSDATA,CURFLD); }
                  else                { strcpy(ACCESSDATA,"###" ); }
                  if(ACCESSDATA[0]=='S'){ ADATA_S=1; }
                  else                  { ADATA_S=0; }
                  if(ACCESSDATA[2]=='U'){ ADATA_U=1; }
                  if(ACCESSDATA[0]!='A' &&  ACCESSDATA[2]!='U'){ ADATA_S=1; }
               }
               // Откуда брать данные при чтении
               if(stricmp(CURCMD,"WHENCEDATA")==0){
                  if(stricmp(CURFLD,"DEVICE")==0){ WHENCEDATA='D'; }
                  else                           { WHENCEDATA='C'; }
               }
               // Период проверки сервера
               if(stricmp(CURCMD,"CHECKSERVER")==0){
				  i=atoi(CURFLD);
				  if(i<1 || i>1000){ CHECKSERVER=10; }
				  else             { CHECKSERVER= i; }
               }
               // Кол-во ошибок(не ответов) OPC сервера для перезагрузки
               if(stricmp(CURCMD,"ERRFORREBOOT")==0){
				  CURFLD[9]=0; i=atoi(CURFLD);
				  if(i<1 || i>1000000){ ErrForReboot=1000; }
				  else                { ErrForReboot=i;    }
               }
               // Повторное добавление тэгов
               if(stricmp(CURCMD,"ADDITEMS2")==0){
                   if(CURFLD[0]>'0'){ ADDITEMS2=1; }
               }
               // Метод добавления тэгов
               if(stricmp(CURCMD,"ADDITEMSALL")==0){
                   if(CURFLD[0]>'0'){ ADDITEMSALL=1; }
               }
               // Имя LOG-файла
               if(stricmp(CURCMD,"LOGFILE")==0){
                  if(strlen(CURFLD)>0){ strcpy(LOGFILE,CURFLD); }
                  else                { strcpy(LOGFILE,"OPCClient.log" ); }
               }
               // Минимально свободное место на диске для записи LOG-файла
               if(stricmp(CURCMD,"LOGDISKFREE")==0){
                  CMDPP=atof(CURFLD); 
                  if(CMDPP<1000000 || CMDPP>1000000000) CMDPP=1000000;
                  LOGDISKFREE=CMDPP;
               }
               // Максимально допустимый размер LOG-файла
               if(stricmp(CURCMD,"LOGMAXFILESZ")==0){
                  CMDPP=atof(CURFLD); 
                  if(CMDPP<100000 || CMDPP>100000000) CMDPP=100000;
                  LOGMAXFILESZ=CMDPP;
               }
               // Действие при достижении максимального размера файла
               if(stricmp(CURCMD,"LOGMAXFILESZ")==0){
                   if(CURFLD[0]=='O'){ LOGMAXFILEDO='O'; }
                   if(CURFLD[0]=='B'){ LOGMAXFILEDO='B'; }
                   if(CURFLD[0]=='N'){ LOGMAXFILEDO='N'; }
                   if(CURFLD[0]=='A'){ LOGMAXFILEDO='A'; }
               }

               // добавление к буферу дополнительного файла конфигурации
               if(stricmp  (CURCMD,"INCLUDE")==0){
                  if(access(CURFLD,4)==0){
                     // открыть указанный во втором поле файл
                     if((hf=open(CURFLD, O_RDONLY | O_BINARY)) >=0){
                         hi=read(hf, READBUI, RDBUFSZ); close(hf);
                         if(hb+hi<RDBUFSZ && hi>0){
                            READBUI[hi]=0;
                            strcat(READBUF, READBUI); 
                            hb=hb+strlen(READBUI);
                         }
                     }
                  }
               } 
            }
            //----------------------------------------------------------------
            if(f==2){ ;; }
            //----------------------------------------------------------------
            if(f==3){ ;; }
            //----------------------------------------------------------------
            if(f==4){ ;; }
            //----------------------------------------------------------------
            if(f==5){ ;; }
            //----------------------------------------------------------------
            if(f==6){ ;; }
            //----------------------------------------------------------------
            if(f==7){ ;; }
            //------------------------------------------------------------------
            // Отдельно выполняемые комманды
            if(stricmp(CURCMD,"CMD")==0){
               CMDPP=atof(CURFLD);
                   i=atoi(CURFLD);
               if(f==1){  // порядковый номер строки комманды
                  if(i>0  && i<CMDMAX) CMDRR=i;
               }
               if(f==2){  // номер соответствия выполняемой комманды
                  if(CURFLD[0]>32 && CURFLD[0]<255) CMDRN[CMDRR][0]=(int)CURFLD[0];
               }
               if(f==3){  // номер канала над которым выполняется комманда
                  if(i>=0 && i<DBBSZ) CMDRN[CMDRR][1]=i;
               }
               if(f==4){  // значение второго аргумента комманды
                  CMDRN[CMDRR][2]=CMDPP;
               }
            }
            //------------------------------------------------------------------
            // Условия вызова отдельно выполняемых комманд
            if(stricmp(CURCMD,"IF")==0){
               CMDPP=atof(CURFLD);
                   i=atoi(CURFLD);
               if(f==1){  // номер переменной (канала)
                  if(i>=0 && i<DBBSZ) CMDIF[CMDNN][0]=i;
               }
               if(f==2){  // знак
                  if(CURFLD[0]=='#') CMDIF[CMDNN][1]=1; if(CURFLD[0]=='!') CMDIF[CMDNN][1]=2;
                  if(CURFLD[0]=='>') CMDIF[CMDNN][1]=3; if(CURFLD[0]=='<') CMDIF[CMDNN][1]=4;
               }
               if(f==3){  // значение
                  CMDIF[CMDNN][2]=CMDPP;
               }
               if(f==4){  // номер комманды
                  if(CMDPP>0 && CMDPP<CMDMAX && CMDRN[i][0]>0){ 
                     CMDIF[CMDNN][3]=CMDPP;
                     if(CMDNN<CMDMAX-1) CMDNN++;
                  }
               }
            }
            //------------------------------------------------------------------
            // Переопределение типа переменных
            if(stricmp(CURCMD,"VTSET")==0){
               if(f==1){
                  i=atoi(CURFLD); if(i>=0 && i<100){ VTOLD[VTCUR]=i; }
                  VTNEW[VTCUR]=VTOLD[VTCUR];
               }
               if(f==2){
                  i=atoi(CURFLD); 
                  if(i>=0 && i<100 && VTOLD[VTCUR]!=i){ 
                     VTNEW[VTCUR]=i;
                     if(VTCUR<VTSETMAX-1){ VTCUR++; }
                  }
               }
            }
            //------------------------------------------------------------------
            // если это комманда заполнения массива DBB
            if(stricmp (CURCMD, "DBB")==0 && f>0){ 
               // номер элемента 
               if(f==1){
                  CURFLD[4]=0;
                  DBBC=atoi(CURFLD); if(DBBC<0||DBBC>=DBBSZ) DBBC=0;;
               }
               // номер элемента ручного преобразования
               if(f==3){
                  CURFLD [4]=0; i=atoi(CURFLD); if(i<0||i>=DBBSZ) i=0;;
				  DBMN[DBBH]=i;
               }
               // тип канала
               if(f==REMFLD-3){
  		          if(CURFLD[0]>0) DBBT[DBBH][0]=CURFLD[0];
				  if(CURFLD[1]>0) DBBT[DBBH][1]=CURFLD[1];
                  vcc[0]=CURFLD[2];vcc[1]=CURFLD[3];vcc[2]=0;
				  i=atoi(vcc);
				  if(i>0 && i<100){ VTCL[DBBH]=i; }
				  else            { VTCL[DBBH]=4; }
                  // проверка и переопределение типа
                  for(i=0; i<VTSETMAX; i++){
                      if(VTCL[DBBH]==VTOLD[i] && VTOLD[i]!=VTNEW[i]){
                         VTCL[DBBH]= VTNEW[i]; break;
                      }
                  }
			   }
               // имя канала
               if(f==REMFLD-2){ 
			      i =DBBH;
                  if(DBBC>0 && DBBT[DBBH][1]=='O'){
                     DBBI[DBBH]=DBBC;  // DBBI[1]=1001; DBBI[2]=1002;
					 DBBO[DBBC]=DBBH;  // DBBO[1001]=1; DBBO[1002]=2;
					 ConvertCurField();
                     strcpy(DBBN[DBBH],CURFLD);
                     if(DBBH<ITEMMAX-1) DBBH++;;
                  } else {
                     DBBT[DBBH][1]=0; DBBI[DBBH]=0;
                  }
				  strcpy(DBRM[i],"");
               }
			   // строка комментария
               if(f>=REMFLD-1){ 
                  cs=cs;
               }
            }
            //----------------------------------------------------------------
            // переход к следующему полю
            f++;
            //----------------------------------------------------------------
         }
         // если это конец строки- обнулить флаги
         if(cs==10||cs==0) { 
            // если это последнее поле (комментарий) в комманде DBB
            if(f>REMFLD-2){
			   strcpy(DBRM[i],CURFLD); 
			}
			bs=0; b=0; f=0; r=0; continue; 
		 }
         // если это строка комментария
         if(cs==';'){ r=1; }
         // служебные символы - пропустить
         if(cs< 32 ||r==1) { continue; }
         // кроме поля примечания - пробелы не учитывать
         if((cs==32||cs=='=')&&(f<REMFLD)){ b=0; continue; }
         // заполнение буфера текщего поля
         if(b<LENFLD-1){ CURFLD[b]=cs; b++; CURFLD[b]=0; }; c=0;
     }
     //-------------------------------------------------------------------------

     return TRUE;
}
//------------------------------------------------------------------------------


//----------------------------------------------------------------------------
// Проверка размера LOG-файла
void CheckLogFileSize(void){
     int a=0;
     // C:=3, D:=4
     CurDiskFreeSpace=0;
     CurLogFileSize  =0;
     // определение свободного места на диске
     _getdiskfree(3,&gDSKSP);
     CurDiskFreeSpace= gDSKSP.bytes_per_sector    *
                       gDSKSP.sectors_per_cluster *
                       gDSKSP.avail_clusters;
     // определение размера лог-файла
     if((FileHD=open(LOGFILE,O_BINARY|O_RDONLY))>=0){
         CurLogFileSize=lseek(FileHD,0L,SEEK_END);
         close(FileHD);
     }
     // если лог-файл больше заданного размера
     if(CurDiskFreeSpace>0 && CurLogFileSize>0){
        if(CurLogFileSize>LOGMAXFILESZ){
           // если определено перезаписывать файл
           if(LOGMAXFILEDO=='O'){ unlink(LOGFILE); }
           // если определено создание резервной копии
           if(LOGMAXFILEDO=='B'){
              strcpy(MOVFILE,LOGFILE);
              for(a=0; a<(int)strlen(MOVFILE); a++){
                 if(MOVFILE[a]=='.') MOVFILE[a]='_';
              }; strcat(MOVFILE,".BAK");
              unlink(MOVFILE); MoveFile(LOGFILE,MOVFILE); 
           }
        }
     }
     return;
}
//----------------------------------------------------------------------------
// Добавление строк в логи
void AddLogStr(char * GetSTR, int mode){
  	 int   cc=myLLogV.GetItemCount(); 
  	 GetCurTime();
  	 sprintf(LogSTR,"%s  %s",TimeStr,GetSTR);
  	 if(mode>0){
        if(cc>100){ myLLogV.DeleteItem(0); }
        myLLogV.InsertItem(cc,LogSTR);
     } else {
        myLLogV.GetItemText(cc-1,0,ssLL,LENSTR-1);
  	    sprintf(LogSTR,"%s%s",ssLL,GetSTR);
        myLLogV.SetItemText(cc-1,0,LogSTR);
     }
     if(CurDiskFreeSpace>LOGDISKFREE){
  	    ff=fopen(LOGFILE,"a+");
        if(ff!=NULL){ fprintf(ff,"%s  %s\n",TimeStr,GetSTR); fclose(ff); }
     }
  	 cc=myLLogV.GetItemCount(); 
     myLLogV.EnsureVisible(cc-1,FALSE);
     myLLogV.SendMessage(WM_PAINT,0,0);
//   CListCtrl::
     return;
}
//----------------------------------------------------------------------------
void WRStat(int Sttt){
       sprintf(Tmp1Str,"Контроль работы программы %d сек",Sttt);
     AddLogStr(Tmp1Str,1);
     return;
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// Вывод данных на экран
void  ViewValues(void){
      int    a=0; 
	  LPVOID pF;
	  va=0, vb=0, vi=0, vx=0, vy=0, vv=0;
      vf=0;
      strcpy(vvtcl,""); strcpy(vvtsr,""); strcpy(vcc,"");
      if(InitDefFL>0){
         for(vi=0; vi<DBBH; vi++){
             va=DBBI[vi]; if(va<0 || va>=DBBSZ  ){ continue; }
		     vx=DBBO[va]; if(vx<0 || vx>=ITEMMAX){ continue; }
		     if (ItemYES[vi]>=0 || VTCL[vx]>90){
				    // преобразование к своему типу (VT_MYMAN...)
				    // (в DBMN[] номер канала от которого делаем преобразование)
                    if(VTCL[vx]==VT_MYMANTIME && DBBI[vx] != DBMN[vx]){
					   vb=DBMN[vx]; if(vb<0 || vb>=DBBSZ  ){ vv++; continue; }
					   vy=DBBO[vb]; if(vy<0 || vy>=ITEMMAX){ vv++; continue; }
		               VariantTimeToSystemTime(DBBD[vb], &mSTM);
				       sprintf(DBBV[va],"%02d%02d%02d",mSTM.wHour,mSTM.wMinute,mSTM.wSecond );
					   strcpy(DBQA[vx],DBQA[vy]); strcpy(DBVT[vx],DBVT[vy]); DBBA[vx]=DBBA[vy];
                       strcpy(DBTM[vx],DBTM[vy]); strcpy(DBRM[vx],DBRM[vy]);
					}
					GetVTSTR(VTCL[vx]); strcpy(vvtcl,VTSTR);
					GetVTSTR(VTSR[vx]); strcpy(vvtsr,VTSTR);
					if(strlen(DBBV[va])<1) sprintf(DBBV[va],"%9.2f",1); DBBV[va][9]=0;
					vvtcl[2]=0; vvtsr[2]=0; DBVT[vx][2]=0;
			        sprintf(vcc,"%d",DBBI[vx]);            // вывод номеров каналов
                    myLView.SetItemText (vv, 1, vcc);
                    myLView.SetItemText (vv, 2, DBBN[vx]); // вывод названий каналов
					myLView.SetItemText (vv, 3, DBBV[va]); // возвращенное значение
                    myLView.SetItemText (vv, 4, vvtcl);    // тип который запрашивал клиент
                    myLView.SetItemText (vv, 5, vvtsr);    // тип который оределен на сервере
                    myLView.SetItemText (vv, 6, DBVT[vx]); // тип с которым пришел ответ
			        sprintf(vcc,"%d",DBBA[vx]);
                    myLView.SetItemText (vv, 7, vcc);      // количество ответов от сервера
                    myLView.SetItemText (vv, 8, DBQA[vx]); // достоверность переменной
                    myLView.SetItemText (vv, 9, DBTM[vx]); // время переменной на сервере
                    myLView.SetItemText (vv,10, DBRM[vx]); // комментарий
					vv++;
                	DBFM[vx]=0;
//					CListCtrl::
             }
         }
		 myLStat.SetItemText (15, 1, ReadStat);
		 myLStat.SetItemText (15, 2, LTMRead );
		 myLStat.SetItemText (16, 1, WritStat);
		 myLStat.SetItemText (16, 2, LTMWrit );
      }
      // Записать счетчик работы программы в область памяти контроля
      PM_i++; if(PM_i>10000 || PM_i<0) PM_i=0; sprintf(PM_s,"%.0f",(float)PM_i); PM_s[9]=0;
 	  a=(int)PM_pF+(PM_CPCNUM-1)*10;
	  pF=(void *)a;
      memcpy(pF,&PM_s,10);
	  PM_s[9]=0;
}
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE QueryInterface( 
          /* [in] */ REFIID riid,
          /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject)
{
        *ppvObject=NULL;
        if(riid==IID_IUnknown){
           *ppvObject=(IUnknown *)(IOPCDataCallback *)this;
            return S_OK;
        }
        if(riid==IID_IOPCDataCallback){
           *ppvObject=static_cast<IOPCDataCallback *>(this);
            return S_OK;
        }
        return E_NOTIMPL;
}
//----------------------------------------------------------------------------
ULONG STDMETHODCALLTYPE AddRef(void){ 
      return ++ref; 
}
//----------------------------------------------------------------------------
ULONG STDMETHODCALLTYPE Release(void){ 
      return --ref; 
}
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
// Синхронная запись (переменные автоклавов начинаются с 1000)
HRESULT  STDMETHODCALLTYPE  SyncWrite(int CHN, double CHV){
	swi=0,  swa=0,  swx =0,  swz=0;
    // если есть добавленные каналы
    if(InitDefFL>0){
	   // для каждого канала
       if(CHN>=0 && CHN<DBBSZ){
          swx=DBBO[CHN]; if(swx<0 || swx>=ITEMMAX){ return S_OK; }
          for(swi=0; swi<DBBR; swi++){
              if((int)ItemDef[swi].hClient==CHN) break;;
          }
          // если определен синхронный интерфейс
          if(lp_IOPCItemMgt != NULL && lp_IOPCSyncIO!=NULL && DBBR>0 && swi<DBBR){
             if(VTSR[swx]>1 && VTSR[swx]<27){
			     swVarVALUE[0].vt= VTSR[swx];
             } else {
			     swVarVALUE[0].vt= VT_EMPTY;
             }
			 swVarVALUE[0].dblVal= (double) CHV;
			 swVarVALUE[0].iVal  = (int)    CHV;
			 swVarVALUE[0].lVal  = (long)   CHV;
             if(swVarVALUE[0].vt== VT_R4){
			    swVarVALUE[0].fltVal= (float)CHV;
             }
             hrL=lp_IOPCSyncIO->Write(1, &phServer[swi], swVarVALUE, &pErrorl); swz=1;
		  }
       }
    }
    if(swz>0){
	   GetCurTime(); strcpy(LTMWrit,TimeStr); Cnt_Writ++; if(Cnt_Writ>10000){ Cnt_Writ=0; }
	}
    sprintf(WritStat,"Count: %d ",Cnt_Writ);
	return S_OK;
}
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
// Синхронное чтение (переменные автоклавов начинаются с 1000)
HRESULT  STDMETHODCALLTYPE  SyncRead(void){
      sra=0, sri=0, srz=0, srx=0, srCnt_One=0;
      srf=0;
      HCVR=0;
      hrL=NULL,  pItemStateL=NULL;
      // если определен синхронный интерфейс
      if(lp_IOPCItemMgt != NULL && lp_IOPCSyncIO!=NULL && ADATA_S>0 && DBBR>0){
         if(WHENCEDATA=='C'){
            hrL=lp_IOPCSyncIO->Read(OPC_DS_CACHE, DBBR, phServer, &pItemStateL, &pErrorl);;
         }
         if(WHENCEDATA=='D'){
            hrL=lp_IOPCSyncIO->Read(OPC_DS_DEVICE,DBBR, phServer, &pItemStateL, &pErrorl);;
         }
         if (SUCCEEDED(hrL)){
             for(sri=0; sri<DBBR; sri++){
                 sra=(int)(ItemDef[sri].hClient); 
				 srz=0;
                 if(sra>1000 && sra<DBBSZ){
					srx=DBBO[sra]; if(srx<0 || srx>=ITEMMAX){ continue; }
                    GetQASTR((int)(pItemStateL[sri].wQuality));
                    strcpy(DBQA[srx],QASTR);
		            srtimestamp = CTime::CTime(pItemStateL[sri].ftTimeStamp);
                    srtmpstr= srtimestamp.Format("%d.%m.%y  %H:%M:%S  ");
				    strcpy(DBTM[srx],srtmpstr);
					GetVTSTR(V_VT(&pItemStateL[sri].vDataValue)); 
					strcpy(DBVT[srx],VTSTR);
					// обработка и преобразование известных типов
                    if (V_VT(&pItemStateL[sri].vDataValue)==VT_I2){
                        srf=(float)V_I2(&pItemStateL[sri].vDataValue); srz=1;
                    }
                    if (V_VT(&pItemStateL[sri].vDataValue)==VT_I4){
                        srf=(float)V_I4(&pItemStateL[sri].vDataValue); srz=1;
                    }
                    if (V_VT(&pItemStateL[sri].vDataValue)==VT_R4){
                        srf=(float)V_R4(&pItemStateL[sri].vDataValue); srz=1;
                    }
                    if (V_VT(&pItemStateL[sri].vDataValue)==VT_R8){
                        srf=(float)V_R8(&pItemStateL[sri].vDataValue); srz=1;
                    }
                    if (V_VT(&pItemStateL[sri].vDataValue)==VT_DATE){
						DBBD[sra]=V_DATE(&pItemStateL[sri].vDataValue);
			            VariantTimeToSystemTime(DBBD[sra], &mSTM);
						srz=2;
					}
                    // передать значение в массив данных по номеру ItemDef
                    if (srz>0){
						DBFM[srx]=1;
						if(DBBA[srx]<1000){ DBBA[srx]++; }
						else              { DBBA[srx]=0; }
			            switch (srz){
						    case 1: { 
								      DBBD[sra]=srf; 
                                      sprintf(tsDBBS,"%9f",srf); 
                                      tsDBBS[9]=0;
                                      strcpy(DBBV[sra],tsDBBS);
									  break; 
									};
				            case 2: {
					                  sprintf(tsDBBS,"%04d%02d%02d",mSTM.wYear,mSTM.wMonth, mSTM.wDay);
                                      tsDBBS[8]=0;
                                      strcpy(DBBV[sra],tsDBBS);
					                  break;
									}
						}; DBBV[sra][DBBLN-1]=0;
						HCVR++;
						srCnt_One++;
                    }
                 }
             }
             Cnt_Call++; if(Cnt_Call>1000000) Cnt_Call=0;;
         }
      }
      if(srz>0){
	     GetCurTime(); strcpy(LTMRead,TimeStr);
         sprintf(ReadStat,"SyncRead  Count All: %d   Count One: %d", Cnt_Call, srCnt_One);
      }
	  return S_OK;
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// Процедура приёма значений
HRESULT     STDMETHODCALLTYPE OnDataChange( 
/* [in] */          DWORD       dwTransid,
/* [in] */          OPCHANDLE   hGroup,
/* [in] */          HRESULT     hrMasterquality,
/* [in] */          HRESULT     hrMastererror,
/* [in] */          DWORD       dwCount,
/* [size_is][in] */ OPCHANDLE   __RPC_FAR   *phClientItems,
/* [size_is][in] */ VARIANT     __RPC_FAR   *pvValues,
/* [size_is][in] */ WORD        __RPC_FAR   *pwQualities,
/* [size_is][in] */ FILETIME    __RPC_FAR   *pftTimeStamps,
/* [size_is][in] */ HRESULT     __RPC_FAR   *pErrors)
{
   int         a=0, i=0, z=0, x=0, Cnt_One=0;
   float       f=0;
   // если флаг доступа к данным через подписку не установлен - выйти
   if(ADATA_U==0) return S_FALSE;;
   HCVR=0;
   // Принять данные из области обмена между программами в массив DBBV
   memcpy(&DBBV,PGCB,DBBSZ*DBBLN);
   for(i=0;i<(int)dwCount;i++){
       a =phClientItems[i];
	   z = 0;
       if(a>1000 && a<DBBSZ){
		  x=DBBO[a]; if(x<0 || x>=ITEMMAX){ continue; }
          GetQASTR((int)(pwQualities[i]));
          strcpy(DBQA[x],QASTR);
          odtimestamp = CTime::CTime(pftTimeStamps[i]);
          odtmpstr= odtimestamp.Format("%d.%m.%y  %H:%M:%S  ");
		  strcpy(DBTM[x],odtmpstr);
		  // записать тип полученной переменной
		  GetVTSTR(V_VT(&pvValues[i])); 
		  strcpy(DBVT[x],VTSTR);
          // обработка и преобразование известных типов
		  if (V_VT(&pvValues[i])==VT_I2){
              f=(float)V_I2(&pvValues[i]); z=1;
          }
          if (V_VT(&pvValues[i])==VT_I4){
              f=(float)V_I4(&pvValues[i]); z=1;
          }
          if (V_VT(&pvValues[i])==VT_R4){
              f=(float)V_R4(&pvValues[i]); z=1;
          }
          if (V_VT(&pvValues[i])==VT_R8){
              f=(float)V_R8(&pvValues[i]); z=1;
          }
          if (V_VT(&pvValues[i])==VT_DATE){
			  DBBD[a]=V_DATE(&pvValues[i]);
			  VariantTimeToSystemTime(DBBD[a], &mSTM);
			  z=2;
		  }
          // передать значение в массив данных по номеру ItemDef
          if(z>0){
			 DBFM[x]=1;
             if(DBBA[x]<10000){ DBBA[x]++; }
             else             { DBBA[x]=0; }
			 switch (z){
			    case 1: { 
                          DBBD[a]=f; 
                          sprintf(tuDBBS,"%9f", f);
                          tuDBBS[9]=0;
                          strcpy(DBBV[a],tuDBBS);
                          break; 
                        };
				case 2: {
					      sprintf(tuDBBS,"%04d%02d%02d",mSTM.wYear,mSTM.wMonth, mSTM.wDay);
                          tuDBBS[8]=0;
                          strcpy(DBBV[a],tuDBBS);
					      break;
						}
			 }; DBBV[a][DBBLN-1]=0;
			 HCVR++;
			 Cnt_One++;
          }
       }
   }
   // Передать данные из массива DBBV в область обмена между программами
   memcpy(PGCB,&DBBV,DBBSZ*DBBLN);
   Cnt_Call++; if(Cnt_Call>10000) Cnt_Call=0;;
   if(z>0){
      GetCurTime(); strcpy(LTMRead,TimeStr);
      sprintf(ReadStat,"Subscribe  Count All: %d   Count One: %d", Cnt_Call, Cnt_One);
   }
   return S_OK;
}
//----------------------------------------------------------------------------
      

//----------------------------------------------------------------------------
//  Инициализация OPC сервера и данных каналов
void   InitOPCCL(void){
      
        SetUserDefaults();

        ioa=0, iok=0, ioi=0, iol=0, iox=0, iov=0, ioz=0;
        strcpy(iotmpWstr,""); strcpy(iotmpWstr1,""); strcpy(iocc,"");

        iohr   = NULL;
        ioOGRP = NULL;

        prDeadBand  = 0;        // [in]  AddGroup
        revUpdRate  = 0;        // [out] AddGroup
        m_hServerGroup = NULL;  // [out] AddGroup

        iosin.dwReserved1=0;    // DWORD
        iosin.pwszName=NULL;    // LPWSTR
        iosin.pAuthInfo=NULL;   // COAUTHINFO __RPC_FAR * 
        iosin.dwReserved2=0;    // DWORD

        iomqi[0].pIID=&IID_IOPCServer;
        iomqi[0].pItf=NULL;
        iomqi[0].hr=0;

        myLView.DeleteAllItems();

        strcpy(ES_AIT1,"");
        strcpy(ES_SPAR,"");
        if(Cnt_Initialize<10000){ Cnt_Initialize++; }
		sprintf(Cnt_Stat,"Count: %d",Cnt_Initialize);
		GetCurTime();
        myLStat.SetItemText ( 0, 1, "");        myLStat.SetItemText ( 0, 2, "");
        myLStat.SetItemText ( 1, 1, "");        myLStat.SetItemText ( 1, 2, "");
        myLStat.SetItemText ( 2, 1, "");        myLStat.SetItemText ( 2, 2, "");  
        myLStat.SetItemText ( 3, 1, "");        myLStat.SetItemText ( 3, 2, "");
        myLStat.SetItemText ( 4, 1, "");        myLStat.SetItemText ( 4, 2, "");
        myLStat.SetItemText ( 5, 1, "");        myLStat.SetItemText ( 5, 2, "");
        myLStat.SetItemText ( 6, 1, "");        myLStat.SetItemText ( 6, 2, "");
        myLStat.SetItemText ( 7, 1, "");        myLStat.SetItemText ( 7, 2, "");
        myLStat.SetItemText ( 8, 1, "");        myLStat.SetItemText ( 8, 2, "");
        myLStat.SetItemText ( 9, 1, "");        myLStat.SetItemText ( 9, 2, "");
        myLStat.SetItemText (10, 1, "");        myLStat.SetItemText (10, 2, "");
        myLStat.SetItemText (11, 1, "");        myLStat.SetItemText (11, 2, "");
        myLStat.SetItemText (12, 1, "");        myLStat.SetItemText (12, 2, "");
        myLStat.SetItemText (13, 1, "");        myLStat.SetItemText (13, 2, "");
        myLStat.SetItemText (14, 1, Cnt_Stat);  myLStat.SetItemText (14, 2, TimeStr);
        myLStat.SetItemText (15, 1, "");        myLStat.SetItemText (15, 2, "");
        myLStat.SetItemText (16, 1, "");        myLStat.SetItemText (16, 2, "");

        myLStat.SetItemText ( 0, 1, OPCSRVNAME);
		if(ADATA_S>0){ strcat(ES_SPAR,"SyncRead  " ); }
		if(ADATA_U>0){ strcat(ES_SPAR,"SubscribeRead  "); }
		if(ADATA_S>0){ 
		   if(strlen(ES_SPAR)<500 && WHENCEDATA=='D'){ strcat(ES_SPAR,"DEVICE "); }
		   if(strlen(ES_SPAR)<500 && WHENCEDATA=='C'){ strcat(ES_SPAR,"CACHE " ); }
		}
		if(strlen(ES_SPAR)<500){
		   sprintf(iocc,"CheckSrv=%ds ",CHECKSERVER); strcat(ES_SPAR,iocc); 
		}
		if(strlen(ES_SPAR)<500){
		   sprintf(iocc,"MEM=%s ",GLMEMNAME); strcat(ES_SPAR,iocc);
		}

		myLStat.SetItemText ( 0, 2, ES_SPAR); 
		myLStat.SendMessage(WM_PAINT,0,0); 

        // если интерфейсы были активизированы- освободить их
        if(lp_IOPCServer!=NULL && lp_IOPCItemMgt!=NULL){
            DestroyFLG=1; OnDestroy();
        }

        //--------------------------------------------------------------------
		// проверка доступа к OPC серверу по IP
		sprintf(Cnt_Stat,"Not Used");
		myLStat.SetItemText( 1, 1, OPCSRVADDR); 
		myLStat.SendMessage(WM_PAINT,0,0);
		myLStat.SetItemText ( 1, 2, Cnt_Stat);
		myLStat.SendMessage(WM_PAINT,0,0); Sleep(50);
        //--------------------------------------------------------------------

        pErrorm                     = NULL;
        lp_IOPCServer               = NULL;
        lp_IOPCBrowse               = NULL;
        lp_IOPCItemMgt              = NULL;
        lp_IOPCSyncIO               = NULL;
        lp_IConnectionPoint         = NULL;
        lp_IConnectionPointContainer= NULL;

        ioclsctx         = CLSCTX_LOCAL_SERVER;
        if(C_IOCLSCTX==1){ ioclsctx = CLSCTX_REMOTE_SERVER; }
        if(C_IOCLSCTX==2){ ioclsctx = CLSCTX_ALL; }

        //--------------------------------------------------------------------
        // Ввод имени компьютера (адреса сервера)
        MultiByteToWideChar(GetACP(),MB_PRECOMPOSED,OPCSRVADDR, -1,(unsigned short *)iotmpWstr,200);
        iosin.pwszName    = (unsigned short *)iotmpWstr;
        iosin.pAuthInfo   = NULL;
        // Ввод имени сервера
        MultiByteToWideChar(GetACP(),MB_PRECOMPOSED,OPCSRVNAME, -1,(unsigned short *)iotmpWstr1,200);
        CLSIDFromProgID((BSTR)iotmpWstr1,&ioclsid);
        //--------------------------------------------------------------------
  	    AddLogStr("Инициализация DCOM:",1);
  	    AddLogStr("CoInitializeEx(NULL,COINIT_MULTITHREADED)...",1);
        // инициализировать DCOM
        iohr = CoInitializeEx(NULL,COINIT_MULTITHREADED);
		HRFlag=iohr; 
        GetHRFlag();
		sprintf(CoInStat,"%s",HRStat); myLStat.SetItemText ( 2, 1, CoInStat);
		if(!FAILED(iohr)){ AddLogStr("OK",0);    myLStat.SetItemText(2,2,"Ok");    }
		else             { AddLogStr("ERROR",0); myLStat.SetItemText(2,2,"Error"); }
		myLStat.SendMessage(WM_PAINT,0,0); Sleep(50);
        //--------------------------------------------------------------------
		  sprintf(Tmp1Str,"CoCreateInstanceEx(ioclsid, NULL,%s,&iosin,1,iomqi)...",
			  ((ioclsctx==CLSCTX_LOCAL_SERVER)?"CLSCTX_LOCAL_SERVER":
			   (ioclsctx==CLSCTX_ALL)?"CLSCTX_ALL":
			   (ioclsctx==CLSCTX_REMOTE_SERVER)?"CLSCTX_REMOTE_SERVER":"?????")
			  );
  	    AddLogStr(Tmp1Str,1);
		iohr = CoCreateInstanceEx(ioclsid, NULL, ioclsctx, &iosin,   1, iomqi);
		HRFlag=iohr; 
        GetHRFlag();
		sprintf(CoCrStat,"%s",HRStat); myLStat.SetItemText ( 3, 1, CoCrStat);
        if(FAILED(iohr)){ AddLogStr("ERROR",0); myLStat.SetItemText(3,2,"Error"); }
		else            { AddLogStr("OK",0);    myLStat.SetItemText(3,2,"Ok");    }
		myLStat.SendMessage(WM_PAINT,0,0);
        if(FAILED(iohr)){ return; }
        //--------------------------------------------------------------------
        // после инициализации DCOMа здесь лежит указатель на интерфейс [0]
        lp_IOPCServer=(IOPCServer *) iomqi[0].pItf;
        // проверить интерфейс
		HRFlag=NOTDEF;
        AddLogStr("Проверка интерфейса сервера:",1);
		AddLogStr("lp_IOPCServer->QueryInterface(IID_IOPCBrowseServerAddressSpace,&lp_IOPCBrowse)...",1);
        if (lp_IOPCServer != NULL){
            iohr = lp_IOPCServer->QueryInterface(IID_IOPCBrowseServerAddressSpace,(LPVOID*)&lp_IOPCBrowse);
		    HRFlag=iohr;
        }
        GetHRFlag(); sprintf(QIBrStat,"%s",HRStat); myLStat.SetItemText( 4, 1, QIBrStat);
		myLStat.SendMessage(WM_PAINT,0,0); Sleep(50);
        if(FAILED(iohr)){ AddLogStr("ERROR",0); myLStat.SetItemText(4,2,"Error"); }
		else            { AddLogStr("OK",0);    myLStat.SetItemText(4,2,"Ok");    }
        //--------------------------------------------------------------------
        // добавить группу
  	    AddLogStr("Добавление группы:",1);
  	    AddLogStr("IOPCServer->AddGroup(",1);
		  sprintf(Tmp1Str,"    %s, TRUE, %d, 0, 0, HSG, updRate, IID_IOPCItemMgt, &lp_IOPCItemMgt",GROUPNAME,m_updRate);
  	    AddLogStr(Tmp1Str,1);
  	    AddLogStr(")...", 1);
        iol=strlen(GROUPNAME); mbstowcs(iosg,GROUPNAME,iol); 
        ioOGRP=SysAllocString(iosg); ioOGRP[iol]=0;
        iohr=lp_IOPCServer->AddGroup(
            ioOGRP,          // [in]  Server name, if NULL OPCServer will generate a unique name
            TRUE,            // [in]  State of group to add
            m_updRate,       // [in]  Requested update rate for group (ms)
            0,               // [in]  Client handle to OPC Group
            0,               // [in]  Time Bias
           &prDeadBand,      // [in]  Percent Deadband
            0,               // [in]  Localization ID
           &m_hServerGroup,  // [out] Server Handle to group
           &revUpdRate,      // [out] Revised update rate
            IID_IOPCItemMgt,
           (LPUNKNOWN*)(&lp_IOPCItemMgt) ); // [out] where to store the interface pointer

		HRFlag=iohr; GetHRFlag();
        sprintf(AddGStat,"%s",HRStat); myLStat.SetItemText ( 5, 1, AddGStat);
        if(!FAILED(iohr)){ AddLogStr("OK",0);     myLStat.SetItemText(5,2,"Ok");    }
		else             { AddLogStr("ERROR",0);  myLStat.SetItemText(5,2,"Error"); }
		myLStat.SendMessage(WM_PAINT,0,0); Sleep(50);
        if( FAILED(iohr)){ DestroyFLG=1; OnDestroy(); return; }
        //--------------------------------------------------------------------
		  sprintf(Tmp1Str,"Запрашиваемая частота обновления: %d мсек",m_updRate);
  	    AddLogStr(Tmp1Str,1);
		  sprintf(Tmp1Str,"Частота обновления установленная на сервере: %d мсек",revUpdRate);
  	    AddLogStr(Tmp1Str,1);
		m_updRate=revUpdRate;
        //--------------------------------------------------------------------
        // инициализация синхронного интерфейса
  	    AddLogStr("Инициализация синхронного интерфейса:",1);
  	    AddLogStr("lp_IOPCItemMgt->QueryInterface(IID_IOPCSyncIO, &lp_IOPCSyncIO)...",1);
		HRFlag=NOTDEF;
        if(lp_IOPCItemMgt != NULL){
           iohr = lp_IOPCItemMgt->QueryInterface(IID_IOPCSyncIO, (LPVOID*)&lp_IOPCSyncIO);
	       HRFlag=iohr; 
        }
        GetHRFlag(); sprintf(QISyStat,"%s",HRStat); myLStat.SetItemText ( 6, 1, QISyStat);
        if(!FAILED(iohr)){ AddLogStr("OK",0);     myLStat.SetItemText(6,2,"Ok");    }
		else             { AddLogStr("ERROR",0);  myLStat.SetItemText(6,2,"Error"); }
		myLStat.SendMessage(WM_PAINT,0,0); Sleep(50);
        //--------------------------------------------------------------------
        // инициализация интерфейса подписки
		HRFlag=NOTDEF;
		myLStat.SetItemText ( 7, 2, "Not Used");
        if(lp_IOPCItemMgt != NULL && ADATA_U>0){
  	       AddLogStr("Инициализация интерфейса подписки (SubScribe):",1);
  	       AddLogStr("lp_IOPCItemMgt->QueryInterface(IID_IConnectionPointContainer,&lp_IConnectionPointContainer)...",1);
           iohr=lp_IOPCItemMgt->QueryInterface(IID_IConnectionPointContainer,(LPVOID*)&lp_IConnectionPointContainer);
           if(FAILED(iohr)){ AddLogStr("ERROR",0); myLStat.SetItemText(7,2,"Error"); }
		   else            { AddLogStr("OK",   0); myLStat.SetItemText(7,2,"Ok");    }
	       HRFlag=iohr; 
        }
        GetHRFlag(); sprintf(QICPStat,"%s",HRStat); myLStat.SetItemText ( 7, 1, QICPStat);
		myLStat.SendMessage(WM_PAINT,0,0); Sleep(50);
        //--------------------------------------------------------------------
        
        
        //--------------------------------------------------------------------
		// если каналы для запроса не определены
        if(DBBH<1){ return; }
        //--------------------------------------------------------------------
        // если были определены каналы для запроса- удалить их
        HRFlag=NOTDEF;
        if (DBBR>0){
  	        AddLogStr("Удаление переменных(тэгов) из списка запроса:",1);
			  sprintf(Tmp1Str,"lp_IOPCItemMgt->RemoveItems(%d, phServer, &pErrorm)...",DBBR);
  	        AddLogStr(Tmp1Str,1);
			iohr=lp_IOPCItemMgt->RemoveItems(DBBR, phServer, &pErrorm);
	        HRFlag=iohr; 
            if(FAILED(iohr)){ AddLogStr("ERROR",0); myLStat.SetItemText(8,2,"Error"); }
		    else            { AddLogStr("OK",   0); myLStat.SetItemText(8,2,"Ok");    }
		} else {
			myLStat.SetItemText ( 8, 2, "Not Used");
        }
        GetHRFlag(); sprintf(RIT1Stat,"%s",HRStat); myLStat.SetItemText ( 8, 1, RIT1Stat);
		myLStat.SendMessage(WM_PAINT,0,0); Sleep(50);
        //--------------------------------------------------------------------

        //--------------------------------------------------------------------
		// обнуление переменных каналов
        pItemResult =new tagOPCITEMRESULT[ITEMMAX];
        pItemResult1=new tagOPCITEMRESULT[ITEMMAX];
        memset(pItemResult ,0,ITEMMAX);
        memset(pItemResult1,0,ITEMMAX);
        pErrorm = NULL;
        pErrorm1= NULL;
        for(ioi=0; ioi<DBBH; ioi++){
			ItemDef[ioi].hClient = 0;
	        ItemDef[ioi].bActive = FALSE;
			ItemDef[ioi].szItemID= NULL;
			phServer[ioi]=NULL;
            ItemYES[ioi]=-1;
		}
        //--------------------------------------------------------------------
            
        //--------------------------------------------------------------------
        // добавление каналов в структуру запроса
        for(ioi=0, iok=0, ioa=0; ioi<DBBH; ioi++){
            if(VTCL[ioi]<90){
			   iol=strlen(DBBN[ioi]);
               ItemDef[iok].hClient = DBBI[ioi];            // идентификаторы
               mbstowcs(iob, DBBN[ioi],iol);	            // имена каналов из char в OLECHAR
               ItemDef[iok].szItemID = SysAllocString(iob);
               ItemDef[iok].szItemID[iol]  = 0;
               ItemDef[iok].szAccessPath = L"";
               ItemDef[iok].bActive      = TRUE;
               ItemDef[iok].dwBlobSize   = 0;
               ItemDef[iok].pBlob        = NULL;
               ItemDef[iok].wReserved    = 0;
                  ItemDef[iok].vtRequestedDataType=VT_EMPTY;
               if(VTCL[ioi]>=0 && VTCL[ioi]<100){
                  ItemDef[iok].vtRequestedDataType=VTCL[ioi];
			   }
               iok++;
            } else { ioa++; }
        };
		  sprintf(Tmp1Str,"Определено переменных(тэгов) для запроса на сервере: %d",iok);
  	    AddLogStr(Tmp1Str,1);
		  sprintf(Tmp1Str,"Определено переменных(тэгов) для локальной обработки: %d",ioa);
  	    AddLogStr(Tmp1Str,1);
  	    AddLogStr("Добавление переменных(тэгов) в список запроса:",1);

        if(ADDITEMSALL>0){
		   sprintf(Tmp1Str,"lp_IOPCItemMgt->AddItems(%d, ItemDef, &pItemResult, &pErrorm)...",iok);
           AddLogStr(Tmp1Str,1);
           iohr=lp_IOPCItemMgt->AddItems(iok, ItemDef, &pItemResult, &pErrorm);
        } else {
           // Добавление по одному тэгу
           for(ioi=0; ioi<iok; ioi++){
               sprintf(Tmp1Str,"lp_IOPCItemMgt->AddItems(1, &ItemDef[%d], &pItemResult1, &pErrorm1)...[ID:%04d]...", 
                       ioi, ItemDef[ioi].hClient);
               AddLogStr(Tmp1Str,1);
               iohr=lp_IOPCItemMgt->AddItems(1, &ItemDef[ioi], &pItemResult1, &pErrorm1);
               if(FAILED(iohr)){ AddLogStr("ERROR",1); break; }
               pItemResult[ioi]=pItemResult1[0];
               AddLogStr("OK",0);
          }
        }
        HRFlag=iohr; 
        GetHRFlag(); sprintf(AIT1Stat,"%s",HRStat); myLStat.SetItemText ( 9, 1, AIT1Stat);
		myLStat.SendMessage(WM_PAINT,0,0); Sleep(50);
        //--------------------------------------------------------------------
        // проверка добавленных каналов
		// определение каналов действительно присутствующих на сервере
		DBBR=0;
        if(!FAILED(iohr)){
            if(ADDITEMSALL>0){ AddLogStr("OK",0); }
            for(ioi=0,  iok=0; ioi<DBBH; ioi++){
				VTSR[ioi]=0;
                ItemYES[ioi]=-1;
                if (VTCL[ioi]>90){ continue; }
                if (pItemResult[iok].hServer!=NULL && pItemResult[iok].hServer!=0){
                    phServer[iok] = pItemResult[iok].hServer;
                    ItemYES [ioi] = ioi;
                    VTSR[ioi]=pItemResult[iok].vtCanonicalDataType;
                    if(DBBR<DBBH){ DBBR++; }
                } else {
                    sprintf(iocc,"%04d ",DBBI[ioi]); iocc[5]=0;
				    if(strlen(ES_AIT1)<LENSTR-7){ strcat(ES_AIT1,iocc); }
                    sprintf(Tmp1Str,"Ошибка добавления ItemID: %d [%s]",DBBI[ioi],DBBN[ioi]);
				    AddLogStr(Tmp1Str,1);
                }
                SysFreeString(ItemDef[iok].szItemID); iok++;
            }
			if(strlen(ES_AIT1)==0){ strcpy(ES_AIT1,"Ok"); }
  	        myLStat.SetItemText (9,2, ES_AIT1);
        } else {
  	        AddLogStr("ERROR",0); myLStat.SetItemText (9,2,"Error");
		}
        pItemResult = NULL;
        pItemResult1= NULL;
        pErrorm     = NULL;
        pErrorm1    = NULL;

        if(DBBR<1 || ES_AIT1[0]!='O'){ return; }

        //--------------------------------------------------------------------
        // Если в конфигурационном файле определено повторное добавление тэгов
        if(ADDITEMS2>0)
        {
        //--------------------------------------------------------------------
        // удаление Items и повторное добавление только тех, что есть на сервере
  	    AddLogStr("Удаление переменных(тэгов) из списка запроса:",1);
		  sprintf(Tmp1Str,"lp_IOPCItemMgt->RemoveItems(%d, phServer, &pErrorm)...",iok);
  	    AddLogStr(Tmp1Str,1);
        iohr=lp_IOPCItemMgt->RemoveItems(iok, phServer, &pErrorm);
        HRFlag=iohr; 
        GetHRFlag(); sprintf(RIT2Stat,"%s",HRStat); myLStat.SetItemText (10,1,RIT2Stat);
        if(FAILED(iohr)){ AddLogStr("ERROR",0); myLStat.SetItemText(10,2,"Error"); }
        else            { AddLogStr("OK",   0); myLStat.SetItemText(10,2,"Ok");    }
		myLStat.SendMessage(WM_PAINT,0,0); Sleep(50);
        //-----------------------------------------------------------------

		pItemResult = NULL;
        pErrorm     = NULL;
        for(ioi=0;ioi<DBBH;ioi++){
			phServer[ioi]            = NULL;
			ItemDef [ioi].hClient    = 0;
	        ItemDef [ioi].bActive    = FALSE;
			ItemDef [ioi].szItemID   = NULL;
            ItemDef [ioi].dwBlobSize = 0;
            ItemDef [ioi].pBlob      = NULL;
            ItemDef [ioi].wReserved  = 0;
		}
        //-----------------------------------------------------------------
        for(ioi=0,DBBR=0; ioi<DBBH; ioi++){
            if (ItemYES[ioi]>=0)
			{
                ItemDef[DBBR].hClient = DBBI[ioi];
				iox=DBBO[DBBI[ioi]];
				iol=strlen(DBBN[iox]);
                mbstowcs(iob, DBBN[iox], iol);
                ItemDef[DBBR].szItemID = SysAllocString(iob);
                ItemDef[DBBR].szItemID[iol]  = 0;
                ItemDef[DBBR].szAccessPath = L"";
                ItemDef[DBBR].bActive      = TRUE;
                   ItemDef[DBBR].vtRequestedDataType=VT_EMPTY;
                if(VTCL[iox]>=0 && VTCL[iox]<100){
                   ItemDef[DBBR].vtRequestedDataType=VTCL[iox];
				}
                if(DBBR<DBBH) DBBR++;;
            } 
        }

  	    AddLogStr("Добавление переменных(тэгов) в список запроса:",1);
		  sprintf(Tmp1Str,"lp_IOPCItemMgt->AddItems(%d, ItemDef, &pItemResult, &pErrorm)...",DBBR);
  	    AddLogStr(Tmp1Str,1);

		iohr=lp_IOPCItemMgt->AddItems(DBBR, ItemDef, &pItemResult, &pErrorm);
        HRFlag=iohr; 
        GetHRFlag(); sprintf(AIT2Stat,"%s",HRStat); myLStat.SetItemText (11, 1, AIT2Stat);
		myLStat.SendMessage(WM_PAINT,0,0); Sleep(50);
		
		if (!FAILED(iohr)){
            for(ioi=0; ioi<DBBR; ioi++){
				iox=ItemDef[ioi].hClient;
                if (pItemResult[ioi].hServer!=NULL && pItemResult[ioi].hServer!=0){
                    phServer[ioi] = pItemResult[ioi].hServer;
                    if(pItemResult[ioi].vtCanonicalDataType>=0 && 
					   pItemResult[ioi].vtCanonicalDataType<90){
					   VTSR[ DBBO[iox] ]=pItemResult[ioi].vtCanonicalDataType;
					}
				}
                SysFreeString(ItemDef[ioi].szItemID);
            }
  	        AddLogStr("OK",0);    myLStat.SetItemText (11,2,"Ok");
        } else {
  	        AddLogStr("ERROR",0); myLStat.SetItemText (11,2,"Error");
		}
        delete pItemResult;

        if(DBBR<1){ return; }
        //--------------------------------------------------------------------
        }  // ADDITEMS2
        else 
        {
           myLStat.SetItemText (10,2,"Not Used");
           myLStat.SetItemText (11,2,"Not Used");
        }
        //--------------------------------------------------------------------
        if(pItemResult!=NULL){ pItemResult=NULL; }
        if(pErrorm    !=NULL){ pErrorm=NULL; }
        //--------------------------------------------------------------------
        // Инициализация ожидания ответов по подписке
        HRFlag=NOTDEF; GetHRFlag(); sprintf(FCPTStat,"%s",HRStat); sprintf(AdvsStat,"%s",HRStat);
		myLStat.SetItemText ( 12, 2, "Not Used");
		myLStat.SetItemText ( 13, 2, "Not Used");
        if(lp_IConnectionPointContainer != NULL && ADATA_U>0){
  	       AddLogStr("Инициализация ожидания ответа от сервера по подписке:",1);
           AddLogStr("lp_IConnectionPointContainer->FindConnectionPoint(IID_IOPCDataCallback,&lp_IConnectionPoint)...",1);
           //
           iohr=lp_IConnectionPointContainer->FindConnectionPoint(IID_IOPCDataCallback,&lp_IConnectionPoint);
           HRFlag=iohr; 
           GetHRFlag(); sprintf(FCPTStat,"%s",HRStat);
           if(!FAILED(iohr)){
		      AddLogStr("OK",0);    myLStat.SetItemText ( 12, 2, "Ok");
              AddLogStr("lp_IConnectionPoint->Advise((IUnknown *)(IOPCDataCallback *)this,&conCookie)...",1);
              //
              iohr=lp_IConnectionPoint->Advise((IUnknown *)(IOPCDataCallback *)this,&conCookie);
              if(!FAILED(iohr)){ AddLogStr("OK",0);    myLStat.SetItemText ( 13, 2, "Ok");    }
			  else             { AddLogStr("ERROR",0); myLStat.SetItemText ( 13, 2, "Error"); }
		   } else {
		      AddLogStr("ERROR",0); myLStat.SetItemText ( 12, 2, "Error");
		   }
           HRFlag=iohr; GetHRFlag(); sprintf(AdvsStat,"%s",HRStat);
        }
        myLStat.SetItemText ( 12, 1, FCPTStat);
        myLStat.SetItemText ( 13, 1, AdvsStat);
		myLStat.SendMessage(WM_PAINT,0,0); Sleep(50);
        //--------------------------------------------------------------------

        //--------------------------------------------------------------------
		// Начальное заполнение окна вывода списка
        for(ioi=0, iov=0, ioz=0; ioi<DBBH; ioi++){
			ioa=DBBI[ioi]; if(ioa<0 || ioa>DBBSZ  ){ continue; }
			iox=DBBO[ioa]; if(iox<0 || iox>ITEMMAX){ continue; }
			if(VTCL[iox]>90 || ItemYES[ioi]>=0){ myLView.InsertItem(0,""); }
		}

        for(ioi=0,iov=0,ioz=0; ioi<DBBH; ioi++){
			ioa=DBBI[ioi]; if(ioa<0 || ioa>=DBBSZ  ){ continue; }
			iox=DBBO[ioa]; if(iox<0 || iox>=ITEMMAX){ continue; }
			if(VTCL[iox]>90 || ItemYES[ioi]>=0){
			   sprintf(iocc,"%d",iov);
               myLView.SetItemText (iov, 0,iocc);
			   sprintf(iocc,"%d",DBBI[iox]);
               myLView.SetItemText (iov, 1,iocc);
               myLView.SetItemText (iov, 2,DBBN[iox]);
               myLView.SetItemText (iov, 3,"NA");
               myLView.SetItemText (iov, 4,"##");
               myLView.SetItemText (iov, 5,"##");
               myLView.SetItemText (iov, 6,"##");
               myLView.SetItemText (iov, 7,"0" );
               myLView.SetItemText (iov, 8,"NA");
               myLView.SetItemText (iov, 9,"##");
               myLView.SetItemText (iov,10,DBRM[iox]);
			   if(VTCL[iox]<90){ ioz++; }
			   iov++;
			}
		}
//		CListCtrl:
		myLView.SendMessage(WM_PAINT,0,0);
        //--------------------------------------------------------------------

        InitDefFL=1;
        InitOPCFL=1;
}
//----------------------------------------------------------------------------




//----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE OnReadComplete( 
            /* [in] */ DWORD dwTransid,
            /* [in] */ OPCHANDLE hGroup,
            /* [in] */ HRESULT hrMasterquality,
            /* [in] */ HRESULT hrMastererror,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *phClientItems,
            /* [size_is][in] */ VARIANT __RPC_FAR *pvValues,
            /* [size_is][in] */ WORD __RPC_FAR *pwQualities,
            /* [size_is][in] */ FILETIME __RPC_FAR *pftTimeStamps,
            /* [size_is][in] */ HRESULT __RPC_FAR *pErrors)
{
	return S_OK;
}
//----------------------------------------------------------------------------
        

//----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE OnWriteComplete( 
            /* [in] */ DWORD dwTransid,
            /* [in] */ OPCHANDLE hGroup,
            /* [in] */ HRESULT hrMastererr,
            /* [in] */ DWORD dwCount,
            /* [size_is][in] */ OPCHANDLE __RPC_FAR *pClienthandles,
            /* [size_is][in] */ HRESULT __RPC_FAR *pErrors)
{
	return S_OK;
}
//----------------------------------------------------------------------------
        

//----------------------------------------------------------------------------
HRESULT STDMETHODCALLTYPE OnCancelComplete( 
            /* [in] */ DWORD dwTransid,
            /* [in] */ OPCHANDLE hGroup)
{
	return S_OK;
}
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
// Dialog Data
        //{{AFX_DATA(COPCClientDlg)
	enum { IDD = IDD_OPCClient_DIALOG };
	//}}AFX_DATA

        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(COPCClientDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL


//----------------------------------------------------------------------------
// Implementation
protected:
        HICON m_hIcon;

        // Generated message map functions
        //{{AFX_MSG(COPCClientDlg)
        virtual BOOL OnInitDialog();
        afx_msg void OnPaint();
        afx_msg void OnDestroy();
        afx_msg void OnTimer(UINT nIDEvent);
        DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
 
        DECLARE_MESSAGE_MAP()

};
//----------------------------------------------------------------------------

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPCClientDLG_H__D481FC4C_2EA8_48DC_B154_BFD40584B2A0__INCLUDED_)
