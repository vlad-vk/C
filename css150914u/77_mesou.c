// coding: cp866
//---------------------------------------------------------------------------
// ������ �뢮� ��ப ����
vd MenuStrOUT(vd){
   _i  b=0;
   for(b=0; b<HSTRC; b++) {
       if (vSTR[b]==5){ MESOut((MOUT*)&LSTR[b],3,b); }
   }
   return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// �뢮� ��⥬��� ᮮ�饭��
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
// ������� ᮮ�饭��, ���஥ ������� ������, � 䠩� ᮮ�饭�� 
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
   // �᫨ ����� 䠩� ���� - �������� ����� ᮮ�饭��
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
   // �᫨ ��諮 ����� ᮮ�饭��
   if (MESSAGE_C!=MESSAGE_P){
       // �᫨ ����� ᮮ�饭�� �ࠢ����
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
               // �᫨ ����� 䠩� ���� - �������� ����� ᮮ�饭��
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
       // �᫨ ������ ����� ᮮ�饭��
       }   else   {
           MESSAGE_C=MESSAGE_P;
       }
   }
   return(0);
}
//---------------------------------------------------------------------------
// �뢮��� ᮮ�饭�� �� ����, �᫨ ��� ⠬ ����
vd rMessageOut(vd){
   _i  a=0;
   //  ��ᬮ���� ���ᨢ ������� ᮮ�饭��
   for(a=0;a<HMSGCH;a++){
      //  �᫨ ����� ⥪�饣� ������ �室�� � �����⨬� ��������
      if(MSGNUMCH[a]>0&&MSGNUMCH[a]<MCH){
         //  �᫨ ����� ᮮ�饭�� ��� ⥪�饣� ������ �� ࠢ�� �।��饬�
         //  ������ ᮮ�饭�� ��� �⮣� ������
         if(CB[MSGNUMCH[a]].v!=CBNUMMSGP[a]){
//printf("WC%d ",(ui)CB[MSGNUMCH[a]].v);
            //  �맢��� �㭪�� ��ࠡ�⪨ ᮮ�饭��
            rMessageWrt(CB[MSGNUMCH[a]].v);
            //  ��������� ����� ��᫥����� ᮮ�饭�� ��� ⥪�饣� ������
            CBNUMMSGP[a]=CB[MSGNUMCH[a]].v;
         }
      }
   } 
   //  �᫨ ���� ᮮ�饭�� �ணࠬ��
   if(CBNUMMSGP[MAXMCH]>0){
//printf("WP%d ",(ui)CBNUMMSGP[MAXMCH]);
      //  �맢��� �㭪�� ��ࠡ�⪨ ᮮ�饭��
      rMessageWrt(CBNUMMSGP[MAXMCH]); CBNUMMSGP[MAXMCH]=0;
   }
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
//  �뢮� ��ப� �� �࠭
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
           // 梥� ����
           if (OStr[b]==126&&
              (OStr[b+1]>47&&OStr[b+1]<58)&&(OStr[b+3]>47&&OStr[b+3]<58)){
               ct[0]=OStr[b+1];ct[1]=OStr[b+2];ct[2]=OStr[b+3];ct[3]=0;
               STR_cs=(us)(strtoul(ct,&ep,10)); 
               b=b+3;
               continue;
           }
           // 梥� 䮭�
           if (OStr[b]== 94&&
              (OStr[b+1]>42&&OStr[b+1]<58)&&(OStr[b+3]>47&&OStr[b+3]<58)){
               ct[0]=OStr[b+1];ct[1]=OStr[b+2];ct[2]=OStr[b+3];ct[3]=0;
               STR_cg=(us)(strtoul(ct,&ep,10)); b=b+3; 
               text_mode(STR_cg);
               continue;
           }
           // ����� ����
           if (OStr[b]== 96&&
              (OStr[b+1]>47&&OStr[b+1]<58)&&(OStr[b+3]>47&&OStr[b+3]<58)){
               ct[0]=OStr[b+1];ct[1]=OStr[b+2];ct[2]=OStr[b+3];ct[3]=0;
               STR_sn=(us)(strtoul(ct,&ep,10)); b=b+3; 
               continue;
           }
           // ���न��� X
           if((OStr[b]== 16||OStr[b]=='')&&
              (OStr[b+1]>47&&OStr[b+1]<58)&&(OStr[b+3]>47&&OStr[b+3]<58)){
               ct[0]=OStr[b+1];ct[1]=OStr[b+2];ct[2]=OStr[b+3];ct[3]=0;
               xc=(us)(strtoul(ct,&ep,10)); b=b+3; 
               continue;
           }
           // ���न��� Y
           if((OStr[b]== 18)&&
              (OStr[b+1]>47&&OStr[b+1]<58)&&(OStr[b+3]>47&&OStr[b+3]<58)){
               ct[0]=OStr[b+1];ct[1]=OStr[b+2];ct[2]=OStr[b+3];ct[3]=0;
               Y=(us)(strtoul(ct,&ep,10)); b=b+3; 
               continue;
           }
           // ���४�஢�� ���न���� Y
           if((OStr[b]== 24||OStr[b]=='')&&
              (OStr[b+1]>42&&OStr[b+1]<58)&&(OStr[b+3]>47&&OStr[b+3]<58)){
               ct[0]=OStr[b+1];ct[1]=OStr[b+2];ct[2]=OStr[b+3];ct[3]=0;
               Y=Y+(_f)(strtoul(ct,&ep,10)); b=b+3;
               continue;
           }
           // ���४�஢�� ���न���� X
           if((OStr[b]== 27)&&
              (OStr[b+1]>42&&OStr[b+1]<58)&&(OStr[b+3]>47&&OStr[b+3]<58)){
               ct[0]=OStr[b+1];ct[1]=OStr[b+2];ct[2]=OStr[b+3];ct[3]=0;
               xc=xc+(_f)(strtoul(ct,&ep,10)); b=b+3; 
               continue;
           }
           //---------------------------------------------------------------
           // �뢮� ��㭪��
           if((OStr[b]==253)&&
              (OStr[b+1]>47&&OStr[b+1]<58)&&(OStr[b+3]>47&&OStr[b+3]<58)){
               ct[0]=OStr[b+1];ct[1]=OStr[b+2];ct[2]=OStr[b+3];ct[3]=0;
               P=(us)(strtoul(ct,&ep,10)); b=b+3; 
               MLoad_Spr(P,2,xc,Y);
               continue;
           }
           // �뢮� ����᪨� �ਬ�⨢�� (��।������ ������)
           if((OStr[b]== 29)&&
              (OStr[b+1]>47&&OStr[b+1]<58)&&(OStr[b+3]>47&&OStr[b+3]<58)){
               ct[0]=OStr[b+1];ct[1]=OStr[b+2];ct[2]=OStr[b+3];ct[3]=0;
               STR_bw=(us)(strtoul(ct,&ep,10)); b=b+3; 
               continue;
           }
           // �뢮� ����᪨� �ਬ�⨢�� (��।������ �����)
           if((OStr[b]== 30)&&
              (OStr[b+1]>47&&OStr[b+1]<58)&&(OStr[b+3]>47&&OStr[b+3]<58)){
               ct[0]=OStr[b+1];ct[1]=OStr[b+2];ct[2]=OStr[b+3];ct[3]=0;
               STR_bh=(us)(strtoul(ct,&ep,10)); b=b+3; 
               continue;
           }
           // �뢮� ����᪨� �ਬ�⨢�� (��।������ � �뢮� �ਬ�⨢�)
           if((OStr[b]== 31)&&
              (OStr[b+1]>47&&OStr[b+1]<58)&&(OStr[b+3]>47&&OStr[b+3]<58)){
               ct[0]=OStr[b+1];ct[1]=OStr[b+2];ct[2]=OStr[b+3];ct[3]=0;
               STR_nn=(us)(strtoul(ct,&ep,10)); b=b+3; 
               MDraw_Box(0,1,STR_nn,xc,Y,STR_bw,STR_bh,STR_cs);
               continue;
           }
           //---------------------------------------------------------------
           // �뢮� ��஢�� ����ᮢ
           if((OStr[b]=='@')&&
              (OStr[b+1]>47&&OStr[b+1]<58)&&(OStr[b+4]>47&&OStr[b+4]<58)){
               c[0]=OStr[b+1]; c[1]=OStr[b+2]; c[2]=OStr[b+3]; 
               c[3]=OStr[b+4]; c[4]=OStr[b+5]; c[5]=OStr[b+6]; 
               // ������� ����� ��६����� �� ��ப�
               n=atoi(c); if(n<0) n=0; if(n>=MYCM) n=MYCM-1;;
               // ������⢮ ��ࠡ�⠭��� ᨬ����� �室��� ��ப�
               h=4;
               // ���� ��� � ����� ������ ��।���� ������⢮
               // ��� ��᫥ ����⮩ (�� 㬮�砭�� 0)
               if(c[4]=='.' && c[5]>=48 && c[5]<58){ f[4]=c[5]; h=6; }
               sprintf(LMVL[n],f,YCM[n]);
               b=b+h;
               // �뢮� ����祭���� ���祭�� �� �࠭
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
           // �뢮� ��� ��⠫��� ᨬ�����
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
// �।���⥫쭠� ��ࠡ�⪠ �뢮����� ��ப
// �८�ࠧ������ ����� ������ � ��ப� � ⥪�饥 ���祭�� ������
// MS  - 㪠��⥫� �� �������� ᮮ�饭��
// fl  - 䫠� �롮� ��������:
// (0-�����, 1-ᮮ�饭��, 3-��ப�, 4-��ࠡ�⪠ ��� �뢮��)
// nm  - ����� ᮮ�饭�� � ���ᨢ�
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
   // ����塞 ��ப� �����祭��
   for(a=0;a<SLEN;a++){ MS__sto[a]=0; }
   // �஢��塞 ���稪� ����砭�� �⮡ࠦ���� ᮮ�饭��
   if (fl<3){
      if (fl== 0){ a=MTimer(3,'h',100); }
      if (fl== 1){ a=MTimer(4,'h',100); }
      // ������ ᥪ㭤� 㬥��蠥� ���稪
      if (MS->cnt>=1){
         if(a==2){ MS->cnt--; }
      }  else {
      // �᫨ ���稪� ��� - �뢮��� ������ ��ப�
         MS->str[0]=0; MS->cnt=30; ErrCNT=0;
      }
   }
   //------------------------------------------------------------------------
   // �८�ࠧ������ ����� ������ � ���祭��
   l=strlen(MS->str); if(l>=SLEN){ l=SLEN-1; }
   for(a=0,b=0;a<=l;a++){
      // �᫨ ����⨫� ���� ����� ������
      if (MS->str[a]=='#'&&b==0){ 
          // �᫨ ����� ��।���� ������
          if (MS->str[a+1]>64){
              for(e=0;e<SLEN-1;e++){
                  // ��।��塞 �뢮� ���祭�� ������
                  if(MS->str[a+1+e]=='#'&&e>0){ 
                     AW_C[e]=0; b=5; break; 
                  }
                  // ��।��塞 �뢮� �����
                  if(MS->str[a+1+e]=='$'&&e>0){ 
                     AW_C[e]=0; b=6; break; 
                  }
                  // ��।��塞 �뢮� ����� ������
                  if(MS->str[a+1+e]=='%'&&e>0){ 
                     AW_C[e]=0; b=7; break; 
                  }
                  // ��।��塞 �뢮� ��������� ������
                  if(MS->str[a+1+e]=='&'&&e>0){ 
                     AW_C[e]=0; b=8; break; 
                  }
                  AW_C[e]=MS->str[a+1+e];
              }  
              na=0; a=a+e+1;
              // �᫨ �� ������ ��࠭��⥫�� ᨬ��� ����� ������
              if (b==0){ nv=0; b=0; continue; }
              // ��।��塞 ����� ������ � ��饩 ���� �������
              for(e=0;e< MCH;e++){
                  if((strcmp(AW_C,CB[e].nm))==0){ nv=e; break; }
              }
              if (e>=MCH){ nv=0; b=0; };  continue;
          }
          b=1; d=0; continue; 
      }
      // ���⠫� ����� ���ᨢ�
      if((MS->str[a]=='.')&&(b==1)){ 
          b=2; cc[d]=0; na=atoi(cc); d=0;
          continue; 
      }
      // ���⠫� ����� ������
      if(((MS->str[a]<48||MS->str[a]>57) && (b==2)) || (d>4)||(MS->str[a]=='#')){
          cc[d]=0; nv=atoi(cc); d=0;
          b=(MS->str[a]== '.')? 3:5;
          if(b==3||MS->str[a]=='#'){ continue; }
      }
      // ���⠫� ������⢮ ��� ��᫥ ����⮩
      if ((MS->str[a]==0 || MS->str[a]==' ') && b==3){
          cc[d]=0; dc=atoi(cc); d=0; b=5;
          if(dc>=10){ 
             df=1; dd=dc/10; dc=dc-dd*10; if(dd>6){ dd=6; }
          }
          continue;
      }
      // �뢮� ����� ������ �� ���� ������ �������
      if (b>0&&MS->str[a]=='$'&&nv>0){ b= 6; a++; }
      // �᫨ � ���� ����� ��६����� ���� '&' -
      // �뢥��  ������਩ � ��६����� �� ���� ������ �������
      if (b>0&&MS->str[a]=='&'&&nv>0){ b= 8; a++; }
      // �뢮� ⨯� ������ �� ���� ������ �������
      if (b>0&&MS->str[a]=='^'&&nv>0){ b= 9; a++; }
      // �뢮� ����� ����஫��� �� ���� ������ �������
      if (b>0&&MS->str[a]=='p'&&nv>0){ b=10; a++; }
      // ��।����� �ଠ� �뢮�� ��६����� �� ����� �� ���� �������
      if (b==5){
          // �ନ�㥬 �ଠ� �뢮�� ��६�����
          if (dc>4){ dc=4; }
          if (df>0){ 
              sprintf(cc,"%d",dd); SFRM_M[2]=cc[0]; df=0;
          }
              sprintf(cc,"%d",dc); SFRM_M[4]=cc[0]; dc=2;
          // ���� ���� �������
          if (na==0){ sprintf(cc,SFRM_M,CB[nv].v); }
          // �����쭠� ���� �������
          if (na==1){ sprintf(cc,SFRM_M, CBL[nv]); }
          // ���� ������� �ࠩ��� COM ���⮢
          if (na==2){ sprintf(cc,SFRM_M, CBS[nv]); }
          // ���� ������� �ࠩ��� NetBios
          if (na==3){ sprintf(cc,SFRM_M, CBN[nv]); }
          // ���� ������� �ࠩ��� TCP/IP
          if (na==4){ sprintf(cc,SFRM_M, CBT[nv]); }
          // ���ᨢ �६����� �१�� �७���
          if (na==5){ sprintf(cc,SFRM_M,VTRT[nv]); }
          // ���ᨢ ��६����� �������� 'y'
          if (na==6){ sprintf(cc,SFRM_M, YCM[nv]); }
          // ���ᨢ �६����� ��६�����
          if (na==7){ sprintf(cc,SFRM_M,RTMP[nv]); }
          b=0; na=0; nv=0;
          strcat (MS__sto,cc); c=c+strlen(cc); cc[0]=0;
      }

      // '$' � ����⢥ ��ப� �������� ��� ������ (⮫쪮 ���� ���� �������)
      if (b==6){
          if(na==0){ strcat(MS__sto,CB[nv].nm); }
          c=c+strlen(CB[nv].nm); cc[0]=0; b=0; na=0; nv=0;
      }
      // '%' � ����⢥ ��ப� �������� ����� ������
      if (b==7){
          sprintf(cc,"%d",nv); 
          if(na==0){ strcat(MS__sto,cc); }
          c=c+strlen(cc); cc[0]=0; b=0; na=0; nv=0;
      }
      // '&' � ����⢥ ��ப� �������� �������਩ ������
      if(b==8){
          if(na==0){ strcat(MS__sto,CB[nv].rm); }
          c=c+strlen(CB[nv].rm); cc[0]=0; b=0; na=0; nv=0;
      }
      // '^' � ����⢥ ��ப� �������� ⨯ ��६����� ⥪�饣� ������
      if(b==9){
          if(na==0){ strcat(MS__sto,itoa(CB[nv].t,cc,10)); }
          c=c+strlen(itoa(CB[nv].t,cc,10)); cc[0]=0; b=0; na=0; nv=0;
      }
      // 'p' � ����⢥ ��ப� �������� ����� ����஫��� ⥪�饣� ������
      if(b==10){
          if(na==0){ strcat(MS__sto,itoa(CB[nv].dn,cc,10)); }
          c=c+strlen(itoa(CB[nv].dn,cc,10)); cc[0]=0; b=0; na=0; nv=0;
      }

      // �ନ஢���� ��ப�
      if (b>0){ 
          cc[d]=MS->str[a]; if(d<9){ d++; }
      } else  { 
          MS__sto[c]=MS->str[a]; if(c<SLEN-1){ c++; }
      }
   }
   if (fl==4){ strcpy(MS->str,MS__sto); return(0); }
   //------------------------------------------------------------------------
   // �뢮� ��ப� �� ���ᨢ� ��ப
   if (fl==3){
      // �᫨ ��।����� �ப��⪠
      if (SSTR[nm].st>0){
         if (SSTR[nm].spc>SSTR[nm].st){
             SSTR[nm].spc=0;
             SSTR[nm].lc=strlen(MS__sto);
             SSTR[nm].lp=text_length(dFnt[MS->sn].dat,MS__sto);
             STR_Out(MS__sto,MS->sn,MS->X,MS->Y,MS->lcs,1);
         }
         SSTR[nm].spc++;
      // �᫨ �ப��⪠ �� ��।����� ���� �뢮��� ᮮ�饭��
      }  else  {
         // ������ ��ப� � ���ᥫ�� MS->lcs (�� XMAX)
         STR_Out(MS__sto,MS->sn,MS->X,MS->Y,MS->lcs,1);
      }
      return(0);
   }
   //------------------------------------------------------------------------
   // ���稪 ����ᮢ�� ᮮ�饭��
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
// �뢮� ���ଠ樨 �� ����
vd MesAUTH(_i md, _i wa){
   _i a=0, b=0, c=0, ws=0;
   if (AUFL>0){
       //--------------------------------------------------------------------
       // �뢮� ����।��� ᬥ��� ��ப
       if (md==1){
           if (AUFL%5==0){
               strcpy(STAT[1].str,MDEC[AUCNT]); STAT[1].cnt=7; AUCNT++;
               if (AUCNT>10){ AUCNT=2; }
           }
           AUFL++;
       }
       //--------------------------------------------------------------------
       // �뢮� � ०��� "������ ��ப�"
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
           // �뤥প� ���� ��। ��砫�� �������� ��ப�
           if (AUw<wa){ AUw++; }
           // ����- ���室 � ᫥���饬� ᨬ����
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
// ����஫� ����� �ணࠬ�� ����஫���
vd CheckCSPC(vd){
   _i a=0;
   if (AUTHNMCH[0]==0) return;
   for(a=0;a<HAUTHC;a++){
       if(AUTHNMCH[a]>0 && AUTHNMCH[a]<10){
          sprintf(STAT[1].str,"�ணࠬ�� PCCS ������� ࠡ��� �१ %.0f ����",AUTHCODE);
          STAT[1].cnt=10;
       }
       if(AUTHNMCH[a]==15){
          sprintf(STAT[1].str,"�訡�� ��⠭���� ���ਧ�樨 PCCS. �몫���� PCCS!");
          STAT[1].cnt=10; break;
       }
       if(AUTHNMCH[a]==19){
          sprintf(STAT[1].str,"�ப ���ਧ�樨 �ணࠬ�� PCCS �����祭. �몫���� PCCS!");
          STAT[1].cnt=10; break;
       }
   }
   return;
}
//---------------------------------------------------------------------------

