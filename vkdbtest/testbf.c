//------------------------------------------------------------------------------
#include "include.h"
#include "vkdbf_.h"
#include "vkdbf.h"

// #define DBMAXFLD  432 // for test

//------------------------------------------------------------------------------
_i main(vd){
    _i a=0, b=0, x=0, z=0, ce=0, md=DBWBUFF;
    _i Sel1=0, Sel2=0,  nBuf1=0, nBuf2=0;
    _c c[11];
    
    dbClearALL();
    dbBFClearALL();
    
    if((Sel1 = dbSelectAvail()) >= 0){
        dbInit(Sel1); 
    } else {
        printf("Error initialize select DB N1\n");
        exit(0);
    }

    if((Sel2 = dbSelectAvail()) >= 0){
        dbInit(Sel2);
    } else {
        printf("Error initialize select DB N2\n");
        exit(1);
    }

    if((nBuf1 = dbBFBufferAvail()) >= 0){
        dbBFInit(nBuf1);
    } else { 
        printf("Error initialize buffer DB N1\n");
        exit(2);
    }
    
    if((nBuf2 = dbBFBufferAvail()) >= 0){
        dbBFInit(nBuf2);
    } else { 
        printf("Error initialize buffer DB N2\n");
        exit(3);
    }
    
    printf("Sel1=%d nBuf1=%d Sel2=%d nBuf2=%d\n", Sel1, nBuf1, Sel2, nBuf2);
    printf("Get Memory for VKDBF library: %d byte\n", dbGetMemLib());

    //--------------------------------------------------------------------------
    for(b = 0; b < DBMAXFLD; b++){
        sprintf(c, "FIELD%02d", b);
        dbFieldFill(Sel1, b, c, 'N', 8, 3);
    }
    
    dbFieldFill(Sel1, 2, "FIELD02", 'L',  1, 0);
    dbFieldFill(Sel1, 3, "FIELD03", 'B',  8, 3);
    dbFieldFill(Sel1, 4, "FIELD04", 'I',  4, 0);
    dbFieldFill(Sel1, 5, "FIELD05", 'D',  8, 0);
    dbFieldFill(Sel1, 6, "FIELD06", 'F',  8, 1);
    dbFieldFill(Sel1, 7, "FIELD07", 'C', 10, 0);
    dbFieldClear(Sel1, b);
    dbCreate(Sel1, "testb1.dbf");
    
    if (dbOpen(Sel1, "testb1.dbf") > 0){
        
        printf("-----------------------------------------------------------\n");
        printf("Database file:[%s]\n",          dbFile[Sel1].Name);
        printf("Count records in DB:%ld\n",     dbHeaw[Sel1].RecCount);
        printf("Len one record:%d\n",           dbHeaw[Sel1].RecLen);
        printf("Number current record:%ld\n",   dbHeaw[Sel1].CRec);
        printf("Offset begin data:%ld\n",       dbHeaw[Sel1].COfs);
        printf("Count fields in DB:%d\n",       dbHeaw[Sel1].FieldCount);
        printf("-----------------------------------------------------------\n");
        printf("Time begin write: %06ld\n",     dbCurDate('T'));
        
        x = dbBFRecInBuf(Sel1, nBuf1);
        dbBFGoFirst(nBuf1);
        printf("RecInBuf=%d\n", x);
    
        for(a = 0, z = 0, md = DBWBUFF; a < 1000; a++){
            
            for(b = 0; b < DBMAXFLD; b++){ 
            
                if (b > 1 && b < 8){
                    if (b == 2){ dbFieldPutLog(Sel1, b, 0, 'Y',        md); }
                    if (b == 3){ dbFieldPutVal(Sel1, b, 0, (_f)a,      md); }
                    if (b == 4){ dbFieldPutVal(Sel1, b, 0, (_f)200,    md); }
                    if (b == 5){ dbFieldPutDat(Sel1, b, 0, "20030331", md); }
                    if (b == 6){ dbFieldPutVal(Sel1, b, 0, (_f)300,    md); }
                    if (b == 7){ dbFieldPutStr(Sel1, b, 0, "TEST!TEST!", md); }
                } else {
                    dbFieldPutVal(Sel1, b, 0, (_f)500, md);
                }
            }

            if (z == x){
                dbBFWrite(Sel1, nBuf1, x, DBBFADD);
                dbBFGoFirst(nBuf1);
                z = 0;
            };

            dbBFRecCopyBuf(Sel1, nBuf1);
            dbBFGoSkip(nBuf1, 1);
            z++;
        }
        
        dbGoLast (Sel1);
        dbBFWrite(Sel1, nBuf1, z, DBBFADD);
        z = 0;
        
        printf("Time end write: %06ld\n",       dbCurDate('T'));
        printf("-----------------------------------------------------------\n");
        printf("Count records in DB:%ld\n",     dbHeaw[Sel1].RecCount);
        printf("Number current record:%ld\n",   dbHeaw[Sel1].CRec);
        printf("-----------------------------------------------------------\n");
    }
    
    //--------------------------------------------------------------------------
    dbGoRec(Sel1, 1);
    dbRecRead(Sel1);
    
    for(b = 0; b < DBMAXFLD; b++){ 
        if (b > 1 && b < 8){
            if (b == 2){ dbFieldPutLog(Sel1, b, 0, 'N',        md); }
            if (b == 3){ dbFieldPutVal(Sel1, b, 0, (_f)111,    md); }
            if (b == 4){ dbFieldPutVal(Sel1, b, 0, (_f)222,    md); }
            if (b == 5){ dbFieldPutDat(Sel1, b, 0, "20030402", md); }
            if (b == 6){ dbFieldPutVal(Sel1, b, 0, (_f)333,    md); }
            if (b == 7){ dbFieldPutStr(Sel1, b, 0, "TEST!TEST!", md); }
       } else {
            dbFieldPutVal(Sel1, b, 0, (_f)555, md);
       }
    }
    dbRecWrite(Sel1);
    
    //--------------------------------------------------------------------------
    dbGoRec(Sel1, 3);
    dbRecRead(Sel1);

    for(b = 0; b < DBMAXFLD; b++){ 
        if (b > 1 && b < 8){
            if (b == 2){ dbFieldPutLog(Sel1, b, 0, 'N',        md); }
            if (b == 3){ dbFieldPutVal(Sel1, b, 0, (_f)111,    md); }
            if (b == 4){ dbFieldPutVal(Sel1, b, 0, (_f)222,    md); }
            if (b == 5){ dbFieldPutDat(Sel1, b, 0, "20030402", md); }
            if (b == 6){ dbFieldPutVal(Sel1, b, 0, (_f)333,    md); }
            if (b == 7){ dbFieldPutStr(Sel1, b, 0, "XXXXXX",   md); }
        } else {
            dbFieldPutVal(Sel1, b, 0, (_f)555, md);
        }
    }
    dbRecWrite(Sel1);
    
    //--------------------------------------------------------------------------
    dbGoRec(Sel1,1);
    dbRecRead(Sel1);
    md = DBWBUFF;
    
    //--------------------------------------------------------------------------
    printf("%s;\n F0=%.0f; F1=%.0f; F2=%c; F3=%.0f; F4=%.0f; F5=%s; F6=%.0f; F7=%s; F8=%.0f.\n",
            dbFieldGetName(Sel1, 0),
            dbFieldGetVal (Sel1, 0, 0, md), dbFieldGetVal(Sel1, 1, 0, md), dbFieldGetLog(Sel1, 2, 0, md),
            dbFieldGetVal (Sel1, 3, 0, md), dbFieldGetVal(Sel1, 4, 0, md), dbFieldGetDat(Sel1, 5, 0, md),
            dbFieldGetVal (Sel1, 6, 0, md), dbFieldGetStr(Sel1, 7, 0, md), dbFieldGetVal(Sel1, 8, 0, md));
    printf("F5[%s] F7[%s]\n", dbFieldGetDat(Sel1, 5, 0, md), dbFieldGetStr(Sel1, 7, 0, md));
   
   //--------------------------------------------------------------------------
    for(b = 0; b < DBMAXFLD; b++){
        sprintf(c, "FIELD%02d", b);
        dbFieldFill(Sel2, b, c, 'N', 8, 3);
    }

    dbFieldFill(Sel2, 2, "FIELD02", 'L',  1, 0);
    dbFieldFill(Sel2, 3, "FIELD03", 'B',  8, 3);
    dbFieldFill(Sel2, 4, "FIELD04", 'I',  4, 0);
    dbFieldFill(Sel2, 5, "FIELD05", 'D',  8, 0);
    dbFieldFill(Sel2, 6, "FIELD06", 'F',  8, 1);
    dbFieldFill(Sel2, 7, "FIELD07", 'C', 10, 0);
    dbFieldClear(Sel2, b);

    a = dbCreate(Sel2, "testb2.dbf");
    
    if (dbOpen(Sel2, "testb2.dbf") > 0){
        dbGoFirst(Sel1); dbGoRec(Sel2, 0);
        
        if((a = dbBFRead(Sel1, nBuf2, 2)) > 0){
        
            ce = dbBFRecDelete(nBuf2, 0, 1);
            if (ce < 0){ 
                printf("CError1=%d\n", ce);
            }

            ce = dbBFRecDelete(nBuf2, 1, 1);
            if (ce < 0){
                printf("CError2=%d\n",ce);
            }

            if (isBFRecDelete(nBuf2, 0)){
                printf("Record 0 in Buf2 deleted.\n");
            }
            a = dbBFWrite(Sel2, nBuf2, 3, DBBFWRADD);
        }
    }
    
    //--------------------------------------------------------------------------
    dbClear(3);
    dbInit (3);

    dbFieldFill (3, 0, "FIELD0", 'C', 100, 0);
    dbFieldClear(3, 1);
    
    dbCreate(3, "testb3.dbf");
    dbOpen  (3, "testb3.dbf");
    dbRecAdd(3);
    
    //--------------------------------------------------------------------------
    dbClose (3); 
    dbClose (Sel1); 
    dbClose (Sel2);
    exit(1);
}
//------------------------------------------------------------------------------
