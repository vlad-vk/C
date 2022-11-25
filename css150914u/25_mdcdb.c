// coding: cp866
//---------------------------------------------------------------------------
//  проверка последних записей архива, если были нарушены данные
_i  CheckRecord(_i SEL){
    _i  a=0,  b=0;
    _l  c=0, hr=0;
    hr=dbRecCount(SEL);
      dbGoLast(SEL);
    c=dbRecCur(SEL);
    for(a=0;a<hr;a++){
        dbRecRead(SEL); 
        b=(dbFieldGetStr(SEL, 0,   0 ,DBWBUFF))[0];
//      b=(dbFieldGetStr(SEL,-1,"CRF",DBWBUFF))[0]; DPrn(0,0,"b=%d\n",b);
        if(b!=10){ dbRecDelete(SEL,-1,1); }
        else     { break; }
        dbGoSkip(SEL,-1);
        if(dbBof(SEL)>0||dbRecCur(SEL)<0){ break; }
    }
    sprintf(SMSG[MMSG],SMSG[73],dbFile[SEL].Name,c,a,hr);
    DPrn(0,0,"%s\n",SMSG[MMSG]);
    return(0);    
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//  создание/открытие базы сообщений
_i  CreateREP(vktvd){
    _i r=0;    
    REPOut=dbSelectAvail  ();   dbInit(REPOut);
    REPBuf=dbBFBufferAvail(); dbBFInit(REPBuf);
    dbCheck(REPOut, FNRep);     dbInit(REPOut);
    if(REPOut<0||REPBuf<0){ 
       sprintf(SMSG[MMSG],SMSG[74],REPOut,REPBuf);
       DPrn(13,0,"%s\n",SMSG[MMSG]);
       dbClear(REPOut); dbBFClear(REPBuf); return(0); 
    }
    if (access(FNRep,0)<0){
        sprintf(SMSG[MMSG],SMSG[75]);
        DPrn(13,0,"%s\n",SMSG[MMSG]);
        dbFieldFill (REPOut,0,"STRING",'C',100,0);
        dbFieldFill (REPOut,1,"FLG"   ,'C',3,0);
        dbFieldFill (REPOut,2,"COL"   ,'C',8,0);
        dbFieldClear(REPOut,3);
        if((r=dbCreate(REPOut,FNRep))<0){
            sprintf(SMSG[MMSG],SMSG[76],FNRep,r);
            DPrn(0,0,"%s\n",SMSG[MMSG]);
            dbClear(REPOut); dbBFClear(REPBuf); return(0); 
        } else {
            sprintf(SMSG[MMSG],SMSG[77]);
            DPrn(13,0,"%s\n",SMSG[MMSG]);
        }
    }
    sprintf(SMSG[MMSG],SMSG[78]);
    DPrn(13,0,"%s\n",SMSG[MMSG]);

    if((r=dbOpen(REPOut,FNRep))<0){
       sprintf(SMSG[MMSG],SMSG[79],FNRep,r);
       DPrn(0,0,"%s\n",SMSG[MMSG]);
       dbClear(REPOut); dbBFClear(REPBuf); return(0); 
    } else {
       sprintf(SMSG[MMSG],SMSG[80]);
       DPrn(13,0,"%s\n",SMSG[MMSG]);
    }; dbBFGoFirst(REPOut);
    return(1);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//  создание/открытие базы архива
int CreateARC(void){
    _i   a=0, cf=0;
    ARCOut=dbSelectAvail  ();   dbInit(ARCOut);
    ARCBuf=dbBFBufferAvail(); dbBFInit(ARCBuf);
    cf=dbCheck(ARCOut,FNArc);   dbInit(ARCOut);
    if(ARCOut<0){ 
       sprintf(SMSG[MMSG],SMSG[81]);
       DPrn(13,0,"%s\n",SMSG[MMSG]);
       dbClear(ARCOut); dbBFClear(ARCBuf); return(0); 
    };
    // создание полей БД архива
    dbFieldFill(ARCOut,a,"CRF",'C',1,0); a++;
    dbFieldFill(ARCOut,a,"TIM",'N',6,0); a++;
    dbFieldFill(ARCOut,a,"TST",'N',6,0); a++;
    dbFieldFill(ARCOut,a,"THR",'C',2,0); a++;
    dbFieldFill(ARCOut,a,"TMN",'C',2,0); a++;
    dbFieldFill(ARCOut,a,"TSC",'C',2,0); a++;
    for(;a<HARC+BREC;a++){
        dbFieldFill(ARCOut,a,CB[ arc_ch[a-BREC] ].nm,'B',8,4);
    }
    dbFieldClear(ARCOut,a);
    // создание или открытие БД архива
    if (access(FNArc,0)<0){
        sprintf(SMSG[MMSG],SMSG[82]);
        DPrn(13,0,"%s\n",SMSG[MMSG]);
        if(dbCreate(ARCOut,FNArc)<0){
           sprintf(SMSG[MMSG],SMSG[83],FNArc);
           DPrn(0,0,"%s\n",SMSG[MMSG]);
           dbClear(ARCOut); dbBFClear(ARCBuf); return(0); 
        } else {
           sprintf(SMSG[MMSG],SMSG[84]);
           DPrn(13,0,"%s\n",SMSG[MMSG]);
        } 
    }
    sprintf(SMSG[MMSG],SMSG[85]);
    DPrn(13,0,"%s\n",SMSG[MMSG]);
    if(dbOpen(ARCOut,FNArc)<0){
       sprintf(SMSG[MMSG],SMSG[86],FNArc);
       DPrn(0,0,"%s\n",SMSG[MMSG]);
       dbClear(ARCOut); dbBFClear(ARCBuf); return(0); 
    } else {
       sprintf(SMSG[MMSG],SMSG[87]);
       DPrn(13,0,"%s\n",SMSG[MMSG]);
    }
    if(cf>0){ CheckRecord(ARCOut); }
    dbBFGoFirst(ARCBuf);
    ReadIDX(0,0);
    TMST=tmm->tm_hour*10000+tmm->tm_min*100+tmm->tm_sec;
    TIMF=tmm->tm_hour*10000+tmm->tm_min*100+tmm->tm_sec;
    ArcRecNum=dbRecCount(ARCOut);
    return(1);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//  создание/открытие базы изменений
int CreateUPD(void){
    _i   a=0, b=0, f=0, cf=0, fh=0;
    UPDOut=dbSelectAvail  ();   dbInit(UPDOut);
    UPDBuf=dbBFBufferAvail(); dbBFInit(UPDBuf);
    cf=dbCheck(UPDOut,FNUpd);   dbInit(UPDOut);
    if(UPDOut<0){ 
       DPrn(13,0,"Error initialized base updated channels [%s]\n",FNUpd);
       dbClear(UPDOut); dbBFClear(UPDBuf); return(0); 
    };
    // создание полей БД изменений
    dbFieldFill(UPDOut,a,"CRF",'C',1,0); a++;
    dbFieldFill(UPDOut,a,"TIM",'N',6,0); a++;
    dbFieldFill(UPDOut,a,"TST",'N',6,0); a++;
    dbFieldFill(UPDOut,a,"THR",'C',2,0); a++;
    dbFieldFill(UPDOut,a,"TMN",'C',2,0); a++;
    dbFieldFill(UPDOut,a,"TSC",'C',2,0); a++;
    for(;a<HUPD+BREC;a++){
        dbFieldFill(UPDOut,a,CB[ upd_ch[a-BREC] ].nm,'B',8,4);
    }
    dbFieldClear(UPDOut,a);
    // создание или открытие БД изменений
    if (access(FNUpd,0)<0){
        if(dbCreate(UPDOut,FNUpd)<0){
           DPrn(0,0,"Error create updateds DataBase [%s]\n",FNUpd);
           dbClear(UPDOut); dbBFClear(UPDBuf); return(0); 
        } else {
           sprintf(SMSG[MMSG],SMSG[84]);
           DPrn(13,0,"Created updateds DataBase [%s]\n",FNUpd);
        } 
    }
    if(dbOpen(UPDOut,FNUpd)<0){
       DPrn(0,0,"Error open updateds DataBase [%s]\n",FNUpd);
       dbClear(UPDOut); dbBFClear(UPDBuf); return(0); 
    } else {
       DPrn(13,0,"Open updateds DataBase [%s]\n",FNUpd);
    }
    if(cf>0){ CheckRecord(UPDOut); }
    dbBFGoFirst(UPDBuf);
    UpdRecNum=dbRecCount(UPDOut); dbGoSkip(UPDOut,-1); b=dbBof(UPDOut);
    
    // если указатель за пределами файла (нет записей)
    if(b>0||UpdRecNum<0){ 
      memset(&upd_ls,0,sizeof(upd_ls));
      // Если есть доступ к файлу последней записи- считать его
      if(access(FNDLS,4)==0){
         if((fh=open(FNDLS,O_RDWR|O_BINARY))>=0){
            read (fh,&upd_ls,sizeof(upd_ls)); close(fh);
      }  }
      // добавить новую запись, получить кол-во полей, и перейти на первую зап
      dbRecAdd(UPDOut); f=dbFieldCount(UPDOut); dbGoFirst(UPDOut);
      dbFieldPutStr(UPDOut,0,0,"\xa",0);
      dbFieldPutStr(UPDOut,3,0, "00",0);
      dbFieldPutStr(UPDOut,4,0, "00",0);
      dbFieldPutStr(UPDOut,5,0, "00",0);
      // заполнить и записать все поля БД
      for(a=6;a<f;a++){ 
          if(dbFieldGetType(UPDOut,a)=='N') dbFieldPutVal(UPDOut,a,0,upd_ls.vv[a-6],0);
          if(dbFieldGetType(UPDOut,a)=='B') dbFieldPutVal(UPDOut,a,0,upd_ls.vv[a-6],0);
      };  dbRecWrite(UPDOut);
      DPrn(0,0,"f[%d] RC[%d] b[%d] \n",f,UpdRecNum,b);
    }
    return(1);
}
//---------------------------------------------------------------------------
