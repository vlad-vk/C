// coding: cp866
//-----------------------------------------------------------------------------
// � �⮬ 䠩�� ��室���� �㭪樨 ��� ࠡ��� � �����ﬨ:
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  �����頥� ������⢮ ����ᥩ � �� ���� � 㤠�����
_l  dbRecCount(_i Sel){ return(dbHeaw[Sel].RecCount); }
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  ��⠭����� 㪠��⥫� �� ����� ������
_l  dbGoFirst(_i Sel){ 
    if(dbHeaw[Sel].CRec<0){ return(-1); }
    dbHeaw[Sel].CRec=0; 
    dbHeaw[Sel].Bof=0; dbHeaw[Sel].Eof=0;
    return(dbHeaw[Sel].CRec);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  ��⠭����� 㪠��⥫� �� ��᫥���� ������
_l  dbGoLast (_i Sel){ 
    if(dbHeaw[Sel].CRec<0){ return(-1); }
    dbHeaw[Sel].CRec=dbHeaw[Sel].RecCount-1;
    dbHeaw[Sel].Bof=0; dbHeaw[Sel].Eof=0;
    return(dbHeaw[Sel].CRec);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  ��⠭����� 㪠��⥫� �� 㪠������ ������ N
_l  dbGoRec  (_i Sel, _l NRec){ 
    if(NRec<0){ dbHeaw[Sel].Bof=1; return(-1); }
    if(NRec>=dbHeaw[Sel].RecCount){ dbHeaw[Sel].Eof=1; return(-2); }
    dbHeaw[Sel].CRec=NRec; dbHeaw[Sel].Bof=0; dbHeaw[Sel].Eof=0;
    return(NRec);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  ��⠭����� 㪠��⥫� �� ᬥ饭�� 㪠������ �����
_l  dbGoSkip(_i Sel, _l SRec){ 
    if(dbHeaw[Sel].CRec+SRec<0){ dbHeaw[Sel].Bof=1; return(-1); }
    if(dbHeaw[Sel].CRec+SRec>=dbHeaw[Sel].RecCount){ dbHeaw[Sel].Eof=1; return(-2); }
    dbHeaw[Sel].CRec=dbHeaw[Sel].CRec+SRec; 
    dbHeaw[Sel].Bof=0; dbHeaw[Sel].Eof=0;
    return(SRec);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  �஢�ઠ ��室� �� ������ �࠭��� �� ��᫥ ��᫥����� ��६�饭��
_i  dbBof(_i Sel){ return(dbHeaw[Sel].Bof); }
//----------------------------------------------------------------------------
//  �஢�ઠ ��室� �� ������ �࠭��� �� ��᫥ ��᫥����� ��६�饭��
_i  dbEof(_i Sel){ return(dbHeaw[Sel].Eof); }
//----------------------------------------------------------------------------
//  �����頥� ����� ⥪�饩 ����� � ��
_l  dbRecCur(_i Sel){ return(dbHeaw[Sel].CRec); }
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  ���������� ���⮩ ����� � ��
_i  dbRecAdd(_i Sel){
    _i  a=0;
    //  �஢���� ������ �����
    if(dbHeaw[Sel].RecLen<0||dbHeaw[Sel].RecLen>MRECLEN){ return(-1); }
    //  ��⠭����� 㪠��⥫� �� ����� 䠩��
     memset(  BRec[Sel],0,dbHeaw[Sel].RecLen);
      lseek(dbFile[Sel].Desk,0,SEEK_END);
    a=write(dbFile[Sel].Desk,BRec[Sel],dbHeaw[Sel].RecLen);
    // �������� ���� ��饣� ������⢠ ����ᥩ � ��������� �� 
    if(dbModifyRecCount(Sel,1)>0){ dbHeaw[Sel].CRec=dbHeaw[Sel].RecCount-1; }
    return(a);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  ������� ���� ����� �� ���
_i  dbRecWrite(_i Sel){
    _i  a=0;
    _l  s=0;
    //  �஢���� ������ �����
    if(dbHeaw[Sel].RecLen<0||dbHeaw[Sel].RecLen>MRECLEN){ return(-1); }
    s=dbHeaw[Sel].COfs+dbHeaw[Sel].RecLen*dbHeaw[Sel].CRec;
      lseek(dbFile[Sel].Desk,s,SEEK_SET);
    a=write(dbFile[Sel].Desk,BRec[Sel],dbHeaw[Sel].RecLen);
    dbFile[Sel].Mod=1;
    return(a);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  ������ ���� ����� � ��᪠
_i  dbRecRead(_i Sel){
    _i  a=0;    
    _l  s=0;
    //  �஢���� ������ �����
    if(dbHeaw[Sel].RecLen<0||dbHeaw[Sel].RecLen>MRECLEN){ return(-1); }
     s=dbHeaw[Sel].COfs+dbHeaw[Sel].RecLen*dbHeaw[Sel].CRec;
        lseek(dbFile[Sel].Desk,s,SEEK_SET);
     a= read (dbFile[Sel].Desk,BRec[Sel],dbHeaw[Sel].RecLen);
     if(BRec[Sel][0]=='*'){ BCRD[Sel][0]='*'; }
     else                 { BCRD[Sel][0]=' '; }
    return(a);
}
//----------------------------------------------------------------------------
//  �஢�ઠ �ਧ���� 㤠����� ����� ��᫥ ����樨 �⥭��
_i  isRecDFL(_i Sel){
    _i  r=0;
    if(BCRD[Sel][0]=='*'){ r=1; }
    return(r);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  ����஢��� ���� ����� �� ����� ������ � �����
_i  dbRecCopy(_i Src, _i Des){
    memcpy(&BRec[Des], &BRec[Src], MRECLEN);
    return(0);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  �஢���� ���� �� ������ 㤠������ ��� ��⠭����� �ਧ��� 㤠�����
_i  dbRecDeleteFN(_i Sel, _l nRec, _i md, _i Flg){
    _i  r=0;
    _l  s=0;
    if(dbHeaw[Sel].RecLen<0||dbHeaw[Sel].RecLen>MRECLEN){ return(-1); }
    if(nRec<0){
       s=dbHeaw[Sel].COfs+dbHeaw[Sel].RecLen*dbHeaw[Sel].CRec;
    } else {
       if(nRec>dbHeaw[Sel].RecCount){ nRec=dbHeaw[Sel].RecCount; }
       s=dbHeaw[Sel].COfs+dbHeaw[Sel].RecLen*nRec;
    }
    lseek(dbFile[Sel].Desk,s,SEEK_SET);
    // �᫨ �� ०�� �஢�ન
    if(md==0){
       read (dbFile[Sel].Desk,BCRD[Sel],1); if(BCRD[Sel][0]=='*'){ r=1; }
    } else {
    // �᫨ �� ०�� ����� �ਧ���� 㤠�����
       if(Flg> 0){ BCRD[Sel][0]='*'; }
       else      { BCRD[Sel][0]=' '; }
       write(dbFile[Sel].Desk,&BCRD[Sel],1); r=1;
    }    
    return (r);
}
//----------------------------------------------------------------------------
//  �஢���� ���� �� ������ 㤠������
_i  isRecDelete(_i Sel, _l nRec){
    return(dbRecDeleteFN(Sel,nRec,0,0));
}
//----------------------------------------------------------------------------
//  ��⠭����� �ਧ��� 㤠����� 㪠������ �����
_i  dbRecDelete(_i Sel, _l nRec, _i Flg){
    return(dbRecDeleteFN(Sel,nRec,1,Flg));
}
//----------------------------------------------------------------------------
