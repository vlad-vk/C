// coding: cp866
//---------------------------------------------------------------------------
// �뢮� ���⮣ࠬ �� �࠭
_i GisScr(vd){
   _i a=0, s=0, GX=0, GY=0;
   _f mdiv=1.0, CVal=0.0;
   //------------------------------------------------------------------------
   // ��������� �࠭
   if (LockKBD!=100){ savscr(0); s=1; }
   //------------------------------------------------------------------------
   // �뢥�� ���⮣ࠬ�� �� �࠭
   for(a=0;a<HGST;a++){
       if (LGST[a].nv==0){ continue; }
       // �஢�ઠ ��室� �� �࠭���
       CVal=Arr_to_Val(LGST[a].na,LGST[a].nv);
       if (CVal<LGST[a].min){ CVal = LGST[a].min; }
       if (CVal>LGST[a].max){ CVal = LGST[a].max; }
       // �஢�ઠ ������� �� 0
       mdiv=LGST[a].max-LGST[a].min; if(mdiv==0.00){ mdiv=0.01; }
       // ��㥬:
       // 䮭 ���⮣ࠬ��
       rectfill((s>0)?pcxtmp:scrsav, LGST[a].X,
                                     LGST[a].Y,
                                     LGST[a].X+LGST[a].W,
                                     LGST[a].Y+LGST[a].H, LGST[a].cg );
       // �᫨ ��������� �� ��ਧ��⠫� (X)
       if (LGST[a].n>0){
           GY=LGST[a].H;
           GX=divf(LGST[a].W, mdiv) * (CVal-LGST[a].min);
       // �᫨ ��������� �� ���⨪��� (Y)
       } else {
           GY=divf(LGST[a].H, mdiv) * (CVal-LGST[a].min);
           GX=LGST[a].W;
       }
       // ���⮣ࠬ��
       rectfill((s>0)?pcxtmp:scrsav, LGST[a].X,
                                     LGST[a].Y+LGST[a].H-GY,
                                     LGST[a].X+GX,
                                     LGST[a].Y+LGST[a].H, LGST[a].cs );
       // ࠬ��
       if (LGST[a].b>0){
           rect    ((s>0)?pcxtmp:scrsav, LGST[a].X,
                                         LGST[a].Y,
                                         LGST[a].X+LGST[a].W,
                                         LGST[a].Y+LGST[a].H, LGST[a].cb);
       }
   }
   //------------------------------------------------------------------------
   // ����⠭����� �࠭
   if (s>0){ savscr(1); }
   //------------------------------------------------------------------------
   return(0);
}
//---------------------------------------------------------------------------
