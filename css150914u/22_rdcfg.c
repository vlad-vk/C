// coding: cp866
//---------------------------------------------------------------------------
// чтение файла конфигурации
_i Read_Cfg(vd){
   _i  lc=1, a=0, b=0, d=0, f=0, j=0, HBR=0, FHPRN=0, pcmd=0;
   uc  c[3]={0,0,0};
   _l  CBSER=0;
   //------------------------------------------------------------------------
   if (BZero==0){
       if (HZRO>0){
           rectfill(screen, LZRO.X, LZRO.Y, LZRO.X+LZRO.W,
                                            LZRO.Y+LZRO.H, LZRO.cs );
       }
       DPrn(11,1,"\n\n--------------------------------------------\n");
       sprintf(SMSG[MMSG],SMSG[45]);
       DPrn(11,0,"%s\n",SMSG[MMSG]);

       HANI=0;              // a
       HSPR=0;              // P
       HSTRC=0;             // s
       HKBD=0;              // k
       HINI=0;              // i
       HRUN=0;              // r
       HKNP=0;              // K
       HMNU=0;              // m
       HVAL=0;              // v
       HGST=0;              // g
       HTRD=0;              // t
       HGET=0;              // G
       HXCV=0;              // c
       HFVW=0;              // f
       HDAT=0;              // D
       HTIM=0;              // T
       HSTA=0;              // S
       HMSS=0;              // M
       HCST=0;              // C
       HPLZ=0;              // p
       HZRO=0;              // z
       HBOX=0;              // b
       HSHK=0;              // h
       HSEL=0;              // O
       HREP=0;              // o
       CDB =1;              // d
       HSLM=0;              // #
       HFVB=0;              // B
       FFST=0;              // F

       // сбросить счетчики текущих запросов по COM порту
       if(CSSER>=ESER){ CSSER=0; }
       if(CCSER>=ESER){ CCSER=0; }
       // сбросить текущий элемент запроса по NetBIOS
       if(CNBQ >=ENBW){ CNBQ=0;  }

       LGET_en=0;           //
       LGET_st=0;           //
       vkey=0;              //
       LockKBD=100;         //
       FVA=0;               // номер активного окна просмотра
       fSTR=0;              // флаг повторного вывода строк
       CEKR=0;              // номер текущего экрана

//     // обнуление списка отображаемых переменных и их значений после каждого экрана
//     for(a=ESER;a<HSER;a++){ CBS[DSER[a]]=0; CBSR[DSER[a]]=0; DSER[a]=0; }
//     for(a=ENBW;a<HNBW;a++){ CBN[DNBW[a]]=0; CBDN[DNBW[a]]=0; DNBW[a]=0; }
//     for(a=ETCP;a<HTCP;a++){ CBT[DTCP[a]]=0; CBTP[DTCP[a]]=0; DTCP[a]=0; }
//     DPrn(31,0,"Zero variables after go to other screen.\n");

       EKNBWCNT=0;          // сброс счетчика запросов после смены экрана
       //---------------------------------------------------------------------
       sprintf(SMSG[MMSG],SMSG[46]);
       DPrn(11,0,"%s\n",SMSG[MMSG]);
       for (a=0;a<MVAL;a++){
            LVAL[a].vw=0; LVAL[a].nv=0; LVAL[a].sf[0]=0;
            strcpy(LVAL[a].str," ");
       }
       DPrn(11,0,"CL VAL\n");
       for (a=0;a<MGET;a++){ LGET[a].vw=0; LGET[a].nv=0; LGET[a].sf[0]=0; }
       DPrn(11,0,"CL GET\n");
       for (a=0;a<MANI;a++){ LANI[a].vw=0; LANI[a].nv=0; }
       DPrn(11,0,"CL ANI\n");
       for (a=0;a<MCST;a++){ LCST[a].vw=0; LCST[a].nv=0; }
       DPrn(11,0,"CL CST\n");
       for (a=0;a<MPLZ;a++){ LPLZ[a].nv=0; LPLZ[a].W=0; LPLZ[a].H=0; LPLZ[a].L=0; }
       DPrn(11,0,"CL PLZ\n");
       for (a=0;a<MGST;a++){ LGST[a].nv=0; }
       DPrn(11,0,"CL GST\n");
       for (a=0;a<MXCV;a++){ LXCV[a].nv=0; }
       DPrn(11,0,"CL XCV\n");
       for (a=0;a<MINI;a++){ 
            if (strcasecmp(LINI[a].fn,FNCfg)==0){
                LASTINI_P=LASTINI_C; LASTINI_C=LINI[a].nn;
            }
            LINI[a].vw=0; LINI[a].nn=0; LINI[a].fn[0]=0; 
       }
       DPrn(11,0,"CL INI\n");
       for (a=0;a<MKBD;a++){
            LKBD[a].cod=0; LKBD[a].flg=0; LKBD[a].str[0]=0; LKBD[a].run=0;
       };   DPrn(11,0,"CL KBD\n");
       for (a=0;a<MREP;a++){
            LREP[a].ff[0]=0; LREP[a].fo[0]=0;
            for(b=0;b<MSREP;b++){ LREP[a].ss[b][0]=0; }
       };   DPrn(11,0,"CL REP\n");
       for (a=0;a<MSTR;a++){
            for(b=0;b<SLEN;b++){ LSTR[a].str[b]=0; }
       };   DPrn(11,0,"CL STR\n");
       //---------------------------------------------------------------------
       // обнуление флагов загрузки изображений
       for (a=0;a<MSPR;a++){ LSPR[a].vw=0; }
       DPrn(11,0,"CL SPR\n");
       //---------------------------------------------------------------------
       // закрыть открытые файлы просмотра
       for (a=0;a<MFVW;a++){
           if (VW[a].ffl==1){
               if(dbOpenTest(VW[a].fhd,0)>0){ dbClose(VW[a].fhd); }
           }
           VW[a].ffl=0; LFVW[a].f1=0; LFVW[a].f2=0;
       };  DPrn(11,0,"CL FVW\n");
       //---------------------------------------------------------------------
       // закрыть открытые файлы архивов
       for (a=0;a<MTRD;a++){
            if(TF[CTC].ff[a]==1){
               if(dbOpenTest(TF[CTC].fh[a],0)>0){ dbClose(TF[CTC].fh[a]); }
            }
            TF[CTC].ff[a]=0;
       };   DPrn(11,0,"CL TRD\n");
       //---------------------------------------------------------------------
       // обнулить переменные кнопок
       for (a=0;a<MKNP;a++){
            LKNP[a].vw=0;
       };   DPrn(11,0,"CL KNP\n");
       //---------------------------------------------------------------------
       // обнулить переменные меню
       for (a=0;a<MMNU;a++){
            LMNU[a].sf=0; LMNU[a].bf=0; LMNU[a].bn=0; LMNU[a].k=0; LMNU[a].vw=0;
       };   DPrn(11,0,"CL MNU\n");
       //---------------------------------------------------------------------
       // обнулить переменные областей
       for(a=0;a<MSEL;a++){ LSEL[a].sp=-1; LSEL[a].k=0; }
       FSLC=0;FSLO=0;FSLF=0;ASEL=NOSEL;
       DPrn(11,0,"CL SEL\n");
       for(a=0;a<MSLM;a++){ LSLM[a].np=0; LSLM[a].fl=0; };
       DPrn(11,0,"CL SLM\n");
       //---------------------------------------------------------------------
       // обнуление переменных трендов
       for(a=0;a<MTRD;a++){
          TF[CTC].gh[a]=0;
          TF[CTC].an[a]=0;
          TF[CTC].am[a]=0;
          TF[CTC].aa[a]=0;
          for(b=0;b<MTRG;b++){
              trv_cc[a][b]=0; LTRD[a].m[b][0]=0;
              LTRD[a].n[b]=0; TF[CTC].ga[a][b]=0; TF[CTC].lt[a]=0;
          }
       }; DPrn(11,0,"CL TRD\n");
       //---------------------------------------------------------------------
       strcpy(FirstINI,""); El_hii=0;
   }
   if(PN_FF==0){
      strcpy(PN_FC,PN_CC);
      strcpy(PN_CC,FNCfg); PN_FF=1; PN_NN[0]=0;
   }
   //------------------------------------------------------------------------


   //------------------------------------------------------------------------
       ClsFlg();
   if (CFirst==0){ strcpy(lcsok,"#AVdiDFTMRSNnLWZYyEeXH"); }
   else          { strcpy(lcsok,"?#WwdKmrigtlsvkGcFfapbBzhoOSMDTPCYyEeQX"); }
   //------------------------------------------------------------------------
   // разбор файла конфигурации
   if(Init_CFG(1)<0){
      sprintf(SMSG[MMSG],SMSG[47],FNCfg);
      sprintf(ErrSTR,"%s\n",SMSG[MMSG]);
       DPrn(0,0,"%s\n",ErrSTR);
      SetFNCfg();
      strcpy(PN_fn[0],FNCfg);
      strcpy(PN_fn[1],FNCfg); FNCfgLD=1; ErrINI=1;
      return(2);
   }
      sprintf(SMSG[MMSG],SMSG[48],FNCfg);
      DPrn(11,0,"%s\n",SMSG[MMSG]);
      if(strlen(FirstINI)<2){ 
         strcpy(FirstINI,FNCfg); strcpy(El_ifn[0],FNCfg); El_hii=1;
      }

   for(a=0;a<MCYC;a++){
      if (Init_CFG(2)){ break; }
      for(lca=0;lca<FHCfg;lca++){
         if (FBuf[lca]==10 && LNstr==1){ LNstr=0; continue; }

         lc=Load_CFG('0','i',(CVT*) &ZERO );
         //------------------------------------------------------------------
         // определения флага чтения комманд из файла
         if (lcsec =='#'){
            if (lcs== 1 ){ lc=Load_CFG('#', 'c',(CVT*) &LCIF.cmd ); }
            if (lcs== 2 ){ lc=Load_CFG('#', 'f',(CVT*) &LCIF.v1  ); }
            if (lcs== 3 ){ lc=Load_CFG('#', 'c',(CVT*) &LCIF.zn  ); }
            if (lcs== 4 ){ lc=Load_CFG('#', 'f',(CVT*) &LCIF.v2  ); }
            //----------------------------------------------------------------
            if (strcasecmp(LCIF.cmd,"END")==0){
//              DPrn(0,0, "COMMAND END\n");
                FLCIF=0;
                LCIF.cmd[0]=0; LCIF.v1=0; LCIF.zn[0]=0; LCIF.v2=0; pcmd=0;
            }
            //----------------------------------------------------------------
            if (lc==10){
//              DPrn(0,0, "COMMAND [%c%c] lcs=%d:\n",LCIF.cmd[0],LCIF.cmd[1],lcs);
                //------------------------------------------------------------
                if (strcasecmp(LCIF.cmd,"DO" )==0){
//                  DPrn(0,0,"COMMAND DO a=%d lca=%ld\n", a, lca);
                    LCIF.cmd[0]=0; HNEXT=0; FLCWa=a; FLCWlca=lca;
                }
                //------------------------------------------------------------
                if (strcasecmp(LCIF.cmd,"NEXT")==0&&lcs>=4){
//                  DPrn(0,0, "COMMAND NEXT:\n");
                    LCIF.cmd[0]=0; f=0; HNEXT++;
                    if(strcasecmp(LCIF.zn,"<" )==0&&(LCIF.v1< LCIF.v2)){
//                     DPrn(0,0, "NEXT %f <  %f\n",LCIF.v1,LCIF.v2);
                       f=1;
                    }
                    if(strcasecmp(LCIF.zn,">" )==0&&(LCIF.v1> LCIF.v2)){
//                     DPrn(0,0, "NEXT %f >  %f\n",LCIF.v1,LCIF.v2);
                       f=1;
                    }
                    if(strcasecmp(LCIF.zn,"=" )==0&&(LCIF.v1==LCIF.v2)){
//                     DPrn(0,0, "NEXT %f =  %f\n",LCIF.v1,LCIF.v2);
                       f=1;
                    }
                    if(strcasecmp(LCIF.zn,"!" )==0&&(LCIF.v1!=LCIF.v2)){
//                     DPrn(0,0, "NEXT %f !  %f\n",LCIF.v1,LCIF.v2);
                       f=1;
                    }
                    // если условие цикла выполняется- продолжить цикл
//                  DPrn(0,0,"NEXT a=%d lca=%ld FLCWa=%d FLCWlca=%ld\n",
//                               a,   lca,    FLCWa,   FLCWlca);
                    if(HNEXT<MNEXT&&f>0){ a=FLCWa; lca=FLCWlca; }
                }
                //------------------------------------------------------------
                if (strcasecmp(LCIF.cmd,"ELSE" )==0){
                    LCIF.cmd[0]=0; FLCIF=1;
                    // если предыдущий IF был проигнорирован (false) -
                    // открываем чтение комманд для секции ELSE
                    if(pcmd==1){ 
                       FLCIF=0; pcmd=0; /* DPrn(0,0, "COMMAND ELSE:\n"); */
                    }
                }
                //------------------------------------------------------------
                if (strcasecmp(LCIF.cmd,"IF" )==0 && lcs>=4){
//                  DPrn(0,0, "COMMAND IF:\n");
                    FLCIF=1; LCIF.cmd[0]=0; pcmd=1;
                    if(strcasecmp(LCIF.zn,"<" )==0){
                       if(LCIF.v1 <  LCIF.v2){ FLCIF=0; pcmd=2; }
//                     DPrn(0,0, "IF   %f <  %f  FLCIF=%d\n",LCIF.v1,LCIF.v2,FLCIF);
                    }
                    if(strcasecmp(LCIF.zn,">" )==0){
                       if(LCIF.v1 >  LCIF.v2){ FLCIF=0; pcmd=3; }
//                     DPrn(0,0, "IF   %f >  %f  FLCIF=%d\n",LCIF.v1,LCIF.v2,FLCIF);
                    }
                    if(strcasecmp(LCIF.zn,"=" )==0){
                       if(LCIF.v1 == LCIF.v2){ FLCIF=0; pcmd=4; }
//                     DPrn(0,0, "IF   %f =  %f  FLCIF=%d\n",LCIF.v1,LCIF.v2,FLCIF);
                    }
                    if(strcasecmp(LCIF.zn,"!" )==0){
                       if(LCIF.v1 != LCIF.v2){ FLCIF=0; pcmd=5; }
//                     DPrn(0,0, "IF   %f !  %f  FLCIF=%d\n",LCIF.v1,LCIF.v2,FLCIF);
                    }
                }
                //------------------------------------------------------------
            }
         }
         //-------------------------------------------------------------------
         // если был установлен флаг условного выполнения (# "IF" v1 "z" v2)
         // комманды не воспринимаются пока не встретится (# "END")
         if(FLCIF>0){
            if (lc == 5 ){ lcs++;    lc=1; }
            if (lc ==10 ){ ClsFlg(); lc=1; }
            continue;
         }
         //-------------------------------------------------------------------

         //-------------------------------------------------------------------
         if (lcsec =='A'){
            if (lcs== 1 ){ lc=Load_CFG('A', 'i',(CVT*) &ARCminsz ); }
            if (lcs== 2 ){ lc=Load_CFG('A', 'i',(CVT*) &ARChmday ); }
            if (lcs== 3 ){ lc=Load_CFG('A', 'i',(CVT*) &ARChfdel ); }
            if (lcs== 4 ){ lc=Load_CFG('A', 'c',(CVT*) &ARCapath ); }
            if (lcs== 5 ){ 
                lc=Load_CFG('A','i',(CVT*) &DBUFlg);
                if(DBUFlg<0||DBUFlg>MCH-12-1){ DBUFlg=0; }
            }
            if (lc ==10 ){ ;; }
         }
         //------------------------------------------------------------------
         if (lcsec =='L'){
            if (lcs== 1 ){ lc=Load_CFG('L', 'c',(CVT*) &FNLgt ); }
            if (lcs== 2 ){ lc=Load_CFG('L', 'i',(CVT*) &VWLgt ); }
            if (lcs== 3 ){ lc=Load_CFG('L', 'i',(CVT*) &VWver ); }
            if (lcs== 4 ){ lc=Load_CFG('L', 'i',(CVT*) &VWini ); }
            if (lcs== 5 ){ lc=Load_CFG('L', 'i',(CVT*) &VWcol ); }
            if (lc ==10 ){ ;; }
         }
         //------------------------------------------------------------------
         if (lcsec =='V'){
            if (lcs== 1 ){ lc=Load_CFG('V', 'i',(CVT*) &VMode  ); }
            if (lcs== 2 ){ lc=Load_CFG('V', 'c',(CVT*) &GRMode ); }
            if (lcs== 3 ){ lc=Load_CFG('V', 'i',(CVT*) &CDVSync); }
            if (lcs== 4 ){ lc=Load_CFG('V', 'i',(CVT*) &FPSLEEP); }
            if (lcs== 5 ){ lc=Load_CFG('V', 'i',(CVT*) &SYSLEEP); }
            if (lc ==10 ){ ;; }
         }
         //------------------------------------------------------------------
         if (lcsec =='X'){
            if (lcs== 1 ){ lc=Load_CFG('X', 'f',(CVT*) &XRCMD ); }
            if (lc ==10 ){ ;; }
         }
         //------------------------------------------------------------------
         if (lcsec =='N'){
            if (lcs== 1 ){ lc=Load_CFG('N', 'i',(CVT*) &NBWS     ); }
            if (lcs== 2 ){ lc=Load_CFG('N', 'i',(CVT*) &REREQH   ); }
            if (lcs== 3 ){ lc=Load_CFG('N', 'i',(CVT*) &SPHREQ   ); } // not USED
            if (lcs== 4 ){ lc=Load_CFG('N', 'i',(CVT*) &BDSER    ); }
            if (lcs== 5 ){ lc=Load_CFG('N', 'i',(CVT*) &NBWAI    ); }
            if (lcs== 6 ){
                           lc=Load_CFG('N', 'i',(CVT*) &NBFRQ    );
                           if (NBFRQ<10){ NBFRQ=10; }
            }
            if (lcs== 7 ){ lc=Load_CFG('N', 'i',(CVT*) &NBWTO    ); }
            if (lcs== 8 ){ lc=Load_CFG('N', 'i',(CVT*) &EKNBWREQ ); }
            if (lcs== 9 ){ lc=Load_CFG('N', 'i',(CVT*) &NBWLANA  ); }
            if (lc ==10 ){
                if(REREQS  <1){ REREQH=3; }
                if(EKNBWREQ<3){ EKNBWREQ=3; }
            }
         }
         //------------------------------------------------------------------
         if (lcsec =='H'){
             if(lcs==1){ lc=Load_CFG('H', 'i',(CVT*) &SRV_NUM[HSRV]); }
             if(lcs==2){ lc=Load_CFG('H', 'c',(CVT*) &SRVIP[HSRV]); }
             if(lcs==3){ lc=Load_CFG('H', 'i',(CVT*) &SRVPORT[HSRV]); }
             if(lc ==10 ){
                DPrn(35,0,"HSRV=%d,SRV_NUM=%d,SRVIP=%s,SRVPORT=%d\n",HSRV,SRV_NUM[HSRV],SRVIP[HSRV],SRVPORT[HSRV]);
                if (HSRV<MPCCS){ HSRV=HSRV+1; }
             }
         }
         //------------------------------------------------------------------
         if (lcsec =='R'){
            if (lcs== 1 ){ lc=Load_CFG('R', 'i',(CVT*) &CRCod[0] ); }
            if (lcs== 2 ){ lc=Load_CFG('R', 'i',(CVT*) &CRCod[1] ); }
            if (lcs== 3 ){ lc=Load_CFG('R', 'i',(CVT*) &CRCod[2] ); }
            if (lc ==10 ){ ;; }
         }
         //------------------------------------------------------------------
         if (lcsec =='Z'){
            if (lcs== 1 ){ lc=Load_CFG('Z', 'i',(CVT*) &SPRN.port ); }
            if (lcs== 2 ){ lc=Load_CFG('Z', 'c',(CVT*) &SPRN.fn   ); }
            if (lcs== 3 ){ lc=Load_CFG('Z', 'i',(CVT*) &SPRN.ws   ); }
            if (lcs== 4 ){ lc=Load_CFG('Z', 'i',(CVT*) &SPRN.X    ); }
            if (lcs== 5 ){ lc=Load_CFG('Z', 'i',(CVT*) &SPRN.Y    ); }
            if (lcs== 6 ){ lc=Load_CFG('Z', 'i',(CVT*) &SPRN.W    ); }
            if (lcs== 7 ){ lc=Load_CFG('Z', 'i',(CVT*) &SPRN.H    ); }
            if (lc ==10 ){ ;; }
         }
         //------------------------------------------------------------------
         if (lcsec =='n'){
            if (lcs== 1 ){ lc=Load_CFG('n', 'i',(CVT*) &LDRV ); }
            if (lc == 10){ ;; }
         }
         //------------------------------------------------------------------
         if (lcsec =='E'){
            if (lcs== 1 ){ lc=Load_CFG('E', 'i',(CVT*) &CEKR  ); }
            if (lcs== 2 ){ lc=Load_CFG('E', 'c',(CVT*) &PN_NN ); }
            if (lc == 10){
                if(CEKR>MEKR-1){ CEKR=MEKR-1; }
                if(PN_NN[0]=='P'||PN_NN[0]=='p'){
                   strcpy(PN_CC,PN_fn[0]);
                }
            }
         }
         //------------------------------------------------------------------
         if (lcsec =='e'){
            if (lcs== 1 ){ lc=Load_CFG('e', 'i',(CVT*) &LEDT.T  ); }
            if (lc ==10 ){ ;; }
         }
         //------------------------------------------------------------------
         if (lcsec =='W'){
            if (lcs== 1 ){
                DPrn(11,0,"W.");
                lc=Load_CFG('W', 'i',(CVT*) &CNVL); 
                if(CNVL>=MSTT){ CNVL=MSTT-1; }
            }
            if (lcs== 2 ){ lc=Load_CFG('W','i',(CVT*) &LSTT[CNVL].X   );  }
            if (lcs== 3 ){ lc=Load_CFG('W','i',(CVT*) &LSTT[CNVL].Y   );  }
            if (lcs== 4 ){ lc=Load_CFG('W','i',(CVT*) &LSTT[CNVL].W   );  }
            if (lcs== 5 ){ lc=Load_CFG('W','i',(CVT*) &LSTT[CNVL].LS  );  }
            if (lcs== 6 ){ lc=Load_CFG('W','i',(CVT*) &LSTT[CNVL].sn  );  }
            if (lcs== 7 ){ lc=Load_CFG('W','i',(CVT*) &LSTT[CNVL].cs  );  }
            if (lcs== 8 ){ lc=Load_CFG('W','i',(CVT*) &LSTT[CNVL].cg  );  }
            if (lcs== 9 ){ lc=Load_CFG('W','i',(CVT*) &LSTT[CNVL].cc  );  }
            if (lcs==10 ){ lc=Load_CFG('W','i',(CVT*) &LSTT[CNVL].kb  );  }
            if (lc ==10 ){
                if (CNVL>=HSTT){ HSTT=CNVL+1; }
                if (HSTT>=MSTT){ HSTT=MSTT-1; }; DPrn(11,0,"\nW end=%d\n",HSTT);
            }
         }
         //------------------------------------------------------------------
         if (lcsec =='Y'){
            if (lcs== 1 ){
                DPrn(11,0,"Y.");
                lc=Load_CFG('Y','i',(CVT*) &CNVL); 
                if(CNVL>=MDIR){ CNVL=MDIR-1; }
            }
            if (lcs== 2 ){ lc=Load_CFG('Y','c',(CVT*) &LDIR[CNVL]); }
            if (lc ==10 ){
                if (CNVL>=HDIR){ HDIR=CNVL+1; }
                if (HDIR>=MDIR){ HDIR=MDIR-1; }; DPrn(11,0,"\nY end=%d\n",HDIR);
            }
         }
         //------------------------------------------------------------------
         if (lcsec =='y'){
            if (lcs== 1 ){
                DPrn(11,0,"y.");
                lc=Load_CFG('y','i',(CVT*) &CNVL); 
                if(CNVL>=MYCM){ CNVL=MYCM-1; }
                LMVL[CNVL][0]=0;
            }
            if (lcs== 2 ){ 
                lc=Load_CFG('y','c',(CVT*) &LMVL[CNVL]); 
                YCM[CNVL]=atof(LMVL[CNVL]); 
            }
            if (lc ==10 ){
                if (CNVL>=HYCM){ HYCM=CNVL+1; }
                if (HYCM>=MYCM){ HYCM=MYCM-1; }; DPrn(11,0,"\ny end=%d\n",HYCM);
            }
         }
         //------------------------------------------------------------------

         //------------------------------------------------------------------
         if (lcsec =='m'){
            if (lcs== 1 ){
                DPrn(11,0,"m.");
                lc=Load_CFG('m','i',(CVT*) &LMNU[HMNU].vw  );
            }
            if (lcs== 2 ){ lc=Load_CFG('m','i',(CVT*) &LMNU[HMNU].X   );  }
            if (lcs== 3 ){ lc=Load_CFG('m','i',(CVT*) &LMNU[HMNU].Y   );  }
            if (lcs== 4 ){ lc=Load_CFG('m','i',(CVT*) &LMNU[HMNU].W   );  }
            if (lcs== 5 ){ lc=Load_CFG('m','i',(CVT*) &LMNU[HMNU].H   );  }
            if (lcs== 6 ){ lc=Load_CFG('m','i',(CVT*) &LMNU[HMNU].c   );  }
            if (lcs== 7 ){ lc=Load_CFG('m','i',(CVT*) &LMNU[HMNU].k   );  }
            if (lcs== 8 ){ lc=Load_CFG('m','i',(CVT*) &LMNU[HMNU].bn  );  }
            if (lcs== 9 ){ lc=Load_CFG('m','c',(CVT*) &LMNU[HMNU].str );  }
            if (lc ==10 ){
                if (HMNU<MMNU-1){ HMNU++; }; DPrn(11,0,"\nm end=%d\n",HMNU);
            }
         }
         //------------------------------------------------------------------
         if (lcsec =='K'){
            if (lcs== 1 ){
                DPrn(11,0,"K.");
                lc=Load_CFG('K', 'i',(CVT*) &CNVL); 
                if(CNVL>=MKNP){ CNVL=MKNP-1; }
            }
            if (lcs== 2 ){ lc=Load_CFG('K','i',(CVT*) &LKNP[CNVL].vw  );  }
            if (lcs== 3 ){ lc=Load_CFG('K','i',(CVT*) &LKNP[CNVL].W   );  }
            if (lcs== 4 ){ lc=Load_CFG('K','i',(CVT*) &LKNP[CNVL].H   );  }
            if (lcs== 5 ){ lc=Load_CFG('K','c',(CVT*) &LKNP[CNVL].fb  );  }
            if (lc ==10 ){
                if (CNVL>=HKNP){ HKNP=CNVL+1; }
                if (HKNP>=MKNP){ HKNP=MKNP-1; }; DPrn(11,0,"\nK end=%d\n",HKNP);
            }
         }
         //------------------------------------------------------------------
         if (lcsec =='O'){
            if (lcs== 1 ){
                DPrn(11,0,"O.");
                lc=Load_CFG('O','i',(CVT*) &LSEL[HSEL].X   );
            }
            if (lcs== 2 ){ lc=Load_CFG('O','i',(CVT*) &LSEL[HSEL].Y   );  }
            if (lcs== 3 ){ lc=Load_CFG('O','i',(CVT*) &LSEL[HSEL].W   );  }
            if (lcs== 4 ){ lc=Load_CFG('O','i',(CVT*) &LSEL[HSEL].H   );  }
            if (lcs== 5 ){ lc=Load_CFG('O','i',(CVT*) &LSEL[HSEL].k   );  }
            if (lcs== 6 ){ lc=Load_CFG('O','i',(CVT*) &LSEL[HSEL].sp  );  }
            if (lcs== 7 ){ lc=Load_CFG('O','i',(CVT*) &LSEL[HSEL].sx  );  }
            if (lcs== 8 ){ lc=Load_CFG('O','i',(CVT*) &LSEL[HSEL].sy  );  }
            if (lcs== 9 ){ lc=Load_CFG('O','i',(CVT*) &LSEL[HSEL].cs  );  }
            if (lcs==10 ){ lc=Load_CFG('O','c',(CVT*) &LSEL[HSEL].str );  }
            if (lc ==10 ){
                if (HSEL<MSEL-1){ HSEL++; }; DPrn(11,0,"\nO end=%d\n",HSEL);
            }
         }
         //------------------------------------------------------------------
         if (lcsec =='?'){
            if (lcs== 1 ){
                DPrn(11,0,"?.");
                lc=Load_CFG('?','i',(CVT*) &CNVL); 
                if(CNVL>=MOPQ){ CNVL=MOPQ-1; }
            }
            if (lcs== 2 ){ lc=Load_CFG('?', 'i',(CVT*) &LOPQ[CNVL].na  ); }
            if (lcs== 3 ){
                if (LOPQ[CNVL].na<0){
                    lc=Load_CFG('?','c',(CVT*) &SVAL); 
                    if(lc==5){ ValStr(CNVL,'?'); }
                }  else  {
                    lc=Load_CFG('?','i',(CVT*) &LOPQ[CNVL].nv );
                }
            }
            if (lcs== 4 ){ lc=Load_CFG('?', 'i',(CVT*) &LOPQ[CNVL].ka  ); }
            if (lcs== 5 ){ lc=Load_CFG('?', 'i',(CVT*) &LOPQ[CNVL].k1  ); }
            if (lcs== 6 ){ lc=Load_CFG('?', 'i',(CVT*) &LOPQ[CNVL].k2  ); }
            if (lcs== 7 ){ lc=Load_CFG('?', 'i',(CVT*) &LOPQ[CNVL].k3  ); }
            if (lcs== 8 ){ lc=Load_CFG('?', 'i',(CVT*) &LOPQ[CNVL].v1  ); }
            if (lcs== 9 ){ lc=Load_CFG('?', 'i',(CVT*) &LOPQ[CNVL].v2  ); }
            if (lcs== 10){ lc=Load_CFG('?', 'i',(CVT*) &LOPQ[CNVL].v3  ); }
            if (lcs== 11){ lc=Load_CFG('?', 'i',(CVT*) &LOPQ[CNVL].g1  ); }
            if (lcs== 12){ lc=Load_CFG('?', 'i',(CVT*) &LOPQ[CNVL].g2  ); }
            if (lcs== 13){ lc=Load_CFG('?', 'i',(CVT*) &LOPQ[CNVL].g3  ); }
            if (lcs== 14){ lc=Load_CFG('?', 'i',(CVT*) &LOPQ[CNVL].gr  ); }
            if (lc == 10){ ;; }
         }
         //------------------------------------------------------------------
         if (lcsec =='Q'){
            if (lcs== 1 ){
                DPrn(11,0,"Q.");
                lc=Load_CFG('Q','i',(CVT*) &CNVL); 
                if(CNVL>=MSLM){ CNVL=MSLM-1; }
            }
            if (lcs== 2 ){ lc=Load_CFG('Q', 'i',(CVT*) &LSLM[CNVL].na  ); }
            if (lcs== 3 ){
                if (LSLM[CNVL].na<0){
                    lc=Load_CFG('Q','c',(CVT*) &SVAL); 
                    if(lc==5){ ValStr(CNVL,'Q'); }
                }  else  {
                    lc=Load_CFG('Q','i',(CVT*) &LSLM[CNVL].nv );
                }
            }
            if (lcs== 4 ){ lc=Load_CFG('Q','i',(CVT*) &LSLM[CNVL].X   );  }
            if (lcs== 5 ){ lc=Load_CFG('Q','i',(CVT*) &LSLM[CNVL].Y   );  }
            if (lcs== 6 ){ lc=Load_CFG('Q','i',(CVT*) &LSLM[CNVL].W   );  }
            if (lcs== 7 ){ lc=Load_CFG('Q','i',(CVT*) &LSLM[CNVL].H   );  }
            if (lcs== 8 ){ lc=Load_CFG('Q','i',(CVT*) &LSLM[CNVL].np  );  }
            if (lcs== 9 ){ lc=Load_CFG('Q','i',(CVT*) &LSLM[CNVL].md  );  }
            if (lcs==10 ){ lc=Load_CFG('Q','f',(CVT*) &LSLM[CNVL].vl  );  }
            if (lcs==11 ){ lc=Load_CFG('Q','f',(CVT*) &LSLM[CNVL].vh  );  }
            if (lcs==12 ){ lc=Load_CFG('Q','i',(CVT*) &LSLM[CNVL].cl  );  }
            if (lcs==13 ){ lc=Load_CFG('Q','i',(CVT*) &LSLM[CNVL].cn  );  }
            if (lcs==14 ){ lc=Load_CFG('Q','i',(CVT*) &LSLM[CNVL].ch  );  }
            if (lc ==10 ){
                if (CNVL>=HSLM){ HSLM=CNVL+1; }
                if (HSLM>=MSLM){ HSLM=MSLM-1; }; DPrn(11,0,"\nQ end=%d\n",HSLM);
            }
         }
         //------------------------------------------------------------------
         if (lcsec =='k'){
            if (lcs== 1 ){
                DPrn(11,0,"k.");
                lc=Load_CFG('k', 'i',(CVT*) &LKBD[HKBD].cod );
            }
            if (lcs== 2 ){ lc=Load_CFG('k', 'i',(CVT*) &LKBD[HKBD].flg ); }
            if (lcs== 3 ){ lc=Load_CFG('k', 'c',(CVT*) &LKBD[HKBD].str ); }
            if (lc ==10 ){
                if (HKBD<MKBD){ HKBD++; }; DPrn(11,0,"\nk end=%d\n",HKBD);
            }
         }
         //------------------------------------------------------------------
         if (lcsec =='s'){
            if (lcs== 1 ){
                DPrn(11,0,"s.");
                lc=Load_CFG('s', 'i',(CVT*) &LSTR[HSTRC].sn  );
            }
            if (lcs== 2 ){ lc=Load_CFG('s', 'i',(CVT*) &LSTR[HSTRC].X   ); }
            if (lcs== 3 ){ lc=Load_CFG('s', 'i',(CVT*) &LSTR[HSTRC].Y   ); }
            if (lcs== 4 ){ lc=Load_CFG('s', 'i',(CVT*) &LSTR[HSTRC].cs  ); }
            if (lcs== 5 ){ lc=Load_CFG('s', 'i',(CVT*) &LSTR[HSTRC].cg  ); }
            if (lcs== 6 ){ lc=Load_CFG('s', 'i',(CVT*) &vSTR[HSTRC]     ); }
            if (lcs== 7 ){ lc=Load_CFG('s', 'c',(CVT*) &LSTR[HSTRC].str ); }
            if (lcs== 8 ){ 
                lc=Load_CFG('s', 'i',(CVT*) &LSTR[HSTRC].lcs );
                if(LSTR[HSTRC].lcs<1) LSTR[HSTRC].lcs=XMAX-1;
            }
            if (lc ==10 ){
                if (HSTRC<MSTR-1){ HSTRC++; }; DPrn(11,0,"\ns end=%d\n",HSTRC);
            }
         }
         //------------------------------------------------------------------


         //------------------------------------------------------------------
         if (lcsec =='v'){
            if (lcs== 1 ){
                DPrn(11,0,"v.");
                lc=Load_CFG('v', 'i',(CVT*) &CNVL); 
                if(CNVL>=MVAL){ CNVL=MVAL-1; }
            }
            if (lcs== 2 ){ lc=Load_CFG('v', 'i',(CVT*) &LVAL[CNVL].vw  ); }
            if (lcs== 3 ){ lc=Load_CFG('v', 'i',(CVT*) &LVAL[CNVL].na  ); }
            if (lcs== 4 ){
                if (LVAL[CNVL].na<0){
                    lc=Load_CFG('v','c',(CVT*) &SVAL); 
                    if(lc==5){ ValStr(CNVL,'v'); }
                }  else  {
                    lc=Load_CFG('v','i',(CVT*) &LVAL[CNVL].nv );
                }
            }
            if (lcs== 5 ){ lc=Load_CFG('v', 'i',(CVT*) &LVAL[CNVL].dc  ); }
            if (lcs== 6 ){ lc=Load_CFG('v', 'i',(CVT*) &LVAL[CNVL].zn  ); }
            if (lcs== 7 ){ lc=Load_CFG('v', 'i',(CVT*) &LVAL[CNVL].X   ); }
            if (lcs== 8 ){ lc=Load_CFG('v', 'i',(CVT*) &LVAL[CNVL].Y   ); }
            if (lcs== 9 ){ lc=Load_CFG('v', 'i',(CVT*) &LVAL[CNVL].W   ); }
            if (lcs==10 ){ lc=Load_CFG('v', 'i',(CVT*) &LVAL[CNVL].cs  ); }
            if (lcs==11 ){ lc=Load_CFG('v', 'i',(CVT*) &LVAL[CNVL].ce  ); }
            if (lcs==12 ){ lc=Load_CFG('v', 'i',(CVT*) &LVAL[CNVL].cg  ); }
            if (lcs==13 ){ lc=Load_CFG('v', 'i',(CVT*) &LVAL[CNVL].sn  ); }
            if (lcs==14 ){ lc=Load_CFG('v', 'c',(CVT*) &LVAL[CNVL].sf  ); }
            if (lc ==10 ){
                if (CNVL>=HVAL){ HVAL=CNVL+1; }
                if (HVAL>=MVAL){ HVAL=MVAL-1; }; DPrn(11,0,"\nv end=%d\n",HVAL);
            }
         }
         //------------------------------------------------------------------


         //------------------------------------------------------------------
         if (lcsec =='g'){
            if (lcs== 1 ){
                DPrn(11,0,"g.");
                lc=Load_CFG('g', 'i',(CVT*) &CNVL); 
                if(CNVL>=MGST){ CNVL=MGST-1; }
            }
            if (lcs== 2 ){ lc=Load_CFG('g', 'i',(CVT*) &LGST[CNVL].na  ); }
            if (lcs== 3 ){
                if (LGST[CNVL].na<0){
                    lc=Load_CFG('g', 'c',(CVT*) &SVAL); 
                    if(lc==5){ ValStr(CNVL,'g'); }
                }  else  {
                    lc=Load_CFG('g', 'i',(CVT*) &LGST[CNVL].nv  );
                }
            }
            if (lcs== 4 ){ lc=Load_CFG('g', 'f',(CVT*) &LGST[CNVL].min ); }
            if (lcs== 5 ){ lc=Load_CFG('g', 'f',(CVT*) &LGST[CNVL].max ); }
            if (lcs== 6 ){ lc=Load_CFG('g', 'i',(CVT*) &LGST[CNVL].X   ); }
            if (lcs== 7 ){ lc=Load_CFG('g', 'i',(CVT*) &LGST[CNVL].Y   ); }
            if (lcs== 8 ){ lc=Load_CFG('g', 'i',(CVT*) &LGST[CNVL].W   ); }
            if (lcs== 9 ){ lc=Load_CFG('g', 'i',(CVT*) &LGST[CNVL].H   ); }
            if (lcs==10 ){ lc=Load_CFG('g', 'i',(CVT*) &LGST[CNVL].n   ); }
            if (lcs==11 ){ lc=Load_CFG('g', 'i',(CVT*) &LGST[CNVL].cg  ); }
            if (lcs==12 ){ lc=Load_CFG('g', 'i',(CVT*) &LGST[CNVL].cs  ); }
            if (lcs==13 ){ lc=Load_CFG('g', 'i',(CVT*) &LGST[CNVL].b   ); }
            if (lcs==14 ){ lc=Load_CFG('g', 'i',(CVT*) &LGST[CNVL].cb  ); }
            if (lc ==10 ){
                if (CNVL>=HGST){ HGST=CNVL+1; }
                if (HGST>=MGST){ HGST=MGST-1; }; DPrn(11,0,"\ng end %d\n",HGST);
            }
         }
         //------------------------------------------------------------------


         //------------------------------------------------------------------
         if (lcsec =='t'){
            if (lcs== 1 ){
                DPrn(11,0,"t.");
                lc=Load_CFG('t', 'f',(CVT*) &LTRD[HTRD].min );
            }
            if (lcs== 2 ){
                lc=Load_CFG('t', 'f',(CVT*) &LTRD[HTRD].max );
                LTRD[HTRD].mind=LTRD[HTRD].min;
                LTRD[HTRD].mino=LTRD[HTRD].min;
            }
            if (lcs== 3 ){ lc=Load_CFG('t', 'f',(CVT*) &LTRD[HTRD].X1  ); }
            if (lcs== 4 ){ lc=Load_CFG('t', 'f',(CVT*) &LTRD[HTRD].Y1  ); }
            if (lcs== 5 ){ lc=Load_CFG('t', 'f',(CVT*) &LTRD[HTRD].W1  ); }
            if (lcs== 6 ){ lc=Load_CFG('t', 'f',(CVT*) &LTRD[HTRD].H1  ); }
            if (lcs== 7 ){
                lc=Load_CFG('t', 'f',(CVT*) &LTRD[HTRD].X2  );
                LTRD[HTRD].maxd=LTRD[HTRD].max;
                LTRD[HTRD].maxo=LTRD[HTRD].max;
            }
            if (lcs== 8 ){ lc=Load_CFG('t', 'f',(CVT*) &LTRD[HTRD].Y2  ); }
            if (lcs== 9 ){ lc=Load_CFG('t', 'f',(CVT*) &LTRD[HTRD].W2  ); }
            if (lcs==10 ){ lc=Load_CFG('t', 'f',(CVT*) &LTRD[HTRD].H2  ); }
            if (lcs==11 ){ lc=Load_CFG('t', 'i',(CVT*) &LTRD[HTRD].fs  ); }
            if (lcs==12 ){ lc=Load_CFG('t', 'i',(CVT*) &LTRD[HTRD].ft  ); }
            if (lcs==13 ){
                lc=Load_CFG('t', 'i',(CVT*) &LTRD[HTRD].sec );
                LTRD[HTRD].seco=LTRD[HTRD].sec;
            }
            if (lcs==14 ){ lc=Load_CFG('t', 'f',(CVT*) &LTRD[HTRD].stx ); }
            if (lcs==15 ){ lc=Load_CFG('t', 'f',(CVT*) &LTRD[HTRD].sty ); }
            if (lcs==16 ){ lc=Load_CFG('t', 'i',(CVT*) &LTRD[HTRD].rsa ); }
            if (lcs==17 ){ lc=Load_CFG('t', 'i',(CVT*) &LTRD[HTRD].hm  ); }
            if (lcs==18 ){ lc=Load_CFG('t', 'i',(CVT*) &LTRD[HTRD].g1  ); }
            if (lcs==19 ){ lc=Load_CFG('t', 'i',(CVT*) &LTRD[HTRD].g2  ); }
            if (lcs==20 ){ lc=Load_CFG('t', 'i',(CVT*) &LTRD[HTRD].cs  ); }
            if (lcs==21 ){ lc=Load_CFG('t', 'i',(CVT*) &LTRD[HTRD].cx  ); }
            if (lcs==22 ){ lc=Load_CFG('t', 'i',(CVT*) &LTRD[HTRD].cy  ); }
            if (lcs==23 ){ lc=Load_CFG('t', 'i',(CVT*) &LTRD[HTRD].cr  ); }
            if (lcs==24 ){ lc=Load_CFG('t', 'i',(CVT*) &LTRD[HTRD].th  ); }
            if (lcs==25 ){ lc=Load_CFG('t', 'c',(CVT*) &LTRD[HTRD].m[0] );}
            if (lcs==26 ){ lc=Load_CFG('t', 'i',(CVT*) &LTRD[HTRD].c[0] );}
            if (lcs==27 ){ lc=Load_CFG('t', 'i',(CVT*) &LTRD[HTRD].z[0] );}
            if (lcs==28 ){ lc=Load_CFG('t', 'f',(CVT*) &LTRD[HTRD].v[0] );}
            if (lcs==29 ){ lc=Load_CFG('t', 'c',(CVT*) &LTRD[HTRD].m[1] );}
            if (lcs==30 ){ lc=Load_CFG('t', 'i',(CVT*) &LTRD[HTRD].c[1] );}
            if (lcs==31 ){ lc=Load_CFG('t', 'i',(CVT*) &LTRD[HTRD].z[1] );}
            if (lcs==32 ){ lc=Load_CFG('t', 'f',(CVT*) &LTRD[HTRD].v[1] );}
            if (lcs==33 ){ lc=Load_CFG('t', 'c',(CVT*) &LTRD[HTRD].m[2] );}
            if (lcs==34 ){ lc=Load_CFG('t', 'i',(CVT*) &LTRD[HTRD].c[2] );}
            if (lcs==35 ){ lc=Load_CFG('t', 'i',(CVT*) &LTRD[HTRD].z[2] );}
            if (lcs==36 ){ lc=Load_CFG('t', 'f',(CVT*) &LTRD[HTRD].v[2] );}
            if (lcs==37 ){ lc=Load_CFG('t', 'c',(CVT*) &LTRD[HTRD].m[3] );}
            if (lcs==38 ){ lc=Load_CFG('t', 'i',(CVT*) &LTRD[HTRD].c[3] );}
            if (lcs==39 ){ lc=Load_CFG('t', 'i',(CVT*) &LTRD[HTRD].z[3] );}
            if (lcs==40 ){ lc=Load_CFG('t', 'f',(CVT*) &LTRD[HTRD].v[3] );}
            if (lcs==41 ){ lc=Load_CFG('t', 'c',(CVT*) &LTRD[HTRD].m[4] );}
            if (lcs==42 ){ lc=Load_CFG('t', 'i',(CVT*) &LTRD[HTRD].c[4] );}
            if (lcs==43 ){ lc=Load_CFG('t', 'i',(CVT*) &LTRD[HTRD].z[4] );}
            if (lcs==44 ){ lc=Load_CFG('t', 'f',(CVT*) &LTRD[HTRD].v[4] );}
            if (lcs==45 ){ lc=Load_CFG('t', 'c',(CVT*) &LTRD[HTRD].m[5] );}
            if (lcs==46 ){ lc=Load_CFG('t', 'i',(CVT*) &LTRD[HTRD].c[5] );}
            if (lcs==47 ){ lc=Load_CFG('t', 'i',(CVT*) &LTRD[HTRD].z[5] );}
            if (lcs==48 ){ lc=Load_CFG('t', 'f',(CVT*) &LTRD[HTRD].v[5] );}
            if (lcs==49 ){ lc=Load_CFG('t', 'c',(CVT*) &LTRD[HTRD].m[6] );}
            if (lcs==50 ){ lc=Load_CFG('t', 'i',(CVT*) &LTRD[HTRD].c[6] );}
            if (lcs==51 ){ lc=Load_CFG('t', 'i',(CVT*) &LTRD[HTRD].z[6] );}
            if (lcs==52 ){ lc=Load_CFG('t', 'f',(CVT*) &LTRD[HTRD].v[6] );}
            if (lcs==53 ){ lc=Load_CFG('t', 'c',(CVT*) &LTRD[HTRD].m[7] );}
            if (lcs==54 ){ lc=Load_CFG('t', 'i',(CVT*) &LTRD[HTRD].c[7] );}
            if (lcs==55 ){ lc=Load_CFG('t', 'i',(CVT*) &LTRD[HTRD].z[7] );}
            if (lcs==56 ){ lc=Load_CFG('t', 'f',(CVT*) &LTRD[HTRD].v[7] );}
            if (lcs==57 ){ lc=Load_CFG('t', 'c',(CVT*) &LTRD[HTRD].m[8] );}
            if (lcs==58 ){ lc=Load_CFG('t', 'i',(CVT*) &LTRD[HTRD].c[8] );}
            if (lcs==59 ){ lc=Load_CFG('t', 'i',(CVT*) &LTRD[HTRD].z[8] );}
            if (lcs==60 ){ lc=Load_CFG('t', 'f',(CVT*) &LTRD[HTRD].v[8] );}
            if (lcs==61 ){ lc=Load_CFG('t', 'c',(CVT*) &LTRD[HTRD].m[9] );}
            if (lcs==62 ){ lc=Load_CFG('t', 'i',(CVT*) &LTRD[HTRD].c[9] );}
            if (lcs==63 ){ lc=Load_CFG('t', 'i',(CVT*) &LTRD[HTRD].z[9] );}
            if (lcs==64 ){ lc=Load_CFG('t', 'f',(CVT*) &LTRD[HTRD].v[9] );}
            if (lc ==10 ){
                ValStr(HTRD,'t');
                if (HTRD<MTRD-1){ HTRD++; }; DPrn(11,0,"\nt end %d\n",HTRD);
            }
         }
         //------------------------------------------------------------------


         //------------------------------------------------------------------
         if (lcsec =='C'){
            if (lcs== 1 ){
                DPrn(11,0,"C.");
                lc=Load_CFG('C', 'i',(CVT*) &CNVL); 
                if(CNVL>=MCST){ CNVL=MCST-1; }
            }
            if (lcs== 2 ){ lc=Load_CFG('C', 'i',(CVT*) &LCST[CNVL].vw  ); }
            if (lcs== 3 ){ lc=Load_CFG('C', 'i',(CVT*) &LCST[CNVL].na  ); }
            if (lcs== 4 ){
                if (LCST[CNVL].na<0){
                    lc=Load_CFG('C', 'c',(CVT*) &SVAL); 
                    if(lc==5){ ValStr(CNVL,'C'); }
                }  else  {
                    lc=Load_CFG('C', 'i',(CVT*) &LCST[CNVL].nv  );
                }
            }
            if (lcs== 5 ){ lc=Load_CFG('C', 'i',(CVT*) &LCST[CNVL].X   ); }
            if (lcs== 6 ){ lc=Load_CFG('C', 'i',(CVT*) &LCST[CNVL].Y   ); }
            if (lcs== 7 ){ lc=Load_CFG('C', 'i',(CVT*) &LCST[CNVL].W   ); }
            if (lcs== 8 ){ lc=Load_CFG('C', 'i',(CVT*) &LCST[CNVL].sn  ); }
            if (lcs== 9 ){ lc=Load_CFG('C', 'i',(CVT*) &LCST[CNVL].cs  ); }
            if (lcs==10 ){ lc=Load_CFG('C', 'i',(CVT*) &LCST[CNVL].cg  ); }
            if (lcs==11 ){ lc=Load_CFG('C', 'c',(CVT*) &LCST[CNVL].C[0]); }
            if (lcs==12 ){ lc=Load_CFG('C', 'c',(CVT*) &LCST[CNVL].C[1]); }
            if (lcs==13 ){ lc=Load_CFG('C', 'c',(CVT*) &LCST[CNVL].C[2]); }
            if (lcs==14 ){ lc=Load_CFG('C', 'c',(CVT*) &LCST[CNVL].C[3]); }
            if (lcs==15 ){ lc=Load_CFG('C', 'c',(CVT*) &LCST[CNVL].C[4]); }
            if (lcs==16 ){ lc=Load_CFG('C', 'c',(CVT*) &LCST[CNVL].C[5]); }
            if (lcs==17 ){ lc=Load_CFG('C', 'c',(CVT*) &LCST[CNVL].C[6]); }
            if (lcs==18 ){ lc=Load_CFG('C', 'c',(CVT*) &LCST[CNVL].C[7]); }
            if (lcs==19 ){ lc=Load_CFG('C', 'c',(CVT*) &LCST[CNVL].C[8]); }
            if (lcs==20 ){ lc=Load_CFG('C', 'c',(CVT*) &LCST[CNVL].C[9]); }
            if (lc ==10 ){
                if (CNVL>=HCST){ HCST=CNVL+1; }
                if (HCST>=MCST){ HCST=MCST;   }; DPrn(11,0,"\nC end %d\n",HCST);
            }
         }
         //------------------------------------------------------------------


         //------------------------------------------------------------------
         if (lcsec =='F'){
            if (lcs== 1 ){ lc=Load_CFG('F', 'i',(CVT*) &LFST.na  ); }
            if (lcs== 2 ){
                if (LFST.na<0){
                    lc=Load_CFG('F', 'c',(CVT*) &SVAL); 
                    if(lc==5){ ValStr(0,'F'); }
                }  else  {
                    lc=Load_CFG('F', 'i',(CVT*) &LFST.nv  );
                }
            }
            if (lcs== 3 ){ lc=Load_CFG('F', 'i',(CVT*) &LFST.X  ); }
            if (lcs== 4 ){ lc=Load_CFG('F', 'i',(CVT*) &LFST.Y  ); }
            if (lcs== 5 ){ lc=Load_CFG('F', 'i',(CVT*) &LFST.W  ); }
            if (lcs== 6 ){ lc=Load_CFG('F', 'i',(CVT*) &LFST.sn ); }
            if (lcs== 7 ){ lc=Load_CFG('F', 'i',(CVT*) &LFST.cs ); }
            if (lcs== 8 ){ lc=Load_CFG('F', 'i',(CVT*) &LFST.cg ); }
            if (lcs== 9 ){ lc=Load_CFG('F', 'i',(CVT*) &LFST.rf ); }
            if (lcs==10 ){ lc=Load_CFG('F', 'i',(CVT*) &LFST.rh ); }
            if (lcs==11 ){ lc=Load_CFG('F', 'c',(CVT*) &LFST.fn ); }
            if (lc ==10 ){
                FFST= 1;
            }
         }
         //------------------------------------------------------------------


         //------------------------------------------------------------------
         if (lcsec =='o'){
            if (lcs== 1 ){
                DPrn(11,0,"o.");
                lc=Load_CFG('o', 'i',(CVT*) &CNVL); 
                if(CNVL>=MREP){ CNVL=MREP-1; }
            }
            if (lcs== 2 ){ lc=Load_CFG('o', 'i',(CVT*) &LREP[CNVL].na  ); }
            if (lcs== 3 ){
                if (LREP[CNVL].na<0){
                    lc=Load_CFG('o','c',(CVT*) &SVAL); 
                    if(lc==5){ ValStr(CNVL,'o'); }
                }  else  {
                    lc=Load_CFG('o','i',(CVT*) &LREP[CNVL].nv );
                }
            }
            if (lcs== 4 ){ lc=Load_CFG('o', 'c',(CVT*) &LREP[CNVL].ff ); }
            if (lcs== 5 ){ lc=Load_CFG('o', 'c',(CVT*) &LREP[CNVL].fo ); }
            if (lc ==10 ){
                if (CNVL>=HREP){ HREP=CNVL+1; }
                if (HREP>=MREP){ HREP=MREP-1; }; DPrn(11,0,"\no end=%d\n",HREP);
            }
         }
         //------------------------------------------------------------------


         //------------------------------------------------------------------
         if (lcsec =='r'){
            if (lcs== 1 ){
                DPrn(11,0,"r.");
                lc=Load_CFG('r', 'i',(CVT*) &LRUN[HRUN].vw  );
            }
            if (lcs== 2 ){ lc=Load_CFG('r', 'i',(CVT*) &LRUN[HRUN].md  ); }
            if (lcs== 3 ){ lc=Load_CFG('r', 'c',(CVT*) &LRUN[HRUN].prg ); }
            if (lc ==10 ){
                if (HRUN<MRUN-1){ HRUN++; }; DPrn(11,0,"\nr end %d\n",HRUN);
            }
         }
         //------------------------------------------------------------------
         if (lcsec =='i'){
            if (lcs== 1 ){
                DPrn(11,0,"i.");
                lc=Load_CFG('i', 'c',(CVT*) &LINI[HINI].fn  );
            }
            if (lcs== 2 ){ lc=Load_CFG('i', 'i',(CVT*) &LINI[HINI].vw  ); }
            if (lcs== 3 ){ lc=Load_CFG('i', 'i',(CVT*) &LINI[HINI].nn  ); }
            if (lc ==10 ){
                // заполняем массив ini файлов включаемых для выполнения
                // на текущем экране(нужно для редактора)
                if(LINI[HINI].vw==2){
                   strcpy(El_ifn[El_hii],LINI[HINI].fn); 
                   El_inn[El_hii]=LINI[HINI].nn; if(El_cii<MINI-2) El_hii++;;
                }
                if (HINI<MINI-1){ HINI++; }; DPrn(11,0,"\ni end %d\n",HINI);
            }
         }
         //------------------------------------------------------------------
         if (lcsec =='l'){
            if (lcs== 1 ){
                DPrn(11,0,"l.");
                lc=Load_CFG('l', 'i',(CVT*) &LPCX.X );
            }
            if (lcs== 2 ){ lc=Load_CFG('l', 'i',(CVT*) &LPCX.Y   ); }
            if (lcs== 3 ){ lc=Load_CFG('l', 'i',(CVT*) &LPCX.W   ); }
            if (lcs== 4 ){ lc=Load_CFG('l', 'i',(CVT*) &LPCX.H   ); }
            if (lcs== 5 ){ lc=Load_CFG('l', 'i',(CVT*) &LPCX.vw  ); }
            if (lcs== 6 ){ lc=Load_CFG('l', 'c',(CVT*) &LPCX.fn  ); }
            if (lc ==10 ){
                if (LPCX.vw==1){ MLoad_PIC(0); BFirst=1; ReDraw=1; }; DPrn(11,0,"\nl end\n");
            }
         }
         //------------------------------------------------------------------
         if (lcsec =='w'){
            if (lcs== 1 ){
                DPrn(11,0,"w.");
                lc=Load_CFG('w', 'i',(CVT*) &CNVL); 
                if(CNVL>=MWIN){ CNVL=MWIN-1; }
            }
            if (lcs== 2 ){ lc=Load_CFG('w', 'i',(CVT*) &LWIN[CNVL].X   ); }
            if (lcs== 3 ){ lc=Load_CFG('w', 'i',(CVT*) &LWIN[CNVL].Y   ); }
            if (lcs== 4 ){ lc=Load_CFG('w', 'i',(CVT*) &LWIN[CNVL].W   ); }
            if (lcs== 5 ){ lc=Load_CFG('w', 'i',(CVT*) &LWIN[CNVL].H   ); }
            if (lcs== 6 ){ lc=Load_CFG('w', 'i',(CVT*) &LWIN[CNVL].vw  ); }
            if (lcs== 7 ){ lc=Load_CFG('w', 'c',(CVT*) &LWIN[CNVL].fn  ); }
            if (lc ==10 ){
                if (CNVL>=HWIN){ HWIN=CNVL+1; }
                if (HWIN>=MWIN){ HWIN=MWIN-1; }; DPrn(11,0,"\nw end %d\n",HWIN);
            }
         }
         //------------------------------------------------------------------


         //------------------------------------------------------------------
         if (lcsec =='P'){
            if (lcs== 1 ){
                DPrn(11,0,"P.");
                lc=Load_CFG('P', 'i',(CVT*) &CNVL); 
                if(CNVL>=MSPR){ CNVL=MSPR-1; }
            }
            if (lcs== 2 ){ lc=Load_CFG('P', 'i',(CVT*) &LSPR[CNVL].X   ); }
            if (lcs== 3 ){ lc=Load_CFG('P', 'i',(CVT*) &LSPR[CNVL].Y   ); }
            if (lcs== 4 ){ lc=Load_CFG('P', 'i',(CVT*) &LSPR[CNVL].W   ); }
            if (lcs== 5 ){ lc=Load_CFG('P', 'i',(CVT*) &LSPR[CNVL].H   ); }
            if (lcs== 6 ){ lc=Load_CFG('P', 'i',(CVT*) &LSPR[CNVL].vw  ); }
            if (lcs== 7 ){ lc=Load_CFG('P', 'c',(CVT*) &LSPR[CNVL].fn  ); }
            if (lcs== 8 ){ lc=Load_CFG('P', 'i',(CVT*) &LSPR[CNVL].SX  ); }
            if (lcs== 9 ){ lc=Load_CFG('P', 'i',(CVT*) &LSPR[CNVL].SY  ); }
            if (lc ==10 ){
                if (CNVL>=HSPR){ HSPR=CNVL+1; }
                if (HSPR>=MSPR){ HSPR=MSPR-1; }; DPrn(11,0,"\nP end %d\n",HSPR);
            }
         }
         //------------------------------------------------------------------


         //------------------------------------------------------------------
         if (lcsec =='a'){
            if (lcs== 1 ){
                DPrn(11,0,"a.");
                lc=Load_CFG('a', 'i',(CVT*) &CNVL); 
                if(CNVL>=MANI){ CNVL=MANI-1; }
            }
            if (lcs== 2 ){ lc=Load_CFG('a', 'i',(CVT*) &LANI[CNVL].vw ); }
            if (lcs== 3 ){ lc=Load_CFG('a', 'i',(CVT*) &LANI[CNVL].na ); }
            if (lcs== 4 ){
                if (LANI[CNVL].na<0){
                    lc=Load_CFG('a', 'c',(CVT*) &SVAL); 
                    if(lc==5){ ValStr(CNVL,'a'); }
                }  else  {
                    lc=Load_CFG('a', 'i',(CVT*) &LANI[CNVL].nv  );
                }
            }
            if (lcs== 5 ){ lc=Load_CFG('a', 'i',(CVT*) &LANI[CNVL].nx  ); }
            if (lcs== 6 ){ lc=Load_CFG('a', 'i',(CVT*) &LANI[CNVL].ny  ); }
            if (lcs== 7 ){ lc=Load_CFG('a', 'i',(CVT*) &LANI[CNVL].nf  ); }
            if (lcs== 8 ){ lc=Load_CFG('a', 'i',(CVT*) &LANI[CNVL].X   ); }
            if (lcs== 9 ){ lc=Load_CFG('a', 'i',(CVT*) &LANI[CNVL].Y   ); }
            if (lcs==10 ){ lc=Load_CFG('a', 'i',(CVT*) &LANI[CNVL].XM  ); }
            if (lcs==11 ){ lc=Load_CFG('a', 'i',(CVT*) &LANI[CNVL].YM  ); }
            if (lcs==12 ){ lc=Load_CFG('a', 'i',(CVT*) &LANI[CNVL].W   ); }
            if (lcs==13 ){ lc=Load_CFG('a', 'i',(CVT*) &LANI[CNVL].H   ); }
            if (lcs==14 ){ lc=Load_CFG('a', 'i',(CVT*) &LANI[CNVL].hp  ); }
            if (lcs==15 ){ lc=Load_CFG('a', 'c',(CVT*) &LANI[CNVL].fp  ); }
            if (lc ==10 ){
                if (CNVL>=HANI){ HANI=CNVL+1; }
                if (HANI>=MANI){ HANI=MANI-1; }; DPrn(11,0,"\na end %d\n",HANI);
            }
         }
         //------------------------------------------------------------------


         //------------------------------------------------------------------
         if (lcsec =='p'){
            if (lcs== 1 ){
                DPrn(11,0,"p.");
                lc=Load_CFG('p', 'i',(CVT*) &CNVL); 
                if(CNVL>=MPLZ){ CNVL=MPLZ-1; }
            }
            if (lcs== 2 ){ lc=Load_CFG('p', 'i',(CVT*) &LPLZ[CNVL].na  ); }
            if (lcs== 3 ){
                if (LPLZ[CNVL].na<0){
                    lc=Load_CFG('p', 'c',(CVT*) &SVAL); 
                    if(lc==5){ ValStr(CNVL,'p'); }
                }  else  {
                    lc=Load_CFG('p', 'i',(CVT*) &LPLZ[CNVL].nv  );
                }
            }
            if (lcs== 4 ){ lc=Load_CFG('p', 'f',(CVT*) &LPLZ[CNVL].min ); }
            if (lcs== 5 ){ lc=Load_CFG('p', 'f',(CVT*) &LPLZ[CNVL].max ); }
            if (lcs== 6 ){ lc=Load_CFG('p', 'f',(CVT*) &LPLZ[CNVL].X   ); }
            if (lcs== 7 ){ lc=Load_CFG('p', 'f',(CVT*) &LPLZ[CNVL].Y   ); }
            if (lcs== 8 ){ lc=Load_CFG('p', 'f',(CVT*) &LPLZ[CNVL].L   ); }
            if (lcs== 9 ){ lc=Load_CFG('p', 'i',(CVT*) &LPLZ[CNVL].W   ); }
            if (lcs==10 ){ lc=Load_CFG('p', 'i',(CVT*) &LPLZ[CNVL].H   ); }
            if (lcs==11 ){ lc=Load_CFG('p', 'i',(CVT*) &LPLZ[CNVL].G   ); }
            if (lcs==12 ){ lc=Load_CFG('p', 'i',(CVT*) &LPLZ[CNVL].cb  ); }
            if (lcs==13 ){ lc=Load_CFG('p', 'i',(CVT*) &LPLZ[CNVL].tb  ); }
            if (lcs==14 ){ lc=Load_CFG('p', 'c',(CVT*) &LPLZ[CNVL].fb  ); }
            if (lcs==15 ){ lc=Load_CFG('p', 'c',(CVT*) &LPLZ[CNVL].STR ); }
            if (lc ==10 ){
                if (CNVL>=HPLZ){ HPLZ=CNVL+1; }
                if (HPLZ>=MPLZ){ HPLZ=MPLZ-1; }; DPrn(11,0,"\np end %d\n",HPLZ);
            }
         }
         //------------------------------------------------------------------


         //------------------------------------------------------------------
         if (lcsec =='h'){
            if (lcs== 1 ){
                DPrn(11,0,"h.");
                lc=Load_CFG('h', 'f',(CVT*) &LSHK[HSHK].X   );
            }
            if (lcs== 2 ){ lc=Load_CFG('h', 'f',(CVT*) &LSHK[HSHK].Y   ); }
            if (lcs== 3 ){ lc=Load_CFG('h', 'f',(CVT*) &LSHK[HSHK].W   ); }
            if (lcs== 4 ){ lc=Load_CFG('h', 'f',(CVT*) &LSHK[HSHK].H   ); }
            if (lcs== 5 ){ lc=Load_CFG('h', 'f',(CVT*) &LSHK[HSHK].mn  ); }
            if (lcs== 6 ){ lc=Load_CFG('h', 'f',(CVT*) &LSHK[HSHK].mx  ); }
            if (lcs== 7 ){ lc=Load_CFG('h', 'i',(CVT*) &LSHK[HSHK].h   ); }
            if (lcs== 8 ){ lc=Load_CFG('h', 'i',(CVT*) &LSHK[HSHK].fl  ); }
            if (lcs== 9 ){ lc=Load_CFG('h', 'i',(CVT*) &LSHK[HSHK].dc  ); }
            if (lcs==10 ){ lc=Load_CFG('h', 'i',(CVT*) &LSHK[HSHK].G   ); }
            if (lcs==11 ){ lc=Load_CFG('h', 'i',(CVT*) &LSHK[HSHK].sn  ); }
            if (lcs==12 ){ lc=Load_CFG('h', 'i',(CVT*) &LSHK[HSHK].cs  ); }
            if (lcs==13 ){ lc=Load_CFG('h', 'i',(CVT*) &LSHK[HSHK].cg  ); }
            if (lcs==14 ){ lc=Load_CFG('h', 'c',(CVT*) &LSHK[HSHK].fr  ); }
            if (lc ==10 ){
                if (HSHK<MSHK-1){ HSHK++; }; DPrn(11,0,"\nh end %d\n",HSHK);
            }
         }
         //------------------------------------------------------------------


         //------------------------------------------------------------------
         if (lcsec =='G'){
            if (lcs== 1 ){
                DPrn(11,0,"G.");
                lc=Load_CFG('G', 'i',(CVT*) &CNVL); 
                if(CNVL>=MGET){ CNVL=MGET-1; }
            }
            if (lcs== 2 ){ lc=Load_CFG('G', 'i',(CVT*) &LGET[CNVL].vw  ); }
            if (lcs== 3 ){ lc=Load_CFG('G', 'i',(CVT*) &LGET[CNVL].na  ); }
            if (lcs== 4 ){
                if (LGET[CNVL].na<0){
                    lc=Load_CFG('G','c',(CVT*) &SVAL); 
                    if(lc==5){ ValStr(CNVL,'G'); }
                }  else  {
                    lc=Load_CFG('G','i',(CVT*) &LGET[CNVL].nv );
                }
            }
            if (lcs== 5 ){ lc=Load_CFG('G', 'i',(CVT*) &LGET[CNVL].dc  ); }
            if (lcs== 6 ){ lc=Load_CFG('G', 'i',(CVT*) &LGET[CNVL].hc  ); }
            if (lcs== 7 ){ lc=Load_CFG('G', 'f',(CVT*) &LGET[CNVL].X   ); }
            if (lcs== 8 ){ lc=Load_CFG('G', 'f',(CVT*) &LGET[CNVL].Y   ); }
            if (lcs== 9 ){ lc=Load_CFG('G', 'f',(CVT*) &LGET[CNVL].W   ); }
            if (lcs==10 ){ lc=Load_CFG('G', 'i',(CVT*) &LGET[CNVL].cs  ); }
            if (lcs==11 ){ lc=Load_CFG('G', 'i',(CVT*) &LGET[CNVL].cr  ); }
            if (lcs==12 ){ lc=Load_CFG('G', 'i',(CVT*) &LGET[CNVL].cg  ); }
            if (lcs==13 ){ lc=Load_CFG('G', 'i',(CVT*) &LGET[CNVL].sn  ); }
            if (lcs==14 ){ lc=Load_CFG('G', 'c',(CVT*) &LGET[CNVL].sf  ); }
            if (lc ==10 ){
                if (CNVL>=HGET){ HGET=CNVL+1; }
                if (HGET>=MGET){ HGET=MGET-1; }; DPrn(11,0,"\nG end %d\n",HGET);
            }
         }
         //------------------------------------------------------------------


         //------------------------------------------------------------------
         if (lcsec =='c'){
            if (lcs== 1 ){
                DPrn(11,0,"c.");
                lc=Load_CFG('c', 'i',(CVT*) &CNVL); 
                if(CNVL>=MXCV){ CNVL=MXCV-1; }
            }
            if (lcs== 2 ){ lc=Load_CFG('c', 'i',(CVT*) &LXCV[CNVL].na  ); }
            if (lcs== 3 ){
                if (LXCV[CNVL].na<0){
                    lc=Load_CFG('c', 'c',(CVT*) &SVAL); 
                    if(lc==5){ ValStr(CNVL,'c'); }
                }  else  {
                    lc=Load_CFG('c', 'i',(CVT*) &LXCV[CNVL].nv  );
                }
            }
            if (lcs== 4 ){ lc=Load_CFG('c', 'i',(CVT*) &LXCV[CNVL].zn  ); }
            if (lcs== 5 ){ lc=Load_CFG('c', 'f',(CVT*) &LXCV[CNVL].vc  ); }
            if (lc ==10 ){
                if (CNVL>=HXCV){ HXCV=CNVL+1; }
                if (HXCV>=MXCV){ HXCV=MXCV-1; }; DPrn(11,0,"\nc end %d\n",HXCV);
            }
         }
         //------------------------------------------------------------------


         //------------------------------------------------------------------
         if (lcsec =='f'){
            if (lcs== 1 ){
                DPrn(11,0,"f.");
                lc=Load_CFG('f', 'i',(CVT*) &LFVW[HFVW].vw  );
            }
            if (lcs== 2 ){ lc=Load_CFG('f', 'f',(CVT*) &LFVW[HFVW].X   ); }
            if (lcs== 3 ){ lc=Load_CFG('f', 'f',(CVT*) &LFVW[HFVW].Y   ); }
            if (lcs== 4 ){ lc=Load_CFG('f', 'f',(CVT*) &LFVW[HFVW].W   ); }
            if (lcs== 5 ){ lc=Load_CFG('f', 'f',(CVT*) &LFVW[HFVW].H   ); }
            if (lcs== 6 ){ lc=Load_CFG('f', 'f',(CVT*) &LFVW[HFVW].L   ); }
            if (lcs== 7 ){ lc=Load_CFG('f', 'f',(CVT*) &LFVW[HFVW].R   ); }
            if (lcs== 8 ){ lc=Load_CFG('f', 'i',(CVT*) &LFVW[HFVW].ps  ); }
            if (lcs== 9 ){ lc=Load_CFG('f', 'i',(CVT*) &LFVW[HFVW].fd  ); }
            if (lcs==10 ){ lc=Load_CFG('f', 'i',(CVT*) &LFVW[HFVW].ws  ); }
            if (lcs==11 ){ lc=Load_CFG('f', 'i',(CVT*) &LFVW[HFVW].spp ); }
            if (lcs==12 ){ lc=Load_CFG('f', 'i',(CVT*) &LFVW[HFVW].sn  ); }
            if (lcs==13 ){ lc=Load_CFG('f', 'i',(CVT*) &LFVW[HFVW].cs  ); }
            if (lcs==14 ){ lc=Load_CFG('f', 'i',(CVT*) &LFVW[HFVW].cg  ); }
            if (lcs==15 ){ lc=Load_CFG('f', 'c',(CVT*) &LFVW[HFVW].fn  ); }
            if (lcs==16 ){ lc=Load_CFG('f', 'i',(CVT*) &LFVW[HFVW].cd  ); }
            if (lcs==17 ){ lc=Load_CFG('f', 'i',(CVT*) &LFVW[HFVW].cc  ); }
            if (lcs==18 ){ lc=Load_CFG('f', 'i',(CVT*) &LFVW[HFVW].cr  ); }
            if (lcs==19 ){ lc=Load_CFG('f', 'i',(CVT*) &LFVW[HFVW].cp  ); }
            if (lcs==20 ){ lc=Load_CFG('f', 'f',(CVT*) &LFVW[HFVW].yt  ); }
            if (lcs==21 ){ lc=Load_CFG('f', 'i',(CVT*) &LFVW[HFVW].f1  ); }
            if (lcs==22 ){ lc=Load_CFG('f', 'i',(CVT*) &LFVW[HFVW].f2  ); }
            if (lcs==23 ){ lc=Load_CFG('f', 'i',(CVT*) &LFVW[HFVW].xc  ); }
            if (lcs==24 ){ lc=Load_CFG('f', 'i',(CVT*) &LFVW[HFVW].xx  ); }
            if (lcs==25 ){ lc=Load_CFG('f', 'i',(CVT*) &LFVW[HFVW].xy  ); }
            if (lcs==26 ){ lc=Load_CFG('f', 'i',(CVT*) &LFVW[HFVW].xw  ); }
            if (lcs==27 ){ lc=Load_CFG('f', 'i',(CVT*) &LFVW[HFVW].xh  ); }
            if (lcs==28 ){ lc=Load_CFG('f', 'i',(CVT*) &LFVW[HFVW].kl  ); }
            if (lcs==29 ){ lc=Load_CFG('f', 'i',(CVT*) &LFVW[HFVW].kr  ); }
            if (lc ==10 ){
                if (HFVW<MFVW-1){ HFVW++; }; DPrn(11,0,"\nf end %d\n",HFVW);
            }
         }
         //------------------------------------------------------------------


         //------------------------------------------------------------------
         if (lcsec =='b'){
            if (lcs== 1 ){
                DPrn(11,0,"b.");
                lc=Load_CFG('b', 'i',(CVT*) &LBOX[HBOX].vw  );
            }
            if (lcs== 2 ){ lc=Load_CFG('b', 'i',(CVT*) &LBOX[HBOX].X   ); }
            if (lcs== 3 ){ lc=Load_CFG('b', 'i',(CVT*) &LBOX[HBOX].Y   ); }
            if (lcs== 4 ){ lc=Load_CFG('b', 'i',(CVT*) &LBOX[HBOX].W   ); }
            if (lcs== 5 ){ lc=Load_CFG('b', 'i',(CVT*) &LBOX[HBOX].H   ); }
            if (lcs== 6 ){ lc=Load_CFG('b', 'i',(CVT*) &LBOX[HBOX].f   ); }
            if (lcs== 7 ){ lc=Load_CFG('b', 'i',(CVT*) &LBOX[HBOX].cs  ); }
            if (lcs== 8 ){ lc=Load_CFG('b', 'i',(CVT*) &LBOX[HBOX].n   ); }
            if (lc ==10 ){
                if (HBOX<MBOX-1){ HBOX++; }; DPrn(11,0,"\nb end %d\n",HBOX);
            }
         }
         //------------------------------------------------------------------
         if (lcsec =='B'){
            if (lcs== 1 ){
                DPrn(11,0,"B.");
                lc=Load_CFG('B', 'i',(CVT*) &CNVL); 
                if(CNVL>=MFVB){ CNVL=MFVB-1; }
            }
            if (lcs== 2 ){ lc=Load_CFG('B', 'i',(CVT*) &LFVB[CNVL].dbf ); }
            if (lcs== 3 ){ lc=Load_CFG('B', 'c',(CVT*) &LFVB[CNVL].fld ); }
            if (lcs== 4 ){ lc=Load_CFG('B', 'c',(CVT*) &LFVB[CNVL].val ); }
            if (lc ==10 ){
                if (CNVL>=HFVB){ HFVB=CNVL+1; }
                if (HFVB>=MFVB){ HFVB=MFVB-1; }; DPrn(11,0,"\np end %d\n",HFVB);
            }
         }
         //------------------------------------------------------------------


         //------------------------------------------------------------------
         if (lcsec =='z'){
            if (lcs== 1 ){ lc=Load_CFG('z', 'i',(CVT*) &LZRO.X  ); }
            if (lcs== 2 ){ lc=Load_CFG('z', 'i',(CVT*) &LZRO.Y  ); }
            if (lcs== 3 ){ lc=Load_CFG('z', 'i',(CVT*) &LZRO.W  ); }
            if (lcs== 4 ){ lc=Load_CFG('z', 'i',(CVT*) &LZRO.H  ); }
            if (lcs== 5 ){ lc=Load_CFG('z', 'i',(CVT*) &LZRO.cs ); }
            HZRO=1;
         }
         //------------------------------------------------------------------
         if (lcsec =='S'){
            if (lcs== 1 ){ lc=Load_CFG('S', 'i',(CVT*) &STAT[CFirst].sn  ); }
            if (lcs== 2 ){ lc=Load_CFG('S', 'i',(CVT*) &STAT[CFirst].X   ); }
            if (lcs== 3 ){ lc=Load_CFG('S', 'i',(CVT*) &STAT[CFirst].Y   ); }
            if (lcs== 4 ){ lc=Load_CFG('S', 'i',(CVT*) &STAT[CFirst].cs  ); }
            if (lcs== 5 ){ lc=Load_CFG('S', 'i',(CVT*) &STAT[CFirst].cg  ); }
            if (lcs== 6 ){ lc=Load_CFG('S', 'i',(CVT*) &STAT[CFirst].lcs ); }
            HSTA=1;
         }
         //------------------------------------------------------------------
         if (lcsec =='M'){
            if (lcs== 1 ){ lc=Load_CFG('M', 'i',(CVT*) &MESS[CFirst].sn  ); }
            if (lcs== 2 ){ lc=Load_CFG('M', 'i',(CVT*) &MESS[CFirst].X   ); }
            if (lcs== 3 ){ lc=Load_CFG('M', 'i',(CVT*) &MESS[CFirst].Y   ); }
            if (lcs== 4 ){ lc=Load_CFG('M', 'i',(CVT*) &MESS[CFirst].cs  ); }
            if (lcs== 5 ){ lc=Load_CFG('M', 'i',(CVT*) &MESS[CFirst].cg  ); }
            if (lcs== 6 ){ lc=Load_CFG('M', 'i',(CVT*) &MESS[CFirst].lcs ); }
            if (lcs== 7 ){
                lc=Load_CFG('M', 'i',(CVT*) &TMSS);
                if(TMSS<60){ TMSS=60; }; if(TMSS>3600){ TMSS=3600; }
            }
            HMSS=1;
         }
         //------------------------------------------------------------------
         if (lcsec =='D'){
            if (lcs== 1 ){ lc=Load_CFG('D', 'i',(CVT*) &VKDATE[CFirst].sn  ); }
            if (lcs== 2 ){ lc=Load_CFG('D', 'i',(CVT*) &VKDATE[CFirst].X   ); }
            if (lcs== 3 ){ lc=Load_CFG('D', 'i',(CVT*) &VKDATE[CFirst].Y   ); }
            if (lcs== 4 ){ lc=Load_CFG('D', 'i',(CVT*) &VKDATE[CFirst].cs  ); }
            if (lcs== 5 ){ lc=Load_CFG('D', 'i',(CVT*) &VKDATE[CFirst].cg  ); }
            if (lcs== 6 ){ lc=Load_CFG('D', 'i',(CVT*) &VKDATE[CFirst].lcs ); }
            HDAT=1;
         }
         //------------------------------------------------------------------
         if (lcsec =='T'){
            if (lcs== 1 ){ lc=Load_CFG('T', 'i',(CVT*) &VKTIME[CFirst].sn  ); }
            if (lcs== 2 ){ lc=Load_CFG('T', 'i',(CVT*) &VKTIME[CFirst].X   ); }
            if (lcs== 3 ){ lc=Load_CFG('T', 'i',(CVT*) &VKTIME[CFirst].Y   ); }
            if (lcs== 4 ){ lc=Load_CFG('T', 'i',(CVT*) &VKTIME[CFirst].cs  ); }
            if (lcs== 5 ){ lc=Load_CFG('T', 'i',(CVT*) &VKTIME[CFirst].cg  ); }
            if (lcs== 6 ){ lc=Load_CFG('T', 'i',(CVT*) &VKTIME[CFirst].lcs ); }
            HTIM=1;
         }
         //------------------------------------------------------------------
         if (lcsec =='d'){
            if (lcs== 0){
               for (a=0;a<MDB ;a++){ Deb[a]=0; }
            }
            if (lcs== CDB){ lc=Load_CFG('d', 'i',(CVT*) &Deb[CDB] ); }
            if (lc == 5 || lc == 10 ){
                if (CDB<MDB-1){ CDB++; }
            }
         }
         //------------------------------------------------------------------

         if (lc == 5 ){ lcs++;    lc=1; }
         if (lc ==10 ){ ClsFlg(); lc=1; }
      }
   }
   Init_CFG(0);

   //------------------------------------------------------------------------
   // автоматическая загрузка ini файла
   BZero=0; FNCfgLD=0;
   for(b=0; b<HINI; b++){
      if (LINI[b].vw>0){
          if ((strcmp(LINI[b].fn,FNCfg))!=0){
               strcpy(FNCfg,LINI[b].fn); FNCfgLD=1;
               if (LINI[b].vw==2){
                   BZero=1; LINI[b].vw=0; return(2); 
               }
          }
      }
      if (LINI[b].nn<MINI){
          strcpy(SINI[LINI[b].nn].fn, LINI[b].fn);
          SINI[LINI[b].nn].vw=LINI[b].vw;
          SINI[LINI[b].nn].nn=LINI[b].nn;
      }
   }

   DPrn(11,0,"10a\n");

   //------------------------------------------------------------------------
   // обработка строк клавиш
   for(b=0; b<HKBD; b++){ LKBD[b].run=LKBD[b].str[0]; LKBD[b].str[0]=32; }
   //------------------------------------------------------------------------
   // Запрещение определений
   if (Debug(70)>0){ EnbA=1; }
   if (EnbA<1){
      if (Debug(71)>0){ Dism=1; }
      else            { Dism=0; }
      if (Debug(72)>0){ DisC=1; }
      else            { DisC=0; }
      if (Debug(73)>0){ DisG=1; }
      else            { DisG=0; }
      if (Dism>0 || DisC>0 || DisG>0){ DisF=1; }
      else                           { DisF=0; }
   }

   DPrn(11,0,"11\n");

   //------------------------------------------------------------------------
   if (Debug(3)>0){
      fprintf(DebOut,"[ m ]\n");
      for(b=0;b<HMNU;b++){
         fprintf(DebOut,"#=%d vw=%d X=%d Y=%d W=%d H=%d c=%d k=%d [%s]\n",
          b, LMNU[b].vw,  LMNU[b].X,   LMNU[b].Y,   LMNU[b].W,
             LMNU[b].H,   LMNU[b].c,   LMNU[b].k,   LMNU[b].str );
      }

      fprintf(DebOut,"[ K ]\n");
      for(b=0;b<HKNP;b++){
         fprintf(DebOut,"#=%d vw=%d W=%d H=%d [%s]\n",
             b, LKNP[b].vw, LKNP[b].W, LKNP[b].H, LKNP[b].fb );
      }

      fprintf(DebOut,"[ O ]\n");
      for(b=0;b<HSEL;b++){
         fprintf(DebOut,"#=%d X=%d Y=%d W=%d H=%d k=%d sp=%d sx=%d sy=%d cs=%d str=[%s]\n",
          b, LSEL[b].X,   LSEL[b].Y,   LSEL[b].W,  LSEL[b].H,
             LSEL[b].k,   LSEL[b].sp,  LSEL[b].sx, LSEL[b].sy, LSEL[b].cs, LSEL[b].str );
      }


      fprintf(DebOut,"[ Q ]\n");
      for(b=0;b<HSLM;b++){
         fprintf(DebOut,"#=%d na=%d nv=%d X=%d Y=%d W=%d H=%d np=%d md=%d vl=%.2f vh=%.2f cl=%d cn=%d ch=%d\n",
          b, LSLM[b].na,LSLM[b].nv,LSLM[b].X, LSLM[b].Y, LSLM[b].W, LSLM[b].H,
             LSLM[b].np,LSLM[b].md,LSLM[b].vl,LSLM[b].vh,LSLM[b].cl,LSLM[b].cn,LSLM[b].ch);
      }

      fprintf(DebOut,"[ o ]\n");
      for(b=0;b<HREP;b++){
         fprintf(DebOut,"#=%d na=%d nv=%d ff=[%s] fo=[%s]\n",
             b, LREP[b].na, LREP[b].nv, LREP[b].ff, LREP[b].fo );
      }

      fprintf(DebOut,"[ k ]\n");
      for(b=0;b<HKBD;b++){
         fprintf(DebOut,"#=%d cod=%d flg=%d run=%c [%s]\n",
          b, LKBD[b].cod, LKBD[b].flg, LKBD[b].run, LKBD[b].str );
      }

      fprintf(DebOut,"[ ? ]\n");
      for(b=0;b<MOPQ;b++){
         fprintf(DebOut,"#=%d na=%d nv=%d ka=%d k1=%d v1=%d k2=%d v2=%d k3=%d v3=%d g1=%d g2=%d g3=%d gr=%d\n",
          b, LOPQ[b].na, LOPQ[b].nv, LOPQ[b].ka, 
             LOPQ[b].k1, LOPQ[b].v1, LOPQ[b].k2, LOPQ[b].v2, 
             LOPQ[b].k3, LOPQ[b].v3, LOPQ[b].g1, LOPQ[b].g2, 
             LOPQ[b].g3, LOPQ[b].gr);
      }

      fprintf(DebOut,"[ s ]\n");
      for(b=0;b<HSTRC;b++){
         fprintf(DebOut,"#=%d sn=%d X=%d Y=%d cs=%d cg=%d vSTR=%d [%s]\n",
          b, LSTR[b].sn,  LSTR[b].X,  LSTR[b].Y,   LSTR[b].cs,
             LSTR[b].cg,  vSTR[b],    LSTR[b].str );
      }

      fprintf(DebOut,"[ v ]\n");
      for(b=0;b<HVAL;b++){
         fprintf(DebOut,"#=%d vw=%d na=%d nv=%d dc=%d zn=%d   X=%d Y=%d W=%d  cs=%d ce=%d cg=%d sn=%d  sf=[%s]\n",
             b,           LVAL[b].vw,  LVAL[b].na,  LVAL[b].nv, LVAL[b].dc, LVAL[b].zn,
             LVAL[b].X,   LVAL[b].Y,   LVAL[b].W,
             LVAL[b].cs,  LVAL[b].ce,  LVAL[b].cg, LVAL[b].sn, LVAL[b].sf);
      }

      fprintf(DebOut,"[ g ]\n");
      for(b=0;b<HGST;b++){
         fprintf(DebOut,"#=%d na=%d nv=%d  [min=%f max=%f]  [X=%d Y=%d W=%d H=%d]  n=%d cg=%d cs=%d b=%d cb=%d\n",
          b, LGST[b].na,
             LGST[b].nv, LGST[b].min, LGST[b].max, LGST[b].X,  LGST[b].Y,
             LGST[b].W,   LGST[b].H,   LGST[b].n,   LGST[b].cg, LGST[b].cs,
             LGST[b].b,   LGST[b].cb );
      }

      fprintf(DebOut,"[ t ]\n");
      for(b=0;b<HTRD;b++){
         fprintf(DebOut,"#=%d [min=%f max=%f]  [X=%.0f Y=%.0f W=%.0f H=%.0f]  [ft=%d fs=%d]\n",
          b, LTRD[b].min, LTRD[b].max,
             LTRD[b].X1,  LTRD[b].Y1,  LTRD[b].W2, LTRD[b].H2,
             LTRD[b].ft,  LTRD[b].fs );
         fprintf(DebOut,"[sec=%d stx=%.0f sty=%.0f hm=%d]  [cg=%d cs=%d  n0=%d c0=%d  n1=%d c1=%d  n2=%d c2=%d  n3=%d c3=%d]\n",
             LTRD[b].sec, LTRD[b].stx, LTRD[b].sty, LTRD[b].hm,
             LTRD[b].g1,  LTRD[b].cs,
             LTRD[b].n[0],LTRD[b].c[0], LTRD[b].n[1],LTRD[b].c[1],
             LTRD[b].n[2],LTRD[b].c[2], LTRD[b].n[3],LTRD[b].c[3]);
      }

      fprintf(DebOut,"[ C ]\n");
      for(b=0;b<HCST;b++){
         fprintf(DebOut,"#=%d [vw=%d na=%d nv=%d] [X=%d Y=%d W=%d] [sn=%d cs=%d cg=%d]\n",
             b,
             LCST[b].vw,  LCST[b].na,  LCST[b].nv,
             LCST[b].X,   LCST[b].Y,   LCST[b].W,
             LCST[b].sn,  LCST[b].cs,  LCST[b].cg );
         fprintf(DebOut,"C0=[%s] C1=[%s] C2=[%s] C3=[%s]\n",
             LCST[b].C[0], LCST[b].C[1], LCST[b].C[2], LCST[b].C[3] );
      }

      fprintf(DebOut,"[ F ]\n");
      fprintf(DebOut,"na=%d nv=%d [X=%d Y=%d W=%d] [sn=%d cs=%d cg=%d] [fn=%s]\n",
             LFST.na,  LFST.nv, LFST.X,  LFST.Y,  LFST.W,
             LFST.sn,  LFST.cs, LFST.cg, LFST.fn  );

      fprintf(DebOut,"[ r ]\n");
      for(b=0;b<HRUN;b++){
         fprintf(DebOut,"#=%d vw=%d md=%d prg=[%s]\n",
          b, LRUN[b].vw, LRUN[b].md, LRUN[b].prg);
      }

      fprintf(DebOut,"[ i ]\n");
      for(b=0;b<HINI;b++){
         fprintf(DebOut,"#=%d [%s] vw=%d nn=%d \n",
          b, SINI[b].fn, SINI[b].vw, SINI[b].nn );
      }

      fprintf(DebOut,"[ w ]\n");
      for(b=0;b<HWIN;b++){
          fprintf(DebOut,"#=%d [X=%d Y=%d W=%d H=%d] vw=%d fn=[%s]\n",
          b, LWIN[b].X, LWIN[b].Y,
             LWIN[b].W, LWIN[b].H, LWIN[b].vw, LWIN[b].fn);
      }
      fprintf(DebOut,"[ P ]\n");
      for(b=0;b<HSPR;b++){
         fprintf(DebOut,"#=%d [X=%d Y=%d W=%d H=%d] vw=%d fn=[%s]\n",
          b, LSPR[b].X, LSPR[b].Y,
             LSPR[b].W, LSPR[b].H, LSPR[b].vw, LSPR[b].fn);
      }
      fprintf(DebOut,"[ a ]\n");
      for(b=0;b<HANI;b++){
         fprintf(DebOut,"#=%d vw=%d [na=%d nv=%d nx=%d ny=%d nf=%d] [X=%d Y=%d XM=%d YM=%d W=%d H=%d] hp=%d fp=[%s]\n",
          b, LANI[b].vw,
             LANI[b].na, LANI[b].nv, LANI[b].nx, LANI[b].ny, LANI[b].nf,
             LANI[b].X , LANI[b].Y,  LANI[b].XM, LANI[b].YM, LANI[b].W,  LANI[b].H,
             LANI[b].hp, LANI[b].fp);
      }
      fprintf(DebOut,"[ p ]\n");
      for(b=0;b<HPLZ;b++){
         fprintf(DebOut,"#=%d [na=%d nv=%d] [min=%.2f max=%.2f X=%.0f Y=%.0f L=%.0f W=%d H=%d G=%d] cb=%d tb=%d fb=[%s]\n",
          b, LPLZ[b].na, LPLZ[b].nv,
             LPLZ[b].min,LPLZ[b].max,
             LPLZ[b].X,  LPLZ[b].Y, LPLZ[b].L, LPLZ[b].W, LPLZ[b].H, LPLZ[b].G,
             LPLZ[b].cb, LPLZ[b].tb,LPLZ[b].fb);
      }

      fprintf(DebOut,"[ G ]\n");
      for(b=0;b<HGET;b++){
         fprintf(DebOut,"#=%d vw=%d na=%d nv=%d  dc=%d hc=%d  X=%.0f Y=%.0f W=%.0f  cs=%d cr=%d cg=%d sn=%d [%s]\n",
          b, LGET[b].vw, LGET[b].na, LGET[b].nv, LGET[b].dc, LGET[b].hc,
             LGET[b].X , LGET[b].Y,  LGET[b].W,  LGET[b].cs, LGET[b].cr,
             LGET[b].cg, LGET[b].sn, LGET[b].sf );
      }

      fprintf(DebOut,"[ W ]\n");
      for(b=0;b<HSTT;b++){
         fprintf(DebOut,"#=%d X=%d Y=%d W=%d sn=%d cs=%d cg=%d cc=%d kb=%d\n",
          b, LSTT[b].X, LSTT[b].Y, LSTT[b].W,
             LSTT[b].sn,LSTT[b].cs,LSTT[b].cg,LSTT[b].cc,LSTT[b].kb);
      }

      fprintf(DebOut,"[ c ]\n");
      for(b=0;b<HXCV;b++){
         fprintf(DebOut,"#=%d na=%d nv=%d zn=%d vc=%f\n",
          b, LXCV[b].na, LXCV[b].nv, LXCV[b].zn, LXCV[b].vc);
      }

      fprintf(DebOut,"[ z ]\n");
      for(b=0;b<1;b++){
         fprintf(DebOut,"X=%d Y=%d W=%d H=%d cs=%d\n",
             LZRO.X, LZRO.Y, LZRO.W, LZRO.H, LZRO.cs);
      }

      fprintf(DebOut,"[ f ]\n");
      for(b=0;b<HFVW;b++){
         fprintf(DebOut,"#=%d vw=%d X=%.0f Y=%.0f W=%.0f H=%.0f yt=%.0f ps=%d fd=%d ws=%d spp=%d sn=%d cs=%d cg=%d [%s]\n",
          b, LFVW[b].vw, LFVW[b].X,  LFVW[b].Y,  LFVW[b].W,  LFVW[b].H , LFVW[b].yt,
             LFVW[b].ps, LFVW[b].fd, LFVW[b].ws, LFVW[b].spp,LFVW[b].sn, LFVW[b].cs, LFVW[b].cg, LFVW[b].fn );
      }

      fprintf(DebOut,"[ b ]\n");
      for(b=0;b<HBOX;b++){
         fprintf(DebOut,"#=%d vw=%d X=%d Y=%d W=%d H=%d f=%d cs=%d n=%d\n",
          b, LBOX[b].vw, LBOX[b].X , LBOX[b].Y , LBOX[b].W , LBOX[b].H ,
             LBOX[b].f , LBOX[b].cs, LBOX[b].n );
      }

      fprintf(DebOut,"[ h ]\n");
      for(b=0;b<HSHK;b++){
         fprintf(DebOut,"#=%d X=%.0f Y=%.0f W=%.0f H=%.0f mn=%.0f mx=%.0f h=%d fl=%d dc=%d\n",
          b, LSHK[b].X,  LSHK[b].Y,  LSHK[b].W,  LSHK[b].H,  LSHK[b].mn,
             LSHK[b].mx, LSHK[b].h,  LSHK[b].fl, LSHK[b].dc);
      }

      fprintf(DebOut,"[ S ]\n");
      fprintf(DebOut,"sn=%d  X=%d Y=%d  cs=%d cg=%d lcs=%d\n",
             STAT[CFirst].sn, STAT[CFirst].X,  STAT[CFirst].Y,
             STAT[CFirst].cs, STAT[CFirst].cg, STAT[CFirst].lcs );

      fprintf(DebOut,"[ M ]\n");
      fprintf(DebOut,"sn=%d  X=%d Y=%d  cs=%d cg=%d lcs=%d\n",
             MESS[CFirst].sn, MESS[CFirst].X,  MESS[CFirst].Y,
             MESS[CFirst].cs, MESS[CFirst].cg, MESS[CFirst].lcs );

      fprintf(DebOut,"[ d ]\n");
      for(b=0;b<MDB;b++){ fprintf(DebOut,"%d ",Deb[b]); }
      fprintf(DebOut,"\n");

      fprintf(DebOut,"[ n ]\n");
      fprintf(DebOut,"%d\n",LDRV);
   }
   //------------------------------------------------------------------------

   //------------------------------------------------------------------------
   if (CFirst>0){
      //---------------------------------------------------------------------
      // загрузка PCX файлов (спрайтов)
      sprintf(SMSG[MMSG],SMSG[49]);
      DPrn(11,0,"%s\n",SMSG[MMSG]);
      for(b=0;b<HSPR;b++){ MLoad_Spr(b,0,0,0); }
      DPrn(11,0,"%s\n",SMSG[0]);
      //---------------------------------------------------------------------
      // рисование графических примитивов
      sprintf(SMSG[MMSG],SMSG[50]);
      DPrn(11,0,"%s\n",SMSG[MMSG]);
      for(b=0; b<HBOX; b++){
          MDraw_Box(b,0,LBOX[b].n,LBOX[b].X,LBOX[b].Y,LBOX[b].W,LBOX[b].H,LBOX[b].cs);
      }
      DPrn(11,0,"%s\n",SMSG[0]);
      //---------------------------------------------------------------------
      // Вывод гистограмм
      GisScr();
      // Определение и вывод строк выбора из файла
      FChoice(0);
      FChoice(1);
      //---------------------------------------------------------------------
      // рисование шкал
      sprintf(SMSG[MMSG],SMSG[51]);
      DPrn(11,0,"%s\n",SMSG[MMSG]);
      for(b=0; b<HSHK; b++){ MDraw_Shk(b); }
      DPrn(11,0,"%s\n",SMSG[0]);
      //---------------------------------------------------------------------
      // вывод сообщений на экран
      sprintf(SMSG[MMSG],SMSG[52]);
      DPrn(11,0,"%s\n",SMSG[MMSG]);
      for(b=0; b<HSTRC; b++){
          if (vSTR[b] >0){ MESOut((MOUT*)&LSTR[b],3,b); }
          if (vSTR[b]==5){ fSTR=1; }
      }
      DPrn(11,0,"%s\n",SMSG[0]);
      //---------------------------------------------------------------------
      // вывод окон просмотра файлов
      sprintf(SMSG[MMSG],SMSG[53]);
      DPrn(11,0,"%s\n",SMSG[MMSG]);
      for(b=0; b<HFVW; b++){
         if(LFVW[b].fd>0){
            strcpy(LFVW[b].fn,FNRep); a=strlen(FNRep);
            LFVW[b].dt[0]=LFVW[b].fn[a-6]; LFVW[b].dt[1]=LFVW[b].fn[a-5];
            LFVW[b].dt[2]='.';
            LFVW[b].dt[3]=LFVW[b].fn[a-8]; LFVW[b].dt[4]=LFVW[b].fn[a-7];
            LFVW[b].dt[5]='.';
            LFVW[b].dt[6]=LFVW[b].fn[a-12]; LFVW[b].dt[7]=LFVW[b].fn[a-11];
            LFVW[b].dt[8]=LFVW[b].fn[a-10]; LFVW[b].dt[9]=LFVW[b].fn[a-9 ];
         }
         if(LFVW[b].vw>0){ FVCode=1; ViewFile(b,0); }
      }; FV_Set(1);
      DPrn(11,0,"%s\n",SMSG[0]);
      //---------------------------------------------------------------------
      // выполнение программ
      for(b=0; b<HRUN; b++){ if (LRUN[b].vw>0) RunPRG(b);; }
      //---------------------------------------------------------------------
      // заполнение общих массивов (дата,время,сообщения,статус)
      if (HDAT==0){ memcpy(&VKDATE[1],&VKDATE[0],sizeof(VKDATE[0])); }
      if (HTIM==0){ memcpy(&VKTIME[1],&VKTIME[0],sizeof(VKTIME[0])); }
      if (HSTA==0){ memcpy(&STAT[1],&STAT[0],LEN__I*6); }
      if (HMSS==0){ memcpy(&MESS[1],&MESS[0],LEN__I*6); }
      //---------------------------------------------------------------------
      // для всех выводимых значений
      sprintf(SMSG[MMSG],SMSG[54]);
      DPrn(11,0,"%s\n",SMSG[MMSG]);
      
      HSER=ESER;
      HNBW=ENBW;
      HTCP=ETCP; for(b=0;b<HSRV;b++) HOWC[b]=HOWE[b];      
      
      for(b=0; b<HVAL; b++){
          // если формат вывода значений в последнем поле явно НЕ определен
          if (LVAL[b].sf[0]<31){
              sprintf(c,"%d",LVAL[b].dc); SFRM_V[4]=c[0];
              strcpy(LVAL[b].sf,SFRM_V);
          }
          // если это канал из общей базы... 
          // определение каналов обмена для каждого экрана
          if (LVAL[b].na==0){ CHSET(LVAL[b].nv); }
      }
      DPrn(11,0,"%s\n",SMSG[0]);
      //---------------------------------------------------------------------
      // выбор каналов для обмена по COM порту из переменных гистограмм
      sprintf(SMSG[MMSG],SMSG[55]);
      DPrn(11,0,"%s\n",SMSG[MMSG]);
      for(b=0; b<HGST; b++){
          // если это канал из общей базы... 
          // определение каналов обмена для каждого экрана
          if (LGST[b].na==0){ CHSET(LGST[b].nv); }
      }
      DPrn(11,0,"%s\n",SMSG[0]);
      //---------------------------------------------------------------------
      // выбор каналов для обмена по COM порту из переменных кнопок
      for(b=0; b<HXCV; b++){
          // если это канал из общей базы... 
          // определение каналов обмена для каждого экрана
          if (LXCV[b].na==0){ CHSET(LXCV[b].nv); }
      }
      //---------------------------------------------------------------------
      // выбор каналов для обмена по COM порту из переменных областей выделений
      for(b=0; b<HSLM; b++){
          // если это канал из общей базы... 
          // определение каналов обмена для каждого экрана
          if (LSLM[b].na==0){ CHSET(LSLM[b].nv); }
      }
      //---------------------------------------------------------------------
      // Загрузка файлов кнопок
      sprintf(SMSG[MMSG],SMSG[56]);
      DPrn(11,0,"%s\n",SMSG[MMSG]);
      if (HKNP>0){
         for(b=0;b<=HKNP;b++){
            if (LKNP[b].vw>0){
               pcxbut = load_bitmap(LKNP[b].fb,pal);
               if (pcxbut){
                   if (LKNP[b].W>WKNP){ LKNP[b].W=WKNP; }
                   if (LKNP[b].H>WKNP){ LKNP[b].H=WKNP; }
                   for(j=0;j<3;j++){
                      clear_bitmap(LKNP[b].sp[j]);
                      blit( pcxbut,LKNP[b].sp[j],LKNP[b].W*j+j,0,0,0,LKNP[b].W,LKNP[b].H);
                   }
               }   else   {
                   sprintf(SMSG[MMSG],SMSG[57]);
                   strcpy(STAT[1].str,SMSG[MMSG]); STAT[1].cnt=10;
               }
               destroy_bitmap(pcxbut);
            }
         }; LKNP[0].vw=0;
      }
      DPrn(11,0,"%s\n",SMSG[0]);
      //---------------------------------------------------------------------
      // отображение первого меню
      sprintf(SMSG[MMSG],SMSG[58]);
      DPrn(11,0,"%s\n",SMSG[MMSG]);
      if (HMNU>0){
         for(b=0,d=0;b<HMNU;b++){
            if (LMNU[b].k ==0){ continue; }
            if (LMNU[b].bn>MKNP-1){ LMNU[b].bn=0; }
            if (LMNU[b].bn>0&&LKNP[ LMNU[b].bn ].vw>0){
                LMNU[b].sf=1; LMNU[b].bf=1;
                draw_sprite(scrsav,LKNP[ LMNU[b].bn ].sp[0],
                                         LMNU[b].X, LMNU[b].Y);
            }
            if (LMNU[b].vw>0&&d==0){ vmnu=b; LMNU[b].bf=2; d=1; }
         }
         // сохранить первый экран под меню
         SRMenu(0,vmnu);
         sprintf(STAT[1].str,"%s",LMNU[vmnu].str);
      }
      // установить изображение курсора и фокус(центр) для него
      curp=0; MCurPL=0;
      DPrn(11,0,"%s\n",SMSG[0]);
      //---------------------------------------------------------------------
      // нарисовать картинки областей
      if (HSEL>0){
          for(a=0;a<HSEL;a++){ DrawSPRKNP(a); }; SRSelect(0,0);
      }
      //---------------------------------------------------------------------
      // информационная строка
      if(ErrCNT==0){
         sprintf(STAT[1].str,MDEC[1],codus_version,OSVER);
      }; STAT[1].cnt=10;
      //---------------------------------------------------------------------
      // обработка переменных трендов
      sprintf(SMSG[MMSG],SMSG[59]);
      DPrn(11,0,"%s\n",SMSG[MMSG]);
      for(a=0;a<HTRD;a++){
         TF[CTC].fs[a]=0; TF[CTC].lm[a]=0;
         if(LTRD[a].sec<_TRS){ LTRD[a].sec=_TRS; }
         if(LTRD[a].sec>MTRS){ LTRD[a].sec=MTRS; }
         // нулевой тренд определяем, как активный
         TF[CTC].an[a]=(a==0)?1:0;
         TF[CTC].ta=0;
         // установить тип графика для отображения архива
         TF[CTC].fh[a]=ARCOut; TF[CTC].ff[a]=2;
         // если тип графика больше 9, установить флаг типа тренда = 3
         if(LTRD[a].th>=10){
            TF[CTC].fh[a]=UPDOut; TF[CTC].ff[a]=3; 
         }
         // определяем каналы в массивах отображения трендов
         for(b=0,TF[CTC].gh[a]=0;b<=MTRG;b++){
            if (LTRD[a].n[b]==0) break;;
            // для постоянных графиков
            for(d=0;d<arc_max;d++){
//!!!          // канал N0 не может быть архивным и не выводится на трендах    
               if(arc_ch[d]==0){ break; }
               // если номер отображаемой переменной, определенной в комманде 
               // вывода тренда совпал с номером, записанным в массив архива
               // или в массив изменений - записать этот номер в массив
               // номеров переменных тренда
               if(LTRD[a].n[b]==arc_ch[d]||LTRD[a].n[b]==upd_ch[d]){
                  trv_cc[a][TF[CTC].gh[a]]=d; TF[CTC].gh[a]++; break;
         }  }  }
         // флаг вывода значений временного среза
         if (LTRD[a].hm-10>0){
             TF[CTC].hm[a]=1; LTRD[a].hm=LTRD[a].hm-10;
         }  else  {
             TF[CTC].hm[a]=0;
         }
         LTRD[a].rst=0;
      }
      DPrn(11,0,"%s\n",SMSG[0]);
      //---------------------------------------------------------------------
      // чтение файла архива
      sprintf(SMSG[MMSG],SMSG[60]);
      DPrn(11,0,"%s\n",SMSG[MMSG]);
      for(a=0;a<HTRD;a++){
          // заполнить массив архива из считанного буфера
          DPrn(DEBLDD,0,"RARead ");   RARead(a,0);
          // передача значений из массива архива в массив тренда
          DPrn(DEBLDD,0,"VA_to_VT "); VA_to_VT(a);
          // проверка шага линии временного среза
          if (TF[CTC].ls[a]<=0){ TF[CTC].ls[a]=1; }
          if (TF[CTC].ls[a]>LTRD[a].sec){ TF[CTC].ls[a]=LTRD[a].sec-10; }
      }
      DPrn(DEBLDD,0,"FARNRd..."); FARNRd();
      DPrn(11,0,"%s\n",SMSG[0]);
      //---------------------------------------------------------------------
      // загрузка ползунков
      sprintf(SMSG[MMSG],SMSG[61]);
      DPrn(11,0,"%s\n",SMSG[MMSG]);
      Polzun(0,0);
      // выбор каналов для обмена по COM порту из переменных ползунков
      for(b=0;b<HPLZ;b++){
          // если это канал из общей базы... 
          // определение каналов обмена для каждого экрана
          if (LPLZ[b].na==0){ CHSET(LPLZ[b].nv); }
      }
      DPrn(11,0,"%s\n",SMSG[0]);
      //---------------------------------------------------------------------
      // выбор каналов для обмена по COM порту из переменных ввода данных
      sprintf(SMSG[MMSG],SMSG[62]);
      DPrn(11,0,"%s\n",SMSG[MMSG]);
      for(b=0;b<HGET;b++){
          // если формат вывода значений в последнем поле явно НЕ определен
          if (LGET[b].sf[0]<31){
              sprintf(c,"%d",LGET[b].dc); SFRM_G[4]=c[0];
              strcpy(LGET[b].sf,SFRM_G);
          }
          // если это канал из общей базы... 
          // определение каналов обмена для каждого экрана
          if (LGET[b].na==0){ CHSET(LGET[b].nv); }
      }
      DPrn(11,0,"%s\n",SMSG[0]);
      //---------------------------------------------------------------------
      // загрузка строк выбора
      Choice(0);
      // выбор каналов для обмена по COM порту из переменных строк выбора
      for(b=0;b<HCST;b++){
          // если это канал из общей базы... 
          // определение каналов обмена для каждого экрана
          if (LCST[b].na==0){ CHSET(LCST[b].nv); }
      }
      //---------------------------------------------------------------------
      // загрузка файлов анимации
      sprintf(SMSG[MMSG],SMSG[63]);
      DPrn(11,0,"%s\n",SMSG[MMSG]);
      Animation(0);
      // выбор каналов для обмена по COM порту из переменных анимации
      for(b=0;b<HANI;b++){
          // если это канал из общей базы... 
          // определение каналов обмена для каждого экрана
          if (LANI[b].na==0){ CHSET(LANI[b].nv); }
      }
      // флаг перерисовки
      ReDraw=1; RKDraw=1;
      if(Debug(20)>0){ TRFL=1; }
      else           { TRFL=0; }
      DPrn(11,0,"%s\n",SMSG[0]);
   //------------------------------------------------------------------------
   }  else  {
      // запись файла сообщений программы, если его нет
      if (access(FNMSG,4)!=0&&Debug(25)>0){
          HDMSG=open(FNMSG,O_BINARY|O_RDWR|O_CREAT,S_IREAD|S_IWRITE);
          sprintf(SMSG[MMSG],";-----------------------------------------------------------------------------\n");
          write(HDMSG,SMSG[MMSG],strlen(SMSG[MMSG]));
          sprintf(SMSG[MMSG],"; This is default messages file for programm CODUS\n");
          write(HDMSG,SMSG[MMSG],strlen(SMSG[MMSG]));
          for(a=0;a<MMSG;a++){
              if(strlen(SMSG[a])>0){
                 sprintf(SMSG[MMSG],"m %03d \"%s\"\n",a,SMSG[a]);
                 write(HDMSG,SMSG[MMSG],strlen(SMSG[MMSG]));
              }
          }
          close(HDMSG);
      }
      // определение видеорежима
      if (VMode==3){ XMAX= 320; YMAX=200; }
      if (VMode==6){ XMAX= 640; YMAX=480; }
      if (VMode==8){ XMAX= 800; YMAX=600; }
      if (VMode==1){ XMAX=1024; YMAX=768; }
      //---------------------------------------------------------------------
      // определение имени рабочей станции
      if (NBWS>99){ NBWS=99; }
      sprintf(tmpcc,"%d",NBWS);
      NMWS[6]=tmpcc[0]; NMWS[7]=tmpcc[1];
      COMWSNum[0]=tmpcc[0]; COMWSNum[1]=tmpcc[1];
      //---------------------------------------------------------------------
      // активизация драйверов
      FLSER=0; FLNBW=0; FLTCP=0;
      if ((_i)fmod(LDRV,10  )>   0){ FLSER=1; }
      if ((_i)fmod(LDRV,100 )>= 10){ FLNBW=1; }
      if ((_i)fmod(LDRV,1000)>=100){ FLTCP=1; }
      //------------------------------------------------------------------
      // активизация драйвера COM порта
      if (BDSER<=0||BDSER>=115200){ CBSER=115200; }
      if (BDSER<115200){ CBSER=57600; }
      if (BDSER< 57600){ CBSER=38400; }
      if (BDSER< 38400){ CBSER=19200; }
      if (BDSER< 19200){ CBSER= 9600; }
      if (BDSER<  9600){ CBSER= 4800; }
      if (BDSER<  4800){ CBSER= 2400; }
      if (BDSER<  2400){ CBSER= 1200; }
      BDSER=CBSER;
      if (FLSER>0){
          SSTAT=1;
          ACSER=(_i)fmod(LDRV,10);
          if(ACSER>0&&ACSER<3){
             CurSP=ACSER-1;
             if(VKSPInit((CurSP))){
                sprintf(SMSG[MMSG],SMSG[64],ACSER);
                DPrn(11,0,"%s\n",SMSG[MMSG]);
                SSTAT=0;
             } else {
                sprintf(SMSG[MMSG],SMSG[65],ACSER,BDSER);
                DPrn(11,0,"%s\n",SMSG[MMSG]);
             }
             VKSPFifoInit();
             if((b=VKSPSet(CurSP,BDSER,SP8N1))<0){
                sprintf(SMSG[MMSG],SMSG[66],ACSER,b);
                DPrn(11,0,"%s\n",SMSG[MMSG]);
                SSTAT=0;
             }
             VKSPHand(DTR|RTS);
          } else { ACSER=0; SSTAT=0; }
      } else {
          SSTAT=0;
      }
      if (SSTAT==0) DPrn(0,0,"COMStat: Disable.\n");
      else          DPrn(0,0,"COMStat: Enable. \n");
      //------------------------------------------------------------------
      // активизация драйвера NetBIOS
      if (FLNBW>0){
          DPrn(11,0,"Init NetBIOS.\n");
          if( WS_NB__Init(1)==0 ){
             NSTAT=0;
             sprintf(SMSG[MMSG],SMSG[67]);
             DPrn(11,0,"%s\n",SMSG[MMSG]);
          }  else  {
             NSTAT=1;
          }
      }
      if (NSTAT==0) DPrn(0,0,"NBWStat: Disable.\n");
      else          DPrn(0,0,"NBWStat: Enable. \n");
      //------------------------------------------------------------------
      // активизация драйвера TCP/IP
      if (FLTCP>0){
          if(DRV_TCP(0)==0) TSTAT=1;
          else              TSTAT=0;
      }
      if (TSTAT==0) DPrn(0,0,"TCPStat: Disable.\n");
      else          DPrn(0,0,"TCPStat: Enable. \n");
      //------------------------------------------------------------------
      // чтение файла комманд принтера
      if (access(SPRN.fn,6)==0){
          FHPRN=open(SPRN.fn,O_BINARY|O_RDONLY);
          if(FHPRN>0){ HBR=read(FHPRN,FBuf,SBuf);
             close(FHPRN);
             FillPRN(HBR);
          }
      }
      //------------------------------------------------------------------
      FLSER=0; FLNBW=0; FLTCP=0;
   }
   //------------------------------------------------------------------------

   //------------------------------------------------------------------------
   // начальные значения для строк ввода данных
   sprintf(SMSG[MMSG],SMSG[69]);
   DPrn(11,0,"%s\n",SMSG[MMSG]);
   for(a=0;a<HGET;a++){
       if (LGET[a].hc>GLEN){ LGET[a].hc=GLEN-1; }
       if (LGET[a].dc>5   ){ LGET[a].dc=5;      }
   }
   //------------------------------------------------------------------------

   sprintf(SMSG[MMSG],SMSG[70]);
   DPrn(11,0,"%s\n",SMSG[MMSG]);
   return(0);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//  определение каналов для обмена... (для каждого экрана)
vd  CHSET(_i nv){
    _i a=0,n=0;

    FLSER=0; FLNBW=0; FLTCP=0;
    if ((_i)fmod(CB[nv].dt,10  )>   0 && SSTAT){ FLSER=1; }
    if ((_i)fmod(CB[nv].dt,100 )>= 10 && NSTAT){ FLNBW=1; }
    if ((_i)fmod(CB[nv].dt,1000)>=100 && TSTAT){ FLTCP=1; }
          
    //-----------------------------------------------------------------------
    // если канал определен для обмена по COM порту
    if (FLSER){
       if (HSER<MSER){
           // проверка на присутствие в массиве канала с таким же номером
           for(a=0;a<HSER;a++){
               if(DSER[a]==nv){ break; }
           }
           // запись канала в массивы обмена по COM порту
           if(a==HSER){
              DPrn(31,0,"DSER[%d]=%d\n",HSER,nv);
              DSER[HSER]=nv;
              NSER[HSER]=CB[nv].dn;
              SSER[HSER]=CB[nv].dd;
              CBSR[nv]=HSER;
              HSER++;
           }
       }
    }
    //-----------------------------------------------------------------------
    // Если канал определен для обмена по NetBIOS
    if (FLNBW){
       if (HNBW<MNBW){
           for(a=0;a<HNBW;a++){
               if(DNBW[a]==nv){ break; }
           }
           if(a==HNBW){
              DPrn(31,0,"E:DNBW[%d]=%d\n",HNBW,nv);
              DNBW[HNBW]=nv;
              NNBW[HNBW]=CB[nv].dn;
              SNBW[HNBW]=CB[nv].dd;
              CBDN[nv]=HNBW;
              HNBW++;
           }
       }
    }
    //-----------------------------------------------------------------------
    if (FLTCP){
       if (HTCP<MTCP){
           for(a=0;a<HTCP;a++){
               if(DTCP[a]==nv){ break; }
           }
           if(a==HTCP){
              DPrn(31,0,"E:DTCP[%d]=%d\n",HTCP,nv);
              n=CB[nv].dn;                  // номер удаленной станции на текущем канале
              if(n>0&&n<MWS){
              SCHN[n][HOWC[n]]=nv;          // номер канала на локальном компьютере CB[]
              RCHN[n][HOWC[n]]=CB[nv].dd;   // номер канала на контроллере
              HOWC[n]++;                    // кол-во каналов для каждой станции
              RSRV[HTCP]=n;                 // номер удаленной станции для каждого канала CB[]
              DTCP[HTCP]=nv;                // номера каналов TCP
              HTCP++;                       // кол-во обработанных переменных TCP
              }
           }
       }
    }
    //-----------------------------------------------------------------------
    return;
}
//---------------------------------------------------------------------------
