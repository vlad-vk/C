// coding: cp866
//---------------------------------------------------------------------------
// чтение файла сообщений
_i Read_Alarm(vd){
   _i  lc=1, a=0, b=0;
   HALR=0;
   strcpy(FNCfg,FNAlmR);
   ClsFlg();
   strcpy(lcsok,"mo");
   //------------------------------------------------------------------------
   // разбор файла сообщений
   Init_CFG(1);
   for(a=0;a<MCYC;a++){
      if (Init_CFG(2)){ break; }
      for(lca=0;lca<FHCfg;lca++){
         lc=Load_CFG('0','i',(CVT*) &ZERO);
         //  определение сообщений программы
         if (lcsec =='m'){
            if (lcs== 1 ){ 
                lc=Load_CFG('m','i',(CVT*) &CALN );  if(CALN>=MALR){ CALN=MALR-1; }
            }
            if (lcs== 2 ){ lc=Load_CFG('m','i',(CVT*) &LALR[CALN].cs  ); }
            if (lcs== 3 ){ lc=Load_CFG('m','i',(CVT*) &LALR[CALN].cg  ); }
            if (lcs== 4 ){ lc=Load_CFG('m','c',(CVT*) &LALR[CALN].mes ); }
            if (lcs== 5 ){ lc=Load_CFG('m','c',(CVT*) &LALR[CALN].flg ); }
            if (lc ==10 ){ 
                if (CALN>=HALR){ HALR=CALN+1; }
                if (HALR>=MALR){ HALR=MALR;   }
            }
         }
         //  определение цвета сообщений оператора
         if (lcsec =='o'){
            if (lcs== 1 ){ lc=Load_CFG('o','i',(CVT*) &OpCS ); }
            if (lcs== 2 ){ lc=Load_CFG('o','i',(CVT*) &OpCG ); }
            if (lc ==10 ){ ;; }
         }
         // если закончен разбор очередной переменной (пробел)
         if (lc == 5 ){ 
             lcs++;  lc=1; 
         }
         // если был встречен конец строки
         if (lc ==10 ){ ClsFlg(); lc=1; }
      }
   }
   Init_CFG(0);
   //------------------------------------------------------------------------
   sprintf(SMSG[MMSG],SMSG[32],FNCfg);
   DPrn(1,1,"\n\n%s\n[ m ] HALR=%d\n",SMSG[MMSG],HALR);
   for(b=0;b<MALR;b++){
       DPrn(1,0,"%3d %2d %2d %s\n",b,LALR[b].cs,LALR[b].cg,LALR[b].mes);
   }
   //------------------------------------------------------------------------
   return(0);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
_i Read_FNMSG(vd){
   _i  lc=1, a=0;
   strcpy(FNCfg,FNMSG);
   ClsFlg();
   strcpy(lcsok,"m");
   //------------------------------------------------------------------------
   // разбор файла сообщений
   Init_CFG(1);
   for(a=0;a<MCYC;a++){
      if (Init_CFG(2)){ break; }
      for(lca=0;lca<FHCfg;lca++){
         lc=Load_CFG('0','i',(CVT*) &ZERO );
         //  определение сообщений программы
         if (lcsec =='m'){
            if (lcs== 1 ){ 
                lc=Load_CFG('m','i',(CVT*) &CALN );  if(CALN>=MMSG){ CALN=MMSG-1; }
            }
            if (lcs== 2 ){ lc=Load_CFG('m','c',(CVT*) &SMSG[CALN] ); }
            if (lc ==10 ){ 
                if (CALN>=HSMSG){ HSMSG=CALN+1; }
                if (HSMSG>=MMSG){ HSMSG=MMSG-1; }
            }
         }
         // если закончен разбор очередной переменной
         if (lc == 5 ){ lcs++;  lc=1;  }
         // если был встречен конец строки
         if (lc ==10 ){ ClsFlg(); lc=1; }
      }
   }
   Init_CFG(0);
   //------------------------------------------------------------------------
   return(0);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//  Выбор строк вывода из файла
vd  FChoice(int m){  
    _i  lc=1, a=0, h=0;
    //------------------------------------------------------------------------
    //  чтение данных из файла выбор строк при старте
    if (m==0&&FFST>0){
        HFST=0;
        if(access(LFST.fn,4)!=0){ 
           sprintf(SMSG[MMSG],SMSG[178],LFST.fn);
           DPrn(0,1,"%s\n",SMSG[MMSG]);
           return; 
        }
        strcpy(FNCfg,LFST.fn);
        ClsFlg();
        strcpy(lcsok,"s");
        //--------------------------------------------------------------------
        // разбор файла строк выбора
        Init_CFG(1);
        for(a=0;a<MCYC;a++){
            if (Init_CFG(2)){ break; }
            for(lca=0;lca<FHCfg;lca++){
                lc=Load_CFG('0','i',(CVT*) &ZERO );
                // определение строк вывода выбора
                if (lcsec =='s'){
                    //  запомнить значение
                    if (lcs== 1 ){ lc=Load_CFG('s','f',(CVT*) &VFST[HFST] ); }
                    //  запомнить строку, сответствующую значению
                    if (lcs== 2 ){ lc=Load_CFG('s','c',(CVT*) &SFST[HFST] ); }
                    if (lc ==10 ){ 
                        if (HFST<MCST-1){ HFST++;   }
                    }
                }
                // если закончен разбор очередной переменной
                if (lc == 5){ lcs++;    lc=1; }
                // если был встречен конец строки
                if (lc ==10){ ClsFlg(); lc=1; }
            }
        }
        Init_CFG(0);
//      for(a=0; a<HFST; a++){
//          DPrn(0,1,"VAL=%f STR=[%d]\n", VFST[HFST], SFST[HFST]);
//      }
        //--------------------------------------------------------------------
    }
    //------------------------------------------------------------------------

    //------------------------------------------------------------------------
    //  вывод строк из массива на экран
    if (m==1&&FFST>0&&HFST>0){
        // сравниваем текущее значение сравниваемой переменной со значениями
        // массива заданных строк
        for(a=0; a<HFST; a++){
            if(Arr_to_Val(LFST.na, LFST.nv)==VFST[a]){ break; }
        }
        // если было найдено совпадение значения строки с текущим значением
        // сравниваемой переменной- вывести строку на экран
        if(a<HFST){
           STR_cs=LFST.cs;
           STR_cg=LFST.cg;
           if(LFST.rf>0){
              h=text_height(dFnt[LFST.sn].dat)+LFST.rh;
              rectfill(scrsav, LFST.X,LFST.Y, LFST.X+LFST.W, LFST.Y+h, LFST.cg);
           }
           STR_Out(SFST[a], LFST.sn, LFST.X, LFST.Y, LFST.W, 1);
        }
    }
    //------------------------------------------------------------------------

    return;
}
//---------------------------------------------------------------------------
