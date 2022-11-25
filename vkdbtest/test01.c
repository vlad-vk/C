//------------------------------------------------------------------------------
#include "include.h"
#include "vkdbf_.h"
#include "vkdbf.h"

//------------------------------------------------------------------------------
_i main(vd){
    _i a=0, b=0, md=1;
    _c c[11];

    dbClearALL();
    
    printf("Get Memory for VKDBF library: %d byte\n", dbGetMemLib());
    
    //--------------------------------------------------------------------------
    for(b = 0; b < DBMAXFLD; b++){
        sprintf(c, "FIELD%02d", b); dbFieldFill(2, b, c,'N', 8, 3);
    }
    
    dbFieldFill(2, 2, "FIELD02", 'L',  1, 0);
    dbFieldFill(2, 3, "FIELD03", 'B',  8, 3);
    dbFieldFill(2, 4, "FIELD04", 'I',  4, 0);
    dbFieldFill(2, 5, "FIELD05", 'D',  8, 0);
    dbFieldFill(2, 6, "FIELD06", 'F',  8, 1);
    dbFieldFill(2, 7, "FIELD07", 'C', 10, 0);
    dbCreate(2, "test01.dbf");
    
    if (dbOpen (2, "test01.dbf") >0){
        
        printf ("-----------------------------------------------------------\n");
        printf ("Database file:[%s]\n",         dbFile[2].Name);
        printf ("Count records in DB:%ld\n",    dbHeaw[2].RecCount);
        printf ("Len one record:%d\n",          dbHeaw[2].RecLen);
        printf ("Number current record:%ld\n",  dbHeaw[2].CRec);
        printf ("Offset begin data:%ld\n",      dbHeaw[2].COfs);
        printf ("Count fields in DB:%d\n",      dbHeaw[2].FieldCount);
        printf ("-----------------------------------------------------------\n");
        printf ("Time begin write: %06ld\n", dbCurDate('T'));
        
        for(a = 0, md = 0; a < 1000; a++){
            dbRecAdd(2);
            for(b = 0; b < DBMAXFLD; b++){ 
                if (b > 1 && b < 8){
                    if (b == 2){ dbFieldPutLog(2, b, 0, 'Y',       md); }
                    if (b == 3){ dbFieldPutVal(2, b, 0, (_f)100,   md); }
                    if (b == 4){ dbFieldPutVal(2, b, 0, (_f)200,   md); }
                    if (b == 5){ dbFieldPutDat(2, b, 0, "20030331",md); }
                    if (b == 6){ dbFieldPutVal(2, b, 0, (_f)300,   md); }
                    if (b == 7){ dbFieldPutStr(2, b, 0, "TEST!TEST!", md); }
                } else {
                    dbFieldPutVal(2, b, 0, (_f)500, md);
                }
            }
            dbRecWrite(2);
        }
        
        printf ("Time end write: %06ld\n",      dbCurDate('T'));
        printf ("-----------------------------------------------------------\n");
        printf ("Count records in DB:%ld\n",    dbHeaw[2].RecCount);
        printf ("Number current record:%ld\n",  dbHeaw[2].CRec);
        printf ("-----------------------------------------------------------\n");
    }
    //--------------------------------------------------------------------------
    dbGoRec(2,1);
    dbRecRead(2);
    
    for(b = 0; b < DBMAXFLD; b++){ 
        if (b > 1 && b < 8){
            if (b == 2){ dbFieldPutLog(2, b, 0, 'N',       md); }
            if (b == 3){ dbFieldPutVal(2, b, 0, (_f)111,   md); }
            if (b == 4){ dbFieldPutVal(2, b, 0, (_f)222,   md); }
            if (b == 5){ dbFieldPutDat(2, b, 0, "20030402",md); }
            if (b == 6){ dbFieldPutVal(2, b, 0, (_f)333,   md); }
            if (b == 7){ dbFieldPutStr(2, b, 0, "TEST!TEST!", md); }
        } else {
            dbFieldPutVal(2, b, 0, (_f)555, md);
        }
    }
    dbRecWrite(2);
    
    //--------------------------------------------------------------------------
    dbGoRec(2,3);
    dbRecRead(2);
    
    for(b = 0; b < DBMAXFLD; b++){ 
        if (b > 1 && b < 8){
            if (b == 2){ dbFieldPutLog(2, b, 0, 'N',       md); }
            if (b == 3){ dbFieldPutVal(2, b, 0, (_f)111,   md); }
            if (b == 4){ dbFieldPutVal(2, b, 0, (_f)222,   md); }
            if (b == 5){ dbFieldPutDat(2, b, 0, "20030402",md); }
            if (b == 6){ dbFieldPutVal(2, b, 0, (_f)333,   md); }
            if (b == 7){ dbFieldPutStr(2, b, 0, "XXXXXX",  md); }
        } else {
            dbFieldPutVal(2, b, 0, (_f)555, md);
        }
    }
    dbRecWrite(2);
    
    //--------------------------------------------------------------------------
    dbGoRec(2,1);
    dbRecRead(2);
    md=0;
    
    //--------------------------------------------------------------------------
    printf("%s;\n F0=%.0f; F1=%.0f; F2=%c; F3=%.0f; F4=%.0f; F5=%s; F6=%.0f; F7=%s; F8=%.0f.\n",
            dbFieldGetName(2, 0),
            dbFieldGetVal (2, 0, 0, md), dbFieldGetVal(2, 1, 0, md), dbFieldGetLog(2, 2, 0, md),
            dbFieldGetVal (2, 3, 0, md), dbFieldGetVal(2, 4, 0, md), dbFieldGetDat(2, 5, 0, md),
            dbFieldGetVal (2, 6, 0, md), dbFieldGetStr(2, 7, 0, md), dbFieldGetVal(2, 8, 0, md));
    printf("F5[%s] F7[%s]\n",dbFieldGetDat(2, 5, 0, md), dbFieldGetStr(2, 7, 0, md));

    //--------------------------------------------------------------------------
    dbClose(2);
    exit(1);
}
//------------------------------------------------------------------------------
