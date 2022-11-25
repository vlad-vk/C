// coding: cp866
//---------------------------------------------------------------------------
// возвращает код последней нажатой клавиши
_i MLastKey(_i md){
   _i    md_key=0;
   // Получить состояние клавиш: Shift,Alt,Ctrl
   KSH=((key[KEY_LSHIFT  ])||(key[KEY_RSHIFT  ]))?1:0;
   KCT=((key[KEY_RCONTROL])||(key[KEY_LCONTROL]))?1:0;
   KAL=((key[KEY_ALT     ])||(key[KEY_ALTGR   ]))?1:0;
   if ( keypressed() ){
        md_key=readkey();
        ACode= md_key &  0xff ;                  // ASCII
        SCode=(md_key >> 8)   & 0xff;
        if (SCode==100){ SCode=SCode+ACode; }    // Scan
        LCode =SCode;                            // Scan Last (not Zero)
        LASTKEY=SCode;
        XCode=0;
   }
   if(KSH>0&&SCode>0){ ECode=SCode+CPSHIFT; XCode=ECode; }
   if(KCT>0&&SCode>0){ ECode=SCode+CPCTRL;  XCode=ECode; }
   if(KAL>0&&SCode>0){ ECode=SCode+CPALT;   XCode=ECode; }
   SCANCODE=SCode;                  // скан  код последней нажатой клавиши
   ASCICODE=ACode;                  // ASCII код ...
   return(0);
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
//  Выделение текущей записи активного окна отображаемого файла
vd  FVMarkCurRec(vd){
    _i nrec=0;
    _i x=0, y=0, w=0, h=0, col=LFVW[FVA].xc;
    if (HFVW<1){ return; }
    // соответствие положения мышки номеру записи файла отображения
    if (mouse_x>LFVW[FVA].X && mouse_x<(LFVW[FVA].X+LFVW[FVA].W) &&
        mouse_y>LFVW[FVA].Y && mouse_y<(LFVW[FVA].Y+LFVW[FVA].H)){
        // номер строки окна на которой установлена мышка
        nrec=abs((mouse_y-LFVW[FVA].Y)/FVCURFNH);
        // номер записи в БД окна активного файла под мышкой
        FVDBCREC=FVCURREC+nrec;
        // пересчитать координаты
        x=LFVW[FVA].X+LFVW[FVA].xx;
        y=LFVW[FVA].Y+LFVW[FVA].xy;
        if(LFVW[FVA].xw<=0){ w=LFVW[FVA].W;  }
        else               { w=LFVW[FVA].xw; }
        if(LFVW[FVA].xh<=0){ h=FVCURFNH; }
        else               { h=LFVW[FVA].xh; }
        // если положение для текущей записи меньше,
        // чем кол-во записей в окне- отобразить выделение
        if(nrec<=FVCURHST){
           xor_mode(TRUE);
           rectfill(scrout, x,
                            y+ nrec*FVCURFNH,
                            x+ w,
                            y+ nrec*FVCURFNH+h, col);
           solid_mode();
        }
        // если была нажата левая  клав мыши- посылаем сканкод опред клавиши
        if((MSKey==MOUSE_KEYL)){ SelCode=LFVW[FVA].kl; }
        // если была нажата правая клав мыши- посылаем сканкод опред клавиши
        if((MSKey==MOUSE_KEYR)){ SelCode=LFVW[FVA].kr; }
//      sprintf(STAT[1].str,"awf_rec=%d rec_in_db=%.0f",nrec,FVDBCREC); STAT[1].cnt=3;
    }
    return;
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// вывод выделенных областей
vd SelMARK(vd){
   _i  a=0,b=0,i=0;
   _i  c=0;
   _f  v=0;
   blit(scrsav,scrout,0,0,0,0,XMAX,YMAX);
   for(a=0;a<HSLM;a++){
       // если определена фигура
       if(LSLM[a].np>0){
          v=Arr_to_Val(LSLM[a].na, LSLM[a].nv);
          c=LSLM[a].cn;
          if(v< LSLM[a].vl){ c=LSLM[a].cl; }
          if(v> LSLM[a].vh){ c=LSLM[a].ch; }
          if(LSLM[a].md< 1){ LSLM[a].md=1; }
          if(LSLM[a].md> LSLM[a].H/2){ LSLM[a].md=LSLM[a].H/2; }
          //-----------------------------------------------------------------
          // вывод прямоугольника (xor)
          if(LSLM[a].np==1&&c>=0){
             for(b=0;b<=LSLM[a].fl;b++){
                 xor_mode(TRUE);
                 for(i=0;i<LSLM[a].H;i++){
                     // нарисовать последнюю линию
                     if(LSLM[a].Y+i*LSLM[a].md>=LSLM[a].Y+ LSLM[a].H){
                        rectfill(scrout, LSLM[a].X,
                                         LSLM[a].Y+LSLM[a].H,
                                         LSLM[a].X+LSLM[a].W,
                                         LSLM[a].Y+LSLM[a].H, c);
                        break;
                     }
                     // закрасить область линиями черезстрочно
                     rectfill(scrout, LSLM[a].X,
                                      LSLM[a].Y+i*LSLM[a].md,
                                      LSLM[a].X+LSLM[a].W,
                                      LSLM[a].Y+i*LSLM[a].md, c);
                 }
                 solid_mode();
             }
          }
          // вывод прямоугольника (просто указанным цветом)
          if(LSLM[a].np==11&&c>=0){
             rectfill(scrout, LSLM[a].X,           LSLM[a].Y,
                              LSLM[a].X+LSLM[a].W, LSLM[a].Y+LSLM[a].H, c);
          }
          //-----------------------------------------------------------------
          // вывод круга (xor)
          if(LSLM[a].np==2&&c>=0){
             for(b=0;b<=LSLM[a].fl;b++){
                 xor_mode(TRUE);
                 circlefill(scrout,LSLM[a].X,LSLM[a].Y,LSLM[a].W, c);
                 solid_mode();
             };
          }
          // вывод круга (просто указанным цветом)
          if(LSLM[a].np==12&&c>=0){
             circlefill(scrout, LSLM[a].X, LSLM[a].Y, LSLM[a].W, c);
          }
          //-----------------------------------------------------------------
       }
   }
   return;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// выделение цветом текущей области
vd SelMask(_i sn, _i color){
   xor_mode(TRUE);
   rectfill(scrsav, LSEL[sn].X, LSEL[sn].Y,
                    LSEL[sn].X+ LSEL[sn].W, LSEL[sn].Y+ LSEL[sn].H, color);
   solid_mode();
   return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
_i MMousSEL(_i n){
   _i a=0, b=0;
   if(n>0){
      if(LKNP[ LSEL[ASEL].sp ].vw>0&&ASEL!=NOSEL){
         draw_sprite(scrsav,LKNP[ LSEL[ASEL].sp ].sp[n],
                            LSEL[ASEL].X+LSEL[ASEL].sx,
                            LSEL[ASEL].Y+LSEL[ASEL].sy);
      }; return(1);
   }
   //------------------------------------------------------------------------
   // для каждой записи описания областей
   for(a=0,b=0;a<HSEL;a++,b=0){
      // если текущие координаты курсора мышки совпали с координатами области
      if (mouse_x>LSEL[a].X && mouse_x<(LSEL[a].X+LSEL[a].W) &&
          mouse_y>LSEL[a].Y && mouse_y<(LSEL[a].Y+LSEL[a].H)){
          // форма курсора
          curp=1; b=1; ASEL=a;
          // строка описания области- в строку статуса
          if(ErrCNT==0){
             sprintf(STAT[1].str,"%s", LSEL[a].str); STAT[1].cnt=1;
          }
          // если была нажата лев клав мыши- посылаем сканкод опред клавиши
          if((MSKey==1)&&(MSKeyL==0)){ SelCode=LSEL[a].k; b=2; FLSEL=b; }
          break;
      }
   }
   //------------------------------------------------------------------------
   if (b>0){
      if((LSEL[a].sp>=0)&&(LSEL[a].sp<MKNP)){
         // снять цветовую маску с текущей области
         if (FSLO!=a&&FSLF==1){
             SelMask(FSLO,LSEL[FSLO].cs); FSLF=0;
         }
         // восстановить экран под предыдущим спрайтом области и
         // запомнить под текущим
         SRSelect(1,FSLO); SRSelect(0,a); FSLO=a;
         // выделить цветовой маской текущую область
         if (FSLF==0){
             SelMask(FSLO,LSEL[FSLO].cs); FSLF=1;
         }
         // если определено отображение кнопки- рисуем ее
         if(LKNP[ LSEL[a].sp ].vw>0){
            draw_sprite(scrsav,LKNP[ LSEL[a].sp ].sp[b],
                               LSEL[a].X+LSEL[a].sx,
                               LSEL[a].Y+LSEL[a].sy);
         }
      }
   } else {
      // снять цветовую маску, если ушли с области
      if (FSLF==1){
          SelMask(FSLO,LSEL[FSLO].cs); FSLF=0;
      }
      // восстановить экран под спрайтом предыдущей области
      SRSelect(1,FSLO);
      ASEL=NOSEL;           // признак отсутствия активной области
   }
   //------------------------------------------------------------------------
   return(0);
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// определение нажатий клавиш мышки
_i MMousKey(vd){
   _i  a=0, b=0, cm=0;
   _f  trd_s1=0, vs=0;
   MSKey=0; curp=0;
   // нажата левая  клавиша мышки
   if (mouse_b & 1){ MSKey=MOUSE_KEYL; }
   // нажата правая клавиша мышки
   if (mouse_b & 2){ MSKey=MOUSE_KEYR; }
   // вывод координат мышки в строке статуса
   if (MSCFL==1){
      if(ErrCNT==0){
         sprintf(STAT[1].str,"M: X=%d,Y=%d, MsKey=%d S=%d;  Kbd: LC=%d,EC=%d KCT=%d,KAL=%d,KSH=%d",
         mouse_x,mouse_y,MSKey,XSpeed,LCode,XCode,KCT,KAL,KSH);
         STAT[1].cnt=5;
      }; curp=1;
   }
   // если курсор мышки установлен в левый верхний угол-
   // вывести в строку статуса текущее имя файла описания
   if(mouse_x==0 && mouse_y==0){
      if(ErrCNT==0){
         sprintf(STAT[1].str,SMSG[180],PN_fn[0],PN_fn[1],FNCfg);
         STAT[1].cnt=5;
      }
   }
   // действия при отпускании левой клавиши мышки
   if (MSKeyO==1 && MSKey==0){
       // если нажата клавиша ALT - передать координаты мыши в буфер обмена
       if(KAL>0){ WndCmd(4); }
       a=0;
   }
   // действия при отпускании правой клавиши мышки
   if (MSKeyO==2 && MSKey==0){
       // если нажата клавиша ALT - передать координаты мыши в буфер обмена
       if(KAL>0||KCT>0){ WndCmd(3); }
       // Если два раза нажата правая клавиша - возврат на предыдущий экран
       if (MSKP[0]==1){ MSKP[0]=2; MSKP[1]=0; MSKP[2]=0; SCode=KEY_P; }
       if (MSKP[0]==0){ MSKP[0]=1; MSKP[2]=0; }
       a=0;
   }
   MSKP[2]++; if(MSKP[2]>10){ MSKP[0]=0; MSKP[1]=0; MSKP[2]=0; }
   MSKeyO=MSKey;
   //
   if (SCode   > 0){ return(0); }
   if (LGET_en > 0){ return(0); }
   //------------------------------------------------------------------------
   // для каждой записи описания меню
   for(a=0;a<HMNU;a++){
      if (LMNU[a].k==0||LMNU[a].vw==0){ continue; }
      // если текущие координаты курсора мышки совпали с координатами
      // пункта меню - выводим рамку и описание
      if ( mouse_x>LMNU[a].X && mouse_x<(LMNU[a].X+LMNU[a].W) &&
           mouse_y>LMNU[a].Y && mouse_y<(LMNU[a].Y+LMNU[a].H)){
           if (vmnu!=a){
              if (LMNU[vmnu].sf>0){
                  draw_sprite(scrsav,LKNP[ LMNU[vmnu].bn ].sp[0],
                              LMNU[vmnu].X,LMNU[vmnu].Y);
              }
              LMNU[vmnu].bf=1;
              SRMenu(1,vmnu); vmnu=a;
              SRMenu(0,vmnu);
           }
           // вывести пояснение пункта меню в строку статуса
           if(ErrCNT==0){
              strcpy(STAT[1].str,LMNU[vmnu].str); STAT[1].cnt=10;
           }
           // вывести спрайт активного пункта - если он определен
           if (LMNU[a].sf>0){
              if(MSKey==1){
                 draw_sprite(scrsav,LKNP[ LMNU[a].bn ].sp[2],
                                    LMNU[a].X,LMNU[a].Y);
                 LMNU[a].sf=3; LMNU[a].bf=3;
              }  else  {
                 draw_sprite(scrsav,LKNP[ LMNU[a].bn ].sp[1],
                                    LMNU[a].X,LMNU[a].Y);
                 LMNU[a].sf=2; LMNU[a].bf=2;
              }
           }
           curp=2; SCode=0; cm=a; ReDraw=1;
           // если была нажата левая клавиша - посылаем сканкод
           // клавиши ENTER
           if (MSKey==1 && MSKeyL==0){ SCode=KEY_ENTER; }
           break;
      }    else    {
           cm=0;
           // если увели мышку с кнопки
      }
   }
   //------------------------------------------------------------------------
   // для каждой записи описания трендов
   for(a=0;a<HTRD;a++){
      // если текущие координаты курсора мышки совпали с координатами тренда
      if ( mouse_x>LTRD[a].X2 && mouse_x<(LTRD[a].X2+LTRD[a].W2) &&
           mouse_y>LTRD[a].Y2 && mouse_y<(LTRD[a].Y2+LTRD[a].H2) &&
          (MSKey==1||MSKey==2)){
           trd_s1=divf(LTRD[a].W2,LTRD[a].sec);
           TF[CTC].lt[a]=0;
           for(b=0;b<LTRD[a].sec;b++){
               vs=LTRD[a].X2+LTRD[a].W2-trd_s1*TF[CTC].lt[a];
               if(vs<mouse_x){ break; }; TF[CTC].lt[a]++;
           }
           if(MSKey==2){ TF[CTC].lm[a]=1; }
           else        { TF[CTC].lm[a]=0; }
           TLKey=1; break;
      }
   }
   //------------------------------------------------------------------------
   MMousSEL(0);
   MSKeyL=MSKey;
   // если курсор на пункте меню - запомнить его позицию (в процентах...)
   if (curp==2){
       MSdX1=divf( (100*(mouse_x-LMNU[cm].X)), ((LMNU[cm].W>0)?LMNU[cm].W:1));

       MSdY1=divf( (100*(mouse_y-LMNU[cm].Y)), ((LMNU[cm].H>0)?LMNU[cm].H:1));
   } else {
       MSdX1=0; MSdY1=0;
   }
   // проверка установки мышки на ползунке
   if (curp==0 && MCurPL==1){
       curp=2;
   }
   // установить изображение курсора и фокус(центр) для него
   set_mouse_sprite(curb[curp]);
   set_mouse_sprite_focus(curx[curp],cury[curp]);
   return(0);
}
//---------------------------------------------------------------------------
_i PutButton(vd){
   _i  a=0, r=0;
   for(a=0;a<HMNU;a++){
      if(LMNU[a].sf >0){
         if(LMNU[a].bf==1){
            draw_sprite(scrsav,LKNP[ LMNU[a].bn ].sp[0],
                        LMNU[a].X,LMNU[a].Y); r=1;
         }
         if(LMNU[a].vw >0){
            if(LMNU[a].bf==2){
               draw_sprite(scrsav,LKNP[ LMNU[a].bn ].sp[1],
                           LMNU[a].X,LMNU[a].Y); r=2;
            }
            if(LMNU[a].bf==3){
               draw_sprite(scrsav,LKNP[ LMNU[a].bn ].sp[2],
                           LMNU[a].X,LMNU[a].Y); r=3;
            }
         }
      }
   }
   return(r);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// проверка места на диске и удаление "старых" файлов архива
// предполагаем, что файлы читаются по мере их записи (от "старых" к "новым")
_i gDiskFree(_i md){
   _i  a=0, b=0, f=0, r=0, secsut=60*60*24;
   sprintf(SMSG[MMSG],SMSG[134],CDrive+65);
   DPrn(13,0,"%s\n",SMSG[MMSG]);
   // получить количество свободного места на диске
   _getdiskfree(CDrive+1,&gDSKSP);
   HMB_Disk=gDSKSP.bytes_per_sector * gDSKSP.sectors_per_cluster *
            gDSKSP.avail_clusters;
   // если не определена переменная минимального размера своб места на диске-
   // определяем ее: колич секунд в сутки * на длинну одной записи архива +
   // макс колич записей отчета * SLEN + колич секунд в сутки * на длинну
   // записи индекса
   if(ARCminsz<MINDSKSZ){
      ARCminsz=(secsut*dbHeaw[ARCOut].RecLen)+(secsut*SLEN)+(secsut*10);
   }
   // если неправильно определили свободное место (для больших дисков)
   if(HMB_Disk<0){ HMB_Disk=ARCminsz; }
   // если это только проверка свободного места
   if(md==1){
      sprintf(SMSG[MMSG],SMSG[135]);
      DPrn(13,0,"%s\n",SMSG[MMSG]);
      if(HMB_Disk<ARCminsz){ return(1); }
      return(0);
   }
   // записать имена файлов в массив ARCfname
   sprintf(FNTmp,"%s/arc/*.dba",FDir);
   r=al_findfirst(FNTmp,&al_ffblk,FA_ARCH);
   for(a=0;a<MARCF;a++){
       if(r!=0){ break; }
       for(b=strlen(al_ffblk.name);b>0;b--){
           if(al_ffblk.name[b]=='.'){ al_ffblk.name[b]=0; break; }
       }
       strcpy(ARCfname[a], al_ffblk.name); ARCftime[a]=al_ffblk.time;
       r=al_findnext (&al_ffblk);
   }
   al_findclose(&al_ffblk); ARCfname[a][0]=0; f=a;
   // сортировка массива имен архивных файлов
   for(a=0;a<f;a++){
       if (ARCftime[a+0]>ARCftime[a+1]){
           SRTftime     =ARCftime[a+0];
           ARCftime[a+0]=ARCftime[a+1];
           ARCftime[a+1]=SRTftime;
           strcpy(SRTfname,     ARCfname[a+0]);
           strcpy(ARCfname[a+0],ARCfname[a+1]);
           strcpy(ARCfname[a+1],SRTfname     );
       }
       for(b=0;b<f;b++){
           if (ARCftime[b+0]>ARCftime[b+1]){
               SRTftime     =ARCftime[b+0];
               ARCftime[b+0]=ARCftime[b+1];
               ARCftime[b+1]=SRTftime;
               strcpy(SRTfname,     ARCfname[b+0]);
               strcpy(ARCfname[b+0],ARCfname[b+1]);
               strcpy(ARCfname[b+1],SRTfname     );
           }
       }
   }

   // DPrn(0,0,"Файлы архивов:\n");
   // for(a=0;a<f;a++){ DPrn(0,0,"%s\n",ARCfname[a]); }

   // если на диске мало места или архивных файлов больше, чем определено
   // в установках - удалить "старые" файлы архива
   if((HMB_Disk<ARCminsz)||(a>ARChmday)){
      if(HMB_Disk<ARCminsz){
         sprintf(SMSG[MMSG],SMSG[136],HMB_Disk,ARCminsz);
         DPrn(0,0,"%s\n",SMSG[MMSG]);
      } else {
         sprintf(SMSG[MMSG],SMSG[137],a,ARChmday);
         DPrn(0,0,"%s\n",SMSG[MMSG]);
      }
      for(a=0;a<ARChfdel;a++){
          if(ARCfname[a][0]==0){ break; }
          sprintf(FNTmp,"%s/arc/%s.dba",FDir,ARCfname[a]);
          if((strcasecmp(FNTmp,FNArc))!=0&&(access(FNTmp,6))==0){
              remove (FNTmp);
              sprintf(FNTmp,"%s/arc/%s.dbr",FDir,ARCfname[a]);
              remove (FNTmp);
              sprintf(FNTmp,"%s/arc/%s.idx",FDir,ARCfname[a]);
              remove (FNTmp);
              sprintf(SMSG[MMSG],SMSG[138],FDir,ARCfname[a]);
              DPrn(0,0,"%s\n",SMSG[MMSG]);
          }
      }
   }
   sprintf(SMSG[MMSG],SMSG[139]);
   DPrn(13,0,"%s\n",SMSG[MMSG]);
   sprintf(SMSG[MMSG],SMSG[140],f,ARChmday);
   DPrn(13,0,"%s\n",SMSG[MMSG]);
   sprintf(SMSG[MMSG],SMSG[141],HMB_Disk,ARCminsz);
   DPrn(13,0,"%s\n",SMSG[MMSG]);
   return(0);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// запомнить экран
_i savscr(_i md){
   if (LockKBD>=MWIN){ return(0); }
   //------------------------------------------------------------------------
   // если есть выведенное окно - запомнить его
   if (md==0){
      blit(scrsav,pcxval,LWIN[LockKBD].X,LWIN[LockKBD].Y,
                     0,0,LWIN[LockKBD].W,LWIN[LockKBD].H);
      blit(scrsav,pcxtmp,0,0,0,0,XMAX,YMAX);
   }
   //------------------------------------------------------------------------
   // если есть запомненное окно - восстановить его
   if (md==1){
      blit(pcxval,pcxtmp,0,0,LWIN[LockKBD].X,LWIN[LockKBD].Y,
                             LWIN[LockKBD].W,LWIN[LockKBD].H);
      blit(pcxtmp,scrsav,0,0,0,0,XMAX,YMAX);
   }
   //------------------------------------------------------------------------
   return(0);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Ожидание запросов для оператора
vd RunOPQ(_i md){
   _i  a=0;
   _f  v=0;
   //-----------------------------------------
   if(LOPQ[COPQ].fr==1){
      if(LOPQ[COPQ].gr>0&&md>0){ GOPK=LOPQ[COPQ].ka; }
      if(COPK>0){
         if(COPK==LOPQ[COPQ].k1){
            Val_to_Arr(LOPQ[COPQ].na, LOPQ[COPQ].nv, LOPQ[COPQ].v1, CBMODYES,'q');
            if(LOPQ[COPQ].g1>0&&LOPQ[COPQ].g3<1000){ GOPK=LOPQ[COPQ].g1; }
            LOPQ[COPQ].fr=0; COPQ++; if(COPQ>=MOPQ){ COPQ=0; };  COPK=0;
         }
         if(COPK==LOPQ[COPQ].k2){
            Val_to_Arr(LOPQ[COPQ].na, LOPQ[COPQ].nv, LOPQ[COPQ].v2, CBMODYES,'q');
            if(LOPQ[COPQ].g2>0&&LOPQ[COPQ].g3<1000){ GOPK=LOPQ[COPQ].g2; }
            LOPQ[COPQ].fr=0; COPQ++; if(COPQ>=MOPQ){ COPQ=0; };  COPK=0;
         }
         if(COPK==LOPQ[COPQ].k3){
            Val_to_Arr(LOPQ[COPQ].na, LOPQ[COPQ].nv, LOPQ[COPQ].v3, CBMODYES,'q');
            if(LOPQ[COPQ].g3>0&&LOPQ[COPQ].g3<1000){ GOPK=LOPQ[COPQ].g3; }
            LOPQ[COPQ].fr=0; COPQ++; if(COPQ>=MOPQ){ COPQ=0; };  COPK=0;
         }
      }
      if(COPK>0){ return; }
   }
   //-----------------------------------------
   for(a=0;a<MOPQ;a++){
       if(LOPQ[COPQ].fr==0){
          v=Arr_to_Val(LOPQ[COPQ].na, LOPQ[COPQ].nv);
          if(v>10){
             GOPK=LOPQ[COPQ].ka;
             COPK=0;
             LOPQ[COPQ].fr=1;
             return;
          }
          COPQ++; if(COPQ>=MOPQ){ COPQ=0; break; }
       }
   }
   //-----------------------------------------
   return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Функции деления
_f divf(_f p1, _f p2){
    if (p2==0){ return(p1); }
                return(p1/p2);
}
//---------------------------------------------------------------------------
_i divi(_i p1, _i p2){
    if (p2==0){ return(p1); }
                return(p1/p2);
}
//---------------------------------------------------------------------------
