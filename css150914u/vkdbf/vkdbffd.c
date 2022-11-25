// coding: cp866
//-----------------------------------------------------------------------------
// В этом файле находятся функции для работы с полями БД:
//-----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  проверить заполнено ли указанное поле в структуре БД
_i  dbFieldTest(_i Sel, _i Fld){
    if(dbField[Sel][Fld].Name[0]==0){ return(-1); }
    if(dbField[Sel][Fld].Tip!='C'&&dbField[Sel][Fld].Tip!='L'&&
       dbField[Sel][Fld].Tip!='D'&&dbField[Sel][Fld].Tip!='M'&&
       dbField[Sel][Fld].Tip!='N'&&dbField[Sel][Fld].Tip!='F'&&
       dbField[Sel][Fld].Tip!='B'&&dbField[Sel][Fld].Tip!='I'){ 
       return(-2);
    }
    return(1);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  очистить указанное поле в структуре БД
vd  dbFieldClear(_i Sel, _i Fld){
    dbField[Sel][Fld].Name[0]=0; dbField[Sel][Fld].Tip=0; 
    dbField[Sel][Fld].Len=0;     dbField[Sel][Fld].Dec=0; 
    dbHExt[Sel].Len=0;
    return;
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  заполнение записи для создания БД
_i  dbFieldFill(_i Sel, _i Fld, uc *FName, uc Tip, uc Len, uc Dec){
    if(Tip!='C'&&Tip!='L'&&Tip!='D'&&Tip!='M'&&Tip!='N'&&
       Tip!='F'&&Tip!='B'&&Tip!='I'){ return(-1); }
    if(Len==0){ return(-2); }
    if(Tip=='B'&&Len!=LENDOUBLE){ Len=LENDOUBLE; }
    if(Tip=='I'&&Len!=LENINTEGR){ Len=LENINTEGR; }
    if(Tip=='D'&&Len!=LENDATE  ){ Len=LENDATE;   }
    if(Tip=='L'&&Len!=LENLOGIC ){ Len=LENLOGIC;  }
    strcpy(dbField[Sel][Fld].Name,FName); dbField[Sel][Fld].Name[10]=0;
    dbField[Sel][Fld].Tip=Tip;
    dbField[Sel][Fld].Len=Len;
    dbField[Sel][Fld].Dec=Dec;
    return(1);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  веpнуть имя поля по его номеру
uc *dbFieldGetName(_i Sel,_i Fld){
    if((dbFieldTest(Sel,Fld))<1){ return(""); }
    return(dbField[Sel][Fld].Name);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  веpнуть тип поля по его номеру
uc  dbFieldGetType(_i Sel,_i Fld){
    if((dbFieldTest(Sel,Fld))<1){ return(0); }
    return(dbField[Sel][Fld].Tip);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  веpнуть размер поля по его номеру
uc  dbFieldGetLen(_i Sel,_i Fld){
    if((dbFieldTest(Sel,Fld))<1){ return(0); }
    return(dbField[Sel][Fld].Len);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  веpнуть количесто символов после запятой по его номеру
uc  dbFieldGetDec(_i Sel,_i Fld){
    if((dbFieldTest(Sel,Fld))<1){ return(0); }
    return(dbField[Sel][Fld].Dec);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  возвpащает номеp поля по его имени, если поля нет, то возращает -1
//  Reg=0 -без учета регистра, иначе с учетом регистра
_i  dbFieldGetNum(_i Sel, _i Reg, uc *FldName){
    _i  a=0, r=-1;
    for(a=0;a<DBMAXFLD;a++){
        if(Reg==0){
           if((strcasecmp(FldName,dbField[Sel][a].Name))==0){ r=a; break; }
        } else {
           if(( strcmp(FldName,dbField[Sel][a].Name))==0){ r=a; break; }
        }
    }
    return(r);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  возвращает количество полей БД из указанной области
_i  dbFieldCount(_i Sel){
    _i r=-1;
    if(dbOpenTest(Sel,0)>=0){
       r=dbHeaw[Sel].FieldCount; 
    }
    return(r);
}
//----------------------------------------------------------------------------
