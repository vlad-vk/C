// coding: cp866
//---------------------------------------------------------------------------
// �஢�ઠ �ࠢ��쭮�� ��������� ����
_i CheckDate(vd){
   _i a, ret=0;
   _c c[5];
   // ������� ���, ����� � �᫮
   for(a=0;a<4;a++){ c[a]=GS.ES[a+6]; }; c[a]=0; CDY=atoi(c);   // ���
   for(a=0;a<2;a++){ c[a]=GS.ES[a+3]; }; c[a]=0; CDM=atoi(c);   // �����
   for(a=0;a<2;a++){ c[a]=GS.ES[a+0]; }; c[a]=0; CDD=atoi(c);   // ����
   if (CDY<2002 || CDY>2100){ ret=1; }
   if (CDM<1    || CDM>12  ){ ret=2; }
   if (CDD<1    || CDD>31  ){ ret=3; }
   return(ret);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// �஢�ઠ �ࠢ��쭮�� ���������� �६���
_i CheckTime(vd){
   _i a, ret=0;
   _c c[5];
   // ������� ���, ������ � ᥪ㭤�
   for(a=0;a<2;a++){ c[a]=GS.ES[a+0]; }; c[a]=0; CTH=atoi(c);   // ���
   for(a=0;a<2;a++){ c[a]=GS.ES[a+3]; }; c[a]=0; CTM=atoi(c);   // ������
   for(a=0;a<2;a++){ c[a]=GS.ES[a+6]; }; c[a]=0; CT1=atoi(c);   // ᥪ㭤�
   if (CTH<0 || CTH>23){ ret=1; }
   if (CTM<0 || CTM>59){ ret=2; }
   if (CT1<0 || CT1>59){ ret=3; }
   RAStE=CTH*10000+CTM*100+CT1;
   return(ret);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ��⠭����� ���� ��ᬮ�� ��娢�
_i SetDateArc(vd){
   _i ta=TF[CTC].ta;
   // �᫨ ��� ������� ���४⭮
   if (CheckDate()==0){
      // ��।��� ��������� ���祭�� � ��६����� ���� ��ᬮ��
      strcpy(TF[CTC].dl[ta],TF[CTC].dt[ta]);
      strcpy(TF[CTC].dt[ta],GS.ES);
      // ��ନ஢��� ��� 䠩�� ��娢�
      if(TF[CTC].ff[ta]==3)
         sprintf(TF[CTC].fa[ta],"%s/arc/%04d%02d%02d.dbu",FDir,CDY,CDM,CDD);
      else
         sprintf(TF[CTC].fa[ta],"%s/arc/%04d%02d%02d.dba",FDir,CDY,CDM,CDD);
      // ��ନ஢��� ��� 䠩�� ������
      sprintf(TF[CTC].fi[ta],"%s/arc/%04d%02d%02d.idx",FDir,CDY,CDM,CDD);
      // �஢�ઠ ����㯠
      if((access(TF[CTC].fa[ta],4))<0){
          sprintf(TF[CTC].fa[ta],"%s/%04d%02d%02d.dba",ARCapath,CDY,CDM,CDD);
          sprintf(TF[CTC].fi[ta],"%s/%04d%02d%02d.idx",ARCapath,CDY,CDM,CDD);
      }
      // �᫨ ��� ����㯠 �� �⥭��- ��⠢��� ⥪�騩 䠩�
      if((access(TF[CTC].fa[ta],4))<0){
         sprintf(STAT[1].str,"�訡�� ����㯠 � 䠩�� ��娢� [%s]",TF[CTC].fa[ta]);
         STAT[1].cnt=10;
         strcpy(TF[CTC].dt[ta],TF[CTC].dl[ta]);
         strcpy(GS.ES         ,TF[CTC].dl[ta]); CheckDate();
         if(TF[CTC].ff[ta]==3)
            sprintf(TF[CTC].fa[ta],"%s/arc/%04d%02d%02d.dbu",FDir,CDY,CDM,CDD);
         else
            sprintf(TF[CTC].fa[ta],"%s/arc/%04d%02d%02d.dba",FDir,CDY,CDM,CDD);
         //
         sprintf(TF[CTC].fi[ta],"%s/arc/%04d%02d%02d.idx",FDir,CDY,CDM,CDD);
      // �᫨ ����� ����...
      }  else  {
          FARVW();
      }
   }  else  {
      sprintf(STAT[1].str,SMSG[115]); STAT[1].cnt=5;
   }
   GS.fe=0;
   return(0);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ��⠭����� �६� ��ᬮ�� ��娢�
_i SetTimeArc(vd){
   _i a=0;
      GS.fe=0;
   if(FTC>0){
      sprintf(STAT[1].str,SMSG[116]); STAT[1].cnt=5;
      return(0);
   }
   if(CheckTime()==0){
      for(a=0;a<HTRD;a++){
          if (TF[CTC].an[a]==1){ RASAT=a; break; }
      }
      strcpy(TF[CTC].tm[RASAT],GS.ES); FTC=1;
   }  else  {
      sprintf(STAT[1].str,SMSG[117]); STAT[1].cnt=5;
   }
   return(0);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ��⠭����� ���� ��ᬮ�� ����
_i SetDateRep(vd){
   _i f1=0;
   // �᫨ ��� ������� ���४⭮
   if (CheckDate()==0){
      // ��।��� ��������� ���祭�� � ��६����� ���� ��ᬮ��
      strcpy(LFVW[FVA].dl,LFVW[FVA].dt);
      strcpy(LFVW[FVA].dt,GS.ES);
      // ��ନ஢��� ��� 䠩�� ����
      sprintf(FNFVW,"%s/arc/%04d%02d%02d.dbr",FDir,CDY,CDM,CDD);
      // �᫨ �।��騩 䠩� �� ⥪�騩 䠩� ॣ����樨 ᮮ�饭��
      if ((strcasecmp(FNRep,LFVW[FVA].fn))!=0){ f1=1; }
      // �஢�ઠ ����㯠
      if((access(FNFVW,4))<0){
          sprintf(FNFVW,"%s/%04d%02d%02d.dbr",ARCapath,CDY,CDM,CDD);
      }  
      // �᫨ ��� ����㯠 �� �⥭��
      if((access(FNFVW,4))<0){
         sprintf(STAT[1].str,SMSG[118],FNFVW);
         strcpy (LFVW[FVA].dt,LFVW[FVA].dl);
         strcpy (GS.ES       ,LFVW[FVA].dl); CheckDate();
         sprintf(FNFVW,"%s/arc/%04d%02d%02d.dbr",FDir,CDY,CDM,CDD);
         strcpy (LFVW[FVA].fn,FNFVW);
      // �᫨ ����� ���� - ������� �।��騩 䠩� � ������ ����
      }  else  {
         if (f1==1){ dbClose(VW[FVA].fhd); }
         strcpy(LFVW[FVA].fn,FNFVW);
      }
      ViewFile(FVA,0);
   }  else  {
      sprintf(STAT[1].str,SMSG[119]); 
   }
   GS.fe=0; STAT[1].cnt=5;
   return(0);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ��⠭����� ��ப� ���᪠ � 䠩��
_i SetFindStr(vd){
   strcpy(VW[FVA].fst,GS.ES); GS.fe=0; return(0);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ���� � ��娢� �� �६���
_i RACTSeek(){
   _i  a=0, t=RASAT, cnt=0;
   if (FTC==0){ return(0); }
   // ��।����� ��砫쭮� ᬥ饭��,䫠� ��砫� ���᪠,����� ��⨢���� �७��
   if (FTC==1&&TF[CTC].ff[t]!=3){
       // �⥭�� �����᭮�� 䠩�� ��� ���᪠ 
       if((ReadIDX(2,t))>0){
           sprintf(STAT[1].str,SMSG[120],TF[CTC].fi[t]);
           STAT[1].cnt=5; FTC=0; return(0);
       }
       // ������⢮ ����ᥩ � 䠩�� ��娢� �� ⥪�騩 ������
       RASFS=dbRecCount(TF[CTC].fh[t]); FTC=2; RASSt=1;
       sprintf(STAT[1].str,SMSG[121]); STAT[1].cnt=5;  return(0);
   }
   // ��।������ ����� ����� �� �����᭮�� 䠩��
   if (FTC==2){
       for(a=RASSt,cnt=0;a<IDXCURS;a++){ 
           if(RAStE<=IDXSER[a].TIM&&IDXSER[a].nREC>0){ FTC=4; break; }
           if(cnt>500){ break; }     // ���稪 ����ᥩ ��� ������ 横��
       }
       RASSt=a; if(a>=IDXCURS){ FTC=3; }
   }
   // �᫨ ��祣� ��宦��� �� ��諨- ��⠭����� ᬥ饭�� �� ����� 䠩��
   if (FTC==3){
       sprintf(STAT[1].str,SMSG[122]);
       RABuf[RASAT].pr=RASFS-1;
       RARead(RASAT,1); TLKey=2; FTC=0;
   }
   // ��।������ ⥪�饣� ᬥ饭�� ��� 䠩�� ��娢�
   if (FTC==4){
       sprintf(STAT[1].str,SMSG[123]); 
//     sprintf(STAT[1].str,"RqTm=%d TIM=%d REC=%d",RAStE,IDXSER[a].TIM,IDXSER[a].nREC);
       RABuf[RASAT].pr=IDXSER[a].nREC;
       RARead(RASAT,2); TLKey=3; FTC=0;
   };  STAT[1].cnt=10;
   return(0);
}
//---------------------------------------------------------------------------
