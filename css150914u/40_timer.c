// coding: cp866
//---------------------------------------------------------------------------
// ���稪� ⠩��� ⨪��
vd tt_tick(vd){
    int a=0;
    for(a=0;a<10;a++) if(HTimer[a]<0xffffff) HTimer[a]++; else HTimer[a]=0;
}
#ifndef WINDOW
END_OF_FUNCTION(tt_tick);
#endif
//---------------------------------------------------------------------------
// ���稪� ⠩��� �����ᥪ㭤
vd tt_msec(vd){
    int a=0;
    for(a=0;a<10;a++) if(mTimer[a]<0xffffff) mTimer[a]++; else mTimer[a]=0;
}
#ifndef WINDOW
END_OF_FUNCTION(tt_msec);
#endif
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// �஢�ઠ ⠩��஢
_i MTimer(ui n, uc mh, ui c){
   // �஢�ઠ �ࠢ��쭮�� ����� ⠩���
   if(n>=HTIMERS){ return(0); }
   // ���㫥��� ���稪� 㪠������� ⠩���
   if(c==0){ 
      if(mh=='m'||mh=='M'){ mTimer[n]=0; }
      else                { HTimer[n]=0; }
      return(0); 
   }
   // ⠩��� �ࠡ�⠫
   if(mh=='m'||mh=='M'){
      if(mTimer[n]>=c){ return(2); }
   } else {
      if(HTimer[n]>=c){ return(2); }
   }
   // ⠩��� �� �믮������
   return(1);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// ������� � �뢥�� ⥪���� ���� � �६�
_i GetTime(_i md){
   _i a=0, b=0;
   // ������� ⥪�饥 �६�
   TMV=time(NULL);
   tmm=localtime(&TMV);
   sprintf(MDate,"%02d.%02d.%04d ",tmm->tm_mday,tmm->tm_mon+1,tmm->tm_year+1900);
   sprintf(MTime,"%02d:%02d:%02d ",tmm->tm_hour,tmm->tm_min,tmm->tm_sec);
   YEA_CMD=tmm->tm_year+1900;
   MON_CMD=tmm->tm_mon +1;
   DAY_CMD=tmm->tm_mday;
   HOU_CMD=tmm->tm_hour;
   MIN_CMD=tmm->tm_min;
   SEC_CMD=tmm->tm_sec;
   if (md==0){ return(0); }
   // ���
   if (VKDATE[CFirst].sn>0){
       for(a=0;a<VKDATE[CFirst].lcs;a++){
           // ��।����� ������ ��ப�
           b=text_length(dFnt[VKDATE[CFirst].sn].dat, MDate);
           // �᫨ ��ப� ������ - ����塞 ��᫥���� ᨬ���
           if (b>VKDATE[CFirst].lcs){ MDate[strlen(MDate)-1]=0; }
           else                   { break; }
       }
       // �뢥�� �� �࠭
       text_mode(VKDATE[CFirst].cg);
       textout(scrsav,dFnt[VKDATE[CFirst].sn].dat,MDate,VKDATE[CFirst].X,VKDATE[CFirst].Y,VKDATE[CFirst].cs);
   }
   // �६�
   if (VKTIME[CFirst].sn>0){
       for(a=0;a<VKTIME[CFirst].lcs;a++){
           // ��।����� ������ ��ப�
           b=text_length(dFnt[VKTIME[CFirst].sn].dat, MTime);
           // �᫨ ��ப� ������ - ����塞 ��᫥���� ᨬ���
           if (b>VKTIME[CFirst].lcs){ MTime[strlen(MTime)-1]=0; }
           else                   { break; }
       }
       // �뢥�� �� �࠭
       text_mode(VKTIME[CFirst].cg);
       textout(scrsav,dFnt[VKTIME[CFirst].sn].dat,MTime,
               VKTIME[CFirst].X,VKTIME[CFirst].Y,VKTIME[CFirst].cs);
   }
   return(1);
}
//---------------------------------------------------------------------------



//****************************************************************************
// QueryPerformanceCounter() ������⢮ ⠪⮢ ���� � ���୮�� �������
// ���⮩ 3.57 MHz  (3579545 ⨪�� �� ���� ᥪ㭤�).
// ��� ���稪 ����� ���,  祬 RDTSC, �� ࠡ�⠥� ⮫쪮 ��� Windows.

// �ࠢ����� ���� ���稪�� � ������ १���� � ��ப���� ��६�����
LARGE_INTEGER  vvkpf,  vvkpc0,  vvkpc1;
char           vvkpfstr[2048];
char * Profile_us(int mode){
  float TE=0;
  if(mode==0){
     // ������� ���祭�� ����� ����७���� ⠩���
     QueryPerformanceFrequency(&vvkpf);
     // ������� ���祭�� ���稪� 0
     QueryPerformanceCounter  (&vvkpc0);
     return(" ");
  }
  // ������� ���祭�� ���稪� 1
  QueryPerformanceCounter(&vvkpc1);
  // ������� ࠧ���� ���稪�� � ᥪ㭤�
  TE=((float)(vvkpc1.QuadPart-vvkpc0.QuadPart)/(float)(vvkpf.QuadPart));
  // ������� � ��ப���� ��६����� ��稪� � ࠧ���� ����� ���� � ����ᥪ㭤��
  sprintf(vvkpfstr,"PC0[%.0f] PC1[%.0f] TE[%.0f us]",
         (float)vvkpc0.QuadPart,(float)vvkpc1.QuadPart,TE*1000000);
  return (vvkpfstr);
}
//----------------------------------------------------------------------------
// ����প� �� 㪠������ ������⢮ ����ᥪ㭤
void win_delay(char what, float how){
     LARGE_INTEGER pc1, pc0, pf;
     float TE;
     // ������� ���祭�� ����� ⠩���
     if (QueryPerformanceFrequency(&pf )){
         // ������� ���祭�� ���稪�0
         QueryPerformanceCounter  (&pc0);
         for(;;){
           // ������� ���祭�� ���稪�1
           QueryPerformanceCounter(&pc1);
           // ���᫨��  ⥪�饥 ������⢮ ⨪�� ⠩��� ����� ���稪��0 � 
           // ���稪��1 �� ᥪ㭤�
           TE=((float)pc1.QuadPart-(float)pc0.QuadPart)/(float)pf.QuadPart;
           // ��ॢ��� �� ���祭�� � ����ᥪ㭤� � �ࠢ���� � �������:
           // �᫨  ࠧ��� ����� ⥪�騬 ���祭��� ���稪� � ����������
           // ����� ��������� ���祭�� ����ᥪ㭤 - ��� �� 横��.
           if(what=='s'||what=='S') TE=TE*1;
           if(what=='m'||what=='M') TE=TE*1000;
           if(what=='u'||what=='U') TE=TE*1000000;
           if (TE >= how) break;
         }
     }
     return;
}
//----------------------------------------------------------------------------
