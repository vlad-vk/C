// coding: cp866
//---------------------------------------------------------------------------
// �뢮� ����� �� ����� ���祭��
_i vkGetCursor(_f X, _f Y, _i sn, _i cr, _i cg){
   _i a=0;
   _c cc[2]={0,0};
   _f hs=0, ws=0, cx=0;
   // ��⠭����� 䫠� �맮�� �����
   GetDATA=1;
   // 
   if (GKey==KEY_ESC || GKey==KEY_ENTER || GKey==KEY_ENTER_PAD){ LGET_rn=1; }
   // ��⠥� ᪮�쪮 ���ᥫ�� ࠢ�� ���� � �ਭ� ������ ᨬ���� 䮭�
   hs=text_height(dFnt[sn].dat);
   ws=text_length(dFnt[sn].dat,"0");
   // ��⠥� �ਭ� ᨬ����� � ���न���� ��� �����
   if (GS.fe>0){
       for(a=0;a<LGET_el;a++){
           cc[0]=GS.ES[a];
           cx=cx+text_length(dFnt[sn].dat,cc);
       }
   } else {
       cx=LGET_el*ws;
   }
   // ���ᮢ��� �����
   if (LGET_rn==0){
       rectfill(scrsav, X+cx, Y+hs-0.5, X+cx+ws, Y+hs-1.5, cr);
       sprintf (STAT[1].str,SMSG[132]);
                STAT[1].cnt=2;
   }
   return(0);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ����� ��ப � ���祭�� �� 蠡����
_i GetStr(_i md ){
   // ⥪�騩 ᨬ��� �����
   _i GSRet=0, a=0, l=0, wc=0, ws=0;
   // ���� ᨬ�����
   _f hs=0;
   // ����� 䫠� �맮�� �����
   GetDATA=0;
   // �㭪�� �맢��� � ०��� �����
   if (md==1 && GS.FL==1){
      CURGETDAT=1;
      LGET_en=1;
      if (GKey>0){
         LGET_eo=LGET_el;
         wc=text_length(dFnt[GS.sn].dat, "�" );
         ws=text_length(dFnt[GS.sn].dat,GS.ES); l=strlen(GS.ES);
         if(GKey>=KEY_0_PAD&&GKey<=KEY_9_PAD){
            GKey =GKey-10;   ACode=GKey+21;
         }
         if( GS.sf[LGET_el]=='0' &&
           ((GKey>=KEY_0&&GKey<=KEY_9)||GKey==KEY_MINUS||GKey==KEY_EQUALS)){
             GS.ES[LGET_el]=ACode; if(LGET_el<GS.LS-1){ LGET_el++; }
         }
         // �����뢠�� ASCII ��� ������
         if (GS.sf[LGET_el]=='#' && ACode>31 && ACode<250){
             // �������� ᨬ��� � ��ப�
             GS.ES[LGET_el]=ACode;
             // �᫨ ����祭 ���᪨� ��
             if (RUS_ENG==0){
                 for(a=0;a<MRUC;a++){
                     if (ACode==RUN[a]){ break; }
                 }
                 // ����� ᨬ��� �� ⠡���� ���᪨� ᨬ�����
                 if (a<MRUC){ GS.ES[LGET_el]=RUC[a]; }
             }
             // �஢���� ������ ��ப� �� ���ᥫ��
             if(ws+2*wc<GS.W||LGET_el<l-1){
                if(LGET_el<GS.LS-1){ LGET_el++; }
             }
         }
         // �᫨ �뫠 ����� ��५�� �����  (<--)
         if (GKey==KEY_LEFT){
             if (LGET_el>0){ LGET_el--; }
         }
         // �᫨ �뫠 ����� ��५�� ��ࠢ� (-->)
         if (GKey==KEY_RIGHT && GS.ES[LGET_el]>0){
             if((LGET_el<GS.LS-1)&&(LGET_el<l-1)){ LGET_el++; }
         }
         // �᫨ �뫠 ����� ������ Home
         if (GKey==KEY_HOME){ LGET_el=0; }
         // �᫨ �뫠 ����� ������ End
         if (GKey==KEY_END){ 
             if(l>0){ LGET_el=l-1; }
         }
         // �᫨ �뫠 ����� ������ BackSpace
         if (GKey==KEY_BACKSPACE && GS.fe>=4){
             if (LGET_el>0){ LGET_el--; }
             for(a=LGET_el;a<strlen(GS.ES);a++){ GS.ES[a]=GS.ES[a+1]; }
             GS.ES[a]=0;
         }
         // �᫨ �뫠 ����� ������ Del
         if (GKey==KEY_DEL && GS.fe>=4){
             for(a=LGET_el;a<strlen(GS.ES);a++){ GS.ES[a]=GS.ES[a+1]; }
             GS.ES[a]=0;
         }
      }
      // �ய����� �㦥��� ᨬ����
      if (GS.sf[LGET_el]=='.' || GS.sf[LGET_el]==',' ||
          GS.sf[LGET_el]==':' || GS.sf[LGET_el]==';' || GS.sf[LGET_el]=='*' ||
          GS.sf[LGET_el]=='-' || GS.sf[LGET_el]=='/' || GS.sf[LGET_el]=='+' )
      {
          GS.ES[LGET_el]=GS.sf[LGET_el];
          if(GKey==KEY_LEFT && LGET_el>0){ LGET_el--; }
          if(GKey!=KEY_LEFT && LGET_el<GS.LS){ LGET_el++; }
      }
      // ��⠥� ᪮�쪮 ���ᥫ�� ࠢ�� ���� 䮭�
      hs=text_height(dFnt[GS.sn].dat);
      // ���ᮢ뢠�� ��אַ㣮�쭨� �����
      rectfill(scrsav, GS.RX1, GS.RY1, GS.RX2, GS.RY2, GS.cg );
      // �뢮� ��ப� �����
      text_mode(GS.cg);
      textout(scrsav, dFnt[GS.sn].dat, GS.ES, GS.TX, GS.TY, GS.cs);
      // �뢮� �����
      vkGetCursor(GS.TX, GS.TY, GS.sn, GS.cc, GS.cg);
      // �������� ���� �� (ESC ��� Enter)
      if (GKey==KEY_ESC || GKey==KEY_ENTER || GKey==KEY_ENTER_PAD){
          GS.FL=0; LGET_en=0; CURGETDAT=0;
      }
      GSRet=GKey; GKey=0;
   }
   return(GSRet);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ���� ��ப ������
_i GetVal(_i en, _i md){
   //
   _i  a=0, b=0, rc=0, sl=0, fc=0, l=0;
   _f hs=0;
   // �᫨ ���� ����饭 - ���
   if (DisG>0){ return(KEY_ESC); }
   // ����� 䫠� �맮�� �����
   GetDATA=0;

   // �᫨ ⥪�騩 ����� ����� ���� �����⨬��� ��� ������� �࠭�
   if (en>0 && en<HGET && LGET[en].nv>0){

      //---------------------------------------------------------------------
      // �᫨ 䫠� ����� �� ��⠭����� - ��⠭����� 䫠� �����
      if((LGET_en==0)&&(md==0)){
          LGET_en=1; LGET[en].vw=1; LGET_st=en; LGET_el=0; fc=1;
          // ���� ���� �������
          if (LGET[en].na==0){ sprintf(VGET[en].pr,LGET[en].sf,CB[LGET[en].nv].v); }
          // �����쭠� ���� �������
          if (LGET[en].na==1){ sprintf(VGET[en].pr,LGET[en].sf,CBL[LGET[en].nv]);  }
          // ���� ������� �ࠩ��� COM ���⮢
          if (LGET[en].na==2){ sprintf(VGET[en].pr,LGET[en].sf,CBS[LGET[en].nv]);  }
          // ���� ������� �ࠩ��� NetBios
          if (LGET[en].na==3){ sprintf(VGET[en].pr,LGET[en].sf,CBN[LGET[en].nv]);  }
          // ���� ������� �ࠩ��� TCP/IP
          if (LGET[en].na==4){ sprintf(VGET[en].pr,LGET[en].sf,CBT[LGET[en].nv]);  }
          // ���ᨢ ��६����� �������� 'y'
          if (LGET[en].na==6){ sprintf(VGET[en].pr,LGET[en].sf,YCM[LGET[en].nv]);  }
          // ���ᨢ �६����� ��६�����
          if (LGET[en].na==7){ sprintf(VGET[en].pr,LGET[en].sf,RTMP[LGET[en].nv]); }
      }
      //---------------------------------------------------------------------


      //---------------------------------------------------------------------
      // �᫨ ��⠭����� 䫠� �����
      if (LGET[en].vw==1){

         CURGETVAL=en;
         //------------------------------------------
         // �஢��塞 ���� �� ������ ������ � ⠡��� ࠧ�襭��� ᨬ�����
         for(a=0;a<MSYM;a++){
             if (GKey==LGC[a]){ break; }
         }
         // ���� "�����" ��� "����" �뢮����� ⮫쪮 � ����樨 LGET_el=0
         // 13,14,25,26 - ����� � ���ᨢ� LGS[MSYM]
         if ((a==13 || a==14 || a==25 || a==26 ) && LGET_el>0){ a=MSYM; }
         // �᫨ ������ ������ � ⠡��� ���������
         if (a<MSYM){
            // �����뢠�� � ।����㥬�� ��ப� ᨬ��� ᮮ⢥��⢨� ������
            VGET[en].pr[LGET_el]=LGS[a];
            // 㢥��稢��� ������ ⥪�饣� ᨬ����
            if (LGET_el<LGET[en].hc-1){ LGET_el++; }
         }
         // �᫨ �뫠 ����� ��५�� �����  (<--)
         if (GKey==KEY_LEFT ){ if (LGET_el>0) LGET_el--;; }
         // �᫨ �뫠 ����� ��५�� ���। (-->)
         if (GKey==KEY_RIGHT){ if (LGET_el<LGET[en].hc-1) LGET_el++;; }
         // �᫨ �뫠 ����� ������ BackSpace
         if (GKey==KEY_BACKSPACE){
            if (LGET_el>0){ LGET_el--; }
            l=strlen(VGET[en].pr);
            for(a=LGET_el; a<l; a++){ VGET[en].pr[a]=VGET[en].pr[a+1]; }
            VGET[en].pr[a]=0;
         }
         // �᫨ �뫠 ����� ������ Del
         if (GKey==KEY_DEL){
            l=strlen(VGET[en].pr);
            for(a=LGET_el; a<l; a++){ VGET[en].pr[a]=VGET[en].pr[a+1]; }
            for(b=a-1;  b<GLEN; b++){ VGET[en].pr[b]=0; }
         }
         //------------------------------------------

         //------------------------------------------
         // ��⠥� ᪮�쪮 ���ᥫ�� ࠢ�� ���� 䮭�
         hs=text_height(dFnt[LGET[en].sn].dat);
         //------------------------------------------

         //------------------------------------------
         // �� �� ����⨨ ������ �����塞 �뢮� �� �࠭
         if (fc>0 || GKey>0){
            // � ����� ��ப� ������塞 �� ������ ᨬ����, ���� �� �멤�� ��
            // �����⨬�� ������⢮ ᨬ����� ��� ࠧ��� ��ப� � ���ᥫ��
            for(a=LGET_el;a<LGET[en].hc;a++){
               sl =strlen(VGET[en].pr);
               if (sl>=LGET[en].hc){ break; }
               VGET[en].pr[sl+1]= 0 ;
               if((text_length(dFnt[LGET[en].sn].dat, VGET[en].pr))>=(_i)(LGET[en].W)){
                  break;
               }
            }
         }
         // ��࠭���� ��ப� �뢮��
         sl=strlen(VGET[en].pr);
         for(a=LGET[en].hc;a>0;a--){
             if(text_length(dFnt[LGET[en].sn].dat, VGET[en].pr)>(_i)(LGET[en].W)){
                 VGET[en].pr[a]=0;
         }   }
        // ���ᮢ뢠�� ��אַ㣮�쭨� �����
        rectfill(scrsav, LGET[en].X,
                 LGET[en].Y,
                 LGET[en].X+ LGET[en].W,
                 LGET[en].Y+ hs,
                 LGET[en].cg );
         text_mode(LGET[en].cg);
         // �뢮� ��ப� �����
         textout(scrsav,dFnt[LGET[en].sn].dat,VGET[en].pr,
                 LGET[en].X,LGET[en].Y,LGET[en].cs);

         //------------------------------------------
         vkGetCursor(LGET[en].X,LGET[en].Y,LGET[en].sn,LGET[en].cr,LGET[en].cg);
      }
      //---------------------------------------------------------------------


      //---------------------------------------------------------------------
      if (GKey>0){
         SCode=0;
         //  �⪠� �� ����� (ESC)
         if (GKey==KEY_ESC){
            LGET_en=0; LGET[en].vw=0; rc=KEY_ESC;
            // �᫨ ����� ���� - ���뫠�� ����⨥ ESC, �⮡� ��� �������
            SCode=(LockKBD!=100)?KEY_ESC:0;
            CURGETVAL=0;
         }

         // �᫨ �� ࠧ�襭 ���� ��ப� � ����� ������ ����砭��
         // ����� (ENTER)
         if (GKey==KEY_ENTER || GKey==KEY_ENTER_PAD){
            LGET_en=0; LGET[en].vw=0; rc=KEY_ESC;
            // �᫨ ����� ���� - ���뫠�� ����⨥ ESC, �⮡� ��� �������
            SCode=(LockKBD!=100)?KEY_ESC:0;
            // ���� ���� �������
            if (LGET[en].na==0){
                CB[LGET[en].nv].v=atof(VGET[en].pr);
                if ( CB[LGET[en].nv].v > CB[LGET[en].nv].h  )
                   { CB[LGET[en].nv].v = CB[LGET[en].nv].h; }
                if ( CB[LGET[en].nv].v < CB[LGET[en].nv].l  )
                   { CB[LGET[en].nv].v = CB[LGET[en].nv].l; }
                FLNBW=0; FLSER=0;
                if ( (_i)fmod(CB[LGET[en].nv].dt, 10)>  0){ FLSER=1; }
                if ( (_i)fmod(CB[LGET[en].nv].dt,100)>=10){ FLNBW=1; }
                if ( FLNBW>0 ){
                     FNBS[ CBDN[LGET[en].nv] ] = 2;
                     CBN [ CBDN[LGET[en].nv] ] = CB[LGET[en].nv].v;
                }
                if ( FLSER>0 ){
                     CSSER=LGET[en].nv;
                     FSRS[ CBSR[CSSER] ] = 2;
                     CBS [ CBSR[CSSER] ] = CB[CSSER].v;
                }
            }
            // �����쭠� ���� �������
            if (LGET[en].na==1){ 
                CBL[LGET[en].nv]=atof(VGET[en].pr);
            }
            // ���� ������� �ࠩ��� COM ���⮢
            if (LGET[en].na==2){
                CBS [LGET[en].nv]=atof(VGET[en].pr);
                FSRS[LGET[en].nv]=2;
            }
            // ���� ������� �ࠩ��� NetBios
            if (LGET[en].na==3){
                CBN [LGET[en].nv]=atof(VGET[en].pr);
                FNBS[LGET[en].nv]=2;
            }
            // ���� ������� �ࠩ��� TCP/IP
            if (LGET[en].na==4){ 
                CBT [LGET[en].nv]=atof(VGET[en].pr);
            }
            // ���ᨢ ��६����� �������� 'y'
            if (LGET[en].na==6){ 
                YCM [LGET[en].nv]=atof(VGET[en].pr); 
                strcpy(LMVL[LGET[en].nv],VGET[en].pr); 
            }
            // ���ᨢ �६����� ��६�����
            if (LGET[en].na==7){ 
                RTMP[LGET[en].nv]=atof(VGET[en].pr);
            }
            CURGETVAL=0;
         }
         GKey=0;
      }
      //---------------------------------------------------------------------
   }
   return(rc);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ���� ࠡ�祩 ��ப�
_i GetWStr(_i nStr, _i md){
   _i  a=0, hsn=0;
   if(nStr>=HSTT){ return(0); }
   //------------------------------------------------------------------------
   //  ���������� �������� ��� ����� ��ப�
   if (md==0){
       CURGETSTR=nStr;
       if(LSTT[nStr].sn<0||LSTT[nStr].sn> MFNT){ LSTT[nStr].sn=SFNT; }
       if(LSTT[nStr].LS<1||LSTT[nStr].LS>=SLEN){ LSTT[nStr].LS=20;   }
       hsn=text_height(dFnt[LSTT[nStr].sn].dat);
       memset(GS.ES,0,SLEN);
       strcpy(GS.ES,LSTT[nStr].ES);              // ��ப� ��� �����
       GS.LS =LSTT[nStr].LS;                     // ����� ᨬ� � ��ப�
       GS.W  =LSTT[nStr].W;                      // ������ ��ப� � ���ᥫ��

       GS.RX1=LSTT[nStr].X;                      // ���न���� ��� ���ᮢ��
       GS.RY1=LSTT[nStr].Y;
       GS.RX2=LSTT[nStr].X+LSTT[nStr].W;
       GS.RY2=LSTT[nStr].Y+hsn;

       GS.TX =LSTT[nStr].X;                      // ���न���� �����
       GS.TY =LSTT[nStr].Y;

       GS.sn =LSTT[nStr].sn;                     // ��ࠬ���� ����
       GS.cs =LSTT[nStr].cs;
       GS.cg =LSTT[nStr].cg;
       GS.cc =LSTT[nStr].cc;
                                                 // ��ப� �ଠ� ��� �����
       for(a=0;a<LSTT[nStr].LS;a++){ 
           GS.sf[a]='#'; 
       };  GS.sf[a]= 0;
       LGET_el=0;                                // ��� ᨬ��� ����� � ��ப�
       GetStr(0);                                // �맮� �㭪樨 ����� ��ப�
       GS.FL =1;                                 // 䫠� ��砫� �����
       GS.fe =7;                                 // 䫠� ����� ࠡ��� ��ப
       CSTT=nStr;                                // ⥪�騩 ����� ��������
   }
   //------------------------------------------------------------------------
   //  ��।�� ��������� ��ப� � �������� ࠡ��� ��ப
   if (md==1){
       // ��।��� ��������� ��ப� � �������� 
       strcpy(LSTT[CSTT].ES,GS.ES); memset(GS.ES,0,SLEN);
       // ������㥬 ��� ᫥���饩 ������
       GVCode=LSTT[CSTT].kb;
       // �᫨ �� ���� ��ப� ���᪠- �������� �⮡ࠦ���� 䠩��
       for(a=0;a<HFVW;a++){
          if(LFVW[a].ws==CSTT){ FVCode=1; ViewFile(FVA,1); break; }
       }
       // ����� 䫠� ����� ��ப�
       GS.fe=0;
       // ���㫨�� ����� ��ப� �����
       CURGETSTR=0;
   }                                             
   return(0);
}
//---------------------------------------------------------------------------
