// coding: cp866
//---------------------------------------------------------------------------
extern struct LPLZ {
   _i  na;                   //
   _i  nv;                   // ����� ��६�����
   _f  min;                  // ���   ���祭��
   _f  max;                  // ����  ���祭��
   _f  X;                    // ���न���� ���� ������ 㣫�
   _f  Y;
   _f  L;                    // ������ ����㭪� � ���ᥫ��
   _i  W;                    // �ਭ� ������
   _i  H;                    // ���� ������
   _i  G;                    // ���ࠢ����� ����㭪�
   _i  cb;                   // 梥� ࠬ�� (0-��� ࠬ��)
   _i  tb;                   // ⨯ (�ଠ) �뢮�� ����㭪�
                             // 1..5-�ଠ; +10-����᪠; +100-����� ��������
   uc  fb [SLEN];            // ��� 䠩�� ������
   uc  STR[SLEN];            // ��ப� �����
   _i  a;                    // 䫠� ��⨢���� ����㭪�
   _i  sf;                   // 䫠� ����㧪� �ࠩ⮢ ������
BITMAP *sp[3];               // �ࠩ�� ������ ����㭪��
BITMAP *ss;                  // ��࠭���� �࠭� ��� ����㭪���
   _f   x1;                  // ���न��� ���� ������ 㣫� ��࠭ ����㭪�
   _f   y1;
   _f   r1;
   _f   r2;
   _f   r3;
   _f   r4;
   _f   vl;
} LPLZ[MPLZ+1];
//---------------------------------------------------------------------------
extern _i HPLZ;              // ⥪�饥 ������⢮ ����㭪��
//---------------------------------------------------------------------------
extern vd Polzun(_i, _i);
//---------------------------------------------------------------------------
extern _i MCurPL;            // 䫠� ��宦����� ����� ��誨
//---------------------------------------------------------------------------
