// coding: cp866
//---------------------------------------------------------------------------
// вывод гистограм на экран
_i GisScr(vd){
   _i a=0, s=0, GX=0, GY=0;
   _f mdiv=1.0, CVal=0.0;
   //------------------------------------------------------------------------
   // запомнить экран
   if (LockKBD!=100){ savscr(0); s=1; }
   //------------------------------------------------------------------------
   // вывести гистограммы на экран
   for(a=0;a<HGST;a++){
       if (LGST[a].nv==0){ continue; }
       // проверка выхода за границы
       CVal=Arr_to_Val(LGST[a].na,LGST[a].nv);
       if (CVal<LGST[a].min){ CVal = LGST[a].min; }
       if (CVal>LGST[a].max){ CVal = LGST[a].max; }
       // проверка деления на 0
       mdiv=LGST[a].max-LGST[a].min; if(mdiv==0.00){ mdiv=0.01; }
       // рисуем:
       // фон гистограммы
       rectfill((s>0)?pcxtmp:scrsav, LGST[a].X,
                                     LGST[a].Y,
                                     LGST[a].X+LGST[a].W,
                                     LGST[a].Y+LGST[a].H, LGST[a].cg );
       // если изменение по горизонтали (X)
       if (LGST[a].n>0){
           GY=LGST[a].H;
           GX=divf(LGST[a].W, mdiv) * (CVal-LGST[a].min);
       // если изменение по вертикали (Y)
       } else {
           GY=divf(LGST[a].H, mdiv) * (CVal-LGST[a].min);
           GX=LGST[a].W;
       }
       // гистограмма
       rectfill((s>0)?pcxtmp:scrsav, LGST[a].X,
                                     LGST[a].Y+LGST[a].H-GY,
                                     LGST[a].X+GX,
                                     LGST[a].Y+LGST[a].H, LGST[a].cs );
       // рамка
       if (LGST[a].b>0){
           rect    ((s>0)?pcxtmp:scrsav, LGST[a].X,
                                         LGST[a].Y,
                                         LGST[a].X+LGST[a].W,
                                         LGST[a].Y+LGST[a].H, LGST[a].cb);
       }
   }
   //------------------------------------------------------------------------
   // восстановить экран
   if (s>0){ savscr(1); }
   //------------------------------------------------------------------------
   return(0);
}
//---------------------------------------------------------------------------
