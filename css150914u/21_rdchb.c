// coding: cp866
//---------------------------------------------------------------------------
// �⥭�� ���� �������
_i Read_CHB(vd){
   _i  lc=1,a=0,b=0,j=0,i=0,n=0; // �६���� ��६����
   HCB=0;                        // ������⢮ ��⠭��� �������
   HIC=0;                        // ������⢮ �����뢠���� �������� 䠩���
   strcpy(FNCfg,FNCHB);          // ��।����� ��� 䠩�� ���䨣��樨
   ClsFlg();                     // ���⪠ 䫠���
   strcpy(lcsok,"vmaied");       // ࠧ�襭�� ���� �⥭�� ���䨣��樨
   HCSD=0;
   HCSE=0;
   HMSGCH=0;
   HAUTHC=0;

   //------------------------------------------------------------------------
   for(;;){
       Init_CFG(1);
       for(a=0;a<MCYC;a++){
           if (Init_CFG(2)){ break; }
           for(lca=0;lca<FHCfg;lca++){
               lc=Load_CFG('0','i',(CVT*) &ZERO );
               //------------------------------------------------------------
               // ���ᠭ�� ��६�����
               if (lcsec =='v'){
                   if (lcs== 1 ){
                        lc=Load_CFG('v','i',(CVT*) &CCB ); if(CCB>=MCH){ CCB=MCH-1; };
                   }
                   if (lcs== 2 ){ lc=Load_CFG('v','c',(CVT*) &CB[CCB].nm ); }
                   if (lcs== 3 ){ lc=Load_CFG('v','i',(CVT*) &CB[CCB].ar ); }
                   if (lcs== 4 ){ lc=Load_CFG('v','i',(CVT*) &CB[CCB].dt ); }
                   if (lcs== 5 ){ lc=Load_CFG('v','i',(CVT*) &CB[CCB].dn ); }
                   if (lcs== 6 ){ lc=Load_CFG('v','i',(CVT*) &CB[CCB].dd ); }
                   if (lcs== 7 ){ lc=Load_CFG('v','i',(CVT*) &CB[CCB].t  ); }
                   if (lcs== 8 ){ lc=Load_CFG('v','i',(CVT*) &CB[CCB].dc ); }
                   if (lcs== 9 ){ lc=Load_CFG('v','f',(CVT*) &CB[CCB].l  ); }
                   if (lcs==10 ){ lc=Load_CFG('v','f',(CVT*) &CB[CCB].h  ); }
                   if (lcs==11 ){ lc=Load_CFG('v','c',(CVT*) &CB[CCB].rm ); }
                   if (lc ==10 ){
                       if (CCB>=HCB){ HCB=CCB+1; }
                       if (HCB>=MCH){ HCB=MCH-1; }
                   }
               }
               //------------------------------------------------------------
               // ����� ������� ᮮ�饭��
               if (lcsec =='m'){
                   if(lcs== HMSGCH+1){ 
                      lc  = Load_CFG('m','i',(CVT*) &MSGNUMCH[HMSGCH]); 
                   }
                   if(lc == 5 || lc == 10 ){
                      if(HMSGCH<MAXMCH){ HMSGCH++; }
                   }
               }
               //------------------------------------------------------------
               // ����� ������� ����� ���ਧ�樨
               if (lcsec =='a'){
                   if(lcs== HAUTHC+1){ 
                      lc  = Load_CFG('a','i',(CVT*) &AUTHNMCH[HAUTHC]); 
                   }
                   if(lc == 5 || lc == 10 ){
                      if(HAUTHC<MAXACH){ HAUTHC++; }
                   }
               }
               //------------------------------------------------------------
               // 䠩� ����祭��
               if (lcsec =='i'){
                   if (lcs== 1 ){ lc=Load_CFG('i', 'c',(CVT*) &IC[HIC] ); }
                   if (lc ==10 ){
                      if (HIC<MIC-1){ HIC++; };
                   }
               }
               //------------------------------------------------------------
               // ����� �⠭権 � ����� ����饭� ������
               if (lcsec =='d'){
//                    DPrn(35,0,"1 Defined PCCSDisable [HCSD=%d lcs=%d lc=%d]\n",HCSD,lcs,lc);
                   if(lcs== HCSD+1){ 
                      lc  = Load_CFG('d','i',(CVT*) &PCCSDisable[HCSD]); 
//                    DPrn(35,0,"2 Defined PCCSDisable[%d]\n",HCSD);
                   }
                   if(lc == 5 || lc == 10 ){
                      if (HCSD<MPCCS-1){ HCSD++; }
                   }
               }
               //------------------------------------------------------------
               // ����� �⠭権 � ����� ࠧ�襭� ������
               if (lcsec =='e'){
//                     DPrn(35,0,"1 Defined PCCSEnable [HCSE=%d lcs=%d lc=%d]\n",HCSE,lcs,lc);
                   if (lcs== HCSE+1){ 
                       lc  = Load_CFG('e','i',(CVT*) &PCCSEnable[HCSE]); 
//                     DPrn(35,0,"2 Defined PCCSEnable[%d]\n",HCSE);
                   }
                   if (lc == 5 || lc == 10 ){
                       if (HCSE<MPCCS-1){ HCSE++; }
                   }
               }
               //------------------------------------------------------------
               if (lc == 5 ){ lcs++;    lc=1; }
               if (lc ==10 ){ ClsFlg(); lc=1; }
               //------------------------------------------------------------
           }
       }
       Init_CFG(0);
       if (HIC<=0){ break; }
       HIC--; strcpy(FNCfg,IC[HIC]);
   }
   //------------------------------------------------------------------------
   // �஢�ઠ �᪫�祭�� ���������� ����஢ PCCS
// for(j=0;j<MPCCS;j++){
//     DPrn(0,1,"HCSE=%d  HCSD=%d  PCCSEnable[%d]=%d  PCCSDisable[%d]=%d\n",
//               HCSE,    HCSD,    j, PCCSEnable[j],  j, PCCSDisable[j]);
// }
   for(j=0;j<MPCCS;j++){
       for(i=0;i<MPCCS;i++){
           if(PCCSEnable[j] >0){
              if(PCCSEnable[j]==PCCSDisable[i]){ PCCSEnable[j]=0; }
           }
       }
   };  HCSE=0;
   // ���஢��� ����� ����஫��஢ � ���ᨢ�
   for(j=0;j<MPCCS;j++){
       if(PCCSEnable[j]>0){ 
          PCCSEnable[HCSE]=PCCSEnable[j]; HCSE++; 
       }
   }
   //------------------------------------------------------------------------


   //------------------------------------------------------------------------
   // ��ࠡ�⪠ �������
   if (HCB>=MCH){ HCB=MCH-1; }

   for(b=0;b<HCB;b++){

      FLSER=0; FLNBW=0; FLTCP=0;
      if ((_i)fmod(CB[b].dt,10  )>   0 && SSTAT){ FLSER=1; }
      if ((_i)fmod(CB[b].dt,100 )>= 10 && NSTAT){ FLNBW=1; }
      if ((_i)fmod(CB[b].dt,1000)>=100 && TSTAT){ FLTCP=1; }

      // ��७�� ������� �� �ࠩ��ࠬ
      if (CB[b].dt==0){
          if(HLOC<MLOC-1){
             DLOC[HLOC]=b;            // ����� ������ � ���ᨢ� CB[]
             HLOC++;
          }
      }

      // ��娢�� ������ ������� � ������ 横�� ������, ⮣��, ��� ��㣨�
      // ⮫쪮, �᫨ ���������� �� ⥪�饬 �࠭�!

      // �᫨ ����� ��।���� ��� ������ �� COM ����� � ��।���� ��� ��娢.
      if (FLSER==1&&CB[b].ar >0){
          if (HSER<MSER-1){
              DSER[HSER]=b;           // ����� ������ � ���ᨢ� CB[]
              NSER[HSER]=CB[b].dn;    // ����� 㤠������ �⠭樨
              SSER[HSER]=CB[b].dd;    // ����� ������ �� 㤠������ ��設�
              CBSR[b]=HSER;           // ����� � ���ᨢ� �ࠩ���
              HSER++;                 // ���-�� ��ࠡ�⠭��� ��६����� SER
          }
      }
      // �᫨ ����� ��।���� ��� ������ �� NetBIOS � ��।���� ��� ��娢.
      if (FLNBW==1&&CB[b].ar >0){
          if (HNBW<MNBW-1){
//            DPrn(0,1,"A:DNBW[%d]=%d\n",HNBW,b);
              DNBW[HNBW]=b;           // ����� ������ � ���ᨢ� CB[]
              NNBW[HNBW]=CB[b].dn;    // ����� 㤠������ �⠭樨
              SNBW[HNBW]=CB[b].dd;    // ����� ������ �� 㤠������ ��設�
              CBDN[b]=HNBW;           // ����� � ���ᨢ� �ࠩ���
              HNBW++;                 // ���-�� ��ࠡ�⠭��� ��६����� NBW
          }
      }
      if (FLTCP==1&&CB[b].ar >0){
          if (HTCP<MTCP-1){ 
              n=CB[b].dn;                // ����� 㤠������ �⠭樨 �� ⥪�饬 ������
              if(n>0&&n<MWS){
              SCHN[n][HOWC[n]]=b;        // ����� ������ �� �����쭮� �������� CB[]
              RCHN[n][HOWC[n]]=CB[b].dd; // ����� ������ �� ����஫���
              HOWC[n]++;                 // ���-�� ������� ��� ������ �⠭樨
              RSRV[HTCP]=n;              // ����� 㤠������ �⠭樨 ��� ������� ������ CB[]
              DTCP[HTCP]=b;              // ����� ������� TCP
              HTCP++;                    // ���-�� ��ࠡ�⠭��� ��६����� TCP
              }
          }
      }

      // �᫨ ����� �㦭� ��娢�஢��� - ����ᨬ ��� �����
      // � ���ᨢ ������� ��娢� � � ���ᨢ ������� ���������
      if (CB[b].ar>=1&&HARC<arc_max-1){ 
          arc_ch[HARC]=b; HARC++; upd_ch[HUPD]=b; HUPD++;
      }
   }

   // sprintf(SMSG[MMSG],SMSG[33],FNCMD);
   // DPrn(0,1,"%s\n",SMSG[MMSG]);
   ESER=HSER;
   ENBW=HNBW;
   ETCP=HTCP; for(b=0;b<HSRV;b++) HOWE[b]=HOWC[b];
   //------------------------------------------------------------------------


   //------------------------------------------------------------------------
   // �뢮� �⫠��筮� ���ଠ樨
   if (Debug(2)>0){
      sprintf(SMSG[MMSG],SMSG[34],FNCfg);
      DPrn(2,1,"\n\n%s\n[ v ] HCB=%d\n",SMSG[MMSG],HCB);
      for (b=0;b<HCB;b++){
           DPrn(2,0,"nm=%s dt=%d dn=%d dd=%d t=%d dc=%d l=%f h=%f rm=%s\n",
                CB[b].nm, CB[b].dt, CB[b].dn, CB[b].dd,
                CB[b].t,  CB[b].dc, CB[b].l,  CB[b].h, CB[b].rm );
      }
      DPrn(2,0,"\n\nHLOC=%d  HSER=%d  HNBW=%d  HTCP=%d  HARC=%d  HUPD=%d\n",
                    HLOC,    HSER,    HNBW,    HTCP,    HARC,    HUPD);
      sprintf(SMSG[MMSG],SMSG[35]);
      DPrn(2,0,"\n\n%s\n",SMSG[MMSG]);
      for(b=0;b<arc_max;b++){
         if(arc_ch[b]>0){ DPrn(2,0,"%d ",arc_ch[b]); }
         if(b%15==0)    { DPrn(2,0,"\n");            }
      }
      sprintf(SMSG[MMSG],SMSG[36]);
      DPrn(2,0,"\n\n%s\n",SMSG[MMSG]);
      for(b=0;b<HSER;b++){
         if(DSER[b]>0){ DPrn(2,0,"%d ",DSER[b]); }
         if(b%15==0)  { DPrn(2,0,"\n");          }
      }
      sprintf(SMSG[MMSG],SMSG[37]);
      DPrn(2,0,"\n\n%s\n",SMSG[MMSG]);
      for(b=0;b<HNBW;b++){
         if(DNBW[b]>0){ DPrn(2,0,"%d ",DNBW[b]); }
         if(b%15==0)  { DPrn(2,0,"\n");          }
      }
   }
   //------------------------------------------------------------------------
   if (Debug(4)>0){
      // ������ ��娢��㥬�� �������
      sprintf(SMSG[MMSG],SMSG[38]);
      DPrn(4,0,"\n\n%s\n",SMSG[MMSG]);
      for(b=0,a=0;b<HCB;b++){
          if(CB[b].ar==1){
             DPrn(4,0,"#=%d nm=%s t=%d l=%f h=%f rm=%s\n",
                  b, CB[b].nm, CB[b].t, CB[b].l, CB[b].h, CB[b].rm );
             a++;
          }
      }
      sprintf(SMSG[MMSG],SMSG[39],a);
      DPrn(4,0,"%s\n\n",SMSG[MMSG]);
      // ������ ����஫��㥬��(�����塞��) �������
      DPrn(4,0,"\n\n�����塞� ����஫��㥬� ������:\n");
      for(b=0,a=0;b<HCB;b++){
          if(CB[b].ar==2){
             DPrn(4,0,"#=%d nm=%s t=%d l=%f h=%f rm=%s\n",
                  b, CB[b].nm, CB[b].t, CB[b].l, CB[b].h, CB[b].rm );
             a++;
          }
      }
      DPrn(4,0,"�ᥣ� �����塞�� �������: %d\n\n",a);
   }
   DPrn(4,0,"\n\n");
   DPrn(4,0,"����� ������� ����訢����� �� COM �����: \n");
   for (j=0;j<HSER;j++){
       DPrn(4,0,"CB[ch]=%d  RemWS=%d  RemCH=%d\n",DSER[j],NSER[j],SSER[j]);
   }
   DPrn(4,0,"\n\n");
   DPrn(4,0,"����� ������� ����訢����� �� NetBIOS: \n");
   for (j=0;j<HNBW;j++){
       DPrn(4,0,"CB[ch]=%d  RemWS=%d  RemCH=%d\n",DNBW[j],NNBW[j],SNBW[j]);
   }
   DPrn(4,0,"\n\n");
   DPrn(4,0,"����� ������� ����訢����� �� TCP(UDP): \n");
   for (b=0;b<SRV_NUM[b];b++)
   for (j=0;j<HOWC[SRV_NUM[b]];j++){
       DPrn(4,0,"SRV_NUM[%d]=%d LocCH=%d RemCH=%d\n",b,SRV_NUM[b],SCHN[SRV_NUM[b]][j],RCHN[SRV_NUM[b]][j]);
   }
   DPrn(4,0,"\n\n");
   DPrn(4,0,"����� ������� ᮮ�饭��: \n");
   for (j=0;j<HMSGCH;j++){
       DPrn(4,0,"%d ",MSGNUMCH[j]);
   }
   DPrn(4,0,"\n\n");

   //------------------------------------------------------------------------

   return(0);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// �⥭�� 䠩�� ������� � ���������� �������� �������
_i Read_CMD(vd){
   _i FH =0;                          // ��⥬�� ����� 䠩��
   _i HBR=0;                          // ������⢮ ��⠭��� ����
   if ((access(FNCMD,4))!=0){
        sprintf(SMSG[MMSG],SMSG[40],FNCMD);
        DPrn(DEBRCMD,0,"%s\n",SMSG[MMSG]);
        HCMD=0; return(0);
   }
   FH=open(FNCMD,O_BINARY|O_RDONLY);
   if (FH>0){
       HBR=read(FH,FBuf,SBuf);
       sprintf(SMSG[MMSG],SMSG[41],HBR,FNCMD);
       DPrn(DEBRCMD,0,"%s\n",SMSG[MMSG]);
   } else {
       sprintf(SMSG[MMSG],SMSG[42],FNCMD);
       DPrn(DEBRCMD,0,"%s\n",SMSG[MMSG]);
   }
   close(FH);
   if (HBR>0){
       FillCMD(HBR);
   } else {
       sprintf(SMSG[MMSG],SMSG[43],FNCMD);
       DPrn(DEBRCMD,0,"%s\n",SMSG[MMSG]);
   }
   return(0);
}
//---------------------------------------------------------------------------
