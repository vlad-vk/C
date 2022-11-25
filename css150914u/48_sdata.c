// coding: cp866
//---------------------------------------------------------------------------
// проверка правильности введенной даты
_i CheckDate(vd){
   _i a, ret=0;
   _c c[5];
   // получить год, месяц и число
   for(a=0;a<4;a++){ c[a]=GS.ES[a+6]; }; c[a]=0; CDY=atoi(c);   // год
   for(a=0;a<2;a++){ c[a]=GS.ES[a+3]; }; c[a]=0; CDM=atoi(c);   // месяц
   for(a=0;a<2;a++){ c[a]=GS.ES[a+0]; }; c[a]=0; CDD=atoi(c);   // день
   if (CDY<2002 || CDY>2100){ ret=1; }
   if (CDM<1    || CDM>12  ){ ret=2; }
   if (CDD<1    || CDD>31  ){ ret=3; }
   return(ret);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// проверка правильности введенного времени
_i CheckTime(vd){
   _i a, ret=0;
   _c c[5];
   // получить часы, минуты и секунды
   for(a=0;a<2;a++){ c[a]=GS.ES[a+0]; }; c[a]=0; CTH=atoi(c);   // часы
   for(a=0;a<2;a++){ c[a]=GS.ES[a+3]; }; c[a]=0; CTM=atoi(c);   // минуты
   for(a=0;a<2;a++){ c[a]=GS.ES[a+6]; }; c[a]=0; CT1=atoi(c);   // секунды
   if (CTH<0 || CTH>23){ ret=1; }
   if (CTM<0 || CTM>59){ ret=2; }
   if (CT1<0 || CT1>59){ ret=3; }
   RAStE=CTH*10000+CTM*100+CT1;
   return(ret);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// установить дату просмотра архива
_i SetDateArc(vd){
   _i ta=TF[CTC].ta;
   // если дата введена корректно
   if (CheckDate()==0){
      // передать введенное значение в переменную даты просмотра
      strcpy(TF[CTC].dl[ta],TF[CTC].dt[ta]);
      strcpy(TF[CTC].dt[ta],GS.ES);
      // сформировать имя файла архива
      if(TF[CTC].ff[ta]==3)
         sprintf(TF[CTC].fa[ta],"%s/arc/%04d%02d%02d.dbu",FDir,CDY,CDM,CDD);
      else
         sprintf(TF[CTC].fa[ta],"%s/arc/%04d%02d%02d.dba",FDir,CDY,CDM,CDD);
      // сформировать имя файла индекса
      sprintf(TF[CTC].fi[ta],"%s/arc/%04d%02d%02d.idx",FDir,CDY,CDM,CDD);
      // проверка доступа
      if((access(TF[CTC].fa[ta],4))<0){
          sprintf(TF[CTC].fa[ta],"%s/%04d%02d%02d.dba",ARCapath,CDY,CDM,CDD);
          sprintf(TF[CTC].fi[ta],"%s/%04d%02d%02d.idx",ARCapath,CDY,CDM,CDD);
      }
      // если нет доступа на чтение- оставить текущий файл
      if((access(TF[CTC].fa[ta],4))<0){
         sprintf(STAT[1].str,"Ошибка доступа к файлу архива [%s]",TF[CTC].fa[ta]);
         STAT[1].cnt=10;
         strcpy(TF[CTC].dt[ta],TF[CTC].dl[ta]);
         strcpy(GS.ES         ,TF[CTC].dl[ta]); CheckDate();
         if(TF[CTC].ff[ta]==3)
            sprintf(TF[CTC].fa[ta],"%s/arc/%04d%02d%02d.dbu",FDir,CDY,CDM,CDD);
         else
            sprintf(TF[CTC].fa[ta],"%s/arc/%04d%02d%02d.dba",FDir,CDY,CDM,CDD);
         //
         sprintf(TF[CTC].fi[ta],"%s/arc/%04d%02d%02d.idx",FDir,CDY,CDM,CDD);
      // если доступ есть...
      }  else  {
          FARVW();
      }
   }  else  {
      sprintf(STAT[1].str,SMSG[115]); STAT[1].cnt=5;
   }
   GS.fe=0;
   return(0);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// установить время просмотра архива
_i SetTimeArc(vd){
   _i a=0;
      GS.fe=0;
   if(FTC>0){
      sprintf(STAT[1].str,SMSG[116]); STAT[1].cnt=5;
      return(0);
   }
   if(CheckTime()==0){
      for(a=0;a<HTRD;a++){
          if (TF[CTC].an[a]==1){ RASAT=a; break; }
      }
      strcpy(TF[CTC].tm[RASAT],GS.ES); FTC=1;
   }  else  {
      sprintf(STAT[1].str,SMSG[117]); STAT[1].cnt=5;
   }
   return(0);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// установить дату просмотра отчета
_i SetDateRep(vd){
   _i f1=0;
   // если дата введена корректно
   if (CheckDate()==0){
      // передать введенное значение в переменную даты просмотра
      strcpy(LFVW[FVA].dl,LFVW[FVA].dt);
      strcpy(LFVW[FVA].dt,GS.ES);
      // сформировать имя файла отчета
      sprintf(FNFVW,"%s/arc/%04d%02d%02d.dbr",FDir,CDY,CDM,CDD);
      // если предыдущий файл не текущий файл регистрации сообщений
      if ((strcasecmp(FNRep,LFVW[FVA].fn))!=0){ f1=1; }
      // проверка доступа
      if((access(FNFVW,4))<0){
          sprintf(FNFVW,"%s/%04d%02d%02d.dbr",ARCapath,CDY,CDM,CDD);
      }  
      // если нет доступа на чтение
      if((access(FNFVW,4))<0){
         sprintf(STAT[1].str,SMSG[118],FNFVW);
         strcpy (LFVW[FVA].dt,LFVW[FVA].dl);
         strcpy (GS.ES       ,LFVW[FVA].dl); CheckDate();
         sprintf(FNFVW,"%s/arc/%04d%02d%02d.dbr",FDir,CDY,CDM,CDD);
         strcpy (LFVW[FVA].fn,FNFVW);
      // если доступ есть - закрыть предыдущий файл и открыть новый
      }  else  {
         if (f1==1){ dbClose(VW[FVA].fhd); }
         strcpy(LFVW[FVA].fn,FNFVW);
      }
      ViewFile(FVA,0);
   }  else  {
      sprintf(STAT[1].str,SMSG[119]); 
   }
   GS.fe=0; STAT[1].cnt=5;
   return(0);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// установить строку поиска в файле
_i SetFindStr(vd){
   strcpy(VW[FVA].fst,GS.ES); GS.fe=0; return(0);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// поиск в архиве по времени
_i RACTSeek(){
   _i  a=0, t=RASAT, cnt=0;
   if (FTC==0){ return(0); }
   // определить начальное смещение,флаг начала поиска,номер активного тренда
   if (FTC==1&&TF[CTC].ff[t]!=3){
       // чтение индексного файла для поиска 
       if((ReadIDX(2,t))>0){
           sprintf(STAT[1].str,SMSG[120],TF[CTC].fi[t]);
           STAT[1].cnt=5; FTC=0; return(0);
       }
       // количество записей в файле архива на текущий момент
       RASFS=dbRecCount(TF[CTC].fh[t]); FTC=2; RASSt=1;
       sprintf(STAT[1].str,SMSG[121]); STAT[1].cnt=5;  return(0);
   }
   // определение номера записи по индексному файлу
   if (FTC==2){
       for(a=RASSt,cnt=0;a<IDXCURS;a++){ 
           if(RAStE<=IDXSER[a].TIM&&IDXSER[a].nREC>0){ FTC=4; break; }
           if(cnt>500){ break; }     // счетчик записей для одного цикла
       }
       RASSt=a; if(a>=IDXCURS){ FTC=3; }
   }
   // если ничего похожего не нашли- установить смещение на конец файла
   if (FTC==3){
       sprintf(STAT[1].str,SMSG[122]);
       RABuf[RASAT].pr=RASFS-1;
       RARead(RASAT,1); TLKey=2; FTC=0;
   }
   // определение текущего смещения для файла архива
   if (FTC==4){
       sprintf(STAT[1].str,SMSG[123]); 
//     sprintf(STAT[1].str,"RqTm=%d TIM=%d REC=%d",RAStE,IDXSER[a].TIM,IDXSER[a].nREC);
       RABuf[RASAT].pr=IDXSER[a].nREC;
       RARead(RASAT,2); TLKey=3; FTC=0;
   };  STAT[1].cnt=10;
   return(0);
}
//---------------------------------------------------------------------------
