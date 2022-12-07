// coding: cp866
//-----------------------------------------------------------------------------
// В этом файле находятся функции работы с базой данных:
//-----------------------------------------------------------------------------
//#include <allegro.h>
#include "../define.h"
#include "../include.h"
#include "vkdbfdb.h"
#include "vkdbffd.c"
#include "vkdbfot.c"
#include "vkdbfrc.c"
#include "vkdbfvl.c"
#include "vkdbfbf.c"

//----------------------------------------------------------------------------
//  очистка заданного массива
vd  dbClear(_i Sel){
    dbFile[Sel].Name[0]=0; dbFile[Sel].Name[1]=0; 
    dbFile[Sel].Desk=-1;   dbFile[Sel].Init=-1; 
    return;
}
//----------------------------------------------------------------------------
//  очистить все массивы
vd  dbClearALL(vd){
    _i  a=0;
    for(a=0;a<DBMAXDBF;a++){ dbClear(a); }
    return;
}
//----------------------------------------------------------------------------
//  начальная инициализация массивов
_i  dbInit(_i Sel){
    if(Sel<0||Sel>=DBMAXDBF){ 
       DPrn(13,1,"dbInit: указанная область %d вне границ.\n",Sel); return(-1); 
    }
    if(dbFile[Sel].Init>=0 ){ 
       DPrn(13,1,"dbInit: указанная область %d уже инициализирована.\n",Sel); return(-2); 
    }
    dbClear(Sel); dbFile[Sel].Init=Sel;
    return (Sel);
}
//----------------------------------------------------------------------------
//  определение свободной области для работы с БД
_i  dbSelectAvail(vd){
    _i  a=0;
    for(a=0;a<DBMAXDBF;a++){
        if(dbFile[a].Name[0]==0 && dbFile[a].Name[1]==0 &&
           dbFile[a].Desk<0     && dbFile[a].Init<0){
           return(a);
        }
    };  
    DPrn(13,1,"dbSelectAvail: нет свободной области.\n"); return(-3);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  проверить открыта ли БД в указанной области
_i  dbOpenTest(_i Sel,uc *FName){
    _i  a=0;
    if (Sel<0){
        for(a=0;a<DBMAXDBF;a++){
            if(strcasecmp(dbFile[a].Name,FName)==0){ return(a); }
        }
        DPrn(13,1,"dbOpenTest: указаннный файл БД не открыт.\n"); 
        return(-4);
    }
    if (Sel>=DBMAXDBF){ 
        DPrn(13,1,"dbOpenTest: указанная область %d вне границ.\n",Sel); 
        return(-5); 
    }
    if (dbFile[Sel].Name[0]==0){
        DPrn(13,1,"dbOpenTest: не определено имя БД для области %d.\n",Sel); 
        return(-6); 
    }
    if (dbFile[Sel].Desk<0){ 
        DPrn(13,1,"dbOpenTest: не определен дескриптор БД для области %d.\n",Sel); 
        return(-7); 
    }
    if (dbFile[Sel].Init<0){
        DPrn(13,1,"dbOpenTest: массив для указанной области %d не инициализирован.\n",Sel);
        return(-8); 
    }
    return(Sel);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  проверка целостности БД и заголовка при необходимости ее корректировка
_i  dbCheck(_i Sel, uc *dbName){
    _i  FDesk=0;                                 // дескриптор файла
    _i  h=0,r=0;
    _l  hbr=0, szf=0, szr=0, szt=0, hrc=0;
    //  проверить существует ли файл БД
    if (access(dbName, 0)<0){ 
        DPrn(13,1,"dbCheck: нет доступа к файлу БД [%s] в области %d.\n",dbName,Sel); 
        return(-9); 
    }
    //  если БД в указанной области уже открыта
    if (dbOpenTest(Sel,0)>=0){ 
        DPrn(13,1,"dbCheck: в указанной области %d БД уже открыта.\n",Sel); 
        return(-10); 
    }
    //  открыть файл
    if((FDesk=open(dbName,O_BINARY|O_RDWR))<0){ 
        DPrn(13,1,"dbCheck: ошибка открытия файла [%s].\n",dbName); 
        return(-11); 
    }
    //  обнулить массив для чтения
    memset(BRead,0,DBBSIZ);
    //  прочитать заголовок открываемого файла
    hbr=read(FDesk,&BRead,DBHDLEN+DBFLDLN*DBMAXFLD+DBBEXT);
    //  если количество считанных байт < 64 (32=заголовок + 32=описание поля)
    if (hbr<DBHDLEN*2){ 
        DPrn(13,1,"dbCheck: ошибка заголовка файла [%s] в области %d.\n",dbName,Sel);
        close(FDesk); return(-12); 
    }
    //  из считанных данных заполнить структуру описания заголовка
    memcpy(&dbHead[Sel],&BRead,DBHDLEN);
    //  размер полного заголовка БД
    memcpy(&h,&dbHead[Sel].HeadSize,2);
    //  проверить правильность длинны заголовка
    if((h<DBHDLEN)||(h>DBHDLEN+DBMAXFLD*DBFLDLN+DBBEXT)){
        DPrn(13,1,"dbCheck: ошибка заголовка файла [%s] в области %d.\n",dbName,Sel);
        close(FDesk); return(-13);
    }
    //  размер одной записи
    memcpy(&dbHeaw[Sel].RecLen,&dbHead[Sel].RecLen,2);
    //  проверка корректности длинны одной записи
    if((dbHeaw[Sel].RecLen<1)||(dbHeaw[Sel].RecLen>(DBMAXFLD*DBSLEN))){ 
        DPrn(13,1,"dbCheck: ошибка заголовка файла [%s] в области %d.\n",dbName,Sel);
        close(FDesk); return(-14); 
    }
    //  количество записей в БД
    memcpy(&dbHeaw[Sel].RecCount,&dbHead[Sel].RecCount,4);
    //  получить размер всего файла
    lseek(FDesk,0,SEEK_END);  szf=lseek(FDesk,0,SEEK_CUR);
    //  получить размер всех записей
    szr=szf-h;
    //  получить предполагаемое количество записей
    hrc=divi(szr, dbHeaw[Sel].RecLen);
    //  обратная проверка размера всех записей БД
    szt=hrc*dbHeaw[Sel].RecLen;
    //  если размеры не совпадают- дописать файл нулями до нужного размера
    if (szt!=szr||(szr==0&&dbHeaw[Sel].RecCount>0)){
        if(szr==0){ dbHeaw[Sel].RecCount=0; }
        else      { dbHeaw[Sel].RecCount=hrc+1; }
        szt=dbHeaw[Sel].RecCount*dbHeaw[Sel].RecLen; h=szt-szr;
        memset(BRead,0,h);
        write (FDesk,BRead,h);
        //  исправить заголовок БД
        memcpy(&dbHead[Sel].RecCount,&dbHeaw[Sel].RecCount,4);
        lseek ( FDesk,0,SEEK_SET); 
        write ( FDesk,&dbHead[Sel],DBHDLEN);
    }
    //  закрыть файл БД
    close(FDesk);
    return (r);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  создать файл БД
_i  dbCreate(_i Sel, uc *dbName){
    _i  FDesk=0;                       // дескриптор файла
    _i  f=0;                           // номер поля
    _i  l[3];                          // длинна одной записи
    _l  cps=0,h=0;                     // смещение в буфере (длинна заголовка)
    //  проверить существует ли файл БД
    if (access(dbName,0)==0){ 
        DPrn(13,1,"dbCreate: ошибка доступа к файлу [%s] в области %d.\n",dbName,Sel);
        return(-21); 
    }
    //  заполнить структуру заголовка
    dbHead [Sel].ID=3;                      // Идентификатор
    dbModifyDate(Sel);                      // Дата модификации
    dbHeaw [Sel].RecCount=0;                // Количество записей в БД
    memcpy(&BRec[Sel],&dbHead[Sel],DBHDLEN);
    cps=cps+DBHDLEN;
    l[0]=1;                                 // l[0]=1 -признак удаления записи
    for(f=0;f<DBMAXFLD;f++){
        if((dbFieldTest(Sel,f))<0){ break; }// если поле в структур не заполн
        memcpy(&dbField[Sel][f].OfsFld,&l[0],2);
        memcpy(&BRec[Sel][cps],&dbField[Sel][f],DBHDLEN); 
        cps=cps+DBHDLEN;
        l[0]=l[0]+(_i)dbField[Sel][f].Len;  // считать общую длинну записи
    }                                       
    // если не было записано ни одного поля
    if (f<1){ 
        DPrn(13,1,"dbCreate: ошибка записи полей в файле [%s] области %d.\n",dbName,Sel);
        dbFieldClear(Sel,0); return(-22); 
    }
    // записать признак конца заголовка
    BRec[Sel][cps]=13;  cps=cps+1;
    // записать расширенный заголовок
    if(dbHExt[Sel].Len>0){                
       memcpy(&BRec[Sel][cps],&dbHExt[Sel].Buf,dbHExt[Sel].Len); 
       cps=cps+dbHExt[Sel].Len;
    }; h=cps;
    // дозаполнение структуры заголовка
    memcpy(&dbHead[Sel].RecCount,&dbHeaw[Sel].RecCount,4);
    memcpy(&dbHead[Sel].HeadSize,&h,2);     // полная длинна заголовка
    memcpy(&dbHead[Sel].RecLen,  &l,2);     // длинна одной записи
    memcpy(&dbHead[Sel].HFld,    &f,2);     // колич полей в базе данных
    dbHead[Sel].LibFlg[0]='V';              // признак создания БД этой библиот
    dbHead[Sel].LibFlg[1]='K';              //
    memcpy(&BRec[Sel][0],&dbHead[Sel],DBHDLEN); 
    //  создать файл БД
    if((FDesk=open(dbName,O_BINARY|O_RDWR|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE))<0){
        DPrn(13,1,"dbCreate: ошибка открытия файла [%s] в области %d.\n",dbName,Sel);
        return(-23);
    }
    // записать заголовк файла БД
    write(FDesk,&BRec[Sel],cps);            // записать весь заголовок
    // закрыть файл БД
    close(FDesk); 
    return(1);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  откpыть DBF-файл
_i  dbOpen(_i Sel, uc *dbName){
    _i  FDesk=0, f=0;
    _l  ofs=0, hbr=0, cps=0, h=0;
    //  проверить доступ к файлу БД
    if (access(dbName,6)!=0){ 
        DPrn(13,1,"dbOpen: в области %d нет доступа к файлу [%s]\n",Sel,dbName); return(-31); 
    }
    //  если БД в указанной области уже открыта
    if (dbOpenTest(Sel,0)>=0){ 
        DPrn(13,1,"dbOpen: в области %d БД уже открыта.\n"); return(-32); 
    }
    //  открыть файл
    if((FDesk=open(dbName,O_BINARY|O_RDWR))<0){ 
        DPrn(13,1,"dbOpen: ошибка открытия файла БД [%s] в области %d\n",dbName,Sel); return(-33); 
    }
    memset(BRead,0,DBBSIZ);
    //  прочитать заголовок открываемого файла
    hbr=read(FDesk,&BRead,DBHDLEN+DBHDLEN*DBMAXFLD+DBBEXT);
    if (hbr<DBHDLEN*2){ 
        close(FDesk); 
        DPrn(13,1,"dbOpen: неправильный размер заголовка БД в области %d.\n",Sel); return(-34); 
    }
    memcpy(&dbHead[Sel],&BRead,DBHDLEN); cps=cps+DBHDLEN;
    //  размер заголовка БД
    memcpy(&h,&dbHead[Sel].HeadSize,2);
    //  размер одной записи
    memcpy(&dbHeaw[Sel].RecLen,&dbHead[Sel].RecLen,2);
    //  проверка корректности считанных данных
    if((h<DBHDLEN)||(dbHeaw[Sel].RecLen<1)){ 
        close(FDesk); 
        DPrn(13,1,"dbOpen: неверный заголовок БД в области %d.\n",Sel); return(-35);
    }
    //  начальное значение для номера текущей записи не определено
    dbHeaw[Sel].CRec=-1;
    //  количество записей в БД
    memcpy(&dbHeaw[Sel].RecCount,&dbHead[Sel].RecCount,4);
    //  начальное смещение для первой записи
    dbHeaw[Sel].COfs=h;
    //  заполнить структуру полей БД
    for(f=0;f<DBMAXFLD;f++){
        memcpy(&dbField[Sel][f],&BRead[cps],DBHDLEN); 
        h=h-DBHDLEN; cps=cps+DBHDLEN;
        if((dbField[Sel][f].Tip!='C'&&dbField[Sel][f].Tip!='L'&&
            dbField[Sel][f].Tip!='D'&&dbField[Sel][f].Tip!='M'&&
            dbField[Sel][f].Tip!='N'&&dbField[Sel][f].Tip!='F'&&
            dbField[Sel][f].Tip!='B'&&dbField[Sel][f].Tip!='I')||(h<=0)){ 
            break; 
        };
        //  занести в структуру смещение текущего поля в записи
        memcpy(&dbField[Sel][f].OfsFld,&ofs,4); 
        dbFielw[Sel][f].OfsFld=ofs;              // запом текущ смещение
        ofs=ofs+dbField[Sel][f].Len; 
    }
    dbHExt[Sel].Len=dbHeaw[Sel].COfs-DBHDLEN-(DBHDLEN*f);
    dbHeaw[Sel].FieldCount=f;
    //  если не считанно ниодного поля - это файл не базы данных
    if (f<1){ 
        DPrn(13,1,"dbOpen: нет считанных полей БД в области %d.\n",Sel);
        close(FDesk); return(-36); 
    }
    //  чтение расширенного заголовка
    if (dbHExt[Sel].Len>0){ 
        memcpy(&dbHExt[Sel].Buf,&BRead[cps],dbHExt[Sel].Len);
    }
    //  записать в структуру количество полей в БД
    memcpy(&dbHead[Sel].HFld,&f,2);
    //  установить указатель БД на запись N0
    if (dbHeaw[Sel].RecCount>0){ dbHeaw[Sel].CRec=0; }
    //  передать имя файла и дескриптор в массивы
    strcpy(dbFile[Sel].Name,dbName); dbFile[Sel].Desk=FDesk; dbFile[Sel].Mod=0;
    //
    DPrn(13,1,"dbOpen: файл БД [%s] открыт в области %d.\n",dbFile[Sel].Name,Sel);
    return(1);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  закрытие БД
vd  dbClose(_i Sel){
    _i  a=0;
    _l  cps=0;
    uc  he=13;
    DPrn(13,1,"dbClose: закрытие файла БД [%s] в области %d:\n",dbFile[Sel].Name,Sel);
    //  правильный номер БД
    if (dbOpenTest(Sel,0)<0){ return; }
    //  если в файл не было записи данных- закрыть его без модификац заголовка
    if (dbFile[Sel].Mod== 0){
        DPrn(13,1,"dbClose: файл БД [%s] закрыт в области %d.\n",dbFile[Sel].Name,Sel);
        close(dbFile[Sel].Desk); dbClear(Sel); return;
    }
    //  заполнить заголовок базы данных
    memcpy(&BRec[Sel],&dbHead[Sel],DBHDLEN); cps=cps+DBHDLEN;
    //  заполнить поля базы данных
    for(a=0;a<dbHeaw[Sel].FieldCount;a++){
        memcpy(&BRec[Sel][cps],&dbField[Sel][a],DBHDLEN); cps=cps+DBHDLEN;
    }
    // установить признак окончания заголовка
    memcpy(&BRec[Sel][cps],&he,1); cps=cps+1;
    //  заполнить расширенный заголовк
    if (dbHExt[Sel].Len>0){ 
        memcpy(&BRec[Sel][cps],&dbHExt[Sel].Buf,dbHExt[Sel].Len); 
        cps=cps+dbHExt[Sel].Len;
    }
    // указатель на начало файла, записать заголовок и закрыть файл
    lseek(dbFile[Sel].Desk,0,SEEK_SET);
    write(dbFile[Sel].Desk,BRec[Sel],cps);
    close(dbFile[Sel].Desk);
    DPrn(13,1,"dbClose: файл БД [%s] закрыт в области %d.\n",dbFile[Sel].Name,Sel);
    dbClear(Sel);
    return;
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  копировать структуру базы данных в новый файл
_i  dbStructCopy(uc *dbSrc, uc *dbDst){
    _i  Sel=0, FDesk=0, hb=0, r=0;
    DPrn(13,1,"dbStructCopy: начало:\n");
    //  создать файл для записи структуры новой базы данных
    if((FDesk=open(dbDst,O_BINARY|O_RDWR|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE))<0){
        DPrn(13,1,"dbStructCopy: ошибка открытия файла [%s]:\n",dbDst);
        return(-1); 
    }
    //  открыть исходную базу данных
    if((Sel=dbOpenTest(-1,dbSrc))<0){
        Sel=dbSelectAvail(); dbInit(Sel);
        if (dbOpen(Sel,dbSrc)<0){ 
            DPrn(13,1,"dbStructCopy: ошибка открытия БД [%s][%d]:\n",dbSrc,Sel);
            close(FDesk); return(-2); 
        }
    }
    //  установить указатель на начало исходной базы данных
    lseek(dbFile[Sel].Desk,0,SEEK_SET);
    //  считать полный заголовок базы данных в буфер
    hb=read (dbFile[Sel].Desk,BRead,dbHeaw[Sel].COfs);
    //  записать заголовок в новый файл
    if(hb==dbHeaw[Sel].COfs){ 
        BRead[4]=0; BRead[5]=0; BRead[6]=0; BRead[7]=0; write(FDesk,BRead,hb);
    } else { 
        r=-3; 
    }
    //  закрыть файлы
    dbClose(Sel); close(FDesk); DPrn(13,1,"dbStructCopy: конец:\n");
    return (r);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  перезапись файла БД исключая записи, помеченные на удаление
_i  dbPack(uc *dbBase, uc *dbTemp){
    _i  a=0, SBase=0, STemp=0;
    _l hr=0;
    // создать временную базу данных
    if (dbStructCopy(dbBase,dbTemp)<0){ return(-1); }
    DPrn(13,1,"dbPack: начало: *******************\n");
    // определить свободную область для баз данных
    if((SBase=dbSelectAvail())<0){ DPrn(13,1,"ER:-2\n"); return(-2); }
    dbInit(SBase);
    if((STemp=dbSelectAvail())<0){ DPrn(13,1,"ER:-3\n"); return(-3); }
    dbInit(STemp);
    // проверка правильности базы данных
    if (dbCheck(SBase,dbBase) <0){ DPrn(13,1,"ER:-4\n"); return(-4); }
    // открыть базы данных
    if (dbOpen (SBase,dbBase)< 0){ DPrn(13,1,"ER:-5\n"); return(-5); }
    if (dbOpen (STemp,dbTemp)< 0){ dbClose(SBase); DPrn(13,1,"ER:-6\n"); return(-6); }
    // получить количество записей в базе данных
    hr=dbRecCount(SBase)-1;
    // перейти на 1-ю запись базы данных
    dbGoFirst(SBase);
    // для каждой записи базы данных
    for(a=0;a<hr;a++){
        // читать текущую запись базы данных
        dbRecRead(SBase);
        if(isRecDFL(SBase)==0){
           // добавить запись во временную базу данных, если нет признака удал.
           if(dbRecAdd (STemp)>0){
              // записать считанную запись во времееную базу
              dbRecCopy(SBase,STemp); dbRecWrite(STemp);
           }
        }
        // перейти на следующую запись базы данных
        dbGoSkip(SBase,1); 
        // если конец базы- выйти из цикла
        if(dbEof(SBase)>0){ break; }
    }
    // закрыть базы
    dbClose(SBase); 
    dbClose(STemp); 
    DPrn(13,1,"dbPack: конец: ********************\n");
    return(0);
}
//----------------------------------------------------------------------------
