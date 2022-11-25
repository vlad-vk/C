// coding: cp866
//-----------------------------------------------------------------------------
// В этом файле находятся функции работы с базой данных через буфер:
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  поиск свободного буфера
_i  dbBFBufferAvail(){
    _i  a=0;
    for(a=0;a<DBMAXBUFF;a++){
        if(dbBUFF[a].Init<0){ return(a); }
    };  return(-1);
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  максимальное количество записей в буфере
_i  dbBFRecInBuf(_i Sel, _i nBuf){
    if( Sel<0|| Sel>=DBMAXDBF ){ return(-1); }
    if(dbHeaw[Sel].RecLen<1){ return(-2); }
    dbBUFF[nBuf].RecCount=divi(DBBSIZ, dbHeaw[Sel].RecLen);
    return(dbBUFF[nBuf].RecCount);
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  очистка буфера с указанным номером
_i  dbBFClear(_i nBuf){
    _i  a=0;    
    if(nBuf<0||nBuf>=DBMAXBUFF){ return(-1); }
    dbBUFF[nBuf].RecCur=-1; 
    dbBUFF[nBuf].RecCount=0;
    dbBUFF[nBuf].Init=-1;
    for(a=0;a<DBBSIZ;a++){ dbBUFF[nBuf].Arr[a]=0; }
    return(0);
}
//-----------------------------------------------------------------------------
//  очистка всех буферов
vd  dbBFClearALL(vd){
    _i  a=0;
    for(a=0;a<DBMAXBUFF;a++){ dbBFClear(a); }    
    return;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  зарезервировать буфер для дальнейшего использования
_i  dbBFInit(_i nBuf){
    if(nBuf<0||nBuf>=DBMAXBUFF){ return(-1); }
    if( dbBUFF[nBuf].Init >= 0){ return(-2); }
    dbBUFF[nBuf].Init=nBuf;
    return(nBuf);
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  чтение файла БД в буфер от текущей записи
_i  dbBFRead(_i Sel, _i nBuf, _i hRec){
    _i  a=0, x=0;
    _l  s=0;
    //  проверить правильность задания областей
    if(nBuf<0||nBuf>=DBMAXBUFF){ return(-1); }
    if( Sel<0|| Sel>=DBMAXDBF ){ return(-1); }
    //  проверить номер текущей записи и длинну записи
    if(dbHeaw[Sel].CRec<0||dbHeaw[Sel].CRec>DBMAXREC){ return(-2); }
    //  проверить длинну записи
    if(dbHeaw[Sel].RecLen<=0||dbHeaw[Sel].RecLen>MRECLEN){ return(-3); }
    //  допустимое количество записей в буфере
    dbBFRecInBuf(Sel,nBuf);
    //  вычислить смещение в файле БД для текущей записи
    s=dbHeaw[Sel].COfs+dbHeaw[Sel].RecLen*dbHeaw[Sel].CRec;
    //  размер одной записи передать в массив буфера
    dbBUFF[nBuf].RecLen=dbHeaw[Sel].RecLen;
    //  вычислить количество байт, соответствующих количеству записей hRec
    x=hRec*dbHeaw[Sel].RecLen; if(x>DBBSIZ){ x=DBBSIZ; }
    //  установить указатель на начало текущей записи в файле БД
    lseek(dbFile[Sel].Desk,s,SEEK_SET);
    //  прочитать файл в буфер
    a=read(dbFile[Sel].Desk, dbBUFF[nBuf].Arr,x);
    //  вернуть количество записей в считанном буфере
    if(a<dbHeaw[Sel].RecLen){ return(0); }    
    return(divi(a, dbHeaw[Sel].RecLen));
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  запись буфера в файл БД (буфер записывается с записи N0 в буфере,
//  в текущую запись БД, кроме режима добавления)
_i  dbBFWrite(_i Sel, _i nBuf, _i hRec, _i mdWR){
    _i  l=0, r=0, w=0, hrc=0;
    _l  s=0, z=0;
    //  проверить правильность задания областей
    if(nBuf<0||nBuf>=DBMAXBUFF){ return(-1); }
    if( Sel<0|| Sel>=DBMAXDBF ){ return(-2); }
    if(mdWR!=DBBFADD&&mdWR!=DBBFWRITE&&mdWR!=DBBFWRADD){ mdWR=DBBFADD; }
    //  проверить длинну записи
    if(dbHeaw[Sel].RecLen<=0||dbHeaw[Sel].RecLen>MRECLEN){ return(-3); }
    //  проверить количество записываемых записей
    if(hRec<0||hRec>(hrc=divi(DBBSIZ,dbHeaw[Sel].RecLen))){ return(-4); }
    //  допустимое количество записей в буфере
    dbBFRecInBuf(Sel,nBuf);
    //  размер одной записи передать в массив буфера
    dbBUFF[nBuf].RecLen=dbHeaw[Sel].RecLen;
    //  вычислить смещение в файле БД для текущей записи
    if(dbHeaw[Sel].CRec<0){ s=dbHeaw[Sel].COfs; dbHeaw[Sel].CRec=0; } 
     s=dbHeaw[Sel].COfs+dbHeaw[Sel].RecLen*(dbHeaw[Sel].CRec);
    //  количество записываемых записей
    if(hRec>0){
       //  вычислить размер файла БД
       z=dbFileSize(Sel); 
       //  вычислить длинну записываемого буфера
       l=hRec*dbHeaw[Sel].RecLen;
       //  установить указатель на начало текущей записи в файле БД:
       if(mdWR==DBBFADD){
          dbModifyRecCount(Sel,hRec);             // поменять заголовок
          lseek(dbFile[Sel].Desk,0,SEEK_END);     // добавление записей
       }
       if(mdWR==DBBFWRITE){                       // перезапись записей
          lseek(dbFile[Sel].Desk,s,SEEK_SET);     // без расширения БД
          if(s+l>z){ l=z-s; }                     // скорректир размер буфера
       }
       if(mdWR==DBBFWRADD ){                      // перезапись записей с
          if(s+l>z){                              // возможным расширением БД
             hRec=divi( ((s+l)-z), dbHeaw[Sel].RecLen);
             dbModifyRecCount(Sel,hRec);
          }
          lseek(dbFile[Sel].Desk,s,SEEK_SET);     
       }
       //  записать буфер на диск в файл БД
       if(hRec>0){
          w=write (dbFile[Sel].Desk,&dbBUFF[nBuf].Arr,l); 
          r=divi(w,dbHeaw[Sel].RecLen);
       }
    }
    return(r);
}
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//  переход на указанную запись в буфере БД
_i  dbBFGoRec(_i nBuf, _i NRec){
    //  проверить правильность задания номера буфера
    if(nBuf<0||nBuf>=DBMAXBUFF){ return(-1); }
    // проверить выход номера записи за границы буфера
    if(NRec<0){ dbBUFF[nBuf].Bof=1; return(-1); }
    if(NRec>=dbBUFF[nBuf].RecCount){ dbBUFF[nBuf].Eof=1; return(-2); }
    // установить номер текущей записи в буфере и флаги
    dbBUFF[nBuf].RecCur=NRec; dbBUFF[nBuf].Bof=0; dbBUFF[nBuf].Eof=0;
    return(NRec);
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  переход по записям в буфере БД
_i  dbBFGoSkip(_i nBuf, _i SRec){
    _i  a=0;    
    //  проверить правильность задания номера буфера
    if(nBuf<0||nBuf>=DBMAXBUFF){ return(-1); }
    // проверить выход номера записи за границы буфера
    if(dbBUFF[nBuf].RecCur+SRec<0){ 
       dbBUFF[nBuf].RecCur=0; dbBUFF[nBuf].Bof=1; a=-1;
    }
    if(dbBUFF[nBuf].RecCur+SRec>=dbBUFF[nBuf].RecCount){ 
       dbBUFF[nBuf].RecCur=dbBUFF[nBuf].RecCount-1; dbBUFF[nBuf].Eof=1; a=-2;
    }
    // установить номер текущей записи в буфере и флаги
    if(a==0){
       dbBUFF[nBuf].RecCur=dbBUFF[nBuf].RecCur+SRec; 
       dbBUFF[nBuf].Bof=0; dbBUFF[nBuf].Eof=0;
    }
    return(dbBUFF[nBuf].RecCur);
}
//-----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  установить указатель на первую запись в буфере
_i  dbBFGoFirst(_i nBuf){ 
    //  проверить правильность задания номера буфера
    if(nBuf<0||nBuf>=DBMAXBUFF){ return(-1); }
    // проверить выход номера записи за границу буфера
    dbBUFF[nBuf].RecCur=0; dbBUFF[nBuf].Bof=0; dbBUFF[nBuf].Eof=0;
    return(dbBUFF[nBuf].RecCur);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  установить указатель на последнюю запись в буфере
_i  dbBFGoLast (_i nBuf){ 
    //  проверить правильность задания номера буфера
    if(nBuf<0||nBuf>=DBMAXBUFF){ return(-1); }
    // установить номер текущей записи в буфере и флаги
    dbBUFF[nBuf].RecCur=dbBUFF[nBuf].RecCount-1; 
    dbBUFF[nBuf].Bof=0; dbBUFF[nBuf].Eof=0;
    return(dbBUFF[nBuf].RecCur);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  проверка выхода за верхнюю границу в буфере после последнего перемещения
_i  dbBFBof(_i nBuf){ 
    //  проверить правильность задания номера буфера
    if(nBuf<0||nBuf>=DBMAXBUFF){ return(-1); }
    return(dbBUFF[nBuf].Bof); 
}
//----------------------------------------------------------------------------
//  проверка выхода за нижнюю границу в буфере после последнего перемещения
_i  dbBFEof(_i nBuf){ 
    //  проверить правильность задания номера буфера
    if(nBuf<0||nBuf>=DBMAXBUFF){ return(-1); }
    return(dbBUFF[nBuf].Eof); 
}
//----------------------------------------------------------------------------
//  возвращает номер текущей записи в буфере
_i  dbBFRecCur(_i nBuf){ 
    //  проверить правильность задания номера буфера
    if(nBuf<0||nBuf>=DBMAXBUFF){ return(-1); }
    return(dbBUFF[nBuf].RecCur); 
}
//----------------------------------------------------------------------------
//  возвращает количество записей в буфере считая и удаленные
_i  dbBFRecCount(_i nBuf){ 
    //  проверить правильность задания номера буфера
    if(nBuf<0||nBuf>=DBMAXBUFF){ return(-1); }
    return(dbBUFF[nBuf].RecCount); 
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  установка признака удаления записи в буфере
_i  dbBFRecDelete(_i nBuf, _i nRec, _i DFL){
    _i  nr=0;  
    //  проверить правильность задания номера буфера
    if(nBuf<0||nBuf>=DBMAXBUFF){ return(-1); }
    //  проверить выход номера записи за границы буфера
    if(nRec<0){ return(-2); }
    if(nRec>=dbBUFF[nBuf].RecCount){ return(-3); }
    //  установить признак удаления
    nr=nRec*dbBUFF[nBuf].RecLen; 
    if(DFL>0){ dbBUFF[nBuf].Arr[nr]='*'; }  // установить признак удаления
    else     { dbBUFF[nBuf].Arr[nr]=' '; }  // убрать признак удаления
    return(0);
}
//----------------------------------------------------------------------------
//  проверка признака удаления записи в буфере
_i  isBFRecDelete(_i nBuf, _i nRec){
    _i  nr=0;  
    //  проверить правильность задания номера буфера
    if(nBuf<0||nBuf>=DBMAXBUFF){ return(-1); }
    //  проверить выход номера записи за границы буфера
    if(nRec<0){ return(-2); }
    if(nRec>=dbBUFF[nBuf].RecCount){ return(-3); }
    //  проверить признак удаления
    nr=nRec*dbBUFF[nBuf].RecLen;
    if(dbBUFF[nBuf].Arr[nr]=='*'){
       return(1);
    }; return(0);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  копировать текущую запись БД в текущую запись буфера
_i  dbBFRecCopyBuf(_i Sel, _i nBuf){
    _i  s=0;
    //  проверить правильность задания областей
    if(nBuf<0||nBuf>=DBMAXBUFF){ return(-1); }
    if( Sel<0|| Sel>=DBMAXDBF ){ return(-2); }
    if(dbBUFF[nBuf].RecCur <0 ){ return(-3); }
    s=dbHeaw[Sel].RecLen*dbBUFF[nBuf].RecCur;
    memcpy(&dbBUFF[nBuf].Arr[s],&BRec[Sel],dbHeaw[Sel].RecLen);
    return(1);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  копировать текущую запись БД в текущую запись буфера
_i  dbBFBufCopyRec(_i Sel, _i nBuf){
    _i  s=0;
    //  проверить правильность задания областей
    if(nBuf<0||nBuf>=DBMAXBUFF){ return(-1); }
    if( Sel<0|| Sel>=DBMAXDBF ){ return(-2); }
    if(dbBUFF[nBuf].RecCur <0 ){ return(-3); }
    s=dbHeaw[Sel].RecLen*dbBUFF[nBuf].RecCur;
    memcpy(&BRec[Sel],&dbBUFF[nBuf].Arr[s],dbHeaw[Sel].RecLen);
    return(1);
}
//----------------------------------------------------------------------------
