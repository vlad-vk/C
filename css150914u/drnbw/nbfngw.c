// coding: cp866
//---------------------------------------------------------------------------
// Функции для работы с NETBIOS
#include "../include.h"
#include "../define.h"
#include "nbfngw.h"


//***************************************************************************
vd DebugINF(uc *LABSTR, uc rw, ui cb){
   if(Debug(30)>0){ 
      printf("%s",LABSTR);
      switch (WS_ncb[cb].ncb_command){
        case 0xA0: printf("ncb_command =[0xA0][SEND DATAGRAM]\n"); break;;
        case 0xA1: printf("ncb_command =[0xA1][RECV DATAGRAM]\n"); break;;
        default  : printf("ncb_command =[0x%0X]\n", WS_ncb[cb].ncb_command); break;;
      }
      printf("ncb_retcode =[0x%0X]\n",WS_ncb[cb].ncb_retcode );
      printf("ncb_num     =[0x%0X]\n",WS_ncb[cb].ncb_num     );
      printf("ncb_length  =[0x%0X]\n",WS_ncb[cb].ncb_length  );
      printf("ncb_length  =[DEC:%0d]\n",WS_ncb[cb].ncb_length);
      printf("              12345678901234567890\n");
      printf("ncb_callname=[%s]   \n",WS_ncb[cb].ncb_callname);
      printf("ncb_name    =[%s]   \n",WS_ncb[cb].ncb_name    );
      printf("ncb_cmd_cplt=[0x%0X]\n",WS_ncb[cb].ncb_cmd_cplt);
      printf("ncb_lana_num=[0x%0X]\n",WS_ncb[cb].ncb_lana_num);
//    if(rw=='r'||rw=='R'){ printf("\n--receive buffer -------\n"); }
//    if(rw=='s'||rw=='S'){ printf("\n--send buffer ----------\n"); }
//    if(rw=='r'||rw=='R'){ fwrite(&WS_RD[cb],LNCB,1,DebOut); }
//    if(rw=='s'||rw=='S'){ fwrite(&WS_SD[cb],LNCB,1,DebOut); }  
//    printf("\n------------------------\n");
      printf("\n");
   }
   return; 
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
uc  WS_NB__GetRetCCode(ui cncb){
    return(WS_ncb[cncb].ncb_retcode);
}
//---------------------------------------------------------------------------
uc  WS_NB__GetFinalCCode(ui cncb){
    return(WS_ncb[cncb].ncb_cmd_cplt);
}
//---------------------------------------------------------------------------
ui  WS_NB__GetNCBSize(ui cncb){
    return(WS_ncb[cncb].ncb_length);
}
//---------------------------------------------------------------------------
uc  WS_NB__GetNetworkN(ui cncb){
    return(WS_ncb[cncb].ncb_num);
}
//---------------------------------------------------------------------------
uc *WS_NB__GetCallName(ui cncb){
    return(WS_ncb[cncb].ncb_callname);
}
//---------------------------------------------------------------------------
uc *WS_NB__GetOurName(ui cncb){
    return(WS_ncb[cncb].ncb_name);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//  Обнуление блока указанного NCB
vd  WS_NB__ZeroNCB(ui cncb){
    memset( &WS_ncb[cncb], 0, sizeof(WS_ncb[cncb]) );
    return;
}
//---------------------------------------------------------------------------
//  Обнуление буферов указанного NCB
vd  WS_NB__ZeroBUR(ui cncb){
    memset( &WS_RD[cncb], 0, LNCBMAX );
    return;
}
//---------------------------------------------------------------------------
vd  WS_NB__ZeroBUS(ui cncb){
    memset( &WS_SD[cncb], 0, LNCBMAX );
    return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
vd WS_NB__ZeroSBF( uc WS_CurNCB ){
   _i  a=0;
   for(a=0;a<BUFSZ;a++){ WS_SD[WS_CurNCB][a]=0; }
   return;
}
//---------------------------------------------------------------------------
vd WS_NB__ZeroRBF( uc WS_CurNCB ){
   _i  a=0;
   for(a=0;a<BUFSZ;a++){ WS_RD[WS_CurNCB][a]=0; }
   return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//  Вызов выполнения комманды NetBIOS
_i  WS_NB__NetBios (ui cncb){
    return(Netbios((PNCB)&WS_ncb[cncb]));
}
//---------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  Определение доступных номеров LANA
int LanaEnum(vd){
    NCB ncb;
    printf("LanaEnum..");
    memset( &ncb, 0, sizeof(NCB) );
    ncb.ncb_command = NCBENUM;
    ncb.ncb_buffer = (PUCHAR) lenum;
    ncb.ncb_length = sizeof(lenum);
    if (Netbios(&ncb) != NRC_GOODRET){ return ncb.ncb_retcode; }
    printf("ok\n");
    return NRC_GOODRET;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//  Добавление имени в список имен сети
ui  WS_NB__AddName(uc *WS_name, ui cncb){
    _c WS_buf[LNAM];
    _i rc=0;
    errno=0;
    printf("NBAddName..");
    WS_NB__ZeroNCB(cncb);
    WS_name[LNAM-1]=0;
    strcpy(WS_buf, WS_name );
    while(strlen(WS_buf)<LNAM-1){ strcat(WS_buf," "); }
    strcpy(WS_ncb[cncb].ncb_name, WS_buf );
    WS_ncb[cncb].ncb_command = WS_NB_WAddName;
    if(ResRes==0){
       WS_ncb[cncb].ncb_lana_num = LAN_NUM[0];
       rc=WS_NB__NetBios(cncb);
       if(rc>0){ ResAdd=1; errno= rc; NSTAT=0; }
       else    { ResAdd=0; NumNAM[0]=WS_ncb[cncb].ncb_num; NSTAT=1; printf("ok\n"); }
    }
    return(errno);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//  Удаление имени из списка имен сети
ui  WS_NB__DeleteName(uc *WS_name, ui cncb){
    _c WS_buf[LNAM];
    _i rc=0;
    errno=0;
    WS_NB__ZeroNCB(cncb);
    WS_name[LNAM-1]=0;
    strcpy( WS_buf, WS_name );
    while( strlen(WS_buf)<LNAM-1){ strcat( WS_buf, " " ); }
    strcpy( WS_ncb[cncb].ncb_name, WS_buf );
    WS_ncb[cncb].ncb_command  = WS_NB_WDeleteName;
    WS_ncb[cncb].ncb_lana_num = LAN_NUM[0];
    rc=WS_NB__NetBios(cncb);
    if(rc>0){ ResDel=1; errno= rc; }
    else    { ResDel=0; }
    return(errno);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//  Установка имени вызываемой станции в текущем NCB
vd  WS_NB__SetCallName(_c *WS_name, ui cncb){
    _c WS_buf [LNAM+1];
       WS_name[LNAM-1]=0;
    strcpy( WS_buf, WS_name );
     while( strlen(WS_buf) < LNAM-1 ){ strcat( WS_buf, " " ); }
    strcpy( WS_ncb[cncb].ncb_callname, WS_buf );
    return;
}
//---------------------------------------------------------------------------
//  Установка имени вызываемой станции в текущем NCB
vd  WS_NB__SetOurName(_c *WS_name, ui cncb){
    _c WS_buf [LNAM+1];
       WS_name[LNAM-1]=0;
    strcpy( WS_buf, WS_name );
     while( strlen(WS_buf) < LNAM-1 ){ strcat( WS_buf, " " ); }
    strcpy( WS_ncb[cncb].ncb_name, WS_buf );
    return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//  Прием данных (датаграмм) из сети NetBIOS
vd  WS_NB__Receive(ui cncb){
    WS_NB__ZeroBUR(cncb);
    WS_ncb[cncb].ncb_buffer  = (PUCHAR)WS_RD[cncb];
    WS_ncb[cncb].ncb_length  = 504; 
    WS_ncb[cncb].ncb_num     = NumNAM[0];
//  WS_ncb[cncb].ncb_num     = 0xff;
    WS_ncb[cncb].ncb_command = WS_NB_ReceiveDatagram;
    if(lenum[0].howa>0){
       WS_ncb[cncb].ncb_lana_num= LAN_NUM[0];
       WS_NB__NetBios(cncb);
    }
    return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//  Посылка данных (датаграмм) в сети NetBIOS
vd  WS_NB__Send(ui cncb, uc *SCallName){
    WS_NB__ZeroNCB(cncb);
    WS_NB__SetCallName (SCallName,cncb);
    WS_NB__SetOurName  (NMWS,cncb);
    WS_ncb[cncb].ncb_buffer  = (PUCHAR)WS_SD[cncb];
    WS_ncb[cncb].ncb_length  = 504;
    WS_ncb[cncb].ncb_num     = NumNAM[0];
    WS_ncb[cncb].ncb_lana_num= LAN_NUM[0];
    WS_ncb[cncb].ncb_command = WS_NB_SendDatagram;
    DebugINF("\nSEND:---\n",'s',cncb);
    WS_NB__NetBios(cncb);
    return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//  Сброс установок NetBIOS для всех адаптеров
_i  WS_NB__Reset(ui cncb){
    _i rc=0;
    _i la=0;
    errno=0;
    // вывести на экран номера установленных адаптеров для сети NetBIOS
    printf("LANA NUMLIST: ");
    for(la=0; la<lenum[0].howa; la++){
        printf( "[%02d] ",lenum[0].lana[la]);
    }
    // установить первый номер адаптера для работы по-умолчанию
    LAN_NUM[0]=lenum[0].lana[0];
    if (NBWLANA>=0) LAN_NUM[0]=NBWLANA;
    printf("\nSet current LANA: [%02d]\n",LAN_NUM[0]);
    //
    printf("Function NBReset: Zero NCB and Data buffers.\n");
    WS_NB__ZeroNCB(cncb);
    WS_NB__ZeroBUS(cncb);
    WS_NB__ZeroBUR(cncb);
    printf("Function NBReset: Fill NCB.\n");
    WS_ncb[cncb].ncb_callname[0] = ucMaxSess;
    WS_ncb[cncb].ncb_callname[2] = ucMaxName;
    WS_ncb[cncb].ncb_callname[3] = uFirstName;
    WS_ncb[cncb].ncb_command     = WS_NB_WResetAdapter;
    WS_ncb[cncb].ncb_lana_num = LAN_NUM[0];
    printf("Function NBReset: Call Netbios...");
    rc=WS_NB__NetBios(cncb);
    if(rc>0){ ResRes=1; errno=rc; }
    else    { ResRes=0; }
    printf("ok\n");
    return(rc);
}
//---------------------------------------------------------------------------


//--------------------------------------------------------------------------
//  Проверка кодов завершения выполнения комманд NetBIOS
_i  Check_RFCode(uc CC, ui cb){
    _i r=0;
//  if(WS_NB__GetRetCCode  (cb)==0x00){ r++; }
//  if(WS_NB__GetFinalCCode(cb)==0x00){ r++; }
    if(WS_NB__GetRetCCode  (cb)!=0xff){ r++; }
    if(WS_NB__GetFinalCCode(cb)!=0xff){ r++; }
    return((r==2)?1:0);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//  Возвращает номер свободного NCB или -1, если свободного NCB нет
_i  WS_NB__AvNCB(vd){
    _i ab=0;
    // Делаем для каждого NCB
    for(ab=0; ab<MNCB; ab++){
       // Если находим свободный блок - выходим, возвращая номер этого блока
       if(Check_RFCode('A',ab)>0){ return(ab); }
    }
    return(-1);
}
//---------------------------------------------------------------------------

//----------------------------------------------------------------------------
_i  WS_NB__Init(vd){
    _i a=0;
    a=LanaEnum();
    if(a!=NRC_GOODRET) return(0);
    WS_NB__Reset(0);
    a=WS_NB__AddName(NMWS,0);
    return(1);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Удаление имени данной станции из списка сети NetBIOS
vd  Quit_NB(vd){
    WS_NB__DeleteName(NMWS,0);
    printf("Delete NetBIOS name [%s]: %c \n",NMWS,(ResDel==0)?'Y':'N');
    return;
}
//----------------------------------------------------------------------------

