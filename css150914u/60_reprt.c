// coding: cp866
//---------------------------------------------------------------------------
//  ������� ���⮢
vd  Report(_i md){
    _i  a=0, b=0, d=0, f=0, e=0, r=(_i)CREP;
    _l  h=0, c=0;
    uc  s[SLEN];
    if (r>=MREP-1){ return; }

    //-----------------------------------------------------------------------
    //  ������� ����
    if (md==1){
        for(a=0;a<PBuf;a++){ BRep[a]=0; }
        for(a=0;a<MSREP;a++){
            for(b=0;b<SLEN;b++){ LREP[r].ss[a][b]=0; }
        }
        // �᫨ 㪠����� 䠩� ��� �� ��।���� - ���
        if (LREP[r].ff[0]==0){ return; }
        // �⥭�� 䠩�� ����
        if ((FRDR=open(LREP[r].ff,vktRDO))>0){
            h=read(FRDR,&BRep,PBuf); close(FRDR);
        } else {
            sprintf(SMSG[MMSG],SMSG[153],LREP[r].ff);
            DPrn(0,0,"%s\n",SMSG[MMSG]);
            sprintf(STAT[1].str,SMSG[154],LREP[r].ff);
            STAT[1].cnt=5; return;
        }
        // ࠧ��� ��⠭���� ���� �� ��ப��
        for(a=0;a<SLEN;a++){ s[a]=0; }
        for(a=0,b=0,c=0; c<h; c++){
            // �᫨ �����㦨�� � ��ப� 㪠����� ����� ��६�����
            if(BRep[c]=='$'){
               for(d=1;d<SLEN-1;d++){
                   if(BRep[c+d]<48||BRep[c+d]>57){ break; }
                   s[d-1]=BRep[c+d]; s[d]=0;
               }
               e=atoi(s);
               strcat(LREP[r].ss[a],LVAL[e].str);
               c=c+d-1; b=strlen(LREP[r].ss[a]); continue;
            }
            // �᫨ �� �����㦥� ����� ��ப�
            if(BRep[c]==13){ continue; }
            if(BRep[c]==10||BRep[c]==0){
               LREP[r].ss[a][b]=0; a++; b=0; 
            } else {
            // ������ ��ப ����
               LREP[r].ss[a][b]=BRep[c]; if(b<SLEN-1){ b++; }
            }
        }
        // ������� ������ 䠩�� ��ᬮ��
        for(f=0;f<HFVW;f++){
            if(VW[f].ffl==1){ dbClose(VW[f].fhd); }; VW[f].ffl=0;
        }
        // ᮧ����� ⠡���� ����
        if (access(LREP[r].fo,0)>=0){ remove(LREP[r].fo); }
        FRMOut=dbSelectAvail(); dbInit(FRMOut);
        dbFieldFill(FRMOut,0,"STRING",'C',100,0);
        if(dbCreate(FRMOut,LREP[r].fo)<0){
           sprintf(SMSG[MMSG],SMSG[155],LREP[r].fo);
           DPrn(0,0,"%s\n",SMSG[MMSG]);
           dbClear(FRMOut); return;
        }
        if(dbOpen(FRMOut,LREP[r].fo)<0){
           sprintf(SMSG[MMSG],SMSG[156],LREP[r].fo);
           DPrn(0,0,"%s\n",SMSG[MMSG]);
           dbClear(FRMOut); return;
        }
        for(b=0;b<a;b++){
            dbRecAdd (FRMOut);
            dbFieldPutStr(FRMOut,0,0,LREP[r].ss[b],DBWBUFF);
            dbRecWrite(FRMOut);
        }
        dbClose(FRMOut);
        // �������(��८�����) 䠩�� ��ᬮ��
        for(f=0;f<HFVW;f++){
            if(LFVW[f].vw>0){ ViewFile(f,0); }
        }
    }
    //-----------------------------------------------------------------------
    return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//  ����� 䠩���
vd  PrintFile(_i md){
    _i  a=0, b=0, e=0;

    //-----------------------------------------------------------------------
    if (md==1){
        // �᫨ ����� 㦥 ����- ��⠭����� �����
        if(FLGP>0){
           if(strlen(PRN_Name)>0){ WPRN_Stop(); }
           sprintf(STAT[1].str,SMSG[157]); STAT[1].cnt=10; FLGP=0;
           return;
        }
        // �᫨ ��� 䠩��� ��� ��ᬮ��
        if(HFVW<1){ return; }
        // ������� ������⢮ ��ப ��� ���� ��� ��⨢���� 䠩��
        HPRN=LFVW[FVA].spp; if(HPRN<0){ HPRN=0; }; if(HPRN>=MPRN){ HPRN=MPRN-1; }
        // ��३� �� ⥪���� ������ � 䠩�� ��ᬮ��
        dbGoRec(VW[FVA].fhd,VW[FVA].fps);
        // ��।��� ��ப� �� 䠩�� � ���ᨢ ����
        for(a=0;a<HPRN;a++){
            dbRecRead(VW[FVA].fhd);
            strcpy(FPRN[a],dbFieldGetStr(VW[FVA].fhd,0,0,0));
            strcat(FPRN[a],"\n");
            dbGoSkip (VW[FVA].fhd,1);
            if(dbEof (VW[FVA].fhd)>0){ a++; break; }
        }
        // ��३� �� ⥪���� ������ � 䠩�� ��ᬮ��
        dbGoRec(VW[FVA].fhd,VW[FVA].fps);
        // ��⠭����� �����न�� 䫠�� � ���祭��
        FPRN[a][0]=0; HPRN=a; CPRN=0; FLGP=1;
        DPrn(DEBPRN,0,"Init=[%s], Ornt=[%s], Pitch=[%s], CharH=[%s], CharB=[%s]\n",
                 PRN_Init,PRN_Ornt,PRN_Pitc,PRN_ChrH,PRN_ChrB);
        // �ந��樠����஢��� �ਭ��
        if(SPRN.port==1){
          if(strlen(PRN_Init)>0){ PrnStr(PRN_Init,0); }
          if(strlen(PRN_Ornt)>0){ PrnStr(PRN_Ornt,0); }
          if(strlen(PRN_Pitc)>0){ PrnStr(PRN_Pitc,0); }
          if(strlen(PRN_ChrH)>0){ PrnStr(PRN_ChrH,0); }
          if(strlen(PRN_ChrB)>0){ PrnStr(PRN_ChrB,0); }
          if(strlen(PRN_LnSp)>0){ PrnStr(PRN_LnSp,0); }
          if(strlen(PRN_Spac)>0){ PrnStr(PRN_Spac,0); }
        }
        sprintf(STAT[1].str,SMSG[158]);
        if(strlen(PRN_Name)>0){
           e=WPRN_GetQueue();
           if(e>0){
             if(e==101){ sprintf(STAT[1].str,"ERROR: get memory size for PRINTER_INFO. "); }
             if(e==102){ sprintf(STAT[1].str,"ERROR: allocated memory for PRINTER_INFO."); }
             if(e==103){ sprintf(STAT[1].str,"ERROR: get PRINTERINFO."); }
             if(e==104){ sprintf(STAT[1].str,"ERROR: get memory size for JOB_INFO. "); }
             if(e==105){ sprintf(STAT[1].str,"ERROR: allocated memory for JOB_INFO."); }
             if(e==106){ sprintf(STAT[1].str,"ERROR: get JOB_INFO."); }
             if(e==107){ sprintf(STAT[1].str,"ERROR: OpenPrinter."); }
             if(e==108){ sprintf(STAT[1].str,"ERROR: printer paper,door or other."); }
             if(e==100){ sprintf(STAT[1].str,"ERROR: printer status."); }
             if(e <100){ sprintf(STAT[1].str,"ERROR: printer queue [%d].",e); }
             STAT[1].cnt=20; FLGP=0; PRN_Stat=0;
           } else {
             WPRN_Start();
           }
        } 
    }
    //-----------------------------------------------------------------------
    if (md==2){
        if(FLGP==0){ return; }
        // �஢�ઠ ����� �ਭ��
        if(SPRN.port==1){ e=PrnStat(); }
        if(e>0){
           if(e==1){ sprintf(STAT[1].str,SMSG[159]); }
           if(e==4){ sprintf(STAT[1].str,SMSG[160]); }
           if(e==5){ sprintf(STAT[1].str,SMSG[161]); }
           if(e==6){ sprintf(STAT[1].str,SMSG[162]); }
           if(e==8){ sprintf(STAT[1].str,SMSG[163]); }
        } else {
           e=strlen(FPRN[CPRN]);
           for(a=0,b=0;a<e;a++){
               if(FPRN[CPRN][a]== 10){ PRN_Cmd[b]=0; break; }
               if(FPRN[CPRN][a]==126||FPRN[CPRN][a]== 94||
                  FPRN[CPRN][a]== 96||FPRN[CPRN][a]== 16||
                  FPRN[CPRN][a]== 17||FPRN[CPRN][a]== 18||
                  FPRN[CPRN][a]== 24||FPRN[CPRN][a]== 25||
                  FPRN[CPRN][a]== 29||FPRN[CPRN][a]== 30||
                  FPRN[CPRN][a]==253||FPRN[CPRN][a]== 31){ a=a+3; continue; }
               PRN_Cmd[b]=FPRN[CPRN][a]; b++;
           }
           if(SPRN.port==1){
              if(strlen(PRN_Cmd )>0){ PrnStr(PRN_Cmd ,0); }
              if(strlen(PRN_Ends)>0){ PrnStr(PRN_Ends,0); }
           } else {
              if(strlen(PRN_Cmd )>0){ WPRN_Write(); }
           }
           CPRN++;
           sprintf(STAT[1].str,SMSG[164],CPRN,HPRN);
        }
        // �᫨ �� ��᫥���� ��ப� ���� - ����� 䫠� ����
        if (FPRN[CPRN][0]==0||CPRN>=HPRN){ 
            sprintf(STAT[1].str,SMSG[165],CPRN);
            WPRN_Stop ();
            FLGP=0;
        };  STAT[1].cnt=5;
    }
    //-----------------------------------------------------------------------
    return;
}
//---------------------------------------------------------------------------



//   dwNeeded=DocumentProperties(NULL,hdcPrint,PRN_Name,NULL,NULL,0);
//   if(pDevMode)HeapFree(GetProcessHeap(),0,pDevMode);
//   pDevMode=(LPDEVMODE)HeapAlloc(GetProcessHeap(),HEAP_NO_SERIALIZE,dwNeeded);
//   DocumentProperties(NULL,hdcPrint,PRN_Name,pDevMode,NULL,DM_OUT_BUFFER);
//   pDevMode->dmOrientation = 0;
//   pDevMode->dmOrientation = 1;
//   pDevMode->dmOrientation = 2;
//   pDevMode->dmFields      = DM_ORIENTATION;
//   DocumentProperties(NULL,hdcPrint,PRN_Name,pDevMode,pDevMode,DM_IN_BUFFER|DM_OUT_BUFFER);

//---------------------------------------------------------------------------
//  �᭮���� �㭪�� ���� �࠭�
DWORD WINAPI mPrintScreen(void){
   int a=0, cl=0, xs=0, xd=0, ys=0, yd=0;
   PRN_Stat=1;
   PRN_ScrS=1;
   printf("Start: mPrintScreen.\n");
   sprintf(STAT[1].str,"���� ��ࠡ�⪠ ���� �࠭�.");
   STAT[1].cnt=20;
   if(strlen(PRN_Name )>0){
      if((WPRN_GetQueue())>0){
        sprintf(STAT[1].str,"�訡�� ���� �࠭�."); 
      } else {
        WPRN_Start();
        get_pallete(pal);

        scrprn=create_bitmap_ex(DCOLOR,YMAX,XMAX); clear_to_color(scrprn,0);
        for(a=0,xs=0,xd=0,ys=767,yd=0;a<1024*768;a++){
            cl=getpixel(scrout,xs,ys); 
            ys--; if(ys<0  ){ ys=767; xs++; if(xs>1023) xs=0; }
            putpixel(scrprn,xd,yd,cl);
            xd++; if(xd>767){ xd=0; if(yd<1024) yd++; }
        }
        save_bitmap("scrprn.pcx",scrprn,pal);

        stretch_blit_to_hdc(scrprn,hdcPrint, SPRN.X,SPRN.Y,
                                             SPRN.X+SPRN.W, SPRN.Y+SPRN.H, 
                                             PRN_X, PRN_Y, 
                                            (SPRN.X+SPRN.W)*4,(SPRN.Y+SPRN.H)*4);

        WPRN_Stop ();
        destroy_bitmap(scrprn);
        sprintf(STAT[1].str,"��ࠡ�⪠ ���� �࠭� �����襭�.");
   }  }
   FLGP=0; PRN_Stat=0; PRN_ScrS=0;
   return(0);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  �맮� ���� ⥪�饣� �࠭�
vd  PrintScrn(_i md){
    if(PRN_ScrS==0){
       thC=CreateThread(NULL,0,
                   (LPTHREAD_START_ROUTINE) &mPrintScreen, 0, 0,
                   (PDWORD) &thPSC);
    } else {
       sprintf(STAT[1].str,"����ୠ� ��ࠡ�⪠ ���� �࠭� �⪫�����.");
       STAT[1].cnt=5;
    }
    return;
}
//---------------------------------------------------------------------------
