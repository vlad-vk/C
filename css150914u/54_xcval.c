// coding: cp866
//---------------------------------------------------------------------------
//  Добавление канала в массив обмена, если его там нету
_i  Ch_in_DNBW(int nv, uc fl){
    _i a=0, r=-1;
    if ((_i)fmod(CB[nv].dt,100)>=10 && NSTAT){
       if (HNBW<MNBW){
           for(a=0;a<HNBW;a++){
               if(DNBW[a]==nv){ /*DPrn(31,0,"E:Ch_in_DNBW:nv=%d\n",nv);*/ break; }
           }
           if(fl=='c'&&CBCMD>0) return(r);
           if(a==HNBW){
              DNBW[HNBW]=nv;
              NNBW[HNBW]=CB[nv].dn;
              SNBW[HNBW]=CB[nv].dd;
              CBDN[nv]=HNBW; r=HNBW;
              if(fl=='c') ENBW++;
              HNBW++;
              DPrn(31,0,"E:Ch_in_DNBW:nv=%d ENBW=%d HNBW=%d\n",nv,ENBW,HNBW);
           }
       }
    }
    return(r);
}
//---------------------------------------------------------------------------
_i  Ch_in_DTCP(int nv, uc fl){
    _i a=0, r=-1, n=0;
    if ((_i)fmod(CB[nv].dt,1000)>=100 && TSTAT){
       if (HTCP<MTCP){
           for(a=0;a<HTCP;a++){
               if(DTCP[a]==nv){ break; }
           }
           if(fl=='c'&&CBCMD>0) return(r);
           if(a==HTCP){
              n=CB[nv].dn;                  // номер удаленной станции на текущем канале
              if(n>0&&n<MWS){
              SCHN[n][HOWC[n]]=nv;          // номер канала на локальном компьютере CB[]
              RCHN[n][HOWC[n]]=CB[nv].dd;   // номер канала на контроллере
              HOWC[n]++;                    // кол-во каналов для каждой станции
              RSRV[HTCP]=n;                 // номер удаленной станции для каждого канала CB[]
              DTCP[HTCP]=nv;                // номера каналов TCP
              if(fl=='c'){ ETCP++; HOWE[n]++; }
              HTCP++;                       // кол-во обработанных переменных TCP
              DPrn(31,0,"E:Ch_in_DTCP:nv=%d ETCP=%d HTCP=%d\n",nv,ETCP,HTCP);
              }
           }
       }
    }
    return(r);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
_f Arr_to_Val(_i na, _i nv){
   //------------------------------------------------------------------------
   // Общая база каналов
   if (na==0){ return(  CB[nv].v); }
   // Локальная база каналов
   if (na==1){ return( CBL[nv]);   }
   // База каналов драйвера COM портов
   if (na==2){ return( CBS[nv]);   }
   // База каналов драйвера NetBios
   if (na==3){ return( CBN[nv]);   }
   // База каналов драйвера TCP/IP
   if (na==4){ return( CBT[nv]);   }
   // Массив временных срезов трендов
   if (na==5){ return(VTRT[nv]);   }
   // Массив значений комманды 'y'
   if (na==6){ return( YCM[nv]); }
   // Массив временных значений
   if (na==7){ return(RTMP[nv]); }
   //------------------------------------------------------------------------
   return(0);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
vd Val_to_Arr(_i na, _i nv, _f val, _i md, uc fl){
   int a=0;
   //------------------------------------------------------------------------
   // Общая база каналов
   if (na==0){
       // меняем значение
       if(md==CBMODYES){
          CB[nv].v=val;
          FLSER=0; FLNBW=0; FLTCP=0;
          if ( (_i)fmod(CB[nv].dt,10  )>  0 && SSTAT){ FLSER=1; }
          if ( (_i)fmod(CB[nv].dt,100 )>=10 && NSTAT){ FLNBW=1; }
          if ( (_i)fmod(CB[nv].dt,1000)>=10 && TSTAT){ FLTCP=1; }
          // если установлен флаг обмена по NetBIOS и нет таймаута NetBIOS-
          // установить значение на канале обмена NetBIOS
          if ( FLNBW>0 && TO_NBW==0 ){
             // Если в массиве нет устанавливаемого канала- добавить его туда,
             // а если есть -  передать в него значение
             a=Ch_in_DNBW(nv,fl);
             if(a<0){
                // передача значения в канал
                // if(fl=='c'){ DPrn(31,1,"31:Val_to_Arr for CMD: nv=%d val=%f CBCMD=%d\n",nv,val,CBCMD); }
                FNBS[ CBDN[nv] ] = 2;
                CBN [ CBDN[nv] ] = CB[nv].v;
             } 
             if(a>0){
                // когда создан новый канал переменной экрана
                DPrn(31,1,"31:NBM_SET: na=0 nv=%d CB[nv].v=%f CB[nv].dt=%d CBDN[nv]=%d\n",
                                            nv,   CB[nv].v,   CB[nv].dt,   CBDN[nv]);
             }
          }
          // если установлен флаг обмена по COM порту и нет таймаута порта-
          // установить значение на канале обмена COM порта
          if ( FLSER>0 && TO_COM==0){
               CSSER=nv;
               FSRS[ CBSR[CSSER] ] = 2;
               CBS [ CBSR[CSSER] ] = CB[CSSER].v;
               // если значение устанавливается из файла CMD
               if(fl=='c'){
                  // если установлен флаг передачи значения
                  if(VALCMD>0){
                     COMPutCH(CurSP, CB[CSSER].dn, CB[CSSER].dd, CB[CSSER].v);
                     out_COM (CurSP);
                  }
               }
               DPrn(31,1,"31:2:CSSER: na=0 nv=%d CB[nv].v=%f\n",nv,CB[nv].v);
          }
       }
   }
   // Локальная база каналов
   if (na==1){  CBL[nv]=val;  }
   // База каналов драйвера COM портов
   if (na==2){  CBS[nv]=val; FSRS[nv]=2; }
   // База каналов драйвера NetBios
   if (na==3){  CBN[nv]=val; FNBS[nv]=2; }
   // База каналов драйвера TCP/IP
   if (na==4){  CBT[nv]=val;  }
   // Массив временных срезов трендов
   if (na==5){ VTRT[nv]=val;  }
   // Массив значений комманды 'y'
   if (na==6){  YCM[nv]=val;  sprintf(LMVL[nv],"%9f",val); }
   // Массив временных значений
   if (na==7){ RTMP[nv]=val;  }
   return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// фиксированное изменение значений каналов
vd XCVal(_i nm){
   _f val=0;

   // если ввод данных запрещен
   if (DisC>0){ return; }

   if (LXCV[nm].nv==0){
       return;
   }

   // =
   if (LXCV[nm].zn==0){
       DPrn(31,1,"VTA: nm=%d na=%d nv=%d val=%f\n",
                         nm,LXCV[nm].na, LXCV[nm].nv, LXCV[nm].vc);
       Val_to_Arr(LXCV[nm].na, LXCV[nm].nv, LXCV[nm].vc, CBMODYES,'x');
   }

   // +
   if (LXCV[nm].zn==1){
       val=(Arr_to_Val(LXCV[nm].na, LXCV[nm].nv) + LXCV[nm].vc);
       Val_to_Arr(LXCV[nm].na, LXCV[nm].nv,val, CBMODYES,'x');
   }

   // -
   if (LXCV[nm].zn==2){
       val=(Arr_to_Val(LXCV[nm].na, LXCV[nm].nv) - LXCV[nm].vc);
       Val_to_Arr(LXCV[nm].na, LXCV[nm].nv,val, CBMODYES,'x');
   }

   // *
   if (LXCV[nm].zn==3){
       val=(Arr_to_Val(LXCV[nm].na, LXCV[nm].nv) * LXCV[nm].vc);
       Val_to_Arr(LXCV[nm].na, LXCV[nm].nv,val, CBMODYES,'x');
   }

   // f
   if (LXCV[nm].zn==4){

   }

   return;
}
//---------------------------------------------------------------------------
