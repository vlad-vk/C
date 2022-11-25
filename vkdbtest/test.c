//------------------------------------------------------------------------------
#include "include.h"
#include "vkdbf_.h"
#include "vkdbf.h"

//------------------------------------------------------------------------------
_i main(vd){
    _i a=0, b=0, md=1;
    uc c[10];
   
    dbClearALL();
    dbFieldFill(0, 0, "NUMBER",  'I',  2, 0);
    dbFieldFill(0, 1, "NAME",    'C', 30, 0);
    dbFieldFill(0, 2, "FLAG",    'L',  1, 0);
    dbFieldFill(0, 3, "DATE",    'D',  8, 0);
    dbFieldFill(0, 4, "SUMMA",   'B',  4, 4);
    dbFieldFill(0, 5, "PHONE",   'N', 10, 0);
    dbFieldFill(0, 6, "TESTNUM", 'F',  5, 1);

    dbCreate(0, "test00.dbf");
    dbOpen  (0, "test00.dbf");
    
    if (dbRecCount(0) < 1){
        for(a = 0; a < 5; a++){
            dbRecAdd(0);
            dbFieldPutVal(0, 0, 0, a, md);
        }
    }
    
    printf("Field_0=[%s][%c,%d,%d]  Field_1=[%s][%c,%d,%d]\n",
            dbFieldGetName(0, 0), dbFieldGetType(0, 0),
            dbFieldGetLen (0, 0), dbFieldGetDec (0, 0),
            dbFieldGetName(0, 1), dbFieldGetType(0, 1),
            dbFieldGetLen (0, 1), dbFieldGetDec (0, 1));
    printf("Number for field [NUMBER], Reg=1: %d\n", (_i)dbFieldGetVal(0, 1, "NUMBER", md));
    printf("Number for field [Number], Reg=1: %d\n", (_i)dbFieldGetVal(0, 1, "Number", md));

    dbGoFirst(0);
    dbFieldPutStr(0, -1, "NAME", "This is the first entry in the database.", md);
    dbGoLast(0);
    dbFieldPutStr(0, -1, "NAME", "This is the last entry in the database.", md);
    dbGoRec(0, 1);
    dbFieldPutStr(0, -1, "NAME", "This is the second post", md);
    dbGoSkip(0, 2);
    dbFieldPutStr(0, -1, "NAME", "This is the fourth post", md);
    dbGoSkip(0, -1);
    dbFieldPutStr(0, -1, "NAME", "333333333333333333333333333333333333333", md);
    dbGoSkip(0, -10);
    
    if(dbBof(0)){ 
        printf("Exceeding the upper limit\n");
        dbGoFirst(0);
    }
    
    dbFieldPutLog(0, -1, "FLAG", 'Y', md);
    dbFieldPutDat(0, -1, "DATE", "D", md);
    dbGoSkip(0, 10);
    
    if(dbEof(0)){ 
        printf("Out of bounds\n");
        dbGoLast(0);
    }
    
    if(dbBof(0)==0 && dbEof(0)==0){ 
        printf("The current record number is within acceptable limits %ld\n", dbRecCur(0));
    }
    
    dbFieldPutLog(0, -1, "FLAG", '1', md);
    dbGoSkip(0, -1);
    dbGoRec (0,  2);
    dbFieldPutDat(0, -1,"DATE", "20030331", md);
    
    if(dbFieldPutLog(0, 0, 0, '1', md) < 0){ 
        printf("Boolean field write error\n");
    }
    
    dbFieldPutVal(0, -1, "PHONE",   58032,          md);
    dbFieldPutVal(0, -1, "TESTNUM", 25.1,           md);
    dbFieldPutVal(0, -1, "SUMMA",   12345678.12345, md);
    dbGoRec (0, 3);
    dbFieldPutVal(0, -1, "SUMMA",   11111111.22222, md);
    dbGoRec (0, 2);
    
    strcpy(c,dbFieldGetDat(0, -1, "DATE", md));
    
    printf("Number: %d   Phone: %ld   Summa: %.4f   Date: %s\n",
        (_i)dbFieldGetVal(0, -1, "NUMBER", md),
        (_l)dbFieldGetVal(0, -1, "PHONE" , md),
        dbFieldGetVal(0, -1, "SUMMA", md), c);
    printf("String[%s][%c]\n", dbFieldGetStr(0, -1, "NAME", md), dbFieldGetLog(0, -1, "FLAG", md));
    
    dbClose(0);
   
    //--------------------------------------------------------------------------
    for(b = 0; b < DBMAXFLD; b++){
        sprintf(c, "FIELD%02d", b);
        dbFieldFill (0, b, c, 'B', 4, 0);
    }
    
    dbFieldClear(0, b);
    dbCreate(0, "test01.dbf");
    
    if (dbOpen(0, "test01.dbf") > 0){
        
        printf ("-----------------------------------------------------------\n");
        printf ("Database file:[%s]\n",             dbFile[0].Name);
        printf ("Number of records in the database:%ld\n", dbHeaw[0].RecCount);
        printf ("Length of one record:%d\n",        dbHeaw[0].RecLen);
        printf ("Current record number:%ld\n",      dbHeaw[0].CRec);
        printf ("Data start offset:%ld\n",          dbHeaw[0].COfs);
        printf ("-----------------------------------------------------------\n");
        printf ("Recording start time: %06ld\n",    dbCurDate('T'));
        
        for(a = 0; a < 1000; a++){
            dbRecAdd(0);
            for(b = 0; b < DBMAXFLD; b++){
                dbFieldPutVal(0, b, 0, (_f)a, md);
            }
        }
        
        printf ("Record end time: %06ld\n",         dbCurDate('T'));
        printf ("-----------------------------------------------------------\n");
        printf ("Number of records in the database:%ld\n", dbHeaw[0].RecCount);
        printf ("Current record number:%ld\n",      dbHeaw[0].CRec);
        printf ("-----------------------------------------------------------\n");
        
        dbClose(0);
    }
    
    //--------------------------------------------------------------------------
    for(b = 0; b < DBMAXFLD; b++){
        sprintf(c, "FIELD%02d", b);
        dbFieldFill (0, b, c, 'B', 4, 0);
    }
    
    dbFieldClear(0, b);
    dbCreate(0, "test02.dbf");
    
    if (dbOpen (0, "test02.dbf") > 0){
        
        printf ("-----------------------------------------------------------\n");
        printf ("Database file:[%s]\n",             dbFile[0].Name);
        printf ("Number of records in the database:%ld\n", dbHeaw[0].RecCount);
        printf ("Length of one record:%d\n",        dbHeaw[0].RecLen);
        printf ("Current record number:%ld\n",      dbHeaw[0].CRec);
        printf ("Data start offset:%ld\n",          dbHeaw[0].COfs);
        printf ("-----------------------------------------------------------\n");
        printf ("Recording start time: %06ld\n",    dbCurDate('T'));
        
        for(a = 0, md = 0; a < 10000; a++){
            dbRecAdd(0);
            for(b = 0; b < DBMAXFLD; b++){ 
                dbFieldPutVal(0, b, 0, (_f)a, md);
            }
            dbRecWrite(0);
        }
        
        printf ("Record end time: %06ld\n",         dbCurDate('T'));
        printf ("-----------------------------------------------------------\n");
        printf ("Number of records in the database:%ld\n", dbHeaw[0].RecCount);
        printf ("Current record number:%ld\n",      dbHeaw[0].CRec);
        printf ("-----------------------------------------------------------\n");
        
        dbClose(0);
    }
    //--------------------------------------------------------------------------
    exit(1);
}
//------------------------------------------------------------------------------
