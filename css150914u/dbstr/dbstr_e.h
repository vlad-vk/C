// coding: cp866
//---------------------------------------------------------------------------
// 5 ����ᥩ �� 50 ����� (������ ���� �� SLEN ᨬ�����)
ex uc GFld[MREC][MFLD][SLEN];
// ⥪��� ��ப�
ex uc GStr[SLEN];
// ������ � ����
ex _i GStrPos;
// ��ப� ᨬ����� ࠧ����⥫��
ex uc GSRFld[LSRR];
// ������⢮ ����� � ⥪�饩 ��ப�
ex _i HowFLD;
// ⥪��� ������ � 䠩��
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
