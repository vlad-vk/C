//------------------------------------------------------------------------------
//  functions for working with records

//------------------------------------------------------------------------------
//  returns the number of records in the database, counting deleted
_l  dbRecCount(_i Sel){
    return(dbHeaw[Sel].RecCount);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  set pointer to first record
_l  dbGoFirst(_i Sel){ 
    if (dbHeaw[Sel].CRec < 0){
        return(-1);
    }
    dbHeaw[Sel].CRec = 0; 
    dbHeaw[Sel].Bof  = 0;
    dbHeaw[Sel].Eof  = 0;
    return(dbHeaw[Sel].CRec);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  set pointer to last entry
_l  dbGoLast(_i Sel){ 
    if (dbHeaw[Sel].CRec < 0){
        return(-1);
    }
    dbHeaw[Sel].CRec = dbHeaw[Sel].RecCount - 1;
    dbHeaw[Sel].Bof  = 0;
    dbHeaw[Sel].Eof  = 0;
    return(dbHeaw[Sel].CRec);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  set a pointer to the specified record N
_l  dbGoRec(_i Sel, _l NRec){ 
    if (NRec < 0){
        dbHeaw[Sel].Bof = 1;
        return(-1);
    }
    if (NRec >= dbHeaw[Sel].RecCount){
        dbHeaw[Sel].Eof = 1;
        return(-2);
    }
    dbHeaw[Sel].CRec = NRec;
    dbHeaw[Sel].Bof  = 0;
    dbHeaw[Sel].Eof  = 0;
    return(NRec);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  set the pointer to the offset of the specified entry
_l  dbGoSkip(_i Sel, _l SRec){ 
    if (dbHeaw[Sel].CRec + SRec < 0){
        dbHeaw[Sel].Bof  = 1;
        return(-1);
    }
    if (dbHeaw[Sel].CRec + SRec >= dbHeaw[Sel].RecCount){
        dbHeaw[Sel].Eof  = 1;
        return(-2);
    }
    dbHeaw[Sel].CRec = dbHeaw[Sel].CRec + SRec; 
    dbHeaw[Sel].Bof  = 0;
    dbHeaw[Sel].Eof  = 0;
    return(SRec);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Checking if the database is out of bounds after the last move
_i  dbBof(_i Sel){
    return(dbHeaw[Sel].Bof);
}
//------------------------------------------------------------------------------
//  Checking if the database has gone below the last move
_i  dbEof(_i Sel){
    return(dbHeaw[Sel].Eof);
}
//------------------------------------------------------------------------------
//  returns the number of the current record in the database
_l  dbRecCur(_i Sel){
    return(dbHeaw[Sel].CRec);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  adding an empty record to the database
_i  dbRecAdd(_i Sel){
    _i  a = 0;
    //  check record length
    if (dbHeaw[Sel].RecLen < 0 || dbHeaw[Sel].RecLen > MRECLEN){
        return(-1);
    }
    //  set pointer to end of file
    memset(BRec[Sel], 0, dbHeaw[Sel].RecLen);
    lseek(dbFile[Sel].Desk, 0, SEEK_END);
    a = write(dbFile[Sel].Desk, BRec[Sel], dbHeaw[Sel].RecLen);
    //  change the total number of records field in the db header
    if (dbModifyRecCount(Sel, 1) > 0){
        dbHeaw[Sel].CRec = dbHeaw[Sel].RecCount - 1;
    }
    return(a);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  write write buffer to disk
_i  dbRecWrite(_i Sel){
    _i  a = 0;
    _l  s = 0;
    //  check record length
    if (dbHeaw[Sel].RecLen < 0 || dbHeaw[Sel].RecLen > MRECLEN){
        return(-1);
    }
    s = dbHeaw[Sel].COfs + dbHeaw[Sel].RecLen * dbHeaw[Sel].CRec;
    lseek(dbFile[Sel].Desk, s, SEEK_SET);
    a = write(dbFile[Sel].Desk, BRec[Sel], dbHeaw[Sel].RecLen);
    dbFile[Sel].Mod = 1;
    return(a);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  read write buffer from disk
_i  dbRecRead(_i Sel){
    _i  a = 0;    
    _l  s = 0;
    //  check record length
    if (dbHeaw[Sel].RecLen < 0 || dbHeaw[Sel].RecLen > MRECLEN){
        return(-1);
    }
    s = dbHeaw[Sel].COfs + dbHeaw[Sel].RecLen * dbHeaw[Sel].CRec;
    lseek(dbFile[Sel].Desk, s, SEEK_SET);
    a = read(dbFile[Sel].Desk, BRec[Sel], dbHeaw[Sel].RecLen);
    if (BRec[Sel][0] == '*'){
        BCRD[Sel][0] =  '*';
    } else {
        BCRD[Sel][0] =  ' ';
    }
    return(a);
}
//------------------------------------------------------------------------------
//  checking the sign of deleting a record after a read operation
_i  isRecDFL(_i Sel){
    _i  r = 0;
    if (BCRD[Sel][0] == '*'){
        r = 1;
    }
    return(r);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  copy the write buffer from one area to another
_i  dbRecCopy(_i Src, _i Des){
    memcpy(&BRec[Des], &BRec[Src], MRECLEN);
    return(0);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  check if the record is deleted or set the deletion flag
_i  dbRecDeleteFN(_i Sel, _l nRec, _i md, _i Flg){
    _i  r = 0;
    _l  s = 0;
    if (dbHeaw[Sel].RecLen < 0 || dbHeaw[Sel].RecLen > MRECLEN){
        return(-1);
    }
    if (nRec < 0){
        s = dbHeaw[Sel].COfs + dbHeaw[Sel].RecLen * dbHeaw[Sel].CRec;
    } else {
        if (nRec>dbHeaw[Sel].RecCount){
            nRec = dbHeaw[Sel].RecCount;
        }
        s = dbHeaw[Sel].COfs + dbHeaw[Sel].RecLen * nRec;
    }
    lseek(dbFile[Sel].Desk, s, SEEK_SET);
    //  if it is check mode
    if (md == 0){
        read(dbFile[Sel].Desk, BCRD[Sel], 1);
        if (BCRD[Sel][0] == '*'){
            r = 1;
        }
    } else {
    //  if this is the deletion sign recording mode
        if (Flg > 0){
            BCRD[Sel][0] = '*';
        } else {
            BCRD[Sel][0] = ' ';
        }
        write(dbFile[Sel].Desk, &BCRD[Sel], 1);
        r = 1;
    }    
    return(r);
}
//------------------------------------------------------------------------------
//  check if the entry is deleted
_i  isRecDelete(_i Sel, _l nRec){
    return(dbRecDeleteFN(Sel, nRec, 0, 0));
}
//------------------------------------------------------------------------------
//  set the deletion flag for the specified record
_i  dbRecDelete(_i Sel, _l nRec, _i Flg){
    return(dbRecDeleteFN(Sel, nRec, 1, Flg));
}
//------------------------------------------------------------------------------
