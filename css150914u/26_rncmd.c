// coding: cp866
//----------------------------------------------------------------------------
//  ࠧ��� ���� �������
_i  FillCMD(_i HBR){
    _i  a=0, n=0, l=0, f=0, i=0, y=0, z=0;
    uc  c=0;
    sprintf(SMSG[MMSG],SMSG[88]);
    DPrn(9,0,"\n%s\n",SMSG[MMSG]);
    for (GStrPos=0;;){
       // ����� ������ ᫥������ ��ப� �� ����
       if(( _mfGetStr(FBuf,HBR))==0){ break; }
       DPrn(9,0,"(%s)\n",GStr);
       if(strlen(GStr)<4){ continue; }
       //----------------------------------------------------------------------
       // ��������� ���� �� ��ப�
       c=PutFields(0,GStr,' ',0);
       LCMD[HCMD].rcmd=0;
       LCMD[HCMD].tcmd=0;
       LCMD[HCMD].gcmd=HCMD+1;
       // ��ࠡ���� �� ����
       for(a=0,n=0,z=0;a<HowFLD;a++){
           // �᫨ ����⨫��� ����������� ����
           if(isFieldb(0,a,0)>0){
              // ������� ⥪�饥 ���� � ��६����� stmp
              strcpy(stmp,GetField(0,a));
              // �஢��塞 ���� ᨬ��� ⥪�饣� ����:
              //---------------------------------------------------------------
              // �᫨ �� �������਩
              if (stmp[0]=='#'){ break; }
              if (stmp[0]==';'){ break; }
              DPrn(9,0," [%s]",stmp);
              //---------------------------------------------------------------
              // �᫨ �� �뢮� �� �࠭
              if((stmp[0]=='O')||(stmp[0]=='o')){
                  // ��������� ��ப�, ����� �㤥� �뢮������ �� �࠭
                  stmp[0]=' '; strcpy(LCMD[HCMD].scmd,stmp);
                  // ����� ������� �뢮�� �� �࠭
                  LCMD[HCMD].rcmd=20;
                  continue;
              }
              //---------------------------------------------------------------
              // �᫨ �� ���� ���� ������ �ࠢ�����
              if(((stmp[0]=='E')||(stmp[0]=='e'))&&((stmp[1]=='L')||(stmp[1]=='l'))){
                  LCMD[HCMD].rcmd=55;
                  // ����� ⥪�饩 ��ப� �������� ������� � ���������� �����
                  LCMD[ WTIF[CCIF] ].gcmd=HCMD;
                  // ��������� ������ ⥪�騩 ����� ��ப� ��������
                  WTIF[CCIF]=HCMD;
              }
              // �᫨ �� ����砭�� ������ �ࠢ�����
              if(((stmp[0]=='F')||(stmp[0]=='f'))&&((stmp[1]=='I')||(stmp[1]=='i'))){
                  LCMD[HCMD].rcmd=56;
                  // ����� ⥪�饩 ��ப� �������� ������� � ���������� �����
                  LCMD[ WTIF[CCIF] ].gcmd=HCMD;
                  if(CCIF>0){ CCIF--; }
              }
              // �᫨ �� ������ �ࠢ�����
              if(((stmp[0]=='I')||(stmp[0]=='i'))&&((stmp[1]=='F')||(stmp[1]=='f'))){
                  LCMD[HCMD].tcmd=2;
                  if(CCIF<MGIF-1){ CCIF++; }
                  // ��������� ����� ⥪�饩 ��ப� ��������
                  WTIF[CCIF]=HCMD;
                  n++;
                  continue;
              }
              //---------------------------------------------------------------
              // �᫨ ��६����� ��।����� ������- ��।����� �� �����
              if (stmp[0]=='$'){
                  l=strlen(stmp);
                  // ��।������ ��ப� �� ᨬ��� �����
                  for(i=0; i<l-1; i++){ stmp[i]=stmp[i+1]; }; stmp[i]=0;
                  // ᮯ��⠢��� ����祭��� ��� � ������� � ��饬 ���ᨢ�
                  for(i=0; i<MCH; i++){
                      // �᫨ ��।������� ��� �뫮 �������- ᮧ���� ��ப�
                      // � ����஬ ������  ��� ��᫥���饩 ��ࠡ�⪨ (p0000)
                      if((strcasecmp(stmp, CB[i].nm))==0){
                          DPrn(9,0,"(%s=p%04d)",stmp,i);
                          sprintf(stmp,"p%04d",i); break;
                      }
                  }
                  // �᫨ ��६����� �� ����� �� �������- ������� ᮮ�饭��
                  if (i>=MCH){
                      sprintf(SMSG[MMSG],SMSG[179],stmp);
                      DPrn(9,0,"\n%s\n",SMSG[MMSG]);
                  }
              }
              //---------------------------------------------------------------
              // �᫨ ��६����� ��।����� ������, ��� 㪠��⥫� �� �����
              // ��६����� ���  ���祭��
              if (stmp[0]=='%'){
                  l=strlen(stmp);
                  // ��।������ ��ப� �� ᨬ��� �����
                  for(i=0; i<l-1; i++){ stmp[i]=stmp[i+1]; }
                  stmp[i]=0; f=0;
                  //-----------------------------------------------------------
                  // �᫨ �� ��ࢮ� ��।������ ����� ��६�����
                  if(z==0){
                     // �᪠�� 㪠������ ��� � ���ᨢ� ����
                     for(i=0; i<HCCM; i++){
                         // �᫨ ��諨 ⠪�� ���- ��३� �� ����⠭����
                         // ���  ���� ��६�����
                         if((strcasecmp(stmp, NCCM[i]))==0){ z=1; f=i; break; }
                     }
                     // �᫨ �� ��諨 ⠪�� ���
                     if (i>=HCCM && i<MCCM && n==0){
                         // �������� ��� ��६����� � ���ᨢ ����
                         strcpy(NCCM[i],stmp);
                         // �������� ��६����� ��� ���祭��, ᮮ⢥��� �����
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
                  // �᫨ �� ����⠭���� ��६����� ��� ���祭��-
                  // �᪠�� 㪠������ ��� � ���ᨢ� ����
                  if(z>0){
                     for(i=f; i<HCCM; i++){
                         // �᫨ ��諨 ⠪�� ���- ����⠢��� ᮮ���� ���祭��
                         if((strcasecmp(stmp, NCCM[i]))==0){
                             strcpy(stmp,PCCM[i]); break;
                         }
                     }
                     // �᫨ ��祣� �� ��諨 - ����⠢��� 0
                     if(i>=HCCM){ strcpy(stmp,"0"); }
                     DPrn(9,0,"PCCM[%d](stmp)=[%s]",i,stmp);
                  }
              }
              //---------------------------------------------------------------
              if((stmp[0]=='X')||(stmp[0]=='x')){
                  // ������� ����� ��६�����
                  stmp[0]=32; LCMD[HCMD].vcmd[n]=atoi(stmp);
                  // ������� ����� ���ᨢ�
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
              // ��⠭����� �ਧ���� ����⢨� ���᫥���
              if (stmp[0]=='='&& LCMD[HCMD].tcmd==0){ LCMD[HCMD].rcmd=1; continue; }
              if (stmp[0]=='+'){ LCMD[HCMD].rcmd=2; continue; }
              if (stmp[0]=='-'){ LCMD[HCMD].rcmd=3; continue; }
              if (stmp[0]=='*'){ LCMD[HCMD].rcmd=4; continue; }
              if (stmp[0]=='/'){ LCMD[HCMD].rcmd=5; continue; }
              // ��⠭����� �ਧ���� �ࠢ�����
              if (stmp[0]=='='&&LCMD[HCMD].tcmd==2){ LCMD[HCMD].rcmd=51; continue; }
              if (stmp[0]=='!'&&LCMD[HCMD].tcmd==2){ LCMD[HCMD].rcmd=52; continue; }
              if (stmp[0]=='>'&&LCMD[HCMD].tcmd==2){ LCMD[HCMD].rcmd=53; continue; }
              if (stmp[0]=='<'&&LCMD[HCMD].tcmd==2){ LCMD[HCMD].rcmd=54; continue; }
              // �᫨ ��諨 �� �⮣� ���� - ����� �� ����⠭�
              // ��⠭����� �ਧ��� ����⠭��
              if (stmp[0]>47&&stmp[0]<58){
                  LCMD[HCMD].ncmd[n]=8;
                  // ��������� ���祭��
                  LCMD[HCMD].zcmd[n]=atof(stmp); if(n<2){ n++; }
              }
           }
       }
       //----------------------------------------------------------------------
       // �᫨ �� ����� �������� � ��ப� � �뫠 ��।����� ��� ���� �������
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
//  �믮������ ������� �� �������� �������
_i  RunCMD(vd){
    _i  a=0, rel[MGIF], cur=0;
    _f  v[3];
    uc  s[MGIF+1];
        s[0]=' '; s[1]=0;
    for(a=0;a<HCMD;a++){
        if(LCMD[a].fcmd==1){
           if(strlen(s)<1){ s[0]=' '; s[1]=0; }
           // ���������� ����⠭��
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
           // ����樨 �믮������ � �㭪樨
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
           // ��⠭���� ���祭�� � ���ᨢ�� ��६�����
           if(LCMD[a].rcmd< 10){
              Val_to_Arr(LCMD[a].ncmd[0],LCMD[a].vcmd[0],v[0],CBMODYES,'c');
//            DPrn(9,0,"Val_to_Arr( na=%d , nv=%d , vl=%f )\n", LCMD[a].ncmd[0], LCMD[a].vcmd[0], v[0]);
              continue;
           }
           // �뢮� �� �࠭ (�� ���᮫�)
           if(LCMD[a].rcmd==20){
              strcpy(YCNV.str,LCMD[a].scmd); MESOut((MOUT*)&YCNV,4,0); // ��ࠡ�⪠ ��ப�...
              printf("%s",YCNV.str);
              continue;
           }
           // ����樨 �ࠢ����� (�஢��塞 ���⭮�)
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
           // �᫨ �।��騩 IF �믮����� (rel[cur]>0) � ���� ᥪ�� EL, �
           // �� �믮������ �ய�᪠��
           if(LCMD[a].rcmd==55){
              DPrn(9,0,"%sel%02d\n",s,cur);
              if(rel[cur]==0){ a=LCMD[a].gcmd-1; }
              continue;
           }
           // ����砭�� �������� IF (FI)
           if(LCMD[a].rcmd==56){
              DPrn(9,0,"%sfi%02d\n",s,cur);
              if(cur>0){ cur--; s[cur+1]=0; }
              continue;
           }
           //------------------------------------------------------------------
        }
    }

    //-------------------------------------------------------------------------
    // ��⠭���� ���� ����⮩ ������ �� ����������� 䠩��
    if(LKSET_CMD>0){ SCode=LASTKEY; LCode=LASTKEY; LKSET_CMD=0; }

    //-------------------------------------------------------------------------
    // �맮� 㪠������� 䠩�� ���ᠭ�� �� ����������� 䠩��
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
    // �뢮� ᮮ�饭�� (codus_al.ini) � ��ப� ����� �� ����������� 䠩��
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
