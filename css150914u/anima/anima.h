// coding: cp866
//---------------------------------------------------------------------------
ex PALLETE pal;              // ������ ����㦥���� ���⨭��
ex BITMAP* scrsav;           // ���� ��� ��࠭���� ��࠭�
ex _f Arr_to_Val(_i,_i);     // ����祭�� ���祭�� ������� �� ���ᨢ��
ex vd Val_to_Arr(_i,_i,_f,_i,uc); // ��⠭���� ���祭�� � ���ᨢ�� �������
ex vd      DPrn (_i,_i, vktuc *, ...);
ex _i      XMAX;             // ���ᨬ���� ���न���� ��࠭�
ex _i      YMAX;             //
ex uc      STAT_str[SLEN];   // �뢮� ᮮ�饭�� ��� ���譨� �㭪権
ex _i      STAT_cnt;         // 䫠� �뢮�� ᮮ�饭�� ...
//---------------------------------------------------------------------------
// ������� ����㧪� 䠩��� �����樨
struct  LANI{
   _i   vw;                  // 䫠� �⮡ࠦ���� �뢮��
   _i   na;                  // ����� ���ᨢ�
   _i   nv;                  // ����� ��६�� � ���ᨢ�, ���� ᮮ� ��� ��
   _i   nx;                  // ����� ��६����� ��� ��������� ���न���� X
   _i   ny;                  // ����� ��६����� ��� ��������� ���न���� Y
   _i   nf;                  // ����� ��६����� 䫠�� � ⮬ �� ���ᨢ�
   _i   X;                   // ��� ���न��� �뢮�� (���孨� ���� 㣮�)
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
   _i   HANI=0;              // ������⢮ ��⠭��� ��ப ��� ����㧪�...
BITMAP *pcxani;              // ���� 䠩�� ���⨭�� �����樨
//---------------------------------------------------------------------------
