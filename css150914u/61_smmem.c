// coding: cp866
//---------------------------------------------------------------------------
//  определить свободную память
_i  TestMem(size_t AMEM){
    if ((VMEM=malloc(AMEM))==NULL){ return(0); }
    free(VMEM); return(1);
}
//---------------------------------------------------------------------------
//  подсчет требуемой для загрузки программы памяти
_i SumMem(vd){
   _i mVAL=0, mGRA=0; 
   VVKT_I=sizeof(XMAX);
   mVAL=mVAL+sizeof(GRMode); 
   mVAL=mVAL+sizeof(ARCapath);
   mVAL=mVAL+sizeof(ARCfname);
   mVAL=mVAL+sizeof(FDir);
   mVAL=mVAL+sizeof(FNDts);
   mVAL=mVAL+sizeof(FNDtf);
   mVAL=mVAL+sizeof(FName);
   mVAL=mVAL+sizeof(FNMSG);
   mVAL=mVAL+sizeof(SMSG);
   mVAL=mVAL+sizeof(CRCod);
   mVAL=mVAL+sizeof(FNDeb);
   mVAL=mVAL+sizeof(Deb);
   mVAL=mVAL+sizeof(FNAlmR);
   mVAL=mVAL+sizeof(FNCfg);
   mVAL=mVAL+sizeof(FNCHB);
   mVAL=mVAL+sizeof(FNini);
   mVAL=mVAL+sizeof(FNSvL);
   mVAL=mVAL+sizeof(FNSvP);
   mVAL=mVAL+sizeof(FBuf);
   mVAL=mVAL+sizeof(FNLgt);
   mVAL=mVAL+sizeof(FNArc);
   mVAL=mVAL+sizeof(FNRep);
   mVAL=mVAL+sizeof(FNFVW);
   mVAL=mVAL+sizeof(FNTmp);
   mVAL=mVAL+sizeof(FNIdx);
   mVAL=mVAL+sizeof(LALR);
   mVAL=mVAL+sizeof(STAT);
   mVAL=mVAL+sizeof(STAT_str);
   mVAL=mVAL+sizeof(MESS);
   mVAL=mVAL+sizeof(MOUT);
   mVAL=mVAL+sizeof(MESSWR);
   mVAL=mVAL+sizeof(MS__sto);
   mVAL=mVAL+sizeof(SFRM_G);
   mVAL=mVAL+sizeof(SFRM_M);
   mVAL=mVAL+sizeof(SFRM_V);
   mVAL=mVAL+sizeof(SFRM_T);
   mVAL=mVAL+sizeof(SFRM_A);
   mVAL=mVAL+sizeof(VKDATE);
   mVAL=mVAL+sizeof(VKTIME);
   mVAL=mVAL+sizeof(MDate);
   mVAL=mVAL+sizeof(MTime);
   mVAL=mVAL+sizeof(LPCX);
   mVAL=mVAL+sizeof(LWIN);
   mVAL=mVAL+sizeof(LSPR);
   mVAL=mVAL+sizeof(LSTR);
   mVAL=mVAL+sizeof(SSTR);
   mVAL=mVAL+sizeof(vSTR);
   mVAL=mVAL+sizeof(LSTT);
   mVAL=mVAL+sizeof(LKBD);
   mVAL=mVAL+sizeof(LINI);
   mVAL=mVAL+sizeof(SINI);
   mVAL=mVAL+sizeof(LRUN);
   mVAL=mVAL+sizeof(PN_fn);
   mVAL=mVAL+sizeof(LKNP);
   mVAL=mVAL+sizeof(LMNU);
   mVAL=mVAL+sizeof(NMWS);
   mVAL=mVAL+sizeof(CBL);
   mVAL=mVAL+sizeof(DLOC);
   mVAL=mVAL+sizeof(CBS);
   mVAL=mVAL+sizeof(FSRS);
   mVAL=mVAL+sizeof(DSER);
   mVAL=mVAL+sizeof(NSER);
   mVAL=mVAL+sizeof(SSER);
   mVAL=mVAL+sizeof(CBN);
   mVAL=mVAL+sizeof(FNBS);
   mVAL=mVAL+sizeof(DNBW);
   mVAL=mVAL+sizeof(NNBW);
   mVAL=mVAL+sizeof(SNBW);
   mVAL=mVAL+sizeof(CBT);
   mVAL=mVAL+sizeof(DTCP);
   mVAL=mVAL+sizeof(CB);
   mVAL=mVAL+sizeof(CBDN);
   mVAL=mVAL+sizeof(CBSR);
   mVAL=mVAL+sizeof(IC);
   mVAL=mVAL+sizeof(CVT);
   mVAL=mVAL+sizeof(lcc);
   mVAL=mVAL+sizeof(lcsok);
   mVAL=mVAL+sizeof(LVAL);
   mVAL=mVAL+sizeof(SVAL);
   mVAL=mVAL+sizeof(LGST);
   mVAL=mVAL+sizeof(LTRD);
   mVAL=mVAL+sizeof(TRFF);
   mVAL=mVAL+sizeof(arc_ch);
   mVAL=mVAL+sizeof(AW_C);
   mVAL=mVAL+sizeof(AW_V);
   mVAL=mVAL+sizeof(RABuf);
   mVAL=mVAL+sizeof(VTRD);
   mVAL=mVAL+sizeof(VARC);
   mVAL=mVAL+sizeof(TF);
   mVAL=mVAL+sizeof(VTRT);
   mVAL=mVAL+sizeof(trv_cc);
   mVAL=mVAL+sizeof(tras);
   mVAL=mVAL+sizeof(try_max);
   mVAL=mVAL+sizeof(mmss);
   mVAL=mVAL+sizeof(LGET);
   mVAL=mVAL+sizeof(VGET);
   mVAL=mVAL+sizeof(LGS);
   mVAL=mVAL+sizeof(LGC);
   mVAL=mVAL+sizeof(RUC);
   mVAL=mVAL+sizeof(RUN);
   mVAL=mVAL+sizeof(GS);
   mVAL=mVAL+sizeof(LXCV);
   mVAL=mVAL+sizeof(LFVW);
   mVAL=mVAL+sizeof(VW);
   mVAL=mVAL+sizeof(LBOX);
   mVAL=mVAL+sizeof(LZRO);
   mVAL=mVAL+sizeof(ECBuf);
   mVAL=mVAL+sizeof(DCBuf);
   mVAL=mVAL+sizeof(MDEC);
   mVAL=mVAL+sizeof(LSHK);
   mVAL=mVAL+sizeof(LSEL);
   mVAL=mVAL+sizeof(LSLM);
   mVAL=mVAL+sizeof(LREP);
   mVAL=mVAL+sizeof(BRep);
   mVAL=mVAL+sizeof(IDXTIM);
   mVAL=mVAL+sizeof(IDXSER);
   mVAL=mVAL+VVKT_I*9*MPSC+SLEN;
   mVAL=mVAL+sizeof(FPRN);
   mVAL=mVAL+sizeof(LDIR);
   mVAL=mVAL+sizeof(LMVL);
   mVAL=mVAL+sizeof(LCMD);
   mVAL=mVAL+sizeof(YCM );
   mVAL=mVAL+sizeof(RTMP);
   mVAL=mVAL+sizeof(WTIF);
   mVAL=mVAL+sizeof(stmp);
   mVAL=mVAL+dbGetMemLib();
   mVAL=mVAL+VVKT_I*250;
   sprintf(SMSG[MMSG],SMSG[166],mVAL);
   DPrn(0,0,"%s\n",SMSG[MMSG]);
   mGRA=mGRA+(XMAX*YMAX*8);                   // отдельные окна
   mGRA=mGRA+( 20 * 32 *3);                   // курсоры
   mGRA=mGRA+(WSPR*WSPR*1*(MSPR+1));          // спрайты
   mGRA=mGRA+(WKNP*WKNP*3*(MKNP+1));          // кнопоки
   mGRA=mGRA+(WANI*WANI*MASP*(MANI+1));       // анимация 
   mGRA=mGRA+(WPLZ*WPLZ*3*(MPLZ+1));          // ползунки
   sprintf(SMSG[MMSG],SMSG[167],mGRA);
   DPrn(0,0,"%s\n",SMSG[MMSG]);
   if(TestMem((mVAL+mGRA))<1){
      sprintf(SMSG[MMSG],SMSG[168]);
      DPrn(0,0,"%s\n",SMSG[MMSG]);
      Mexit(0,0); 
   }
   return((mVAL+mGRA));
}
//---------------------------------------------------------------------------
