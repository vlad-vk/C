//------------------------------------------------------------------------------
#include "include.h"
#include "vkdbf_.h"
#include "vkdbf.h"

//------------------------------------------------------------------------------
_i main(vd){
    _i a=0, Sel1=0;

    dbClearALL();

    if((Sel1 = dbSelectAvail()) >= 0){ 
        dbInit(Sel1);
    }

    if (dbOpen(Sel1, "testb2.dbf") > 0){

        for(a = 0; a < dbHeaw[Sel1].RecCount; a++){

            if (a == 0 || a == 1 || a == 2 || a == 3 || a == 4 || a == 5){
                if (isRecDelete(Sel1, a) ==1){
                    dbRecDelete(Sel1, a, 0);
                } else {
                    dbRecDelete(Sel1, a, 1);
                }
            }

            if(isRecDelete(Sel1, a) == 1){
                printf("Record N%d deleted\n", a);
            }
        }
    }
    dbClose(Sel1);

    a = dbPack("testb2.dbf", "testb2.dbr");

    return(0);
}
//------------------------------------------------------------------------------
