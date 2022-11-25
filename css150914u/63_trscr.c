// coding: cp866
//---------------------------------------------------------------------------
// ��ࠢ����� �७����
vd TRD_Set(_i md){
   _i a=0;
   _f D=0,N=0,M=0;
   // ��� ������� �७�� �࠭�
   for(a=0;a<HTRD;a++){
      // �᫨ �� ��⨢�� �७�
      if (TF[CTC].an[a]==1){
          //-----------------------------------------------------------------
          // CTRL+TAB - ��४��祭�� ����� �७���� ����㦥����� �࠭�
          if (md==1){
             // 㡨ࠥ� � ���� �ਧ��� ��⨢����
             TF[CTC].an[a]=0;
             // ��⠭�������� �ਧ��� ��⨢���� ��� ᫥���饣� �७��
             if (a>=HTRD-1){ TF[CTC].an[  0]=1; TF[CTC].ta=0;   }
             else          { TF[CTC].an[a+1]=1; TF[CTC].ta=a+1; }
             break;
          }
          //-----------------------------------------------------------------
          // CTRL+'+',CTRL+'-',CTRL+'*' -��������� ������⢠ ᥪ㭤 � �७��
          if (md==2){
             // 㬥��襭�� ������⢠ ᥪ㭤 � ��䨪� �७�� (CTRL+'-')
             if (KCode==KEY_PLUS_PAD ){ LTRD[a].sec=LTRD[a].sec-60; }
             if (KCode==KEY_EQUALS   ){ LTRD[a].sec=LTRD[a].sec-60; }
             // 㢥��祭�� ������⢠ ᥪ㭤 � ��䨪� �७�� (CTRL+'+')
             if (KCode==KEY_MINUS_PAD){ LTRD[a].sec=LTRD[a].sec+60; }
             if (KCode==KEY_MINUS    ){ LTRD[a].sec=LTRD[a].sec+60; }
             // ��⠭����� ��ࢮ��砫쭮� ������⢮ ᥪ㭤   (CTRL+'*')
             if (KCode==KEY_ASTERISK ){ LTRD[a].sec=LTRD[a].seco;   }
             // �஢�ઠ �࠭���� ���祭��
             if (LTRD[a].sec<_TRS){ LTRD[a].sec=_TRS; }
             if (LTRD[a].sec>MTRS){ LTRD[a].sec=MTRS; }
             // ������� ��䨪�
             if (TF[CTC].aa[a]==1){ RARead(a,1); }
             else                 { RARead(a,5); }; VA_to_VT(a);
             // �஢�ઠ �࠭�� �६������ �१�
             if (TF[CTC].lt[a]>LTRD[a].sec){ TF[CTC].lt[a]=LTRD[a].sec; }
             if (TF[CTC].lt[a]< 0){ TF[CTC].lt[a]=0; }
             break;
          }
          //-----------------------------------------------------------------
          // CTRL+A - ���室 ����� ०����� �७��
          // �᫨ CTRL+D   | CTRL+T   -�믮����� ��� �㭪� � ��३� �� md=8|9
          // �᫨ CTRL+PgUp| CTRL+PgDn-�믮����� ��� �㭪� � ��३� �� md=7
          if(((md==8||md==9||md==7)&&TF[CTC].aa[a]==0)||md==3){
             // �᫨ �� �७� �뢮����� �� ⥪�騩 ��娢
             if(TF[CTC].ff[a]==1){
                // ������� 䠩� �� ��娢�
                dbClose(TF[CTC].fh[a]); TF[CTC].ff[a]=0;
                // ��८�।����� ��� 䠩�� �� ��娢�
                FARNRd();
             }
             if(TF[CTC].ff[a]!=3){
                TF[CTC].fh[a]=ARCOut;
             } else {
                TF[CTC].fh[a]=UPDOut;
             }
             TF[CTC].aa[a]=(TF[CTC].aa[a]==1)?0:1;
             TF[CTC].am[a]=(TF[CTC].aa[a]==1)?1:0;
             // �⥭�� 䠩�� ��娢�
             if(TF[CTC].aa[a]==0){ RARead(a,0); }  // �᫨ � ०��� �७��
             if(TF[CTC].aa[a]==1){ RARead(a,5); }  // �᫨ � ०��� ��娢�
             VA_to_VT(a);
          }
          //-----------------------------------------------------------------
          // ��।������� �६������ �१� �७��
          // CTRL <-, CTRL ->, CTRL HOME, CTRL END
          if (md==4){
             if (KCode==KEY_LEFT){ 
                 TF[CTC].lt[a]=TF[CTC].lt[a]+1; 
             }
             if (KCode==KEY_RIGHT){ 
                 TF[CTC].lt[a]=TF[CTC].lt[a]-1;
                 if(TF[CTC].lm[a]==1&&LTRD[a].rst==0){ 
                    TF[CTC].lt[a]=TF[CTC].lt[a]-1; 
                 }
             }
             if (KCode==KEY_HOME ){ TF[CTC].lt[a]=-1; }
             if (KCode==KEY_END  ){ TF[CTC].lt[a]=LTRD[a].sec; }
             // �஢�ઠ �࠭�� �६������ �१�
             if (TF[CTC].lt[a]>LTRD[a].sec){ TF[CTC].lt[a]=LTRD[a].sec; }
             if (TF[CTC].lt[a]<0){
                 if (TF[CTC].lm[a]==0){ TF[CTC].lt[a]= 0; }
                 else                 { TF[CTC].lt[a]=-1; }
             }
          }
          //-----------------------------------------------------------------
          // CTRL+1...CTRL+9 - ���/�몫 ������� �७�� ��� �⮡ࠦ����
          if (md==5){
             if (TF[CTC].ga[a][KCode-KEY_0-1]==1){ TF[CTC].ga[a][KCode-KEY_0-1]=0; }
             else                                { TF[CTC].ga[a][KCode-KEY_0-1]=1; }
          }
          //-----------------------------------------------------------------
          // CTRL+INS, CTRL+DEL - ��������� 蠣� �६������ �१�
          if (md==6){
             if (KCode==KEY_0_PAD  ){ LTRD[a].rsa=LTRD[a].rsa+1; }
             if (KCode==KEY_INSERT ){ LTRD[a].rsa=LTRD[a].rsa+5; }
             if (KCode==KEY_DEL_PAD){ LTRD[a].rsa=LTRD[a].rsa-1; }
             if (KCode==KEY_DEL    ){ LTRD[a].rsa=LTRD[a].rsa-5; }
             if (LTRD[a].rsa< 1    ){ LTRD[a].rsa=36; }
             if (LTRD[a].rsa>36    ){ LTRD[a].rsa= 1; }
             // ������� ��䨪�
             if (TF[CTC].aa[a]==1){ RARead(a,1); }
             else                 { RARead(a,5); }; VA_to_VT(a);
          }
          //-----------------------------------------------------------------
          // CTRL+PgDn, CTRL+PgUp - ��ॢ�� �७�� � ०�� [A], ���⠭��
          //  �७�� �� ������⢮ ᥪ㭤 � �७��
          if (md==7){
             if(TF[CTC].aa[a]==1){
                // PgDn - ���⠥� �����
                if(KCode==KEY_PGDN){ RARead(a,3); }
                // PgUp - ���⠥� ���।
                if(KCode==KEY_PGUP){ RARead(a,4); }
             }
          }
          //-----------------------------------------------------------------
          // CTRL+D - ���� ���� 䠩�� ��娢� ��� ��ᬮ��
          if (md==8){
             // �᫨ �� ��⨢�� �७� - ��������� �������� �����
             if(TF[CTC].an[a]==1 && TF[CTC].aa[a]==1){
               strcpy(GS.ES,TF[CTC].dt[a]);
               GS.LS =10;
               GS.RX1=GS.RDX1;
               GS.RY1=GS.RDY1;
               GS.RX2=GS.RDX2;
               GS.RY2=GS.RDY2;
               GS.TX =GS.TDX;
               GS.TY =GS.TDY;
               GS.W  =200;
               GS.sn =LTRD[a].ft;
               GS.cc =LTRD[a].cr;
               GS.cs =LTRD[a].cr;
               GS.cg =LTRD[a].g1;
               strcpy(GS.sf,"00.00.0000");
               LGET_el=0;
               GetStr(0);
               GS.FL =1;
               GS.fe =1;
               md=3;
             }
          }
          //-----------------------------------------------------------------
          // CTRL+T - ���� �६��� ��ᬮ�� ��娢�
          if (md==9){
             // �᫨ �� ��⨢�� �७� - ��������� �������� �����
             if(TF[CTC].an[a]==1 && TF[CTC].aa[a]==1){
               strcpy(GS.ES,TF[CTC].tm[a]);
               GS.LS =8;
               GS.RX1=GS.RTX1;
               GS.RY1=GS.RTY1;
               GS.RX2=GS.RTX2;
               GS.RY2=GS.RTY2;
               GS.TX =GS.TTX;
               GS.TY =GS.TTY;
               GS.W  =200;
               GS.sn =LTRD[a].ft;
               GS.cc =LTRD[a].cr;
               GS.cs =LTRD[a].cr;
               GS.cg =LTRD[a].g1;
               strcpy(GS.sf,"00:00:00");
               LGET_el=0;
               GetStr(0);
               GS.FL =1;
               GS.fe =2;
               md=3;
             }
          }
          //-----------------------------------------------------------------
          // ��������� 誠�� �७��
          // CTRL+>,CTRL+<,CTRL+����,CTRL+�����,CTRL+M,CTRL+/
          if (md==10){
             // �᫨ �� ��⨢�� �७�
             if(TF[CTC].an[a]==1){
                D=LTRD[a].maxd-LTRD[a].mind;
                N=LTRD[a].max-LTRD[a].min;
                // ��४��祭�� ०��� 蠣� �� 誠�� �७��
                if(KCode==KEY_M    ){ mTRDS=(mTRDS>0)?0:1; }
                if(mTRDS >0){ M=N; }
                else        { M=D; }
                // ����⠭������� ��砫��� ���祭�� ����������
                if(KCode==KEY_SLASH){
                   mTRDZ=(mTRDZ>0)?0:1;
                   if(mTRDZ>0){
                      LTRD[a].mino=LTRD[a].min;
                      LTRD[a].maxo=LTRD[a].max;
                      LTRD[a].min=LTRD[a].mind;
                      LTRD[a].max=LTRD[a].maxd;
                   } else {
                      LTRD[a].min=LTRD[a].mino;
                      LTRD[a].max=LTRD[a].maxo;
                   }
                }
                // ᦠ⨥ ��������� �⮡ࠦ���� �७�� �� 10%
                // �� �������� ���������
                if(KCode==KEY_STOP){
                   if (LTRD[a].max+(D*0.1)<LTRD[a].maxd*2){
                       LTRD[a].max=LTRD[a].max+(D*0.1);
                   }
                }
                // ���७�� ��������� �⮡ࠦ���� �७�� �� 10%
                if(KCode==KEY_COMMA){
                   if (LTRD[a].max-(D*0.1)>LTRD[a].min){
                       LTRD[a].max=LTRD[a].max-(D*0.1);
                   }
                }
                // �������� �� 誠�� �७�� �����-����
                if(KCode==KEY_UP){
                   if(LTRD[a].max+(M*0.1)<LTRD[a].maxd*2){
                      LTRD[a].min=LTRD[a].min+(M*0.1);
                      LTRD[a].max=LTRD[a].max+(M*0.1);
                   }
                }
                if(KCode==KEY_DOWN){
                   if(LTRD[a].min-(M*0.1)>=LTRD[a].mind){
                      LTRD[a].min=LTRD[a].min-(M*0.1);
                      LTRD[a].max=LTRD[a].max-(M*0.1);
                   } else {
                      LTRD[a].max=LTRD[a].max-(LTRD[a].min-LTRD[a].mind);
                      LTRD[a].min=LTRD[a].mind;
                   }
                }
                if(KCode==KEY_BACKSLASH){
                   LTRD[a].sec=LTRD[a].seco;
                }
             }
          }
          //-----------------------------------------------------------------
          // CTRL+ENTER ��������� ०��� �������� �६������ �१�
          if (md==11){
             // �᫨ �� ��⨢�� �७�
             if(TF[CTC].an[a]==1){
                if(TF[CTC].lm[a]==1){ TF[CTC].lm[a]=0; }
                else                { TF[CTC].lm[a]=1; }
             }
          }
          //-----------------------------------------------------------------
      }
   }
   return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ���������� ���ᨢ�� �७��� ॠ��묨 ���祭�ﬨ �� ���� �������
// �⮡ࠦ���� ��� ०��� ���������
vd TRFill(_i md){
   _i  a=0,b=0,t=0;
   //  ��� ������� �७�� �࠭�
   for(t=0;t< HTRD;t++){
      //---------------------------------------------------------------------
      //  ���������� ���ᨢ� �७�� ॠ��묨 ���祭�ﬨ � �������
//    if(LTRD[t].th<10){
         // ⥪�饥 ���祭�� 蠣� �뢮�� �७��
         LTRD[t].rst++;
         // �᫨ ⥪�饥 ���祭�� 蠣� �뢮�� �७�� ����� ��� ࠢ��
         // 蠣� �⥭��  ����ᥩ  �� ��娢�
         if (LTRD[t].rst>=LTRD[t].rsa){
             // 
             if (md==1){
                // ��� ������ ᥪ㭤� �७��� ��७��� �����
                for(a=MTRS-LTRD[t].sec;a<MTRS;a++){
                    VTRD[t].s[a].t=VTRD[t].s[a+1].t;
                    VTRD[t].s[a].h=VTRD[t].s[a+1].h;
                    VTRD[t].s[a].m=VTRD[t].s[a+1].m;
                    VTRD[t].s[a].s=VTRD[t].s[a+1].s;
                    for(b=0;b< TF[CTC].gh[t];b++){
                        VTRD[t].s[a].v[b]=VTRD[t].s[a+1].v[b];
                    }
//    DPrn(0,0,"[%d] %d:%d:%d=%f\n",a,
//            VTRD[t].s[a].h,VTRD[t].s[a].m,VTRD[t].s[a].s,VTRD[t].s[a].v[0]);
                }
             }
             VTRD[t].s[MTRS].t=TMST;
             VTRD[t].s[MTRS].h=tmm->tm_hour;
             VTRD[t].s[MTRS].m=tmm->tm_min;
             VTRD[t].s[MTRS].s=tmm->tm_sec;
             for(b=0;b <TF[CTC].gh[t];b++){
                 VTRD[t].s[MTRS].v[b]=CB[arc_ch[trv_cc[t][b]]].v;
             }
//    DPrn(0,0,"!%d! %d:%d:%d=%f\n",MTRS,
//         VTRD[t].s[MTRS].h,VTRD[t].s[MTRS].m,VTRD[t].s[MTRS].s,VTRD[t].s[MTRS].v[0]);
             RABuf[t].po=RABuf[t].po+LTRD[t].rst;
             LTRD[t].rst=0;
         }
//    } else {
      //---------------------------------------------------------------------
      //

//    }
      //---------------------------------------------------------------------
   }
   //------------------------------------------------------------------------
   return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ��।�� ������ �� ���ᨢ� �⮡ࠦ���� ��娢� � ���ᨢ �⮡ࠦ���� �७��
vd VA_to_VT(_i t){
   _i  a=0, e=0;
   for(a=MTRS-LTRD[t].sec;a<=MTRS;a++){
       VTRD[t].s[a].t= VARC[t].s[a].t;
       VTRD[t].s[a].h= VARC[t].s[a].h;
       VTRD[t].s[a].m= VARC[t].s[a].m;
       VTRD[t].s[a].s= VARC[t].s[a].s;
       for(e=0;e<TF[CTC].gh[t];e++){ VTRD[t].s[a].v[e]=VARC[t].s[a].v[e]; }
//
//     DPrn(0,0,"[%d] %d:%d:%d=%f\n",
//          a,VTRD[t].s[a].h,VTRD[t].s[a].m,VTRD[t].s[a].s,VTRD[t].s[a].v[0]);
   }
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// �㭪�� �⥭�� ������ �� 䠩�� ���������
_i RAReadUpd(_i n,_i SEL){
   int  e=0, h=0, m=0, r=0, s=0;
   // ������⢮ ����ᥩ � 䠩�� ��������� �� ⥪�騩 ������
   r=RABuf[n].pt=dbRecCount(SEL)-1;
   // ��३� �� ��᫥���� ������
   dbGoLast(SEL); e=dbEof(SEL);
   // �᫨ 㪠��⥫� �� �।����� 䠩�� (��� ����ᥩ)
   if(e>0||r<0){ 
      ;;
   }
   // ��������� �६� � ���祭�� �७�� ⥪�饩 �����
   h=atoi(dbFieldGetStr(SEL,0,"THR",0));
   m=atoi(dbFieldGetStr(SEL,0,"TMN",0));
   s=atoi(dbFieldGetStr(SEL,0,"TSC",0));

   DPrn(0,0,"RAU: S[%d] h[%d] m[%d] s[%d] e[%d] r[%d]\n",SEL,h,m,s,e,r);

//�᫨ �����-� ��ࠬ��� ���������, �ନ����� ������ � �६����
//� ���祭�ﬨ ��� ��� ��ࠬ��஢ ��㯯� (�७��) ��� ����� ��� ��娢��㥬�� ������� (������ �� �࣠����樨 ��娢�...).
//�७� ��⮨� �� N-����ᥩ, ��� H-��ࠬ��஢ TR[N][H].
//�� ���ᮢ�� �७�� ������塞 ���ᨢ �� ⥪�饩 ����� ��娢� �
//������ ��᫥���饩. �᫨ ᫥����� ������ ����� ���-� �� �࠭�楩
//�७��, � ���祭�� �� �७�� ������ ��⠢����� ⠪��, ����� �뫮
//(���� �� ������ �� �窨 ���������).

   // ����� ���������� �७�� �� ⥪�饣� �६��� �� �६��� � ����������� �����
    
   // �᫨ �த����⥫쭮��� ����� �७�� �����稫���-���

   // �᫨ �६� ⥪�饩 ����� �७�� ����� �६��� ��砫� ��⮪(00:00)-���

   // ⥯��� ���室�� � �।��饩 �����  � �� ��� **


   return(0);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// �㭪�� �⥭�� ������ �� 䠩�� ��娢�
_i RARead(_i t, _i md){
   // ��।����� �६���� ��६���� � ०�� �⥭��
   _i  a=0, b=0, fr=0, fd=0, n=0, z=0, fn=dbFieldCount(TF[CTC].fh[t]);
   DPrn(12,0,"\n");
   DPrn(12,0,"ArcFlush...");
   // ����� ��᫥���� ���� ��娢�
   ARCFlush();
   DPrn(12,0,"%s\n",SMSG[0]);
   z= LTRD[t].sec*LTRD[t].rsa;
   // �᫨ ⥪�騩 �७� �⮡ࠦ��� ������ ���������- 
   // ��३� �� �㭪�� �⥭�� �⮣� ⨯� ��娢�
   if(LTRD[t].th>=10){ RAReadUpd(t,TF[CTC].fh[t]); return(0); }
   //------------------------------------------------------------------------
   // ������⢮ ����ᥩ � 䠩�� ��娢� �� ⥪�騩 ������
   RABuf[t].pt=dbRecCount(TF[CTC].fh[t])-1;
   // ��⠭����� ��ࠬ���� �७�� ��� ��ࢮ�� �⥭�� ��娢�
   if(md==0){ 
      RABuf[t].pr=RABuf[t].pt; RABuf[t].po=RABuf[t].pt; LTRD[t].rst=0; 
   }
   // �� ��������� ������⢠ ����ᥩ � 蠣� �⥭�� ����ᥩ �७�� 
   if(md==1){;;}
   // � ��।����� ᬥ饭�� � 䠩�� (�� ���᪥)
   if(md==2){;;}
   // ����� ������ PgDn (�த������� ���뢠��� �����)
   if(md==3){
      if(RABuf[t].pr-z>0){ RABuf[t].pr=RABuf[t].pr-z; }
   }
   // ����� ������ PgUp (�த������� ���뢠��� ���।)
   if(md==4){
      if(RABuf[t].pr+z<RABuf[t].pt){ RABuf[t].pr=RABuf[t].pr+z; } 
      else                         { RABuf[t].pr=RABuf[t].pt;   }
   }
   // �����뢠��� ��娢� �� ���室��
   if(md==5){ RABuf[t].pr=RABuf[t].po; }        
   //
   sprintf(SMSG[MMSG],SMSG[169],RABuf[t].pr,dbRecCount(TF[CTC].fh[t]));
   DPrn(12,0,"%s\n",SMSG[MMSG]);
   //------------------------------------------------------------------------
   // �⥭�� ������ �� 䠩�� ��娢�
   sprintf(SMSG[MMSG],SMSG[170]);
   DPrn(12,0,"%s\n",SMSG[MMSG]);
   // ���㫨�� ⥪���� ��� �७�� (��������� ��䨪� ���祭�� �७�� ��ﬨ)
   for(a=MTRS;a>=MTRS-LTRD[t].sec;a--){
       VARC[t].s[a].t=0; VARC[t].s[a].h=0; VARC[t].s[a].m=0; VARC[t].s[a].s=0;
       for(b=0;b<TF[CTC].gh[t];b++){ VARC[t].s[a].v[b]=0; };
   }
   sprintf(SMSG[MMSG],SMSG[171]);
   DPrn(12,0,"%s\n",SMSG[MMSG]);

   // ��� ������ ����� (ᥪ㭤�) �७��
   for(a=MTRS,n=0;a>=MTRS-LTRD[t].sec;a--,n=n+LTRD[t].rsa){
       if (RABuf[t].pr-n<0){ break; };
       // ��३� �� ᫥������ ������
       dbGoRec(TF[CTC].fh[t],RABuf[t].pr-n);
       // �᫨ ���諨 �� ��᫥���� ������ �� ��娢�- ���
       if(dbBof(TF[CTC].fh[t])>0||dbEof(TF[CTC].fh[t])>0){
          sprintf(SMSG[MMSG],SMSG[172],RABuf[t].pr-n);
          DPrn(12,0,"%s\n",SMSG[MMSG]);
          break;
       }
       // ������ ������ �� ��娢�
       fr=dbRecRead(TF[CTC].fh[t]); 
       if(fr<0){ 
          sprintf(SMSG[MMSG],SMSG[173],RABuf[t].pr-n);
          DPrn(0,0,"%s\n",SMSG[MMSG]);
          break; 
       }
       // �����, ����祭�� �� 㤠�����, �� ���뢠����
       fd=isRecDFL(TF[CTC].fh[t]); if(fd>0){ a++; continue; }
       // ��।��� ���祭�� �� ���� ����� � ���ᨢ ��娢�
       VARC[t].s[a].t=(_i) dbFieldGetVal(TF[CTC].fh[t],BREC-4,0,0); 
       VARC[t].s[a].h=atoi(dbFieldGetStr(TF[CTC].fh[t],BREC-3,0,0));
       VARC[t].s[a].m=atoi(dbFieldGetStr(TF[CTC].fh[t],BREC-2,0,0));
       VARC[t].s[a].s=atoi(dbFieldGetStr(TF[CTC].fh[t],BREC-1,0,0));
       // ��� ������� ���� �७��
       for(b=0;b<TF[CTC].gh[t];b++){
           // �᫨ ����� ⥪�饣� ����  �����, 祬 �������� -
           // ��� �� 横�� ��।�� ���祭��
           if(trv_cc[t][b]+BREC>=fn){ 
              sprintf(SMSG[MMSG],SMSG[174],trv_cc[t][b]+BREC,fn);
              DPrn(0,0,"%s\n",SMSG[MMSG]); // D: Not find field [N%d fn=%d]
              break; 
           }
           // ��।��� ���祭�� ������� ���� �� ���� ����� � ���ᨢ ��娢�
           VARC[t].s[a].v[b]=dbFieldGetVal(TF[CTC].fh[t],trv_cc[t][b]+BREC,0,0);
       }
       DPrn(12,0,"\n[%d] %d:%d:%d=%f\n",
               a,VARC[t].s[a].h,VARC[t].s[a].m,VARC[t].s[a].s,VARC[t].s[a].v[0]);
   }
   //------------------------------------------------------------------------
   return(0);
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
// �뢮� �७��� �� �࠭
vd TRDScr(vd){
   _f g2=0, g3=0;                      // 䫠��
   _i  a=0, b=0, i=0, j=0, t=0;        // ��६���� 横���
   _f h1=0,h2=0,vl=0;                  // ������
   _f ws=0;                            // 蠣� � ���ᥫ��
   _f trd_s1=0, trd_s2=0;              // ...
   _f X1=0, X2=0, r=0, txx=0;          // ���न���� ��� ����஥��� ��䨪�
   _f XX=0, zz[5]={0,0,0,0,0};
   _f tX1=0,tX2=0,tY1=0,tY2=0;         // ��� ����஥��� ⠡���� �६ �१�
   _f Y1[MTRG];                        // ...
   _f Y2[MTRG];                        // ...
   uc cc[3]={0,0,0};                   // ...
   _f vs=0, mdiv=0, hs=0;              // ��६���� ���᫥���
   _f kx1=0, ky1=0, kx2=0, ky2=0;      // ���� 㢥��祭�� ���न���
   _i trfl=0;                          // 䫠� ����訢���� �७��
   _i pnt[16];                         // ���ᨢ ���न���
   _i cs=0;                            // 梥� ���ᮢ�� ⥪�� ��� ��䨪�
   _i th=0;                            // ⮫騭� ����� �७���

   // �᫨ �� ��⠭����� 䫠� ᬥ�� ᥪ㭤� � �� �뫮 ����� ������� ������
   // �ࠢ����� �७���-���
   if (TF[CTC].fl[a]==0&&TLKey==0){ return; }
   // ��� ��� ��।������� �७��� ⥪�饣� �࠭�
   for(a=0;a<HTRD;a++){
      // �᫨ ��䨪 � ०��� ��娢� � �� �뫮 ����� ������� ������
      // �ࠢ����� �७���-���
      if (TF[CTC].aa[a]==1&&TLKey==0){ continue; }; TLKey=0; FLSEL=1;
      // ��⠥� ᪮�쪮 ���ᥫ�� ࠢ�� ���� 䮭� �६���
      h1=text_height(dFnt[LTRD[a].ft].dat)+2;
      // ��⠥� ᪮�쪮 ���ᥫ�� ࠢ�� ���� 䮭� 誠��
      h2=text_height(dFnt[LTRD[a].fs].dat)+2;
      //
      X1=0;X2=0;r=0;
      for(b=0;b<MTRG;b++){ Y1[b]=0;Y2[b]=0; }
      // �஢��塞 ������⢮ 蠣�� �७��
      if (LTRD[a].stx>MSTP){ LTRD[a].stx=MSTP; }
      if (LTRD[a].sty>MSTP){ LTRD[a].sty=MSTP; }
      //---------------------------------------------------------------------
      // ��।��塞 ������ �뢮�� 誠�� �७��
      sprintf(cc,"%d",CB[LTRD[a].n[0]].dc); SFRM_T[4]=cc[0];
      sprintf(try_max,SFRM_T,LTRD[a].max);
      // ��।��塞 ����� ��� 誠�� �७�� �� ������ �뢮�� �६���
      vl=text_length(dFnt[LTRD[a].fs].dat,"00:00:00");
      txx=vl*0.05;
      // �����뢠�� 蠣 ����� �� �� X
      ws= divf(LTRD[a].W2, ((LTRD[a].stx>0)?LTRD[a].stx:1));
      // �����뢠�� 蠣 ����� �� �� Y
      hs= divf(LTRD[a].H2, ((LTRD[a].sty>0)?LTRD[a].sty:1));
      // ��।��塞 蠣 誠�� �७��
      vs= divf((LTRD[a].max-LTRD[a].min),((LTRD[a].sty>0)?LTRD[a].sty:1));
      // �������� �뢮����� 誠��
      if (LTRD[a].max-LTRD[a].min>0){ mdiv=LTRD[a].max-LTRD[a].min; }
      else                          { mdiv=1;                       }
      // ������⢮ ���ᥫ�� � 1 ᥪ㭤�
      trd_s1= divf(LTRD[a].W2,  LTRD[a].sec);
      // ������⢮ ᥪ㭤 � ����� 蠣�
      trd_s2= divf(LTRD[a].sec, LTRD[a].stx);
      //---------------------------------------------------------------------
      // ��㥬 䮭
      ky1=4.5;  tY1=h1*ky1;
      for(t=0;t<MTRG;t++){
          if (LTRD[a].hm>0 && TF[CTC].gh[a]>t && TF[CTC].hm[a]==0){
              if((t % LTRD[a].hm)==0){ ky1=ky1+1.5; tY1=h1*ky1; }
          }
      }
      rectfill(scrsav, LTRD[a].X1,
                       LTRD[a].Y1,
                       LTRD[a].X1+ LTRD[a].W1, LTRD[a].Y1+LTRD[a].H1,
                       LTRD[a].g1 );
      rectfill(scrsav, LTRD[a].X2,
                       LTRD[a].Y2,
                       LTRD[a].X2+ LTRD[a].W2, LTRD[a].Y2+LTRD[a].H2,
                       LTRD[a].g2 );

      //  ��㥬 ����� �� ��ਧ��⠫� (Y) � 誠�� �७��
      for(b=0;b<=LTRD[a].sty;b++){
         line (scrsav, LTRD[a].X2,
                       LTRD[a].Y2+ LTRD[a].H2 - hs*b,
                       LTRD[a].X2+ LTRD[a].W2 + 4,
                       LTRD[a].Y2+ LTRD[a].H2 - hs*b, LTRD[a].cs );
         text_mode(LTRD[a].g1);
         sprintf(try_max,SFRM_T,vs*b+LTRD[a].min);
         textout(scrsav,dFnt[LTRD[a].fs].dat,try_max,
                       LTRD[a].X2+ LTRD[a].W2 + vl*0.25,
                       LTRD[a].Y2+ LTRD[a].H2 - hs*b-hs*0.25,
                       LTRD[a].cy);
      }
      //---------------------------------------------------------------------
      // ��㥬 �����⨪� ��� �뢮�� �६��� � ���祭�� �६������ �१�
      for(t=0,ky1=2.0,ky2=3.5;t<MTRG+2;t++){
         if (t==0){
             tX1=vl*0.0-txx; tY1=h1*2.0; tX2=vl*1.1; tY2=h1*3.5;
         }
         if (t==1){
             tX1=vl*1.1;     tY1=h1*2.0; tX2=vl*2.6; tY2=h1*3.5;
         }
         //--------
         if (t==2){
             if (TF[CTC].hm[a]>0){ XX=tX2+vl*0.25; }
             else                { XX=0; }
         }
         // ���誨 ���祭��
         if (LTRD[a].hm>0&&t>1){
            if (TF[CTC].gh[a]>t -2){
               if(((t-2) % LTRD[a].hm) > 0){
                   kx1=kx1+1.1;  kx2=kx2+1.1;
                   tX1=vl*kx1;   tX2=vl*kx2;
                }  else  {
                   if (TF[CTC].hm[a]==0){ ky1=ky1+1.5; ky2=ky2+1.5; }
                   kx1=0; kx2=1.1;
                   tX1=vl*kx1-txx;  tY1=h1*ky1;   tX2=vl*kx2;  tY2=h1*ky2;
               }
               // ������� ���誨 梥⮬ ���� �७��
               rectfill(scrsav,LTRD[a].X2+XX+tX1, LTRD[a].Y2+LTRD[a].H2+tY1*1.1, 
                         LTRD[a].X2+XX+tX2, LTRD[a].Y2+LTRD[a].H2+tY2*1.1,
                         LTRD[a].g2 );
               // �뢥�� ����� ���襪 ��� �⮡ࠦ���� ���祭�� �६������ �१�
               text_mode(LTRD[a].g2);
               sprintf(try_max,"%d",t-1);
               textout(scrsav,dFnt[1].dat,try_max,
                              LTRD[a].X2+XX+tX1+1,
                              LTRD[a].Y2+LTRD[a].H2+tY1*1.1+1,
                              LTRD[a].cs);
            }
         }
         // ������� ���誨 �६��� � ���� 梥⮬ ���� �७��
         if (LTRD[a].hm>0&&t<2){
            rectfill(scrsav,LTRD[a].X2+XX+tX1, LTRD[a].Y2+LTRD[a].H2+tY1*1.1, 
                            LTRD[a].X2+XX+tX2, LTRD[a].Y2+LTRD[a].H2+tY2*1.1,
                            LTRD[a].g2 );
         }
         // ࠬ�� ����� ���襪 梥⮬ �⪨
         rect (scrsav, LTRD[a].X2+XX+tX1, LTRD[a].Y2+LTRD[a].H2+tY1*1.1,
                       LTRD[a].X2+XX+tX2, LTRD[a].Y2+LTRD[a].H2+tY2*1.1,
                       LTRD[a].cs );
         // ��������� ���न���� ������ �뢮�� �६��� ��⨢���� �७��
         if (t==0&&TF[CTC].an[a]==1){
             GS.RTX1=LTRD[a].X2+XX+tX1+1; GS.RTY1=LTRD[a].Y2+LTRD[a].H2+tY1*1.1+1;
             GS.RTX2=LTRD[a].X2+XX+tX2-1; GS.RTY2=LTRD[a].Y2+LTRD[a].H2+tY2*1.1-1;
         }
         // ��������� ���न���� ������ �뢮�� ���� ��⨢���� �७��
         if (t==1&&TF[CTC].an[a]==1){
             GS.RDX1=LTRD[a].X2+XX+tX1+1; GS.RDY1=LTRD[a].Y2+LTRD[a].H2+tY1*1.1+1;
             GS.RDX2=LTRD[a].X2+XX+tX2-1; GS.RDY2=LTRD[a].Y2+LTRD[a].H2+tY2*1.1-1;
         }
         // ��������� ���न���� ���襪 ���祭��
             GS.TTX =LTRD[a].X2+0;      GS.TTY =LTRD[a].Y2+LTRD[a].H2+h1*2.45*1.1;
             GS.TDX =LTRD[a].X2+vl*1.3; GS.TDY =LTRD[a].Y2+LTRD[a].H2+h1*2.45*1.1;
      }
      //---------------------------------------------------------------------
      text_mode(LTRD[a].g1);
      // ��।����� �뢮� ᮮ⢥������� ᨬ�����
      tras[2]=(TF[CTC].aa[a]==1)?'A':'C';
      // �� ��⨢��� ��䨪� � ����� ���孥� 㣫� �뢮��� ०�� �⮡ࠦ����
      if(TF[CTC].an[a]==1){
         tras[0]='[';tras[4]=']';
      }  else {
         tras[0]=' ';tras[4]=' ';
      }
      textout(scrsav,dFnt[LTRD[a].fs].dat,tras,
                    LTRD[a].X2+ vl*0.3,
                    LTRD[a].Y2- h2,
                    LTRD[a].cx);
      // �뢥�� ������⢮ ᥪ㭤 � ��䨪� � 蠣 �६������ �१�
      sprintf(mmss,"[%04d]  [%02d]",LTRD[a].sec,LTRD[a].rsa);
      textout(scrsav,dFnt[LTRD[a].fs].dat,mmss,
                    LTRD[a].X2+ vl*1.5,
                    LTRD[a].Y2- h2,
                    LTRD[a].cx);
      //---------------------------------------------------------------------
      // �᫨ ��⠭����� 䫠� ᬥ�� ᥪ㭤
      if (TF[CTC].fl[a]>0){
          // ������� ������⢮ ���ᥫ�� ��� 1 ᥪ
          TF[CTC].st[a]=TF[CTC].st[a]+trd_s1;
          // ���室 �� ����� ������ 蠣�
          if (TF[CTC].st[a]>=trd_s2*trd_s1){ TF[CTC].st[a]=0; }
          if (TF[CTC].fl[a]>0){ TF[CTC].fl[a]=0; }
      }
      if (TF[CTC].fs[a]==0){ TF[CTC].st[a]=0; TF[CTC].fs[a]=1; }
      // �� ��ࢮ� ���室� � ०�� [A] - ����� ����� �� ��娢�
      if (TF[CTC].am[a]==1){
          TF[CTC].am[a]=0;
      };
      //---------------------------------------------------------------------


      //---------------------------------------------------------------------
      // ��㥬 ���⨪���� ����� �७���
      zz[1]=divf(LTRD[a].W2, LTRD[a].stx);
      for(b=0;b<=LTRD[a].stx;b++){
         line (scrsav, LTRD[a].X2+ LTRD[a].W2- zz[1]*b,
                       LTRD[a].Y2+ LTRD[a].H2+ 5,
                       LTRD[a].X2+ LTRD[a].W2- zz[1]*b,
                       LTRD[a].Y2,
                       LTRD[a].cs);
      }
      //---------------------------------------------------------------------
      // ������ �६������ �१� � ���ᨢ� ᮮ⢥��⢥��� ०���
      if (TF[CTC].lm[a]==1&&LTRD[a].rst==0){ 
          TF[CTC].lt[a]=TF[CTC].lt[a]+1; 
      }
      if (TF[CTC].lt[a]>LTRD[a].sec){ TF[CTC].lt[a]=LTRD[a].sec; }
      // �� ⥪�饣� ����� � ���ᨢ� �� �㫥���� (��� ������ ᥪ㭤�)
      for(b=MTRS,j=0,g2=trd_s2,g3=0; b> MTRS-LTRD[a].sec-2; b--){
          // ��㥬 ����� �६������ �१�
          if (TF[CTC].lt[a]==r){
              line (scrsav, 
                      LTRD[a].X2+ LTRD[a].W2- trd_s1*TF[CTC].lt[a],
                      LTRD[a].Y2+ LTRD[a].H2+ 5,
                      LTRD[a].X2+ LTRD[a].W2- trd_s1*TF[CTC].lt[a],
                      LTRD[a].Y2- h2*0.5,
                      LTRD[a].cr);
         }
         // ��।��塞 ����� ����� � ���ᨢ� VTRD ��� ⥪�饩 
         // ���⨪��쭮� ����� (�����)
         s_in_tr=MTRS-trd_s2*g3;
         // ��।��塞 ��ப��� ��� �६��� ��� ������ �����
         // ०�� [C]
         if (TF[CTC].aa[a]==0) {
            sprintf(mmss,"%02d:%02d:%02d",
                          VTRD[a].s[s_in_tr].h,
                          VTRD[a].s[s_in_tr].m,
                          VTRD[a].s[s_in_tr].s);
         // ०�� [A]
         } else {
            sprintf(mmss,"%02d:%02d:%02d",
                          VARC[a].s[s_in_tr].h,
                          VARC[a].s[s_in_tr].m,
                          VARC[a].s[s_in_tr].s);
         }
         // �뢥�� ���祭�� �६��� ��� ������ ���⨪��쭮� �����
         if (g2>=trd_s2){
             text_mode(LTRD[a].g1);
             textout  (scrsav,dFnt[LTRD[a].ft].dat,mmss,
                              LTRD[a].X2+LTRD[a].W2-zz[1]*g3,
                              LTRD[a].Y2+LTRD[a].H2+h1,
                              LTRD[a].cx);
             g2=0; g3++;
         };  g2++;
         //--------------------------------------
         // ��� ������� ������ �७��
         for(i=0,kx1=0.2,ky1=2.4;i<TF[CTC].gh[a];i++){
            // ��।������ 䫠�� � 梥� �ᮢ���� ��䨪�
            if (LTRD[a].c[i]>=1000){ trfl=1; cs=LTRD[a].c[i]-1000; }
            else                   { trfl=0; cs=LTRD[a].c[i];      }
            // ⥪�饥 ���祭�� �� ���ᨢ� VTRD[] �ਢ��� � 誠��
            if (TF[CTC].aa[a]==0){
                Y2[i]=divf(LTRD[a].H2,mdiv)*(VTRD[a].s[b].v[i]-LTRD[a].min);
                if(LTRD[a].z[i]==1){ Y2[i]=divf(LTRD[a].H2,mdiv)*(VTRD[a].s[b].v[i]+LTRD[a].v[i]-LTRD[a].min); }
                if(LTRD[a].z[i]==2){ Y2[i]=divf(LTRD[a].H2,mdiv)*(VTRD[a].s[b].v[i]-LTRD[a].v[i]-LTRD[a].min); }
                if(LTRD[a].z[i]==3){ Y2[i]=divf(LTRD[a].H2,mdiv)*(VTRD[a].s[b].v[i]*LTRD[a].v[i]-LTRD[a].min); }
            }   else  {
                Y2[i]=divf(LTRD[a].H2, mdiv)*(VARC[a].s[b].v[i]-LTRD[a].min);
                if(LTRD[a].z[i]==1){ Y2[i]=divf(LTRD[a].H2,mdiv)*(VARC[a].s[b].v[i]+LTRD[a].v[i]-LTRD[a].min); }
                if(LTRD[a].z[i]==2){ Y2[i]=divf(LTRD[a].H2,mdiv)*(VARC[a].s[b].v[i]-LTRD[a].v[i]-LTRD[a].min); }
                if(LTRD[a].z[i]==3){ Y2[i]=divf(LTRD[a].H2,mdiv)*(VARC[a].s[b].v[i]*LTRD[a].v[i]-LTRD[a].min); }
            }
            // �᫨ �� ����� �६������ �१� - ��������� ���祭�� ��䨪��
            // � ���ᨢ VTRT[]
            if (r==TF[CTC].lt[a]){
               // ������� ���祭�� ⥪�饣� ������ �� �६����� �१�
               if (TF[CTC].aa[a]==0){ VTRT[a*MTRG+i]=VTRD[a].s[b].v[i]; }
               else                 { VTRT[a*MTRG+i]=VARC[a].s[b].v[i]; }
//             // �८�ࠧ����� ���祭�� � ��⮬ ���४�஢��
//             if (LTRD[a].z[i]==1){ VTRT[a*MTRG+i]=VTRT[a*MTRG+i]+LTRD[a].v[i]; }
//             if (LTRD[a].z[i]==2){ VTRT[a*MTRG+i]=VTRT[a*MTRG+i]-LTRD[a].v[i]; }
//             if (LTRD[a].z[i]==3){ VTRT[a*MTRG+i]=VTRT[a*MTRG+i]*LTRD[a].v[i]; }
               // �뢥�� �६� �६������ �१�
               sprintf(TF[CTC].tm[a],"%02d:%02d:%02d",
                     ((TF[CTC].aa[a]==0)?VTRD[a].s[b].h:VARC[a].s[b].h),
                     ((TF[CTC].aa[a]==0)?VTRD[a].s[b].m:VARC[a].s[b].m),
                     ((TF[CTC].aa[a]==0)?VTRD[a].s[b].s:VARC[a].s[b].s));
//!!!
               text_mode(LTRD[a].g2);
               textout  (scrsav,dFnt[LTRD[a].ft].dat,TF[CTC].tm[a],
                                   LTRD[a].X2+0.0,LTRD[a].Y2+LTRD[a].H2+h1*2.45*1.1,LTRD[a].cr);
               // �뢥�� ���� ��娢���� 䠩��
               textout  (scrsav,dFnt[LTRD[a].ft].dat,TF[CTC].dt[a],
                                   LTRD[a].X2+vl*1.3,LTRD[a].Y2+LTRD[a].H2+h1*2.45*1.1,LTRD[a].cr);
               // ��������� ���न���� �뢮�� �६��� � ���� ��⨢���� �७��
               if (TF[CTC].an[a]==1){
                   GS.TTX =LTRD[a].X2+0;      GS.TTY =LTRD[a].Y2+LTRD[a].H2+h1*2.45*1.1;
                   GS.TDX =LTRD[a].X2+vl*1.3; GS.TDY =LTRD[a].Y2+LTRD[a].H2+h1*2.45*1.1;
               }
               // �뢥�� ���祭�� �६������ �१�
               if (LTRD[a].hm>0){
                  if (i==2){
                      if (TF[CTC].hm[a]>0){ XX=vl*2.85; }
                      else                { XX=0; }
                  }
                  if((i % LTRD[a].hm) > 0){
                      kx1=kx1+1.1;
                      tX1=vl*kx1;
                  }  else  {
                      if (TF[CTC].hm[a]==0){ ky1=ky1+1.5; }
                      kx1=0.2;
                      tX1=vl*kx1;  tY1=h1*ky1;
                  }
                  sprintf(try_max,SFRM_T,VTRT[a*MTRG+i]);
                  if(TF[CTC].ga[a][i]==0){
                     textout  (scrsav,dFnt[LTRD[a].ft].dat,try_max,
                                      LTRD[a].X2+XX+tX1,LTRD[a].Y2+LTRD[a].H2+tY1*1.1,cs);
                  }
               }
            }
            // �஢���� ��室 �� �࠭���
            if (Y2[i]>LTRD[a].H2){ Y2[i]=LTRD[a].H2; }
            if (Y2[i]<0         ){ Y2[i]=0; }
            // ��㥬 ��䨪
            if(TF[CTC].ga[a][i]==0&&r>0){
               // ⮫騭� �����
               if (LTRD[a].th>10) th=LTRD[a].th-10;
               else               th=LTRD[a].th;
               for(zz[0]=0;zz[0]< th;zz[0]++){
                   // ��㥬 ��䨪, ���� �� ��諨 �� �࠭��� ����
                   if (X2 <=LTRD[a].W2){
                   if(trfl==0){
                      // ��䨪 ����� �����
                          line(scrsav, 
                               LTRD[a].X2+ LTRD[a].W2- X1,
                               LTRD[a].Y2+ LTRD[a].H2- Y1[i]+zz[0],
                               LTRD[a].X2+ LTRD[a].W2- X2,
                               LTRD[a].Y2+ LTRD[a].H2- Y2[i]+zz[0], 
                               cs);
                       } else {
                          // ��䨪 ����� �����客����� ������
                          pnt[0]=LTRD[a].X2+LTRD[a].W2-X1;
                          pnt[1]=LTRD[a].Y2+LTRD[a].H2-Y1[i];
                          pnt[2]=LTRD[a].X2+LTRD[a].W2-X2+1;
                          pnt[3]=LTRD[a].Y2+LTRD[a].H2-Y2[i];
                          pnt[4]=LTRD[a].X2+LTRD[a].W2-X2+1;
                          pnt[5]=LTRD[a].Y2+LTRD[a].H2;
                          pnt[6]=LTRD[a].X2+LTRD[a].W2-X1;
                          pnt[7]=LTRD[a].Y2+LTRD[a].H2;
                          polygon(scrsav, 4, pnt, cs);
//                      //line(scrsav,pnt[0],pnt[1],pnt[2],pnt[3],cs);
//                      //line(scrsav,pnt[2],pnt[3],pnt[4],pnt[5],cs);
//                      //line(scrsav,pnt[4],pnt[5],pnt[6],pnt[7],cs);
//                      //line(scrsav,pnt[6],pnt[7],pnt[0],pnt[1],cs);
                       }
                   }
               }
            }
            // �뤥���� �� ��䨪� �६�, ����� ��娢 �� �����뢠���
            CMS1=(TF[CTC].aa[a]==0) ? VTRD[a].s[b].t : VARC[a].s[b].t;
            if (CMS1!=CMS2&&r>0&&r<=LTRD[a].sec){
                rectfill(scrsav,
                         LTRD[a].X2 + LTRD[a].W2 - X1,
                         LTRD[a].Y2 + LTRD[a].H2+ 3,
                         LTRD[a].X2 + LTRD[a].W2 - X1,
                         LTRD[a].Y2,
                         LTRD[a].cx);
            };  CMS2 =CMS1;
            // �� ��ࢮ� ��室� ��㥬 "��࠭���" �७��
            if (j==0){
               if (trkr==0){
                  if(TF[CTC].ga[a][i]==0){
                     circlefill(scrsav, LTRD[a].X2+ LTRD[a].W2+ 3,
                                        LTRD[a].Y2+ LTRD[a].H2- Y2[i], 2, cs);
                  }
               }
            }
            Y1[i]=Y2[i];
         }
         //--------------------------------------
         j=1;                          // 䫠� ��ࢮ�� 横�� ������� �७��
         X1=X2;                        // �����⠭�������� ���न����
         r++;                          // 㢥��稢��� ����� ᥪ㭤�  � �७��
         X2=trd_s1*r;                  // trd_s1-������⢮ ���� � 1 ᥪ㭤�
         // �᫨ �� ����� �७�� -��室��
         if (X2>LTRD[a].W2&&g3>LTRD[a].stx+1){ break; }
      }
      if (TF[CTC].fl[a]>0){ TF[CTC].fl[a]=0; }
      // �뢥�� �६� ��᫥���� ����� ��䨪�
      if (g3<=LTRD[a].stx){
          textout  (scrsav,dFnt[LTRD[a].ft].dat,mmss,
                           LTRD[a].X2+LTRD[a].W2-zz[1]*g3,
                           LTRD[a].Y2+LTRD[a].H2+h1,LTRD[a].cx);
      }
   }
}
//---------------------------------------------------------------------------
