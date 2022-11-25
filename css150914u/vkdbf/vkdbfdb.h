// coding: cp866
//----------------------------------------------------------------------------
#define  DBHDLEN    32            // ������ ��������� DBF 䠩��
#define  DBFLDLN    32            // ������ ���� ��
//----------------------------------------------------------------------------
//  ���ᠭ�� �������� ����७��� �㭪権
int dbOpenTest(int, uc*);
//----------------------------------------------------------------------------
//  ࠡ�稥 ��६���� � ���ᨢ�
struct   tm        *dbtm;
time_t   dbTMV=0;                 // �������� ��� ����� ⥪�� �६���
uc  BHead[DBHDLEN];               // ���� �⥭�� ���������
uc  BRead[DBBSIZ];                // ���� ��� �⥭�� ������
uc  BRec [DBMAXDBF][MRECLEN];     // ���� ����� �����
uc  BCRD [DBMAXDBF][1];           // �ਧ���� 㤠����� ����ᥩ
//----------------------------------------------------------------------------
uc  BSFld[5][DBSLEN];             // ���� ��� ����७��� ࠡ�� � ��ப���
_i  dbDebug=0;                    // 䫠� �뢮�� �⫠��筮� ���ଠ樨
//----------------------------------------------------------------------------
//  ������� ���� 䠩��� 
struct {
       uc  Name[DBSLEN];          // ��� 䠩�� ��
       _i  Desk;                  // ���ਯ�� (��⥬�� �����)
       _i  Mod;                   // 䫠� ����䨪�樨 䠩�� 1-�뫠 ����䨪���
       _i  Init;                  // 䫠� ���樠����樨 ���ᨢ�
} dbFile[DBMAXDBF];
//----------------------------------------------------------------------------
//  ������� ��������� ���� ������
struct {
       uc  ID;                    // [1] �����䨪��p
       uc  Year;                  // [3] ��� ��᫥����� ����������
       uc  Mon;
       uc  Day;
       uc  RecCount[4];           // [4] ������⢮ ����ᥩ ������ 㤠�����
       uc  HeadSize[2];           // [2] ������ ����� ���������
       uc  RecLen[2];             // [2] ����� ����� �����
       uc  LibFlg[2];             // [2] 䫠� ᮧ����� �� �⮩ ������⥪�� (LB)
       uc  Tranz;                 // [1] 0/1 ����p��� �� �p�����樨 ��� DB4
       uc  HFld[2];               // [2] ������⢮ ����� � �� (LB)
       uc  CRec[4];               // [4] ����� ⥪�饩 ����� � �� (LB)
       uc  COfs[4];               // [4] ᬥ饭�� � 䠩�� ��� ⥪�� �����(LB)
       uc  Rezerv1[2];            // [2]
       uc  Rezerv2[1];            // [1]
       uc  MDX;                   // [1] ������祭 �� 䠩� ������� ������ MDX
       uc  Reserv3[3];            // [3] १��.
} dbHead[DBMAXDBF];               // >>> �ᥣ� 32 ����.
//----------------------------------------------------------------------------
//  ������� ���७���� ���������
struct {
       uc  Buf[DBBEXT];           // ���७�� ��������� 䠩���
       _i  Len;                   // ������ ���७���� ���������
} dbHExt[DBMAXDBF];
//----------------------------------------------------------------------------
//  ࠡ��� ������� ��������� ���� ������
struct {
       _l  RecCount;              // ������⢮ ����ᥩ � ��
       _i  RecLen;                // ������ ����� �����
       _l  CRec;                  // ����� ⥪�饩 ����� � ��
       _l  COfs;                  // ��砫 ᬥ饭 � 䠩�� ��� ⥪�� �����
       _i  Bof;                   // 䫠� ����⪨ ��室� �� ��砫� ��
       _i  Eof;                   // 䫠� ����⪨ ��室� �� ����� ��
       _i  FieldCount;            // ������⢮ ����� � ����⮩ ��
} dbHeaw[DBMAXDBF];
_i     CurDBF=0;                  // ����� ⥪�饩 ��
//----------------------------------------------------------------------------
//  ������� ����� ���� ������
struct {
       uc  Name[11];              // [11] ���,�����稢�����
       uc  Tip;                   // [1]  ⨯ C,L,D,M,N,F
       uc  OfsFld[4];             // [4] 㪠��⥫� �� ���� ����� �����
       uc  Len;                   // [1] ����� ����
       uc  Dec;                   // [1] ������⢮ ������ ��᫥ ����⮩
       uc  Rezer1[2];             // [2] ᬥ饭�� ����
       uc  Rezer2[11];            // [11] 
       uc  TegMTX;                // [1] 0/1 ����� ⥣� ����������� 䠩� MTX ��� DB4
} dbField[DBMAXDBF][DBMAXFLD];    // �ᥣ� 32 ���� ��� ������ ����
//----------------------------------------------------------------------------
//  ࠡ��� ������� ����� ���� ������
struct {
       _l  OfsFld;                // 㪠��⥫� �� ���� ����� �����
} dbFielw[DBMAXDBF][DBMAXFLD];
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//  ࠡ��� ������� ��� ࠡ��� � ���ࠬ� ��� ������
struct {
       uc  Arr[DBBSIZ];           // ���� ��� ࠡ��� � �����
       _i  RecCount;              // ������⢮ ����ᥩ � ����
       _i  RecCur;                // ⥪��� ������ � ����
       _i  RecLen;                // ������ ����� � ���� (�� ����
                                  // ���������� ��᫥ ����権 �⥭��/�����)
       _i  Init;                  // 䫠� ���樠����樨 (��墠�) ����
       _i  Bof;                   // 䫠� ��室� �� ������ �࠭��� ����
       _i  Eof;                   // 䫠� ��室� �� ������ �࠭��� ����
} dbBUFF  [DBMAXBUFF];
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
ex _f     divf(_f, _f);
ex _i     divi(_i, _i);
ex vd     DPrn(_i, _i, uc *, ...);
//----------------------------------------------------------------------------