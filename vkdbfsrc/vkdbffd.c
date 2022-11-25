//------------------------------------------------------------------------------
// functions for working with database fields

//------------------------------------------------------------------------------
//  check if the specified field in the database structure is filled
_i  dbFieldTest(_i Sel, _i Fld){
    if (dbField[Sel][Fld].Name[0] == 0){
        return(-1);
    }
    if (dbField[Sel][Fld].Tip != 'C' && dbField[Sel][Fld].Tip != 'L' &&
        dbField[Sel][Fld].Tip != 'D' && dbField[Sel][Fld].Tip != 'M' &&
        dbField[Sel][Fld].Tip != 'N' && dbField[Sel][Fld].Tip != 'F' &&
        dbField[Sel][Fld].Tip != 'B' && dbField[Sel][Fld].Tip != 'I'){ 
        return(-2);
    }
    return(1);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  clear the specified field in the database structure
vd  dbFieldClear(_i Sel, _i Fld){
    dbField[Sel][Fld].Name[0]   = 0;
    dbField[Sel][Fld].Tip       = 0; 
    dbField[Sel][Fld].Len       = 0;
    dbField[Sel][Fld].Dec       = 0; 
    dbHExt[Sel].Len             = 0;
    return;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  filling in a record to create a database
_i  dbFieldFill(_i Sel, _i Fld, uc *FName, uc Tip, uc Len, uc Dec){
    if (Tip != 'C' && Tip != 'L' && Tip != 'D' && Tip != 'M' && Tip != 'N' &&
        Tip != 'F' && Tip != 'B' && Tip != 'I'){
        return(-1);
    }
    if (Len == 0){
        return(-2);
    }
    if (Tip == 'B' && Len != LENDOUBLE){ Len = LENDOUBLE; }
    if (Tip == 'I' && Len != LENINTEGR){ Len = LENINTEGR; }
    if (Tip == 'D' && Len != LENDATE  ){ Len = LENDATE;   }
    if (Tip == 'L' && Len != LENLOGIC ){ Len = LENLOGIC;  }
    
    strcpy(dbField[Sel][Fld].Name, FName);
    
    dbField[Sel][Fld].Name[10]  = 0;
    dbField[Sel][Fld].Tip       = Tip;
    dbField[Sel][Fld].Len       = Len;
    dbField[Sel][Fld].Dec       = Dec;
    
    return(1);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  return field name by its number
uc *dbFieldGetName(_i Sel, _i Fld){
    if((dbFieldTest(Sel, Fld)) < 1){
        return("");
    }
    return(dbField[Sel][Fld].Name);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  return field type by its number
uc  dbFieldGetType(_i Sel, _i Fld){
    if((dbFieldTest(Sel, Fld)) < 1){
        return(0);
    }
    return(dbField[Sel][Fld].Tip);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  return field size by its number
uc  dbFieldGetLen(_i Sel, _i Fld){
    if((dbFieldTest(Sel, Fld)) < 1){
        return(0);
    }
    return(dbField[Sel][Fld].Len);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  return the number of characters after the decimal point by its number
uc  dbFieldGetDec(_i Sel, _i Fld){
    if((dbFieldTest(Sel, Fld)) < 1){
        return(0);
    }
    return(dbField[Sel][Fld].Dec);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  returns the field number by its name, if the field does not exist, then returns -1
//  Reg=0 - case-insensitive, otherwise case-sensitive
_i  dbFieldGetNum(_i Sel, _i Reg, uc *FldName){
    _i  a = 0, r = -1;

    for(a = 0; a < DBMAXFLD; a++){
        if(Reg == 0){
            if((stricmp(FldName, dbField[Sel][a].Name)) == 0){
               r = a;
               break;
            }
        } else {
            if((strcmp(FldName, dbField[Sel][a].Name)) == 0){
               r = a;
               break;
            }
        }
    }
    return(r);
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  returns the number of database fields from the specified area
_i  dbFieldCount(_i Sel){
    _i r = -1;
    if (dbOpenTest(Sel, 0) >= 0){
        r = dbHeaw[Sel].FieldCount;
    }
    return(r);
}
//------------------------------------------------------------------------------

