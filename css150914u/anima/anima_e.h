// coding: cp866
//---------------------------------------------------------------------------
// ������� ����㧪� 䠩��� �����樨
extern  struct  LANI{
   _i   vw;                  // 䫠� �⮡ࠦ���� �뢮��
   _i   na;                  // ����� ���ᨢ�
   _i   nv;                  // ����� ��६�� � ���ᨢ�, ���� ᮮ� ��� ��
   _i   nx;                  // ����� ��६����� ��� ��������� ���न���� X
   _i   ny;                  // ����� ��६����� ��� ��������� ���न���� Y
   _i   nf;                  // ����� ��६����� 䫠�� � ⮬ �� ���ᨢ�
   _i   X;                   // ���न��� �뢮�� (���孨� ���� 㣮�)
   _i   Y;                   //
   _i   XM;                  // ����筠� ���न��� �뢮��
   _i   YM;                  //
   _i   W;                   // �ਭ� �ࠩ� (����� ���⨭��)
   _i   H;                   // ���� �ࠩ�
   _i   hp;                  // ���� ���-�� �ࠩ⮢ ��� �⮡ࠦ����
   uc   fp[SLEN];            // ��� 䠩�� � ���⨭���� (�ࠩ⠬�)
BITMAP *sp[MASP];            // ���⨭�� �ࠩ⮢
BITMAP *ss;                  // �६���� ���� 䠩�� ���⨭�� �����樨
   _i   x1;                  // �������⥫�� ���न���� ��������
   _i   y1;                  // �ࠩ⮢
   _i   vx;                  // ��࠭���� ��砫�� 䫠� �⮡ࠦ����
   _i   sX;
   _i   sY;
}  LANI[MANI+1];
ex _i   HANI;                // ������⢮ ��⠭��� ��ப ��� ����㧪�...
//---------------------------------------------------------------------------
ex vd Animation(_i);
//---------------------------------------------------------------------------
