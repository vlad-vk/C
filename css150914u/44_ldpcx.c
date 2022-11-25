// coding: cp866
//---------------------------------------------------------------------------
// ����㧪� PCX 䠩��� �� ����㧪� �࠭�
vd MLoad_PIC(_i md){
   pcxbuf = load_bitmap(LPCX.fn,pal);
   if (!pcxbuf){
       sprintf(SMSG[MMSG],"D: Error load file [%s].",LPCX.fn);
       DPrn (0,0,"%s\n",SMSG[MMSG]);
       sprintf(ErrSTR,SMSG[MMSG]); ErrINI=1;
       STAT[1].cnt=10;  return;
   }
   set_pallete(pal);
   if(md==0){
      blit(pcxbuf,scrsav,0,0, LPCX.X, LPCX.Y, LPCX.W, LPCX.H);
   } else {
      masked_blit(pcxbuf,scrsav,0,0, LPCX.X, LPCX.Y, LPCX.W, LPCX.H);
   }
   destroy_bitmap(pcxbuf);
   return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ���ᮢ�� �ࠩ⮢ ������
_i DrawSPRKNP(_i a){
   if((LSEL[a].sp>=0)&&(LSEL[a].sp<MKNP)){
       if(LKNP[ LSEL[a].sp ].vw>0){
          draw_sprite(scrsav,LKNP[ LSEL[a].sp ].sp[0],
                             LSEL[a].X+LSEL[a].sx, LSEL[a].Y+LSEL[a].sy);
       }
   }
   return(0);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ����㧪� 䠩��� ����ࠦ����, ��� �ࠩ⮢
_i MLoad_Spr(_i b, _i md, _i X, _i Y){
   // ����㧪� ����ࠦ���� �� 䠩���
   if(md==0){
      pcxbut=load_bitmap(LSPR[b].fn,pal);
      if (pcxbut){
          if (LSPR[b].W>WSPR){ LSPR[b].W=WSPR; }
          if (LSPR[b].H>WSPR){ LSPR[b].H=WSPR; }
          clear_to_color(LSPR[b].p,0);
          blit(pcxbut,LSPR[b].p,LSPR[b].SX,LSPR[b].SY,0,0,LSPR[b].W,LSPR[b].H);
      }
      destroy_bitmap(pcxbut);
   }
   // �⮡ࠦ���� ����㦥���� �ࠩ⮢ � ����� 横��� ����ᮢ��
   if(md==1 && LSPR[b].vw==1){
      draw_sprite(scrsav,LSPR[b].p,LSPR[b].X,LSPR[b].Y);
   }
   // �⮡ࠦ���� �ࠩ⮢ �ਪ९������ � ��ப��
   if(md==2 && LSPR[b].vw==2){
      draw_sprite(scrsav,LSPR[b].p,X,Y);
   }
   // �⮡ࠦ���� ��᫥���� �ࠩ⮢
   if(md==3 && LSPR[b].vw==3){
      masked_blit(LSPR[b].p,scrsav,0,0,
                  LSPR[b].X,LSPR[b].Y,LSPR[b].W,LSPR[b].H);
   }
   return(0);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// �ᮢ���� ����᪨� �ਬ�⨢��
vd MDraw_Box(_i a,_i md,_i n, _i X, _i Y, _i W, _i H, _i cs){
   if (LBOX[a].vw==0&&md==0){ return; }
   // ��אַ㣮�쭨��
   if (n==0){
       rect      (scrsav, X, Y, X+W, Y+H, cs);
   }
   if (n==1){
       rectfill  (scrsav, X, Y, X+W, Y+H, cs);
   }
   // ���㦭���
   if (n==2){
       circle    (scrsav, X, Y, W, cs);
   }
   if (n==3){
       circlefill(scrsav, X, Y, W, cs);
   }
   return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// �ᮢ���� 誠�
vd MDraw_Shk(_i n){
   _i  b=0;
   _f hs=0, sg=0, vs=0;
   // ��⠥� ᪮�쪮 ���ᥫ�� ࠢ�� ���� 䮭� 誠��
   hs= text_height(dFnt[LSHK[n].sn].dat)+2;
   // �����뢠�� 蠣 �����
   if (LSHK[n].G==1){ sg= divf(LSHK[n].H,((LSHK[n].h>0)?LSHK[n].h:1)); }
   else             { sg= divf(LSHK[n].W,((LSHK[n].h>0)?LSHK[n].h:1)); }
   // ��।��塞 蠣 誠�� �७��
   vs=divf((LSHK[n].mx-LSHK[n].mn),((LSHK[n].h>0)?LSHK[n].h:1));
   // �뢮� 誠�� �� �࠭
   for(b=0;b<=LSHK[n].h;b++){
       // 誠�� �� �����
       if (LSHK[n].fl==0||LSHK[n].fl==2){
           line (scrsav, LSHK[n].X,
                         LSHK[n].Y+ LSHK[n].H - sg*b,
                         LSHK[n].X+ LSHK[n].W,
                         LSHK[n].Y+ LSHK[n].H - sg*b, LSHK[n].cs );
       }
       // 誠�� �� ���
       if (LSHK[n].fl==1||LSHK[n].fl==2){
           sprintf(try_max,LSHK[n].fr,vs*b);
           text_mode(LSHK[n].cg);
           textout(scrsav,dFnt[LSHK[n].sn].dat,try_max,
                          LSHK[n].X  ,
                          LSHK[n].Y+ LSHK[n].H - sg*b-sg*0.25,
                          LSHK[n].cs);
       }
   }
   return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ����ᮢ�� �࠭�
vd CodusScrRefresh(vd){
   // �뢮� �뤥����� (������ �࠭� scrsav � �࠭ scrout)
   SelMARK();
   // �뢮� �뤥����� �⮡ࠦ������ ��� ��ᬮ�� ⥪�⮢��� 䠩��
   FVMarkCurRec();
   // ���� ।���� �࠭�
   GetXYEL(2);
   //
   show_mouse(scrout);
   blit(scrout,screen,0,0,0,0,XMAX,YMAX);
   show_mouse(NULL);
   if(CDVSync>0){ vsync(); }
#ifdef WINDOW
   PostMessage(myWND,WM_QUERYNEWPALETTE,0,0);
#endif
   return;
}
//---------------------------------------------------------------------------
// �뢮� ��ப� ����� ����� �� �࠭ (��� �஬�������)
vd STAT_to_Screen(vd){
   CodusDirectScr=1; MESOut((MOUT*)&STAT[1],0,0); CodusDirectScr=0;
   return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ������ ⥪�饣� �࠭� �� ������ ������ (Shift-/)
vd SaveScreen(_i md){
   STAT[1].cnt=5;
   if(SPRN.ws<0||SPRN.ws>=MSTT){
      sprintf(SMSG[MMSG],SMSG[107]);        // "D: Not defined work string file name."
      DPrn (0,0,"%s\n",SMSG[MMSG]);
      sprintf(STAT[1].str,SMSG[108]);       // "S: �� ������ ࠡ��� ��ப� ����� 䠩��."
      return; 
   }
   get_pallete(pal);
   scrprn=create_sub_bitmap(screen, SPRN.X, SPRN.Y, SPRN.W, SPRN.H);
   // �᫨ ������ ��ப� ����� 䠩�� > 0
   if(strlen(LSTT[SPRN.ws].ES)>0){
      // �뢥�� ᮮ�饭�� � ����� 䠩��
      sprintf(STAT[1].str,SMSG[177],LSTT[SPRN.ws].ES); STAT[1].cnt=10;
      STAT_to_Screen();
      // �஢�ઠ � ������ ����᪮�� 䠩��
      if(save_bitmap(LSTT[SPRN.ws].ES,scrprn,pal)!=0){
         sprintf(STAT[1].str,SMSG[109],LSTT[SPRN.ws].ES); 
      } else {
         sprintf(STAT[1].str,SMSG[110],LSTT[SPRN.ws].ES); 
      }
   } else {
      sprintf(SMSG[MMSG],SMSG[111],SPRN.ws); 
      DPrn (0,0,"%s\n",SMSG[MMSG]);
      sprintf(STAT[1].str,SMSG[112]);
   }
   destroy_bitmap(scrprn);
   return;
}
//---------------------------------------------------------------------------
