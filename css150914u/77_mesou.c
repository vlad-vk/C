// coding: cp866
//---------------------------------------------------------------------------
// повторный вывод строк меню
vd MenuStrOUT(vd){
   _i  b=0;
   for(b=0; b<HSTRC; b++) {
       if (vSTR[b]==5){ MESOut((MOUT*)&LSTR[b],3,b); }
   }
   return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// вывод системных сообщений
vd smPRN(uc sfs[SLEN],_i pY,_i cl){
   sfn=strlen(sfs);
   if (pY>0){ sfy=sfy+sfh; sfx=1; }
   textout(screen,dFnt[SFNT].dat,sfs,sfx,sfy,cl);
// textout_ex(screen, dFnt[SFNT].dat, sfs, sfx, sfy, makecol24(255, 0, 0), 0);
   if (sfx+sfn*sfl<XMAX){ sfx=sfx+sfn*sfl;    }
   else                 { sfy=sfy+sfh; sfx=1; }
   return;
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// записать сообщение, которое добавил оператор, в файл сообщений 
_i AddOpMess(_i nW){
   _i nMess=0;
   _c sMess[4]={0,0,0,0};
   if(HSTT<1||nW>=HSTT||nW<0){ return(0); }
   if(LSTT[nW].ES[0]=='#'&&LSTT[nW].ES[1]=='#'){
      sMess[0]=LSTT[nW].ES[2]; 
      sMess[1]=LSTT[nW].ES[3];
      sMess[2]=LSTT[nW].ES[4];
      nMess=atoi(sMess); if(nMess<0||nMess>=MALR){ nMess=0; }
      sprintf(MESSWR,"%02d:%02d:%02d >>> %03d %s",
      tmm->tm_hour,tmm->tm_min,tmm->tm_sec,nMess,LALR[nMess].mes);
   } else {
      sprintf(MESSWR,"%02d:%02d:%02d >>> %s",
      tmm->tm_hour,tmm->tm_min,tmm->tm_sec,LSTT[nW].ES);
   }
   sprintf(MESSCOL,"~%03d^%03d",OpCS, OpCG);
   // если открыт файл отчета - добавить новое сообщение
   if (RepFL>0){
       if (dbOpenTest(REPOut,0)>0){
           dbRecAdd(REPOut);
           dbFieldPutStr(REPOut, -1, "STRING", MESSWR         , DBWDISK);
           dbFieldPutStr(REPOut, -1, "FLG"   , LALR[nMess].flg, DBWDISK);
           dbFieldPutStr(REPOut, -1, "COL"   , MESSCOL        , DBWDISK);
           FVW_Add=1;
       }
   }  else  {
       sprintf(STAT[1].str,SMSG[175]);
       STAT[1].cnt=10;
   }
   return(0);
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
_i rMessageWrt(_i CURMSGNUM){
   MESSAGE_C=CURMSGNUM;
   // если пришло новое сообщение
   if (MESSAGE_C!=MESSAGE_P){
       // если номер сообщения правильный
       if (MESSAGE_C>0 && MESSAGE_C<HALR){
           if (strlen(LALR[(_i)(MESSAGE_C)].mes)>0){
               MESSAGE_P =MESSAGE_C; MESS[1].cnt=TMSS;
               sprintf(MESS[1].str,"%02d:%02d:%02d %s",
                       tmm->tm_hour,tmm->tm_min,tmm->tm_sec,
                       LALR[(_i)(MESSAGE_C)].mes);
               MESOut ((MOUT*)&MESS[1],4,0);
               sprintf(MESSWR,"%s",MS__sto);
               sprintf(MESSCOL,"~%03d^%03d",
                       LALR[(_i)(MESSAGE_C)].cs, LALR[(_i)(MESSAGE_C)].cg);
               // если открыт файл отчета - добавить новое сообщение
               if (RepFL>0){
                   if (dbOpenTest(REPOut,0)>0){
                       dbRecAdd(REPOut);
                       dbFieldPutStr(REPOut,-1,"STRING",MESSWR ,DBWDISK);
                       dbFieldPutStr(REPOut,-1,"FLG"   ,LALR[(_i)(MESSAGE_C)].flg,DBWDISK);
                       dbFieldPutStr(REPOut,-1,"COL"   ,MESSCOL,DBWDISK);
                       FVW_Add=1;
                   }
               }  else  {
                   sprintf(STAT[1].str,SMSG[176]);
                   STAT[1].cnt=10;
               }
           }
       // если неверный номер сообщения
       }   else   {
           MESSAGE_C=MESSAGE_P;
       }
   }
   return(0);
}
//---------------------------------------------------------------------------
// выводим сообщение из буфера, если оно там есть
vd rMessageOut(vd){
   _i  a=0;
   //  просмотреть массив каналов сообщений
   for(a=0;a<HMSGCH;a++){
      //  если номер текущего канала входит в допустимый диапазон
      if(MSGNUMCH[a]>0&&MSGNUMCH[a]<MCH){
         //  если номер сообщения для текущего канала не равен предыдущему
         //  номеру сообщения для этого канала
         if(CB[MSGNUMCH[a]].v!=CBNUMMSGP[a]){
//printf("WC%d ",(ui)CB[MSGNUMCH[a]].v);
            //  вызвать функцию обработки сообщения
            rMessageWrt(CB[MSGNUMCH[a]].v);
            //  запомнить номер последнего сообщения для текущего канала
            CBNUMMSGP[a]=CB[MSGNUMCH[a]].v;
         }
      }
   } 
   //  если есть сообщения программы
   if(CBNUMMSGP[MAXMCH]>0){
//printf("WP%d ",(ui)CBNUMMSGP[MAXMCH]);
      //  вызвать функцию обработки сообщения
      rMessageWrt(CBNUMMSGP[MAXMCH]); CBNUMMSGP[MAXMCH]=0;
   }
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
//  Вывод строки на экран
vd  STR_Out(uc *OStr,_i Fnt,_f X,_f Y,_f W,_i md){
    _i    b=0,  P=0,  n=0,  sl=0,  h=0;
    _f   xc=X, wc=0;
    _c   cc[2]={0,0}, ct[4]={0,0,0,0}, *ep;
    _c    c[9]={0,0,0,0,0,0,0,0,0};
    _c    f[7]={'%','-','9','.','0','f', 0 };

    sl=strlen(OStr);
    text_mode(STR_cg);

    if (md>0){
       for(b=0;b<sl;b++){
           //---------------------------------------------------------------
           // цвет шрифта
           if (OStr[b]==126&&
              (OStr[b+1]>47&&OStr[b+1]<58)&&(OStr[b+3]>47&&OStr[b+3]<58)){
               ct[0]=OStr[b+1];ct[1]=OStr[b+2];ct[2]=OStr[b+3];ct[3]=0;
               STR_cs=(us)(strtoul(ct,&ep,10)); 
               b=b+3;
               continue;
           }
           // цвет фона
           if (OStr[b]== 94&&
              (OStr[b+1]>42&&OStr[b+1]<58)&&(OStr[b+3]>47&&OStr[b+3]<58)){
               ct[0]=OStr[b+1];ct[1]=OStr[b+2];ct[2]=OStr[b+3];ct[3]=0;
               STR_cg=(us)(strtoul(ct,&ep,10)); b=b+3; 
               text_mode(STR_cg);
               continue;
           }
           // номер шрифта
           if (OStr[b]== 96&&
              (OStr[b+1]>47&&OStr[b+1]<58)&&(OStr[b+3]>47&&OStr[b+3]<58)){
               ct[0]=OStr[b+1];ct[1]=OStr[b+2];ct[2]=OStr[b+3];ct[3]=0;
               STR_sn=(us)(strtoul(ct,&ep,10)); b=b+3; 
               continue;
           }
           // координата X
           if((OStr[b]== 16||OStr[b]=='')&&
              (OStr[b+1]>47&&OStr[b+1]<58)&&(OStr[b+3]>47&&OStr[b+3]<58)){
               ct[0]=OStr[b+1];ct[1]=OStr[b+2];ct[2]=OStr[b+3];ct[3]=0;
               xc=(us)(strtoul(ct,&ep,10)); b=b+3; 
               continue;
           }
           // координата Y
           if((OStr[b]== 18)&&
              (OStr[b+1]>47&&OStr[b+1]<58)&&(OStr[b+3]>47&&OStr[b+3]<58)){
               ct[0]=OStr[b+1];ct[1]=OStr[b+2];ct[2]=OStr[b+3];ct[3]=0;
               Y=(us)(strtoul(ct,&ep,10)); b=b+3; 
               continue;
           }
           // корректировка координаты Y
           if((OStr[b]== 24||OStr[b]=='')&&
              (OStr[b+1]>42&&OStr[b+1]<58)&&(OStr[b+3]>47&&OStr[b+3]<58)){
               ct[0]=OStr[b+1];ct[1]=OStr[b+2];ct[2]=OStr[b+3];ct[3]=0;
               Y=Y+(_f)(strtoul(ct,&ep,10)); b=b+3;
               continue;
           }
           // корректировка координаты X
           if((OStr[b]== 27)&&
              (OStr[b+1]>42&&OStr[b+1]<58)&&(OStr[b+3]>47&&OStr[b+3]<58)){
               ct[0]=OStr[b+1];ct[1]=OStr[b+2];ct[2]=OStr[b+3];ct[3]=0;
               xc=xc+(_f)(strtoul(ct,&ep,10)); b=b+3; 
               continue;
           }
           //---------------------------------------------------------------
           // вывод рисунков
           if((OStr[b]==253)&&
              (OStr[b+1]>47&&OStr[b+1]<58)&&(OStr[b+3]>47&&OStr[b+3]<58)){
               ct[0]=OStr[b+1];ct[1]=OStr[b+2];ct[2]=OStr[b+3];ct[3]=0;
               P=(us)(strtoul(ct,&ep,10)); b=b+3; 
               MLoad_Spr(P,2,xc,Y);
               continue;
           }
           // вывод графических примитивов (определение длинны)
           if((OStr[b]== 29)&&
              (OStr[b+1]>47&&OStr[b+1]<58)&&(OStr[b+3]>47&&OStr[b+3]<58)){
               ct[0]=OStr[b+1];ct[1]=OStr[b+2];ct[2]=OStr[b+3];ct[3]=0;
               STR_bw=(us)(strtoul(ct,&ep,10)); b=b+3; 
               continue;
           }
           // вывод графических примитивов (определение высоты)
           if((OStr[b]== 30)&&
              (OStr[b+1]>47&&OStr[b+1]<58)&&(OStr[b+3]>47&&OStr[b+3]<58)){
               ct[0]=OStr[b+1];ct[1]=OStr[b+2];ct[2]=OStr[b+3];ct[3]=0;
               STR_bh=(us)(strtoul(ct,&ep,10)); b=b+3; 
               continue;
           }
           // вывод графических примитивов (определение и вывод примитива)
           if((OStr[b]== 31)&&
              (OStr[b+1]>47&&OStr[b+1]<58)&&(OStr[b+3]>47&&OStr[b+3]<58)){
               ct[0]=OStr[b+1];ct[1]=OStr[b+2];ct[2]=OStr[b+3];ct[3]=0;
               STR_nn=(us)(strtoul(ct,&ep,10)); b=b+3; 
               MDraw_Box(0,1,STR_nn,xc,Y,STR_bw,STR_bh,STR_cs);
               continue;
           }
           //---------------------------------------------------------------
           // вывод цифровых макросов
           if((OStr[b]=='@')&&
              (OStr[b+1]>47&&OStr[b+1]<58)&&(OStr[b+4]>47&&OStr[b+4]<58)){
               c[0]=OStr[b+1]; c[1]=OStr[b+2]; c[2]=OStr[b+3]; 
               c[3]=OStr[b+4]; c[4]=OStr[b+5]; c[5]=OStr[b+6]; 
               // получить номер переменной из строки
               n=atoi(c); if(n<0) n=0; if(n>=MYCM) n=MYCM-1;;
               // количество обработанных символов входной строки
               h=4;
               // шестая цифра в номере канала определяет количество
               // цифр после запятой (по умолчанию 0)
               if(c[4]=='.' && c[5]>=48 && c[5]<58){ f[4]=c[5]; h=6; }
               sprintf(LMVL[n],f,YCM[n]);
               b=b+h;
               // вывод полученного значения на экран
               wc=text_length(dFnt[STR_sn].dat,LMVL[n]);
               if(xc+wc>=X+W){ break; }
               if(CodusDirectScr>0){
                  textout(screen,dFnt[STR_sn].dat,LMVL[n],xc,Y,STR_cs);
               } else {
                  textout(scrsav,dFnt[STR_sn].dat,LMVL[n],xc,Y,STR_cs);
               }
               xc=xc+wc; if(xc>=X+W){ break; }
               continue;
           }
           //---------------------------------------------------------------
           // вывод всех остальных символов
           cc[0]=OStr[b]; cc[1]=0; wc=text_length(dFnt[STR_sn].dat,cc);
//         if(xc+wc>=X+W){ break; }
           if(xc+wc> X+W){ break; }
           if(CodusDirectScr>0){
              textout(screen,dFnt[STR_sn].dat,cc,xc,Y,STR_cs);
           } else {
              textout(scrsav,dFnt[STR_sn].dat,cc,xc,Y,STR_cs);
           }
           xc=xc+wc; if(xc>=X+W){ break; }
           //---------------------------------------------------------------
       }
    }  else  {
       for(b=sl;b>0;b--){
          wc=text_length(dFnt[Fnt].dat,OStr);
          if(wc<W) break;
          OStr[b]=0;
       }
       if(CodusDirectScr>0){
          textout(screen,dFnt[Fnt].dat,OStr,xc,Y,STR_cs);
       } else {
          textout(scrsav,dFnt[Fnt].dat,OStr,xc,Y,STR_cs);
       }
    }
    return;
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// Предварительная обработка выводимых строк
// Преобразование номера канала в строке в текущее значение канала
// MS  - указатель на структуру сообщений
// fl  - флаг выбора структуры:
// (0-статус, 1-сообщения, 3-строки, 4-обработка без вывода)
// nm  - номер сообщения в массиве
_i MESOut(MOUT * MS, _i fl, _i nm){
   _i  a=0, b=0, c=0, d=0, e=0, l=0, na=0, dc=2, nv=0, dd=0, df=0;
   uc  cc[9]={0,0,0,0,0,0,0,0,0};
   _i  ht=15;
   if (fl!=4) ht=text_height(dFnt[MS->sn].dat);
   if (fl!=4){
       if ((MS->sn==0)||(MS->sn>MFNT)){ return(0); }
       if ((fl==3)&&(nm>HSTRC)){ return(0); }
       STR_cg=(us)MS->cg;
       STR_cs=(us)MS->cs;
       STR_sn=(us)MS->sn;
   }
   // обнуляем строку назначения
   for(a=0;a<SLEN;a++){ MS__sto[a]=0; }
   // проверяем счетчики окончания отображения сообщений
   if (fl<3){
      if (fl== 0){ a=MTimer(3,'h',100); }
      if (fl== 1){ a=MTimer(4,'h',100); }
      // каждую секунду уменьшаем счетчик
      if (MS->cnt>=1){
         if(a==2){ MS->cnt--; }
      }  else {
      // если счетчика нет - выводим пустую строку
         MS->str[0]=0; MS->cnt=30; ErrCNT=0;
      }
   }
   //------------------------------------------------------------------------
   // преобразование номера канала в значение
   l=strlen(MS->str); if(l>=SLEN){ l=SLEN-1; }
   for(a=0,b=0;a<=l;a++){
      // если встретили знак номера канала
      if (MS->str[a]=='#'&&b==0){ 
          // если канал определен именем
          if (MS->str[a+1]>64){
              for(e=0;e<SLEN-1;e++){
                  // определяем вывод значения канала
                  if(MS->str[a+1+e]=='#'&&e>0){ 
                     AW_C[e]=0; b=5; break; 
                  }
                  // определяем вывод имени
                  if(MS->str[a+1+e]=='$'&&e>0){ 
                     AW_C[e]=0; b=6; break; 
                  }
                  // определяем вывод номера канала
                  if(MS->str[a+1+e]=='%'&&e>0){ 
                     AW_C[e]=0; b=7; break; 
                  }
                  // определяем вывод комментария канала
                  if(MS->str[a+1+e]=='&'&&e>0){ 
                     AW_C[e]=0; b=8; break; 
                  }
                  AW_C[e]=MS->str[a+1+e];
              }  
              na=0; a=a+e+1;
              // если не найден ограничительный символ имени канала
              if (b==0){ nv=0; b=0; continue; }
              // определяем номер канала в общей базе каналов
              for(e=0;e< MCH;e++){
                  if((strcmp(AW_C,CB[e].nm))==0){ nv=e; break; }
              }
              if (e>=MCH){ nv=0; b=0; };  continue;
          }
          b=1; d=0; continue; 
      }
      // прочитали номер массива
      if((MS->str[a]=='.')&&(b==1)){ 
          b=2; cc[d]=0; na=atoi(cc); d=0;
          continue; 
      }
      // прочитали номер канала
      if(((MS->str[a]<48||MS->str[a]>57) && (b==2)) || (d>4)||(MS->str[a]=='#')){
          cc[d]=0; nv=atoi(cc); d=0;
          b=(MS->str[a]== '.')? 3:5;
          if(b==3||MS->str[a]=='#'){ continue; }
      }
      // прочитали количество цифр после запятой
      if ((MS->str[a]==0 || MS->str[a]==' ') && b==3){
          cc[d]=0; dc=atoi(cc); d=0; b=5;
          if(dc>=10){ 
             df=1; dd=dc/10; dc=dc-dd*10; if(dd>6){ dd=6; }
          }
          continue;
      }
      // вывод имени канала из базы данных каналов
      if (b>0&&MS->str[a]=='$'&&nv>0){ b= 6; a++; }
      // если в конце номера переменной знак '&' -
      // вывести  коментарий к переменной из базы данных каналов
      if (b>0&&MS->str[a]=='&'&&nv>0){ b= 8; a++; }
      // вывод типа канала из базы данных каналов
      if (b>0&&MS->str[a]=='^'&&nv>0){ b= 9; a++; }
      // вывод номера контроллера из базы данных каналов
      if (b>0&&MS->str[a]=='p'&&nv>0){ b=10; a++; }
      // определить формат вывода переменной по данным из базы каналов
      if (b==5){
          // формируем формат вывода переменной
          if (dc>4){ dc=4; }
          if (df>0){ 
              sprintf(cc,"%d",dd); SFRM_M[2]=cc[0]; df=0;
          }
              sprintf(cc,"%d",dc); SFRM_M[4]=cc[0]; dc=2;
          // Общая база каналов
          if (na==0){ sprintf(cc,SFRM_M,CB[nv].v); }
          // Локальная база каналов
          if (na==1){ sprintf(cc,SFRM_M, CBL[nv]); }
          // База каналов драйвера COM портов
          if (na==2){ sprintf(cc,SFRM_M, CBS[nv]); }
          // База каналов драйвера NetBios
          if (na==3){ sprintf(cc,SFRM_M, CBN[nv]); }
          // База каналов драйвера TCP/IP
          if (na==4){ sprintf(cc,SFRM_M, CBT[nv]); }
          // Массив временных срезов трендов
          if (na==5){ sprintf(cc,SFRM_M,VTRT[nv]); }
          // Массив переменных комманды 'y'
          if (na==6){ sprintf(cc,SFRM_M, YCM[nv]); }
          // Массив временных переменных
          if (na==7){ sprintf(cc,SFRM_M,RTMP[nv]); }
          b=0; na=0; nv=0;
          strcat (MS__sto,cc); c=c+strlen(cc); cc[0]=0;
      }

      // '$' В качестве строки добавить имя канала (только общая база каналов)
      if (b==6){
          if(na==0){ strcat(MS__sto,CB[nv].nm); }
          c=c+strlen(CB[nv].nm); cc[0]=0; b=0; na=0; nv=0;
      }
      // '%' В качестве строки добавить номер канала
      if (b==7){
          sprintf(cc,"%d",nv); 
          if(na==0){ strcat(MS__sto,cc); }
          c=c+strlen(cc); cc[0]=0; b=0; na=0; nv=0;
      }
      // '&' В качестве строки добавить комментарий канала
      if(b==8){
          if(na==0){ strcat(MS__sto,CB[nv].rm); }
          c=c+strlen(CB[nv].rm); cc[0]=0; b=0; na=0; nv=0;
      }
      // '^' В качестве строки добавить тип переменной текущего канала
      if(b==9){
          if(na==0){ strcat(MS__sto,itoa(CB[nv].t,cc,10)); }
          c=c+strlen(itoa(CB[nv].t,cc,10)); cc[0]=0; b=0; na=0; nv=0;
      }
      // 'p' В качестве строки добавить номер контроллера текущего канала
      if(b==10){
          if(na==0){ strcat(MS__sto,itoa(CB[nv].dn,cc,10)); }
          c=c+strlen(itoa(CB[nv].dn,cc,10)); cc[0]=0; b=0; na=0; nv=0;
      }

      // формирование строки
      if (b>0){ 
          cc[d]=MS->str[a]; if(d<9){ d++; }
      } else  { 
          MS__sto[c]=MS->str[a]; if(c<SLEN-1){ c++; }
      }
   }
   if (fl==4){ strcpy(MS->str,MS__sto); return(0); }
   //------------------------------------------------------------------------
   // Вывод строки из массива строк
   if (fl==3){
      // если определена прокрутка
      if (SSTR[nm].st>0){
         if (SSTR[nm].spc>SSTR[nm].st){
             SSTR[nm].spc=0;
             SSTR[nm].lc=strlen(MS__sto);
             SSTR[nm].lp=text_length(dFnt[MS->sn].dat,MS__sto);
             STR_Out(MS__sto,MS->sn,MS->X,MS->Y,MS->lcs,1);
         }
         SSTR[nm].spc++;
      // если прокрутка не определена просто выводим сообщение
      }  else  {
         // Длинна строки в пикселах MS->lcs (до XMAX)
         STR_Out(MS__sto,MS->sn,MS->X,MS->Y,MS->lcs,1);
      }
      return(0);
   }
   //------------------------------------------------------------------------
   // счетчик перерисовки сообщений
   MESCnt[fl]++;
   if (MESCnt[fl]>5||CodusDirectScr>0){
       MESCnt[fl]=0;
      if (MS->cg>=0){
          if (CodusDirectScr>0){
              rectfill(screen,MS->X,MS->Y,MS->X+MS->lcs,MS->Y+ht,MS->cg);
          } else {
              rectfill(scrsav,MS->X,MS->Y,MS->X+MS->lcs,MS->Y+ht,MS->cg);
          }
      }
      STR_Out (MS__sto,MS->sn,MS->X,MS->Y,MS->lcs,1);
   }
   return(0);
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// вывод информации об авторе
vd MesAUTH(_i md, _i wa){
   _i a=0, b=0, c=0, ws=0;
   if (AUFL>0){
       //--------------------------------------------------------------------
       // вывод поочередной сменой строк
       if (md==1){
           if (AUFL%5==0){
               strcpy(STAT[1].str,MDEC[AUCNT]); STAT[1].cnt=7; AUCNT++;
               if (AUCNT>10){ AUCNT=2; }
           }
           AUFL++;
       }
       //--------------------------------------------------------------------
       // вывод в режиме "бегущая строка"
       if (md==2){
           //
           for(a=0, b=AUx, c=AUCNT; a<SLEN; ){
               if (MDEC[c][b]==10||MDEC[c][b]==0){
                   c++; b=0; if(c>9){ c=2; }; continue;
               }
               if (MDEC[c][b]< 32){ b++; continue; }
               astr[a]=MDEC[c][b]; astr[a+1]=0; a++; b++;
               ws=text_length(dFnt[STAT[1].sn].dat,astr);
               if (ws>=STAT[1].lcs-5){ break; }
           }
           sprintf(STAT[1].str,"%s ",astr); STAT[1].cnt=3;
           // выдержка паузы перед началом движения строки
           if (AUw<wa){ AUw++; }
           // иначе- переход к следующему символу
           else       { AUx++; AUw=0; }
           if (AUx>strlen(MDEC[AUCNT])){
               AUCNT++; if(AUCNT>9){ AUCNT=2; }; AUx=0;
           }
       }
       //--------------------------------------------------------------------
   }
   return;
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// контроль кодов программы контроллера
vd CheckCSPC(vd){
   _i a=0;
   if (AUTHNMCH[0]==0) return;
   for(a=0;a<HAUTHC;a++){
       if(AUTHNMCH[a]>0 && AUTHNMCH[a]<10){
          sprintf(STAT[1].str,"Программа PCCS закончит работу через %.0f дней",AUTHCODE);
          STAT[1].cnt=10;
       }
       if(AUTHNMCH[a]==15){
          sprintf(STAT[1].str,"Ошибка установки авторизации PCCS. Выключить PCCS!");
          STAT[1].cnt=10; break;
       }
       if(AUTHNMCH[a]==19){
          sprintf(STAT[1].str,"Срок авторизации программы PCCS закончен. Выключить PCCS!");
          STAT[1].cnt=10; break;
       }
   }
   return;
}
//---------------------------------------------------------------------------

