// coding: cp866
//---------------------------------------------------------------------------
ex _i VKSPInit(ui);
ex _i VKSPFifoInit(vd);
ex _i VKSPSet(ui,ui,ui);
ex vd VKSPHand(ui);
ex vd VKSPStop(vd);
us  VKSPOut    (ui,us);
_i  VKSPOutStat   (ui);
ex _i COMGetCH(ui);
ex vd COMPutCH(ui,_i,_i,_f);
ex vd COMReqCH(ui,_i,_i);
ex vd inp_COM(ui);
ex vd out_COM(ui);
ex vd ClearCB(ui);
//---------------------------------------------------------------------------
ex uc SPSBuf[SPMAX][SPBSZ];             // ���� ���뫪�
ex uc SPRBuf[SPMAX][SPBSZ];             // ���� �ਥ��
ex uc SPTBuf[SPMAX][14];                // ���� ��।�� �ਭ���� ������
ex uc ICTS[SPMAX];                      // ���� ����� �� ���뫪�
ex uc ICTR[SPMAX];                      // ���� ����� �� �ਥ��
ex uc ICTQ[SPMAX];                      // ���� ����� ���뢠���
ex uc ICTM[SPMAX];                      // ���� ����� ������
ex uc TMPB[SPMAX];                      // ���� ���樠����樨
ex _i SPIO[SPMAX];                      // ���� COM ���⮢
ex _i LSSB[SPMAX];                      // ������ ��ப� ���뫪�
ex _i CSSB[SPMAX];                      // N ⥪�饣� ᨬ���� ���� ���뫪�
ex _i CSRB[SPMAX];                      // N ⥪�饣� ᨬ���� ���� �ਥ��
ex uc COMTmp[5];                        // ��ப� �८�ࠧ������
ex uc COMCRC[SPMAX];                    // ����஫쭠� �㬬�
ex uc COMCRCTmp[14];                    // �६���� ���� ������ CRC
ex _i COMBusy[SPMAX];                   // 䫠� ࠡ��� � COM ���⮬
ex _f COMCVal [SPMAX];                  // �ਭ��� ���祭�� ⥪��� �������
ex _f COMSCVal[SPMAX];                  // ���祭�� ��� ��⠭����  �� ������
ex _i COMCCh  [SPMAX];                  // ����� ⥪�饣� �ਭ�⮣� ������
ex _i TOInp[SPMAX];                     // ⠩���� �������� �ਥ��
ex _i SPRESULT[SPMAX];                  // ��� �訡�� �� ���� ���⮢
//---------------------------------------------------------------------------
// ������� ����� �� ��।��� ���祭�� �������
ex struct {
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
ex struct {
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
ex struct {
   uc   Begin;                          // 1
   uc   Addr[2];                        // 2
   uc   Code;                           // 1
   uc   nCHA[4];                        // 4
   _f   Val   __attribute__((packed));  // 4
   uc   CRC;                            // 1
   uc   End;                            // 1
}  COMANS[SPMAX];                       // = 14 ����
//---------------------------------------------------------------------------
