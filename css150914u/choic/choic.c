// coding: cp866
//---------------------------------------------------------------------------
// �롮� ��ப� �� ��६�����
#include <allegro.h>
#include "../include.h"
#include "../define.h"
#include "choic.h"
//---------------------------------------------------------------------------
vd Choice(_i md){
   _i a=0, na=0, h=0;
// _i b=0, w =0, l=0, s=0;
   //------------------------------------------------------------------------
   //  ��।������ ��砫��� ���祭�� ��ப �뢮�� �� ����㧪� �࠭�
   if (md==0){
       for(a=0;a<HCST;a++){
           // �㫥�� �祩�� ���ᨢ�� �� ��ࠡ��뢠����
           if (LCST[a].nv==0){ continue; }
           // ��⠭���� ��砫��� ���祭��
           if (LCST[a].sn< 0 || LCST[a].sn>MFNT){ LCST[a].sn= 17; }
           if (LCST[a].cs< 0 || LCST[a].cs> 254){ LCST[a].cs= 14; }
           if (LCST[a].cg<-1 || LCST[a].cg> 254){ LCST[a].cg=  0; }
           if (LCST[a].X < 0 || LCST[a].X >XMAX){ LCST[a].X =  0; }
           if (LCST[a].Y < 0 || LCST[a].Y >YMAX){ LCST[a].Y =  0; }
           if (LCST[a].W < 0 || LCST[a].W >XMAX){ LCST[a].W =100; }
           LCST[a].vl =-1;
           // �᫨ 䮭 ��।����, ��� �஧���- ��������� ��砫�� 䮭
           if (LCST[a].cg< 0){
               h =text_height(dFnt[LCST[a].sn].dat);
               blit(scrsav,LCST[a].pb,LCST[a].X,LCST[a].Y,0,0,LCST[a].X,h);
           }
       }
   }
   //------------------------------------------------------------------------
   if (md==1){
       for(a=0;a<HCST;a++){
//         DPrn(0,1,"LCST[%03d]: HCST=%d na=%d nv=%d vl=%0.f vw=%d \n", 
//                   a,  HCST,   na,   LCST[a].nv,  LCST[a].vl,  LCST[a].vw);
           if (LCST[a].nv==0){ continue; }
           if (LCST[a].vw >0){
               // ������� ����� ��ப� ��� ��࠭���� ����
               h =text_height(dFnt[LCST[a].sn].dat);
               // ������� ⥪�饥 ���祭�� ��६����� �易���� � 
               // ��ப�� �롮�
               na=(_i)Arr_to_Val(LCST[a].na, LCST[a].nv);
               // �஢���� �����⨬� �࠭��� ����祭���� ���祭��
               if (na<     0){ na=      0; }
               if (na>=MCSTV){ na=MCSTV-1; }
               // �᫨ ���祭�� �� �����﫮�� � 䫠� �⮡ࠦ���� ��
               // �������᪨�- ��室��
               if (na==LCST[a].vl&&LCST[a].vw!=5){ continue; }
               // ��������� ⥪�饥 ���祭��
               LCST[a].vl=na;
               // �᫨ 䮭 ��।����, ��� ����ﭭ�- ���ᮢ���
               // ��אַ㣮�쭨� 䮭�  ��� ��ப� �롮�
               if (LCST[a].cg>=0){
                   rectfill(scrsav,
                       LCST[a].X,
                       LCST[a].Y,
                       LCST[a].X+ LCST[a].W,
                       LCST[a].Y+ h,
                       LCST[a].cg );
               }
//----------------------------------------------------------------------------
//             // �᫨ ��ப� ������ - ��१��� ��ப�
//             s=strlen(LCST[a].C[na]);
//             for(b=0;b<s;b++){
//                 w=text_length(dFnt[LCST[a].sn].dat,LCST[a].C[na]);
//                 l=strlen(LCST[a].C[na]);
//                 if (w<=LCST[a].W){ break; }
//                 else             { LCST[a].C[na][l-1]=0; }
//             }
//----------------------------------------------------------------------------
               // �᫨ 䮭 ��� �뢮����� ��ப� ��।����, ��� �஧���
               if (LCST[a].cg<0){
                   // �஢���� �࠭��� ��������� �࠭� ��� �뢮�� ��ப�
                   if (LCST[a].W>MCSTPBX){ LCST[a].W=MCSTPBX; }
                   if (h        >MCSTPBY){ h        =MCSTPBY; }
                   // ����⠭����� ���������� �࠭ 䮭�
                   blit(LCST[a].pb,scrsav,0,0,LCST[a].X,LCST[a].Y,LCST[a].W,h);
               }
               // ��⠭����� 梥� � ���� �뢮�� ��ப
               STR_cs=LCST[a].cs;
               STR_cg=LCST[a].cg;
               STR_sn=LCST[a].sn;
               STR_Out(LCST[a].C[na], STR_sn, LCST[a].X,LCST[a].Y,LCST[a].W,1);
//----------------------------------------------------------------------------
//             // �뢥�� ��ப� �� �࠭
//             text_mode(LCST[a].cg);
//             textout(scrsav,dFnt[LCST[a].sn].dat,
//                     LCST[a].C[na],
//                     LCST[a].X,
//                     LCST[a].Y,
//                     LCST[a].cs);
//----------------------------------------------------------------------------
           }
       }
   }
   //------------------------------------------------------------------------
   return;
}
//---------------------------------------------------------------------------
