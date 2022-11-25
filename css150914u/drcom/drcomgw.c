// coding: cp866
//---------------------------------------------------------------------------
// �롮� ��ப� �� ��६�����
#include <allegro.h>
#include <winalleg.h>
#include "../include.h"
#include "../define.h"
#include "drcom.h"

//---------------------------------------------------------------------------
// ��⥬�� ����� ����⮣� 䠩�� COM ����
HANDLE         SPHandle[SPMAX];
// ��।������ ������� ��� ࠡ��� � COM ���⮬
COMMTIMEOUTS   CommTimeOuts;
DCB            dcb;
COMSTAT        CommState;
OVERLAPPED     Overlap;

//---------------------------------------------------------------------------
// ������ ���� �ਥ�� �ࠩ���
vd VKSPRBClear(ui p){
   PurgeComm(SPHandle[p], PURGE_RXCLEAR);
   return;
}
//---------------------------------------------------------------------------
// ������ ���� ��।�� �ࠩ���
vd VKSPWBClear(ui p){
   PurgeComm(SPHandle[p], PURGE_TXCLEAR);
   return;
}
//---------------------------------------------------------------------------
// ������ ���� �ਥ�� �ணࠬ��
vd VKSPRBZero(ui p){
   _i  a=0;
   for(a=0;a<SPBSZ;a++){ SPRBuf[p][a]=0; }
   return;
}
//---------------------------------------------------------------------------
// ������ ���� ��।�� �ணࠬ��
vd VKSPWBZero(ui p){
   _i  a=0;
   for(a=0;a<SPBSZ;a++){ SPSBuf[p][a]=0; }
   return;
}
//---------------------------------------------------------------------------
// ������ �� �����
vd VKSPAClear(ui p){
   VKSPRBClear(p); VKSPWBClear(p); VKSPRBZero(p); VKSPWBZero(p);
   return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ������� COM ����
vd VKSPStop(vd){ CloseHandle(SPHandle[CurSP]); }
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ������ ���� �� �室���� ���� COM ����
uc VKSPGetByte(ui p){
   int    result;
   DWORD  temp;
   uc     ReadByte[2];
   ClearCommError(SPHandle[p],&temp,&CommState);
   if(!temp){
      result=ReadFile(SPHandle[p],ReadByte,1,&temp,&Overlap);
      if(result){ return(unsigned char)ReadByte[0]; }
   }
   return(0);
}
//---------------------------------------------------------------------------
// ������ ����� �� COM ���� � ���� �ਥ��
us VKSPIn(ui p,us NumBytes){
   int    result, a=0;
   DWORD  temp;
   ClearCommError(SPHandle[p],&temp,&CommState);
   if(!temp){
      result=ReadFile(SPHandle[p],SPRBuf[p],NumBytes,&temp,&Overlap);
      if(result){ 
         if(Debug(6)>0){
             DPrn(6,1,"06:RCV:");
              for(a=0;a<NumBytes;a++){ DPrn(6,0,"%c",SPRBuf[p][a]); }
             DPrn(6,0,"\n");
         }
         return(unsigned short)temp; 
      }
   } else {
      DPrn(6,1,"06:ERR: VKSPIn()->!temp\n");
   }
   return(0);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ������� ����� �� ���� � COM ����
us VKSPOut(ui p,us NumBytes){
   int    a=0, result;
   DWORD  temp;
   if(NumBytes>0){
      DPrn(5,1,"05:SND:");
      for(a=0;a<NumBytes;a++){ DPrn(5,0,"%c",SPSBuf[p][a]); }
      DPrn(5,0,"\n");
      ClearCommError  (SPHandle[p],&temp,&CommState);  
      result=WriteFile(SPHandle[p],SPSBuf[p],NumBytes,&temp,&Overlap);
      if(result){ return(unsigned short)temp; }
   }
   return(0);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ��⠭����� ��ࠬ����� COM ����
_i VKSPInit(ui p){ return(0); }
_i VKSPSet (ui p, ui Baud, ui Control){
   DWORD CBAUD=CBR_115200;
   uc    SPName[20];
   if (p==0){ strcpy(SPName,"COM1"); }
   if (p==1){ strcpy(SPName,"COM2"); }
   if (p==2){ strcpy(SPName,"LPT1"); }
   // ������ 䠩� COM ����     
   SPHandle[p]= CreateFile( SPName, GENERIC_READ | GENERIC_WRITE, 0, NULL,
                            OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
   // �஢���� ���४⭮��� ������ COM ����
   if(SPHandle[p]==INVALID_HANDLE_VALUE){ return(-1); }
   if(!SetupComm(SPHandle[p],SPBSZ,SPBSZ)){ return(-2); }


   if(!GetCommState(SPHandle[p],&dcb)){ return(-3); }
   // ��।������ ᪮���
   if(Baud<115200){ CBAUD=CBR_57600; }
   if(Baud<57600 ){ CBAUD=CBR_38400; }
   if(Baud<38400 ){ CBAUD=CBR_19200; }
   if(Baud<19200 ){ CBAUD=CBR_9600;  }
   if(Baud<9600  ){ CBAUD=CBR_4800;  }
   if(Baud<4800  ){ CBAUD=CBR_2400;  }
   if(Baud<2400  ){ CBAUD=CBR_1200;  }

   // ������ ��⠭���� � �������� DCB
// #define DTR_CONTROL_DISABLE   0 
// #define DTR_CONTROL_HANDSHAKE 2
// #define RTS_CONTROL_DISABLE   0
// #define RTS_CONTROL_ENABLE    1
// #define RTS_CONTROL_HANDSHAKE 2
// #define RTS_CONTROL_TOGGLE    3
   dcb.BaudRate         = CBAUD;
   dcb.fBinary          = TRUE;
   dcb.fOutxCtsFlow     = FALSE;
   dcb.fOutxDsrFlow     = FALSE;
   dcb.fDtrControl      = DTR_CONTROL_HANDSHAKE;
   dcb.fDsrSensitivity  = FALSE;
   dcb.fNull            = FALSE;
   dcb.fRtsControl      = RTS_CONTROL_ENABLE;
   dcb.fAbortOnError    = FALSE;
   dcb.ByteSize         = 8;                // 8
   dcb.Parity           = 0;                // N
   dcb.StopBits         = 0;                // 1
   if(!SetCommState(SPHandle[p],&dcb)){ return(-4); }

   // ������ ⠩����� � ��������           
   if(!GetCommTimeouts(SPHandle[p],&CommTimeOuts)){ return(-5); }
   CommTimeOuts.ReadIntervalTimeout         = TimeOutInterval;
   CommTimeOuts.ReadTotalTimeoutMultiplier  = TimeOutChar;
   CommTimeOuts.ReadTotalTimeoutConstant    = TimeOutWait;
   CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
   CommTimeOuts.WriteTotalTimeoutConstant   = 0;
   if(!SetCommTimeouts(SPHandle[p],&CommTimeOuts)){ return(-6); }

   CSRB[p]=0;
   return(1);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// �����頥� ������⢮ ᨬ����� � ���� �ਥ��, ����� �� �� ��⠭��
// �㭪樥� Read()
_i VKSPInStat(ui p){
   DWORD  temp=0;
   ClearCommError(SPHandle[p],&temp,&CommState); SPRESULT[p]=temp;
   return (unsigned short) CommState.cbInQue;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// �����頥� ������⢮ ᨬ����� � ���� ��।��, ����� �� �� ��।���
// �㭪樥� Write()
_i VKSPOutStat(ui p){
   DWORD  temp=0;
   ClearCommError(SPHandle[p],&temp, &CommState); SPRESULT[p]=temp;
   return (unsigned short) CommState.cbOutQue;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
vd VKSPHand(ui Hand){ return; }
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
ui VKSPStat(vd){ return 0; }
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Detects UART type, enables FIFO if it exists.
// Returns 0 if 16550 UART and FIFO enabled.
// Returns 1 if 16450 UART.
// Returns 2 if less than 16450 UART.
_i VKSPFifoInit(vd){
   return 0;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ���⪠ ���� �ਥ��
vd ClearCB(ui p){
   VKSPRBClear(p); 
   VKSPRBZero (p);
   CSRB [p]=0; 
   TOInp[p]=0;
   COMBusy[p]=0; 
   return;
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// ��।�� ������ �� ���� �ਥ��
vd inp_COM(ui p){
   _i  a=0,b=0;
   //  �᫨ �� �� ०�� �⥭�� - ���
   if (COMBusy[p]!= 2){ 
       DPrn(6,1,"?CB%d ",COMBusy[p]); 
       return; 
   }
   //  ������ �����ᥪ㭤� 㢥��稢��� ���稪 ⠩���� �ਥ��
   if(MTimer(8,'m',1)==2){ 
      MTimer(8,'m',0); if(TOInp[p]<10000) TOInp[p]++;
   }
   //  �᫨ ���� ��।�� �����- ��� (����� ����砭�� ��।��)
   if (VKSPOutStat(p)>SPBSZ-14){ 
       DPrn(6,1,"06:INP:OUT buffer full.\n"); 
       return; 
   }
   // ��� ������ ��� �ਥ�� - ���
   if((b=VKSPInStat(p))<1){ 
       DPrn(6,0,"-");
       return; 
   }
// // ������� ����� �� ���� Wndows � ���� �ਥ��
   DPrn(6,1,"06:RCV:");
   for(a=0;a<b;a++){
       SPRBuf[p][a]=VKSPGetByte(p);
       DPrn(6,0,"%c",SPRBuf[p][a]);
   };  DPrn(6,0,"\n");
   VKSPRBClear(p); if(CSRB[p]<0) CSRB[p]=0;
   // ��।��� ����� �� ���� �ਥ�� �� �६���� ���� ��ࠡ�⪨
   for(a=0;a<b;a++){
       SPTBuf[p][CSRB[p]]=SPRBuf[p][a];
       if(SPFLBEG< 0){ 
          if(SPRBuf[p][a]=='@'){ SPFLBEG=0; }
       }
       if(SPFLBEG>=0){ 
          if(CSRB[p]<14){ CSRB[p]++; }
          SPFLBEG++;
          if(SPRBuf[p][a]=='*'&&SPFLBEG>=13){ 
             SPFLBEG=-1;
             if(CSRB[p]==14){ 
                memcpy(&COMANS[p],&SPTBuf[p],14); COMGetCH(p);
             }
             CSRB[p]=0;
          }
       }
   }
   if(MTimer(8,'m',1)==2){ 
      MTimer(8,'m',0); if(TOInp[p]<10000){ TOInp[p]++; }
   }
   return;
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// ���뫪� ������ � COM ����
vd out_COM(ui p){
   // �᫨ �ணࠬ�� �� � ०��� ��।�� - ���
   if(COMBusy[p]!=1){ return; }
   //  �᫨ ���� ��।�� ����� - ���  
   if(VKSPOutStat(p)>SPBSZ-14*2){ return; }
   // ��।��� ����� �� ࠡ�祣� ���� � ���� �ࠩ���
   VKSPOut(p,LSSB[p]);
   // ���㫨�� 䫠��
   CSSB[p]=0; LSSB[p]=0; CSRB[p]=0; TOInp[p]=0; COMBusy[p]=2;
   return;
}
//---------------------------------------------------------------------------







//---------------------------------------------------------------------------
// �ନ஢���� ����� �� ����祭�� ���祭�� ������
vd COMReqCH(ui p,_i WS,_i CHNum){
   _i a=0;
   if(COMBusy[p]!=0) return;
   COMCRC[p]=0;
   COMREQ[p].Begin='@';
   if(WS>99){ WS=99; }
   if(WS< 0){ WS= 0; }
   COMTmp[0]=0; COMTmp[1]=0; COMTmp[2]=0; COMTmp[3]=0; COMTmp[4]=0;
   sprintf(COMTmp,"%d",WS);
   if(COMTmp[1]==0){ COMTmp[1]=COMTmp[0]; COMTmp[0]='0'; }
   COMREQ[p].Addr[0]=COMTmp[0];
   COMREQ[p].Addr[1]=COMTmp[1];
   COMREQ[p].Code='Q';
   if(CHNum>=MCH){ CHNum=0; }
   if(CHNum<   0){ CHNum=0; }
   COMTmp[0]=0; COMTmp[1]=0; COMTmp[2]=0; COMTmp[3]=0; COMTmp[4]=0;
   sprintf(COMTmp,"%d",CHNum);
   for(a=0;a<4;a++){
       if(COMTmp[3]==0){
          COMTmp[3]=COMTmp[2]; COMTmp[2]=COMTmp[1];
          COMTmp[1]=COMTmp[0]; COMTmp[0]='0';
       }
   }
   COMREQ[p].nCH1[0]=COMTmp[0];
   COMREQ[p].nCH1[1]=COMTmp[1];
   COMREQ[p].nCH1[2]=COMTmp[2];
   COMREQ[p].nCH1[3]=COMTmp[3];
   COMREQ[p].nCH2[0]=COMTmp[0];
   COMREQ[p].nCH2[1]=COMTmp[1];
   COMREQ[p].nCH2[2]=COMTmp[2];
   COMREQ[p].nCH2[3]=COMTmp[3];
   memcpy(&COMCRCTmp,&COMREQ[p],14);
   for(a=1;a<12;a++){ COMCRC[p]=COMCRC[p]^COMCRCTmp[a]; }
   COMREQ[p].CRC=COMCRC[p];
   COMREQ[p].End='*';
   LSSB[p]=14;
   memcpy(&SPSBuf[p],&COMREQ[p],LSSB[p]); CSSB[p]=0;
   DPrn(7,1,"07:REQ:%s\n",SPSBuf[p]);
   COMBusy[p]=1;
   return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// �ନ஢���� ����� �� ��⠭���� ���祭�� �� ������
vd COMPutCH(ui p,_i WS,_i CHNum,_f Val){
   _i a=0;
   if(COMBusy[p]!=0) return;
   COMCRC[p]=0;
   if(WS>99){ WS=99; }
   if(WS< 0){ WS= 0; }
   for(a=0;a<5;a++){ COMTmp[a]=0; }
   sprintf(COMTmp,"%d",WS);
   if(COMTmp[1]==0){ COMTmp[1]=COMTmp[0]; COMTmp[0]='0'; }
   COMSET[p].Begin='@';
   COMSET[p].Addr[0]=COMTmp[0];
   COMSET[p].Addr[1]=COMTmp[1];
   COMSET[p].Code='A';
   if(CHNum>=MCH){ CHNum=0; }
   if(CHNum<   0){ CHNum=0; }
   sprintf(COMTmp,"%d",CHNum);
   for(a=0;a<4;a++){
       if(COMTmp[3]==0){
          COMTmp[3]=COMTmp[2]; COMTmp[2]=COMTmp[1];
          COMTmp[1]=COMTmp[0]; COMTmp[0]='0';
       }
   }
   COMSET[p].nCHS[0]=COMTmp[0];
   COMSET[p].nCHS[1]=COMTmp[1];
   COMSET[p].nCHS[2]=COMTmp[2];
   COMSET[p].nCHS[3]=COMTmp[3];
   COMSET[p].Val=Val;
   memcpy(&COMCRCTmp,&COMSET[p],14);
   for(a=1;a<12;a++){ COMCRC[p]=COMCRC[p]^COMCRCTmp[a]; }
   COMSET[p].CRC=COMCRC[p];
   COMSET[p].End='*';
   LSSB[p]=14;
   memcpy(&SPSBuf[p],&COMSET[p],LSSB[p]);
   COMBusy[p]=1; CSSB[p]=0;
   DPrn(7,1,"07:PUT:");
   for(a=0;a<14;a++) DPrn(7,0,"%c",SPSBuf[p][a]);
   DPrn(7,0,"\n");
   return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// �᫨ ����稫� �⢥� �� ����� ���祭�� ������
_i COMGetCH(ui p){
   _i a=0;
   uc CCh[5]={0,0,0,0,0};
   COMCVal[p]=0;
   DPrn(6,1,"06:GET:");
   for(a=0;a<14;a++) DPrn(6,0,"%c",SPTBuf[p][a]);
   DPrn(6,0,"\n");
   if (COMANS[p].Begin!='@'||COMANS[p].End!='*'){ 
       DPrn(6,1,"06:GET:BAD:@*\n"); return(0); 
   }
   COMCRC[p]=0;
   memcpy(&COMCRCTmp,&COMANS[p],14);
   for(a=1;a<12;a++){ COMCRC[p]=COMCRC[p]^COMCRCTmp[a]; }
   if (COMANS[p].CRC!=COMCRC[p]){ 
      DPrn(6,1,"06:GET:BAD:CRC\n"); return(0); 
   }
   CCh[0]=COMANS[p].nCHA[0]; CCh[1]=COMANS[p].nCHA[1];
   CCh[2]=COMANS[p].nCHA[2]; CCh[3]=COMANS[p].nCHA[3];
   COMCCh[p]=atoi(CCh);
   if(COMCCh[p]>MCH-1||COMCCh[p]<1){ 
      DPrn(6,1,"06:GET:BAD:ChNUM\n"); return(0); 
   }
   COMAnsVal(p,'z'); TOInp[p]=0;
   if(COMANS[p].Code=='q'){ COMAnsVal(p,'q'); return(1); }
   if(COMANS[p].Code=='a'){ COMAnsVal(p,'a'); return(2); }
   return(3);
}
//---------------------------------------------------------------------------
