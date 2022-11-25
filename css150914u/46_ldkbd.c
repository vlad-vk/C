// coding: cp866
//---------------------------------------------------------------------------
// Загрузка файла изображения по нажатию клавиши
_i KBDLoadPCX(_i n,_i mode){
   _i a=LockKBD, k=5;
   // загрузка рисунка по нажатию клавиши
   if((LWIN[n].vw==0)&&(mode==0)&&(a==100)&&(LWIN[n].W>0)){
      if (n>=MWIN){ return(0); }
      get_pallete(palsav);
      blit(scrsav,pcxsav,LWIN[n].X,LWIN[n].Y,0,0,LWIN[n].W,LWIN[n].H);
      LPCX.X=LWIN[n].X; LPCX.Y=LWIN[n].Y; LPCX.W=LWIN[n].W; LPCX.H=LWIN[n].H;
      strcpy(LPCX.fn,LWIN[n].fn);
      MLoad_PIC(1);
      LWIN[n].vw=3; LockKBD=n; k=0;
      return(1);
   }
   // если была нажата клавиша выхода - сначала восстанавливаем экран
   if((mode>0)&&(LWIN[a].vw> 2)&&(LockKBD!=100)){
      if (a>=MWIN){ return(0); }
      set_pallete(palsav);
      blit(pcxsav,scrsav,0,0,LWIN[a].X,LWIN[a].Y,LWIN[a].W,LWIN[a].H);
      LWIN[a].vw=0; LockKBD=100; k=1;
      if (LANI[atoi(LKBD[a].str)].vw==2){ LANI[atoi(LKBD[a].str)].vw=0; }
   }
   return(0);
}
//---------------------------------------------------------------------------
