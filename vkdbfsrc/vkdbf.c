//------------------------------------------------------------------------------
#include "vkdbf_.h"
#include "include.h"
#include "vkdbfdb.h"
#include "vkdbffd.c"
#include "vkdbfot.c"
#include "vkdbfrc.c"
#include "vkdbfvl.c"
#include "vkdbfbf.c"

//------------------------------------------------------------------------------
//  debug output
vd  dbDPrn (vktuc *strfrm, ...){
    va_list argptr;
    if (dbDebug > 0){
        va_start(argptr, strfrm);
        vprintf (strfrm, argptr);
        va_end  (argptr);
    }
    return;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  clearing the given array
vd  dbClear(_i Sel){
    dbFile[Sel].Name[0] =  0;
    dbFile[Sel].Name[1] =  0;
    dbFile[Sel].Desk    = -1;
    dbFile[Sel].Init    = -1;
    return;
}
//------------------------------------------------------------------------------
//  clear all arrays
vd  dbClearALL(vd){
    _i  a = 0;
    for(a = 0; a < DBMAXDBF; a++){
        dbClear(a);
    }
    return;
}
//------------------------------------------------------------------------------
//  initialization of arrays
_i  dbInit(_i Sel){
    if (Sel < 0 || Sel >= DBMAXDBF){ 
        dbDPrn("dbInit: specified area %d is out of bounds.\n", Sel);
        return(-1); 
    }
    if (dbFile[Sel].Init >= 0 ){ 
        dbDPrn("dbInit: specified area %d is already initialized.\n", Sel);
        return(-2); 
    }
    dbClear(Sel);
    dbFile[Sel].Init = Sel;
    return(Sel);
}
//------------------------------------------------------------------------------
//  defining a free area for working with a database
_i  dbSelectAvail(vd){
    _i  a = 0;
    for(a = 0; a < DBMAXDBF; a++){
        if (dbFile[a].Name[0] == 0 && dbFile[a].Name[1] == 0 &&
            dbFile[a].Desk     < 0 && dbFile[a].Init     < 0){
            return(a);
        }
    };  
    dbDPrn("dbSelectAvail: no free area.\n");
    return(-3);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  check if the database is open in the specified area
_i  dbOpenTest(_i Sel,uc *FName){
    _i  a=0;
    
    if (Sel < 0){
        for(a = 0; a < DBMAXDBF; a++){
            if (stricmp(dbFile[a].Name, FName) == 0){
                return(a);
            }
        }
        dbDPrn("dbOpenTest: the specified database file is not open.\n"); 
        return(-4);
    }
    
    if (Sel >= DBMAXDBF){ 
        dbDPrn("dbOpenTest: specified area %d is out of bounds.\n", Sel); 
        return(-5); 
    }
    
    if (dbFile[Sel].Name[0] == 0){
        dbDPrn("dbOpenTest: DB name not defined for realm %d.\n", Sel); 
        return(-6); 
    }
    
    if (dbFile[Sel].Desk < 0){ 
        dbDPrn("dbOpenTest: DB descriptor for scope not defined %d.\n", Sel); 
        return(-7); 
    }
    
    if (dbFile[Sel].Init < 0){
        dbDPrn("dbOpenTest: array for specified area %d is not initialized.\n", Sel);
        return(-8); 
    }
    
    return(Sel);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  checking the integrity of the database and header, if necessary, adjusting it
_i  dbCheck(_i Sel, uc *dbName, ui md){
    _i  FDesk=0;                                 // file descriptor
    _i  h=0,r=0;
    _l  hbr=0, szf=0, szr=0, szt=0, hrc=0;
    uc  rc[2];
    
    dbDPrn("dbCheck: the area in which the database file is open [%s]: Sel=%d.\n", dbName, Sel);

    //  check if database file exists
    if (access(dbName, 0) < 0){ 
        dbDPrn("dbCheck: no access to db file [%s] in scope %d.\n", dbName, Sel); 
        return(-9); 
    }
    
    //  if the database in the specified area is already open
    if (dbOpenTest(Sel, 0) > 0){ 
        dbDPrn("dbCheck: DB is already open in the specified area %d.\n", Sel); 
        return(-10); 
    }
    
    //  open file
    if((FDesk = open(dbName, O_BINARY|O_RDWR)) < 0){ 
        dbDPrn("dbCheck: file open error [%s].\n", dbName); 
        return(-11); 
    }

    //  file open error
    memset(BRead, 0, DBBSIZ);
    
    //  zero array for reading
    hbr = read(FDesk, &BRead, DBHDLEN + DBHDLEN * DBMAXFLD + DBBEXT);
    
    //  read the title of the opened file)
    if (hbr < DBHDLEN * 2){ 
        dbDPrn("dbCheck: error header file [%s] in the area %d.\n", dbName, Sel);
        close(FDesk);
        return(-12); 
    }

    //  fill in the header description structure from the read data
    memcpy(&dbHead[Sel], &BRead, DBHDLEN);
    
    //  size of full database header
    memcpy(&h, &dbHead[Sel].HeadSize, 2);
    dbDPrn("dbCheck: db file header size [%s]: h=%d.\n", dbName, h);
    
    //  check if header length is correct
    if((h < DBHDLEN) || (h > DBHDLEN + DBMAXFLD * DBHDLEN + DBBEXT)){
        dbDPrn("dbCheck: error header file [%s] in the area %d.\n", dbName, Sel);
        close(FDesk);
        return(-13);
    }

    //  single record size
    memcpy(&dbHeaw[Sel].RecLen, &dbHead[Sel].RecLen, 2);
    dbDPrn("dbCheck: single record size [%s]: RecLen=%d.\n", dbName, dbHeaw[Sel].RecLen);
    
    //  checking the correctness of the length of one record
    if((dbHeaw[Sel].RecLen < 1) || (dbHeaw[Sel].RecLen > (DBMAXFLD * DBSLEN))){ 
        dbDPrn("dbCheck: error header file [%s] in the area %d.\n", dbName, Sel);
        close(FDesk);
        return(-14);
    }

    //  number of records in the database
    memcpy(&dbHeaw[Sel].RecCount, &dbHead[Sel].RecCount, 4);
    dbDPrn("dbCheck: number of records in the database file [%s]: RecCount=%d.\n", dbName, dbHeaw[Sel].RecCount);

    //  number of records in the database file
    lseek(FDesk, -1, SEEK_END); read( FDesk, &rc, 1);
    lseek(FDesk,  0, SEEK_END); szf = tell(FDesk); 
    dbDPrn("dbCheck: last character in db file [%s]: rc=%d(0x%x).\n", dbName, rc[0], rc[0]);

    //  get size of all records
    szr = szf - h;
    dbDPrn("dbCheck: the size of all records in the db file [%s]: szr=%ld.\n", dbName, szr);

    //  the size of all records in the db file
    hrc = szr / dbHeaw[Sel].RecLen;
    dbDPrn("dbCheck: estimated number of records in the database file [%s]: hrc=%ld.\n", dbName, hrc);

    //  backchecking the size of all database records
    szt = hrc * dbHeaw[Sel].RecLen;

    //  If the file is larger ONLY by 1, everything is fine.
    //  some editors (DB) add an end-of-file sign to the end of the file
    if (szr == szt + 1 && (rc[0] == 0x1a || rc[0] == 0x00)){
        md = INFO;
    }
    
    //  if the sizes do not match, append the file with spaces to the desired size
    if((szt != szr || (szr == 0 && dbHeaw[Sel].RecCount > 0)) && (md == CORR)){
        if (szr == 0){
            dbHeaw[Sel].RecCount = 0;
        } else {
            dbHeaw[Sel].RecCount = hrc + 1;
        }
        
        szt = dbHeaw[Sel].RecCount * dbHeaw[Sel].RecLen;
        h = szt - szr;
        memset(BRead, 32, h);
        write (FDesk, BRead, h);
        
        //  fix db header
        memcpy(&dbHead[Sel].RecCount, &dbHeaw[Sel].RecCount, 4);
        lseek ( FDesk, 0, SEEK_SET);           
        write ( FDesk, &dbHead[Sel], DBHDLEN);
        r=1;
    }
    //  close db file
    close(FDesk);
    return(r);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  create db file
_i  dbCreate(_i Sel, uc *dbName){
    _i  FDesk=0;                            // file descriptor
    _i  f=0;                                // field number
    _i  h=0;                                // total header length
    _i  l[3];                               // length of one record
    _l  cps=0;                              // buffer offset

    // check if database file exists
    if (access(dbName, 0) == 0){
        dbDPrn("dbCreate: file [%s] access error in the area %d.\n", dbName, Sel);
        return(-21); 
    }

    // fill in the header structure
    dbHead [Sel].ID = 3;                    // Identifier
    dbModifyDate(Sel);                      // Modification date
    dbHeaw[Sel].RecCount = 0;               // Number of records in the database
    memcpy(&BRec[Sel], &dbHead[Sel], DBHDLEN);
    
    cps = cps + DBHDLEN;
    h = h + DBHDLEN;
    l[0] = 1;                               // l[0]=1 - record deletion flag
    
    for(f = 0; f < DBMAXFLD; f++){            // if the field in the structures is not filled
        if((dbFieldTest(Sel, f)) < 0 ){
            break;
        }
        
        memcpy(&dbField[Sel][f].OfsFld, &l[0], 2);
        memcpy(&BRec[Sel][cps], &dbField[Sel][f], DBHDLEN);
        
        cps = cps + DBHDLEN;
        h = h + DBHDLEN;
        l[0] = l[0] + (_i)dbField[Sel][f].Len;  // count the total length of the record
    }                                       

    // if no fields were written
    if (f < 1){ 
        dbDPrn("dbCreate: error writing fields in file [%s] areas %d.\n", dbName, Sel);
        dbFieldClear(Sel, 0);
        return(-22);
    }
    
    // write header terminator
    BRec[Sel][cps] = 13;
    cps = cps + 1;
    h = h + 1;
    
    // write extended header
    if (dbHExt[Sel].Len > 0){                
        memcpy(&BRec[Sel][cps], &dbHExt[Sel].Buf, dbHExt[Sel].Len); 
        cps = cps + dbHExt[Sel].Len;
        h = h + dbHExt[Sel].Len;
    }
    
    // padding the header structure
    memcpy(&dbHead[Sel].RecCount, &dbHeaw[Sel].RecCount, 4);
    memcpy(&dbHead[Sel].HeadSize, &h, 2);   // total heading length
    memcpy(&dbHead[Sel].RecLen,   &l, 2);   // length of one record
    memcpy(&dbHead[Sel].HFld,     &f, 2);   // number of fields in the database
    dbHead[Sel].LibFlg[0] = 'V';            // sign of creating a database of this library
    dbHead[Sel].LibFlg[1] = 'K';            //
    memcpy(&BRec[Sel][0], &dbHead[Sel], DBHDLEN);
    
    // create db file
    if((FDesk = open(dbName, O_BINARY|O_RDWR|O_CREAT|O_TRUNC)) < 0){
        dbDPrn("dbCreate: error open file [%s] in the area %d.\n", dbName, Sel);
        return(-23);
    }
    
    // write db file header
    write(FDesk, &BRec[Sel], cps);
    
    // close file
    close(FDesk); 
    
    return(1);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  open dbf file
_i  dbOpen(_i Sel, uc *dbName){
    _i  FDesk=0, f=0, h=0;
    _l  ofs=0, hbr=0, cps=0;

    // open dbf file
    if (access(dbName, 6) != 0){ 
        dbDPrn("dbOpen: File [%s] cannot be accessed in area %d\n", dbName, Sel);
        return(-31); 
    }

    // if the database in the specified area is already open
    if (dbOpenTest(Sel, 0) > 0){ 
        dbDPrn("dbOpen: DB is already open in area %d.\n");
        return(-32);
    }

    // open file
    if((FDesk=open(dbName, O_BINARY|O_RDWR)) < 0){ 
        dbDPrn("dbOpen: error opening database file [%s] in area %d\n", dbName, Sel);
        return(-33);
    }
    memset(BRead, 0, DBBSIZ);

    // read the title of the opened file
    hbr = read(FDesk, &BRead, DBHDLEN + DBHDLEN * DBMAXFLD + DBBEXT);
    if (hbr < DBHDLEN * 2){ 
        close(FDesk);
        dbDPrn("dbOpen: wrong db header size in area %d.\n", Sel);
        return(-34); 
    }
    memcpy(&dbHead[Sel], &BRead, DBHDLEN);
    cps = cps + DBHDLEN;

    // db header size
    memcpy(&h, &dbHead[Sel].HeadSize, 2);

    // single record size
    memcpy(&dbHeaw[Sel].RecLen, &dbHead[Sel].RecLen, 2);

    // checking the correctness of the read data
    if((h < DBHDLEN) || (dbHeaw[Sel].RecLen < 1)){ 
        close(FDesk);
        dbDPrn("dbOpen: invalid database header in area %d.\n", Sel);
        return(-35);
    }

    // initial value for current record number is not defined
    dbHeaw[Sel].CRec= -1;

    // number of records in the database
    memcpy(&dbHeaw[Sel].RecCount, &dbHead[Sel].RecCount, 4);
    
    // start offset for first entry
    memcpy(&dbHeaw[Sel].COfs, &dbHead[Sel].HeadSize, 2);
    
    // fill in the structure of the database fields
    for(f = 0; f < DBMAXFLD; f++){

        memcpy(&dbField[Sel][f], &BRead[cps], DBHDLEN); 
        h = h - DBHDLEN;
        cps = cps + DBHDLEN;

        if((dbField[Sel][f].Tip != 'C' && dbField[Sel][f].Tip != 'L' &&
            dbField[Sel][f].Tip != 'D' && dbField[Sel][f].Tip != 'M' &&
            dbField[Sel][f].Tip != 'N' && dbField[Sel][f].Tip != 'F' &&
            dbField[Sel][f].Tip != 'B' && dbField[Sel][f].Tip != 'I') ||(h <= 0)){ 
            break; 
        };

        // enter the offset of the current field in the record into the structure
        memcpy(&dbField[Sel][f].OfsFld, &ofs, 4); 

        // store current offset
        dbFielw[Sel][f].OfsFld = ofs;
        ofs = ofs + dbField[Sel][f].Len;
    }

    dbHExt[Sel].Len = dbHeaw[Sel].COfs - DBHDLEN - (DBHDLEN * f);
    dbHeaw[Sel].FieldCount = f;

    // if not a single field is read, this is not a database file
    if (f < 1){ 
        dbDPrn("dbOpen: no read db fields in area %d.\n", Sel);
        close(FDesk);
        return(-36); 
    }

    // extended header reading
    if (dbHExt[Sel].Len > 0){ 
        memcpy(&dbHExt[Sel].Buf, &BRead[cps], dbHExt[Sel].Len);
    }
    
    // write the number of fields in the database to the structure
    memcpy(&dbHead[Sel].HFld, &f, 2);
    
    // set the database pointer to record N0
    if (dbHeaw[Sel].RecCount > 0){
        dbHeaw[Sel].CRec = 0;
    }
    
    // pass filename and descriptor to arrays
    strcpy(dbFile[Sel].Name, dbName);
    dbFile[Sel].Desk = FDesk;
    dbFile[Sel].Mod  = 0;
    
    //
    dbDPrn("dbOpen: database file [%s] is open in area %d.\n", dbFile[Sel].Name, Sel);
    return(1);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  closing the database
vd  dbClose(_i Sel){
    _i  a=0;
    _l  cps=0;
    
    dbDPrn("dbClose: closing database file [%s] in area %d:\n", dbFile[Sel].Name, Sel);
    
    // correct database number
    if (dbOpenTest(Sel,0) < 1){
        return;
    }
    
    // if there was no data written to the file, close it without modifying the header
    if (dbFile[Sel].Mod == 0){
        dbDPrn("dbClose: database file [%s] closed in area %d.\n", dbFile[Sel].Name, Sel);
        close(dbFile[Sel].Desk);
        dbClear(Sel);
        return;
    }
    
    // fill in the database header
    memcpy(&BRec[Sel], &dbHead[Sel], DBHDLEN);
    cps = cps + DBHDLEN;
    
    // fill in the database fields
    for(a = 0; a < dbHeaw[Sel].FieldCount; a++){
        memcpy(&BRec[Sel][cps], &dbField[Sel][a], DBHDLEN);
        cps = cps + DBHDLEN;
    }
    
    // fill in extended header
    if (dbHExt[Sel].Len > 0){ 
        memcpy(&BRec[Sel][cps], &dbHExt[Sel].Buf, dbHExt[Sel].Len); 
        cps = cps + dbHExt[Sel].Len;
    }
    
    // pointer to the beginning of the file, write the header and close the file
    lseek(dbFile[Sel].Desk, 0, SEEK_SET);
    write(dbFile[Sel].Desk, BRec[Sel], cps);
    close(dbFile[Sel].Desk);

    dbDPrn("dbClose: database file [%s] closed in area %d.\n", dbFile[Sel].Name, Sel);
    dbClear(Sel);

    return;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  copy the database structure to a new file
_i  dbStructCopy(uc *dbSrc, uc *dbDst){
    _i  Sel=0, FDesk=0, hb=0, r=0;
    
    dbDPrn("dbStructCopy: Start:\n");
    // create a file to write the structure of the new database
    if((FDesk = open(dbDst, O_BINARY|O_RDWR|O_CREAT|O_TRUNC)) < 0){
        dbDPrn("dbStructCopy: error open file [%s]:\n", dbDst);
        return(-1);
    }
    
    // open source database
    if((Sel = dbOpenTest(-1, dbSrc)) < 0){ 
        Sel = dbSelectAvail(); dbInit(Sel);
        if (dbOpen(Sel,dbSrc)<0){ 
            dbDPrn("dbStructCopy: error opening database [%s][%d]:\n", dbSrc, Sel);
            close(FDesk);
            return(-2);
        }
    }

    // set the pointer to the beginning of the source database
    lseek(dbFile[Sel].Desk, 0, SEEK_SET);

    // read full database header into buffer
    hb = read(dbFile[Sel].Desk, BRead, dbHeaw[Sel].COfs);
    
    // write header to new file
    if (hb == dbHeaw[Sel].COfs){ 
        BRead[4] = BRead[5] = BRead[6] = BRead[7] = 0;
        write(FDesk, BRead, hb);
    } else { 
        r=-3; 
    }

    // close files
    dbClose(Sel);
    close(FDesk);
    dbDPrn("dbStructCopy: end:\n");
    
    return (r);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  overwriting the database file excluding records marked for deletion
_i  dbPack(uc *dbBase, uc *dbTemp){
    _i SBase=0, STemp=0;
    _l hr=0;
    
    // create temporary database
    if (dbStructCopy(dbBase, dbTemp) < 0){
        return(-1);
    }
    dbDPrn("dbPack: Start: *******************\n");

    // define free area for databases
    if((SBase = dbSelectAvail()) < 0){
        dbDPrn("ER:-2\n");
        return(-2);
    }
    dbInit(SBase);
    if((STemp = dbSelectAvail()) < 0){
        dbDPrn("ER:-3\n");
        return(-3);
    }
    dbInit(STemp);

    // database validation
    if (dbCheck(SBase, dbBase, INFO) < 0){
        dbDPrn("ER:-4\n");
        return(-4);
    }

    // open databases
    if (dbOpen(SBase, dbBase) < 0){
        dbDPrn("ER:-5\n");
        return(-5);
    }
    if (dbOpen(STemp, dbTemp) < 0){
        dbClose(SBase);
        dbDPrn("ER:-6\n");
        return(-6);
    }

    // get the number of records in the database
    hr = dbRecCount(SBase) - 1;

    // jump to 1st database record
    dbGoFirst(SBase);

    // for each database entry
    for(;;){
        // read current database record
        dbRecRead(SBase);
        if (isRecDFL(SBase) == 0){
            // add an entry to the temporary database if there is no sign of deleted.
            if (dbRecAdd(STemp) > 0){
                // write the read record to the temporary database
                dbRecCopy(SBase, STemp);
                dbRecWrite(STemp);
            }
        }
        // move to next database record if end of db - exit
        if((dbGoSkip(SBase, 1)) < 0){
            break;
        }
    }

    // close bases
    dbClose(SBase); 
    dbClose(STemp);
    
    dbDPrn("dbPack: end: ********************\n");
    return(0);
}
//------------------------------------------------------------------------------
