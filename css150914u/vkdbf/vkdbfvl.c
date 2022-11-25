// coding: cp866
//-----------------------------------------------------------------------------
// В этом файле находятся функции для записи значений в поля БД и
// получении значений из полей БД
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  записать стpоку в поле базы данных
_l  dbFieldPutStr(_i Sel, _i Fld, uc *FNm, uc *cStr, _i md){
    _i  a= strlen(cStr);
    _i  b= 0, c=0;
    _l  r=-1;
    _i  FOfs=0;
    uc  s[DBSLEN];
    if (Fld<0){ Fld=dbFieldGetNum(Sel,0,FNm); }
    if (dbFieldTest(Sel,Fld)<0||dbField[Sel][Fld].Tip!='C'){ return(-1); }
    // установить указатель в БД
    if((r=dbFieldOffset(Sel,Fld,md))>0){
        FOfs=(_i)r;
        if(a>dbField[Sel][Fld].Len){ 
           a=dbField[Sel][Fld].Len; 
        }
           b=dbField[Sel][Fld].Len-a;
        if(md==DBWBUFF){
           memcpy(&BRec[Sel][FOfs],cStr,a);  
           for(c=0;c<b;c++){ BRec[Sel][FOfs+a+c]=32; }
        } else {
           memcpy(&s,cStr,a);
           for(c=0;c<b;c++){ s[a+c]=32; }
           r=write(dbFile[Sel].Desk,s,a+c); dbFile[Sel].Mod=1;
        }
    } 
    return(r);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  записать дату в поле базы данных
_l  dbFieldPutDat(_i Sel, _i Fld, uc *FNm, uc *cStr, _i md){
    _l  r=-1;
    _i  FOfs=0;
    _c  s[8];
    if (Fld<0){ Fld=dbFieldGetNum(Sel,0,FNm); }
    if (dbField[Sel][Fld].Tip!='D'){ return(-1); }
    if (cStr[0]=='D'){ 
        dbCurDate('D');
        sprintf(s,"%04d%02d%02d",
                dbtm->tm_year+1900,dbtm->tm_mon+1,dbtm->tm_mday);
    } else {
        strcpy(s,cStr);
    }    
    if((r=dbFieldOffset(Sel,Fld,md))>0){ 
        FOfs=(_i)r;
        if(md==DBWBUFF){
           memcpy(&BRec[Sel][FOfs],&s,(_i)LENDATE);
        } else {
           r=write(dbFile[Sel].Desk,s,(_i)LENDATE); 
           dbFile[Sel].Mod=1;
        }
    }
    return(r);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  записать логическое значение в поле базы данных
_l  dbFieldPutLog(_i Sel, _i Fld, uc *FNm, uc Log, _i md){
    _l  r=-1;
    _i  FOfs=0;
    if (Fld<0){ Fld=dbFieldGetNum(Sel,0,FNm); }
    if (dbField[Sel][Fld].Tip!='L'){ return(-1); }
    if((r=dbFieldOffset(Sel,Fld,md))>0){
        FOfs=(_i)r;
        if(md==DBWBUFF){
           memcpy(&BRec[Sel][FOfs],&Log,1);
        } else {
           r=write(dbFile[Sel].Desk,&Log,1);
           dbFile[Sel].Mod=1;
        }
    }
    return(r);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  записать числовое значение в поле базы данных
_l  dbFieldPutVal(_i Sel, _i Fld, uc *FNm, _d Val, _i md){
    uc  c[10];
    _i  l= 0;
    _i  d= 0;
    _i  i= 0;
    _i  FOfs=0;
    _l  r=-1;
    if (Fld<0){ Fld=dbFieldGetNum(Sel,0,FNm); }
    if((r=dbFieldOffset(Sel,Fld,md))>0){
        FOfs=(_i)r;
        // записать число в строковом виде
        if(dbField[Sel][Fld].Tip=='N'||dbField[Sel][Fld].Tip=='F'){
           l=dbField[Sel][Fld].Len;
           d=dbField[Sel][Fld].Dec;
           sprintf(c,"%c%02d.%01df",'%',l,d);
           sprintf(BSFld[4],c,Val); 
           if(md==DBWBUFF){
              memcpy(&BRec[Sel][FOfs],&BSFld[4],l);
           } else {
              r=write(dbFile[Sel].Desk,&BSFld[4],l);
              dbFile[Sel].Mod=1;
           }
           return(r);
        }
        // записать число в бинарном виде типа double
        if(dbField[Sel][Fld].Tip=='B'){
           if(md==DBWBUFF){
              memcpy(&BRec[Sel][FOfs],&Val,LENDOUBLE);
           } else {
              r=write(dbFile[Sel].Desk,&Val,LENDOUBLE);
              dbFile[Sel].Mod=1;
           }
           return(r);
        }
        // записать число в бинарном виде типа integer
        if(dbField[Sel][Fld].Tip=='I'){
           memset(&i,0,LENINTEGR); i=(_i)Val;
           if(md==DBWBUFF){
              memcpy(&BRec[Sel][FOfs],&i,LENINTEGR);
           } else {
              r=write(dbFile[Sel].Desk,&i,LENINTEGR);
              dbFile[Sel].Mod=1;
           }
        }
    } 
    return(r);
}
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
//  получить стpоку из поля базы данных
uc *dbFieldGetStr(_i Sel, _i Fld, uc *FNm, _i md){
    _i  FOfs=0;
    uc  s[DBSLEN];
    if (Fld<0){ Fld=dbFieldGetNum(Sel,0,FNm); }
    if (dbField[Sel][Fld].Tip!='C'){ return(""); }
    if((FOfs=(_i)dbFieldOffset(Sel,Fld,md))>0){
        if(md==DBWBUFF){
            memcpy(&s,&BRec[Sel][FOfs],dbField[Sel][Fld].Len);
        } else {
            read(dbFile[Sel].Desk,&s,dbField[Sel][Fld].Len);
        };  s[dbField[Sel][Fld].Len]=0;  strcpy(BSFld[0],s);
    }
    if(strlen(s)>0){ return(BSFld[0]); }; return("");
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  получить стpоку даты из поля базы данных
uc *dbFieldGetDat(_i Sel, _i Fld, uc *FNm, _i md){
    _i  FOfs=0;    
    uc  s[10]={0,0,0,0,0,0,0,0,0,0};    
    if (Fld<0){ Fld=dbFieldGetNum(Sel,0,FNm); }
    if (dbField[Sel][Fld].Tip!='D'){ return(""); }
    if((FOfs=(_i)dbFieldOffset(Sel,Fld,md))>0){ 
        if(md==DBWBUFF){
            memcpy(&s,&BRec[Sel][FOfs],LENDATE);
        } else {
            read(dbFile[Sel].Desk,&s,LENDATE); 
        };  s[LENDATE]=0; strcpy(BSFld[1],s);
    }
    if(strlen(s)>0){ return(BSFld[1]); }; return("");
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  получить логическое значение из поля базы данных
uc  dbFieldGetLog(_i Sel, _i Fld, uc *FNm, _i md){
    _i  FOfs=0;    
    uc  c=0;
    if (Fld<0){ Fld=dbFieldGetNum(Sel,0,FNm); }
    if (dbField[Sel][Fld].Tip!='L'){ return(0); }
    if((FOfs=(_i)dbFieldOffset(Sel,Fld,md))>0){ 
        if(md==DBWBUFF){
           memcpy(&c,&BRec[Sel][FOfs],1);
        } else {
           read(dbFile[Sel].Desk,&c,1);
        }
    } 
    return(c);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  получить цифровое значение из поля базы данных
_d  dbFieldGetVal(_i Sel, _i Fld, uc *FNm, _i md){
    _i  FOfs=0;    
    _i  i[3];
    _d  d[3];
    if (Fld<0){ Fld=dbFieldGetNum(Sel,0,FNm); }
    if (dbField[Sel][Fld].Tip!='N'&&dbField[Sel][Fld].Tip!='F'&&
        dbField[Sel][Fld].Tip!='B'&&dbField[Sel][Fld].Tip!='I'){ return(0); }
    if((FOfs=(_i)dbFieldOffset(Sel,Fld,md))>0){           
        if(dbField[Sel][Fld].Tip=='N'||dbField[Sel][Fld].Tip=='F'){
           if(dbField[Sel][Fld].Len>=DBSLEN){ dbField[Sel][Fld].Len=DBSLEN-1; }
           if(md==DBWBUFF){
              memcpy(&BSFld[3],&BRec[Sel][FOfs],dbField[Sel][Fld].Len);
           } else {
              read(dbFile[Sel].Desk,&BSFld[3],dbField[Sel][Fld].Len); 
           }
           BSFld[3][dbField[Sel][Fld].Len]=0;  d[0]=atof(BSFld[3]);
           return(d[0]);
        }
        if(dbField[Sel][Fld].Tip=='B'){
           if(md==DBWBUFF){
              memcpy(&d,&BRec[Sel][FOfs],LENDOUBLE);
           } else {
              read(dbFile[Sel].Desk,&d,LENDOUBLE); 
           }; return(d[0]);
        }
        if(dbField[Sel][Fld].Tip=='I'){
           if(md==DBWBUFF){
              memcpy(&i,&BRec[Sel][FOfs],LENINTEGR);
           } else {
              read(dbFile[Sel].Desk,&i,LENINTEGR);
           }; return(i[0]);
        }
    } 
    return(0);
}
//----------------------------------------------------------------------------
