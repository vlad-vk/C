// coding: cp866
//----------------------------------------------------------------------------
#define             LENFLD              100         // макс длинна строкового поля
#define             MAXRQ               255         // макс кол-во запросов
#define             LNAM                16          // длинна NetBIOS имени
#define             LNCB                504         // размер передаваемого пакета 

ex   _i             SRV_NUM[MWS];                   // номера контроллеров по порядку
ex   _i             RSRV[MTCP];                     // номера контроллеров для каждого канала
ex   _i             HOWC[MWS];                      // кол-во запрашиваемых каналов для каждого контроллера
ex   _i             RCHN[MWS+1][MTCP];              // номер канала на удаленном сервере
ex   _i             SCHN[MWS+1][MTCP];              // номер канала на локальном компьютере

ex struct  CB{
   uc   nm[SLEN];            // имя канала
   _i   ar;                  // признак записи значений канала в архив
   _i   dt;                  // тип драйвера   (0-лок,1-COM,2-NB,3-TCP)
   _i   dn;                  // номер порта,станции
   _i   dd;                  // номер переменной на удаленной станции
   _i   t;                   // тип переменной (1-вход,2-выход,3-вход/выход)
   _i   dc;                  // количество цифр после запятой
   _f   l;                   // нижняя граница
   _f   h;                   // верхняя граница
   uc   rm[SLEN];            // комментарий
   _f   v;                   // значение канала
}  CB[MCH];

_i                  UDSel=0;                        // счетчик паузы UDP посылок

//----------------------------------------------------------------------------
// переменные для запросов через UDP
#define             MBSZ                512         // максимальный размер буферов обмена
#define             WS_VERSION_REQD     0x0101      // номер версии WinSock
WSADATA             stWSAData;                      // номер подверсии библиотеки WinSock
SOCKET              hSockS[MWS+1];                  // сокет соединения
SOCKADDR_IN         rmaddr;                         // структура адреса удаленной машины
SOCKADDR_IN         myaddr;                         // структура адреса удаленной машины
uc                  SUBUF[MBSZ];                    // буфер передачи через UDP
uc                  RUBUF[MBSZ];                    // буфер приема через UDP
ui                  UDPRQ=1;                        // флаг включения для запросов UDP 
_i                  TOUDP=2;                        // таймауты и паузы
uc                  SRVIP[MWS+1][LENFLD];           // адреса IP на которые будут делаться запросы
ui                  SRVPORT[MWS+1];                 // номера портов для запросов
ui                  CurSIP=0;                       // текущий номер сервера(контроллера)(IP) для запроса
ui                  UDReq=0;                        // номер запроса по UDP
ui                  UDRC[MWS];                      // счетчики текущих каналов для запросов
ui                  UDRQ[MAXRQ][80+1];              // номера текущих запрошенных каналов
//----------------------------------------------------------------------------


//****************************************************************************
// БУФЕР ПРИЕМА
struct  UDPR1{                              // Code=1
   uc   Code;                               // 1
   uc   NData;                              // 1 (Кол-во запрашиваемых данных)
   uc   Const0;                             // 1
   uc   NReq;                               // 1 (Номер запроса)
   uc   Const1;                             // 1
   uc   Const2;                             // 1
   _s   ReqData[124];                       // 2*124=248 (N запраш каналов)
   _s   ChNum  [124];                       // 2*124=248 (N каналов на запрашивающ стороне)
   uc   Const3;                             // 1
   uc   NWS;                                // 1
}  UDPR1;                                   // Всего 504 байта
//----------------------------------------------------------------------------
struct  UDPR2{                              // Code=2
   uc   Code;                               // 1
   uc   NReq;                               // 1
   ui   Time      __attribute__((packed));  // 4
   _f   Data[124] __attribute__((packed));  // 4*124=496
   uc   Const;                              // 1    
   uc   NWS;                                // 1
}  UDPR2;                                   // Всего 504 байта
//----------------------------------------------------------------------------
struct  UDPR7{                              // Code=7
   uc   Code;                               // 1
   uc   Const0;                             // 1
   _s   NData;                              // 2
   _s   ChNum [80];                         // 2*80=160
   _f   ChData[80] __attribute__((packed)); // 4*80=320
   uc   Const3[19];                         // 19
   uc   NWS;                                // 1
}  UDPR7;                                   // Всего: 504 байта
//----------------------------------------------------------------------------
// ответ на запрос с номерами каналов
struct  UDPR27{                             // Code=27
   uc   Code;                               // 1
   uc   Const0;                             // 1
   _s   NData;                              // 2
   _s   ChNum [80];                         // 2*80=160
   _f   ChData[80] __attribute__((packed)); // 4*80=320
   uc   Const3[18];                         // 18         
   uc   NReq;                               // 1
   uc   NWS;                                // 1
}  UDPR27;                                  // Всего: 504 байта
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// БУФЕР ПЕРЕДАЧИ
struct  UDPS1{                              // Code=1
   uc   Code;                               // 1
   uc   NData;                              // 1 (Кол-во запрашиваемых данных)
   uc   Const0;                             // 1 
   uc   NReq;                               // 1 (Номер запроса)
   uc   Const1;                             // 1
   uc   Const2;                             // 1
   _s   ReqData[124];                       // 2*124=248 (N запраш каналов)
   _s   ChNum  [124];                       // 2*124=248 (N каналов на запрашивающей стороне)
   uc   Const3;                             // 1
   uc   NWS;                                // 1
}  UDPS1;                                   // Всего 504 байта
//----------------------------------------------------------------------------
struct  UDPS2{                              // Code=2
   uc   Code;                               // 1
   uc   NReq;                               // 1
   ui   Time      __attribute__((packed));  // 4
   _f   Data[124] __attribute__((packed));  // 4*124=496
   uc   Const;                              // 1
   uc   NWS;                                // 1
}  UDPS2;                                   // Всего 504 байта
//----------------------------------------------------------------------------
struct  UDPS7{                              // Code=7
   uc   Code;                               // 1
   uc   Const0;                             // 1
   _s   NData      __attribute__((packed)); // 2
   _s   ChNum [80] __attribute__((packed)); // 2*80=160
   _f   ChData[80] __attribute__((packed)); // 4*80=320
   uc   Const3[19];                         // 19
   uc   NWS;                                // 1
}  UDPS7;                                   // Всего: 504 байта
//----------------------------------------------------------------------------
// ответ на запрос с номерами каналов
struct  UDPS27{                             // Code=27
   uc   Code;                               // 1
   uc   Const0;                             // 1
   _s   NData;                              // 2
   _s   ChNum [80];                         // 2*80=160
   _f   ChData[80] __attribute__((packed)); // 4*80=320
   uc   Const3[18];                         // 18         
   uc   NReq;                               // 1
   uc   NWS;                                // 1
}  UDPS27;                                  // Всего: 504 байта
//****************************************************************************


//----------------------------------------------------------------------------
//  Получить номер текущей рабочей станции из строкового имени NetBIOS
ui  WS_UDP_GetLocNWS(vd){
    uc  c[4]={0,0,0,0};
    c[0]=NMWS[6]; c[1]=NMWS[7]; c[2]=0; if(c[1]==32) c[1]=0;
    return(atoi(c));
}
//----------------------------------------------------------------------------
//  Формирование ответного блока для сетевого ответа
vd  WS_UDP_Answer(vd){
    _i  i =0;
    ui  NV=0;
    ul  atime =0;
    memcpy(&UDPR1,&RUBUF,504);
    memset(&UDPS2,0,sizeof(UDPS2));
    UDPS2.Code=2;
    UDPS2.NReq= UDPR1.NReq;
    atime=time(NULL);
    UDPS2.Time=(ul)atime; 
    if (UDPR1.NData>=124){ UDPR1.NData=124-1; }
    for(i=0; i<UDPR1.NData; i++){
        NV=(UDPR1.ReqData[i] & 0x0fff);
        UDPS2.Data[i]=CB[NV].v;
    }
    UDPS2.Const=2;
    memcpy(&SUBUF,&UDPS2,LNCB);
    memset(&UDPS2,0,sizeof(UDPS2));
    memset(&UDPR1,0,sizeof(UDPR1));
    return;
}
//----------------------------------------------------------------------------
//  Формирование ответного блока для сетевого ответа с номерами каналов
vd  WS_UDP_Answer27(vd){
    _i  i =0;
    _i  NV=0;
    memcpy(&UDPR1,&RUBUF,504);
    memset(&UDPS27,0,sizeof(UDPS27));
    UDPS27.Code=27;
    UDPS27.NReq= UDPR1.NReq;
    // не более 80 запрашиваемых каналов
    if (UDPR1.NData>=80){ UDPR1.NData=80; }
    // значения для этих каналов
    for(i=0; i<UDPR1.NData; i++){
        UDPS27.ChNum[i]=UDPR1.ChNum[i];
        NV=(UDPR1.ReqData[i] & 0x0fff);
        if(NV<0||NV>MCH) break;
        UDPS27.ChData[i]=CB[NV].v;
    };  UDPS27.NData= i;
    UDPS27.NWS=(uc)WS_UDP_GetLocNWS();
    memcpy(&SUBUF,&UDPS27,LNCB);
    memset(&UDPS27,0,sizeof(UDPS27));
    memset(&UDPR1 ,0,sizeof(UDPR1));
    return;
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  Передача принятых из сети данных в массив обмена
vd  WS_UDP_GetFromNet(vd){
    ui  i=0,NV=0;
    memcpy(&UDPR7,&RUBUF,LNCB);
    if (UDPR7.NData>80){ UDPR7.NData=80; }
    for(i=0; i<UDPR7.NData; i++){
        NV=UDPR7.ChNum[i] & 0x0fff;
        CB[NV].v=UDPR7.ChData[i];
    }
    memset(&UDPR7,0,sizeof(UDPR7));
    return;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  Запрос на получение значений каналов
//  NWS -  Номер станции в сети у которой будут запрашиваться каналы
_i  WS_UDP_ReqToNet(ui NWS){
    _i  j=0, b=0;
    uc  c[8]={0,0,0,0,0,0,0,0};
    ui  z1;
    // Проверить номер запрашиваемой станции
    if ((NWS<1)||(NWS>MWS)){ DPrn(38,0,"38 WS_UDP_ReqToNet: 1<NWS|NWS>MWS\n"); return(1); }
    if (HOWC[NWS]==0){ DPrn(38,0,"38 WS_UDP_ReqToNet: HOWC[NWS]=0\n"); return(2); }
    // Преобразовать номер станции в строку
    itoa(NWS,c,10);                          
    // Заполнить структуру запроса:
    memset(&UDPS1,0,sizeof(UDPS1));
    memset(&SUBUF,0,sizeof(SUBUF));
    // Код операции запроса
    UDPS1.Code=1;
    // Запрос ответа с номерами каналов
    UDPS1.Const0=27;
    // Номер станции на которую посылаем запрос
    UDPS1.Const1=NWS;
    // Номер запроса
    UDReq++; if(UDReq>=MAXRQ){ UDReq=0; }
    UDPS1.NReq=UDReq;
    z1=UDReq;
DPrn(38,0,"38 WS_UDP_ReqToNet: REQ%d.%d:%s ",NWS,UDReq,SRVIP[CurSIP]);
    // Продолжать запрос с канала на котором остановились
    for(j=UDRC[NWS],b=0;;j++,b++,UDRC[NWS]++){
        // Если номер текущего канала больше, чем общее количество каналов
        // для данного сервера - установить счетчик текущего канала на 0,выйти
        if(j>=HOWC[NWS]){ UDRC[NWS]=0; break; }
        // Запрашивать по 80 каналов
        if(b>=80){ break; }
        // Записать в запрос номер канала на локальной машине для
        // записи  их в ответный блок на удаленной машине
        UDPS1.ChNum[b]  =(_s)SCHN[NWS][ j ];
        // Записать в запрос номер запрашиваемого канала на удаленной станции
        UDPS1.ReqData[b]=(_s)RCHN[NWS][ j ];
DPrn(38,0,"J%d:L%d:R%d ",j,(_i)UDPS1.ChNum[b],(_i)UDPS1.ReqData[b]);
        // Запомнить локальные номера запрашиваемых каналов 
        // для присвоения значен ответов
        UDRQ[UDReq][b]  =SCHN[NWS][ j ];
    }
DPrn(38,0,"\n");

// NWS-номер контрол,z1-номер запроса,SCHN[NWS]-1ый номер канала на уд станц,HOWC-кол запр канал
DPrn(38,0,"38 WS_UDP_ReqToNet: NWS:%02d Send:%03d FN:%04d HW:%03d HC:%03d %s:%d\n",
           NWS,z1,SCHN[NWS][0],b,HOWC[NWS],SRVIP[CurSIP],SRVPORT[CurSIP]);

    // незапрашиваемые каналы дописать отриц значениями
    for(j=b; j<80; j++){ UDRQ[UDReq][j]=-1; UDPS1.ChNum[b]=0; }
    // Кол-во запрашиваемых каналов
    UDPS1.NData=(uc)(b);
    // Передать заполненную структуру в буфер посылок
    memcpy(&SUBUF,&UDPS1,LNCB);
    memset(&UDPS1,0,sizeof(UDPS1));
    return(0);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Передача принятых из сети ответов в массив обмена
vd WS_UDP_GetReq(vd){
   ui a=0, NReq=0;
   // Копировать принятый буфер в структуру
   memcpy(&UDPR2,&RUBUF,LNCB);
   memset(&RUBUF,0,sizeof(RUBUF));
   NReq=UDPR2.NReq;
DPrn(37,0,"37 WS_UDP_GetReq: AnswerNR:%03d\n",NReq);
   // Если номер запроса неправильный выйти
   if (NReq<0||NReq>=MAXRQ){ return; }
   // Записать принятые значения каналов в массив
   for(a=0;a<NReq;a++){
       // если был сформирован неполный блок и данных больше нет- выйти
       if( UDRQ[ NReq ][a]<=0 ){ break; }
       CB[ UDRQ[ NReq ][a] ].v = UDPR2.Data[a];
   }
   memset(&UDPR2,0,sizeof(UDPR2));
   return;
}
//----------------------------------------------------------------------------
// Передача принятых из сети ответов с номерами каналов в массив обмена
vd WS_UDP_GetR27(vd){
   _i a=0, ch=0, NReq=0, NData=0;
   // Копировать принятый буфер в структуру
   memcpy(&UDPR27,&RUBUF,LNCB);
   memset(&RUBUF,0,sizeof(RUBUF));
   NReq =UDPR27.NReq;
   NData=UDPR27.NData;
DPrn(36,0,"36 WS_UDP_GetR27: Answer27:%03d:%02d  ",NReq,NData);
   // Если номер запроса неправильный выйти
   if (NReq>=MAXRQ){ return; }
   // Записать принятые значения каналов в массив
   for(a=0;a<NData;a++){
       ch=UDPR27.ChNum[a]; if(ch<0||ch>=MCH) continue;
DPrn(36,0,"%d=%4.1f ",ch,UDPR27.ChData[a]);
       CB[ ch ].v = UDPR27.ChData[a];
   }
DPrn(36,0," OK\n");
   memset(&UDPR27,0,sizeof(UDPR27));
   return;
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  старое название функции: vd SendRecvUDP(vd)
_i  TCP_exchange(vd){
    int n=0, r=1,   b=0,j=0;
    int rcvLen=sizeof(SOCKADDR_IN);
    ul  flags=1;
    if(UDSel==0){ CurSIP++; if(CurSIP>=HSRV) CurSIP=0; }
    if(!hSockS[CurSIP])hSockS[CurSIP]=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    ioctlsocket(hSockS[CurSIP],FIONBIO,&flags);
    rmaddr.sin_family      =  AF_INET;
    rmaddr.sin_addr.s_addr =  inet_addr(SRVIP[CurSIP]);
    rmaddr.sin_port        =  htons(SRVPORT[CurSIP]);
    if(UDSel==0){
           r=WS_UDP_ReqToNet(SRV_NUM[CurSIP]);
           n=sendto(hSockS[CurSIP], SUBUF, LNCB, 0, (SOCKADDR*)&rmaddr, sizeof(rmaddr));
           UDSel=1;
DPrn(35,0,"\n35 TCP_Exchange: CurSIP=%d:SRV_NUM=%d,SRVIP=%s:SRVPORT=%d:SOCKS=%d:TOUDP=%d:hSockS=%X\n",
           CurSIP,SRV_NUM[CurSIP],SRVIP[CurSIP],SRVPORT[CurSIP],n,TOUDP,hSockS[CurSIP]);
           return;
    }
    if(UDSel>TOUDP){
DPrn(35,0,"35 TCP_Exchange: Receive...\n");
        n=recvfrom(hSockS[CurSIP],RUBUF,LNCB,0,(SOCKADDR*)&rmaddr,&rcvLen);
    }
    if(n==504&&RUBUF[503]>0&&RUBUF[503]!=(uc)WS_UDP_GetLocNWS()){
DPrn(35,0,"35 TCP_Exchange: R%d:%d!=%d\n",n,RUBUF[503],WS_UDP_GetLocNWS());
       if(RUBUF[0]==2 ){ WS_UDP_GetReq(); }
       if(RUBUF[0]==27){ WS_UDP_GetR27(); }
       UDSel=-2;
    }
    UDSel++; if(UDSel>TOUDP*2) UDSel=0;
    return(0);
}
//----------------------------------------------------------------------------

