// coding: cp866
//---------------------------------------------------------------------------
// определение имени начального файла конфигурации
vd SetFNCfg(vd){
   _i fd=0, a=0;
   _i hb=0;
   CDrive=_al_getdrive(); 
   _al_getdcwd(CDrive,CDir,SLEN); strcat(CDir,"/");
   // прочитать из файла рабочий каталог
   if ((fd=open("codus.ini",O_BINARY|O_RDONLY))<1){ Mexit(1,1); }
   hb=read(fd,&FDir,SLEN);
   close(fd);
   for(a=0;a<hb;a++){ if(FDir[a]<33) FDir[a]=0;; }
   // определить имена стартовых файлов конфигурации
   strcpy(FNCfg, FDir); strcat(FNCfg, "/codus.ini");
   strcpy(FNini, FDir); strcat(FNini, "/codus.ini");
   strcpy(FNSvL, FDir); strcat(FNSvL, "/codus.dtl");
   strcpy(FNSvP, FDir); strcat(FNSvP, "/codus.dtp");
   strcpy(FNDLS, FDir); strcat(FNDLS, "/codus.dls");
   strcpy(FNCMD, FDir); strcat(FNCMD, "/codus.cmd");
   sprintf(SMSG[MMSG],SMSG[30],FNCMD);
   DPrn(0,1,"%s\n",SMSG[MMSG]);
   strcpy(FNAlmR,FDir); strcat(FNAlmR,"/codus_al.ini");
   strcpy(FNCHB, FDir); strcat(FNCHB, "/codus_ch.ini");
   strcpy(FName[2], FDir); strcat(FName[2], "/codus.dae");
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// кодирование/декодирование строк
// md='e':кодирование, md='d':декодирование. alg=1:старый алгоритм.alg=2:новый
// len- длинна передаваемой строкии
vd EDCode(uc md, _i alg, _i len){
   _i  a=0, b=0, c=0, e=0;
   //  таблица символов
   //              0   1   2   3   4   5   6   7   8   9
   uc  tbl [33]={ 'q','w','e','r','t','y','u','i','o','p',
                  'a','s','d','f','g','h','j','k','l','.',
                  'z','x','c','v','b','n','m',' ','_','@', 0, 10, 13 };
   //  строка кода (по таблице)
   //             v  v  k  s  c  a  d  a
   _i  scd[ECCS]={23,23,17,11,22,10,12,10};
   //  дополнительная строка кодирования
   _i  ecc[ECCM]={2,201,30,67,145,1,84,215,111,7,74,12,240,183,60,211,55};
   _i  ecd=0, ecb=0, eca=7;
   for(a=0; a<len/2; a++, e++){
       c=(_i)(tbl[scd[ecb]]) & 0xff;
       // кодировка строк
       if(md=='e'||md=='E'){
          b=DCBuf[a];
          ECBuf[a*2]= b + c * ecc[ecd] + ecc[eca];
       } else {
       // декодировка строк
          if(alg==1){ b=ECBuf[a*1]; }
          if(alg==2){ b=ECBuf[a*2]; }
          DCBuf[a]  = b - c * ecc[ecd] - ecc[eca];
       }
       eca  =  eca + 1;
       ecb  =  ecb + 1;
       ecd  =  ecd + 1;
       if(ecb>=ECCS){ ecb=0; }
       if(eca>=ECCM){ eca=0; }
       if(ecd>=ECCM){ ecd=0; }
  }
  return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// чтение файла кодированных строк
vd Read_ESt(vd){
   _i fn=0, a=0, b=0, d=0, f=0, ECHB=0;
   if ((fn=open(FName[2],O_BINARY|O_RDONLY))<1){ return; }
   ECHB=read(fn,ECBuf,PBuf); close(fn); if(ECHB<100){ return; }
   EDCode('d',1,ECHB);
   for(a=0; a<MDST; a++){
       for(b=0; b<SLEN; b++){ MDEC[a][b]=0; }
   }
   for(a=0; a<ECHB; a++){
       if( DCBuf[a]==10 ){
           MDEC[b][d]=0; d=0; b=0; f=0;
           if (b>MDST-2){ break;    }
           else         { continue; }
       }
       if (f==3){ continue; }
       if (DCBuf[a]< 32) { d= 0; b=0; continue; }
       if (DCBuf[a]==32 && f==0){
           MDEC[0][d]=0; b=atoi(MDEC[0]); f=1; d=0; continue;
       }
       if (f<2 && DCBuf[a]==59){ f=3; continue; }
       MDEC[b][d]=DCBuf[a]; if(d<SLEN-1){ d++; }
   }
   return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// проверка контрольных сумм системных файлов
_i CRCheck(vd){
   _i  a=0, fh=0, r=0, z=0, hc=0;
   ul  crm=0;
   for(z=0; z<3; z++){
       if ((fh=open(FName[z],vktRDO))<1){
          sprintf(SMSG[MMSG],SMSG[31],FName[z]);
          DPrn(0,1,"%s\n",SMSG[MMSG]);
          return(1);
       }
       for(crm=0;;){
          for(a=0;a<PBuf;a++){ DCBuf[a]=0; }
          hc=read(fh,DCBuf,PBuf);
          for(a=0;a<hc;a++){ crm=crm+((ul)DCBuf[a]&0xff); }
          if (hc!=PBuf){ break; }
       }
       close(fh);
       if (CRCod[z]!=crm){ r++; }
       if((CRCod[z]<1000)||(crm<1000)){ r++; }
       DPrn(51,1,"FName=[%s] CRCod=[%ld] crm=[%ld]\n",FName[z],CRCod[z],crm);
   }
   return(r);
}
//---------------------------------------------------------------------------
