// coding: cp866
//----------------------------------------------------------------------------
//
#define MAXSOCK              10        // ���� ���-�� ��蠥��� ᮪�⮢
#define LENFLD              100        // ���� ������ ��ப����� ����

//----------------------------------------------------------------------------
ex  uc  SRVIP[MWS+1][LENFLD];          // ���� IP �� ����� ���� �������� ������
ex  ui  SRVPORT[MWS+1];                // ����� ���⮢ ��� ����ᮢ
//----------------------------------------------------------------------------
ex  _i  TCP_init (void);
ex  _i  TCP_close(void);
ex  _i  TCP_ExitSRV;                   // 䫠� ��室� �� ����ணࠬ�
//----------------------------------------------------------------------------
