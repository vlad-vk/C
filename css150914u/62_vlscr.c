// coding: cp866
//---------------------------------------------------------------------------
// �뢮� ���祭�� ��६�����
_i ValScr(vd){
   _i a=0, b=0, s=0, j=0;
   //------------------------------------------------------------------------
   // �뢥�� ��६���� �� �࠭
   for(a=1;a<HVAL;a++){
      // �뢮���� �� �࠭, �᫨ ����� � ���� ������ > 0
      if (LVAL[a].nv==0){ continue; }
      // ���� ���� �������
      if (LVAL[a].na==0){ sprintf(LVAL[a].str,LVAL[a].sf,CB[LVAL[a].nv].v); }
      // �����쭠� ���� �������
      if (LVAL[a].na==1){ sprintf(LVAL[a].str,LVAL[a].sf,CBL[LVAL[a].nv]);  }
      // ���� ������� �ࠩ��� COM ���⮢
      if (LVAL[a].na==2){ sprintf(LVAL[a].str,LVAL[a].sf,CBS[LVAL[a].nv]);  }
      // ���� ������� �ࠩ��� NetBios
      if (LVAL[a].na==3){ sprintf(LVAL[a].str,LVAL[a].sf,CBN[LVAL[a].nv]);  }
      // ���� ������� �ࠩ��� TCP/IP
      if (LVAL[a].na==4){ sprintf(LVAL[a].str,LVAL[a].sf,CBT[LVAL[a].nv]);  }
      // ���ᨢ �६����� �१�� �७���
      if (LVAL[a].na==5){ sprintf(LVAL[a].str,LVAL[a].sf,VTRT[LVAL[a].nv]); }
      // ���ᨢ ��६����� �������� 'y'
      if (LVAL[a].na==6){ sprintf(LVAL[a].str,LVAL[a].sf, YCM[LVAL[a].nv]); }
      // ���ᨢ �६����� ��६�����
      if (LVAL[a].na==7){ sprintf(LVAL[a].str,LVAL[a].sf,RTMP[LVAL[a].nv]); }
      // �뢮���� �� �࠭, �᫨ ��⠭����� 䫠� �⮡ࠦ����
      if (LVAL[a].vw==0){ continue; }
      // ��㥬 䮭
      b=text_height(dFnt[LVAL[a].sn].dat);
      rectfill(scrsav, LVAL[a].X,
                       LVAL[a].Y,
                       LVAL[a].X+ LVAL[a].W,
                       LVAL[a].Y+ b,
                       LVAL[a].cg );
      // ����� 横� � ����� ��ப� �����뢠�� 0, ���� ������ ��ப� ��
      // �⠭�� ࠢ���, ��।������� LVAL[].W
      for(j=0;j<strlen(LVAL[a].str);j++){
         // ��।����� ������ ��ப�
         b=text_length(dFnt[LVAL[a].sn].dat,LVAL[a].str);
         // �᫨ ��ப� ������ - ����塞 ��᫥���� ᨬ���
         if (b>LVAL[a].W){ LVAL[a].str[strlen(LVAL[a].str)-1]=0; }
         else { break; }
      }
      // ��।����� 梥� �뢮�� ᨬ�����
      LVAL[a].cc=LVAL[a].cs;
      // ��।����� 梥� ᨬ����� �� ���室� ⥪�饣� ���祭�� �� �࠭���
      // (⮫쪮 ��� ��饩 ���� �������)
      if (LVAL[a].na==0){
         if (CB[LVAL[a].nv].v>CB[LVAL[a].nv].h){ LVAL[a].cc=LVAL[a].ce; }
         if (CB[LVAL[a].nv].v<CB[LVAL[a].nv].l){ LVAL[a].cc=LVAL[a].ce; }
//  DPrn(0,0,"Val=%f Str=[%s] LVAL[a].nv=%d a=%d CB.h=%f CB.l=%f \n",
//       CB[LVAL[a].nv].v, LVAL[a].str, LVAL[a].nv, a, CB[LVAL[a].nv].h,
//       CB[LVAL[a].nv].l);
      }
      text_mode(LVAL[a].cg);
      textout(((s>0)?pcxtmp:scrsav),dFnt[LVAL[a].sn].dat,LVAL[a].str,
                          LVAL[a].X,     LVAL[a].Y,  LVAL[a].cc);
   }
   //------------------------------------------------------------------------
   return(0);
}
//---------------------------------------------------------------------------
