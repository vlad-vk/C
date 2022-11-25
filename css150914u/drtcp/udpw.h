// coding: cp866
//----------------------------------------------------------------------------
#define             LENFLD              100         // ���� ������ ��ப����� ����
#define             MAXRQ               255         // ���� ���-�� ����ᮢ
#define             LNAM                16          // ������ NetBIOS �����
#define             LNCB                504         // ࠧ��� ��।�������� ����� 

ex   _i             SRV_NUM[MWS];                   // ����� ����஫��஢ �� ���浪�
ex   _i             RSRV[MTCP];                     // ����� ����஫��஢ ��� ������� ������
ex   _i             HOWC[MWS];                      // ���-�� ����訢����� ������� ��� ������� ����஫���
ex   _i             RCHN[MWS+1][MTCP];              // ����� ������ �� 㤠������ �ࢥ�
ex   _i             SCHN[MWS+1][MTCP];              // ����� ������ �� �����쭮� ��������

ex struct  CB{
   uc   nm[SLEN];            // ��� ������
   _i   ar;                  // �ਧ��� ����� ���祭�� ������ � ��娢
   _i   dt;                  // ⨯ �ࠩ���   (0-���,1-COM,2-NB,3-TCP)
   _i   dn;                  // ����� ����,�⠭樨
   _i   dd;                  // ����� ��६����� �� 㤠������ �⠭樨
   _i   t;                   // ⨯ ��६����� (1-�室,2-��室,3-�室/��室)
   _i   dc;                  // ������⢮ ��� ��᫥ ����⮩
   _f   l;                   // ������ �࠭��
   _f   h;                   // ������ �࠭��
   uc   rm[SLEN];            // �������਩
   _f   v;                   // ���祭�� ������
}  CB[MCH];

_i                  UDSel=0;                        // ���稪 ���� UDP ���뫮�

//----------------------------------------------------------------------------
// ��६���� ��� ����ᮢ �१ UDP
#define             MBSZ                512         // ���ᨬ���� ࠧ��� ���஢ ������
#define             WS_VERSION_REQD     0x0101      // ����� ���ᨨ WinSock
WSADATA             stWSAData;                      // ����� ������ᨨ ������⥪� WinSock
SOCKET              hSockS[MWS+1];                  // ᮪�� ᮥ�������
SOCKADDR_IN         rmaddr;                         // ������� ���� 㤠������ ��設�
SOCKADDR_IN         myaddr;                         // ������� ���� 㤠������ ��設�
uc                  SUBUF[MBSZ];                    // ���� ��।�� �१ UDP
uc                  RUBUF[MBSZ];                    // ���� �ਥ�� �१ UDP
ui                  UDPRQ=1;                        // 䫠� ����祭�� ��� ����ᮢ UDP 
_i                  TOUDP=2;                        // ⠩����� � ����
uc                  SRVIP[MWS+1][LENFLD];           // ���� IP �� ����� ���� �������� ������
ui                  SRVPORT[MWS+1];                 // ����� ���⮢ ��� ����ᮢ
ui                  CurSIP=0;                       // ⥪�騩 ����� �ࢥ�(����஫���)(IP) ��� �����
ui                  UDReq=0;                        // ����� ����� �� UDP
ui                  UDRC[MWS];                      // ���稪� ⥪��� ������� ��� ����ᮢ
ui                  UDRQ[MAXRQ][80+1];              // ����� ⥪��� ����襭��� �������
//----------------------------------------------------------------------------


//****************************************************************************
// ����� ������
struct  UDPR1{                              // Code=1
   uc   Code;                               // 1
   uc   NData;                              // 1 (���-�� ����訢����� ������)
   uc   Const0;                             // 1
   uc   NReq;                               // 1 (����� �����)
   uc   Const1;                             // 1
   uc   Const2;                             // 1
   _s   ReqData[124];                       // 2*124=248 (N ����� �������)
   _s   ChNum  [124];                       // 2*124=248 (N ������� �� ����訢��� ��஭�)
   uc   Const3;                             // 1
   uc   NWS;                                // 1
}  UDPR1;                                   // �ᥣ� 504 ����
//----------------------------------------------------------------------------
struct  UDPR2{                              // Code=2
   uc   Code;                               // 1
   uc   NReq;                               // 1
   ui   Time      __attribute__((packed));  // 4
   _f   Data[124] __attribute__((packed));  // 4*124=496
   uc   Const;                              // 1    
   uc   NWS;                                // 1
}  UDPR2;                                   // �ᥣ� 504 ����
//----------------------------------------------------------------------------
struct  UDPR7{                              // Code=7
   uc   Code;                               // 1
   uc   Const0;                             // 1
   _s   NData;                              // 2
   _s   ChNum [80];                         // 2*80=160
   _f   ChData[80] __attribute__((packed)); // 4*80=320
   uc   Const3[19];                         // 19
   uc   NWS;                                // 1
}  UDPR7;                                   // �ᥣ�: 504 ����
//----------------------------------------------------------------------------
// �⢥� �� ����� � ����ࠬ� �������
struct  UDPR27{                             // Code=27
   uc   Code;                               // 1
   uc   Const0;                             // 1
   _s   NData;                              // 2
   _s   ChNum [80];                         // 2*80=160
   _f   ChData[80] __attribute__((packed)); // 4*80=320
   uc   Const3[18];                         // 18         
   uc   NReq;                               // 1
   uc   NWS;                                // 1
}  UDPR27;                                  // �ᥣ�: 504 ����
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// ����� ��������
struct  UDPS1{                              // Code=1
   uc   Code;                               // 1
   uc   NData;                              // 1 (���-�� ����訢����� ������)
   uc   Const0;                             // 1 
   uc   NReq;                               // 1 (����� �����)
   uc   Const1;                             // 1
   uc   Const2;                             // 1
   _s   ReqData[124];                       // 2*124=248 (N ����� �������)
   _s   ChNum  [124];                       // 2*124=248 (N ������� �� ����訢��饩 ��஭�)
   uc   Const3;                             // 1
   uc   NWS;                                // 1
}  UDPS1;                                   // �ᥣ� 504 ����
//----------------------------------------------------------------------------
struct  UDPS2{                              // Code=2
   uc   Code;                               // 1
   uc   NReq;                               // 1
   ui   Time      __attribute__((packed));  // 4
   _f   Data[124] __attribute__((packed));  // 4*124=496
   uc   Const;                              // 1
   uc   NWS;                                // 1
}  UDPS2;                                   // �ᥣ� 504 ����
//----------------------------------------------------------------------------
struct  UDPS7{                              // Code=7
   uc   Code;                               // 1
   uc   Const0;                             // 1
   _s   NData      __attribute__((packed)); // 2
   _s   ChNum [80] __attribute__((packed)); // 2*80=160
   _f   ChData[80] __attribute__((packed)); // 4*80=320
   uc   Const3[19];                         // 19
   uc   NWS;                                // 1
}  UDPS7;                                   // �ᥣ�: 504 ����
//----------------------------------------------------------------------------
// �⢥� �� ����� � ����ࠬ� �������
struct  UDPS27{                             // Code=27
   uc   Code;                               // 1
   uc   Const0;                             // 1
   _s   NData;                              // 2
   _s   ChNum [80];                         // 2*80=160
   _f   ChData[80] __attribute__((packed)); // 4*80=320
   uc   Const3[18];                         // 18         
   uc   NReq;                               // 1
   uc   NWS;                                // 1
}  UDPS27;                                  // �ᥣ�: 504 ����
//****************************************************************************


//----------------------------------------------------------------------------
//  ������� ����� ⥪�饩 ࠡ�祩 �⠭樨 �� ��ப����� ����� NetBIOS
ui  WS_UDP_GetLocNWS(vd){
    uc  c[4]={0,0,0,0};
    c[0]=NMWS[6]; c[1]=NMWS[7]; c[2]=0; if(c[1]==32) c[1]=0;
    return(atoi(c));
}
//----------------------------------------------------------------------------
//  ��ନ஢���� �⢥⭮�� ����� ��� �⥢��� �⢥�
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
//  ��ନ஢���� �⢥⭮�� ����� ��� �⥢��� �⢥� � ����ࠬ� �������
vd  WS_UDP_Answer27(vd){
    _i  i =0;
    _i  NV=0;
    memcpy(&UDPR1,&RUBUF,504);
    memset(&UDPS27,0,sizeof(UDPS27));
    UDPS27.Code=27;
    UDPS27.NReq= UDPR1.NReq;
    // �� ����� 80 ����訢����� �������
    if (UDPR1.NData>=80){ UDPR1.NData=80; }
    // ���祭�� ��� ��� �������
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
//  ��।�� �ਭ���� �� �� ������ � ���ᨢ ������
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
//  ����� �� ����祭�� ���祭�� �������
//  NWS -  ����� �⠭樨 � �� � ���ன ���� ����訢����� ������
_i  WS_UDP_ReqToNet(ui NWS){
    _i  j=0, b=0;
    uc  c[8]={0,0,0,0,0,0,0,0};
    ui  z1;
    // �஢���� ����� ����訢����� �⠭樨
    if ((NWS<1)||(NWS>MWS)){ DPrn(38,0,"38 WS_UDP_ReqToNet: 1<NWS|NWS>MWS\n"); return(1); }
    if (HOWC[NWS]==0){ DPrn(38,0,"38 WS_UDP_ReqToNet: HOWC[NWS]=0\n"); return(2); }
    // �८�ࠧ����� ����� �⠭樨 � ��ப�
    itoa(NWS,c,10);                          
    // ��������� �������� �����:
    memset(&UDPS1,0,sizeof(UDPS1));
    memset(&SUBUF,0,sizeof(SUBUF));
    // ��� ����樨 �����
    UDPS1.Code=1;
    // ����� �⢥� � ����ࠬ� �������
    UDPS1.Const0=27;
    // ����� �⠭樨 �� ������ ���뫠�� �����
    UDPS1.Const1=NWS;
    // ����� �����
    UDReq++; if(UDReq>=MAXRQ){ UDReq=0; }
    UDPS1.NReq=UDReq;
    z1=UDReq;
DPrn(38,0,"38 WS_UDP_ReqToNet: REQ%d.%d:%s ",NWS,UDReq,SRVIP[CurSIP]);
    // �த������ ����� � ������ �� ���஬ ��⠭�������
    for(j=UDRC[NWS],b=0;;j++,b++,UDRC[NWS]++){
        // �᫨ ����� ⥪�饣� ������ �����, 祬 ��饥 ������⢮ �������
        // ��� ������� �ࢥ� - ��⠭����� ���稪 ⥪�饣� ������ �� 0,���
        if(j>=HOWC[NWS]){ UDRC[NWS]=0; break; }
        // ����訢��� �� 80 �������
        if(b>=80){ break; }
        // ������� � ����� ����� ������ �� �����쭮� ��設� ���
        // �����  �� � �⢥�� ���� �� 㤠������ ��設�
        UDPS1.ChNum[b]  =(_s)SCHN[NWS][ j ];
        // ������� � ����� ����� ����訢������ ������ �� 㤠������ �⠭樨
        UDPS1.ReqData[b]=(_s)RCHN[NWS][ j ];
DPrn(38,0,"J%d:L%d:R%d ",j,(_i)UDPS1.ChNum[b],(_i)UDPS1.ReqData[b]);
        // ��������� ������� ����� ����訢����� ������� 
        // ��� ��᢮���� ���祭 �⢥⮢
        UDRQ[UDReq][b]  =SCHN[NWS][ j ];
    }
DPrn(38,0,"\n");

// NWS-����� ����஫,z1-����� �����,SCHN[NWS]-1� ����� ������ �� � �⠭�,HOWC-��� ���� �����
DPrn(38,0,"38 WS_UDP_ReqToNet: NWS:%02d Send:%03d FN:%04d HW:%03d HC:%03d %s:%d\n",
           NWS,z1,SCHN[NWS][0],b,HOWC[NWS],SRVIP[CurSIP],SRVPORT[CurSIP]);

    // ������訢���� ������ ������� ���� ���祭�ﬨ
    for(j=b; j<80; j++){ UDRQ[UDReq][j]=-1; UDPS1.ChNum[b]=0; }
    // ���-�� ����訢����� �������
    UDPS1.NData=(uc)(b);
    // ��।��� ����������� �������� � ���� ���뫮�
    memcpy(&SUBUF,&UDPS1,LNCB);
    memset(&UDPS1,0,sizeof(UDPS1));
    return(0);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ��।�� �ਭ���� �� �� �⢥⮢ � ���ᨢ ������
vd WS_UDP_GetReq(vd){
   ui a=0, NReq=0;
   // ����஢��� �ਭ��� ���� � ��������
   memcpy(&UDPR2,&RUBUF,LNCB);
   memset(&RUBUF,0,sizeof(RUBUF));
   NReq=UDPR2.NReq;
DPrn(37,0,"37 WS_UDP_GetReq: AnswerNR:%03d\n",NReq);
   // �᫨ ����� ����� ���ࠢ���� ���
   if (NReq<0||NReq>=MAXRQ){ return; }
   // ������� �ਭ��� ���祭�� ������� � ���ᨢ
   for(a=0;a<NReq;a++){
       // �᫨ �� ��ନ஢�� ������� ���� � ������ ����� ���- ���
       if( UDRQ[ NReq ][a]<=0 ){ break; }
       CB[ UDRQ[ NReq ][a] ].v = UDPR2.Data[a];
   }
   memset(&UDPR2,0,sizeof(UDPR2));
   return;
}
//----------------------------------------------------------------------------
// ��।�� �ਭ���� �� �� �⢥⮢ � ����ࠬ� ������� � ���ᨢ ������
vd WS_UDP_GetR27(vd){
   _i a=0, ch=0, NReq=0, NData=0;
   // ����஢��� �ਭ��� ���� � ��������
   memcpy(&UDPR27,&RUBUF,LNCB);
   memset(&RUBUF,0,sizeof(RUBUF));
   NReq =UDPR27.NReq;
   NData=UDPR27.NData;
DPrn(36,0,"36 WS_UDP_GetR27: Answer27:%03d:%02d  ",NReq,NData);
   // �᫨ ����� ����� ���ࠢ���� ���
   if (NReq>=MAXRQ){ return; }
   // ������� �ਭ��� ���祭�� ������� � ���ᨢ
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
//  ��஥ �������� �㭪樨: vd SendRecvUDP(vd)
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

