// coding: cp866
//---------------------------------------------------------------------------
   ex vd      DPrn (_i,_i, vktuc *, ...);
   ex _f      TOSERCNT;                 // ���-�� ⠩���⮢ CBL[25]
   ex _i      TOSER;                    // ⠩���� ��� �ਥ�� �� COM �����
   ex uc      STAT_str[SLEN];           // �뢮� ᮮ�饭�� ��� ���譨� �㭪権
   ex _i      STAT_cnt;                 // 䫠� �뢮�� ᮮ�饭�� ...
   ex uc      COMWSNum[2];              // ����� ������ ࠡ�祩 �⠭樨
   ex _i      Debug(_i);                // �஢�ઠ ���� ��� �� �⫠� ���
   ex ul      BDSER;                    // ᪮���� ������ �� COM �����
   ex _i      MTimer(ui, uc, ui);       // �㭪�� ⠩���
   ex _i      CurSP;                    // ����� ⥪�饣� COM ����
   ex _f      divf(_f, _f);
   ex _i      divi(_i, _i);
   ex vd      COMAnsVal(ui, uc);
//---------------------------------------------------------------------------
   uc SPSBuf[SPMAX][SPBSZ];             // ���� ���뫪�
   uc SPRBuf[SPMAX][SPBSZ];             // ���� �ਥ��
   uc SPTBuf[SPMAX][14];                // ���� ��।�� �ਭ���� ������
   uc ICTS[SPMAX]={0,0,0};              // ���� ����� �� ���뫪�
   uc ICTR[SPMAX]={0,0,0};              // ���� ����� �� �ਥ��
   uc ICTQ[SPMAX]={0,0,0};              // ���� ����� ���뢠���
   uc ICTM[SPMAX]={0,0,0};              // ���� ����� ������
   uc TMPB[SPMAX]={0,0,0};              // ���� ���樠����樨
   _i SPIO[SPMAX]={0x3f8,0x2f8,0x378};  // ���� ���⮢
   _i LSSB[SPMAX]={0,0,0};              // ������ ��ப� ���뫪�
   _i CSSB[SPMAX]={0,0,0};              // N ⥪�饣� ᨬ���� ���� ���뫪�
   _i CSRB[SPMAX]={0,0,0};              // N ⥪�饣� ᨬ���� ���� �ਥ��
   uc COMTmp[5]={0,0,0,0,0};            // ��ப� �८�ࠧ������
   uc COMCRC[SPMAX]={0,0,0};            // ����஫쭠� �㬬�
   uc COMCRCTmp[14];                    // �६���� ���� ������ CRC
   _i COMBusy[SPMAX]={0,0,0};           // 䫠� ࠡ��� � COM ���⮬
   _f COMCVal [SPMAX]={0,0,0};          // �ਭ��� ���祭�� ⥪��� �������
   _f COMSCVal[SPMAX]={0,0,0};          // ���祭�� ��� ��⠭����  �� ������
   _i COMCCh  [SPMAX]={0,0,0};          // ����� ⥪�饣� �ਭ�⮣� ������
   _i TOInp[SPMAX]={0,0,0};             // ⠩���� �������� �ਥ��
   _i COMHBR=0;                         // ᪮�쪮 ���� �뫮 ��⠭�� � ����
   _i SPRESULT[SPMAX]={0,0,0};          // ��� �訡�� �� ���� ���⮢
   _i SPFLBEG=-1;                       // 䫠� ��砫� �ਥ�� ����� ������

//---------------------------------------------------------------------------
#ifndef _VKSP_H_
#define _VKSP_H_
_i  VKSPInit      (ui);
vd  VKSPHand      (ui);
_i  VKSPSet (ui,ui,ui);
vd  VKSPStop      (vd);
us  VKSPOut    (ui,us);
_i  VKSPOutStat   (ui);
_i  VKSPInStat    (ui);
us  VKSPIn     (ui,us);
#endif
_i  COMGetCH(ui);
//---------------------------------------------------------------------------
// ������� ����� �� ��।��� ���祭�� �������
struct {
   uc   Begin;                          // 1
   uc   Addr[2];                        // 2
   uc   Code;                           // 1
   uc   nCH1[4];                        // 4
   uc   nCH2[4];                        // 4
   uc   CRC;                            // 1
   uc   End;                            // 1
}  COMREQ[SPMAX];                       // = 14 ����
//---------------------------------------------------------------------------
// ������� ����� �� ��⠭���� ���祭�� �������
struct {
   uc   Begin;                          // 1
   uc   Addr[2];                        // 2
   uc   Code;                           // 1
   uc   nCHS[4];                        // 4
   _f   Val __attribute__((packed));    // 4
   uc   CRC;                            // 1
   uc   End;                            // 1
}  COMSET[SPMAX];                       // = 14 ����
//---------------------------------------------------------------------------
// ������� �⢥� �� �����
struct {
   uc   Begin;                          // 1
   uc   Addr[2];                        // 2
   uc   Code;                           // 1
   uc   nCHA[4];                        // 4
   _f   Val   __attribute__((packed));  // 4
   uc   CRC;                            // 1
   uc   End;                            // 1
}  COMANS[SPMAX];                       // = 14 ����
//---------------------------------------------------------------------------
