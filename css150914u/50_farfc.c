// coding: cp866
//---------------------------------------------------------------------------
// Открытие файлов архива и изменений для просмотра
vd FARVW(vd){
   _i ta=TF[CTC].ta;
   if  (TF[CTC].ff[ta]==1){ 
        dbClose(TF[CTC].fh[ta]);
        TF[CTC].ff[ta]=0;
   }
   //------------------------------------------------------------------------
   // Если работаем с файлом архива
   if(LTRD[ta].th<10){
      // Если установленное имя БД (FNArc) != текущему (TF[CTC].fa[ta])
      if ((strcasecmp(TF[CTC].fa[ta],FNArc))!=0){
          // Определение свободной области для работы с БД архива
          if((TF[CTC].fh[ta]=dbSelectAvail())<0){           // Нет свободной     
              sprintf(SMSG[MMSG],SMSG[124]);                // области для работы
              DPrn(13,0,"%s\n",SMSG[MMSG]);             // с БД архива
              return;                                       //
          }
          // Инициализация и открытие БД архива
          dbInit(TF[CTC].fh[ta]);
          if(dbOpen(TF[CTC].fh[ta],TF[CTC].fa[ta])<0){      // Ошибка открытия 
             sprintf(STAT[1].str,SMSG[125],TF[CTC].fa[ta]); // файла  БД архива
             dbClear(TF[CTC].fh[ta]);                       // 
             TF[CTC].fh[ta]=ARCOut;                         //
             TF[CTC].ff[ta]=2;                              //
             return;
          }
          sprintf(STAT[1].str,SMSG[126],TF[CTC].fa[ta]);     // Файл БД архива
          TF[CTC].ff[ta]=1;                                  // открыт для работы
          // чтение файла архива
          RARead(ta,0);                                      
      //----------------------------------------------------------------------
      }  else  {
         // работа с текущим файлом БД архива (за текущую дату)
         sprintf(STAT[1].str,SMSG[127]);                    // Текущий файл
         TF[CTC].fh[ta]=ARCOut;                             // БД архива
         TF[CTC].ff[ta]=2;                                  //
         RARead(ta,0);
      } 
   } else {
   //------------------------------------------------------------------------
   // Если работаем с файлом изменений
      if ((strcasecmp(TF[CTC].fa[ta],FNUpd))!=0){
          // Определение свободной области для работы с БД изменений
          if((TF[CTC].fh[ta]=dbSelectAvail())<0){
              DPrn(13,0,"Нет свободной области для БД изменений [%s]\n",FNUpd);
              return;
          }
          // инициализация и открытие БД изменений
          dbInit(TF[CTC].fh[ta]);
          if(dbOpen(TF[CTC].fh[ta],TF[CTC].fa[ta])<0){
             sprintf(STAT[1].str,"Ошибка открытия БД изменений [%s]",FNUpd);
             dbClear(TF[CTC].fh[ta]);
             TF[CTC].fh[ta]=UPDOut;
             TF[CTC].ff[ta]=3;
             return;
          }
          sprintf(STAT[1].str,"Открыт файл БД изменений [%s]",FNUpd);
          TF[CTC].ff[ta]=1;
          // чтение файла БД изменений
          RARead(ta,0);                                      
       }  else  {
          // работа с текущим файлом БД изменений (за текущую дату)
          sprintf(STAT[1].str,"Файл изменений за текущую дату");
          TF[CTC].fh[ta]=UPDOut;
          TF[CTC].ff[ta]=3;
          RARead(ta,0);
       }
       DPrn(13,0,"UPD check test: [%s]\n",FNUpd);
   }
   //------------------------------------------------------------------------
   TLKey=4; STAT[1].cnt=5;
   return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// открытие файлов изменений
vd FUPOpen(vd){
   if(DBUFlg==0) return;
   if(UpdFL>0){ dbClose(UPDOut); }
   UpdFL=1;
   if((CreateUPD())==0){ 
       sprintf(STAT[1].str,"Error create(open) updates data file [%s]",FNUpd);
       DPrn(13,0,"%s\n",STAT[1].str);
       STAT[1].cnt=5;   fflush(DebOut);
       UpdFL=0; 
   }
   return;
}
//---------------------------------------------------------------------------
// открытие файлов архива и отчета тревог при переходе на новые(другие) сутки
vd FAROpen(vd){
   _i a=0;
   // закрыть текущие файлы
   if(ArcFL>0){ dbClose(ARCOut); }
   if(RepFL>0){ dbClose(REPOut); }
   if(UpdFL>0){ dbClose(UPDOut); UpdFL=0; }
   dbBFClearALL();
   ArcFL=1;
   RepFL=1;
   // открыть новые файлы
   if ((CreateARC())==0) {
      sprintf(SMSG[MMSG],SMSG[128],FNArc);
      DPrn(13,0,"%s\n",SMSG[MMSG]);
      sprintf(STAT[1].str,SMSG[129],FNArc);
      STAT[1].cnt=5; ArcFL=0; fflush(DebOut);
   }
   if ((CreateREP())==0) {
      sprintf(SMSG[MMSG],SMSG[130],FNRep);
      DPrn(13,0,"%s\n",SMSG[MMSG]);
      sprintf(STAT[1].str,SMSG[131],FNRep);
      STAT[1].cnt=5; RepFL=0; fflush(DebOut);
   }
   FUPOpen();
   gDiskFree(0);
   // Получить текущий день недели
   int yea=tmm->tm_year+1900, mon=tmm->tm_mon+1, day=tmm->tm_mday;
   int ai= (14 - mon) / 12;
   int yi= yea - ai;
   int mi= mon + 12 * ai - 2;
   WKDAY = ((7000 + (day + yi+ yi/ 4 - yi/ 100 + yi/ 400 + (31 * mi) / 12)) - 1 ) % 7;
   // printf(" WKDAY=%f ",WKDAY);
   return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
vd FARNRd(vd){
   _i a=0;
   for(a=0;a<MTRD;a++){
      // определение имени БД архива для тренда
      if(TF[CTC].ff[a]==3)
         sprintf(TF[CTC].fa[a],"%s/arc/%d%02d%02d.dbu",
                 FDir,tmm->tm_year+1900,tmm->tm_mon+1,tmm->tm_mday);
      else
         sprintf(TF[CTC].fa[a],"%s/arc/%d%02d%02d.dba",
                 FDir,tmm->tm_year+1900,tmm->tm_mon+1,tmm->tm_mday);
      // определение имени файла индекса БД архива
      sprintf(TF[CTC].fi[a],"%s/arc/%d%02d%02d.idx",
              FDir,tmm->tm_year+1900,tmm->tm_mon+1,tmm->tm_mday);
      // запомнить дату определения имени БД архива для тренда
      TF[CTC].yr[a]=tmm->tm_year+1900;
      TF[CTC].mn[a]=tmm->tm_mon +1;
      TF[CTC].dn[a]=tmm->tm_mday;
      sprintf(TF[CTC].dt[a],"%02d.%02d.%4d",
              tmm->tm_mday,tmm->tm_mon+1,tmm->tm_year+1900);
   }
   return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// определить имя текущего файла изменений
vd FUPName(int year, int mon, int day){
   sprintf(FNUpd,"%s/arc/%04d%02d%02d.dbu", FDir,year,mon,day);
   return;
}
//---------------------------------------------------------------------------
// определить имя текущего файла архива и отчета тревог
vd FARName(vd){
   sprintf(FNUpd,"%s/arc/%04d%02d%02d.dbu",
           FDir,tmm->tm_year+1900,tmm->tm_mon+1,tmm->tm_mday);
   sprintf(FNArc,"%s/arc/%04d%02d%02d.dba",
           FDir,tmm->tm_year+1900,tmm->tm_mon+1,tmm->tm_mday);
   sprintf(FNRep,"%s/arc/%04d%02d%02d.dbr",
           FDir,tmm->tm_year+1900,tmm->tm_mon+1,tmm->tm_mday);
   sprintf(FNIdx,"%s/arc/%04d%02d%02d.idx",
           FDir,tmm->tm_year+1900,tmm->tm_mon+1,tmm->tm_mday);
   return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// сброс буфера архива на диск
vd ARCFlush(vd){
   if(ARCBFR>0&&ArcFL>0&&dbOpenTest(ARCOut,0)>=0){
      dbBFWrite(ARCOut,ARCBuf,ARCBFR,DBBFADD); 
      dbBFGoFirst(ARCBuf); ReadIDX(1,0); ARCBFR=0; IDXCNT=0;
   }
   return;     
}
//---------------------------------------------------------------------------
// запись архивных значений в буфер и запись буфера в файл
vd ArcWrite(vd){
   _i a=0;
   // если новая секунда
   GetTime(0);
   if (tmm->tm_sec!=tmm_seco){
       tmm_seco=tmm->tm_sec; 
       // установить флаги смены секунд для трендов
       for(a=0;a<HTRD;a++){ TF[CTC].fl[a]=1; }; 
       TRFill(1);
       TIMF=tmm->tm_hour*10000+tmm->tm_min*100+tmm->tm_sec;
       // нулевые значения не обрабатываются (при старте)
       if(TIMF<1){ return; }
       // записать текущие значения архивных каналов
       a=0;
       dbFieldPutStr(ARCOut,a,0,"\xa",DBWBUFF); a++;
       dbFieldPutVal(ARCOut,a,0, TIMF,DBWBUFF); a++;
       dbFieldPutVal(ARCOut,a,0, TMST,DBWBUFF); a++;
       sprintf(AW_C,"%02d",tmm->tm_hour);
       dbFieldPutStr(ARCOut,a,0, AW_C,DBWBUFF); a++;
       sprintf(AW_C,"%02d",tmm->tm_min );               
       dbFieldPutStr(ARCOut,a,0, AW_C,DBWBUFF); a++;
       sprintf(AW_C,"%02d",tmm->tm_sec );
       dbFieldPutStr(ARCOut,a,0, AW_C,DBWBUFF); a++;
       // для каждого архивируемого канала
       for(;a<HARC+BREC;a++){
          // проверка диапазона
          if (arc_ch[a-BREC]<1||arc_ch[a-BREC]>=MCH){ continue;  }
          dbFieldPutVal(ARCOut,a,0, CB[arc_ch[a-BREC]].v, DBWBUFF);
       }
       dbBFRecCopyBuf(ARCOut,ARCBuf); dbBFGoSkip(ARCBuf,1); ARCBFR++;
       if(dbBFEof(ARCBuf)>0||ARCBFR>=ARSEC){ ARCFlush(); } 
       IDXTIM[IDXCNT].TIM =TIMF;
       IDXTIM[IDXCNT].nREC=ArcRecNum; ArcRecNum++; IDXCUR++; IDXCNT++;
       FARName();
       // при переходе на ноыве сутки
       if(/*((tmm->tm_hour==0)&&(tmm_houro==23))||*/(tmm->tm_mday!=tmm_mdayo)){ 
            IDXCNT=0; IDXCUR=0; ARCFlush(); FAROpen(); 
       }
       tmm_houro=tmm->tm_hour;
       tmm_mdayo=tmm->tm_mday;
   }
   return;
}
//---------------------------------------------------------------------------


//----------------------------------------------------------------------------
_i  yeaC=0,monC=0,dayC=0, houC=0,minC=0,secC=0, cro=0;
uc  WrStrUpd[SLEN];
//---------------------------------------------------------------------------
//  запись файла БД изменений каналов
vd  UpdWrite(vd){
    int   a=0, fh=-1, no=0, vv=0, crc=0, crt=0;
    if(DBUFlg==0) return;;
    // получить текущее время
    GetTime(0);
    TIMF=tmm->tm_hour*10000+tmm->tm_min*100+tmm->tm_sec;
    // нулевые значения не обрабатываются (при старте)
    if(TIMF<1){ return; }
    // если флаг передачи на контроллере = 1 и шаг передачи на контроллере = 3
    if(CB[DBUFlg+ 9].v==1&&CB[DBUFlg+11].v==3){
       yeaC=CB[DBUFlg+ 0].v;
       monC=CB[DBUFlg+ 1].v;
       dayC=CB[DBUFlg+ 2].v;
       houC=CB[DBUFlg+ 3].v;
       minC=CB[DBUFlg+ 4].v;
       secC=CB[DBUFlg+ 5].v;
       no  =CB[DBUFlg+ 6].v;
       vv  =CB[DBUFlg+ 7].v;
       crt =CB[DBUFlg+ 8].v;

       // посчитать CRC текущей записи
       crc =monC+dayC+houC+minC+secC+no+vv;
       TIMF=houC*10000+minC*100+secC;

       // DPrn(0,0,"y=%d m=%d d=%d h=%d m=%d s=%d no=%d vv=%d crt=%d crc=%d\n",
       //         yeaC,monC,dayC,houC,minC,secC,no,vv,crt,crc);

       // если CRC посчитанное и CRC принятое (crt=CB[38].v) равны и
       // CRC текущей записи и CRC предыдущей записи не равны
       if(crc==crt&&crc!=upd_ls.crc){
          // Если есть доступ к файлу последней записи- считать его
          if(access(FNDLS,6)==0){
             if((fh=open(FNDLS,O_RDWR|O_BINARY))>=0){
                read (fh,&upd_ls,sizeof(upd_ls));
                close(fh);
             }
          }
          // сформировать имя файла изменений
          FUPName(yeaC,monC,dayC);
          // открыть или создать данный файл на диске
          FUPOpen(); 
          // если файл базы изменений был открыт
          if(UpdFL>0){
             // если был открыт(создан) файл последней записи
             if((fh=open(FNDLS,O_CREAT|O_RDWR|O_BINARY,S_IREAD|S_IWRITE))>=0){
                // записать текущие значения архивных каналов в БД
                a=0;
                // добавить запись в БД каналов изменений
                dbRecAdd(UPDOut);
                // заполнить поля БД
                dbFieldPutStr(UPDOut,a,0,"\xa",0); a++;
                dbFieldPutVal(UPDOut,a,0, TIMF,0); a++;
                dbFieldPutVal(UPDOut,a,0, TMST,0); a++;
                sprintf(AW_C,"%02d",houC);
                dbFieldPutStr(UPDOut,a,0, AW_C,0); a++;
                sprintf(AW_C,"%02d",minC);               
                dbFieldPutStr(UPDOut,a,0, AW_C,0); a++;
                sprintf(AW_C,"%02d",secC);
                dbFieldPutStr(UPDOut,a,0, AW_C,0); a++;
                // для каждого архивируемого канала записать значения в буфер
                for(;a<HUPD+BREC;a++){
                   // если номер канала вне диапазона- перейти к следующ каналу
                   if (upd_ch[a-BREC]<1||upd_ch[a-BREC]>=MCH){ continue;  }
                   // если номер переданного канала совпал с номером в ячейке
                   // значений каналов- поменять значение на этом канале
                   if (no==upd_ch[a-BREC]){ upd_ls.vv[a-BREC]=vv; }
                   // записать значение текущего канала (upd_ls.vv[]) в текущее
                   // поле текущей записи БД изменений
                   dbFieldPutVal(UPDOut,a,0, upd_ls.vv[a-BREC],0);
                }
                // записать заполненную запись на диск
                dbRecWrite(UPDOut);
                // записать принятые данные в файл последней записи
                upd_ls.yea=yeaC;
                upd_ls.mon=monC;
                upd_ls.day=dayC;
                upd_ls.hou=houC;
                upd_ls.min=minC;
                upd_ls.sec=secC;
                upd_ls.no =no;
                upd_ls.crc=crc;
                write(fh,&upd_ls,sizeof(upd_ls));
                close(fh);
             }
          }
       }
       // DPrn(0,0,"uY=%d,Y=%d,uM=%d,M=%d,uD=%d,D=%d,uh=%d,h=%d,um=%d,m=%d,us=%d,s=%d,un=%d,n=%d,uc=%d,c=%d\n",
       //      upd_ls.yea,yeaC,upd_ls.mon,monC,upd_ls.day,dayC,
       //      upd_ls.hou,houC,upd_ls.min,minC,upd_ls.sec,secC,
       //      upd_ls.no, no,  upd_ls.crc,crc);
       // если полученные значения с контроллера равны записанным
       // послать комманду контроллеру о переходе к следующей записи:
       if(CB[DBUFlg+12].v==1||CB[DBUFlg+12].v==2){
       // DPrn(0,0,"Set answer flag.\n");
          // передать на канал DBUFlg+10 значение 2 CB[DBUFlg+10].v=2 и 
          // установить флаг передачи значения CBSF[DBUFlg+10]=2
          CBSN=DBUFlg+10; CB[CBSN].v=2; CBSF[CBSN]=2;
       }
    }
    return;
}
//---------------------------------------------------------------------------

