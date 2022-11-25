// coding: cp866
//---------------------------------------------------------------------------
// �ࠢ����� ��ᬮ�஬
vd FV_Set(_i md){
   // �᫨ �� ��।����� �� ������ 䠩�� ��� ��ᬮ�� - ��室��
   if (HFVW<1){ return; }
   // ALT+TAB, Alt+N - ��४��祭�� ����� ������ ��⨢���� �࠭�
   if (md==1){
       if (FVA+1<HFVW){ FVA++; }
       else           { FVA=0; }
       FVCode=1; ViewFile(FVA,1);
   }
   // ALT+D - ���� ���� ��� ��ᬮ�� 䠩�� ����
   if (md==2 && LFVW[FVA].fd>0){
       strcpy(GS.ES,LFVW[FVA].dt);
       GS.LS =10;
       GS.RX1=GS.RDXV1;
       GS.RY1=GS.RDYV1;
       GS.RX2=GS.RDXV2;
       GS.RY2=GS.RDYV2;
       GS.TX =GS.TDXV;
       GS.TY =GS.TDYV;
       GS.W  =200;
       GS.sn =LFVW[FVA].sn;
       GS.cc =LFVW[FVA].cc;
       GS.cs =LFVW[FVA].cd;
       GS.cg =LFVW[FVA].cg;
       strcpy(GS.sf,"00.00.0000");
       LGET_el=0;
       GetStr(0);
       GS.FL =1;
       GS.fe =3;
   }
   // ALT+G - ���� ��������� ��ப�
   if (md==4 && LFVW[FVA].fd>0){
       if (VW[FVA].fpp==0){ VW[FVA].fpp=0; }
       else               { VW[FVA].fpp=VW[FVA].fps+1; }
       GS.fe=5;
   }
   // ALT+T - ����� ���� ��������� ��ப�
   if (md==5 && LFVW[FVA].fd>0){
       if (VW[FVA].fpp==0){ VW[FVA].fpp=VW[FVA].fsz; }
       else               { VW[FVA].fpp=VW[FVA].fps-1; }
       GS.fe=6;
   }
   return;
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// ��ᬮ�� 䠩���
vd ViewFile(_i nm, _i md){
   _i  a=0, cl=0, fnd=0, fs=0, x=0, y=0, w[2], r=0, rr=0, rb=0, nBuf=0, hff=0;
   _f  cp[2];
   if(((HFVW<1)||(FVCode<1))&&md>0){ return; };
   RKDraw=1; FLSEL=1;
   //------------------------------------------------------------------------
   // ����⨥ 䠩��
   if (md==0){
      if ((strcasecmp(LFVW[nm].fn,FNRep))!=0){
         if(access(LFVW[nm].fn,0)<0){
            sprintf(ErrSTR,SMSG[142],LFVW[nm].fn);
            sprintf(SMSG[MMSG],SMSG[143],LFVW[nm].fn);
            DPrn(0,0,"%s\n",SMSG[MMSG]);
            ErrINI=1; STAT[1].cnt=10; VW[nm].ffl=0; return;
         }
         VW[nm].fhd=dbSelectAvail(); dbInit(VW[nm].fhd);
         if(dbOpen(VW[nm].fhd,LFVW[nm].fn)<0){
            sprintf(ErrSTR,SMSG[144],LFVW[nm].fn);
            sprintf(SMSG[MMSG],SMSG[145],LFVW[nm].fn);
            DPrn(0,0,"%s\n",SMSG[MMSG]);
            ErrINI=1; STAT[1].cnt=10; VW[nm].ffl=0; dbClear(VW[nm].fhd); 
            return;
         }
         VW[nm].ffl=1;
      }  else  {
         VW[nm].fhd=REPOut;
         VW[nm].ffl=2;
      }
      sprintf(STAT[1].str,SMSG[146],LFVW[nm].fn);
      // ��।����� ����� ���� ��� ������� ���� �⮡ࠦ����
      VW[nm].hhs=text_height(dFnt[LFVW[nm].sn].dat)+2;
      // ��।����� ������⢮ ��ப � ���� �⮡ࠦ����
      VW[nm].hst=(_i)(divi(LFVW[nm].H,(VW[nm].hhs+1)));
      if(VW[nm].hst<1){ VW[nm].hst=1; }
      // ��।����� ��砫�� ��⠭���� 梥�
      STR_cs=LFVW[nm].cs;
      STR_cg=LFVW[nm].cg;
      // ������⢮ ����ᥩ � 䠩�� ��ᬮ��
      VW[nm].fsz=dbRecCount(VW[nm].fhd);
      // ��३� �� ����� ������ � ����� ��
      dbGoFirst(VW[nm].fhd); VW[nm].fps=0; dbRecRead(VW[nm].fhd);
      // ��砫쭠� ��⠭���� 䫠��� ��ᬮ��
      md=1; fs=1; VW[nm].fcp=0; VW[nm].fpp=0;
   }
   //------------------------------------------------------------------------

   //------------------------------------------------------------------------
   // ���� ��ப� � 䠩��
   if((GS.fe==5 || GS.fe==6) && nm==FVA ){
      // �஢�ઠ ������ ��ப� ��� ���᪠
      if (strlen(LSTT[LFVW[nm].ws].ES)<1){
          sprintf(STAT[1].str,SMSG[147]);
          GS.fe=0; STAT[1].cnt=5; return;
      }
      // ��אַ� ����
      if (GS.fe==5){
          sprintf(STAT[1].str,SMSG[148]); STAT[1].cnt=5;
          for(a=VW[nm].fpp,r=0;a<VW[nm].fsz;a++,r++){
              dbGoRec(VW[nm].fhd,a); dbRecRead(VW[nm].fhd);
              strcpy(VW[nm].fbr, dbFieldGetStr(VW[nm].fhd,-1,"STRING",0));
              if(((strstr(VW[nm].fbr, LSTT[LFVW[nm].ws].ES))!=NULL) ||
                 ((strstr(dbFieldGetStr(VW[nm].fhd,-1,"FLG",0),LSTT[LFVW[nm].ws].ES))!=NULL) ){
                   fnd=5; break; 
              }
              if (r>MSTS){ VW[nm].fpp=a; return; }
      }; };
      // ����� ����
      if (GS.fe==6){
          sprintf(STAT[1].str,SMSG[149]); STAT[1].cnt=5;
          for(a=VW[nm].fpp,r=0;a>0;a--,r++){
              dbGoRec(VW[nm].fhd,a); dbRecRead(VW[nm].fhd);
              strcpy(VW[nm].fbr, dbFieldGetStr(VW[nm].fhd,-1,"STRING",0));
              if(((strstr(VW[nm].fbr, LSTT[LFVW[nm].ws].ES ))!=NULL) || 
                 ((strstr(dbFieldGetStr(VW[nm].fhd,-1,"FLG",0),LSTT[LFVW[nm].ws].ES))!=NULL) ){
                   fnd=6; break; 
              }
              if (r>MSTS){ VW[nm].fpp=a; return; }
      }; };
      if (fnd>0){
          sprintf(STAT[1].str,SMSG[150]); 
          VW[nm].fps=a; VW[nm].fpp=a;
      } else {
          sprintf(STAT[1].str,SMSG[151]); 
      }
      STAT[1].cnt=5; GS.fe=0; md=1;
   }
   //------------------------------------------------------------------------

   //------------------------------------------------------------------------
   // �᫨ �� 䠩� ᮮ�饭��
   if(LFVW[nm].fd>0 && FVA==nm){
      // �᫨ � 䠩� ᮮ�饭�� ��������� ����� ᮮ�饭��:
      // ��᫥���� ������ ������ �� End - ������� 䠩� � ⥪�饩 ����樨
      if(FVCodL!=KEY_END){ md=1; }
      else {
      // ��᫥���� ������ ������ End - ��⠭����� ��� ������ KEY_END
         if(FVCode==0||FVCode==1){ FVCode=KEY_END; }
         // �᫨ ��⠭����� 䫠� ���������� ����� - ���� ���
         if(FVW_Add>0){ FVW_Add=0; FVCode=KEY_END; }
      }
   }
   //------------------------------------------------------------------------

   //------------------------------------------------------------------------
   // PgDn (+)
   if (FVCode==KEY_PGDN && VW[nm].ffl>0 && FVA==nm){
       VW[nm].fsz=dbRecCount(VW[nm].fhd);
       if (VW[nm].fsz>VW[nm].fps+VW[nm].hst+1){
           VW[nm].fps=VW[nm].fps+VW[nm].hst+1;
       };  md=1;
   }
   //------------------------------------------------------------------------

   //------------------------------------------------------------------------
   // End (+)
   if (FVCode==KEY_END && VW[nm].ffl>0 && FVA==nm){
      VW[nm].fsz=dbRecCount(VW[nm].fhd);
      if (VW[nm].fsz>VW[nm].fps+VW[nm].hst+1){
          if (VW[nm].fsz-VW[nm].hst>0){ VW[nm].fps=VW[nm].fsz-VW[nm].hst; }
          else                        { VW[nm].fps=0; }
      };  md=1; 
   }
   //------------------------------------------------------------------------

   //------------------------------------------------------------------------
   // PgUp
   if (FVCode==KEY_PGUP && VW[nm].ffl>0 && FVA==nm){
       if (VW[nm].fps-VW[nm].hst-1>0){ VW[nm].fps=VW[nm].fps-VW[nm].hst-1; }
       else                          { VW[nm].fps=0; }
       md=1;
   }
   //------------------------------------------------------------------------

   //------------------------------------------------------------------------
   // Home (+)
   if (FVCode==KEY_HOME && VW[nm].ffl>0 && FVA==nm){
       VW[nm].fps=0; md=1;
   }
   //------------------------------------------------------------------------


   // �᫨ ����� ���� �� ������ �ࠢ����� ��ᬮ�஬
   // 䠩� ����� ��� ��ᬮ��, ���� ��⨢�� � 䠩��� ��娢�
   if(((FVCode==KEY_HOME)||(FVCode==KEY_PGUP)||
       (FVCode==KEY_END )||(FVCode==KEY_PGDN))&&
       (FVA==nm)&&(VW[FVA].ffl>0)){ md=1; FVCodL=FVCode; }


   //------------------------------------------------------------------------
   // �뢮� 䠩�� �� �࠭
   if (md==1 && VW[nm].ffl>0 && FVA==nm ){
      // ������⢮ ����ᥩ � ��
      VW[nm].fsz=dbRecCount(VW[nm].fhd);
      // ����� ⥪�饩 ��࠭���
      VW[nm].fcp=divi(VW[nm].fps,VW[nm].hst);
      // ������⢮ ��࠭�� � 䠩��
      VW[nm].fhp=(_i)ceil(divf(VW[nm].fsz,VW[nm].hst));
      // ࠧ��� ����㭪�
      VW[nm].fcz=divf(LFVW[nm].H,VW[nm].fhp);
      //---------------------------------------------------------------------
      // ���ᮢ��� ��אַ㣮�쭨� �뢮�� 䠩��
      rectfill(scrsav, LFVW[nm].X,
                       LFVW[nm].Y,
                       LFVW[nm].X+ LFVW[nm].W,
                       LFVW[nm].Y+ LFVW[nm].H,
                       LFVW[nm].cg );
      //---------------------------------------------------------------------
      // �뢮� ��ப �� �࠭
      x=(LFVW[nm].X+LFVW[nm].L);
      y=(LFVW[nm].Y+5);
      // ��।����� ������ ��ப� ����
      w[0]=text_length(dFnt[LFVW[nm].sn].dat,"77.77.7777");
      // ��⠭����� 㪠��⥫� �� ������ � 䠩�� ��
      dbGoRec(VW[nm].fhd,VW[nm].fps);
      FVCURREC =VW[nm].fps;                 // ����� ⥪�饩 ��ࢮ� �����
      FVCURFNH =VW[nm].hhs;                 // ���� ���� �⮡ࠦ 䠩��
      FVCURHST =VW[nm].hst;                 // ������⢮ ��ப � ���� 䠩��
      FVSELDBA =VW[nm].fhd;                 // ����� ������ �� ��⨢ 䠩��
      FVCURPAGE=VW[nm].fcp;                 // ����� ⥪�饩 ��࠭���
      FVLSTPAGE=VW[nm].fhp-1;               // ��饥 ������⢮ ��࠭��
      //---------------------------------------------------------------------
      // ����� ᢮����� ���� ��, ������ ��� � ���樠����஢���
      nBuf=dbBFBufferAvail(); dbBFClear(nBuf); dbBFInit(nBuf);
      // ������� ���-�� ����ᥩ � ���� � ��⠭����� 㪠��⥫� �� ������ N0
      dbBFRecInBuf(VW[nm].fhd,nBuf); dbBFGoFirst(nBuf);
      // ������ ����� �� 䠩�� �� � ����
      rr=dbBFRead(VW[nm].fhd,nBuf,VW[nm].hst+1);
      // ��⠭����� 梥� � ���� �뢮�� ��ப
      STR_cs=LFVW[nm].cs;
      STR_cg=LFVW[nm].cg;
      STR_sn=LFVW[nm].sn;
      // �஢���� �ࠢ��쭮��� �������� �����
      if(LFVW[nm].f1>1){ LFVW[nm].f1=2; }
      if(LFVW[nm].f1<1){ LFVW[nm].f1=0; }
      if(LFVW[nm].f2>1){ LFVW[nm].f2=2; }
      if(LFVW[nm].f2<1){ LFVW[nm].f2=0; }
      cl=dbFieldGetNum(VW[nm].fhd,0,"COL");
      // �뢥�� ��ப� �� �࠭
      for(a=VW[nm].fps,rb=0;; a++,rb++){
          if(rb>rr){ md=20; break; }
          // ��।��� ����� �� ⥪�饩 ����� ���� � ���� ����� ��
          dbBFBufCopyRec(VW[nm].fhd,nBuf);
          // ��ନ஢��� ��ப� �� �������� ����� ��� �뢮�� �� �࠭
          if(LFVW[nm].f1>0){
             strcpy (FVFLD1,dbFieldGetStr(VW[nm].fhd,LFVW[nm].f1-1,0,0));
          } else {
             strcpy (FVFLD1,"");
          }
          if(LFVW[nm].f2>0){
             strcpy (FVFLD2,dbFieldGetStr(VW[nm].fhd,LFVW[nm].f2-1,0,0));
          } else {
             strcpy (FVFLD2,"");
          }
          // �᫨ ����� 䨫��� � �᫮��� 䨫���樨 �� �믮������� -
          // ��३� � ᫥���饩 �����
          if((FSON==0 && FEON==0) ||
             ((strstr(FVFLD1, LSTT[LFVW[nm].ws].ES ))!=NULL && FSON>0) ||
             ((strstr(FVFLD2, LSTT[LFVW[nm].ws].ES ))!=NULL && FEON>0) ){
             //
             if(LFVW[nm].f1<1&&LFVW[nm].f2<1){
                strcpy (FVFLD1,dbFieldGetStr(VW[nm].fhd,-1,"STRING",0));
                strcpy (FVFLD2,"");
             }
             if(cl>=0){ 
                strcpy (FVCOLR,dbFieldGetStr(VW[nm].fhd,-1,"COL",0));
             } else {
                strcpy (FVCOLR,"");
             }
             // �뢥�� ��ப� �� ⥪�饩 ����� �� �࠭
             sprintf(FVSSTR,"%s%s%s",FVCOLR,FVFLD1,FVFLD2);
             STR_Out(FVSSTR,STR_sn,x,y, LFVW[nm].W-LFVW[nm].R,LFVW[nm].vw);
          }
          // ��������� ��� ᫥���饩 ��ப�
          y=y+VW[nm].hhs;
          // ��३� � ᫥���饩 ����� � ����
          dbBFGoSkip(nBuf,1); 
          // �᫨ �� ��᫥���� ������ � ����
          if(dbBFEof(nBuf)>0){ md=21; break; }

          // �஢�ન ��室� �� �࠭��� ����ᥩ
          if(a>=VW[nm].fsz){ 
             VW[nm].fcp=divf(VW[nm].fsz,VW[nm].hst); md=22; break; }
          if(a>=VW[nm].fps+VW[nm].hst){ md=23; break; }
      }
//    sprintf(STAT[1].str,"rr=%d   rb=%d  md=%d",rr,rb,md); STAT[1].cnt=10;
//    sprintf(ErrSTR,"FVCode=%d FVCodL=%d",FVCode,FVCodL); ErrINI=1;

      // ������ ����
      dbBFClear(nBuf);
      //---------------------------------------------------------------------
      // 㪠��⥫� ⥪�饣� ��������� � 䠩�� (����� ��砫쭮� �����)
      if (LFVW[nm].ps>0){
          if (VW[nm].fsz>1){
              // ��砫쭠� ���न��� ����㭪� ���������
              cp[0]=VW[nm].fcp*VW[nm].fcz;
              // ����筠�  ���न��� ����㭪� ���������
              cp[1]=cp[0]+VW[nm].fcz;
              // �஢�ઠ ��室� �� �࠭��� �⮡ࠦ���� 䠩��
              if(cp[1]>LFVW[nm].H){ cp[1]=LFVW[nm].H; }
              // �뢮� ����㭪� ��������� �� �࠭
              rectfill(scrsav, LFVW[nm].X+ LFVW[nm].W-5,
                               LFVW[nm].Y+ cp[0],
                               LFVW[nm].X+ LFVW[nm].W,
                               LFVW[nm].Y+ cp[1],
                               LFVW[nm].cp );
          }
      }
      //---------------------------------------------------------------------
      // �뢮� ���� � ��ப� ���᪠
      if (LFVW[nm].fd>0){
          // ������� �뢮� ��ப� ���᪠
          hff=text_height(dFnt[ LSTT[LFVW[nm].ws].sn ].dat)+1;
          rectfill(scrsav, LSTT[LFVW[nm].ws].X,
                           LSTT[LFVW[nm].ws].Y,
                           LSTT[LFVW[nm].ws].X+LSTT[LFVW[nm].ws].W,
                           LSTT[LFVW[nm].ws].Y+hff,
                           LSTT[LFVW[nm].ws].cg );
          text_mode(LFVW[nm].cg);
          // �뢮� ����
          textout(scrsav,dFnt[LFVW[nm].sn].dat,LFVW[nm].dt,
                           LFVW[nm].X+LFVW[nm].L,
                           LFVW[nm].Y+LFVW[nm].H+VW[nm].hhs*0.4+LFVW[nm].yt,
                           LFVW[nm].cd);
          // �뢮� ��ப� ���᪠
          textout(scrsav,dFnt[LFVW[nm].sn].dat,LSTT[LFVW[nm].ws].ES,
                           LSTT[LFVW[nm].ws].X,
                           LSTT[LFVW[nm].ws].Y,
                           LFVW[nm].cd);
          if (FVA==nm){
              // ��������� ���न���� �뢮�� ����
              GS.RDXV1=LFVW[nm].X+LFVW[nm].L;
              GS.RDYV1=LFVW[nm].Y+LFVW[nm].H+VW[nm].hhs*0.3+1;
              GS.RDXV2=LFVW[nm].X+LFVW[nm].L+w[0]+5;
              GS.RDYV2=LFVW[nm].Y+LFVW[nm].H+VW[nm].hhs*1.3-1;
              GS.TDXV =LFVW[nm].X+LFVW[nm].L;
              GS.TDYV =LFVW[nm].Y+LFVW[nm].H+VW[nm].hhs*0.4+LFVW[nm].yt;
              // ��������� ���न���� �뢮�� ���᪠
              GS.RFXV1=LFVW[nm].X+LSTT[LFVW[nm].ws].X;
              GS.RFYV1=LFVW[nm].Y+LFVW[nm].H+VW[nm].hhs*0.3+1;
              GS.RFXV2=LFVW[nm].X+LSTT[LFVW[nm].ws].W;
              GS.RFYV2=LFVW[nm].Y+LFVW[nm].H+VW[nm].hhs*1.3-1;
              GS.TFXV =LFVW[nm].X+LSTT[LFVW[nm].ws].X;
              GS.TFYV =LFVW[nm].Y+LFVW[nm].H+VW[nm].hhs*0.4;
          }
      }
      //---------------------------------------------------------------------
   }
   //------------------------------------------------------------------------
   if(nm==FVA&&FVCode>0){ FVCode=0; }
   if(FVW_Add==1){ FVW_Add=0; }
   return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//  ������ ���祭�� � 㪠������ ���� ⥪�饩 ����� �� ��⨢ 䠩�� ��ᬮ�� 
vd  FVDBFLDWR(int ndef){
    _l nreco=0;
    // �᫨ �� �뫮 ��।����� ����⢨� ��� ࠡ��� � ��
    if(HFVB<=1){ return; }
    // �᫨ ����� ��।������ ����� ���� ��।��������
    if(ndef>=HFVB){ return; }
    // �᫨ 㪠������� ���� � �� ���- ���
    if(dbFieldGetNum(FVSELDBA, 0,LFVB[ndef].fld)<0){ return; }
    // ��������� ����� ⥪�饩 �����
    nreco= dbRecCur (FVSELDBA);
    // ��३� �� �㦭�� ������
    if(dbGoRec(FVSELDBA,FVDBCREC)>=0){
       // �������� ���祭�� � 㪠������ ���� �� ��������
       dbFieldPutStr(FVSELDBA,-1,LFVB[ndef].fld,LFVB[ndef].val,DBWDISK);
    }
    // ��३� �� ������ � ���������� ����஬
    dbGoRec(FVSELDBA,nreco);
    return;
}
//---------------------------------------------------------------------------
