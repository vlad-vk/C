// coding: cp866
//---------------------------------------------------------------------------
// ����⨥ 䠩��� ��娢� � ��������� ��� ��ᬮ��
vd FARVW(vd){
   _i ta=TF[CTC].ta;
   if  (TF[CTC].ff[ta]==1){ 
        dbClose(TF[CTC].fh[ta]);
        TF[CTC].ff[ta]=0;
   }
   //------------------------------------------------------------------------
   // �᫨ ࠡ�⠥� � 䠩��� ��娢�
   if(LTRD[ta].th<10){
      // �᫨ ��⠭�������� ��� �� (FNArc) != ⥪�饬� (TF[CTC].fa[ta])
      if ((strcasecmp(TF[CTC].fa[ta],FNArc))!=0){
          // ��।������ ᢮������ ������ ��� ࠡ��� � �� ��娢�
          if((TF[CTC].fh[ta]=dbSelectAvail())<0){           // ��� ᢮������     
              sprintf(SMSG[MMSG],SMSG[124]);                // ������ ��� ࠡ���
              DPrn(13,0,"%s\n",SMSG[MMSG]);             // � �� ��娢�
              return;                                       //
          }
          // ���樠������ � ����⨥ �� ��娢�
          dbInit(TF[CTC].fh[ta]);
          if(dbOpen(TF[CTC].fh[ta],TF[CTC].fa[ta])<0){      // �訡�� ������ 
             sprintf(STAT[1].str,SMSG[125],TF[CTC].fa[ta]); // 䠩��  �� ��娢�
             dbClear(TF[CTC].fh[ta]);                       // 
             TF[CTC].fh[ta]=ARCOut;                         //
             TF[CTC].ff[ta]=2;                              //
             return;
          }
          sprintf(STAT[1].str,SMSG[126],TF[CTC].fa[ta]);     // ���� �� ��娢�
          TF[CTC].ff[ta]=1;                                  // ����� ��� ࠡ���
          // �⥭�� 䠩�� ��娢�
          RARead(ta,0);                                      
      //----------------------------------------------------------------------
      }  else  {
         // ࠡ�� � ⥪�騬 䠩��� �� ��娢� (�� ⥪���� ����)
         sprintf(STAT[1].str,SMSG[127]);                    // ����騩 䠩�
         TF[CTC].fh[ta]=ARCOut;                             // �� ��娢�
         TF[CTC].ff[ta]=2;                                  //
         RARead(ta,0);
      } 
   } else {
   //------------------------------------------------------------------------
   // �᫨ ࠡ�⠥� � 䠩��� ���������
      if ((strcasecmp(TF[CTC].fa[ta],FNUpd))!=0){
          // ��।������ ᢮������ ������ ��� ࠡ��� � �� ���������
          if((TF[CTC].fh[ta]=dbSelectAvail())<0){
              DPrn(13,0,"��� ᢮������ ������ ��� �� ��������� [%s]\n",FNUpd);
              return;
          }
          // ���樠������ � ����⨥ �� ���������
          dbInit(TF[CTC].fh[ta]);
          if(dbOpen(TF[CTC].fh[ta],TF[CTC].fa[ta])<0){
             sprintf(STAT[1].str,"�訡�� ������ �� ��������� [%s]",FNUpd);
             dbClear(TF[CTC].fh[ta]);
             TF[CTC].fh[ta]=UPDOut;
             TF[CTC].ff[ta]=3;
             return;
          }
          sprintf(STAT[1].str,"����� 䠩� �� ��������� [%s]",FNUpd);
          TF[CTC].ff[ta]=1;
          // �⥭�� 䠩�� �� ���������
          RARead(ta,0);                                      
       }  else  {
          // ࠡ�� � ⥪�騬 䠩��� �� ��������� (�� ⥪���� ����)
          sprintf(STAT[1].str,"���� ��������� �� ⥪���� ����");
          TF[CTC].fh[ta]=UPDOut;
          TF[CTC].ff[ta]=3;
          RARead(ta,0);
       }
       DPrn(13,0,"UPD check test: [%s]\n",FNUpd);
   }
   //------------------------------------------------------------------------
   TLKey=4; STAT[1].cnt=5;
   return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ����⨥ 䠩��� ���������
vd FUPOpen(vd){
   if(DBUFlg==0) return;
   if(UpdFL>0){ dbClose(UPDOut); }
   UpdFL=1;
   if((CreateUPD())==0){ 
       sprintf(STAT[1].str,"Error create(open) updates data file [%s]",FNUpd);
       DPrn(13,0,"%s\n",STAT[1].str);
       STAT[1].cnt=5;   fflush(DebOut);
       UpdFL=0; 
   }
   return;
}
//---------------------------------------------------------------------------
// ����⨥ 䠩��� ��娢� � ���� �ॢ�� �� ���室� �� ����(��㣨�) ��⪨
vd FAROpen(vd){
   _i a=0;
   // ������� ⥪�騥 䠩��
   if(ArcFL>0){ dbClose(ARCOut); }
   if(RepFL>0){ dbClose(REPOut); }
   if(UpdFL>0){ dbClose(UPDOut); UpdFL=0; }
   dbBFClearALL();
   ArcFL=1;
   RepFL=1;
   // ������ ���� 䠩��
   if ((CreateARC())==0) {
      sprintf(SMSG[MMSG],SMSG[128],FNArc);
      DPrn(13,0,"%s\n",SMSG[MMSG]);
      sprintf(STAT[1].str,SMSG[129],FNArc);
      STAT[1].cnt=5; ArcFL=0; fflush(DebOut);
   }
   if ((CreateREP())==0) {
      sprintf(SMSG[MMSG],SMSG[130],FNRep);
      DPrn(13,0,"%s\n",SMSG[MMSG]);
      sprintf(STAT[1].str,SMSG[131],FNRep);
      STAT[1].cnt=5; RepFL=0; fflush(DebOut);
   }
   FUPOpen();
   gDiskFree(0);
   // ������� ⥪�騩 ���� ������
   int yea=tmm->tm_year+1900, mon=tmm->tm_mon+1, day=tmm->tm_mday;
   int ai= (14 - mon) / 12;
   int yi= yea - ai;
   int mi= mon + 12 * ai - 2;
   WKDAY = ((7000 + (day + yi+ yi/ 4 - yi/ 100 + yi/ 400 + (31 * mi) / 12)) - 1 ) % 7;
   // printf(" WKDAY=%f ",WKDAY);
   return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
vd FARNRd(vd){
   _i a=0;
   for(a=0;a<MTRD;a++){
      // ��।������ ����� �� ��娢� ��� �७��
      if(TF[CTC].ff[a]==3)
         sprintf(TF[CTC].fa[a],"%s/arc/%d%02d%02d.dbu",
                 FDir,tmm->tm_year+1900,tmm->tm_mon+1,tmm->tm_mday);
      else
         sprintf(TF[CTC].fa[a],"%s/arc/%d%02d%02d.dba",
                 FDir,tmm->tm_year+1900,tmm->tm_mon+1,tmm->tm_mday);
      // ��।������ ����� 䠩�� ������ �� ��娢�
      sprintf(TF[CTC].fi[a],"%s/arc/%d%02d%02d.idx",
              FDir,tmm->tm_year+1900,tmm->tm_mon+1,tmm->tm_mday);
      // ��������� ���� ��।������ ����� �� ��娢� ��� �७��
      TF[CTC].yr[a]=tmm->tm_year+1900;
      TF[CTC].mn[a]=tmm->tm_mon +1;
      TF[CTC].dn[a]=tmm->tm_mday;
      sprintf(TF[CTC].dt[a],"%02d.%02d.%4d",
              tmm->tm_mday,tmm->tm_mon+1,tmm->tm_year+1900);
   }
   return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ��।����� ��� ⥪�饣� 䠩�� ���������
vd FUPName(int year, int mon, int day){
   sprintf(FNUpd,"%s/arc/%04d%02d%02d.dbu", FDir,year,mon,day);
   return;
}
//---------------------------------------------------------------------------
// ��।����� ��� ⥪�饣� 䠩�� ��娢� � ���� �ॢ��
vd FARName(vd){
   sprintf(FNUpd,"%s/arc/%04d%02d%02d.dbu",
           FDir,tmm->tm_year+1900,tmm->tm_mon+1,tmm->tm_mday);
   sprintf(FNArc,"%s/arc/%04d%02d%02d.dba",
           FDir,tmm->tm_year+1900,tmm->tm_mon+1,tmm->tm_mday);
   sprintf(FNRep,"%s/arc/%04d%02d%02d.dbr",
           FDir,tmm->tm_year+1900,tmm->tm_mon+1,tmm->tm_mday);
   sprintf(FNIdx,"%s/arc/%04d%02d%02d.idx",
           FDir,tmm->tm_year+1900,tmm->tm_mon+1,tmm->tm_mday);
   return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ��� ���� ��娢� �� ���
vd ARCFlush(vd){
   if(ARCBFR>0&&ArcFL>0&&dbOpenTest(ARCOut,0)>=0){
      dbBFWrite(ARCOut,ARCBuf,ARCBFR,DBBFADD); 
      dbBFGoFirst(ARCBuf); ReadIDX(1,0); ARCBFR=0; IDXCNT=0;
   }
   return;     
}
//---------------------------------------------------------------------------
// ������ ��娢��� ���祭�� � ���� � ������ ���� � 䠩�
vd ArcWrite(vd){
   _i a=0;
   // �᫨ ����� ᥪ㭤�
   GetTime(0);
   if (tmm->tm_sec!=tmm_seco){
       tmm_seco=tmm->tm_sec; 
       // ��⠭����� 䫠�� ᬥ�� ᥪ㭤 ��� �७���
       for(a=0;a<HTRD;a++){ TF[CTC].fl[a]=1; }; 
       TRFill(1);
       TIMF=tmm->tm_hour*10000+tmm->tm_min*100+tmm->tm_sec;
       // �㫥�� ���祭�� �� ��ࠡ��뢠���� (�� ����)
       if(TIMF<1){ return; }
       // ������� ⥪�騥 ���祭�� ��娢��� �������
       a=0;
       dbFieldPutStr(ARCOut,a,0,"\xa",DBWBUFF); a++;
       dbFieldPutVal(ARCOut,a,0, TIMF,DBWBUFF); a++;
       dbFieldPutVal(ARCOut,a,0, TMST,DBWBUFF); a++;
       sprintf(AW_C,"%02d",tmm->tm_hour);
       dbFieldPutStr(ARCOut,a,0, AW_C,DBWBUFF); a++;
       sprintf(AW_C,"%02d",tmm->tm_min );               
       dbFieldPutStr(ARCOut,a,0, AW_C,DBWBUFF); a++;
       sprintf(AW_C,"%02d",tmm->tm_sec );
       dbFieldPutStr(ARCOut,a,0, AW_C,DBWBUFF); a++;
       // ��� ������� ��娢��㥬��� ������
       for(;a<HARC+BREC;a++){
          // �஢�ઠ ���������
          if (arc_ch[a-BREC]<1||arc_ch[a-BREC]>=MCH){ continue;  }
          dbFieldPutVal(ARCOut,a,0, CB[arc_ch[a-BREC]].v, DBWBUFF);
       }
       dbBFRecCopyBuf(ARCOut,ARCBuf); dbBFGoSkip(ARCBuf,1); ARCBFR++;
       if(dbBFEof(ARCBuf)>0||ARCBFR>=ARSEC){ ARCFlush(); } 
       IDXTIM[IDXCNT].TIM =TIMF;
       IDXTIM[IDXCNT].nREC=ArcRecNum; ArcRecNum++; IDXCUR++; IDXCNT++;
       FARName();
       // �� ���室� �� ��뢥 ��⪨
       if(/*((tmm->tm_hour==0)&&(tmm_houro==23))||*/(tmm->tm_mday!=tmm_mdayo)){ 
            IDXCNT=0; IDXCUR=0; ARCFlush(); FAROpen(); 
       }
       tmm_houro=tmm->tm_hour;
       tmm_mdayo=tmm->tm_mday;
   }
   return;
}
//---------------------------------------------------------------------------


//----------------------------------------------------------------------------
_i  yeaC=0,monC=0,dayC=0, houC=0,minC=0,secC=0, cro=0;
uc  WrStrUpd[SLEN];
//---------------------------------------------------------------------------
//  ������ 䠩�� �� ��������� �������
vd  UpdWrite(vd){
    int   a=0, fh=-1, no=0, vv=0, crc=0, crt=0;
    if(DBUFlg==0) return;;
    // ������� ⥪�饥 �६�
    GetTime(0);
    TIMF=tmm->tm_hour*10000+tmm->tm_min*100+tmm->tm_sec;
    // �㫥�� ���祭�� �� ��ࠡ��뢠���� (�� ����)
    if(TIMF<1){ return; }
    // �᫨ 䫠� ��।�� �� ����஫��� = 1 � 蠣 ��।�� �� ����஫��� = 3
    if(CB[DBUFlg+ 9].v==1&&CB[DBUFlg+11].v==3){
       yeaC=CB[DBUFlg+ 0].v;
       monC=CB[DBUFlg+ 1].v;
       dayC=CB[DBUFlg+ 2].v;
       houC=CB[DBUFlg+ 3].v;
       minC=CB[DBUFlg+ 4].v;
       secC=CB[DBUFlg+ 5].v;
       no  =CB[DBUFlg+ 6].v;
       vv  =CB[DBUFlg+ 7].v;
       crt =CB[DBUFlg+ 8].v;

       // ������� CRC ⥪�饩 �����
       crc =monC+dayC+houC+minC+secC+no+vv;
       TIMF=houC*10000+minC*100+secC;

       // DPrn(0,0,"y=%d m=%d d=%d h=%d m=%d s=%d no=%d vv=%d crt=%d crc=%d\n",
       //         yeaC,monC,dayC,houC,minC,secC,no,vv,crt,crc);

       // �᫨ CRC ����⠭��� � CRC �ਭ�⮥ (crt=CB[38].v) ࠢ�� �
       // CRC ⥪�饩 ����� � CRC �।��饩 ����� �� ࠢ��
       if(crc==crt&&crc!=upd_ls.crc){
          // �᫨ ���� ����� � 䠩�� ��᫥���� �����- ����� ���
          if(access(FNDLS,6)==0){
             if((fh=open(FNDLS,O_RDWR|O_BINARY))>=0){
                read (fh,&upd_ls,sizeof(upd_ls));
                close(fh);
             }
          }
          // ��ନ஢��� ��� 䠩�� ���������
          FUPName(yeaC,monC,dayC);
          // ������ ��� ᮧ���� ����� 䠩� �� ��᪥
          FUPOpen(); 
          // �᫨ 䠩� ���� ��������� �� �����
          if(UpdFL>0){
             // �᫨ �� �����(ᮧ���) 䠩� ��᫥���� �����
             if((fh=open(FNDLS,O_CREAT|O_RDWR|O_BINARY,S_IREAD|S_IWRITE))>=0){
                // ������� ⥪�騥 ���祭�� ��娢��� ������� � ��
                a=0;
                // �������� ������ � �� ������� ���������
                dbRecAdd(UPDOut);
                // ��������� ���� ��
                dbFieldPutStr(UPDOut,a,0,"\xa",0); a++;
                dbFieldPutVal(UPDOut,a,0, TIMF,0); a++;
                dbFieldPutVal(UPDOut,a,0, TMST,0); a++;
                sprintf(AW_C,"%02d",houC);
                dbFieldPutStr(UPDOut,a,0, AW_C,0); a++;
                sprintf(AW_C,"%02d",minC);               
                dbFieldPutStr(UPDOut,a,0, AW_C,0); a++;
                sprintf(AW_C,"%02d",secC);
                dbFieldPutStr(UPDOut,a,0, AW_C,0); a++;
                // ��� ������� ��娢��㥬��� ������ ������� ���祭�� � ����
                for(;a<HUPD+BREC;a++){
                   // �᫨ ����� ������ ��� ���������- ��३� � ᫥���� ������
                   if (upd_ch[a-BREC]<1||upd_ch[a-BREC]>=MCH){ continue;  }
                   // �᫨ ����� ��।������ ������ ᮢ��� � ����஬ � �祩��
                   // ���祭�� �������- �������� ���祭�� �� �⮬ ������
                   if (no==upd_ch[a-BREC]){ upd_ls.vv[a-BREC]=vv; }
                   // ������� ���祭�� ⥪�饣� ������ (upd_ls.vv[]) � ⥪�饥
                   // ���� ⥪�饩 ����� �� ���������
                   dbFieldPutVal(UPDOut,a,0, upd_ls.vv[a-BREC],0);
                }
                // ������� ����������� ������ �� ���
                dbRecWrite(UPDOut);
                // ������� �ਭ��� ����� � 䠩� ��᫥���� �����
                upd_ls.yea=yeaC;
                upd_ls.mon=monC;
                upd_ls.day=dayC;
                upd_ls.hou=houC;
                upd_ls.min=minC;
                upd_ls.sec=secC;
                upd_ls.no =no;
                upd_ls.crc=crc;
                write(fh,&upd_ls,sizeof(upd_ls));
                close(fh);
             }
          }
       }
       // DPrn(0,0,"uY=%d,Y=%d,uM=%d,M=%d,uD=%d,D=%d,uh=%d,h=%d,um=%d,m=%d,us=%d,s=%d,un=%d,n=%d,uc=%d,c=%d\n",
       //      upd_ls.yea,yeaC,upd_ls.mon,monC,upd_ls.day,dayC,
       //      upd_ls.hou,houC,upd_ls.min,minC,upd_ls.sec,secC,
       //      upd_ls.no, no,  upd_ls.crc,crc);
       // �᫨ ����祭�� ���祭�� � ����஫��� ࠢ�� ����ᠭ��
       // ��᫠�� �������� ����஫���� � ���室� � ᫥���饩 �����:
       if(CB[DBUFlg+12].v==1||CB[DBUFlg+12].v==2){
       // DPrn(0,0,"Set answer flag.\n");
          // ��।��� �� ����� DBUFlg+10 ���祭�� 2 CB[DBUFlg+10].v=2 � 
          // ��⠭����� 䫠� ��।�� ���祭�� CBSF[DBUFlg+10]=2
          CBSN=DBUFlg+10; CB[CBSN].v=2; CBSF[CBSN]=2;
       }
    }
    return;
}
//---------------------------------------------------------------------------

