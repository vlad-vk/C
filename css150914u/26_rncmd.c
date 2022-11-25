// coding: cp866
//----------------------------------------------------------------------------
//  разбор буфера комманд
_i  FillCMD(_i HBR){
    _i  a=0, n=0, l=0, f=0, i=0, y=0, z=0;
    uc  c=0;
    sprintf(SMSG[MMSG],SMSG[88]);
    DPrn(9,0,"\n%s\n",SMSG[MMSG]);
    for (GStrPos=0;;){
       // взять каждую следующую строку из буфера
       if(( _mfGetStr(FBuf,HBR))==0){ break; }
       DPrn(9,0,"(%s)\n",GStr);
       if(strlen(GStr)<4){ continue; }
       //----------------------------------------------------------------------
       // заполнить поля из строки
       c=PutFields(0,GStr,' ',0);
       LCMD[HCMD].rcmd=0;
       LCMD[HCMD].tcmd=0;
       LCMD[HCMD].gcmd=HCMD+1;
       // обработать все поля
       for(a=0,n=0,z=0;a<HowFLD;a++){
           // если встретилось заполненное поле
           if(isFieldb(0,a,0)>0){
              // получить текущее поле в переменную stmp
              strcpy(stmp,GetField(0,a));
              // проверяем первый символ текущего поля:
              //---------------------------------------------------------------
              // если это комментарий
              if (stmp[0]=='#'){ break; }
              if (stmp[0]==';'){ break; }
              DPrn(9,0," [%s]",stmp);
              //---------------------------------------------------------------
              // если это вывод на экран
              if((stmp[0]=='O')||(stmp[0]=='o')){
                  // запомнить строку, которая будет выводиться на экран
                  stmp[0]=' '; strcpy(LCMD[HCMD].scmd,stmp);
                  // номер команды вывода на экран
                  LCMD[HCMD].rcmd=20;
                  continue;
              }
              //---------------------------------------------------------------
              // если это вторая часть оператора сравнения
              if(((stmp[0]=='E')||(stmp[0]=='e'))&&((stmp[1]=='L')||(stmp[1]=='l'))){
                  LCMD[HCMD].rcmd=55;
                  // номер текущей строки комманды записать в запомненный номер
                  LCMD[ WTIF[CCIF] ].gcmd=HCMD;
                  // запомнить заново текущий номер строки комманды
                  WTIF[CCIF]=HCMD;
              }
              // если это окончание оператора сравнения
              if(((stmp[0]=='F')||(stmp[0]=='f'))&&((stmp[1]=='I')||(stmp[1]=='i'))){
                  LCMD[HCMD].rcmd=56;
                  // номер текущей строки комманды записать в запомненный номер
                  LCMD[ WTIF[CCIF] ].gcmd=HCMD;
                  if(CCIF>0){ CCIF--; }
              }
              // если это оператор сравнения
              if(((stmp[0]=='I')||(stmp[0]=='i'))&&((stmp[1]=='F')||(stmp[1]=='f'))){
                  LCMD[HCMD].tcmd=2;
                  if(CCIF<MGIF-1){ CCIF++; }
                  // запомнить номер текущей строки комманды
                  WTIF[CCIF]=HCMD;
                  n++;
                  continue;
              }
              //---------------------------------------------------------------
              // если переменная определена именем- определить ее номер
              if (stmp[0]=='$'){
                  l=strlen(stmp);
                  // передвинуть строку на символ влево
                  for(i=0; i<l-1; i++){ stmp[i]=stmp[i+1]; }; stmp[i]=0;
                  // сопоставить полученное имя с именами в общем массиве
                  for(i=0; i<MCH; i++){
                      // если определенное имя было найдено- создать строку
                      // с номером канала  для последующей обработки (p0000)
                      if((strcasecmp(stmp, CB[i].nm))==0){
                          DPrn(9,0,"(%s=p%04d)",stmp,i);
                          sprintf(stmp,"p%04d",i); break;
                      }
                  }
                  // если переменная по имени не найдена- записать сообщение
                  if (i>=MCH){
                      sprintf(SMSG[MMSG],SMSG[179],stmp);
                      DPrn(9,0,"\n%s\n",SMSG[MMSG]);
                  }
              }
              //---------------------------------------------------------------
              // если переменная определена именем, как указатель на другую
              // переменную или  значение
              if (stmp[0]=='%'){
                  l=strlen(stmp);
                  // передвинуть строку на символ влево
                  for(i=0; i<l-1; i++){ stmp[i]=stmp[i+1]; }
                  stmp[i]=0; f=0;
                  //-----------------------------------------------------------
                  // если это первое определение имени переменной
                  if(z==0){
                     // искать указанное имя в массиве имен
                     for(i=0; i<HCCM; i++){
                         // если нашли такое имя- перейти на подстановку
                         // для  него переменной
                         if((strcasecmp(stmp, NCCM[i]))==0){ z=1; f=i; break; }
                     }
                     // если не нашли такое имя
                     if (i>=HCCM && i<MCCM && n==0){
                         // добавить имя переменной в массив имен
                         strcpy(NCCM[i],stmp);
                         // добавить переменную или значение, соответств имени
                         for(f=a+1;f<HowFLD;f++){
                             if(isFieldb(0,f,0)>0){
                                strcpy(stmp,GetField(0,f));
                                if(stmp[0]!='='){
                                   strcpy(PCCM[i],stmp);
                                   if(HCCM<MCCM-1) HCCM++; 
                                   break;
                                }
                             }
                         }
                         DPrn(9,0,"NCCM[%d]=[%s]\t\tPCCM[%d]=[%s]\n",
                                   i,NCCM[i],       i,PCCM[i]);
                         a=HowFLD; 
                         continue;
                     }
                  }
                  //-----------------------------------------------------------
                  // если это подстановка переменной или значения-
                  // искать указанное имя в массиве имен
                  if(z>0){
                     for(i=f; i<HCCM; i++){
                         // если нашли такое имя- подставить соотвующ значение
                         if((strcasecmp(stmp, NCCM[i]))==0){
                             strcpy(stmp,PCCM[i]); break;
                         }
                     }
                     // если ничего не нашли - подставить 0
                     if(i>=HCCM){ strcpy(stmp,"0"); }
                     DPrn(9,0,"PCCM[%d](stmp)=[%s]",i,stmp);
                  }
              }
              //---------------------------------------------------------------
              if((stmp[0]=='X')||(stmp[0]=='x')){
                  // получить номер переменной
                  stmp[0]=32; LCMD[HCMD].vcmd[n]=atoi(stmp);
                  // получить номер массива
                  LCMD[HCMD].ncmd[n]=7; if(n<2){ n++; };
                  continue;
              }
              if((stmp[0]=='P')||(stmp[0]=='p')){
                  stmp[0]=32; LCMD[HCMD].vcmd[n]=atoi(stmp);
                  LCMD[HCMD].ncmd[n]=0; if(n<2){ n++; };
                  continue;
              }
              if((stmp[0]=='L')||(stmp[0]=='l')){
                  stmp[0]=32; LCMD[HCMD].vcmd[n]=atoi(stmp);
                  LCMD[HCMD].ncmd[n]=1; if(n<2){ n++; };
                  continue;
              }
              if((stmp[0]=='C')||(stmp[0]=='c')){
                  stmp[0]=32; LCMD[HCMD].vcmd[n]=atoi(stmp);
                  LCMD[HCMD].ncmd[n]=2; if(n<2){ n++; };
                  continue;
              }
              if((stmp[0]=='N')||(stmp[0]=='n')){
                  stmp[0]=32; LCMD[HCMD].vcmd[n]=atoi(stmp);
                  LCMD[HCMD].ncmd[n]=3; if(n<2){ n++; };
                  continue;
              }
              if((stmp[0]=='T')||(stmp[0]=='t')){
                  stmp[0]=32; LCMD[HCMD].vcmd[n]=atoi(stmp);
                  LCMD[HCMD].ncmd[n]=4; if(n<2){ n++; };
                  continue;
              }
              if((stmp[0]=='Y')||(stmp[0]=='y')){
                  stmp[0]=32; LCMD[HCMD].vcmd[n]=atoi(stmp);
                  LCMD[HCMD].ncmd[n]=6; if(n<2){ n++; };
                  continue;
              }
              //---------------------------------------------------------------
              // установить признаки действий вычислений
              if (stmp[0]=='='&& LCMD[HCMD].tcmd==0){ LCMD[HCMD].rcmd=1; continue; }
              if (stmp[0]=='+'){ LCMD[HCMD].rcmd=2; continue; }
              if (stmp[0]=='-'){ LCMD[HCMD].rcmd=3; continue; }
              if (stmp[0]=='*'){ LCMD[HCMD].rcmd=4; continue; }
              if (stmp[0]=='/'){ LCMD[HCMD].rcmd=5; continue; }
              // установить признаки сравнений
              if (stmp[0]=='='&&LCMD[HCMD].tcmd==2){ LCMD[HCMD].rcmd=51; continue; }
              if (stmp[0]=='!'&&LCMD[HCMD].tcmd==2){ LCMD[HCMD].rcmd=52; continue; }
              if (stmp[0]=='>'&&LCMD[HCMD].tcmd==2){ LCMD[HCMD].rcmd=53; continue; }
              if (stmp[0]=='<'&&LCMD[HCMD].tcmd==2){ LCMD[HCMD].rcmd=54; continue; }
              // если дошли до этого места - значит это константа
              // установить признак константы
              if (stmp[0]>47&&stmp[0]<58){
                  LCMD[HCMD].ncmd[n]=8;
                  // запомнить значение
                  LCMD[HCMD].zcmd[n]=atof(stmp); if(n<2){ n++; }
              }
           }
       }
       //----------------------------------------------------------------------
       // если это конец комманды и строки и была определена хоть одна команда
       if(LCMD[HCMD].rcmd>0){
          LCMD[HCMD].fcmd=1;
          if(HCMD<MCMD-1){ HCMD++; }
          ClearGStr();
       }
       DPrn(9,0,"\n");
    }
    return(0);
}
//----------------------------------------------------------------------------



//----------------------------------------------------------------------------
//  выполнение комманд из структуры комманд
_i  RunCMD(vd){
    _i  a=0, rel[MGIF], cur=0;
    _f  v[3];
    uc  s[MGIF+1];
        s[0]=' '; s[1]=0;
    for(a=0;a<HCMD;a++){
        if(LCMD[a].fcmd==1){
           if(strlen(s)<1){ s[0]=' '; s[1]=0; }
           // заполненные константы
           if(LCMD[a].ncmd[1]==8){
              v[1]=LCMD[a].zcmd[1];
           } else {
              v[1]=Arr_to_Val(LCMD[a].ncmd[1], LCMD[a].vcmd[1]);
           }
           if(LCMD[a].ncmd[2]==8){
              v[2]=LCMD[a].zcmd[2];
           } else {
              v[2]=Arr_to_Val(LCMD[a].ncmd[2], LCMD[a].vcmd[2]);
           }
           // операции выполнения и функции
           if(LCMD[a].rcmd==1){                                           // =
              v[0]=v[1]; 
              DPrn(9,0,"%s[na:%d nv:%d] = %f\n",
                         s,LCMD[a].ncmd[0],LCMD[a].vcmd[0],v[1]);
           }                              
           if(LCMD[a].rcmd==2){                                           // +
              v[0]=v[1]+v[2]; 
              DPrn(9,0,"%s[na:%d nv:%d](%f) = [na:%d nv:%d](%f) + [na:%d nv:%d](%f)\n",
                         s,LCMD[a].ncmd[0],LCMD[a].vcmd[0], v[0],
                           LCMD[a].ncmd[1],LCMD[a].vcmd[1], v[1],
                           LCMD[a].ncmd[2],LCMD[a].vcmd[2], v[2]);
           }                         
           if(LCMD[a].rcmd==3){                                           // -
              v[0]=v[1]-v[2]; 
              DPrn(9,0,"%s[na:%d nv:%d](%f) = [na:%d nv:%d](%f) - [na:%d nv:%d](%f)\n",
                         s,LCMD[a].ncmd[0],LCMD[a].vcmd[0], v[0],
                           LCMD[a].ncmd[1],LCMD[a].vcmd[1], v[1],
                           LCMD[a].ncmd[2],LCMD[a].vcmd[2], v[2]);
           }                         
           if(LCMD[a].rcmd==4){                                           // *
              v[0]=v[1]*v[2]; 
              DPrn(9,0,"%s[na:%d nv:%d](%f) = [na:%d nv:%d](%f) * [na:%d nv:%d](%f)\n",
                         s,LCMD[a].ncmd[0],LCMD[a].vcmd[0], v[0],
                           LCMD[a].ncmd[1],LCMD[a].vcmd[1], v[1],
                           LCMD[a].ncmd[2],LCMD[a].vcmd[2], v[2]);
           }                         
           if(LCMD[a].rcmd==5){                                           // :
              v[0]=divf(v[1],((v[2]==0)?1:v[2])); 
              DPrn(9,0,"%s[na:%d nv:%d](%f) = [na:%d nv:%d](%f) / [na:%d nv:%d](%f)\n",
                         s,LCMD[a].ncmd[0],LCMD[a].vcmd[0], v[0],
                           LCMD[a].ncmd[1],LCMD[a].vcmd[1], v[1],
                           LCMD[a].ncmd[2],LCMD[a].vcmd[2], v[2]);
           }
           // Установка значений в массивах переменных
           if(LCMD[a].rcmd< 10){
              Val_to_Arr(LCMD[a].ncmd[0],LCMD[a].vcmd[0],v[0],CBMODYES,'c');
//            DPrn(9,0,"Val_to_Arr( na=%d , nv=%d , vl=%f )\n", LCMD[a].ncmd[0], LCMD[a].vcmd[0], v[0]);
              continue;
           }
           // вывод на экран (на консоль)
           if(LCMD[a].rcmd==20){
              strcpy(YCNV.str,LCMD[a].scmd); MESOut((MOUT*)&YCNV,4,0); // обработка строки...
              printf("%s",YCNV.str);
              continue;
           }
           // операции сравнения (проверяем обратное)
           if(LCMD[a].rcmd> 50){
              if(LCMD[a].rcmd< 55){
                 if(cur<MGIF-1){ s[cur+1]=32; cur++; s[cur+1]=0; }
                 rel[cur]=0;
              }
           }
           if(LCMD[a].rcmd==51){                                     // ==
              DPrn(9,0,"%sif%02d [na:%d nv:%d](%f) == [na:%d nv:%d](%f)\n",
                         s,cur,LCMD[a].ncmd[1], LCMD[a].vcmd[1],v[1],
                               LCMD[a].ncmd[2], LCMD[a].vcmd[2],v[2]);
              if(v[1]!=v[2]){ a=LCMD[a].gcmd-1; rel[cur]=1; }
              continue;
           }
           if(LCMD[a].rcmd==52){                                     // !=
              DPrn(9,0,"%sif%02d [na:%d nv:%d](%f) != [na:%d nv:%d](%f)\n",
                         s,cur,LCMD[a].ncmd[1], LCMD[a].vcmd[1],v[1],
                               LCMD[a].ncmd[2], LCMD[a].vcmd[2],v[2]);
              if(v[1]==v[2]){ a=LCMD[a].gcmd-1; rel[cur]=2; }
              continue;
           }
           if(LCMD[a].rcmd==53){                                     // >
              DPrn(9,0,"%sif%02d [na:%d nv:%d](%f)  > [na:%d nv:%d](%f)\n",
                         s,cur,LCMD[a].ncmd[1], LCMD[a].vcmd[1],v[1],
                               LCMD[a].ncmd[2], LCMD[a].vcmd[2],v[2]);
              if(v[1]<=v[2]){ a=LCMD[a].gcmd-1; rel[cur]=3; }
              continue;
           }
           if(LCMD[a].rcmd==54){                                     // <
              DPrn(9,0,"%sif%02d [na:%d nv:%d](%f)  < [na:%d nv:%d](%f)\n",
                         s,cur,LCMD[a].ncmd[1], LCMD[a].vcmd[1],v[1],
                               LCMD[a].ncmd[2], LCMD[a].vcmd[2],v[2]);
              if(v[1]>=v[2]){ a=LCMD[a].gcmd-1; rel[cur]=4; }
              continue;
           }
           //------------------------------------------------------------------
           // если предыдущий IF выполнялся (rel[cur]>0) и есть секция EL, то
           // ее выполнение пропускаем
           if(LCMD[a].rcmd==55){
              DPrn(9,0,"%sel%02d\n",s,cur);
              if(rel[cur]==0){ a=LCMD[a].gcmd-1; }
              continue;
           }
           // окончание структуры IF (FI)
           if(LCMD[a].rcmd==56){
              DPrn(9,0,"%sfi%02d\n",s,cur);
              if(cur>0){ cur--; s[cur+1]=0; }
              continue;
           }
           //------------------------------------------------------------------
        }
    }

    //-------------------------------------------------------------------------
    // Установка кода нажатой клавиши из коммандного файла
    if(LKSET_CMD>0){ SCode=LASTKEY; LCode=LASTKEY; LKSET_CMD=0; }

    //-------------------------------------------------------------------------
    // Вызов указанного файла описания из коммандного файла
    if(FNSET_CMD>0){
//printf(" TEST\n");
       if(FNINI_CMD>0 && FNINI_CMD<MINI){
          strcpy(FNCfg   , SINI[(int)FNINI_CMD].fn);
          strcpy(PN_fn[0], PN_fn[1]);
          strcpy(PN_fn[1], FNCfg   );
//printf(" cf0=[%s] cf1=[%s]\n",PN_fn[0],PN_fn[1]);
          FNCfgLD=1;
       }; FNSET_CMD=0;
    }
    //-------------------------------------------------------------------------

    //-------------------------------------------------------------------------
    // Вывод сообщения (codus_al.ini) в строку статуса из коммандного файла
    if(NUMAL_CMD>0&&NUMAL_CMD<HALR){
       if(SECAL_CMD<1){ SECAL_CMD=10; }
       if(ERRAL_CMD>0){
          sprintf(ErrSTR,LALR[(_i)(NUMAL_CMD)].mes); ErrINI=1;
       } else {
          sprintf(STAT[1].str,LALR[(_i)(NUMAL_CMD)].mes);
       }
       STAT[1].cnt=SECAL_CMD; NUMAL_CMD=0; SECAL_CMD=0; ERRAL_CMD=0;
    }
    //-------------------------------------------------------------------------

    return(0);
}
//----------------------------------------------------------------------------
