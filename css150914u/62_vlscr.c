// coding: cp866
//---------------------------------------------------------------------------
// вывод значений переменных
_i ValScr(vd){
   _i a=0, b=0, s=0, j=0;
   //------------------------------------------------------------------------
   // вывести переменные на экран
   for(a=1;a<HVAL;a++){
      // выводить на экран, если номер в базе канлов > 0
      if (LVAL[a].nv==0){ continue; }
      // Общая база каналов
      if (LVAL[a].na==0){ sprintf(LVAL[a].str,LVAL[a].sf,CB[LVAL[a].nv].v); }
      // Локальная база каналов
      if (LVAL[a].na==1){ sprintf(LVAL[a].str,LVAL[a].sf,CBL[LVAL[a].nv]);  }
      // База каналов драйвера COM портов
      if (LVAL[a].na==2){ sprintf(LVAL[a].str,LVAL[a].sf,CBS[LVAL[a].nv]);  }
      // База каналов драйвера NetBios
      if (LVAL[a].na==3){ sprintf(LVAL[a].str,LVAL[a].sf,CBN[LVAL[a].nv]);  }
      // База каналов драйвера TCP/IP
      if (LVAL[a].na==4){ sprintf(LVAL[a].str,LVAL[a].sf,CBT[LVAL[a].nv]);  }
      // Массив временных срезов трендов
      if (LVAL[a].na==5){ sprintf(LVAL[a].str,LVAL[a].sf,VTRT[LVAL[a].nv]); }
      // Массив переменных комманды 'y'
      if (LVAL[a].na==6){ sprintf(LVAL[a].str,LVAL[a].sf, YCM[LVAL[a].nv]); }
      // Массив временных переменных
      if (LVAL[a].na==7){ sprintf(LVAL[a].str,LVAL[a].sf,RTMP[LVAL[a].nv]); }
      // выводить на экран, если установлен флаг отображения
      if (LVAL[a].vw==0){ continue; }
      // рисуем фон
      b=text_height(dFnt[LVAL[a].sn].dat);
      rectfill(scrsav, LVAL[a].X,
                       LVAL[a].Y,
                       LVAL[a].X+ LVAL[a].W,
                       LVAL[a].Y+ b,
                       LVAL[a].cg );
      // каждый цикл в конец строки дописываем 0, пока длинна строки не
      // станет равной, определеннию LVAL[].W
      for(j=0;j<strlen(LVAL[a].str);j++){
         // определить длинну строки
         b=text_length(dFnt[LVAL[a].sn].dat,LVAL[a].str);
         // если строка большая - обнуляем последний символ
         if (b>LVAL[a].W){ LVAL[a].str[strlen(LVAL[a].str)-1]=0; }
         else { break; }
      }
      // определить цвет вывода символов
      LVAL[a].cc=LVAL[a].cs;
      // определить цвет символов при переходе текущего значения за границы
      // (только для общей базы каналов)
      if (LVAL[a].na==0){
         if (CB[LVAL[a].nv].v>CB[LVAL[a].nv].h){ LVAL[a].cc=LVAL[a].ce; }
         if (CB[LVAL[a].nv].v<CB[LVAL[a].nv].l){ LVAL[a].cc=LVAL[a].ce; }
//  DPrn(0,0,"Val=%f Str=[%s] LVAL[a].nv=%d a=%d CB.h=%f CB.l=%f \n",
//       CB[LVAL[a].nv].v, LVAL[a].str, LVAL[a].nv, a, CB[LVAL[a].nv].h,
//       CB[LVAL[a].nv].l);
      }
      text_mode(LVAL[a].cg);
      textout(((s>0)?pcxtmp:scrsav),dFnt[LVAL[a].sn].dat,LVAL[a].str,
                          LVAL[a].X,     LVAL[a].Y,  LVAL[a].cc);
   }
   //------------------------------------------------------------------------
   return(0);
}
//---------------------------------------------------------------------------
