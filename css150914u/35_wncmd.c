// coding: cp866
//---------------------------------------------------------------------------
// �믮������ ������� � �㭪権 Windows
_i WndCmd(_i nCmd){
   _i     a=0, rx=0, ry=0;
   LPSTR  TextBuff;
   CODUSWnd=win_get_window();
   //------------------------------------------------------------------------
   //  ᢥ���� �࠭ �� ������ Windows
   if (nCmd==1){ 
       //--------------------------------------------------------------------
       // �����⨥ ⥪�饣� ���� ��⥬ ��४��祭�� �� ��㣮� ����
       // (����� ���⮩ �ணࠬ��)
       if(access("codusz.exe",0)==0){ WinExec("codusz.exe",2); }
       //--------------------------------------------------------------------
       // �����⨥ ���� �⠭����� ᯮᮡ��
       else                         { CloseWindow(CODUSWnd); }
   }
   //------------------------------------------------------------------------
   if (nCmd==2){
       //--------------------------------------------------------------------
       // �몫�祭�� ������
//     SendMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER,2);    
       //--------------------------------------------------------------------
       // ����� ���譥� �ணࠬ��
//     ShellExecute(CODUSWnd,"Open","calc.exe",NULL,NULL,SW_SHOWNORMAL);
   }     
   //------------------------------------------------------------------------
   // ������� ���न���� ����� � ���� ������
   if (nCmd==3||nCmd==4){
       //  ALT+RMouseKey
       if (nCmd==3){
           for(a=0;a<MSCSCB;a++){ CBSCBuff[a]=0; }
           if(strlen(CBWRBuff)>SLEN-20){ for(a=0;a<SLEN;a++) CBWRBuff[a]=0; };
           if(KCT>0){ rx=mouse_x-CBM_X; ry=mouse_y-CBM_Y; }
           else     { rx=mouse_x;       ry=mouse_y;       }
           sprintf(CBSCBuff,"   %d %d",rx,ry);
           strcat (CBWRBuff,CBSCBuff);
       } else {
       //  ALT+LMouseKey
           for(a=0;a<SLEN;a++){ CBWRBuff[a]=0; }
           sprintf(CBWRBuff,   "%d %d",mouse_x,mouse_y);
           CBM_X=mouse_x; CBM_Y=mouse_y;
       }
       // ������� �������� ���� �����
       CBData=GlobalAlloc(GHND,SLEN);
       // ��䨪�஢��� ���
       TextBuff=(LPSTR)GlobalLock(CBData);
       // ������� ����� � ���� �����
       lstrcpy(TextBuff, CBWRBuff);
       // ��䨪�஢��� ���� �����
       GlobalUnlock(CBData);
       // ������ ��������
       OpenClipboard (CBFWnd);
       // ������ ��������
       EmptyClipboard();
       // ��।����� ����� � �������थ
       SetClipboardData(CF_TEXT,CBData);
       // ������� ��������
       CloseClipboard();
   }
   //------------------------------------------------------------------------
   return(nCmd);
}
//---------------------------------------------------------------------------




//----------------------------------------------------------------------------
HFONT WPRN_Font(HDC hdc, TCHAR * szFaceName, int iDeciPtHeight,int iDeciPtWidth, int iAttributes, BOOL fLogRes)
{
    FLOAT       cxDpi, cyDpi;
    HFONT       hFont;
    LOGFONT     lf;
    POINT       pt;
    TEXTMETRIC  tm;

    SaveDC (hdc);
    SetGraphicsMode     (hdc, GM_ADVANCED);
    ModifyWorldTransform(hdc, NULL, MWT_IDENTITY);
    SetViewportOrgEx    (hdc, 0, 0, NULL);
    SetWindowOrgEx      (hdc, 0, 0, NULL);

    if (fLogRes){
        cxDpi = (FLOAT) GetDeviceCaps (hdc, LOGPIXELSX);
        cyDpi = (FLOAT) GetDeviceCaps (hdc, LOGPIXELSY);
    } else  {
        cxDpi = (FLOAT) (25.4 * GetDeviceCaps (hdc, HORZRES) / GetDeviceCaps (hdc, HORZSIZE));
        cyDpi = (FLOAT) (25.4 * GetDeviceCaps (hdc, VERTRES) / GetDeviceCaps (hdc, VERTSIZE));
    }
    pt.x = (int) (iDeciPtWidth  * cxDpi / 72);
    pt.y = (int) (iDeciPtHeight * cyDpi / 72);
    DPtoLP (hdc, &pt, 1) ;

    lf.lfHeight = - (int) (fabs (pt.y) / 10.0 + 0.5) ;
    lf.lfWidth          = 0 ;
    lf.lfEscapement     = 0 ;
    lf.lfOrientation    = PRN_Forn;
    lf.lfWeight         = PRN_Fbls;
    lf.lfItalic         = PRN_Fitl;
    lf.lfUnderline      = PRN_Fund;
    lf.lfStrikeOut      = PRN_Fsou;
    lf.lfCharSet        = PRN_Fchr;
    lf.lfOutPrecision   = 0 ;
    lf.lfClipPrecision  = 0 ;
    lf.lfQuality        = PRN_Fqua;
    lf.lfPitchAndFamily = 0 ;
    lstrcpy (lf.lfFaceName, szFaceName) ;
    hFont = CreateFontIndirect (&lf) ;

    if (iDeciPtWidth != 0){
        hFont = (HFONT)SelectObject(hdc, hFont) ;
        GetTextMetrics  (hdc, &tm) ;
        DeleteObject    (SelectObject (hdc, hFont)) ;
        lf.lfWidth  =   (int)(tm.tmAveCharWidth * fabs (pt.x) / fabs (pt.y) + 0.5) ;
        hFont       =   CreateFontIndirect (&lf) ;
    }
    RestoreDC (hdc, -1) ;
    return hFont ;
}
//----------------------------------------------------------------------------


//---------------------------------------------------------------------------
//  c���ﭨ� ��।� ����
_i  WPRN_GetQueue(void){
    _i r=0;
    HANDLE          hPrn;
    DWORD           cByteNeeded, nReturned, cByteUsed, pStatus;
    PRINTER_INFO_2  *pPrinterInfo   = NULL;
    JOB_INFO_2      *pJobStorage    = NULL;
    // ���ਯ�� �ਭ��
   if(!OpenPrinter(TEXT(PRN_Name),&hPrn,NULL)){ return(107); }
    // ������� ����室��� ࠧ��� ����� ��� ���ଠ樨 � �ਭ��
    if(!GetPrinter(hPrn,2,NULL,0,&cByteNeeded)){
       if(GetLastError()!=ERROR_INSUFFICIENT_BUFFER){ ClosePrinter(hPrn); return(101); }
    }
    pPrinterInfo=(PRINTER_INFO_2*)malloc(cByteNeeded);
    if(!(pPrinterInfo)){ ClosePrinter(hPrn); return(102); }
    // ������� ���ଠ�� � �ਭ��
    if(!GetPrinter(hPrn,2,(LPSTR)pPrinterInfo,cByteNeeded,&cByteUsed)){
       ClosePrinter(hPrn); free(pPrinterInfo); pPrinterInfo=NULL; return(103);
    }
    // ������� ����室��� ࠧ��� ����� ��� ᯨ᪠ �������
    if(!EnumJobs(hPrn,0,pPrinterInfo->cJobs,2,NULL,0,(LPDWORD)&cByteNeeded,(LPDWORD)&nReturned)){
       if(GetLastError()!=ERROR_INSUFFICIENT_BUFFER){
          ClosePrinter(hPrn); free(pPrinterInfo); pPrinterInfo=NULL; return(104);
    }  }
    // �뤥���� ������ ��� ���ଠ樨 � ��������
    pJobStorage=(JOB_INFO_2*)malloc(cByteNeeded);
    if(!pJobStorage){ ClosePrinter(hPrn); free(pPrinterInfo); pPrinterInfo=NULL; return(105); }
    ZeroMemory(pJobStorage, cByteNeeded);
    // ������� ᯨ᮪ �������
    if(!EnumJobs(hPrn,0,pPrinterInfo->cJobs,2,(LPBYTE)pJobStorage,
                cByteNeeded,(LPDWORD)&cByteUsed,(LPDWORD)&nReturned)){
        ClosePrinter(hPrn); free(pPrinterInfo); free(pJobStorage); 
        pJobStorage=NULL; pPrinterInfo=NULL; return(106);
    }
    // �஢���� ����� �ਭ��
    pStatus=pPrinterInfo->Status;
    if(pStatus&(PRINTER_STATUS_ERROR|PRINTER_STATUS_PAPER_JAM|PRINTER_STATUS_PAPER_OUT|
       PRINTER_STATUS_PAPER_PROBLEM |PRINTER_STATUS_OUTPUT_BIN_FULL|
       PRINTER_STATUS_NOT_AVAILABLE |PRINTER_STATUS_NO_TONER|PRINTER_STATUS_OUT_OF_MEMORY|
       PRINTER_STATUS_OFFLINE|PRINTER_STATUS_DOOR_OPEN)){
        ClosePrinter(hPrn); free(pPrinterInfo); free(pJobStorage); 
        pJobStorage=NULL; pPrinterInfo=NULL; return(108);
    }
    // �஢���� ������ ������� � ��।� �� �訡��
    for(r=0;r<nReturned;r++){
       if(pJobStorage[r].Status&&JOB_STATUS_PRINTING){
          // �᫨ ������� � ���ﭨ� �訡��- ������ �訡��
          if(pJobStorage[r].Status &
            (JOB_STATUS_ERROR|JOB_STATUS_OFFLINE|JOB_STATUS_PAPEROUT|JOB_STATUS_BLOCKED_DEVQ)){
             r=100; break;
          }
        }
    }
    ClosePrinter(hPrn); free(pPrinterInfo);
    // �᫨ �訡�� ��� � ��।� ����� r=0
    if(r<100){ r=(ui)nReturned; }
//  printf("nQueue=%d\n",r);
   return(r);
}
//---------------------------------------------------------------------------
// ����⨥ �ਭ�� (��砫� ����)
_i WPRN_Start(void){
   _i r=0;
    // ������ ����� � �ਭ���
    hdcPrint = CreateDC(TEXT("WINSPOOL"), TEXT(PRN_Name), NULL, NULL);
    ZeroMemory(&doci ,sizeof(DOCINFO));
    doci.cbSize = sizeof(DOCINFO);
    doci.lpszDocName = TEXT("Codus Print");
    doci.fwType = 0;
    PRN_X=PRN_offX;
    PRN_Y=PRN_offY;
    // ���⮢��� ����� ���㬥�� � ��࠭���
    StartDoc (hdcPrint, &doci);
    StartPage(hdcPrint);
    // ��।����� ⥪�騥 ��ࠬ���� ���� (������ ��砫��� ���)
    SetTextAlign    (hdcPrint, GetTextAlign(hdcPrint) | TA_UPDATECP);
    GetTextMetrics  (hdcPrint, &txtm);
    // ��⠭����� ���� ��� ����
    hFont=WPRN_Font (hdcPrint, TEXT(PRN_Font), PRN_Fsiz, 0, 0, TRUE);
    hOldFont=SelectObject(hdcPrint,hFont);
    // ��砫�� ����� (X,Y)
    MoveToEx(hdcPrint, PRN_X, PRN_Y, NULL);
    // �᫨ ��� �訡�� �ਭ�� � ��।� ���� �����- ࠧ���� ����� �� �ਭ��
    PRN_Stat=1; 
   return(r);
}
//---------------------------------------------------------------------------
// ����� �� �ਭ��
_i WPRN_Write(void){
   int    r=0;
   if (PRN_Stat>0){
        MoveToEx(hdcPrint, PRN_X, PRN_Y += txtm.tmHeight + txtm.tmExternalLeading, NULL);
        TextOut (hdcPrint,     0, 0, TEXT(PRN_Cmd), lstrlen(TEXT(PRN_Cmd)));
   }
   return(r);
}
//---------------------------------------------------------------------------
// �����⨥ �ਭ�� (����� ����)
_i WPRN_Stop(void){
   _i r=0;
   if (PRN_Stat>0){
        EndPage (hdcPrint);
        EndDoc  (hdcPrint);
        DeleteDC(hdcPrint);
        PRN_Stat=0;
   }
   return(r);
}
//---------------------------------------------------------------------------
