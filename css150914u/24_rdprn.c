// coding: cp866
//---------------------------------------------------------------------------
//  чтение определений из файла описания принтера

//---------------------------------------------------------------------------
//  обработка строки (проверка всключения HEX-значений символов)
_i  strCheck(vd){
    _i  r=0, a=0, v=0, l=strlen(stmp);
    uc  s[3]={0,0,0};
    for(r=0; r<l; r++){
       if(stmp[r]=='#'){
          s[0]=stmp[r+1];  s[1]=stmp[r+2]; v=(_i)strtol(s,NULL,16); r=r+2;
          if(v>=0&&v<256){ stmp[a]=(uc) v; a++; }
       } else {
          stmp[a]=stmp[r]; a++;
       }
    }; stmp[a]=0;
    return(0);
}
//---------------------------------------------------------------------------
//  обработка файла описаний
_i  FillPRN(_i HBR){
    _i  a=0, b=0, f=0, n=0;
    uc  c=0;
    sprintf(SMSG[MMSG],SMSG[72]);
    DPrn(DEBPRN,0,"\n%s\n",SMSG[MMSG]);
    for(GStrPos=0;;){
       // взять каждую следующую строку из буфера
       if(( _mfGetStr(FBuf,HBR))==0){ break; }
       if ( strlen(GStr)<4 ){ continue; }
       //----------------------------------------------------------------------
       // заполнить поля из строки
       c=PutFields(0,GStr,' ',0); f=0;
       // обработать все поля
       for(a=0,n=0;a<HowFLD;a++){
           // если идет обработка строковой переменной
           if(f==1&&n>0){ c=PutFields(0,GStr,34,39,0); a=1; f=2; }
           // если встретилось заполненное поле
           if(isFieldb(0,a,0)>0){
              // получить текущее поле в переменную stmp
              strcpy(stmp,GetField(0,a)); b=0;
              // проверяем первый символ текущего поля:
              //---------------------------------------------------------------
              // если это комментарий
              if (stmp[0]==';'){ break; }
              // если это начало строковой переменной
              if((stmp[0]==39||stmp[0]==34)&&(f==0)){ f=1; continue; }
              // если включена отладка
              DPrn(DEBPRN,0,"[%s]",stmp); b=1;
              //---------------------------------------------------------------
              if (strcasecmp(stmp,"INIT")==0){ n=1; continue; }
              if (n==1){ strcpy(PRN_Init,stmp); }
              //---------------------------------------------------------------
              if (strcasecmp(stmp,"ORIENTATION")==0){ n=2; continue; }
              if (n==2){ strcpy(PRN_Ornt,stmp); }
              //---------------------------------------------------------------
              if (strcasecmp(stmp,"PITCH")==0){ n=3; continue; }
              if (n==3){ strcpy(PRN_Pitc,stmp); }
              //---------------------------------------------------------------
              if (strcasecmp(stmp,"CHARH")==0){ n=4; continue; }
              if (n==4){ strcpy(PRN_ChrH,stmp); }
              //---------------------------------------------------------------
              if (strcasecmp(stmp,"CHARB")==0){ n=5; continue; }
              if (n==5){ strcpy(PRN_ChrB,stmp); }
              //---------------------------------------------------------------
              if (strcasecmp(stmp,"LINESPACE")==0){ n=6; continue; }
              if (n==6){ strcpy(PRN_LnSp,stmp); }
              //---------------------------------------------------------------
              if (strcasecmp(stmp,"SPACING" )==0){ n=7; continue; }
              if (n==7){ strcpy(PRN_Spac,stmp); }
              //---------------------------------------------------------------
              if (strcasecmp(stmp,"PRINTSTR")==0){ n=8; continue; }
              if (n==8){ strcpy(PRN_Strn,stmp); }
              //---------------------------------------------------------------
              if (strcasecmp(stmp,"ENDSTRING")==0){ n=9; continue; }
              if (n==9){ strCheck(); strcpy(PRN_Ends,stmp); }
              //---------------------------------------------------------------

              //---------------------------------------------------------------
              if (strcasecmp(stmp,"NAME")==0){ n=10; continue; }
              if (n==10){ strcpy(PRN_Name,stmp); printf("PRN_Name[%s] ",PRN_Name); }
              //---------------------------------------------------------------
              if (strcasecmp(stmp,"FONTNAME")==0){ n=11; continue; }
              if (n==11){ strcpy(PRN_Font,stmp); printf("PRN_Font[%s] ",PRN_Font); }
              //---------------------------------------------------------------
              if (strcasecmp(stmp,"CHARSET")==0){ n=12; continue; }
              if (n==12){ PRN_Fchr=atoi(stmp); printf("PRN_Fchr[%d] ",PRN_Fchr); }
              //---------------------------------------------------------------
              if (strcasecmp(stmp,"FONTSIZE")==0){ n=13; continue; }
              if (n==13){ PRN_Fsiz=atoi(stmp); printf("PRN_Fsiz[%d] ",PRN_Fsiz); }
              //---------------------------------------------------------------
              if (strcasecmp(stmp,"FONTITAL")==0){ n=14; continue; }
              if (n==14){ PRN_Fitl=atoi(stmp); printf("PRN_Fitl[%d] ",PRN_Fitl); }
              //---------------------------------------------------------------
              if (strcasecmp(stmp,"FONTUNDR")==0){ n=15; continue; }
              if (n==15){ PRN_Fund=atoi(stmp); printf("PRN_Fund[%d] ",PRN_Fund); }
              //---------------------------------------------------------------
              if (strcasecmp(stmp,"FONTSOUT")==0){ n=16; continue; }
              if (n==16){ PRN_Fsou=atoi(stmp); printf("PRN_Fsou[%d] ",PRN_Fsou); }
              //---------------------------------------------------------------
              if (strcasecmp(stmp,"FONTORNT")==0){ n=17; continue; }
              if (n==17){ PRN_Forn=atoi(stmp); printf("PRN_Forn[%d] ",PRN_Forn); }
              //---------------------------------------------------------------
              if (strcasecmp(stmp,"FONTQUAL")==0){ n=18; continue; }
              if (n==18){ PRN_Fqua=atoi(stmp); printf("PRN_Fqua[%d] ",PRN_Fqua); }
              //---------------------------------------------------------------
              if (strcasecmp(stmp,"FONTBOLD")==0){ n=19; continue; }
              if (n==19){ PRN_Fbls=atoi(stmp); printf("PRN_Fbls[%d] ",PRN_Fbls); }
              //---------------------------------------------------------------
              if (strcasecmp(stmp,"OFFSETX" )==0){ n=20; continue; }
              if (n==20){ PRN_offX=atoi(stmp); printf("PRN_offX[%d] ",PRN_offX); }
              //---------------------------------------------------------------
              if (strcasecmp(stmp,"OFFSETY" )==0){ n=21; continue; }
              if (n==21){ PRN_offY=atoi(stmp); printf("PRN_offY[%d] ",PRN_offY); }
              //---------------------------------------------------------------
           }
           if(b>0&&n>0){ DPrn(DEBPRN,0,"\n"); }
           if(f==2) break;
       }
       //----------------------------------------------------------------------
    }
    DPrn(DEBPRN,0,"\n");
    return(1);
}
//----------------------------------------------------------------------------
