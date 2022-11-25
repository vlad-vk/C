// coding: cp866
//-----------------------------------------------------------------------------
// В этом файле находятся функции для работы с записями:
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  возвращает количество записей в БД считая и удаленные
_l  dbRecCount(_i Sel){ return(dbHeaw[Sel].RecCount); }
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  установить указатель на первую запись
_l  dbGoFirst(_i Sel){ 
    if(dbHeaw[Sel].CRec<0){ return(-1); }
    dbHeaw[Sel].CRec=0; 
    dbHeaw[Sel].Bof=0; dbHeaw[Sel].Eof=0;
    return(dbHeaw[Sel].CRec);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  установить указатель на последнюю запись
_l  dbGoLast (_i Sel){ 
    if(dbHeaw[Sel].CRec<0){ return(-1); }
    dbHeaw[Sel].CRec=dbHeaw[Sel].RecCount-1;
    dbHeaw[Sel].Bof=0; dbHeaw[Sel].Eof=0;
    return(dbHeaw[Sel].CRec);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  установить указатель на указанную запись N
_l  dbGoRec  (_i Sel, _l NRec){ 
    if(NRec<0){ dbHeaw[Sel].Bof=1; return(-1); }
    if(NRec>=dbHeaw[Sel].RecCount){ dbHeaw[Sel].Eof=1; return(-2); }
    dbHeaw[Sel].CRec=NRec; dbHeaw[Sel].Bof=0; dbHeaw[Sel].Eof=0;
    return(NRec);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  установить указатель на смещение указанной записи
_l  dbGoSkip(_i Sel, _l SRec){ 
    if(dbHeaw[Sel].CRec+SRec<0){ dbHeaw[Sel].Bof=1; return(-1); }
    if(dbHeaw[Sel].CRec+SRec>=dbHeaw[Sel].RecCount){ dbHeaw[Sel].Eof=1; return(-2); }
    dbHeaw[Sel].CRec=dbHeaw[Sel].CRec+SRec; 
    dbHeaw[Sel].Bof=0; dbHeaw[Sel].Eof=0;
    return(SRec);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  проверка выхода за верхнюю границу БД после последнего перемещения
_i  dbBof(_i Sel){ return(dbHeaw[Sel].Bof); }
//----------------------------------------------------------------------------
//  проверка выхода за нижнюю границу БД после последнего перемещения
_i  dbEof(_i Sel){ return(dbHeaw[Sel].Eof); }
//----------------------------------------------------------------------------
//  возвращает номер текущей записи в БД
_l  dbRecCur(_i Sel){ return(dbHeaw[Sel].CRec); }
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  добавление пустой записи в БД
_i  dbRecAdd(_i Sel){
    _i  a=0;
    //  проверить длинну записи
    if(dbHeaw[Sel].RecLen<0||dbHeaw[Sel].RecLen>MRECLEN){ return(-1); }
    //  установить указатель на конец файла
     memset(  BRec[Sel],0,dbHeaw[Sel].RecLen);
      lseek(dbFile[Sel].Desk,0,SEEK_END);
    a=write(dbFile[Sel].Desk,BRec[Sel],dbHeaw[Sel].RecLen);
    // изменить поле общего количества записей в заголовке БД 
    if(dbModifyRecCount(Sel,1)>0){ dbHeaw[Sel].CRec=dbHeaw[Sel].RecCount-1; }
    return(a);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  записать буфер записи на диск
_i  dbRecWrite(_i Sel){
    _i  a=0;
    _l  s=0;
    //  проверить длинну записи
    if(dbHeaw[Sel].RecLen<0||dbHeaw[Sel].RecLen>MRECLEN){ return(-1); }
    s=dbHeaw[Sel].COfs+dbHeaw[Sel].RecLen*dbHeaw[Sel].CRec;
      lseek(dbFile[Sel].Desk,s,SEEK_SET);
    a=write(dbFile[Sel].Desk,BRec[Sel],dbHeaw[Sel].RecLen);
    dbFile[Sel].Mod=1;
    return(a);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  прочитать буфер записи с диска
_i  dbRecRead(_i Sel){
    _i  a=0;    
    _l  s=0;
    //  проверить длинну записи
    if(dbHeaw[Sel].RecLen<0||dbHeaw[Sel].RecLen>MRECLEN){ return(-1); }
     s=dbHeaw[Sel].COfs+dbHeaw[Sel].RecLen*dbHeaw[Sel].CRec;
        lseek(dbFile[Sel].Desk,s,SEEK_SET);
     a= read (dbFile[Sel].Desk,BRec[Sel],dbHeaw[Sel].RecLen);
     if(BRec[Sel][0]=='*'){ BCRD[Sel][0]='*'; }
     else                 { BCRD[Sel][0]=' '; }
    return(a);
}
//----------------------------------------------------------------------------
//  проверка признака удаления записи после операции чтения
_i  isRecDFL(_i Sel){
    _i  r=0;
    if(BCRD[Sel][0]=='*'){ r=1; }
    return(r);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  копировать буфер записи из одной области в другую
_i  dbRecCopy(_i Src, _i Des){
    memcpy(&BRec[Des], &BRec[Src], MRECLEN);
    return(0);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  проверить является ли запись удаленной или установить признак удаления
_i  dbRecDeleteFN(_i Sel, _l nRec, _i md, _i Flg){
    _i  r=0;
    _l  s=0;
    if(dbHeaw[Sel].RecLen<0||dbHeaw[Sel].RecLen>MRECLEN){ return(-1); }
    if(nRec<0){
       s=dbHeaw[Sel].COfs+dbHeaw[Sel].RecLen*dbHeaw[Sel].CRec;
    } else {
       if(nRec>dbHeaw[Sel].RecCount){ nRec=dbHeaw[Sel].RecCount; }
       s=dbHeaw[Sel].COfs+dbHeaw[Sel].RecLen*nRec;
    }
    lseek(dbFile[Sel].Desk,s,SEEK_SET);
    // если это режим проверки
    if(md==0){
       read (dbFile[Sel].Desk,BCRD[Sel],1); if(BCRD[Sel][0]=='*'){ r=1; }
    } else {
    // если это режим записи признака удаления
       if(Flg> 0){ BCRD[Sel][0]='*'; }
       else      { BCRD[Sel][0]=' '; }
       write(dbFile[Sel].Desk,&BCRD[Sel],1); r=1;
    }    
    return (r);
}
//----------------------------------------------------------------------------
//  проверить является ли запись удаленной
_i  isRecDelete(_i Sel, _l nRec){
    return(dbRecDeleteFN(Sel,nRec,0,0));
}
//----------------------------------------------------------------------------
//  установить признак удаления указанной записи
_i  dbRecDelete(_i Sel, _l nRec, _i Flg){
    return(dbRecDeleteFN(Sel,nRec,1,Flg));
}
//----------------------------------------------------------------------------
