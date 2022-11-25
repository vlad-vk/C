//------------------------------------------------------------------------------
//  additional functions

//------------------------------------------------------------------------------
//  memory size of library structures
_i  dbGetMemLib(vd){
    _i  AM = 0;
    AM = AM + sizeof(BHead  );
    AM = AM + sizeof(BRead  );
    AM = AM + sizeof(BSFld  );
    AM = AM + sizeof(BRec   );
    AM = AM + sizeof(dbFile );
    AM = AM + sizeof(dbHead );
    AM = AM + sizeof(dbHExt );
    AM = AM + sizeof(dbHeaw );
    AM = AM + sizeof(dbField);
    AM = AM + sizeof(dbFielw);
#ifndef OBC
    AM = AM + sizeof(dbBUFF);
#endif
    return(AM);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  return calculated database size
_l  dbFileSize(_i Sel){
    _i  sz  = 0;
    if (Sel < 0 || Sel >= DBMAXDBF){
        return(-1);
    }
    
    sz = dbHeaw[Sel].COfs + dbHeaw[Sel].RecCount * dbHeaw[Sel].RecLen;
    
    return(sz);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  fill in the structure of the current date and time
ul  dbCurDate(uc md){
    ul  r = 0;
    dbTMV = time(NULL);
    dbtm  = localtime(&dbTMV);
    
    // return current date
    if (md == 'D' || md == 'd' || md == 0){
        r = (((ul) dbtm->tm_year + (ul) 1900) * (ul) 10000) +
            (((ul) dbtm->tm_mon  + (ul) 1   ) * (ul) 100  ) + (ul) dbtm->tm_mday;
    }
    
    // return current time
    if (md == 'T' || md == 't' || md == 1){
        r = ((ul) dbtm->tm_hour * (ul) 10000) +
            ((ul) dbtm->tm_min  * (ul) 100  ) + (ul) dbtm->tm_sec;
    }
    
    return(r);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  set pointer in db file for current record and specific field
_l  dbFieldOffset(_i Sel, _i Fld, _i md){
    _l  r = 0, s = 0;
    
    if (md == 0){
        return(dbFielw[Sel][Fld].OfsFld + 1);
    }
    if (dbHeaw[Sel].CRec < 0){
        return(-1);
    }
    
    s = dbHeaw[Sel].COfs + dbHeaw[Sel].RecLen * dbHeaw[Sel].CRec + dbFielw[Sel][Fld].OfsFld + 1;
    
    r = lseek(dbFile[Sel].Desk, s, SEEK_SET);
    
    if (r == EBADF ){ r = -5; }
    if (r == EINVAL){ r = -7; }
    if (r == ESPIPE){ r = -9; }
    
    return(r);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  last update date modification
vd  dbModifyDate(_i Sel){

    dbCurDate('D');

    dbHead[Sel].Year = (uc) (dbtm->tm_year - 100);
    dbHead[Sel].Mon  = (uc) (dbtm->tm_mon  + 1);
    dbHead[Sel].Day  = (uc) (dbtm->tm_mday);
    
    return;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  modification of the number of entries
_i  dbModifyRecCount(_i Sel, _i md){
    _i  a = 0;    
    
    //  deleting one entry
    if (md == -1){
        if (dbHeaw[Sel].RecCount > 0){ 
            dbHeaw[Sel].RecCount--;
            a = dbHeaw[Sel].RecCount; 
        }
    }
    
    //  adding an entry
    if (md > 0){
        if (dbHeaw[Sel].RecCount + md<DBMAXREC - 1){ 
            dbHeaw[Sel].RecCount = dbHeaw[Sel].RecCount + md; 
            a = dbHeaw[Sel].RecCount; 
        }
    }
    
    // overwrite title as is (title fix)
    if (md == 0){
        a = dbHeaw[Sel].RecCount;
    }
    
    // if the number of entries has been changed, overwrite the header
    if (a > 0){
        memcpy(&dbHead[Sel].RecCount, &dbHeaw[Sel].RecCount, 4);

        lseek ( dbFile[Sel].Desk, 0, SEEK_SET);           

        write ( dbFile[Sel].Desk, &dbHead[Sel], DBHDLEN);

        dbFile[Sel].Mod = 1;
    }
    return(a);    
}
//------------------------------------------------------------------------------
