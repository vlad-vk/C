// coding: cp866
//---------------------------------------------------------------------------
// счетчики таймера тиков
vd tt_tick(vd){
    int a=0;
    for(a=0;a<10;a++) if(HTimer[a]<0xffffff) HTimer[a]++; else HTimer[a]=0;
}
#ifndef WINDOW
END_OF_FUNCTION(tt_tick);
#endif
//---------------------------------------------------------------------------
// счетчики таймера миллисекунд
vd tt_msec(vd){
    int a=0;
    for(a=0;a<10;a++) if(mTimer[a]<0xffffff) mTimer[a]++; else mTimer[a]=0;
}
#ifndef WINDOW
END_OF_FUNCTION(tt_msec);
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Проверка таймеров
_i MTimer(ui n, uc mh, ui c){
   // проверка правильности номера таймера
   if(n>=HTIMERS){ return(0); }
   // обнуление счетчика указанного таймера
   if(c==0){ 
      if(mh=='m'||mh=='M'){ mTimer[n]=0; }
      else                { HTimer[n]=0; }
      return(0); 
   }
   // таймер сработал
   if(mh=='m'||mh=='M'){
      if(mTimer[n]>=c){ return(2); }
   } else {
      if(HTimer[n]>=c){ return(2); }
   }
   // таймер еще выполняется
   return(1);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// получить и вывести текущую дату и время
_i GetTime(_i md){
   _i a=0, b=0;
   // получить текущее время
   TMV=time(NULL);
   tmm=localtime(&TMV);
   sprintf(MDate,"%02d.%02d.%04d ",tmm->tm_mday,tmm->tm_mon+1,tmm->tm_year+1900);
   sprintf(MTime,"%02d:%02d:%02d ",tmm->tm_hour,tmm->tm_min,tmm->tm_sec);
   YEA_CMD=tmm->tm_year+1900;
   MON_CMD=tmm->tm_mon +1;
   DAY_CMD=tmm->tm_mday;
   HOU_CMD=tmm->tm_hour;
   MIN_CMD=tmm->tm_min;
   SEC_CMD=tmm->tm_sec;
   if (md==0){ return(0); }
   // дата
   if (VKDATE[CFirst].sn>0){
       for(a=0;a<VKDATE[CFirst].lcs;a++){
           // определить длинну строки
           b=text_length(dFnt[VKDATE[CFirst].sn].dat, MDate);
           // если строка большая - обнуляем последний символ
           if (b>VKDATE[CFirst].lcs){ MDate[strlen(MDate)-1]=0; }
           else                   { break; }
       }
       // вывести на экран
       text_mode(VKDATE[CFirst].cg);
       textout(scrsav,dFnt[VKDATE[CFirst].sn].dat,MDate,VKDATE[CFirst].X,VKDATE[CFirst].Y,VKDATE[CFirst].cs);
   }
   // время
   if (VKTIME[CFirst].sn>0){
       for(a=0;a<VKTIME[CFirst].lcs;a++){
           // определить длинну строки
           b=text_length(dFnt[VKTIME[CFirst].sn].dat, MTime);
           // если строка большая - обнуляем последний символ
           if (b>VKTIME[CFirst].lcs){ MTime[strlen(MTime)-1]=0; }
           else                   { break; }
       }
       // вывести на экран
       text_mode(VKTIME[CFirst].cg);
       textout(scrsav,dFnt[VKTIME[CFirst].sn].dat,MTime,
               VKTIME[CFirst].X,VKTIME[CFirst].Y,VKTIME[CFirst].cs);
   }
   return(1);
}
//---------------------------------------------------------------------------



//****************************************************************************
// QueryPerformanceCounter() количество тактов берет с опорного генератора
// частотой 3.57 MHz  (3579545 тиков за одну секунду).
// Этот счетчик более точный,  чем RDTSC, но работает только под Windows.

// Сравнение двух счетчиков и запись результата в строковую переменную
LARGE_INTEGER  vvkpf,  vvkpc0,  vvkpc1;
char           vvkpfstr[2048];
char * Profile_us(int mode){
  float TE=0;
  if(mode==0){
     // получить значение частоты внутреннего таймера
     QueryPerformanceFrequency(&vvkpf);
     // получить значение счетчика 0
     QueryPerformanceCounter  (&vvkpc0);
     return(" ");
  }
  // получить значение счетчика 1
  QueryPerformanceCounter(&vvkpc1);
  // получить разницу счетчиков в секунду
  TE=((float)(vvkpc1.QuadPart-vvkpc0.QuadPart)/(float)(vvkpf.QuadPart));
  // записать в строковую переменную сетчики и разницу между ними в микросекундах
  sprintf(vvkpfstr,"PC0[%.0f] PC1[%.0f] TE[%.0f us]",
         (float)vvkpc0.QuadPart,(float)vvkpc1.QuadPart,TE*1000000);
  return (vvkpfstr);
}
//----------------------------------------------------------------------------
// Задержка на указанное количество микросекунд
void win_delay(char what, float how){
     LARGE_INTEGER pc1, pc0, pf;
     float TE;
     // получить значение частоты таймера
     if (QueryPerformanceFrequency(&pf )){
         // получить значение счетчика0
         QueryPerformanceCounter  (&pc0);
         for(;;){
           // получить значение счетчика1
           QueryPerformanceCounter(&pc1);
           // вычислить  текущее количество тиков таймера между счетчиком0 и 
           // счетчиком1 за секунду
           TE=((float)pc1.QuadPart-(float)pc0.QuadPart)/(float)pf.QuadPart;
           // перевести это значение в микросекунды и сравнить с заданным:
           // если  разница между текущим значением счетчика и запомненным
           // больше задонного значения микросекунд - выйти из цикла.
           if(what=='s'||what=='S') TE=TE*1;
           if(what=='m'||what=='M') TE=TE*1000;
           if(what=='u'||what=='U') TE=TE*1000000;
           if (TE >= how) break;
         }
     }
     return;
}
//----------------------------------------------------------------------------
