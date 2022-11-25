// coding: cp866
//-----------------------------------------------------------------------------
// � �⮬ 䠩�� ��室���� �㭪樨 ࠡ��� � ����� ������ �१ ����:
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  ���� ᢮������� ����
_i  dbBFBufferAvail(){
    _i  a=0;
    for(a=0;a<DBMAXBUFF;a++){
        if(dbBUFF[a].Init<0){ return(a); }
    };  return(-1);
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  ���ᨬ��쭮� ������⢮ ����ᥩ � ����
_i  dbBFRecInBuf(_i Sel, _i nBuf){
    if( Sel<0|| Sel>=DBMAXDBF ){ return(-1); }
    if(dbHeaw[Sel].RecLen<1){ return(-2); }
    dbBUFF[nBuf].RecCount=divi(DBBSIZ, dbHeaw[Sel].RecLen);
    return(dbBUFF[nBuf].RecCount);
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  ���⪠ ���� � 㪠����� ����஬
_i  dbBFClear(_i nBuf){
    _i  a=0;    
    if(nBuf<0||nBuf>=DBMAXBUFF){ return(-1); }
    dbBUFF[nBuf].RecCur=-1; 
    dbBUFF[nBuf].RecCount=0;
    dbBUFF[nBuf].Init=-1;
    for(a=0;a<DBBSIZ;a++){ dbBUFF[nBuf].Arr[a]=0; }
    return(0);
}
//-----------------------------------------------------------------------------
//  ���⪠ ��� ���஢
vd  dbBFClearALL(vd){
    _i  a=0;
    for(a=0;a<DBMAXBUFF;a++){ dbBFClear(a); }    
    return;
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  ��१�ࢨ஢��� ���� ��� ���쭥�襣� �ᯮ�짮�����
_i  dbBFInit(_i nBuf){
    if(nBuf<0||nBuf>=DBMAXBUFF){ return(-1); }
    if( dbBUFF[nBuf].Init >= 0){ return(-2); }
    dbBUFF[nBuf].Init=nBuf;
    return(nBuf);
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  �⥭�� 䠩�� �� � ���� �� ⥪�饩 �����
_i  dbBFRead(_i Sel, _i nBuf, _i hRec){
    _i  a=0, x=0;
    _l  s=0;
    //  �஢���� �ࠢ��쭮��� ������� �����⥩
    if(nBuf<0||nBuf>=DBMAXBUFF){ return(-1); }
    if( Sel<0|| Sel>=DBMAXDBF ){ return(-1); }
    //  �஢���� ����� ⥪�饩 ����� � ������ �����
    if(dbHeaw[Sel].CRec<0||dbHeaw[Sel].CRec>DBMAXREC){ return(-2); }
    //  �஢���� ������ �����
    if(dbHeaw[Sel].RecLen<=0||dbHeaw[Sel].RecLen>MRECLEN){ return(-3); }
    //  �����⨬�� ������⢮ ����ᥩ � ����
    dbBFRecInBuf(Sel,nBuf);
    //  ���᫨�� ᬥ饭�� � 䠩�� �� ��� ⥪�饩 �����
    s=dbHeaw[Sel].COfs+dbHeaw[Sel].RecLen*dbHeaw[Sel].CRec;
    //  ࠧ��� ����� ����� ��।��� � ���ᨢ ����
    dbBUFF[nBuf].RecLen=dbHeaw[Sel].RecLen;
    //  ���᫨�� ������⢮ ����, ᮮ⢥������� �������� ����ᥩ hRec
    x=hRec*dbHeaw[Sel].RecLen; if(x>DBBSIZ){ x=DBBSIZ; }
    //  ��⠭����� 㪠��⥫� �� ��砫� ⥪�饩 ����� � 䠩�� ��
    lseek(dbFile[Sel].Desk,s,SEEK_SET);
    //  ������ 䠩� � ����
    a=read(dbFile[Sel].Desk, dbBUFF[nBuf].Arr,x);
    //  ������ ������⢮ ����ᥩ � ��⠭��� ����
    if(a<dbHeaw[Sel].RecLen){ return(0); }    
    return(divi(a, dbHeaw[Sel].RecLen));
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  ������ ���� � 䠩� �� (���� �����뢠���� � ����� N0 � ����,
//  � ⥪���� ������ ��, �஬� ०��� ����������)
_i  dbBFWrite(_i Sel, _i nBuf, _i hRec, _i mdWR){
    _i  l=0, r=0, w=0, hrc=0;
    _l  s=0, z=0;
    //  �஢���� �ࠢ��쭮��� ������� �����⥩
    if(nBuf<0||nBuf>=DBMAXBUFF){ return(-1); }
    if( Sel<0|| Sel>=DBMAXDBF ){ return(-2); }
    if(mdWR!=DBBFADD&&mdWR!=DBBFWRITE&&mdWR!=DBBFWRADD){ mdWR=DBBFADD; }
    //  �஢���� ������ �����
    if(dbHeaw[Sel].RecLen<=0||dbHeaw[Sel].RecLen>MRECLEN){ return(-3); }
    //  �஢���� ������⢮ �����뢠���� ����ᥩ
    if(hRec<0||hRec>(hrc=divi(DBBSIZ,dbHeaw[Sel].RecLen))){ return(-4); }
    //  �����⨬�� ������⢮ ����ᥩ � ����
    dbBFRecInBuf(Sel,nBuf);
    //  ࠧ��� ����� ����� ��।��� � ���ᨢ ����
    dbBUFF[nBuf].RecLen=dbHeaw[Sel].RecLen;
    //  ���᫨�� ᬥ饭�� � 䠩�� �� ��� ⥪�饩 �����
    if(dbHeaw[Sel].CRec<0){ s=dbHeaw[Sel].COfs; dbHeaw[Sel].CRec=0; } 
     s=dbHeaw[Sel].COfs+dbHeaw[Sel].RecLen*(dbHeaw[Sel].CRec);
    //  ������⢮ �����뢠���� ����ᥩ
    if(hRec>0){
       //  ���᫨�� ࠧ��� 䠩�� ��
       z=dbFileSize(Sel); 
       //  ���᫨�� ������ �����뢠����� ����
       l=hRec*dbHeaw[Sel].RecLen;
       //  ��⠭����� 㪠��⥫� �� ��砫� ⥪�饩 ����� � 䠩�� ��:
       if(mdWR==DBBFADD){
          dbModifyRecCount(Sel,hRec);             // �������� ���������
          lseek(dbFile[Sel].Desk,0,SEEK_END);     // ���������� ����ᥩ
       }
       if(mdWR==DBBFWRITE){                       // ��१����� ����ᥩ
          lseek(dbFile[Sel].Desk,s,SEEK_SET);     // ��� ���७�� ��
          if(s+l>z){ l=z-s; }                     // ᪮�४�� ࠧ��� ����
       }
       if(mdWR==DBBFWRADD ){                      // ��१����� ����ᥩ �
          if(s+l>z){                              // �������� ���७��� ��
             hRec=divi( ((s+l)-z), dbHeaw[Sel].RecLen);
             dbModifyRecCount(Sel,hRec);
          }
          lseek(dbFile[Sel].Desk,s,SEEK_SET);     
       }
       //  ������� ���� �� ��� � 䠩� ��
       if(hRec>0){
          w=write (dbFile[Sel].Desk,&dbBUFF[nBuf].Arr,l); 
          r=divi(w,dbHeaw[Sel].RecLen);
       }
    }
    return(r);
}
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
//  ���室 �� 㪠������ ������ � ���� ��
_i  dbBFGoRec(_i nBuf, _i NRec){
    //  �஢���� �ࠢ��쭮��� ������� ����� ����
    if(nBuf<0||nBuf>=DBMAXBUFF){ return(-1); }
    // �஢���� ��室 ����� ����� �� �࠭��� ����
    if(NRec<0){ dbBUFF[nBuf].Bof=1; return(-1); }
    if(NRec>=dbBUFF[nBuf].RecCount){ dbBUFF[nBuf].Eof=1; return(-2); }
    // ��⠭����� ����� ⥪�饩 ����� � ���� � 䫠��
    dbBUFF[nBuf].RecCur=NRec; dbBUFF[nBuf].Bof=0; dbBUFF[nBuf].Eof=0;
    return(NRec);
}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//  ���室 �� ������ � ���� ��
_i  dbBFGoSkip(_i nBuf, _i SRec){
    _i  a=0;    
    //  �஢���� �ࠢ��쭮��� ������� ����� ����
    if(nBuf<0||nBuf>=DBMAXBUFF){ return(-1); }
    // �஢���� ��室 ����� ����� �� �࠭��� ����
    if(dbBUFF[nBuf].RecCur+SRec<0){ 
       dbBUFF[nBuf].RecCur=0; dbBUFF[nBuf].Bof=1; a=-1;
    }
    if(dbBUFF[nBuf].RecCur+SRec>=dbBUFF[nBuf].RecCount){ 
       dbBUFF[nBuf].RecCur=dbBUFF[nBuf].RecCount-1; dbBUFF[nBuf].Eof=1; a=-2;
    }
    // ��⠭����� ����� ⥪�饩 ����� � ���� � 䫠��
    if(a==0){
       dbBUFF[nBuf].RecCur=dbBUFF[nBuf].RecCur+SRec; 
       dbBUFF[nBuf].Bof=0; dbBUFF[nBuf].Eof=0;
    }
    return(dbBUFF[nBuf].RecCur);
}
//-----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  ��⠭����� 㪠��⥫� �� ����� ������ � ����
_i  dbBFGoFirst(_i nBuf){ 
    //  �஢���� �ࠢ��쭮��� ������� ����� ����
    if(nBuf<0||nBuf>=DBMAXBUFF){ return(-1); }
    // �஢���� ��室 ����� ����� �� �࠭��� ����
    dbBUFF[nBuf].RecCur=0; dbBUFF[nBuf].Bof=0; dbBUFF[nBuf].Eof=0;
    return(dbBUFF[nBuf].RecCur);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  ��⠭����� 㪠��⥫� �� ��᫥���� ������ � ����
_i  dbBFGoLast (_i nBuf){ 
    //  �஢���� �ࠢ��쭮��� ������� ����� ����
    if(nBuf<0||nBuf>=DBMAXBUFF){ return(-1); }
    // ��⠭����� ����� ⥪�饩 ����� � ���� � 䫠��
    dbBUFF[nBuf].RecCur=dbBUFF[nBuf].RecCount-1; 
    dbBUFF[nBuf].Bof=0; dbBUFF[nBuf].Eof=0;
    return(dbBUFF[nBuf].RecCur);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  �஢�ઠ ��室� �� ������ �࠭��� � ���� ��᫥ ��᫥����� ��६�饭��
_i  dbBFBof(_i nBuf){ 
    //  �஢���� �ࠢ��쭮��� ������� ����� ����
    if(nBuf<0||nBuf>=DBMAXBUFF){ return(-1); }
    return(dbBUFF[nBuf].Bof); 
}
//----------------------------------------------------------------------------
//  �஢�ઠ ��室� �� ������ �࠭��� � ���� ��᫥ ��᫥����� ��६�饭��
_i  dbBFEof(_i nBuf){ 
    //  �஢���� �ࠢ��쭮��� ������� ����� ����
    if(nBuf<0||nBuf>=DBMAXBUFF){ return(-1); }
    return(dbBUFF[nBuf].Eof); 
}
//----------------------------------------------------------------------------
//  �����頥� ����� ⥪�饩 ����� � ����
_i  dbBFRecCur(_i nBuf){ 
    //  �஢���� �ࠢ��쭮��� ������� ����� ����
    if(nBuf<0||nBuf>=DBMAXBUFF){ return(-1); }
    return(dbBUFF[nBuf].RecCur); 
}
//----------------------------------------------------------------------------
//  �����頥� ������⢮ ����ᥩ � ���� ���� � 㤠�����
_i  dbBFRecCount(_i nBuf){ 
    //  �஢���� �ࠢ��쭮��� ������� ����� ����
    if(nBuf<0||nBuf>=DBMAXBUFF){ return(-1); }
    return(dbBUFF[nBuf].RecCount); 
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  ��⠭���� �ਧ���� 㤠����� ����� � ����
_i  dbBFRecDelete(_i nBuf, _i nRec, _i DFL){
    _i  nr=0;  
    //  �஢���� �ࠢ��쭮��� ������� ����� ����
    if(nBuf<0||nBuf>=DBMAXBUFF){ return(-1); }
    //  �஢���� ��室 ����� ����� �� �࠭��� ����
    if(nRec<0){ return(-2); }
    if(nRec>=dbBUFF[nBuf].RecCount){ return(-3); }
    //  ��⠭����� �ਧ��� 㤠�����
    nr=nRec*dbBUFF[nBuf].RecLen; 
    if(DFL>0){ dbBUFF[nBuf].Arr[nr]='*'; }  // ��⠭����� �ਧ��� 㤠�����
    else     { dbBUFF[nBuf].Arr[nr]=' '; }  // ���� �ਧ��� 㤠�����
    return(0);
}
//----------------------------------------------------------------------------
//  �஢�ઠ �ਧ���� 㤠����� ����� � ����
_i  isBFRecDelete(_i nBuf, _i nRec){
    _i  nr=0;  
    //  �஢���� �ࠢ��쭮��� ������� ����� ����
    if(nBuf<0||nBuf>=DBMAXBUFF){ return(-1); }
    //  �஢���� ��室 ����� ����� �� �࠭��� ����
    if(nRec<0){ return(-2); }
    if(nRec>=dbBUFF[nBuf].RecCount){ return(-3); }
    //  �஢���� �ਧ��� 㤠�����
    nr=nRec*dbBUFF[nBuf].RecLen;
    if(dbBUFF[nBuf].Arr[nr]=='*'){
       return(1);
    }; return(0);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  ����஢��� ⥪���� ������ �� � ⥪���� ������ ����
_i  dbBFRecCopyBuf(_i Sel, _i nBuf){
    _i  s=0;
    //  �஢���� �ࠢ��쭮��� ������� �����⥩
    if(nBuf<0||nBuf>=DBMAXBUFF){ return(-1); }
    if( Sel<0|| Sel>=DBMAXDBF ){ return(-2); }
    if(dbBUFF[nBuf].RecCur <0 ){ return(-3); }
    s=dbHeaw[Sel].RecLen*dbBUFF[nBuf].RecCur;
    memcpy(&dbBUFF[nBuf].Arr[s],&BRec[Sel],dbHeaw[Sel].RecLen);
    return(1);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  ����஢��� ⥪���� ������ �� � ⥪���� ������ ����
_i  dbBFBufCopyRec(_i Sel, _i nBuf){
    _i  s=0;
    //  �஢���� �ࠢ��쭮��� ������� �����⥩
    if(nBuf<0||nBuf>=DBMAXBUFF){ return(-1); }
    if( Sel<0|| Sel>=DBMAXDBF ){ return(-2); }
    if(dbBUFF[nBuf].RecCur <0 ){ return(-3); }
    s=dbHeaw[Sel].RecLen*dbBUFF[nBuf].RecCur;
    memcpy(&BRec[Sel],&dbBUFF[nBuf].Arr[s],dbHeaw[Sel].RecLen);
    return(1);
}
//----------------------------------------------------------------------------
