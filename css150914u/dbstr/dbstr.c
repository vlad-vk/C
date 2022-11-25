// coding: cp866
//---------------------------------------------------------------------------
// �㭪樨 ��� ࠡ��� � ��ப���

#include "../include.h"
#include "../define.h"
#include "dbstr.h"

//---------------------------------------------------------------------------
// ������� ᫥������ ��ப� �� ����
_i _mfGetStr(uc *rBuf, _i hbr){
   _i  a=0, b=0;
   // ��� ������� ᨬ���� ����, ���� �� ����� ��ப�
   for(a=GStrPos; a<hbr; a++){
       // ��।����� ����� ��ப�
       if (rBuf[a]==10){ 
           GStrPos=a+1; GStr[b]=0; return(1); 
       }
       // ������� ��ப�
       if (rBuf[a]>=27){ GStr[b]=rBuf[a]; if(b<SLEN-1){ b++; }; GStr[b]=0; }
   }  
   GStrPos=0;
   return(0);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ��������� ���ᨢ ����� ⥪�饩 ��ப� (�����)
// r    - ����� �ନ�㥬�� �����
// GStr - ࠧ��ࠥ��� ��ப�
_i PutFields(_i r, uc *GStr, ...){
   va_list argptr;
   _i a=0, b=0, c=0, d=0, e=0, f=0;
   _i l=strlen(GStr);
   // �᫨ ��ப� ����� - ���
   if(l<1){ return(0); }
   // ��।�� ��ࠬ���஢ � ���ᨢ ࠧ����⥫��
   va_start(argptr,GStr);
   for(a=0;a<SLEN;a++){
       GSRFld[a]=(uc)va_arg(argptr,int); if(GSRFld[a]==0){ break; }
   }
   va_end  (argptr);
   // ���㫨�� ��ࢮ� ����
   GFld[r][b][c]=0;
   // ��� ������� ᨬ���� ��ப�
   for(a=0; a<=l; a++, e=0){
       // �᫨ ����⨫�� ᨬ��� ࠧ����⥫� �����
       for(d=0; d<LSRR && GSRFld[d]>0; d++){
           if((GStr[a]==GSRFld[d])||(GStr[a]==0)){
               // ��������� ���� ����⨢訩�� ࠧ����⥫� �����
               if(f==0){ f=GSRFld[d]; }
               // ����� 䫠��
               e=1; if(b<MFLD-1){ b++; }; c=0; GFld[r][b][0]=0; 
           }
       }
       // ��������� ⥪�饥 ����
       if (e==0){
           GFld[r][b][c]=GStr[a]; if(c<SLEN-1){ c++; }; GFld[r][b][c]=0;
       }
   }
   // � ��⠢���� ����� ���㫨�� �㫥��� ᨬ���
   if (e==0){ GFld[r][b][0]=0; }
   for(b++,HowFLD=b; b<MFLD; b++){ GFld[r][b][0]=0; }
   // ������ ���� ����⨢訩�� ࠧ����⥫� �����
   return(f);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ������ ��ப� �� ����訢������ ����
uc * GetField(_i nREC, _i nFLD){
     if (GFld[nREC][nFLD][0]==0){ return(""); }
     else                       { return(GFld[nREC][nFLD]); }
}
//---------------------------------------------------------------------------
// ������� �� ��।����� ��ப� �������� �� ������ ����
uc   FldStr[1024];
uc * GetFieldFromStr(uc *String, uc Razdelit, _i FNumber){
     _i  a=0, b=0, f=0, L=strlen(String), r=1, z=0;
     for(a=0; a<L; a++){
         // �᫨ ⥪�騩 ᨬ��� ࠧ����⥫� � �뫮 ��⠭� ����
         if((String[a]==Razdelit&&f==1)||(String[a]==0)){
            b=0; f=0; if(r==FNumber) break; r++; continue;
         }
         if(String[a]==Razdelit) continue;
         // �᫨ ⥪�騩 ᨬ��� �� ࠧ����⥫�, �।��騩 �� ࠧ����⥫�
         if(String[a]!=Razdelit&&f==0) f=1;
         // �᫨ ����� ���� ࠢ�� ���������- ����� ��� � ��६�����
         if(r==FNumber){
            FldStr[b]=String[a]; if(b<1023) b++; FldStr[b]=0; z=1;
         }
     }
     if(z==1){ return(FldStr); }
     else    { return("");     }
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ������ ����� 㪠������� ���� (0-��� ����, 1-����)
_i isField(_i nREC, _i nFLD){
   if (GFld[nREC][nFLD][0]==0){ return(0); }
   else                       { return(1); }
}
//---------------------------------------------------------------------------
// ������ ���� � ����
_i isFieldb(_i nREC, _i nFLD, _i bt){ return(GFld[nREC][nFLD][bt]); }
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ������ ������⢮ �宦����� ��������� ᨬ���� � 㪠������ ����
_i GetCHR_in_FLD(_i nREC, _i nFLD, uc sCHR){
   _i  a=0, r=0;
   for(a=0; a<strlen(GFld[nREC][nFLD]); a++){
       if(GFld[nREC][nFLD][a]==sCHR){ r++; }
   }; return(r);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ������ ⥪���� ��ப�
_i ClearGStr(vd){
   _i  a=0;
   for(a=0; a<SLEN; a++){ GStr[a]=0; }
   return(0);
}
//---------------------------------------------------------------------------

