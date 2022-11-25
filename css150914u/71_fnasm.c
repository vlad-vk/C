// coding: cp866
//---------------------------------------------------------------------------
//  ��ᥬ����� �㭪樨


//***************************************************************************
//  �㭪樨 ��� ࠡ��� � �ਭ�஬ �१ BIOS:

//---------------------------------------------------------------------------
//  ����஢�� ���ﭨ� �ਭ��
uc  PrnChkErr(uc b){
    _i r=0, LPETO=0, LPEIO=0, LPCON=0, LPEOP=0, LPNBS=0;
    LPETO=(b>>0) & 1;            // * �஢���� ��� 0 (1=TimeOut Error)
    LPEIO=(b>>3) & 1;            // * �஢���� ��� 3 (1=IO Error)
    LPCON=(b>>4) & 1;            //   �஢���� ��� 4 (1=connected)
    LPEOP=(b>>5) & 1;            // * �஢���� ��� 5 (1=Out Paper)
    LPNBS=(b>>7) & 1;            //   �஢���� ��� 7 (1=Not Busy)
    if(LPETO>0){ r=1; }          //   TimeOut
    if(LPEIO>0){ r=4; }          //   IOError
    if(LPEOP>0){ r=5; }          //   OutPaper
    if(LPCON<1){ r=6; }          //   NoConect
    if(LPNBS<1){ r=8; }          //   PrnBusy
    if(Debug(16)){ r=0; }
    return(r);
}
//---------------------------------------------------------------------------
//  ��᫠�� ᨬ��� � �ਭ��
uc  PrnChar(uc CurChar){
    uc  r=0;
    if (BIOS_FUNCTIONS==1){
        __asm__ (\
           "movb $0x0, %%ah\n"\
           "movb %1,   %%al\n"\
           "movw $0x0, %%dx\n"\
           "int  $0x17\n"\
           "movb %%ah, %0" 
        :  "=g" (r)
        :   "g" (CurChar)
        );
    }
    DPrn(DEBPRN,0,"%c",CurChar);
    return(PrnChkErr(r));
}
//---------------------------------------------------------------------------
//  �஢���� ����� �ਭ��
uc  PrnStat(vd){
    uc  r=0;
    if (BIOS_FUNCTIONS==1){
        __asm__ (\
           "movb $0x2, %%ah\n"\
           "movw $0x0, %%dx\n"\
           "int  $0x17\n"\
           "movb %%ah, %0" 
        :  "=g" (r)
        );
    }
    return(PrnChkErr(r));
}
//---------------------------------------------------------------------------
//  ���樠������ �ਭ��
uc  PrnInit(vd){
    uc  r=0;
    if (BIOS_FUNCTIONS==1){
        __asm__ (\
           "movb $0x1, %%ah\n"\
           "movw $0x0, %%dx\n"\
           "int  $0x17\n"\
           "movb %%ah, %0" 
        :  "=g" (r)
        );
    }
    return(PrnChkErr(r));
}
//---------------------------------------------------------------------------
//  ����� ��।���� ��ப� (� ०��� DOS �/� BIOS)
uc  PrnStr(uc *PrnBuf, _i m){
    _i  a=0,h=0;
    uc  r=0;
    h=strlen(PrnBuf); 
    if (m==0){ 
        for(a=0;a<h;a++) PrnChar(PrnBuf[a]);;
    } 
    return(r);
}
//***************************************************************************
