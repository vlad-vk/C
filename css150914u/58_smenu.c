// coding: cp866
//---------------------------------------------------------------------------
// m=0-��࠭��� �࠭ ��� ����, m=1-����⠭����� �࠭, n-����� � ���ᨢ�
_i SRMenu(_i m, _i n){
   _i fr =0, col=0;
   if(((n<0)||(n>=HMNU))&&(LMNU[n].k==0)){ return(0); }
   if (m==0){
       // ��࠭��� �࠭ ��� ���� 
       blit(scrsav, pcxmnu,
                    LMNU[n].X,    LMNU[n].Y,
                    0,            0,           
                    LMNU[n].W+1,  LMNU[n].H+1 );
       // �᫨ ��।����� ��� ���� ����
       if (LMNU[0].X!=0){ 
           // �᫨ 梥� ����� 1000- �ᮢ��� ������� ࠬ��
           if (LMNU[n].c>1000){ fr=1; col=LMNU[n].c-1000; }
           else               { fr=0; col=LMNU[n].c; }
           // �᫨ ��।���� 梥� ࠬ��- ���ᮢ��� ࠬ��
           if (LMNU[n].c>0){
               rect(scrsav, LMNU[n].X, LMNU[n].Y,
                            LMNU[n].X+ LMNU[n].W,
                            LMNU[n].Y+ LMNU[n].H, col);
               if (fr>0){
                   rect(scrsav, LMNU[n].X+1, LMNU[n].Y+1,
                                LMNU[n].X+ LMNU[n].W-1,
                                LMNU[n].Y+ LMNU[n].H-1, col);
               }
           }
       }
   }
   if (m==1){
       blit(pcxmnu, scrsav,
                    0,           0,
                    LMNU[n].X,   LMNU[n].Y,
                    LMNU[n].W+1, LMNU[n].H+1 );
   }
   return(0);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// m=0-��࠭��� �࠭, m=1-����⠭����� �࠭, n-����� � ���ᨢ�
_i SRSelect(_i m, _i n){
   if((n<0)||(n>=HSEL)){ return(0); }
   // ��������� ��砫�� �࠭ ��� ��������     
   if (m==0){
        blit(scrsav, pcxsss,
                     LSEL[n].X+LSEL[n].sx,  LSEL[n].Y+LSEL[n].sy,
                     0,                     0,
                     LKNP[ LSEL[n].sp ].W,  LKNP[ LSEL[n].sp ].H );
   }      
   // ����⠭����� ���������� �࠭
   if (m==1){
        blit(pcxsss, scrsav,
                     0,                     0,
                     LSEL[n].X+LSEL[n].sx,  LSEL[n].Y+LSEL[n].sy,
                     LKNP[ LSEL[n].sp ].W,  LKNP[ LSEL[n].sp ].H );
   }
   return(0);
}
//---------------------------------------------------------------------------

