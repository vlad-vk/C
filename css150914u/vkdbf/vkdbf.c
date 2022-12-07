// coding: cp866
//-----------------------------------------------------------------------------
// � �⮬ 䠩�� ��室���� �㭪樨 ࠡ��� � ����� ������:
//-----------------------------------------------------------------------------
//#include <allegro.h>
#include "../define.h"
#include "../include.h"
#include "vkdbfdb.h"
#include "vkdbffd.c"
#include "vkdbfot.c"
#include "vkdbfrc.c"
#include "vkdbfvl.c"
#include "vkdbfbf.c"

//----------------------------------------------------------------------------
//  ���⪠ ��������� ���ᨢ�
vd  dbClear(_i Sel){
    dbFile[Sel].Name[0]=0; dbFile[Sel].Name[1]=0; 
    dbFile[Sel].Desk=-1;   dbFile[Sel].Init=-1; 
    return;
}
//----------------------------------------------------------------------------
//  ������ �� ���ᨢ�
vd  dbClearALL(vd){
    _i  a=0;
    for(a=0;a<DBMAXDBF;a++){ dbClear(a); }
    return;
}
//----------------------------------------------------------------------------
//  ��砫쭠� ���樠������ ���ᨢ��
_i  dbInit(_i Sel){
    if(Sel<0||Sel>=DBMAXDBF){ 
       DPrn(13,1,"dbInit: 㪠������ ������� %d ��� �࠭��.\n",Sel); return(-1); 
    }
    if(dbFile[Sel].Init>=0 ){ 
       DPrn(13,1,"dbInit: 㪠������ ������� %d 㦥 ���樠����஢���.\n",Sel); return(-2); 
    }
    dbClear(Sel); dbFile[Sel].Init=Sel;
    return (Sel);
}
//----------------------------------------------------------------------------
//  ��।������ ᢮������ ������ ��� ࠡ��� � ��
_i  dbSelectAvail(vd){
    _i  a=0;
    for(a=0;a<DBMAXDBF;a++){
        if(dbFile[a].Name[0]==0 && dbFile[a].Name[1]==0 &&
           dbFile[a].Desk<0     && dbFile[a].Init<0){
           return(a);
        }
    };  
    DPrn(13,1,"dbSelectAvail: ��� ᢮������ ������.\n"); return(-3);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  �஢���� ����� �� �� � 㪠������ ������
_i  dbOpenTest(_i Sel,uc *FName){
    _i  a=0;
    if (Sel<0){
        for(a=0;a<DBMAXDBF;a++){
            if(strcasecmp(dbFile[a].Name,FName)==0){ return(a); }
        }
        DPrn(13,1,"dbOpenTest: 㪠������ 䠩� �� �� �����.\n"); 
        return(-4);
    }
    if (Sel>=DBMAXDBF){ 
        DPrn(13,1,"dbOpenTest: 㪠������ ������� %d ��� �࠭��.\n",Sel); 
        return(-5); 
    }
    if (dbFile[Sel].Name[0]==0){
        DPrn(13,1,"dbOpenTest: �� ��।����� ��� �� ��� ������ %d.\n",Sel); 
        return(-6); 
    }
    if (dbFile[Sel].Desk<0){ 
        DPrn(13,1,"dbOpenTest: �� ��।���� ���ਯ�� �� ��� ������ %d.\n",Sel); 
        return(-7); 
    }
    if (dbFile[Sel].Init<0){
        DPrn(13,1,"dbOpenTest: ���ᨢ ��� 㪠������ ������ %d �� ���樠����஢��.\n",Sel);
        return(-8); 
    }
    return(Sel);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  �஢�ઠ 楫��⭮�� �� � ��������� �� ����室����� �� ���४�஢��
_i  dbCheck(_i Sel, uc *dbName){
    _i  FDesk=0;                                 // ���ਯ�� 䠩��
    _i  h=0,r=0;
    _l  hbr=0, szf=0, szr=0, szt=0, hrc=0;
    //  �஢���� ������� �� 䠩� ��
    if (access(dbName, 0)<0){ 
        DPrn(13,1,"dbCheck: ��� ����㯠 � 䠩�� �� [%s] � ������ %d.\n",dbName,Sel); 
        return(-9); 
    }
    //  �᫨ �� � 㪠������ ������ 㦥 �����
    if (dbOpenTest(Sel,0)>=0){ 
        DPrn(13,1,"dbCheck: � 㪠������ ������ %d �� 㦥 �����.\n",Sel); 
        return(-10); 
    }
    //  ������ 䠩�
    if((FDesk=open(dbName,O_BINARY|O_RDWR))<0){ 
        DPrn(13,1,"dbCheck: �訡�� ������ 䠩�� [%s].\n",dbName); 
        return(-11); 
    }
    //  ���㫨�� ���ᨢ ��� �⥭��
    memset(BRead,0,DBBSIZ);
    //  ������ ��������� ���뢠����� 䠩��
    hbr=read(FDesk,&BRead,DBHDLEN+DBFLDLN*DBMAXFLD+DBBEXT);
    //  �᫨ ������⢮ ��⠭��� ���� < 64 (32=��������� + 32=���ᠭ�� ����)
    if (hbr<DBHDLEN*2){ 
        DPrn(13,1,"dbCheck: �訡�� ��������� 䠩�� [%s] � ������ %d.\n",dbName,Sel);
        close(FDesk); return(-12); 
    }
    //  �� ��⠭��� ������ ��������� �������� ���ᠭ�� ���������
    memcpy(&dbHead[Sel],&BRead,DBHDLEN);
    //  ࠧ��� ������� ��������� ��
    memcpy(&h,&dbHead[Sel].HeadSize,2);
    //  �஢���� �ࠢ��쭮��� ������ ���������
    if((h<DBHDLEN)||(h>DBHDLEN+DBMAXFLD*DBFLDLN+DBBEXT)){
        DPrn(13,1,"dbCheck: �訡�� ��������� 䠩�� [%s] � ������ %d.\n",dbName,Sel);
        close(FDesk); return(-13);
    }
    //  ࠧ��� ����� �����
    memcpy(&dbHeaw[Sel].RecLen,&dbHead[Sel].RecLen,2);
    //  �஢�ઠ ���४⭮�� ������ ����� �����
    if((dbHeaw[Sel].RecLen<1)||(dbHeaw[Sel].RecLen>(DBMAXFLD*DBSLEN))){ 
        DPrn(13,1,"dbCheck: �訡�� ��������� 䠩�� [%s] � ������ %d.\n",dbName,Sel);
        close(FDesk); return(-14); 
    }
    //  ������⢮ ����ᥩ � ��
    memcpy(&dbHeaw[Sel].RecCount,&dbHead[Sel].RecCount,4);
    //  ������� ࠧ��� �ᥣ� 䠩��
    lseek(FDesk,0,SEEK_END);  szf=lseek(FDesk,0,SEEK_CUR);
    //  ������� ࠧ��� ��� ����ᥩ
    szr=szf-h;
    //  ������� �।���������� ������⢮ ����ᥩ
    hrc=divi(szr, dbHeaw[Sel].RecLen);
    //  ���⭠� �஢�ઠ ࠧ��� ��� ����ᥩ ��
    szt=hrc*dbHeaw[Sel].RecLen;
    //  �᫨ ࠧ���� �� ᮢ������- ������� 䠩� ��ﬨ �� �㦭��� ࠧ���
    if (szt!=szr||(szr==0&&dbHeaw[Sel].RecCount>0)){
        if(szr==0){ dbHeaw[Sel].RecCount=0; }
        else      { dbHeaw[Sel].RecCount=hrc+1; }
        szt=dbHeaw[Sel].RecCount*dbHeaw[Sel].RecLen; h=szt-szr;
        memset(BRead,0,h);
        write (FDesk,BRead,h);
        //  ��ࠢ��� ��������� ��
        memcpy(&dbHead[Sel].RecCount,&dbHeaw[Sel].RecCount,4);
        lseek ( FDesk,0,SEEK_SET); 
        write ( FDesk,&dbHead[Sel],DBHDLEN);
    }
    //  ������� 䠩� ��
    close(FDesk);
    return (r);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  ᮧ���� 䠩� ��
_i  dbCreate(_i Sel, uc *dbName){
    _i  FDesk=0;                       // ���ਯ�� 䠩��
    _i  f=0;                           // ����� ����
    _i  l[3];                          // ������ ����� �����
    _l  cps=0,h=0;                     // ᬥ饭�� � ���� (������ ���������)
    //  �஢���� ������� �� 䠩� ��
    if (access(dbName,0)==0){ 
        DPrn(13,1,"dbCreate: �訡�� ����㯠 � 䠩�� [%s] � ������ %d.\n",dbName,Sel);
        return(-21); 
    }
    //  ��������� �������� ���������
    dbHead [Sel].ID=3;                      // �����䨪���
    dbModifyDate(Sel);                      // ��� ����䨪�樨
    dbHeaw [Sel].RecCount=0;                // ������⢮ ����ᥩ � ��
    memcpy(&BRec[Sel],&dbHead[Sel],DBHDLEN);
    cps=cps+DBHDLEN;
    l[0]=1;                                 // l[0]=1 -�ਧ��� 㤠����� �����
    for(f=0;f<DBMAXFLD;f++){
        if((dbFieldTest(Sel,f))<0){ break; }// �᫨ ���� � ������� �� ������
        memcpy(&dbField[Sel][f].OfsFld,&l[0],2);
        memcpy(&BRec[Sel][cps],&dbField[Sel][f],DBHDLEN); 
        cps=cps+DBHDLEN;
        l[0]=l[0]+(_i)dbField[Sel][f].Len;  // ����� ����� ������ �����
    }                                       
    // �᫨ �� �뫮 ����ᠭ� �� ������ ����
    if (f<1){ 
        DPrn(13,1,"dbCreate: �訡�� ����� ����� � 䠩�� [%s] ������ %d.\n",dbName,Sel);
        dbFieldClear(Sel,0); return(-22); 
    }
    // ������� �ਧ��� ���� ���������
    BRec[Sel][cps]=13;  cps=cps+1;
    // ������� ���७�� ���������
    if(dbHExt[Sel].Len>0){                
       memcpy(&BRec[Sel][cps],&dbHExt[Sel].Buf,dbHExt[Sel].Len); 
       cps=cps+dbHExt[Sel].Len;
    }; h=cps;
    // ������������ �������� ���������
    memcpy(&dbHead[Sel].RecCount,&dbHeaw[Sel].RecCount,4);
    memcpy(&dbHead[Sel].HeadSize,&h,2);     // ������ ������ ���������
    memcpy(&dbHead[Sel].RecLen,  &l,2);     // ������ ����� �����
    memcpy(&dbHead[Sel].HFld,    &f,2);     // ����� ����� � ���� ������
    dbHead[Sel].LibFlg[0]='V';              // �ਧ��� ᮧ����� �� �⮩ �������
    dbHead[Sel].LibFlg[1]='K';              //
    memcpy(&BRec[Sel][0],&dbHead[Sel],DBHDLEN); 
    //  ᮧ���� 䠩� ��
    if((FDesk=open(dbName,O_BINARY|O_RDWR|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE))<0){
        DPrn(13,1,"dbCreate: �訡�� ������ 䠩�� [%s] � ������ %d.\n",dbName,Sel);
        return(-23);
    }
    // ������� �������� 䠩�� ��
    write(FDesk,&BRec[Sel],cps);            // ������� ���� ���������
    // ������� 䠩� ��
    close(FDesk); 
    return(1);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  ��p��� DBF-䠩�
_i  dbOpen(_i Sel, uc *dbName){
    _i  FDesk=0, f=0;
    _l  ofs=0, hbr=0, cps=0, h=0;
    //  �஢���� ����� � 䠩�� ��
    if (access(dbName,6)!=0){ 
        DPrn(13,1,"dbOpen: � ������ %d ��� ����㯠 � 䠩�� [%s]\n",Sel,dbName); return(-31); 
    }
    //  �᫨ �� � 㪠������ ������ 㦥 �����
    if (dbOpenTest(Sel,0)>=0){ 
        DPrn(13,1,"dbOpen: � ������ %d �� 㦥 �����.\n"); return(-32); 
    }
    //  ������ 䠩�
    if((FDesk=open(dbName,O_BINARY|O_RDWR))<0){ 
        DPrn(13,1,"dbOpen: �訡�� ������ 䠩�� �� [%s] � ������ %d\n",dbName,Sel); return(-33); 
    }
    memset(BRead,0,DBBSIZ);
    //  ������ ��������� ���뢠����� 䠩��
    hbr=read(FDesk,&BRead,DBHDLEN+DBHDLEN*DBMAXFLD+DBBEXT);
    if (hbr<DBHDLEN*2){ 
        close(FDesk); 
        DPrn(13,1,"dbOpen: ���ࠢ���� ࠧ��� ��������� �� � ������ %d.\n",Sel); return(-34); 
    }
    memcpy(&dbHead[Sel],&BRead,DBHDLEN); cps=cps+DBHDLEN;
    //  ࠧ��� ��������� ��
    memcpy(&h,&dbHead[Sel].HeadSize,2);
    //  ࠧ��� ����� �����
    memcpy(&dbHeaw[Sel].RecLen,&dbHead[Sel].RecLen,2);
    //  �஢�ઠ ���४⭮�� ��⠭��� ������
    if((h<DBHDLEN)||(dbHeaw[Sel].RecLen<1)){ 
        close(FDesk); 
        DPrn(13,1,"dbOpen: ������ ��������� �� � ������ %d.\n",Sel); return(-35);
    }
    //  ��砫쭮� ���祭�� ��� ����� ⥪�饩 ����� �� ��।�����
    dbHeaw[Sel].CRec=-1;
    //  ������⢮ ����ᥩ � ��
    memcpy(&dbHeaw[Sel].RecCount,&dbHead[Sel].RecCount,4);
    //  ��砫쭮� ᬥ饭�� ��� ��ࢮ� �����
    dbHeaw[Sel].COfs=h;
    //  ��������� �������� ����� ��
    for(f=0;f<DBMAXFLD;f++){
        memcpy(&dbField[Sel][f],&BRead[cps],DBHDLEN); 
        h=h-DBHDLEN; cps=cps+DBHDLEN;
        if((dbField[Sel][f].Tip!='C'&&dbField[Sel][f].Tip!='L'&&
            dbField[Sel][f].Tip!='D'&&dbField[Sel][f].Tip!='M'&&
            dbField[Sel][f].Tip!='N'&&dbField[Sel][f].Tip!='F'&&
            dbField[Sel][f].Tip!='B'&&dbField[Sel][f].Tip!='I')||(h<=0)){ 
            break; 
        };
        //  ������ � �������� ᬥ饭�� ⥪�饣� ���� � �����
        memcpy(&dbField[Sel][f].OfsFld,&ofs,4); 
        dbFielw[Sel][f].OfsFld=ofs;              // ����� ⥪�� ᬥ饭��
        ofs=ofs+dbField[Sel][f].Len; 
    }
    dbHExt[Sel].Len=dbHeaw[Sel].COfs-DBHDLEN-(DBHDLEN*f);
    dbHeaw[Sel].FieldCount=f;
    //  �᫨ �� ��⠭�� �������� ���� - �� 䠩� �� ���� ������
    if (f<1){ 
        DPrn(13,1,"dbOpen: ��� ��⠭��� ����� �� � ������ %d.\n",Sel);
        close(FDesk); return(-36); 
    }
    //  �⥭�� ���७���� ���������
    if (dbHExt[Sel].Len>0){ 
        memcpy(&dbHExt[Sel].Buf,&BRead[cps],dbHExt[Sel].Len);
    }
    //  ������� � �������� ������⢮ ����� � ��
    memcpy(&dbHead[Sel].HFld,&f,2);
    //  ��⠭����� 㪠��⥫� �� �� ������ N0
    if (dbHeaw[Sel].RecCount>0){ dbHeaw[Sel].CRec=0; }
    //  ��।��� ��� 䠩�� � ���ਯ�� � ���ᨢ�
    strcpy(dbFile[Sel].Name,dbName); dbFile[Sel].Desk=FDesk; dbFile[Sel].Mod=0;
    //
    DPrn(13,1,"dbOpen: 䠩� �� [%s] ����� � ������ %d.\n",dbFile[Sel].Name,Sel);
    return(1);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  �����⨥ ��
vd  dbClose(_i Sel){
    _i  a=0;
    _l  cps=0;
    uc  he=13;
    DPrn(13,1,"dbClose: �����⨥ 䠩�� �� [%s] � ������ %d:\n",dbFile[Sel].Name,Sel);
    //  �ࠢ���� ����� ��
    if (dbOpenTest(Sel,0)<0){ return; }
    //  �᫨ � 䠩� �� �뫮 ����� ������- ������� ��� ��� ����䨪�� ���������
    if (dbFile[Sel].Mod== 0){
        DPrn(13,1,"dbClose: 䠩� �� [%s] ������ � ������ %d.\n",dbFile[Sel].Name,Sel);
        close(dbFile[Sel].Desk); dbClear(Sel); return;
    }
    //  ��������� ��������� ���� ������
    memcpy(&BRec[Sel],&dbHead[Sel],DBHDLEN); cps=cps+DBHDLEN;
    //  ��������� ���� ���� ������
    for(a=0;a<dbHeaw[Sel].FieldCount;a++){
        memcpy(&BRec[Sel][cps],&dbField[Sel][a],DBHDLEN); cps=cps+DBHDLEN;
    }
    // ��⠭����� �ਧ��� ����砭�� ���������
    memcpy(&BRec[Sel][cps],&he,1); cps=cps+1;
    //  ��������� ���७�� ��������
    if (dbHExt[Sel].Len>0){ 
        memcpy(&BRec[Sel][cps],&dbHExt[Sel].Buf,dbHExt[Sel].Len); 
        cps=cps+dbHExt[Sel].Len;
    }
    // 㪠��⥫� �� ��砫� 䠩��, ������� ��������� � ������� 䠩�
    lseek(dbFile[Sel].Desk,0,SEEK_SET);
    write(dbFile[Sel].Desk,BRec[Sel],cps);
    close(dbFile[Sel].Desk);
    DPrn(13,1,"dbClose: 䠩� �� [%s] ������ � ������ %d.\n",dbFile[Sel].Name,Sel);
    dbClear(Sel);
    return;
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  ����஢��� �������� ���� ������ � ���� 䠩�
_i  dbStructCopy(uc *dbSrc, uc *dbDst){
    _i  Sel=0, FDesk=0, hb=0, r=0;
    DPrn(13,1,"dbStructCopy: ��砫�:\n");
    //  ᮧ���� 䠩� ��� ����� �������� ����� ���� ������
    if((FDesk=open(dbDst,O_BINARY|O_RDWR|O_CREAT|O_TRUNC,S_IREAD|S_IWRITE))<0){
        DPrn(13,1,"dbStructCopy: �訡�� ������ 䠩�� [%s]:\n",dbDst);
        return(-1); 
    }
    //  ������ ��室��� ���� ������
    if((Sel=dbOpenTest(-1,dbSrc))<0){
        Sel=dbSelectAvail(); dbInit(Sel);
        if (dbOpen(Sel,dbSrc)<0){ 
            DPrn(13,1,"dbStructCopy: �訡�� ������ �� [%s][%d]:\n",dbSrc,Sel);
            close(FDesk); return(-2); 
        }
    }
    //  ��⠭����� 㪠��⥫� �� ��砫� ��室��� ���� ������
    lseek(dbFile[Sel].Desk,0,SEEK_SET);
    //  ����� ����� ��������� ���� ������ � ����
    hb=read (dbFile[Sel].Desk,BRead,dbHeaw[Sel].COfs);
    //  ������� ��������� � ���� 䠩�
    if(hb==dbHeaw[Sel].COfs){ 
        BRead[4]=0; BRead[5]=0; BRead[6]=0; BRead[7]=0; write(FDesk,BRead,hb);
    } else { 
        r=-3; 
    }
    //  ������� 䠩��
    dbClose(Sel); close(FDesk); DPrn(13,1,"dbStructCopy: �����:\n");
    return (r);
}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//  ��१����� 䠩�� �� �᪫��� �����, ����祭�� �� 㤠�����
_i  dbPack(uc *dbBase, uc *dbTemp){
    _i  a=0, SBase=0, STemp=0;
    _l hr=0;
    // ᮧ���� �६����� ���� ������
    if (dbStructCopy(dbBase,dbTemp)<0){ return(-1); }
    DPrn(13,1,"dbPack: ��砫�: *******************\n");
    // ��।����� ᢮������ ������� ��� ��� ������
    if((SBase=dbSelectAvail())<0){ DPrn(13,1,"ER:-2\n"); return(-2); }
    dbInit(SBase);
    if((STemp=dbSelectAvail())<0){ DPrn(13,1,"ER:-3\n"); return(-3); }
    dbInit(STemp);
    // �஢�ઠ �ࠢ��쭮�� ���� ������
    if (dbCheck(SBase,dbBase) <0){ DPrn(13,1,"ER:-4\n"); return(-4); }
    // ������ ���� ������
    if (dbOpen (SBase,dbBase)< 0){ DPrn(13,1,"ER:-5\n"); return(-5); }
    if (dbOpen (STemp,dbTemp)< 0){ dbClose(SBase); DPrn(13,1,"ER:-6\n"); return(-6); }
    // ������� ������⢮ ����ᥩ � ���� ������
    hr=dbRecCount(SBase)-1;
    // ��३� �� 1-� ������ ���� ������
    dbGoFirst(SBase);
    // ��� ������ ����� ���� ������
    for(a=0;a<hr;a++){
        // ���� ⥪���� ������ ���� ������
        dbRecRead(SBase);
        if(isRecDFL(SBase)==0){
           // �������� ������ �� �६����� ���� ������, �᫨ ��� �ਧ���� 㤠�.
           if(dbRecAdd (STemp)>0){
              // ������� ��⠭��� ������ �� �६����� ����
              dbRecCopy(SBase,STemp); dbRecWrite(STemp);
           }
        }
        // ��३� �� ᫥������ ������ ���� ������
        dbGoSkip(SBase,1); 
        // �᫨ ����� ����- ��� �� 横��
        if(dbEof(SBase)>0){ break; }
    }
    // ������� ����
    dbClose(SBase); 
    dbClose(STemp); 
    DPrn(13,1,"dbPack: �����: ********************\n");
    return(0);
}
//----------------------------------------------------------------------------
