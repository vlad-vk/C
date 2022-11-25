// coding: cp866
//---------------------------------------------------------------------------
// Установка сохраненных значений
vd ReadSave(_i md){
   _i fd=0;
   // чтение установок при запуске программы
   if (md==0){
      if((fd=open(FNSvL,vktRDO))>0){ read(fd,&CBL,sizeof(CBL)); close(fd); }
      if((fd=open(FNSvP,vktRDO))>0){ read(fd,&CB ,sizeof(CB )); close(fd); }
   }
   // сохранение установок при выходе из программы
   if (md==1){
      if((fd=open(FNSvL,vktWRT,S_IREAD|S_IWRITE))>0){
          write(fd,&CBL,sizeof(CBL)); close(fd);
      }
      if((fd=open(FNSvP,vktWRT,S_IREAD|S_IWRITE))>0){
          write(fd,&CB ,sizeof(CB )); close(fd);
      }
   }
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Чтение файла индекса
_i ReadIDX(_i md, _i trd){
   _i fd=0, a=0, b=0;
   //-------------------------------------------------------------------------
   // чтение файла индекса     
   if (md==0){
       for(a=0;a<MRECI;a++){ IDXTIM[a].TIM=0; IDXTIM[a].nREC=0; }
       if((access(FNIdx,0))<0){
           fd=creat(FNIdx,S_IREAD|S_IWRITE); close(fd); IDXCNT=0;
       }
       if((fd=open(FNIdx,vktRDO))>0){
           read (fd,&IDXCUR,LEN__I);
           read (fd,&IDXCUR,LEN__I);
         b=read (fd,&IDXTIM,MRECI*LEN__I*2);
           close(fd);
       }
   }
   //-------------------------------------------------------------------------
   // запись файла индекса
   if (md==1){
       if (IDXCNT>0){
           if((fd=open(FNIdx,vktWRO,S_IREAD|S_IWRITE))>0){
               // в ячейке 0 - количество записей индекса
               lseek(fd,0,SEEK_SET);
               write(fd,&IDXCUR,LEN__I);  
               write(fd,&IDXCUR,LEN__I);  
               // массив записей индекса
               lseek(fd,0,SEEK_END);
               write(fd,&IDXTIM,LEN__I*IDXCNT*2);
               close(fd);
           };  IDXCNT=0;
       }
   }
   //-------------------------------------------------------------------------
   // чтение файла индекса для поиска
   if (md==2){
       IDXCURS=0;
       for(a=0;a<MRECI;a++){ IDXSER[a].TIM=0; IDXSER[a].nREC=0; }
       if((fd=open(TF[CTC].fi[trd],vktRDO))>0){
           read (fd,&IDXCURS,LEN__I);
           read (fd,&IDXCURS,LEN__I);
           read (fd,&IDXSER, MRECI*LEN__I*2);
           close(fd); 
       } else {
           sprintf(SMSG[MMSG],SMSG[71],TF[CTC].fi[trd]);
           DPrn(13,0,"%s\n",SMSG[MMSG]);
           return(1);
       };
   }
   //-------------------------------------------------------------------------
   return(0);
}
//---------------------------------------------------------------------------
