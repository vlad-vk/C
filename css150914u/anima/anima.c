// coding: cp866
//---------------------------------------------------------------------------
// �뢮� ���⨭�� �����樨
#include <allegro.h>
#include "../include.h"
#include "../define.h"
#include "anima.h"


//---------------------------------------------------------------------------
vd Animation(_i md){
   _i  a=0, b=0, w=0, sc=0, fl=0, aX=0, aY=0;

   //------------------------------------------------------------------------
   // ����㧪� �ࠩ⮢ �� �⥭�� ���䨣��樨
   // (�뢠�� ᫥�� �� �㭪樨 clear_to_color
   if (md==0){
       for(a=0;a<MANI;a++){
           if (LANI[a].nv==0){ continue; }
           pcxani=load_bitmap(LANI[a].fp,pal);
           if (pcxani){
               if (LANI[a].hp<=0){ LANI[a].hp=1; }
               if (LANI[a].hp>MASP){ LANI[a].hp=MASP; }
               for(b=0;b<LANI[a].hp;b++){
                   w=LANI[a].W*b+b;
                   clear_to_color(LANI[a].sp[b],0);
                   blit (pcxani,LANI[a].sp[b],0+w,0,0,0,LANI[a].W,LANI[a].H);
               }
           }   else   {
               DPrn(0,0,"�訡�� ����㧪� 䠩�� �����樨 [%s]\n",LANI[a].fp);
               LANI[a].vw=0;
           }
           destroy_bitmap(pcxani);
           LANI[a].x1=LANI[a].X;
           LANI[a].y1=LANI[a].Y;
           Val_to_Arr(LANI[a].na, LANI[a].nf,0, CBMODNO, 'a');
           LANI[a].vx=LANI[a].vw;
       }
   }
   //------------------------------------------------------------------------

   //------------------------------------------------------------------------
   // �⮡ࠦ���� ���⨭�� �����樨
   if (md==1){
       for(a=0;a<MANI;a++){
           if (LANI[a].nv==0){ continue; }
           fl=(_i)Arr_to_Val(LANI[a].na, LANI[a].nf);
           // �몫�祭�� �����樨
           if (fl>=400000){
               LANI[a].vw =LANI[a].vx;
               Val_to_Arr(LANI[a].na, LANI[a].nf, 0, CBMODNO, 'a');
           }
           if (LANI[a].vw>0){
               // ������� ⥪�騥 ���祭�� �易���� ��६�����
               sc=(_i)Arr_to_Val(LANI[a].na, LANI[a].nv);    // ����� �ࠩ�
               if (sc <=0){ sc=0; }
               if (sc >=LANI[a].hp){ sc=LANI[a].hp-1; }
               if (LANI[a].nf>0){
                  aX=(_i)Arr_to_Val(LANI[a].na, LANI[a].nx); // ������� �� X
                  aY=(_i)Arr_to_Val(LANI[a].na, LANI[a].ny); // ������� �� Y
               }
               // ��������� �࠭ ��� �ࠩ⮬
               blit( scrsav,    LANI[a].ss,
                     LANI[a].X, LANI[a].Y,             0,          0,
                     LANI[a].W, LANI[a].H
                   );
               // �⮡ࠧ��� ���⨭�� �ࠩ�
               draw_sprite( scrsav,
                            LANI[a].sp[sc],
                            LANI[a].X,
                            LANI[a].Y
                          );
               //------------------------------------------------------------
               // ��������� ���न���� �뢮�� �ࠩ� (��� ��᫥����.���⠭.)
               LANI[a].sX=LANI[a].X;
               LANI[a].sY=LANI[a].Y;
               //------------------------------------------------------------
               if (fl>0){
                   LANI[a].X=LANI[a].X+aX;
                   LANI[a].Y=LANI[a].Y+aY;
                   // ��室 �� �࠭��� X
                   if (LANI[a].X > LANI[a].XM){
                       LANI[a].X = LANI[a].XM;
                       Val_to_Arr(LANI[a].na, LANI[a].nf, 200001, CBMODNO, 'a');
                   }
                   if (LANI[a].X < LANI[a].x1){
                       LANI[a].X = LANI[a].x1;
                       Val_to_Arr(LANI[a].na, LANI[a].nf, 200002, CBMODNO, 'a');
                   }
                   // ��室 �� �࠭��� Y
                   if (LANI[a].Y > LANI[a].YM){
                       LANI[a].Y = LANI[a].YM;
                       Val_to_Arr(LANI[a].na, LANI[a].nf, 200003, CBMODNO, 'a');
                   }
                   if (LANI[a].Y < LANI[a].y1){
                       LANI[a].Y = LANI[a].y1;
                       Val_to_Arr(LANI[a].na, LANI[a].nf, 200004, CBMODNO, 'a');
                   }
               }
               //------------------------------------------------------------
           }
       }
   }
   //------------------------------------------------------------------------

   //------------------------------------------------------------------------
   // ����⠭������� �࠭� ��� �ࠩ⮬
   if (md==2){
       for(a=HANI;a>=0;a--){
           if (LANI[a].nv==0){ continue; }
           // ����⠭������� ������������ �࠭�
           if (LANI[a].vw>0){
               blit( LANI[a].ss,     scrsav,
                              0,          0,  LANI[a].sX ,  LANI[a].sY,
                     LANI[a].W ,  LANI[a].H
                   );
           }
       }
   }
   //------------------------------------------------------------------------

   //------------------------------------------------------------------------
   // ��⠭���� � �����⨥ �����樨 �� ������ ������
   if (md==3){
       for(a=0;a<MANI;a++){
           if (LANI[a].nv==0){ continue; }
           fl=(_i)Arr_to_Val(LANI[a].na, LANI[a].nf);
           if (LANI[a].vw==a+MANI*2 && fl<300000 ){
               if(LANI[a].nf>0){ Val_to_Arr(LANI[a].na, LANI[a].nf, 300000, CBMODNO, 'a'); }
               else            { Val_to_Arr(LANI[a].na, LANI[a].nf, 400000, CBMODNO, 'a'); }
           }
       }
   }
   //------------------------------------------------------------------------

   //------------------------------------------------------------------------
   // ��⠭���� � �����⨥ �����樨
   if (md==4){
       for(a=0;a<MANI;a++){
           if (LANI[a].nv==0){ continue; }
           fl=(_i)Arr_to_Val(LANI[a].na, LANI[a].nf);
           if (LANI[a].vw==a+MANI*3 && fl<300000 ){
               if(LANI[a].nf>0){ Val_to_Arr(LANI[a].na, LANI[a].nf, 300000, CBMODNO, 'a'); }
               else            { Val_to_Arr(LANI[a].na, LANI[a].nf, 400000, CBMODNO, 'a'); }
           }
       }
   }
   //------------------------------------------------------------------------
   return;
}
//---------------------------------------------------------------------------
