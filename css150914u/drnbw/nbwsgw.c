// coding: cp866
//---------------------------------------------------------------------------
#include "../include.h"
#include "../define.h"
#include "nbwsgw.h"

//---------------------------------------------------------------------------
vd  WS_NB__ZeroWS_R1(uc ncb){
    memset(&WS_R1[ncb], 0, sizeof(WS_R1[ncb]));
    return;
}
//---------------------------------------------------------------------------
vd  WS_NB__ZeroWS_R2(uc ncb){
    memset(&WS_R2[ncb], 0, sizeof(WS_R2[ncb]));
    return;
}
//---------------------------------------------------------------------------
vd  WS_NB__ZeroWS_R7(uc ncb){
    memset(&WS_R7[ncb], 0, sizeof(WS_R7[ncb]));
    return;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
vd  WS_NB__ZeroWS_S1(uc ncb){
    memset(&WS_S1[ncb], 0, sizeof(WS_S1[ncb]));
    return;
}
//---------------------------------------------------------------------------
vd  WS_NB__ZeroWS_S2(uc ncb){
    memset(&WS_S2[ncb], 0, sizeof(WS_S2[ncb]));
    return;
}
//---------------------------------------------------------------------------
vd  WS_NB__ZeroWS_S7(uc ncb){
    memset(&WS_S7[ncb], 0, sizeof(WS_S7[ncb]));
    return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ��ନ஢���� �⢥⭮�� ����� ��� �⥢��� �⢥�
vd WS_NB__Answer(ui inncb, ui ouncb){
   ui i =0;
   ui NV=0;
   ul ltime=0;
   // ��।�� ���� �ਭ�⮣� ����� � �������� �����
   memcpy(&WS_R1[inncb],&WS_RD[inncb],504);
   // ������ ���� �ਭ�⮣� �����
   WS_NB__ZeroRBF(inncb);
   // ������ ���� ���뫪� �⢥� �� �����
   WS_NB__ZeroSBF(ouncb);
   // ������ �������� ���뫪� �⢥� �� �����
   WS_NB__ZeroWS_S2(ouncb);
   WS_S2[ouncb].Code=2;                          // ���
   WS_S2[ouncb].NReq=WS_R1[inncb].NReq;          // ����� �����
   ltime=time(NULL);
   WS_S2[ouncb].Time=(ul)ltime;                  // �६� ����������
   if (WS_R1[inncb].NData>=MRCH){ WS_R1[inncb].NData=MRCH-1; }
   for(i=0; i<WS_R1[inncb].NData; i++){
       NV=(WS_R1[inncb].ReqData[i] & 0x0fff);    // �����
       WS_S2[ouncb].Data[i]=CB[NV].v;            // ���祭�� � ����
   }
   WS_S2[ouncb].Const[1]=2;
   // ������ �������� �ਭ�⮣� �����
   WS_NB__ZeroWS_R1(inncb);
   return;
}
//---------------------------------------------------------------------------


//--------------------------------------------------------------------------
// ��।�� �ਭ���� �� �� ������ � ���ᨢ ������
vd WS_NB__GetFromNet(ui n){
   ui i=0,NV=0;
   // ��।�� ���� �ਭ���� ������ � �������� �ਭ���� ������
   memcpy (&WS_R7[n],&WS_RD[n],504);
   // ������ ���� �ਥ�� �ਭ���� ������
   WS_NB__ZeroRBF(n);
   if (WS_R7[n].NData>79){ WS_R7[n].NData=79; }
   for(i=0; i<WS_R7[n].NData; i++){
      NV=WS_R7[n].ChNum[i] & 0x0fff;
//    CBN [NBRQ[n].ca[i]] = WS_R2[n].Data[i];
//    Val_in_DBB(NV,WS_R7[n].ChData[i]);
   }
   // ����� �������� �ਥ�� �ਭ���� ������
   WS_NB__ZeroWS_R7(n);
   return;
}
//---------------------------------------------------------------------------


//--------------------------------------------------------------------------
// ��।�� �ਭ���� �� �� �⢥⮢ � ��騩 ���ᨢ
vd WS_NB__GetReq(ui n){
   ui b=0, z=0;
   // ��।��� ����� �� ���� �ਥ�� � �������� �⢥�
   memcpy (&WS_R2[n],&WS_RD[n],504);
   if(Debug(33)>0){
      DPrn (33,0,"\n[RECV BUF:\n");
      for(b=0;b<504;b++){ DPrn(33,0,"%c",WS_RD[n][b]); }
      DPrn (33,0,"\n]\n");
   }
   // ������ ���� �ਥ�� �⢥�
   WS_NB__ZeroRBF(n);
   // ����� �����
   z= WS_R2[n].NReq;
   if(EKNBWCNT<EKNBWREQ){ EKNBWCNT++; }
   // �஢���� �ࠢ��쭮��� �ਭ�⮣� ���� �����:
   // �᫨ ����� ��襤襣� ����� � �����⨬�� �।���� �
   // ����� ��ࢮ�� ������ � ��������� ����� > 0

   // printf("z=%d MQR=%d NBRQ[z].cq[0]=%d",z,MQR,NBRQ[z].cq[0]);

   if(z>0 && z<MQR && NBRQ[z].cq[0]>0){
       DPrn(33,0,"ANREQ=%d  ",z);
       // ��� ������� ������ �����
       for(b=0;b<MRCH;b++){
           // �᫨ ����⨫�� �ਧ��� ���� ᯨ᪠ ����訢����� �������
           if (NBRQ[z].cq[b]<1){ break; }
           if (FNBS[NBRQ[z].ca[b]]==2){ continue; }

           // �᫨ �࠭ ��������- ����� � DBNW[] ��� �⮡ࠦ����
           // (�� ��� ��娢�) �����﫨�� ⮦�.
           // ���⮬� �ਭ��� � ��ண� ����� ���祭��
           // (��� �।��饣� �࠭�) ���������  � CB[] ������ �࠭�.
           // ���� �� �ਤ�� ����� ��᫠��� � ������ �࠭�.

           // �᫨ ��� �ਧ���� ����� � ��娢- ����� �� ����� ���
           // �⮡ࠦ����.
           // �᫨ ⥪�騩 ����� ����� ����� ������������-
           // ����� �� ����� � �।��饣� �࠭�- ��� �� ���ਭ�����
           if(CB[ DNBW [NBRQ[z].ca[b]] ].ar==0){
              if(EKNBWCNT<EKNBWREQ){ 
                 DPrn(33,0,"AXX%d:V%f  ",NBRQ[z].cq[b],WS_R2[n].Data[b]);
                 continue; 
              }
           }
           // ����- ������� ���祭�� ����訢������ ������ � ���ᨢ� �������
           CBN [ NBRQ[z].ca[b] ] = WS_R2[n].Data[b];
           if((CB[ DNBW [NBRQ[z].ca[b]] ].t==1)||
              (CB[ DNBW [NBRQ[z].ca[b]] ].t==3)){
               CB[ DNBW [NBRQ[z].ca[b]] ].v = WS_R2[n].Data[b];
           }
           DPrn(33,0,"ANQ%d:V%f  ",NBRQ[z].cq[b],WS_R2[n].Data[b]);
       }
       // ���㫨�� ᯨ᮪ ������� �ਭ�⮣� ����� �����
       for(b=0;b<MRCH;b++){ NBRQ[z].cq[b]=0; NBRQ[z].ca[b]=0; }
   } else {
      DPrn(33,0,"���ࠢ���� ����� ��� ������ �����.\n");
   }
   // ������ �������� �ਥ�� �⢥�
   WS_NB__ZeroWS_R2(n);
   return;
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// ��⠭����� �⥢�� ��� 㤠������ ��設�
ui NB_SetRName(ui NWS){
   if ((NWS>99)||(NWS<1)){ return(0); }     // �஢��.����� �⠭樨
   sprintf(nbwtmpcc,"%d",NWS);              // �८��.����� �⠭樨 � ��ப�
   WS_NBName[6]=nbwtmpcc[0];                // ������ ����� �⠭樨 � ���
   WS_NBName[7]=nbwtmpcc[1];    
   return(1);
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// ����� ���祭�� �������
ui WS_NB__ReqToNet(ui NWS){
   _i b=0, j=0, i=200;
   NB_SetRName(NWS);
   // ������� ����� ᢮������� NCB
   i=WS_NB__AvNCB(); CurNCFQ=0;
   // �᫨ ���� ᢮����� NCB
   if (i>=0){
      // ������ ���� � �������� ���뫪� ����ᮢ
      WS_NB__ZeroSBF(i);
      WS_NB__ZeroWS_S1(i);
      // ��������� �������� �����:
      // ��� ����樨 �����
      WS_S1[i].Code= 1;
      // ����� �����
      WS_S1[i].NReq=QRC;
      DPrn(DEBNBQ,0,"SNREQ=%d  ",QRC);
      // ����� ����訢����� ������� �� 㤠������ �⠭樨
      for(j=0;j<MRCH;j++){
          // �᫨ ����� ������ ����� 1-��� (0-�ਧ��� ���� ����� �������)
          if(NBRQ[QRC].cq[j]<1){ break; }
          // ������� ����� ����訢������ ������ � �������� �����
          WS_S1[i].ReqData[j]=NBRQ[QRC].cq[j];
          DPrn(DEBNBQ,0,"NBQ%d  ",NBRQ[QRC].cq[j]);
      }
      // ���-�� ����訢����� �������
      WS_S1[i].NData=(uc)j;
      // ��।��� ��ନ஢����� �������� ����� � ���� ���뫮�
      memcpy (&WS_SD[i],&WS_S1[i],504);
      // �᫨ ���-�� ����訢����� ������� ����� ���- ��᫠�� �����
      if(j>0){ WS_NB__Send(i,WS_NBName); }
      // ������ ���� ����ᮢ
      if(Debug(DEBNBQ)>0){
         DPrn(DEBNBQ,0,"\n[REQS BUF:\n");
         for(b=0;b<504;b++){ DPrn(DEBNBQ,0,"%c",WS_SD[i][b]); }
         DPrn(DEBNBQ,0,"\n]\n");
      }
      // �����⠭����� ���稪 ��।�
      QRC++; if(QRC>=MQR){ QRC=1; }
      CurNCBQ=i; CurNCFQ=1;
      return(0);
// } else {
//     sprintf(STAT_str,"ReqToNet: ��� ᢮������� NCB."); STAT_cnt=10;
//     DPrn(DEBNBQ,0,"ReqToNet: ��� ᢮������� NCB.\n");
   }
   return(2);
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// ���뫪� ��⠭����
ui WS_NB__SetToNet(ui NWS){
   _i a=0,  b=0 , j=0, i=200, n=0;
   NB_SetRName(NWS);
   // ������� ����� ᢮������� NCB
   i=WS_NB__AvNCB();  CurNCFS=0;
   // �᫨ ���� ᢮����� NCB
   if (i>=0 && i<NCBWR){
      // ������ ���� � �������� ���뫪� ��⠭����
      WS_NB__ZeroSBF(i);
      WS_NB__ZeroWS_S7(i);
      // ��������� �������� ���뫪�:
      // ��� ����樨 ���뫪�
      WS_S7[i].Code =7;
      // ��⠭��������� ������
      for(j=0,n=0;j<MSCH;j++){
          if(NBST[STC].cq[j] < 1){ break; }
          if(CB[ DNBW[ NBST[STC].ca[j] ] ].t==2||
             CB[ DNBW[ NBST[STC].ca[j] ] ].t==3){
             DPrn(31,0,"STN%d:V%f  ",NBST[STC].cq[j],CBN[NBST[STC].ca[j]]);
             WS_S7[i].ChNum [n]=(_s)NBST[STC].cq[j];
             WS_S7[i].ChData[n]=(_f)CBN[NBST[STC].ca[j]]; n++;
          }
      }
      DPrn(31,0,"\nJJJ%d\n",j);
      // ���-�� ���뫠���� �������
      WS_S7[i].NData=(_s)n;
      // ��।��� ��ନ஢����� �������� ��⠭���� � ���� ���뫮�
      memcpy (&WS_SD[i],&WS_S7[i],504);
      // �᫨ ���-�� ��⠭���������� ������� ����� ���- ��᫠�� �����
      if(n>0){ WS_NB__Send(i,WS_NBName); }
      // ������ ���� ���뫮�
      if(Debug(31)>0){
         DPrn(31,0,"\n[SEND BUF:\n");
         for(b=0;b<504;b++){ DPrn(31,0,"%c",WS_SD[i][b]); }
         DPrn(31,0,"\n]\n");
      }
      // ���㫨�� ᯨ᮪ �������, ����� ��।��� �� ��⠭����
      for(a=0;a<MSCH;a++){ NBST[STC].cq[a]=0; NBST[STC].ca[a]=0; }
      // �����⠭����� ���稪 ��।�
      STC++; if(STC>=MQR){ STC=1; }
      CurNCBS=i; CurNCFS=1;
   } else {
       sprintf(STAT_str,"SetToNet: ��� ᢮������� NCB."); STAT_cnt=10;
       DPrn(31,0,"SetToNet: ��� ᢮������� NCB.\n");
   }
   return(0);
}
//--------------------------------------------------------------------------
