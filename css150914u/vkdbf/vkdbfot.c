// coding: cp866
//-----------------------------------------------------------------------------
// � �⮬ 䠩�� ��室���� �������⥫�� �㭪樨:
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  ࠧ��� ����� ������� ������⥪�
_i  dbGetMemLib(vd){
    _i  AM=0;
    AM=AM+sizeof(BHead  );
    AM=AM+sizeof(BRead  );
    AM=AM+sizeof(BSFld  );
    AM=AM+sizeof(BRec   );
    AM=AM+sizeof(dbFile );
    AM=AM+sizeof(dbHead );
    AM=AM+sizeof(dbHExt );
    AM=AM+sizeof(dbHeaw );
    AM=AM+sizeof(dbField);
    AM=AM+sizeof(dbFielw);
    AM=AM+sizeof(dbBUFF );
    return(AM);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  �������� ���᫥��� ࠧ��� ��
_l  dbFileSize(_i Sel){
    _i  sz =0;
    if( Sel<0||Sel>=DBMAXDBF ){ return(-1); }
    sz=dbHeaw[Sel].COfs+dbHeaw[Sel].RecCount*dbHeaw[Sel].RecLen;
    return(sz);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  ��������� �������� ⥪�饩 ���� � �६���
ul  dbCurDate(uc md){
    ul  r=0;
    dbTMV=time(NULL);  dbtm=localtime(&dbTMV);
    // ������ ⥪���� ����
    if(md=='D'||md=='d'||md==0){
       r=(((ul)dbtm->tm_year+(ul)1900)*(ul)10000)+
         (((ul)dbtm->tm_mon +(ul)1   )*(ul)100  )+(ul)dbtm->tm_mday;
    }
    // ������ ⥪�饥 �६�
    if(md=='T'||md=='t'||md==1){
       r=((ul)dbtm->tm_hour*(ul)10000)+
         ((ul)dbtm->tm_min *(ul)100  )+(ul)dbtm->tm_sec;
    }
    return(r);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  ��⠭����� 㪠��⥫� � 䠩�� �� ��� ⥪�饩 ����� � ��।�������� ����
_l  dbFieldOffset(_i Sel, _i Fld, _i md){
    _l  r=0, s=0;
    if(md==0){ return(dbFielw[Sel][Fld].OfsFld+1); }
    if(dbHeaw[Sel].CRec<0){ return(-1); }
    s=dbHeaw[Sel].COfs+
      dbHeaw[Sel].RecLen*dbHeaw[Sel].CRec + dbFielw[Sel][Fld].OfsFld + 1;
    r=lseek(dbFile[Sel].Desk,s,SEEK_SET);
    if(r==EBADF ){ r=-5; /* printf("EBADF\n");  */ }
    if(r==EINVAL){ r=-7; /* printf("EINVAL\n"); */ }
    if(r==ESPIPE){ r=-9; /* printf("ESPIPE\n"); */ }
    return(r);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  ����䨪��� ���� ��᫥����� ����������
vd  dbModifyDate(_i Sel){
    dbCurDate('D');
    dbHead[Sel].Year=(uc)(dbtm->tm_year-100);
    dbHead[Sel].Mon =(uc)(dbtm->tm_mon+1);
    dbHead[Sel].Day =(uc)(dbtm->tm_mday);
    return;
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  ����䨪��� ������⢠ ����ᥩ
_i  dbModifyRecCount(_i Sel, _i md){
    _i  a=0;    
    //  㤠����� ����� �����
    if (md==-1){
        if (dbHeaw[Sel].RecCount>0){ 
            dbHeaw[Sel].RecCount--; a=dbHeaw[Sel].RecCount; 
        }
    }
    // ���������� �����
    if (md>0){
        if (dbHeaw[Sel].RecCount+md<DBMAXREC-1){ 
            dbHeaw[Sel].RecCount=dbHeaw[Sel].RecCount+md; 
            a=dbHeaw[Sel].RecCount; 
        }
    }
    // ��१������ ��������� ��� ���� (��ࠢ����� ���������)
    if (md==0){ a=dbHeaw[Sel].RecCount; }
    // �᫨ ������⢮ ����ᥩ �뫮 ��������- ��१������ ���������
    if (a>0){
        memcpy(&dbHead[Sel].RecCount,&dbHeaw[Sel].RecCount,4);
        lseek ( dbFile[Sel].Desk,0,SEEK_SET);           
        write ( dbFile[Sel].Desk,&dbHead[Sel],DBHDLEN);
        dbFile[Sel].Mod=1;
    }
    return(a);    
}
//----------------------------------------------------------------------------
