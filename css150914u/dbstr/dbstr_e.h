// coding: cp866
//---------------------------------------------------------------------------
// 5 записей по 50 полей (каждое поле по SLEN символов)
ex uc GFld[MREC][MFLD][SLEN];
// текущая строка
ex uc GStr[SLEN];
// позиция в буфере
ex _i GStrPos;
// строка символов разделителей
ex uc GSRFld[LSRR];
// количество полей в текущей строке
ex _i HowFLD;
// текущая позиция в файле
ex _i GBufCFP;
//---------------------------------------------------------------------------
ex _i  _mfGetStr (uc *,_i);
ex _i   PutFields(_i,uc *, ...);
ex uc * GetField (_i,_i);
ex _i   isField  (_i,_i);
ex _i   isFieldb (_i,_i,_i);
ex _i   GetCHR_in_FLD(_i,_i,uc);
ex _i   ClearGStr(vd);
ex uc * GetFieldFromStr(uc *,uc ,_i);
//---------------------------------------------------------------------------
