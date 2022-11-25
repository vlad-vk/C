// coding: cp866
//---------------------------------------------------------------------------
// управление просмотром
vd FV_Set(_i md){
   // если не определено ни одного файла для просмотра - выходим
   if (HFVW<1){ return; }
   // ALT+TAB, Alt+N - переключение между окнами активного экрана
   if (md==1){
       if (FVA+1<HFVW){ FVA++; }
       else           { FVA=0; }
       FVCode=1; ViewFile(FVA,1);
   }
   // ALT+D - ввод даты для просмотра файла отчета
   if (md==2 && LFVW[FVA].fd>0){
       strcpy(GS.ES,LFVW[FVA].dt);
       GS.LS =10;
       GS.RX1=GS.RDXV1;
       GS.RY1=GS.RDYV1;
       GS.RX2=GS.RDXV2;
       GS.RY2=GS.RDYV2;
       GS.TX =GS.TDXV;
       GS.TY =GS.TDYV;
       GS.W  =200;
       GS.sn =LFVW[FVA].sn;
       GS.cc =LFVW[FVA].cc;
       GS.cs =LFVW[FVA].cd;
       GS.cg =LFVW[FVA].cg;
       strcpy(GS.sf,"00.00.0000");
       LGET_el=0;
       GetStr(0);
       GS.FL =1;
       GS.fe =3;
   }
   // ALT+G - поиск введенной строки
   if (md==4 && LFVW[FVA].fd>0){
       if (VW[FVA].fpp==0){ VW[FVA].fpp=0; }
       else               { VW[FVA].fpp=VW[FVA].fps+1; }
       GS.fe=5;
   }
   // ALT+T - обратный поиск введенной строки
   if (md==5 && LFVW[FVA].fd>0){
       if (VW[FVA].fpp==0){ VW[FVA].fpp=VW[FVA].fsz; }
       else               { VW[FVA].fpp=VW[FVA].fps-1; }
       GS.fe=6;
   }
   return;
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// просмотр файлов
vd ViewFile(_i nm, _i md){
   _i  a=0, cl=0, fnd=0, fs=0, x=0, y=0, w[2], r=0, rr=0, rb=0, nBuf=0, hff=0;
   _f  cp[2];
   if(((HFVW<1)||(FVCode<1))&&md>0){ return; };
   RKDraw=1; FLSEL=1;
   //------------------------------------------------------------------------
   // открытие файла
   if (md==0){
      if ((strcasecmp(LFVW[nm].fn,FNRep))!=0){
         if(access(LFVW[nm].fn,0)<0){
            sprintf(ErrSTR,SMSG[142],LFVW[nm].fn);
            sprintf(SMSG[MMSG],SMSG[143],LFVW[nm].fn);
            DPrn(0,0,"%s\n",SMSG[MMSG]);
            ErrINI=1; STAT[1].cnt=10; VW[nm].ffl=0; return;
         }
         VW[nm].fhd=dbSelectAvail(); dbInit(VW[nm].fhd);
         if(dbOpen(VW[nm].fhd,LFVW[nm].fn)<0){
            sprintf(ErrSTR,SMSG[144],LFVW[nm].fn);
            sprintf(SMSG[MMSG],SMSG[145],LFVW[nm].fn);
            DPrn(0,0,"%s\n",SMSG[MMSG]);
            ErrINI=1; STAT[1].cnt=10; VW[nm].ffl=0; dbClear(VW[nm].fhd); 
            return;
         }
         VW[nm].ffl=1;
      }  else  {
         VW[nm].fhd=REPOut;
         VW[nm].ffl=2;
      }
      sprintf(STAT[1].str,SMSG[146],LFVW[nm].fn);
      // определить высоту шрифта для каждого окна отображения
      VW[nm].hhs=text_height(dFnt[LFVW[nm].sn].dat)+2;
      // определить количество строк в окне отображения
      VW[nm].hst=(_i)(divi(LFVW[nm].H,(VW[nm].hhs+1)));
      if(VW[nm].hst<1){ VW[nm].hst=1; }
      // определить начальные установки цвета
      STR_cs=LFVW[nm].cs;
      STR_cg=LFVW[nm].cg;
      // количество записей в файле просмотра
      VW[nm].fsz=dbRecCount(VW[nm].fhd);
      // перейти на первую запись и считать ее
      dbGoFirst(VW[nm].fhd); VW[nm].fps=0; dbRecRead(VW[nm].fhd);
      // начальная установка флагов просмотра
      md=1; fs=1; VW[nm].fcp=0; VW[nm].fpp=0;
   }
   //------------------------------------------------------------------------

   //------------------------------------------------------------------------
   // поиск строки в файле
   if((GS.fe==5 || GS.fe==6) && nm==FVA ){
      // проверка длинны строки для поиска
      if (strlen(LSTT[LFVW[nm].ws].ES)<1){
          sprintf(STAT[1].str,SMSG[147]);
          GS.fe=0; STAT[1].cnt=5; return;
      }
      // прямой поиск
      if (GS.fe==5){
          sprintf(STAT[1].str,SMSG[148]); STAT[1].cnt=5;
          for(a=VW[nm].fpp,r=0;a<VW[nm].fsz;a++,r++){
              dbGoRec(VW[nm].fhd,a); dbRecRead(VW[nm].fhd);
              strcpy(VW[nm].fbr, dbFieldGetStr(VW[nm].fhd,-1,"STRING",0));
              if(((strstr(VW[nm].fbr, LSTT[LFVW[nm].ws].ES))!=NULL) ||
                 ((strstr(dbFieldGetStr(VW[nm].fhd,-1,"FLG",0),LSTT[LFVW[nm].ws].ES))!=NULL) ){
                   fnd=5; break; 
              }
              if (r>MSTS){ VW[nm].fpp=a; return; }
      }; };
      // обратный поиск
      if (GS.fe==6){
          sprintf(STAT[1].str,SMSG[149]); STAT[1].cnt=5;
          for(a=VW[nm].fpp,r=0;a>0;a--,r++){
              dbGoRec(VW[nm].fhd,a); dbRecRead(VW[nm].fhd);
              strcpy(VW[nm].fbr, dbFieldGetStr(VW[nm].fhd,-1,"STRING",0));
              if(((strstr(VW[nm].fbr, LSTT[LFVW[nm].ws].ES ))!=NULL) || 
                 ((strstr(dbFieldGetStr(VW[nm].fhd,-1,"FLG",0),LSTT[LFVW[nm].ws].ES))!=NULL) ){
                   fnd=6; break; 
              }
              if (r>MSTS){ VW[nm].fpp=a; return; }
      }; };
      if (fnd>0){
          sprintf(STAT[1].str,SMSG[150]); 
          VW[nm].fps=a; VW[nm].fpp=a;
      } else {
          sprintf(STAT[1].str,SMSG[151]); 
      }
      STAT[1].cnt=5; GS.fe=0; md=1;
   }
   //------------------------------------------------------------------------

   //------------------------------------------------------------------------
   // если это файл сообщений
   if(LFVW[nm].fd>0 && FVA==nm){
      // если в файл сообщений добавлено новое сообщение:
      // последняя нажатая клавиша не End - перечитать файл с текущей позиции
      if(FVCodL!=KEY_END){ md=1; }
      else {
      // последняя нажатая клавиша End - установить код клавиши KEY_END
         if(FVCode==0||FVCode==1){ FVCode=KEY_END; }
         // если установлен флаг добавления записи - снять его
         if(FVW_Add>0){ FVW_Add=0; FVCode=KEY_END; }
      }
   }
   //------------------------------------------------------------------------

   //------------------------------------------------------------------------
   // PgDn (+)
   if (FVCode==KEY_PGDN && VW[nm].ffl>0 && FVA==nm){
       VW[nm].fsz=dbRecCount(VW[nm].fhd);
       if (VW[nm].fsz>VW[nm].fps+VW[nm].hst+1){
           VW[nm].fps=VW[nm].fps+VW[nm].hst+1;
       };  md=1;
   }
   //------------------------------------------------------------------------

   //------------------------------------------------------------------------
   // End (+)
   if (FVCode==KEY_END && VW[nm].ffl>0 && FVA==nm){
      VW[nm].fsz=dbRecCount(VW[nm].fhd);
      if (VW[nm].fsz>VW[nm].fps+VW[nm].hst+1){
          if (VW[nm].fsz-VW[nm].hst>0){ VW[nm].fps=VW[nm].fsz-VW[nm].hst; }
          else                        { VW[nm].fps=0; }
      };  md=1; 
   }
   //------------------------------------------------------------------------

   //------------------------------------------------------------------------
   // PgUp
   if (FVCode==KEY_PGUP && VW[nm].ffl>0 && FVA==nm){
       if (VW[nm].fps-VW[nm].hst-1>0){ VW[nm].fps=VW[nm].fps-VW[nm].hst-1; }
       else                          { VW[nm].fps=0; }
       md=1;
   }
   //------------------------------------------------------------------------

   //------------------------------------------------------------------------
   // Home (+)
   if (FVCode==KEY_HOME && VW[nm].ffl>0 && FVA==nm){
       VW[nm].fps=0; md=1;
   }
   //------------------------------------------------------------------------


   // если нажата одна из клавиши управления просмотром
   // файл открыт для просмотра, является активным и файлом архива
   if(((FVCode==KEY_HOME)||(FVCode==KEY_PGUP)||
       (FVCode==KEY_END )||(FVCode==KEY_PGDN))&&
       (FVA==nm)&&(VW[FVA].ffl>0)){ md=1; FVCodL=FVCode; }


   //------------------------------------------------------------------------
   // вывод файла на экран
   if (md==1 && VW[nm].ffl>0 && FVA==nm ){
      // количество записей в БД
      VW[nm].fsz=dbRecCount(VW[nm].fhd);
      // номер текущей страницы
      VW[nm].fcp=divi(VW[nm].fps,VW[nm].hst);
      // количество страниц в файле
      VW[nm].fhp=(_i)ceil(divf(VW[nm].fsz,VW[nm].hst));
      // размер ползунка
      VW[nm].fcz=divf(LFVW[nm].H,VW[nm].fhp);
      //---------------------------------------------------------------------
      // зарисовать прямоугольник вывода файла
      rectfill(scrsav, LFVW[nm].X,
                       LFVW[nm].Y,
                       LFVW[nm].X+ LFVW[nm].W,
                       LFVW[nm].Y+ LFVW[nm].H,
                       LFVW[nm].cg );
      //---------------------------------------------------------------------
      // вывод строк на экран
      x=(LFVW[nm].X+LFVW[nm].L);
      y=(LFVW[nm].Y+5);
      // определить длинну строки даты
      w[0]=text_length(dFnt[LFVW[nm].sn].dat,"77.77.7777");
      // установить указатель на запись в файле БД
      dbGoRec(VW[nm].fhd,VW[nm].fps);
      FVCURREC =VW[nm].fps;                 // номер текущей первой записи
      FVCURFNH =VW[nm].hhs;                 // высота шрифта отображ файла
      FVCURHST =VW[nm].hst;                 // количество строк в окне файла
      FVSELDBA =VW[nm].fhd;                 // номер области БД актив файла
      FVCURPAGE=VW[nm].fcp;                 // номер текущей страницы
      FVLSTPAGE=VW[nm].fhp-1;               // общее количество страниц
      //---------------------------------------------------------------------
      // выбрать свободный буфер БД, очистить его и инициализировать
      nBuf=dbBFBufferAvail(); dbBFClear(nBuf); dbBFInit(nBuf);
      // получить кол-во записей в буфере и установить указатель на запись N0
      dbBFRecInBuf(VW[nm].fhd,nBuf); dbBFGoFirst(nBuf);
      // прочитать записи из файла БД в буфер
      rr=dbBFRead(VW[nm].fhd,nBuf,VW[nm].hst+1);
      // установить цвет и шрифт вывода строк
      STR_cs=LFVW[nm].cs;
      STR_cg=LFVW[nm].cg;
      STR_sn=LFVW[nm].sn;
      // проверить правильность заданных полей
      if(LFVW[nm].f1>1){ LFVW[nm].f1=2; }
      if(LFVW[nm].f1<1){ LFVW[nm].f1=0; }
      if(LFVW[nm].f2>1){ LFVW[nm].f2=2; }
      if(LFVW[nm].f2<1){ LFVW[nm].f2=0; }
      cl=dbFieldGetNum(VW[nm].fhd,0,"COL");
      // вывести строки на экран
      for(a=VW[nm].fps,rb=0;; a++,rb++){
          if(rb>rr){ md=20; break; }
          // передать данные из текущей записи буфера в буфер записи БД
          dbBFBufCopyRec(VW[nm].fhd,nBuf);
          // сформировать строку из заданных полей для вывода на экран
          if(LFVW[nm].f1>0){
             strcpy (FVFLD1,dbFieldGetStr(VW[nm].fhd,LFVW[nm].f1-1,0,0));
          } else {
             strcpy (FVFLD1,"");
          }
          if(LFVW[nm].f2>0){
             strcpy (FVFLD2,dbFieldGetStr(VW[nm].fhd,LFVW[nm].f2-1,0,0));
          } else {
             strcpy (FVFLD2,"");
          }
          // если задан фильтр и условие фильтрции не выполнилось -
          // перейти к следующей записи
          if((FSON==0 && FEON==0) ||
             ((strstr(FVFLD1, LSTT[LFVW[nm].ws].ES ))!=NULL && FSON>0) ||
             ((strstr(FVFLD2, LSTT[LFVW[nm].ws].ES ))!=NULL && FEON>0) ){
             //
             if(LFVW[nm].f1<1&&LFVW[nm].f2<1){
                strcpy (FVFLD1,dbFieldGetStr(VW[nm].fhd,-1,"STRING",0));
                strcpy (FVFLD2,"");
             }
             if(cl>=0){ 
                strcpy (FVCOLR,dbFieldGetStr(VW[nm].fhd,-1,"COL",0));
             } else {
                strcpy (FVCOLR,"");
             }
             // вывести строку из текущей записи на экран
             sprintf(FVSSTR,"%s%s%s",FVCOLR,FVFLD1,FVFLD2);
             STR_Out(FVSSTR,STR_sn,x,y, LFVW[nm].W-LFVW[nm].R,LFVW[nm].vw);
          }
          // положение для следующей строки
          y=y+VW[nm].hhs;
          // перейти к следующей записи в буфере
          dbBFGoSkip(nBuf,1); 
          // если это последняя запись в буфере
          if(dbBFEof(nBuf)>0){ md=21; break; }

          // проверки выхода за границу записей
          if(a>=VW[nm].fsz){ 
             VW[nm].fcp=divf(VW[nm].fsz,VW[nm].hst); md=22; break; }
          if(a>=VW[nm].fps+VW[nm].hst){ md=23; break; }
      }
//    sprintf(STAT[1].str,"rr=%d   rb=%d  md=%d",rr,rb,md); STAT[1].cnt=10;
//    sprintf(ErrSTR,"FVCode=%d FVCodL=%d",FVCode,FVCodL); ErrINI=1;

      // очистить буфер
      dbBFClear(nBuf);
      //---------------------------------------------------------------------
      // указатель текущего положения в файле (номер начальной записи)
      if (LFVW[nm].ps>0){
          if (VW[nm].fsz>1){
              // начальная координата ползунка положения
              cp[0]=VW[nm].fcp*VW[nm].fcz;
              // конечная  координата ползунка положения
              cp[1]=cp[0]+VW[nm].fcz;
              // проверка выхода за границу отображения файла
              if(cp[1]>LFVW[nm].H){ cp[1]=LFVW[nm].H; }
              // вывод ползунка положения на экран
              rectfill(scrsav, LFVW[nm].X+ LFVW[nm].W-5,
                               LFVW[nm].Y+ cp[0],
                               LFVW[nm].X+ LFVW[nm].W,
                               LFVW[nm].Y+ cp[1],
                               LFVW[nm].cp );
          }
      }
      //---------------------------------------------------------------------
      // вывод даты и строки поиска
      if (LFVW[nm].fd>0){
          // закрасить вывод строки поиска
          hff=text_height(dFnt[ LSTT[LFVW[nm].ws].sn ].dat)+1;
          rectfill(scrsav, LSTT[LFVW[nm].ws].X,
                           LSTT[LFVW[nm].ws].Y,
                           LSTT[LFVW[nm].ws].X+LSTT[LFVW[nm].ws].W,
                           LSTT[LFVW[nm].ws].Y+hff,
                           LSTT[LFVW[nm].ws].cg );
          text_mode(LFVW[nm].cg);
          // вывод даты
          textout(scrsav,dFnt[LFVW[nm].sn].dat,LFVW[nm].dt,
                           LFVW[nm].X+LFVW[nm].L,
                           LFVW[nm].Y+LFVW[nm].H+VW[nm].hhs*0.4+LFVW[nm].yt,
                           LFVW[nm].cd);
          // вывод строки поиска
          textout(scrsav,dFnt[LFVW[nm].sn].dat,LSTT[LFVW[nm].ws].ES,
                           LSTT[LFVW[nm].ws].X,
                           LSTT[LFVW[nm].ws].Y,
                           LFVW[nm].cd);
          if (FVA==nm){
              // запомнить координаты вывода даты
              GS.RDXV1=LFVW[nm].X+LFVW[nm].L;
              GS.RDYV1=LFVW[nm].Y+LFVW[nm].H+VW[nm].hhs*0.3+1;
              GS.RDXV2=LFVW[nm].X+LFVW[nm].L+w[0]+5;
              GS.RDYV2=LFVW[nm].Y+LFVW[nm].H+VW[nm].hhs*1.3-1;
              GS.TDXV =LFVW[nm].X+LFVW[nm].L;
              GS.TDYV =LFVW[nm].Y+LFVW[nm].H+VW[nm].hhs*0.4+LFVW[nm].yt;
              // запомнить координаты вывода поиска
              GS.RFXV1=LFVW[nm].X+LSTT[LFVW[nm].ws].X;
              GS.RFYV1=LFVW[nm].Y+LFVW[nm].H+VW[nm].hhs*0.3+1;
              GS.RFXV2=LFVW[nm].X+LSTT[LFVW[nm].ws].W;
              GS.RFYV2=LFVW[nm].Y+LFVW[nm].H+VW[nm].hhs*1.3-1;
              GS.TFXV =LFVW[nm].X+LSTT[LFVW[nm].ws].X;
              GS.TFYV =LFVW[nm].Y+LFVW[nm].H+VW[nm].hhs*0.4;
          }
      }
      //---------------------------------------------------------------------
   }
   //------------------------------------------------------------------------
   if(nm==FVA&&FVCode>0){ FVCode=0; }
   if(FVW_Add==1){ FVW_Add=0; }
   return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//  Запись значения в указанное поле текущей записи БД актив файла просмотра 
vd  FVDBFLDWR(int ndef){
    _l nreco=0;
    // если не было определено действий для работы с БД
    if(HFVB<=1){ return; }
    // если номер определения больше макс определенного
    if(ndef>=HFVB){ return; }
    // если указанного поля в БД нет- выйти
    if(dbFieldGetNum(FVSELDBA, 0,LFVB[ndef].fld)<0){ return; }
    // запомнить номер текущей записи
    nreco= dbRecCur (FVSELDBA);
    // перейти на нужную запись
    if(dbGoRec(FVSELDBA,FVDBCREC)>=0){
       // поменять значение в указанном поле на заданное
       dbFieldPutStr(FVSELDBA,-1,LFVB[ndef].fld,LFVB[ndef].val,DBWDISK);
    }
    // перейти на запись с запомненный номером
    dbGoRec(FVSELDBA,nreco);
    return;
}
//---------------------------------------------------------------------------
