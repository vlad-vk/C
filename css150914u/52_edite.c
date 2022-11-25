// coding: cp866
//---------------------------------------------------------------------------
// ������஢���� �࠭�

//---------------------------------------------------------------------------
// ��ନ஢���� ����� ��ப�
_i nstrform(_i x, _i y, _i xv, _i yv){ 
   _i  a=0;
   sprintf(El_CSF[x],"%04d",xv);
   sprintf(El_CSF[y],"%04d",yv);
   // �᫨ �뫠 ��࠭� �������� ����⠭�������
   if(SCode==KEY_U){
      // �᫨ ����⠭�������� ��祣�- ��室��
      if(El_UND==0){ return(0); }
      El_CSF[0][0]=El_CCC[0]; El_CSF[0][1]=0;
   }
   // �᫨ �뫠 ��࠭� �������� 㤠����� �����- ��������஢��� ��ப�
   if(SCode==KEY_D){
      strcpy (El_CSO,El_CSS);          // ��������� ��ப� ��� ����⠭�������
      strcpy (El_CSS,";;");
   } else {
      strcpy (El_CSO,El_CSS);          // ��������� ��ப� ��� ����⠭�������
      strcpy (El_CSS,"  ");
   }
   // ��ନ஢��� ����� ��ப� ���ᠭ�� ����� �࠭�
   for(a=0;a<=El_FLD;a++){
      if(El_CSF[a][0]==0){ break; }
      strcat(El_CSS,El_CSF[a]); strcat(El_CSS," ");
   }; strcat(El_CSS,"\n"); 
   El_UND=1; 
   X__UnO=X__Old; Y__UnO=Y__Old; X__UnN=xv; Y__UnN=yv;  El_UTp=El_Typ;
   return(1);
}
//---------------------------------------------------------------------------
// �����஢���� ��ப� ��� ���᪠ ��ࠬ��஢
_i StrParScan(vd){
   _i r=0;
   // �᫨ ���� ���न���� ᮢ���� � ���묨 � �뫠 ����� ���� �� ������
   // ��।������ ����� ����樨 �����- ����� ��祣� �� �������- ��室��
   if(X__Old==X__New && Y__Old==Y__New && (SCode!=KEY_D && SCode!=KEY_U))
      return(0);;
   // ���� ���न���� �� ����� ���� �㫥�묨
   if(X__New==0 && Y__New==0) return(0);;
   // �᫨ ��諨 �㦭�� ��ப�- ���塞 � ��ப� ���न���� �� ����
   if(El_Typ== 1 && atoi(El_CSF[1])==X__Old && atoi(El_CSF[2])==Y__Old){ // LSEL
      nstrform(1,2,X__New,Y__New); r=1;
   }
   if(El_Typ== 2 && atoi(El_CSF[1])==X__Old && atoi(El_CSF[2])==Y__Old){ // LSPR
      nstrform(1,2,X__New,Y__New); r=1;
   }
   if(El_Typ== 3 && atoi(El_CSF[2])==X__Old && atoi(El_CSF[3])==Y__Old){ // LSTR
      nstrform(2,3,X__New,Y__New); r=1;
   }
   if(El_Typ== 4 && atoi(El_CSF[7])==X__Old && atoi(El_CSF[8])==Y__Old){ // LVAL
      nstrform(7,8,X__New,Y__New); r=1;
   }
   if(El_Typ== 5 && atoi(El_CSF[7])==X__Old && atoi(El_CSF[8])==Y__Old){ // LGET
      nstrform(7,8,X__New,Y__New); r=1;
   }
   if(El_Typ== 6 && atoi(El_CSF[6])==X__Old && atoi(El_CSF[7])==Y__Old){ // LGST
      nstrform(6,7,X__New,Y__New); r=1;
   }
   if(El_Typ== 7 && atoi(El_CSF[2])==X__Old && atoi(El_CSF[3])==Y__Old){ // LWIN
      nstrform(2,3,X__New,Y__New); r=1;
   }
   if(El_Typ== 8 && atoi(El_CSF[2])==X__Old && atoi(El_CSF[3])==Y__Old){ // LMNU
      nstrform(2,3,X__New,Y__New); r=1;
   }
   if(El_Typ== 9 && atoi(El_CSF[2])==X__Old && atoi(El_CSF[3])==Y__Old){ // LBOX
      nstrform(2,3,X__New,Y__New); r=1;
   }
   if(El_Typ==10 && atoi(El_CSF[1])==X__Old && atoi(El_CSF[2])==Y__Old){ // LSHK
      nstrform(1,2,X__New,Y__New); r=1;
   }
   if(El_Typ==11 && atoi(El_CSF[4])==X__Old && atoi(El_CSF[5])==Y__Old){ // LSLM
      nstrform(4,5,X__New,Y__New); r=1;
   }
   if(El_Typ==12 && atoi(El_CSF[5])==X__Old && atoi(El_CSF[6])==Y__Old){ // LCST
      nstrform(5,6,X__New,Y__New); r=1;
   }
   if(El_Typ==13 && atoi(El_CSF[8])==X__Old && atoi(El_CSF[9])==Y__Old){ // LANI
      nstrform( 8, 9,X__New,Y__New);
      nstrform(10,11,atoi(El_CSF[10])+(X__New-X__Old),
                     atoi(El_CSF[11])+(Y__New-Y__Old)); r=1;
   }

// DPrn(0,0,"SC=%d El_Typ=%d  X=[%d]?[%d] Y=[%d]?[%d]\n",
//         SCode,El_Typ,atoi(El_CSF[2]),X__Old,atoi(El_CSF[3]),Y__Old);
   return(r);
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ��१����� 䠩��� ���䨣��樨 � ���묨 ���न��⠬�
vd RewriteINI(){
   _i  fd=0;                                // ��⥬�� ����� 䠩��
   ul  hb=0, a=0;                           // ���稪 ��⠭��� ����
   ul  LLF1=0, LLF2=0;                      // ������ � ���� ��᫥����� LF
   _i  lf=0;                                // 䫠� ��ॢ��� ��ப�
   _i  b=0,c=0,z=0;                         // ���稪 ����樨 � ���� ��ப�
   _i  s=0;                                 // �ਧ��� �஡��� � ��ப�
   _i  next=0;                              // 䫠� ���室� � ᫥���饩 ��ப�
   _i  cyc=0;                               // ���稪 横���
   _i  mod=0;                               // �ਧ��� ����䨪�樨
   El_CCC[0]='�';
   if(El_Typ== 1){ El_CCC[0]='O'; }
   if(El_Typ== 2){ El_CCC[0]='P'; }
   if(El_Typ== 3){ El_CCC[0]='s'; }
   if(El_Typ== 4){ El_CCC[0]='v'; }
   if(El_Typ== 5){ El_CCC[0]='G'; }
   if(El_Typ== 6){ El_CCC[0]='g'; }
   if(El_Typ== 7){ El_CCC[0]='w'; }
   if(El_Typ== 8){ El_CCC[0]='m'; }
   if(El_Typ== 9){ El_CCC[0]='b'; }
   if(El_Typ==10){ El_CCC[0]='h'; }
   if(El_Typ==11){ El_CCC[0]='Q'; }
   if(El_Typ==12){ El_CCC[0]='C'; }
   if(El_Typ==13){ El_CCC[0]='a'; }
   El_cii=0;
   // ������ ���� � ������ ����砥��� �� �믮������ ini-䠩��
   for(cyc=0;cyc<El_hii;cyc++){
       if((fd=open(El_ifn[cyc],vktRDO))>0){
           hb=read (fd,&FBuf,SBuf);
           close(fd);
       }
       El_FLD=0; b=0;  c=0; LLF1=0; LLF2=0; lf=1; s=0; next=0;
       for(z =0; z<MXSFLD;  z++){ El_CSF[z][0]=0; }
       for(a =0; a<hb; a++){
           El_CSS[b]=FBuf[a]; El_CSS[b+1]=0; if(b<SLEN*2-2){ b++; }
           // �᫨ �� ��ॢ�� ��ப�- ��������� ������
           if(FBuf[a]==10||FBuf[a]==0){ 
              LLF1=LLF2; LLF2=a+1; 
              if(lf==2){ 
                // �᫨ ��ப� �뫠 ��������- ��१������ ⥪�騩 䠩� ���ᠭ��
                if(StrParScan()>0){
                  if((fd=open(El_ifn[cyc],vktWRT,S_IREAD|S_IWRITE))>0){
                     write(fd,&FBuf,LLF1);             // ���� 䠩�� �� ��ப�
                     write(fd,&El_CSS,strlen(El_CSS)); // ��ப� ���ᠭ��
                     write(fd,&FBuf[LLF2],hb-LLF2);    // ���� ��᫥ ��ப�
                      read(fd,&FBuf,SBuf);
                     close(fd); a=LLF1+strlen(El_CSS); mod++;
                  }
                }
              }
              for(z =0; z<MXSFLD; z++){ El_CSF[z][0]=0; }
              El_FLD=0; b=0; c=0; lf=1; s=0; next=0; continue;;
           }
           // ��砫�� �஡��� �� ���뢠��
           if(lf==1&&FBuf[a]<=32){ continue; }
           // �ࠢ���騥 ᨬ���� �� ���뢠��
           if(FBuf[a]<32){ continue; }
           // �ய����� �஡��� ����� ���ﬨ ࠧ��ࠥ��� ��ப(�뤥���� ����)
           if(lf >1&&FBuf[a]<=32&&s==1){ continue; }
           else                        { s=0;      }
           // �᫨ ��⠭����� 䫠� ᫥� ��ப�- �ய����� �� ᨬ���� �� � ��ப�
           if(next==1){ continue; }
           // �᫨ �� �᪮��� �������� ��� ����⠭�������(㤠������)
           if(lf==1 && FBuf[a]==';' && FBuf[a+1]==';' && FBuf[a+2]==El_CCC[0]
              && SCode==KEY_U){ lf=2; }
           // �᫨ �� �᪮��� ��������- ��⠭����� 䫠� lf
           if(lf==1 && FBuf[a]==El_CCC[0]){ lf=2; }
           // �᫨ �� ��㣠� ��������- ��⠭����� 䫠� ᫥���饩 ��ப�
           if(lf==1 && FBuf[a]!=El_CCC[0]){ next=1; continue; }
           // �᫨ ����⨫�� �஡��- ��३� � ᫥���饬� ����
           if(lf >1 && FBuf[a]<=32 && s==0){ 
              if(El_FLD<MXSFLD-1) El_FLD++;; s=1; c=0; continue; 
           }
           // ������ ⥪�饣� ���� ��ப�
           El_CSF[El_FLD][c+0]=FBuf[a]; 
           El_CSF[El_FLD][c+1]=0; 
           El_CSF[El_FLD+1][0]=0; 
           if(c<SLEN-2){ c++; }
       }
       // �⮡� ��砩�� �� ��横�����
       if(mod>50){ DPrn(0,0,"RewriteINI: mod>50 SCode=%d\n",SCode); break; }
   }
   // ��।����� ���� ini-䠩�, ��� ⥪�騩 䠩� ���ᠭ�� �
   // ��⠭����� 䫠� �⥭�� � ����㧪� 䠩�� ���ᠭ��(��ॣ�㧨�� �࠭)
   if(mod>0){ strcpy(FNCfg,FirstINI); FNCfgLD=1; }
   return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ������� ��� ��६����� �� ��饣� ���ᨢ�
vd GetNVName(_i na, _i nv){
   strcpy(El_Nam," ");
   strcpy(El_Str," ");
   if(na!=0){ return; }
   if(nv< 0 || nv>=MCH){ return; }
   strcpy(El_Nam,CB[nv].nm);
   strcpy(El_Str,CB[nv].rm);
   return;
}
//---------------------------------------------------------------------------
// ����祭�� ��ப� ����⢨� ��� ������
vd GetKeyDat(vd){
   _i  a=0, b=0, i=0;
   uc  z=0;
   // �஢���� �� ���ᠭ�� ������
   for(a=0; a<HKBD; a++){
       // �᫨ ������ ᮢ�����騩 ᪠�-���
       if(LKBD[a].cod==El_Key){
          strcpy(El_Str," ");
          strcpy(El_SS1," ");
          // ������� ��ப� � �������� ����⢨� ��� ������� ᪠�-����
          sprintf(El_Nam,"[%s][%c]",LKBD[a].str,LKBD[a].run);
          //------------------------------------

          //------------------------------------
          // �᫨ �������� ����⢨� ����㧪� ini-䠩��- ������� ��� ���, �����
          if(LKBD[a].run=='i'){
             // ��������� ����� ini-䠩��
             i=atoi(LKBD[a].str); El_SI1=i;
             // ���� ��� ini 䠩�� �� ������
             for(b=0; b<MINI; b++){
                 if(LINI[b].nn==i){ strcpy(El_SS1,LINI[b].fn); break; }
             }
          }
          //------------------------------------
          if(LKBD[a].run=='c'){
             i=atoi(LKBD[a].str); El_SI1=i; z='N'; strcpy(El_Str," ");
             if(i>=0 && i<MXCV){
                if(LXCV[i].zn==0) z='=';
                if(LXCV[i].zn==1) z='+';
                if(LXCV[i].zn==2) z='-';
                if(LXCV[i].zn==3) z='*';
                sprintf(El_SS1,"na=%d nv=%d  '%s'  zn='%c'  vc=%f\n",
                        LXCV[i].na,LXCV[i].nv,
                      ((LXCV[i].na==0)?CB[LXCV[i].nv].nm:NULL),
                        z,LXCV[i].vc);
                if(LXCV[i].na==0){ strcpy(El_Str,CB[LXCV[i].nv].rm); }
             }
          }
          //------------------------------------
          if(LKBD[a].run=='W'){
             i=atoi(LKBD[a].str); El_SI1=i;
             if(i>=0 && i<MSTT){
                sprintf(El_SS1,"kb=%d  ES=[%s]\n", LSTT[i].kb,LSTT[i].ES);
             }
          }
          //------------------------------------
          break;
       }
   }
   return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// �������� ���祭�� ���न���� X � Y ��� ��������� ⨯� ���ᨢ�
_i GetXYNW(_i na, _i  p){
   if(El_Typ== 1){ Y__New=LSEL[El_Num].Y; X__New=LSEL[El_Num].X; }
   if(El_Typ== 2){ Y__New=LSPR[El_Num].Y; X__New=LSPR[El_Num].X; }
   if(El_Typ== 3){ Y__New=LSTR[El_Num].Y; X__New=LSTR[El_Num].X; }
   if(El_Typ== 4){ Y__New=LVAL[El_Num].Y; X__New=LVAL[El_Num].X; }
   if(El_Typ== 5){ Y__New=LGET[El_Num].Y; X__New=LGET[El_Num].X; }
   if(El_Typ== 6){ Y__New=LGST[El_Num].Y; X__New=LGST[El_Num].X; }
   if(El_Typ== 7){ Y__New=LWIN[El_Num].Y; X__New=LWIN[El_Num].X; }
   if(El_Typ== 8){ Y__New=LMNU[El_Num].Y; X__New=LMNU[El_Num].X; }
   if(El_Typ== 9){ Y__New=LBOX[El_Num].Y; X__New=LBOX[El_Num].X; }
   if(El_Typ==10){ Y__New=LSHK[El_Num].Y; X__New=LSHK[El_Num].X; }
   if(El_Typ==11){ Y__New=LSLM[El_Num].Y; X__New=LSLM[El_Num].X; }
   if(El_Typ==12){ Y__New=LCST[El_Num].Y; X__New=LCST[El_Num].X; }
   if(El_Typ==13){ Y__New=LANI[El_Num].Y; X__New=LANI[El_Num].X; }
   if(p==0) return(X__New);
            return(Y__New);
}
//---------------------------------------------------------------------------
// ��⠭����� ���祭�� ���न���� X � Y ��� ��������� ⨯� ���ᨢ�
vd SetXYNW(_i na, _i x, _i y){
   if(El_Typ== 1){ LSEL[El_Num].Y=y; LSEL[El_Num].X=x; }
   if(El_Typ== 2){ LSPR[El_Num].Y=y; LSPR[El_Num].X=x; }
   if(El_Typ== 3){ LSTR[El_Num].Y=y; LSTR[El_Num].X=x; }
   if(El_Typ== 4){ LVAL[El_Num].Y=y; LVAL[El_Num].X=x; }
   if(El_Typ== 5){ LGET[El_Num].Y=y; LGET[El_Num].X=x; }
   if(El_Typ== 6){ LGST[El_Num].Y=y; LGST[El_Num].X=x; }
   if(El_Typ== 7){ LWIN[El_Num].Y=y; LWIN[El_Num].X=x; }
   if(El_Typ== 8){ LMNU[El_Num].Y=y; LMNU[El_Num].X=x; }
   if(El_Typ== 9){ LBOX[El_Num].Y=y; LBOX[El_Num].X=x; }
   if(El_Typ==10){ LSHK[El_Num].Y=y; LSHK[El_Num].X=x; }
   if(El_Typ==11){ LSLM[El_Num].Y=y; LSLM[El_Num].X=x; }
   if(El_Typ==12){ LCST[El_Num].Y=y; LCST[El_Num].X=x; }
   if(El_Typ==13){ LANI[El_Num].Y=y; LANI[El_Num].X=x; }
   return;
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// ��८�।������ ���न��� ��࠭���� �����
vd GetXYEL(_i mode){
   _i  mx=13;
   _i   a=0,  q=0, w=0, h=0, f=0, z=0, d=Debug(95);
   //---------------------------------------------------
   // �᫨ �� �த������� �।��饣� 蠣�- ��஢��� ����⨥ Shift+Z
   if (El_Que>=100){
       El_Que=1; El_Cel=0; mode=1; KSH=1; XYMode=0; SCode=KEY_Z;
   }
   //---------------------------------------------------
   // Shift+B - ��⠭����/��⨥ 䫠�� �뢮�� ࠬ�� �����
   if (mode==1 && KSH>0 && SCode==KEY_B){ 
       El_Brd=(El_Brd>0)?0:1; XYMode=0; SCode=KEY_Z;
   }
   //---------------------------------------------------
   // Shift+V - ��⠭����/��⨥ 䫠�� �뢮�� ࠬ�� ��� ����⮢ �࠭�
   if (mode==1 && KSH>0 && SCode==KEY_V && d>0){ 
       El_BAL=(El_BAL>0)?0:1; XYMode=0; SCode=KEY_Z;
   }
   //---------------------------------------------------
   // Shift+C - ����� ����������� ��������� ⥪�饣� �����
   if (mode==1 && ((KSH>0 && SCode==KEY_C)||(SCode==KEY_ESC)) ){ 
       El_Que=0; El_Cel=0; XYMode=0; SCode=0; return; 
   }
   //---------------------------------------------------
   // Shift+X - ��३� � ��㣮�� ������ ��� ��� ���न���
   if((mode==1 && KSH>0 && SCode==KEY_X)||(El_Que>mx)){
       if(XYMode==0) El_Que=0;;
       mode =1; El_Cel++; XYMode=0; SCode=KEY_Z;
   }
   //---------------------------------------------------
   // Shift+D - 㤠����� ����� � �࠭� (��᫥ 㤠����� ���室 � ᫥� �)
   if (mode==1 && KSH>0 &&  SCode==KEY_D && XYMode>0 && d>0){
       X__New=X__Old; Y__New=Y__Old;
       WndCmd(4); XYMode=0; RewriteINI(); SCode= KEY_Z; mode=1; El_Cel++;
   }
   //---------------------------------------------------
   // Shift+U - ����⠭������� ����������� ����� �� �࠭�
   if (mode==1 && KSH>0 &&  SCode==KEY_U && XYMode>0 && d>0){
       if(El_UND>0){
          X__Old=X__UnN; Y__Old=Y__UnN; X__New=X__UnO; Y__New=Y__UnO;
          position_mouse(X__New,Y__New);
          XYMode=0; El_Typ=El_UTp; RewriteINI(); El_Que=100;
          return;
       }
   }
   //---------------------------------------------------
   // Shift+UP,DOWN,LEFT,RIGHT- ��⠭���� ����� ���न��� ⥪�饣� �����
   if (mode==1 && KSH>0 && XYMode>0 && d>0 &&
      (SCode==KEY_UP||SCode==KEY_DOWN||SCode==KEY_LEFT||SCode==KEY_RIGHT)){ 
          GetXYNW(El_Typ,0); 
       if(SCode==KEY_UP   ){
          if(Y__New>     1) Y__New--;
       }
       if(SCode==KEY_DOWN ){ 
          if(Y__New<YMAX-1) Y__New++; 
       }
       if(SCode==KEY_LEFT ){ 
          if(X__New>     1) X__New--;
       }
       if(SCode==KEY_RIGHT){ 
          if(X__New<XMAX-1) X__New++;
       }
       SetXYNW(El_Typ,X__New,Y__New);
       position_mouse(X__New,Y__New);
       WndCmd(4); XYMode=0; RewriteINI(); SCode=KEY_Z;
   } 
   //---------------------------------------------------
   // Shift+N - ��⠭���� ����� ���न��� ⥪�饣� �����
   if(mode==1 && KSH>0 && SCode==KEY_N && d>0){
      if(XYMode>0){
         if(El_Typ== 1){ LSEL[El_Num].X=mouse_x; LSEL[El_Num].Y=mouse_y; f=1; }
         if(El_Typ== 2){ LSPR[El_Num].X=mouse_x; LSPR[El_Num].Y=mouse_y; f=1; }
         if(El_Typ== 3){ LSTR[El_Num].X=mouse_x; LSTR[El_Num].Y=mouse_y; f=1; }
         if(El_Typ== 4){ LVAL[El_Num].X=mouse_x; LVAL[El_Num].Y=mouse_y; f=1; }
         if(El_Typ== 5){ LGET[El_Num].X=mouse_x; LGET[El_Num].Y=mouse_y; f=1; }
         if(El_Typ== 6){ LGST[El_Num].X=mouse_x; LGST[El_Num].Y=mouse_y; f=1; }
         if(El_Typ== 7){ LWIN[El_Num].X=mouse_x; LWIN[El_Num].Y=mouse_y; f=1; }
         if(El_Typ== 8){ LMNU[El_Num].X=mouse_x; LMNU[El_Num].Y=mouse_y; f=1; }
         if(El_Typ== 9){ LBOX[El_Num].X=mouse_x; LBOX[El_Num].Y=mouse_y; f=1; }
         if(El_Typ==10){ LSHK[El_Num].X=mouse_x; LSHK[El_Num].Y=mouse_y; f=1; }
         if(El_Typ==11){ LSLM[El_Num].X=mouse_x; LSLM[El_Num].Y=mouse_y; f=1; }
         if(El_Typ==12){ LCST[El_Num].X=mouse_x; LCST[El_Num].Y=mouse_y; f=1; }
         if(El_Typ==13){ LANI[El_Num].X=mouse_x; LANI[El_Num].Y=mouse_y; f=1; }
         X__New=mouse_x; Y__New=mouse_y;
         WndCmd(4); XYMode=0; if(f>0) RewriteINI();; SCode=KEY_Z;
      }
   }
   //---------------------------------------------------
   // Shift+Z - ��।������ 㪠������� ����� � ��� ���न���
   if(mode==1 && KSH>0 && SCode==KEY_Z){
      //------------------------------------------------
      if(XYMode==0){
         if(El_Que==0){ El_Que=1; El_Cel=0; }
         // LSEL (������)
         if(El_Que==1&&XYMode==0){
            for(a=El_Cel; a<HSEL; a++){
                if(LSEL[a].k>0 &&
                   mouse_x>=LSEL[a].X && mouse_x<=(LSEL[a].X+LSEL[a].W) &&
                   mouse_y>=LSEL[a].Y && mouse_y<=(LSEL[a].Y+LSEL[a].H)){
                   X__Old=LSEL[a].X;     Y__Old=LSEL[a].Y;
                   W__Old=LSEL[a].W;     H__Old=LSEL[a].H; 
                   XYMode=1; El_Typ=1; El_Que=1; El_Num=a;
                   El_Key=LSEL[a].k; GetKeyDat();
                   sprintf(EDTS1,"Cmd=[O]%d LSEL[%d] [X=%d Y=%d] W=%d(%d) H=%d(%d)",
                           HSEL,El_Num, X__Old, Y__Old, W__Old,X2_Old,H__Old,Y2_Old);
                   sprintf(EDTS2,"key=%03d str=%s  >>%s",El_Key, El_Nam,El_Str);
                   sprintf(EDTS3,"run[%d]=[%s]",El_SI1, El_SS1);
                   break;
                }
                // �᫨ ����� �� ��諨- ��३� � ᫥���饬�
            };  El_Cel=a; if(a>=HSEL){ El_Que=2; El_Cel=0; }  
         }
         // LSPR (�ࠩ��)
         if(El_Que==2&&XYMode==0){
            for(a=El_Cel; a<HSPR; a++){
                if(LSPR[a].vw>0 &&
                   mouse_x>=LSPR[a].X && mouse_x<=(LSPR[a].X+LSPR[a].W) &&
                   mouse_y>=LSPR[a].Y && mouse_y<=(LSPR[a].Y+LSPR[a].H)){
                   X__Old=LSPR[a].X;     Y__Old=LSPR[a].Y;
                   W__Old=LSPR[a].W;     H__Old=LSPR[a].H; 
                   XYMode=2; El_Typ=2; El_Que=2; El_Num=a;
                   sprintf(EDTS1,"Cmd=[P]%d LSPR[%d] [X=%d Y=%d] W=%d(%d) H=%d(%d)",
                           HSPR,El_Num, X__Old, Y__Old, W__Old,X2_Old,H__Old,Y2_Old);
                   sprintf(EDTS2," ");
                   sprintf(EDTS3," ");
                   break;
                }
            };  El_Cel=a; if(a>=HSPR){ El_Que=3; El_Cel=0; }
         }
         // LSTR (��ப�)
         if(El_Que==3&&XYMode==0){
            for(a=El_Cel; a<HSTRC; a++){
                h=text_height(dFnt[LSTR[a].sn].dat);
                w=text_length(dFnt[LSTR[a].sn].dat,LSTR[a].str);
                if(LSTR[a].str[0]>0 && 
                   mouse_x>=LSTR[a].X && mouse_x<=(LSTR[a].X+w) &&
                   mouse_y>=LSTR[a].Y && mouse_y<=(LSTR[a].Y+h)){
                   X__Old=LSTR[a].X;     Y__Old=LSTR[a].Y;
                   H__Old=h;             W__Old=w;
                   XYMode=3; El_Typ=3; El_Que=3; El_Num=a;
                   sprintf(EDTS1,"Cmd=[s]%d LSTR[%d] [X=%d Y=%d] W=%d(%d) H=%d(%d)",
                           HSTRC,El_Num, X__Old, Y__Old, W__Old,X2_Old,H__Old,Y2_Old);
                   sprintf(EDTS2," ");
                   sprintf(EDTS3," ");
                   break;
                }
            };  El_Cel=a; if(a>=HSTRC){ El_Que=4; El_Cel=0; }
         }
         // LVAL (���祭��)
         if(El_Que==4&&XYMode==0){
            for(a=El_Cel; a<HVAL; a++){
                h=text_height(dFnt[LVAL[a].sn].dat);
                if(LVAL[a].nv>0 &&
                   mouse_x>=LVAL[a].X && mouse_x<=(LVAL[a].X+LVAL[a].W) &&
                   mouse_y>=LVAL[a].Y && mouse_y<=(LVAL[a].Y+h)){
                   XYMode=4; El_Typ=4; El_Que=4; El_Num=a;
                   El_NA=LVAL[a].na;     El_NV=LVAL[a].nv; 
                   X__Old=LVAL[a].X;     Y__Old=LVAL[a].Y;
                   H__Old=h;             W__Old=LVAL[a].W; 
                   GetNVName(El_NA,El_NV);
                   El_VL=Arr_to_Val(El_NA,El_NV);
                   sprintf(EDTS1,"Cmd=[v]%d LVAL[%d] [X=%d Y=%d] W=%d(%d) H=%d(%d)",
                           HVAL,El_Num,X__Old,Y__Old, W__Old,X2_Old,H__Old,Y2_Old); 
                   sprintf(EDTS2,"na=%d nv=%d vl=%f [%s]  >>%s", El_NA,El_NV,El_VL,El_Nam,El_Str);
                   sprintf(EDTS3," ");
                   break;
                }
            };  El_Cel=a; if(a>=HVAL){ El_Que=5; El_Cel=0; }
         }
         // LGET (���� ���祭��)
         if(El_Que==5&&XYMode==0){
            for(a=El_Cel; a<HGET; a++){
                h=text_height(dFnt[LGET[a].sn].dat);
                if(LGET[a].nv>0 &&
                   mouse_x>=LGET[a].X && mouse_x<=(LGET[a].X+LGET[a].W) &&
                   mouse_y>=LGET[a].Y && mouse_y<=(LGET[a].Y+h)){
                   XYMode=5; El_Typ=5; El_Que=5; El_Num=a;
                   El_NA=LGET[a].na;   El_NV =LGET[a].nv; 
                   X__Old=LGET[a].X;   Y__Old=LGET[a].Y;
                   H__Old=h;           W__Old=LGET[a].W; 
                   GetNVName(El_NA,El_NV);
                   El_VL=Arr_to_Val(El_NA,El_NV);
                   sprintf(EDTS1,"Cmd=[G]%d LGET[%d] [X=%d Y=%d] W=%d(%d) H=%d(%d)",
                           HGET,El_Num,X__Old,Y__Old, W__Old,X2_Old,H__Old,Y2_Old); 
                   sprintf(EDTS2,"na=%d nv=%d vl=%f [%s]  >>%s",El_NA,El_NV,El_VL,El_Nam,El_Str);
                   sprintf(EDTS3," ");
                   break;
                }
            };  El_Cel=a; if(a>=HGET){ El_Que=6; El_Cel=0; }
         }
         // LGST (���⮣ࠬ��)
         if(El_Que==6&&XYMode==0){
            for(a=El_Cel; a<HGST; a++){
                if(LGST[a].nv>0 &&
                   mouse_x>=LGST[a].X && mouse_x<=(LGST[a].X+LGST[a].W) &&
                   mouse_y>=LGST[a].Y && mouse_y<=(LGST[a].Y+LGST[a].H)){
                   X__Old=LGST[a].X;     Y__Old=LGST[a].Y;
                   W__Old=LGST[a].W;     H__Old=LGST[a].H; 
                   El_NA =LGST[a].na;    El_NV =LGST[a].nv; 
                   GetNVName(El_NA,El_NV);
                   El_VL=Arr_to_Val(El_NA,El_NV);
                   XYMode=6; El_Typ=6; El_Que=6; El_Num=a;
                   sprintf(EDTS1,"Cmd=[g]%d LGST[%d] [X=%d Y=%d] W=%d(%d) H=%d(%d)",
                           HGST,El_Num, X__Old, Y__Old, W__Old,X2_Old,H__Old,Y2_Old); 
                   sprintf(EDTS2,"na=%d nv=%d vl=%f [%s]  >>%s",El_NA,El_NV,El_VL,El_Nam,El_Str);
                   sprintf(EDTS3," ");
                   break;
                }
            };  El_Cel=a; if(a>=HGST){ El_Que=7; El_Cel=0; }
         }
         // LWIN (����)
         if(El_Que==7&&XYMode==0){
            for(a=El_Cel; a<HWIN; a++){
                if(LWIN[a].vw>0 &&
                   mouse_x>=LWIN[a].X && mouse_x<=(LWIN[a].X+LWIN[a].W) &&
                   mouse_y>=LWIN[a].Y && mouse_y<=(LWIN[a].Y+LWIN[a].H)){
                   X__Old=LWIN[a].X;     Y__Old=LWIN[a].Y; 
                   W__Old=LWIN[a].W;     H__Old=LWIN[a].H; 
                   XYMode=7; El_Typ=7; El_Que=7; El_Num=a; 
                   sprintf(El_Nam,"%s",LWIN[a].fn);
                   sprintf(EDTS1,"Cmd=[w]%d LWIN[%d] [X=%d Y=%d] W=%d(%d) H=%d(%d)",
                           HWIN,El_Num, X__Old, Y__Old, W__Old,X2_Old,H__Old,Y2_Old);
                   sprintf(EDTS2,"File=[%s]", El_Nam);
                   sprintf(EDTS3," ");
                   break;
                }
            };  El_Cel=a; if(a>=HWIN){ El_Que=8; El_Cel=0; }
         }
         // LMNU (����)
         if(El_Que==8&&XYMode==0){
            for(a=El_Cel; a<HMNU; a++){
                if(LMNU[a].vw>0 &&
                   mouse_x>=LMNU[a].X && mouse_x<=(LMNU[a].X+LMNU[a].W) &&
                   mouse_y>=LMNU[a].Y && mouse_y<=(LMNU[a].Y+LMNU[a].H)){
                   X__Old=LMNU[a].X;     Y__Old=LMNU[a].Y;
                   W__Old=LMNU[a].W;     H__Old=LMNU[a].H; 
                   XYMode=8; El_Typ=8; El_Que=8; El_Num=a;
                   El_Key=LMNU[a].k; GetKeyDat();
                   sprintf(EDTS1,"Cmd=[m]%d LMNU[%d] [X=%d Y=%d] W=%d(%d) H=%d(%d)",
                           HMNU,El_Num, X__Old, Y__Old, W__Old,X2_Old,H__Old,Y2_Old);
                   sprintf(EDTS2,"key=%03d str=%s  >>%s", El_Key,El_Nam,El_Str);
                   sprintf(EDTS3,"[%s]", El_SS1);
                   break;
                }
                // �᫨ ����� �� ��諨- ��३� � ᫥���饬�
            };  El_Cel=a; if(a>=HMNU){ El_Que=9; El_Cel=0; }  
         }
         // LBOX (����᪨� �ਬ�⨢)
         if(El_Que==9&&XYMode==0){
            for(a=El_Cel; a<HBOX; a++){
                if(LBOX[a].vw>0 &&
                   mouse_x>=LBOX[a].X && mouse_x<=(LBOX[a].X+LBOX[a].W) &&
                   mouse_y>=LBOX[a].Y && mouse_y<=(LBOX[a].Y+LBOX[a].H)){
                   X__Old=LBOX[a].X;     Y__Old=LBOX[a].Y;
                   W__Old=LBOX[a].W;     H__Old=LBOX[a].H; 
                   XYMode=9; El_Typ=9; El_Que=9; El_Num=a; El_SI1=LBOX[a].n;
                   sprintf(EDTS1,"Cmd=[b]%d LBOX[%d] [X=%d Y=%d] W=%d(%d) H=%d(%d)",
                           HBOX,El_Num, X__Old, Y__Old, W__Old,X2_Old,H__Old,Y2_Old);
                   sprintf(EDTS2,"n=%d",El_SI1);
                   sprintf(EDTS3," ");
                   break;
                }
                // �᫨ ����� �� ��諨- ��३� � ᫥���饬�
            };  El_Cel=a; if(a>=HBOX){ El_Que=10; El_Cel=0; }  
         }
         // LSHK (誠��)
         if(El_Que==10&&XYMode==0){
            for(a=El_Cel; a<HSHK; a++){
                if(mouse_x>=LSHK[a].X && mouse_x<=(LSHK[a].X+LSHK[a].W) &&
                   mouse_y>=LSHK[a].Y && mouse_y<=(LSHK[a].Y+LSHK[a].H)){
                   X__Old=LSHK[a].X;     Y__Old=LSHK[a].Y;
                   W__Old=LSHK[a].W;     H__Old=LSHK[a].H; 
                   XYMode=10;El_Typ=10;El_Que=10;El_Num=a;
                   sprintf(EDTS1,"Cmd=[h]%d LSHK[%d] [X=%d Y=%d] W=%d(%d) H=%d(%d)",
                           HSHK,El_Num, X__Old, Y__Old, W__Old,X2_Old,H__Old,Y2_Old);
                   sprintf(EDTS2,"min=%f max=%f howp=%d [%s]",
                           LSHK[a].mn,LSHK[a].mx,LSHK[a].h,LSHK[a].fr);
                   sprintf(EDTS3," ");
                   break;
                }
                // �᫨ ����� �� ��諨- ��३� � ᫥���饬�
            };  El_Cel=a; if(a>=HSHK){ El_Que=11; El_Cel=0; }  
         }
         // LSLM (����訢���� ������)
         if(El_Que==11&&XYMode==0){
            for(a=El_Cel; a<HSLM; a++){
                if(LSLM[a].np>0 &&
                   mouse_x>=LSLM[a].X && mouse_x<=(LSLM[a].X+LSLM[a].W) &&
                   mouse_y>=LSLM[a].Y && mouse_y<=(LSLM[a].Y+LSLM[a].H)){
                   X__Old=LSLM[a].X;     Y__Old=LSLM[a].Y;
                   W__Old=LSLM[a].W;     H__Old=LSLM[a].H; 
                   El_NA =LSLM[a].na;    El_NV =LSLM[a].nv; 
                   GetNVName(El_NA,El_NV);
                   El_VL=Arr_to_Val(El_NA,El_NV);
                   XYMode=11;El_Typ=11;El_Que=11;El_Num=a;
                   sprintf(EDTS1,"Cmd=[Q]%d LSLM[%d] [X=%d Y=%d] W=%d(%d) H=%d(%d)",
                           HSLM, El_Num, X__Old, Y__Old, W__Old,X2_Old,H__Old,Y2_Old);
                   sprintf(EDTS2,"na=%d nv=%d vl=%f [%s]  >>%s", El_NA,El_NV,El_VL,El_Nam,El_Str);
                   sprintf(EDTS3,"np=%d md=%d  vlo=%f vhi=%f  clo=%d cn=%d chi=%d",
                           LSLM[a].np,LSLM[a].md,LSLM[a].vl,LSLM[a].vh,LSLM[a].cl,LSLM[a].cn,LSLM[a].ch);
                   break;
                }
                // �᫨ ����� �� ��諨- ��३� � ᫥���饬�
            };  El_Cel=a; if(a>=HSLM){ El_Que=12; El_Cel=0; }  
         }
         // LCST (��࠭�� ��ப� �⮡ࠦ����)
         if(El_Que==12&&XYMode==0){
            for(a=El_Cel; a<HCST; a++){
                h=text_height(dFnt[LCST[a].sn].dat);
                if(LCST[a].nv>0 &&
                   mouse_x>=LCST[a].X && mouse_x<=(LCST[a].X+LCST[a].W) &&
                   mouse_y>=LCST[a].Y && mouse_y<=(LCST[a].Y+h)){
                   X__Old=LCST[a].X;     Y__Old=LCST[a].Y;
                   W__Old=LCST[a].W;     H__Old=h;
                   El_NA =LCST[a].na;    El_NV =LCST[a].nv; 
                   GetNVName(LCST[a].na,LCST[a].nv);
                   El_VL=Arr_to_Val(El_NA,El_NV);
                   XYMode=12;El_Typ=12;El_Que=12;El_Num=a;
                   sprintf(EDTS1,"Cmd=[C]%d LCST[%d] [X=%d Y=%d] W=%d(%d) H=%d(%d)",
                           HCST,El_Num, X__Old, Y__Old, W__Old,X2_Old,H__Old,Y2_Old);
                   sprintf(EDTS2,"na=%d nv=%d vl=%f [%s]  >>%s", El_NA,El_NV,El_VL,El_Nam,El_Str);
                   sprintf(EDTS3," ");  
                   break;
                }
                // �᫨ ����� �� ��諨- ��३� � ᫥���饬�
            };  El_Cel=a; if(a>=HCST){ El_Que=13; El_Cel=0; }  
         }
         // LANI (������ �����樨)
         if(El_Que==13&&XYMode==0){
            for(a=El_Cel; a<HANI; a++){
                if(LANI[a].nv>0 &&
                   mouse_x>=LANI[a].X && mouse_x<=(LANI[a].X+LANI[a].W) &&
                   mouse_y>=LANI[a].Y && mouse_y<=(LANI[a].Y+LANI[a].H)){
                   X__Old=LANI[a].X;     Y__Old=LANI[a].Y;
                   W__Old=LANI[a].W;     H__Old=LANI[a].H;
                   El_NA =LANI[a].na;    El_NV =LANI[a].nv; 
                   GetNVName(LANI[a].na,LANI[a].nv);
                   El_VL=Arr_to_Val(El_NA,El_NV);
                   XYMode=13;El_Typ=13;El_Que=13;El_Num=a;
                   sprintf(EDTS1,"Cmd=[A]%d LANI[%d] [X=%d Y=%d] W=%d(%d) H=%d(%d)",
                            HANI, El_Num,  X__Old, Y__Old, W__Old,X2_Old,H__Old,Y2_Old);
                   sprintf(EDTS2,"na=%d nv=%d vl=%f [%s]  >>%s", El_NA,El_NV,El_VL,El_Nam,El_Str);
                   sprintf(EDTS3,"fp=[%s]",LANI[a].fp);  
                   break;
                }
                // �᫨ ����� �� ��諨- ��३� � ᫥���饬�
            };  El_Cel=a; if(a>=HANI){ El_Que=100; El_Cel=0; }  
         }
         // ��᫥ ��� �஢�ப, �᫨ ������ �����- ������ ��।������
         if(XYMode>0&&El_Que<100){ 
            SCode=0; X2_Old=X__Old+W__Old; Y2_Old=Y__Old+H__Old;
         }
      }
      //------------------------------------------------
   }
   //---------------------------------------------------


   //---------------------------------------------------
   // �뢥�� �� �࠭ ���ଠ樮��� ��ப� � ���� ।���� (every_c)
   if(XYMode>0&&El_Que<100){
      sprintf(EDTS4,"[X=%d Y=%d]  [%.0f]", mouse_x, mouse_y, El_Cyc);
      //------------------------------------------------
      // �뢮� ࠬ�� ⥪�饣� �����
      if(El_Brd>0){
         rect(scrout, X__Old, Y__Old, X2_Old, Y2_Old, EDTRC);
      }
      // �뢮� ࠬ�� ��� ����⮢
      if(El_BAL>0){
         for(a=0; a<HSEL; a++){
             if(LSEL[a].k>0){ 
                rect(scrout, LSEL[a].X, LSEL[a].Y, LSEL[a].X+LSEL[a].W, LSEL[a].Y+LSEL[a].H, EDTRC);
             }
         }
         for(a=0; a<HSPR; a++){
             if(LSPR[a].vw>0){ 
                rect(scrout, LSPR[a].X, LSPR[a].Y, LSPR[a].X+LSPR[a].W, LSPR[a].Y+LSPR[a].H, EDTRC);
             }
         }
         for(a=0; a<HSTRC;a++){
             if(LSTR[a].str[0]>0){ 
                h=text_height(dFnt[LSTR[a].sn].dat);
                w=text_length(dFnt[LSTR[a].sn].dat,LSTR[a].str);
                rect(scrout, LSTR[a].X, LSTR[a].Y, LSTR[a].X+w, LSTR[a].Y+h, EDTRC);
             }
         }
         for(a=0; a<HVAL; a++){
             if(LVAL[a].nv>0){ 
                h=text_height(dFnt[LVAL[a].sn].dat);
                rect(scrout, LVAL[a].X, LVAL[a].Y, LVAL[a].X+LVAL[a].W, LVAL[a].Y+h, EDTRC);
             }
         }
         for(a=0; a<HGET; a++){
             if(LGET[a].nv>0){ 
                h=text_height(dFnt[LGET[a].sn].dat);
                rect(scrout, LGET[a].X, LGET[a].Y, LGET[a].X+LGET[a].W, LGET[a].Y+h, EDTRC);
             }
         }
         for(a=0; a<HGST; a++){
             if(LGST[a].nv>0){ 
                rect(scrout, LGST[a].X, LGST[a].Y, LGST[a].X+LGST[a].W, LGST[a].Y+LGST[a].H, EDTRC);
             }
         }
         for(a=0; a<HWIN; a++){
             if(LWIN[a].vw>0){ 
                rect(scrout, LWIN[a].X, LWIN[a].Y, LWIN[a].X+LWIN[a].W, LWIN[a].Y+LWIN[a].H, EDTRC);
             }
         }
         for(a=0; a<HMNU; a++){
             if(LMNU[a].vw>0){ 
                rect(scrout, LMNU[a].X, LMNU[a].Y, LMNU[a].X+LMNU[a].W, LMNU[a].Y+LMNU[a].H, EDTRC);
             }
         }
         for(a=0; a<HBOX; a++){
             if(LBOX[a].vw>0){ 
                rect(scrout, LBOX[a].X, LBOX[a].Y, LBOX[a].X+LBOX[a].W, LBOX[a].Y+LBOX[a].H, EDTRC);
             }
         }
         for(a=0; a<HSHK; a++){
                rect(scrout, LSHK[a].X, LSHK[a].Y, LSHK[a].X+LSHK[a].W, LSHK[a].Y+LSHK[a].H, EDTRC);
         }
         for(a=0; a<HSLM; a++){
             if(LSLM[a].np>0){ 
                rect(scrout, LSLM[a].X, LSLM[a].Y, LSLM[a].X+LSLM[a].W, LSLM[a].Y+LSLM[a].H, EDTRC);
             }
         }
         for(a=0; a<HCST; a++){
             if(LCST[a].nv>0){ 
                h=text_height(dFnt[LCST[a].sn].dat);
                rect(scrout, LCST[a].X, LCST[a].Y, LCST[a].X+LCST[a].W, LCST[a].Y+h, EDTRC);
             }
         }
         for(a=0; a<HANI; a++){
             if(LANI[a].nv>0){ 
                rect(scrout, LANI[a].X, LANI[a].Y, LANI[a].X+LANI[a].W, LANI[a].Y+LANI[a].H, EDTRC);
             }
         }
      }
      //------------------------------------------------
      // ������ ���� ।����
      clear_to_color(winedt,15);
      text_mode(15);
      // �뢮� ���ଠ樮���� ��ப � ���� ।����
      z=1; q=text_length(dFnt[EDTSN].dat,EDTS1)+20;
      h=text_height(dFnt[EDTSN].dat);
     rectfill(winedt, 0,0,   XMAX,z,                    1); z=z+5;
      textout(winedt, dFnt[EDTSN].dat, EDTS1, 5, z,     4); 
      textout(winedt, dFnt[EDTSN].dat, EDTS4, q, z,     5); z=z+h+3;
      textout(winedt, dFnt[EDTSN].dat, EDTS2, 5, z,   179); z=z+h+3;
      textout(winedt, dFnt[EDTSN].dat, EDTS3, 5, z,     1);
      // �뢮� ���� ।���� �� �࠭
      blit(winedt,scrout, 0,0, 0, YMAX-EDT_H, XMAX, EDT_H);
      //------------------------------------------------
   }
   //---------------------------------------------------
   return;
}
//---------------------------------------------------------------------------

