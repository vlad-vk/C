//------------------------------------------------------------------------------
// This file contains functions for working with the database through the buffer:

//------------------------------------------------------------------------------
//  search for free buffer
_i  dbBFBufferAvail(){
    _i  a = 0;
    for(a = 0; a < DBMAXBUFF; a++){
        if (dbBUFF[a].Init < 0){ 
            return(a);
        }
    }
    return(-1);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  maximum number of buffer entries
_i  dbBFRecInBuf(_i Sel, _i nBuf){
    if (Sel < 0 || Sel >= DBMAXDBF){ 
        return(-1);
    }
    if (dbHeaw[Sel].RecLen < 1){
        return(-2);
    }
    dbBUFF[nBuf].RecCount = DBBSIZ / dbHeaw[Sel].RecLen;
    return(dbBUFF[nBuf].RecCount);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  clearing the buffer with the specified number
_i  dbBFClear(_i nBuf){
    _i  a = 0;    
    
    if (nBuf < 0 || nBuf >= DBMAXBUFF){
        return(-1);
    }
    
    dbBUFF[nBuf].RecCur     = -1; 
    dbBUFF[nBuf].RecCount   =  0;
    dbBUFF[nBuf].Init       = -1;
    
    for(a = 0; a < DBBSIZ; a++){
        dbBUFF[nBuf].Arr[a] =  0;
    }
    return(0);
}
//------------------------------------------------------------------------------
//  clearing all buffers
vd  dbBFClearALL(vd){
    _i  a = 0;
    for(a = 0; a < DBMAXBUFF; a++){
        dbBFClear(a);
    }
    return;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  reserve a buffer for future use
_i  dbBFInit(_i nBuf){
    if (nBuf < 0 || nBuf >= DBMAXBUFF){
        return(-1);
    }
    if (dbBUFF[nBuf].Init >= 0){
        return(-2);
    }
    dbBUFF[nBuf].Init = nBuf;
    return(nBuf);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  reading the database file into the buffer from the current record
_i  dbBFRead(_i Sel, _i nBuf, _i hRec){
    _i  a=0, x=0;
    _l  s=0;

    //  check the correctness of the assignment of areas
    if (nBuf< 0 || nBuf >= DBMAXBUFF){
        return(-1);
    }
    if (Sel < 0 ||  Sel >= DBMAXDBF ){
        return(-1);
    }

    //  check current record number and record length
    if (dbHeaw[Sel].CRec < 0 || dbHeaw[Sel].CRec > DBMAXREC){
        return(-2);
    }

    //  check record length
    if (dbHeaw[Sel].RecLen <= 0 || dbHeaw[Sel].RecLen > MRECLEN){
        return(-3);
    }

    //  allowed number of entries in the buffer
    dbBFRecInBuf(Sel, nBuf);

    //  calculate offset in db file for current record
    s = dbHeaw[Sel].COfs + dbHeaw[Sel].RecLen * dbHeaw[Sel].CRec;

    //  the size of one record to transfer to the buffer array
    dbBUFF[nBuf].RecLen = dbHeaw[Sel].RecLen;

    //  calculate the number of bytes corresponding to the number of hRec records
    x = hRec * dbHeaw[Sel].RecLen;
    if (x > DBBSIZ){
        x = DBBSIZ;
    }

    //  set the pointer to the beginning of the current record in the database file
    lseek(dbFile[Sel].Desk, s, SEEK_SET);

    //  read file into buffer
    a = read(dbFile[Sel].Desk, dbBUFF[nBuf].Arr, x);

    //  return the number of records in the read buffer
    if (a < dbHeaw[Sel].RecLen){
        return(0);
    }
    
    return(a / dbHeaw[Sel].RecLen);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  write buffer to database file (the buffer is written from the N0 record in
//  the buffer to the current DB record, except for the append mode)
_i  dbBFWrite(_i Sel, _i nBuf, _i hRec, _i mdWR){
    _i  l=0, r=0, w=0, hrc=0;
    _l  s=0, z=0;

    //  check the correctness of the assignment of areas
    if (nBuf < 0 || nBuf >= DBMAXBUFF){
        return(-1);
    }
    if (Sel < 0 || Sel >= DBMAXDBF){
        return(-2);
    }
    if (mdWR != DBBFADD && mdWR != DBBFWRITE && mdWR != DBBFWRADD){
        mdWR  = DBBFADD;
    }

    //  check record length
    if (dbHeaw[Sel].RecLen <= 0 || dbHeaw[Sel].RecLen > MRECLEN){
        return(-3);
    }

    //  check the number of records being written
    if (hRec < 0 || hRec > (hrc = (DBBSIZ / dbHeaw[Sel].RecLen))){
        return(-4);
    }

    //  allowed number of entries in the buffer
    dbBFRecInBuf(Sel, nBuf);

    //  the size of one record to transfer to the buffer array
    dbBUFF[nBuf].RecLen = dbHeaw[Sel].RecLen;

    //  calculate offset in db file for current record
    if (dbHeaw[Sel].CRec < 0){
        s = dbHeaw[Sel].COfs;
        dbHeaw[Sel].CRec = 0;
    }
    s = dbHeaw[Sel].COfs + dbHeaw[Sel].RecLen * (dbHeaw[Sel].CRec);
    
    //  number of recorded records
    if (hRec > 0){

        //  calculate db file size
        z = dbFileSize(Sel); 

        //  calculate the length of the buffer being written
        l = hRec*dbHeaw[Sel].RecLen;

        //  set the pointer to the beginning of the current record in the database file:
        if(mdWR == DBBFADD){
            dbModifyRecCount(Sel, hRec);            // change title
            lseek(dbFile[Sel].Desk, 0, SEEK_END);   // adding records
        }
        // overwriting records without db extension
        if (mdWR == DBBFWRITE){                     
            lseek(dbFile[Sel].Desk, s, SEEK_SET);    
            if(s + l > z){ l = z - s; }             // adjusted buffer size
        }

        // overwriting records with possible DB extension
        if (mdWR == DBBFWRADD){                    
            if(s + l > z){                         
                hRec = ((s + l) - z) / dbHeaw[Sel].RecLen; 
                dbModifyRecCount(Sel, hRec);
            }
            lseek(dbFile[Sel].Desk, s, SEEK_SET);     
        }

        //  write buffer to disk to db file
        if (hRec > 0){
            w = write(dbFile[Sel].Desk, &dbBUFF[nBuf].Arr, l); 
            r = w / dbHeaw[Sel].RecLen;
        }
    }
    return(r);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  jump to the specified record in the database buffer
_i  dbBFGoRec(_i nBuf, _i NRec){
    
    // check if the buffer number is correct
    if (nBuf < 0 || nBuf >= DBMAXBUFF){
        return(-1);
    }

    // check if the record number is out of buffer bounds
    if (NRec < 0){
        dbBUFF[nBuf].Bof = 1;
        return(-1);
    }

    if (NRec >= dbBUFF[nBuf].RecCount){
        dbBUFF[nBuf].Eof = 1;
        return(-2);
    }

    // set current buffer entry number and flags
    dbBUFF[nBuf].RecCur = NRec;
    dbBUFF[nBuf].Bof    = 0;
    dbBUFF[nBuf].Eof    = 0;
    
    return(NRec);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  traversing records in the database buffer
_i  dbBFGoSkip(_i nBuf, _i SRec){
    _i  a = 0;
    
    // check if the buffer number is correct
    if (nBuf < 0 || nBuf >= DBMAXBUFF){
        return(-1);
    }
    
    // check if the record number is out of buffer bounds
    if (dbBUFF[nBuf].RecCur + SRec < 0){ 
        dbBUFF[nBuf].RecCur = 0;
        dbBUFF[nBuf].Bof    = 1;
        a = -1;
    }
    if (dbBUFF[nBuf].RecCur + SRec >= dbBUFF[nBuf].RecCount){ 
        dbBUFF[nBuf].RecCur = dbBUFF[nBuf].RecCount - 1;
        dbBUFF[nBuf].Eof = 1;
        a = -2;
    }
    
    // set current buffer entry number and flags
    if (a == 0){
        dbBUFF[nBuf].RecCur = dbBUFF[nBuf].RecCur + SRec; 
        dbBUFF[nBuf].Bof = 0;
        dbBUFF[nBuf].Eof = 0;
    }
    
    return(dbBUFF[nBuf].RecCur);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  set the pointer to the first entry in the buffer
_i  dbBFGoFirst(_i nBuf){ 

    // check if the buffer number is correct
    if (nBuf < 0 || nBuf >= DBMAXBUFF){
        return(-1);
    }

    // check if the record number is out of the buffer
    dbBUFF[nBuf].RecCur = 0;
    dbBUFF[nBuf].Bof    = 0;
    dbBUFF[nBuf].Eof    = 0;

    return(dbBUFF[nBuf].RecCur);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  set the pointer to the last entry in the buffer
_i  dbBFGoLast (_i nBuf){ 

    // check if the buffer number is correct
    if (nBuf < 0 || nBuf >= DBMAXBUFF){
        return(-1);
    }

    // set current buffer entry number and flags
    dbBUFF[nBuf].RecCur = dbBUFF[nBuf].RecCount - 1; 
    dbBUFF[nBuf].Bof = 0;
    dbBUFF[nBuf].Eof = 0;

    return(dbBUFF[nBuf].RecCur);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  check for overflow in the buffer after the last move
_i  dbBFBof(_i nBuf){ 
    // check if the buffer number is correct
    if (nBuf < 0 || nBuf >= DBMAXBUFF){
        return(-1);
    }
    return(dbBUFF[nBuf].Bof); 
}
//------------------------------------------------------------------------------
//  check for underflow in the buffer after the last move
_i  dbBFEof(_i nBuf){ 
    // check if the buffer number is correct
    if (nBuf < 0 || nBuf >= DBMAXBUFF){
        return(-1);
    }
    return(dbBUFF[nBuf].Eof); 
}
//------------------------------------------------------------------------------
//  returns the number of the current record in the buffer
_i  dbBFRecCur(_i nBuf){ 
    // check if the buffer number is correct
    if (nBuf < 0 || nBuf >= DBMAXBUFF){
        return(-1);
    }
    return(dbBUFF[nBuf].RecCur); 
}
//------------------------------------------------------------------------------
//  returns the number of records in the buffer counting and removed
_i  dbBFRecCount(_i nBuf){ 
    // check if the buffer number is correct
    if (nBuf < 0 || nBuf >= DBMAXBUFF){
        return(-1);
    }
    return(dbBUFF[nBuf].RecCount); 
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  setting the flag for deleting a record in the buffer
_i  dbBFRecDelete(_i nBuf, _i nRec, _i DFL){
    _i  nr = 0;  
    
    // check if the buffer number is correct
    if (nBuf < 0 || nBuf >= DBMAXBUFF){
        return(-1);
    }
    
    // check if the record number is out of buffer bounds
    if (nRec < 0){
        return(-2);
    }
    
    if (nRec >= dbBUFF[nBuf].RecCount){
        return(-3);
    }
    
    // set deletion flag
    nr = nRec * dbBUFF[nBuf].RecLen; 
    if (DFL > 0){
        dbBUFF[nBuf].Arr[nr] = '*';
    } else { 
    // remove deletion flag
        dbBUFF[nBuf].Arr[nr] = ' ';
    }
    
    return(0);
}
//------------------------------------------------------------------------------
//  checking the sign of deleting a record in the buffer
_i  isBFRecDelete(_i nBuf, _i nRec){
    _i  nr = 0;  
    
    // check if the buffer number is correct
    if (nBuf < 0 || nBuf >= DBMAXBUFF){
        return(-1);
    }
    
    // check if the record number is out of buffer bounds
    if (nRec < 0){
        return(-2);
    }
    if (nRec >= dbBUFF[nBuf].RecCount){
        return(-3);
    }
    
    // check deletion sign
    nr = nRec * dbBUFF[nBuf].RecLen;
    if (dbBUFF[nBuf].Arr[nr] == '*'){
        return(1);
    }
    
    return(0);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  copy the current database entry to the current buffer entry
_i  dbBFRecCopyBuf(_i Sel, _i nBuf){
    _i s = 0;
    
    // check the correctness of the assignment of areas
    if (nBuf < 0 || nBuf >= DBMAXBUFF){
        return(-1);
    }
    if (Sel < 0 || Sel >= DBMAXDBF){
        return(-2);
    }
    if (dbBUFF[nBuf].RecCur < 0){
        return(-3);
    }
    
    s = dbHeaw[Sel].RecLen * dbBUFF[nBuf].RecCur;
    
    memcpy(&dbBUFF[nBuf].Arr[s], &BRec[Sel], dbHeaw[Sel].RecLen);

    return(1);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  copy the current database entry to the current buffer entry
_i  dbBFBufCopyRec(_i Sel, _i nBuf){
    _i s = 0;
    
    // check the correctness of the assignment of areas
    if (nBuf < 0 || nBuf >= DBMAXBUFF){
        return(-1);
    }
    if (Sel < 0 || Sel >= DBMAXDBF ){
        return(-2);
    }
        if(dbBUFF[nBuf].RecCur < 0){
        return(-3);
    }
    
    s = dbHeaw[Sel].RecLen * dbBUFF[nBuf].RecCur;
    
    memcpy(&BRec[Sel], &dbBUFF[nBuf].Arr[s], dbHeaw[Sel].RecLen);
    
    return(1);
}
//------------------------------------------------------------------------------
