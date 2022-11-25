// coding: cp866
//---------------------------------------------------------------------------
// вывод курсора при вводе значений
_i vkGetCursor(_f X, _f Y, _i sn, _i cr, _i cg){
   _i a=0;
   _c cc[2]={0,0};
   _f hs=0, ws=0, cx=0;
   // установить флаг вызова ввода
   GetDATA=1;
   // 
   if (GKey==KEY_ESC || GKey==KEY_ENTER || GKey==KEY_ENTER_PAD){ LGET_rn=1; }
   // считаем сколько пикселов равна высота и ширина одного символа фонта
   hs=text_height(dFnt[sn].dat);
   ws=text_length(dFnt[sn].dat,"0");
   // считаем ширину символов и координату для курсора
   if (GS.fe>0){
       for(a=0;a<LGET_el;a++){
           cc[0]=GS.ES[a];
           cx=cx+text_length(dFnt[sn].dat,cc);
       }
   } else {
       cx=LGET_el*ws;
   }
   // нарисовать курсор
   if (LGET_rn==0){
       rectfill(scrsav, X+cx, Y+hs-0.5, X+cx+ws, Y+hs-1.5, cr);
       sprintf (STAT[1].str,SMSG[132]);
                STAT[1].cnt=2;
   }
   return(0);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Вввод строк и значений по шаблону
_i GetStr(_i md ){
   // текущий символ ввода
   _i GSRet=0, a=0, l=0, wc=0, ws=0;
   // высота символов
   _f hs=0;
   // сбросить флаг вызова ввода
   GetDATA=0;
   // функция вызвана в режиме ввода
   if (md==1 && GS.FL==1){
      CURGETDAT=1;
      LGET_en=1;
      if (GKey>0){
         LGET_eo=LGET_el;
         wc=text_length(dFnt[GS.sn].dat, "Ж" );
         ws=text_length(dFnt[GS.sn].dat,GS.ES); l=strlen(GS.ES);
         if(GKey>=KEY_0_PAD&&GKey<=KEY_9_PAD){
            GKey =GKey-10;   ACode=GKey+21;
         }
         if( GS.sf[LGET_el]=='0' &&
           ((GKey>=KEY_0&&GKey<=KEY_9)||GKey==KEY_MINUS||GKey==KEY_EQUALS)){
             GS.ES[LGET_el]=ACode; if(LGET_el<GS.LS-1){ LGET_el++; }
         }
         // записываем ASCII код клавиши
         if (GS.sf[LGET_el]=='#' && ACode>31 && ACode<250){
             // введенный символ в строку
             GS.ES[LGET_el]=ACode;
             // если включен русский язык
             if (RUS_ENG==0){
                 for(a=0;a<MRUC;a++){
                     if (ACode==RUN[a]){ break; }
                 }
                 // взять символ из таблицы русских символов
                 if (a<MRUC){ GS.ES[LGET_el]=RUC[a]; }
             }
             // проверить длинну строки по пикселам
             if(ws+2*wc<GS.W||LGET_el<l-1){
                if(LGET_el<GS.LS-1){ LGET_el++; }
             }
         }
         // если была нажата стрелка влево  (<--)
         if (GKey==KEY_LEFT){
             if (LGET_el>0){ LGET_el--; }
         }
         // если была нажата стрелка вправо (-->)
         if (GKey==KEY_RIGHT && GS.ES[LGET_el]>0){
             if((LGET_el<GS.LS-1)&&(LGET_el<l-1)){ LGET_el++; }
         }
         // если была нажата клавиша Home
         if (GKey==KEY_HOME){ LGET_el=0; }
         // если была нажата клавиша End
         if (GKey==KEY_END){ 
             if(l>0){ LGET_el=l-1; }
         }
         // если была нажата клавиша BackSpace
         if (GKey==KEY_BACKSPACE && GS.fe>=4){
             if (LGET_el>0){ LGET_el--; }
             for(a=LGET_el;a<strlen(GS.ES);a++){ GS.ES[a]=GS.ES[a+1]; }
             GS.ES[a]=0;
         }
         // если была нажата клавиша Del
         if (GKey==KEY_DEL && GS.fe>=4){
             for(a=LGET_el;a<strlen(GS.ES);a++){ GS.ES[a]=GS.ES[a+1]; }
             GS.ES[a]=0;
         }
      }
      // пропустить служебные символы
      if (GS.sf[LGET_el]=='.' || GS.sf[LGET_el]==',' ||
          GS.sf[LGET_el]==':' || GS.sf[LGET_el]==';' || GS.sf[LGET_el]=='*' ||
          GS.sf[LGET_el]=='-' || GS.sf[LGET_el]=='/' || GS.sf[LGET_el]=='+' )
      {
          GS.ES[LGET_el]=GS.sf[LGET_el];
          if(GKey==KEY_LEFT && LGET_el>0){ LGET_el--; }
          if(GKey!=KEY_LEFT && LGET_el<GS.LS){ LGET_el++; }
      }
      // считаем сколько пикселов равна высота фонта
      hs=text_height(dFnt[GS.sn].dat);
      // зарисовываем прямоугольник ввода
      rectfill(scrsav, GS.RX1, GS.RY1, GS.RX2, GS.RY2, GS.cg );
      // вывод строки ввода
      text_mode(GS.cg);
      textout(scrsav, dFnt[GS.sn].dat, GS.ES, GS.TX, GS.TY, GS.cs);
      // вывод курсора
      vkGetCursor(GS.TX, GS.TY, GS.sn, GS.cc, GS.cg);
      // закончить ввод по (ESC или Enter)
      if (GKey==KEY_ESC || GKey==KEY_ENTER || GKey==KEY_ENTER_PAD){
          GS.FL=0; LGET_en=0; CURGETDAT=0;
      }
      GSRet=GKey; GKey=0;
   }
   return(GSRet);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Ввод строк данных
_i GetVal(_i en, _i md){
   //
   _i  a=0, b=0, rc=0, sl=0, fc=0, l=0;
   _f hs=0;
   // если ввод запрещен - выйти
   if (DisG>0){ return(KEY_ESC); }
   // сбросить флаг вызова ввода
   GetDATA=0;

   // если текущий элемент меньше макс допустимого для данного экрана
   if (en>0 && en<HGET && LGET[en].nv>0){

      //---------------------------------------------------------------------
      // если флаг ввода не установлен - установить флаг ввода
      if((LGET_en==0)&&(md==0)){
          LGET_en=1; LGET[en].vw=1; LGET_st=en; LGET_el=0; fc=1;
          // Общая база каналов
          if (LGET[en].na==0){ sprintf(VGET[en].pr,LGET[en].sf,CB[LGET[en].nv].v); }
          // Локальная база каналов
          if (LGET[en].na==1){ sprintf(VGET[en].pr,LGET[en].sf,CBL[LGET[en].nv]);  }
          // База каналов драйвера COM портов
          if (LGET[en].na==2){ sprintf(VGET[en].pr,LGET[en].sf,CBS[LGET[en].nv]);  }
          // База каналов драйвера NetBios
          if (LGET[en].na==3){ sprintf(VGET[en].pr,LGET[en].sf,CBN[LGET[en].nv]);  }
          // База каналов драйвера TCP/IP
          if (LGET[en].na==4){ sprintf(VGET[en].pr,LGET[en].sf,CBT[LGET[en].nv]);  }
          // Массив переменных комманды 'y'
          if (LGET[en].na==6){ sprintf(VGET[en].pr,LGET[en].sf,YCM[LGET[en].nv]);  }
          // Массив временных переменных
          if (LGET[en].na==7){ sprintf(VGET[en].pr,LGET[en].sf,RTMP[LGET[en].nv]); }
      }
      //---------------------------------------------------------------------


      //---------------------------------------------------------------------
      // если установлен флаг ввода
      if (LGET[en].vw==1){

         CURGETVAL=en;
         //------------------------------------------
         // проверяем есть ли нажатая клавиша в таблице разрешенных символов
         for(a=0;a<MSYM;a++){
             if (GKey==LGC[a]){ break; }
         }
         // знак "минус" или "плюс" выводится только в позиции LGET_el=0
         // 13,14,25,26 - номер в массиве LGS[MSYM]
         if ((a==13 || a==14 || a==25 || a==26 ) && LGET_el>0){ a=MSYM; }
         // если нажатая клавиша в таблице присутствует
         if (a<MSYM){
            // записываем в редактируемую строку символ соответствия клавише
            VGET[en].pr[LGET_el]=LGS[a];
            // увеличиваем позицию текущего символа
            if (LGET_el<LGET[en].hc-1){ LGET_el++; }
         }
         // если была нажата стрелка назад  (<--)
         if (GKey==KEY_LEFT ){ if (LGET_el>0) LGET_el--;; }
         // если была нажата стрелка вперед (-->)
         if (GKey==KEY_RIGHT){ if (LGET_el<LGET[en].hc-1) LGET_el++;; }
         // если была нажата клавиша BackSpace
         if (GKey==KEY_BACKSPACE){
            if (LGET_el>0){ LGET_el--; }
            l=strlen(VGET[en].pr);
            for(a=LGET_el; a<l; a++){ VGET[en].pr[a]=VGET[en].pr[a+1]; }
            VGET[en].pr[a]=0;
         }
         // если была нажата клавиша Del
         if (GKey==KEY_DEL){
            l=strlen(VGET[en].pr);
            for(a=LGET_el; a<l; a++){ VGET[en].pr[a]=VGET[en].pr[a+1]; }
            for(b=a-1;  b<GLEN; b++){ VGET[en].pr[b]=0; }
         }
         //------------------------------------------

         //------------------------------------------
         // считаем сколько пикселов равна высота фонта
         hs=text_height(dFnt[LGET[en].sn].dat);
         //------------------------------------------

         //------------------------------------------
         // При любом нажатии клавиши изменяем вывод на экран
         if (fc>0 || GKey>0){
            // к концу строки добавляем по одному символу, пока не выйдем за
            // допустимое количество символов или размер строки в пикселах
            for(a=LGET_el;a<LGET[en].hc;a++){
               sl =strlen(VGET[en].pr);
               if (sl>=LGET[en].hc){ break; }
               VGET[en].pr[sl+1]= 0 ;
               if((text_length(dFnt[LGET[en].sn].dat, VGET[en].pr))>=(_i)(LGET[en].W)){
                  break;
               }
            }
         }
         // ограничить строку вывода
         sl=strlen(VGET[en].pr);
         for(a=LGET[en].hc;a>0;a--){
             if(text_length(dFnt[LGET[en].sn].dat, VGET[en].pr)>(_i)(LGET[en].W)){
                 VGET[en].pr[a]=0;
         }   }
        // зарисовываем прямоугольник ввода
        rectfill(scrsav, LGET[en].X,
                 LGET[en].Y,
                 LGET[en].X+ LGET[en].W,
                 LGET[en].Y+ hs,
                 LGET[en].cg );
         text_mode(LGET[en].cg);
         // вывод строки ввода
         textout(scrsav,dFnt[LGET[en].sn].dat,VGET[en].pr,
                 LGET[en].X,LGET[en].Y,LGET[en].cs);

         //------------------------------------------
         vkGetCursor(LGET[en].X,LGET[en].Y,LGET[en].sn,LGET[en].cr,LGET[en].cg);
      }
      //---------------------------------------------------------------------


      //---------------------------------------------------------------------
      if (GKey>0){
         SCode=0;
         //  отказ от ввода (ESC)
         if (GKey==KEY_ESC){
            LGET_en=0; LGET[en].vw=0; rc=KEY_ESC;
            // Если открыто окно - посылаем нажатие ESC, чтобы его закрыть
            SCode=(LockKBD!=100)?KEY_ESC:0;
            CURGETVAL=0;
         }

         // если был разрешен ввод строки и нажата клавиша окончания
         // ввода (ENTER)
         if (GKey==KEY_ENTER || GKey==KEY_ENTER_PAD){
            LGET_en=0; LGET[en].vw=0; rc=KEY_ESC;
            // Если открыто окно - посылаем нажатие ESC, чтобы его закрыть
            SCode=(LockKBD!=100)?KEY_ESC:0;
            // Общая база каналов
            if (LGET[en].na==0){
                CB[LGET[en].nv].v=atof(VGET[en].pr);
                if ( CB[LGET[en].nv].v > CB[LGET[en].nv].h  )
                   { CB[LGET[en].nv].v = CB[LGET[en].nv].h; }
                if ( CB[LGET[en].nv].v < CB[LGET[en].nv].l  )
                   { CB[LGET[en].nv].v = CB[LGET[en].nv].l; }
                FLNBW=0; FLSER=0;
                if ( (_i)fmod(CB[LGET[en].nv].dt, 10)>  0){ FLSER=1; }
                if ( (_i)fmod(CB[LGET[en].nv].dt,100)>=10){ FLNBW=1; }
                if ( FLNBW>0 ){
                     FNBS[ CBDN[LGET[en].nv] ] = 2;
                     CBN [ CBDN[LGET[en].nv] ] = CB[LGET[en].nv].v;
                }
                if ( FLSER>0 ){
                     CSSER=LGET[en].nv;
                     FSRS[ CBSR[CSSER] ] = 2;
                     CBS [ CBSR[CSSER] ] = CB[CSSER].v;
                }
            }
            // Локальная база каналов
            if (LGET[en].na==1){ 
                CBL[LGET[en].nv]=atof(VGET[en].pr);
            }
            // База каналов драйвера COM портов
            if (LGET[en].na==2){
                CBS [LGET[en].nv]=atof(VGET[en].pr);
                FSRS[LGET[en].nv]=2;
            }
            // База каналов драйвера NetBios
            if (LGET[en].na==3){
                CBN [LGET[en].nv]=atof(VGET[en].pr);
                FNBS[LGET[en].nv]=2;
            }
            // База каналов драйвера TCP/IP
            if (LGET[en].na==4){ 
                CBT [LGET[en].nv]=atof(VGET[en].pr);
            }
            // Массив переменных комманды 'y'
            if (LGET[en].na==6){ 
                YCM [LGET[en].nv]=atof(VGET[en].pr); 
                strcpy(LMVL[LGET[en].nv],VGET[en].pr); 
            }
            // Массив временных переменных
            if (LGET[en].na==7){ 
                RTMP[LGET[en].nv]=atof(VGET[en].pr);
            }
            CURGETVAL=0;
         }
         GKey=0;
      }
      //---------------------------------------------------------------------
   }
   return(rc);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Ввод рабочей строки
_i GetWStr(_i nStr, _i md){
   _i  a=0, hsn=0;
   if(nStr>=HSTT){ return(0); }
   //------------------------------------------------------------------------
   //  заполнение структуры для ввода строки
   if (md==0){
       CURGETSTR=nStr;
       if(LSTT[nStr].sn<0||LSTT[nStr].sn> MFNT){ LSTT[nStr].sn=SFNT; }
       if(LSTT[nStr].LS<1||LSTT[nStr].LS>=SLEN){ LSTT[nStr].LS=20;   }
       hsn=text_height(dFnt[LSTT[nStr].sn].dat);
       memset(GS.ES,0,SLEN);
       strcpy(GS.ES,LSTT[nStr].ES);              // строка для ввода
       GS.LS =LSTT[nStr].LS;                     // колич симв в строке
       GS.W  =LSTT[nStr].W;                      // длинна строки в пикселах

       GS.RX1=LSTT[nStr].X;                      // координаты для зарисовки
       GS.RY1=LSTT[nStr].Y;
       GS.RX2=LSTT[nStr].X+LSTT[nStr].W;
       GS.RY2=LSTT[nStr].Y+hsn;

       GS.TX =LSTT[nStr].X;                      // координаты ввода
       GS.TY =LSTT[nStr].Y;

       GS.sn =LSTT[nStr].sn;                     // параметры шрифта
       GS.cs =LSTT[nStr].cs;
       GS.cg =LSTT[nStr].cg;
       GS.cc =LSTT[nStr].cc;
                                                 // строка формата для ввода
       for(a=0;a<LSTT[nStr].LS;a++){ 
           GS.sf[a]='#'; 
       };  GS.sf[a]= 0;
       LGET_el=0;                                // нач символ ввода в строке
       GetStr(0);                                // вызов функции ввода строки
       GS.FL =1;                                 // флаг начала ввода
       GS.fe =7;                                 // флаг ввода рабочих строк
       CSTT=nStr;                                // текущий элемент структуры
   }
   //------------------------------------------------------------------------
   //  передача введенной строки в структуру рабочих строк
   if (md==1){
       // передать введенную строку в структуру 
       strcpy(LSTT[CSTT].ES,GS.ES); memset(GS.ES,0,SLEN);
       // генерируем код следующей клавиши
       GVCode=LSTT[CSTT].kb;
       // если это ввод строки поиска- обновить отображение файла
       for(a=0;a<HFVW;a++){
          if(LFVW[a].ws==CSTT){ FVCode=1; ViewFile(FVA,1); break; }
       }
       // сбросить флаг ввода строки
       GS.fe=0;
       // обнулить номер строки ввода
       CURGETSTR=0;
   }                                             
   return(0);
}
//---------------------------------------------------------------------------
