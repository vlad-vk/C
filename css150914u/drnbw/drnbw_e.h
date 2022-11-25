// coding: cp866
//---------------------------------------------------------------------------
// 䠩� ���������� � ���ᠭ�� ��६�����
#define  MNCB                           8           // ���-�� NCB
#define  LNCB                           504         // ������ ���� ���뫪�-�ਥ��
#define  LNCBMAX                        512         // ���� ��������� ������ ���� NCB

ex struct  NBRQ  {
   _i   cq[MRCH+1];                   // ����� ������� �� 㤠��� ��設�
   _i   ca[MRCH+1];                   // ����� ������� ��� �⢥�
}  NBRQ[MQR+1];
ex struct  NBST  {
   _i   cq[MRCH+1];                   // ����� ������� ��� ��⠭����
   _i   ca[MRCH+1];                   // ����� ������� ��� �⢥�
}  NBST[MQR+1];

ex ui   CurNCBQ;                      // ����� ⥪�饣� NCB �����
ex ui   CurNCFQ;                      // 䫠� ��।������ ������ NCB �����
ex ui   CurNCBS;                      // ����� ⥪�饣� NCB ���뫪�
ex ui   CurNCFS;                      // 䫠� ��।������ ������ NCB ���뫪�
ex uc   QRC;                          // �����  ⥪�饩 ��।� ��� �����
ex uc   STC;                          // �����  ⥪�饩 ��।� ��� ��⠭����
ex uc   WS_RD[MNCB+1][LNCBMAX];

ex ui   WS_NB__Init(ui);              // ���樠������ NetBIOS
ex ui   WS_NB__AddName(uc*);          // ���������� ����� � ᯨ᮪ ��
ex ui   WS_NB__DeleteName();          // 㤠�����  ����� �� ᯨ᪠ ��
ex ui   WS_NB__ReqToNet  (ui);        // ���뫪� ����ᮢ
ex ui   WS_NB__SetToNet  (ui);        // ���뫪� ��⠭����
ex uc   WS_NB__Cancel    (uc);        // �⬥�� ��᫥���� ������� Receive
ex _i   WS_NB__AvNCB     (vd);
ex ui   WS_NB__ReqToNet  (ui);
ex vd   WS_NB__GetFromNet(ui);
ex vd   WS_NB__GetReq    (ui);
ex vd   WS_NB__Answer    (ui,ui);
ex uc   WS_NB__GetRetCCode  (ui);
ex uc   WS_NB__GetFinalCCode(ui);
ex uc * WS_NB__GetCallName(ui);
ex vd   WS_NB__ZeroNCB(ui);
ex vd   WS_NB__Receive(ui);
ex vd   WS_NB__Send(ui,uc*);
ex vd   Quit_NB(vd);
//---------------------------------------------------------------------------
