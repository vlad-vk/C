// MyListCtrl.cpp : implementation file
//
#include "stdafx.h"
#include "OPCClient.h"

#include "MyListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl

CMyListCtrl::CMyListCtrl()
{
}

CMyListCtrl::~CMyListCtrl()
{
}

BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CMyListCtrl)
    ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw) 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl message handlers

//char   CMyListCtrl::FontNM[200]="Arial";
//char   CMyListCtrl::FontNM[200]="Tahoma";
//char   CMyListCtrl::FontNM[200]="Courier";
//char   CMyListCtrl::FontNM[200]="Courier New";
//char   CMyListCtrl::FontNM[200]="Fixedsys";
//char   CMyListCtrl::FontNM[200]="Terminal";
//char   CMyListCtrl::FontNM[200]="Times New Roman Cyr";
//char   CMyListCtrl::FontNM[200]="Lucida Console";
  char   CMyListCtrl::FontNM[200]="Microsoft Sans Serif";
int    CMyListCtrl::FontSZ=14;
DWORD  CMyListCtrl::FontCL=RGB(0,0,0);
int    CMyListCtrl::FontST=FW_NORMAL;


//----------------------------------------------------------------------------
void CMyListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult) 
{ 
    NMLVCUSTOMDRAW* pLVCustomDraw = (NMLVCUSTOMDRAW*)pNMHDR; 
    NMCUSTOMDRAW pLV = (NMCUSTOMDRAW) pLVCustomDraw->nmcd;
	LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;
	char    mySTR[500];
	int     ci  = 0;
   *pResult = 0;

    switch(pLVCustomDraw->nmcd.dwDrawStage) 
    { 
        case CDDS_PREPAINT: 
		{
               *pResult = CDRF_NOTIFYITEMDRAW;
               break; 
		}
        case CDDS_ITEMPREPAINT: 
		{
              *pResult = CDRF_NOTIFYSUBITEMDRAW;
			   break;
        }
       case  CDDS_SUBITEM | CDDS_PREPAINT | CDDS_ITEM:
		{
		// pLV.hdr.idFrom = IDC_�������_����
		// pLV.dwItemSpec = ����� �������� item-� (������)
		// pLVCustomDraw->iSubItem = ����� �������� �������
			   ci=pLVCustomDraw->iSubItem;
			   //------------------------------------------------------------
               if(pLV.hdr.idFrom==IDC_STAT){
				  if(ci==0){ pLVCustomDraw->clrText = RGB(0, 65,105); }
				  if(ci==1){ pLVCustomDraw->clrText = RGB(0,120,100); }
				  if(ci==2){ pLVCustomDraw->clrText = RGB(0,  0,220); }
                  strcpy(mySTR,CListCtrl::GetItemText(pLV.dwItemSpec,2));
				  if(CListCtrl::GetItemText(pLV.dwItemSpec,0)=="AddItems 1"){
				     // ���� ���� ������ �� ������������ �� �������-
					 // ������� �� ������ ������� ������
					 if(mySTR[0]!='O'&&mySTR[0]!=0){
 					    pLVCustomDraw->clrText = RGB(200,0,0);
					 }
                  }
				  // ���� ��� �����-�� �������� ���� �������� ������ (Error)
				  // ������� ������� ������� ������
				  if(mySTR[0]=='E' && ci==2){ 
					 pLVCustomDraw->clrText = RGB(200,0,0); 
				  }
               }
			   //------------------------------------------------------------
               if(pLV.hdr.idFrom==IDC_LIST1){ 
				  // ����� � ������ ������������ � PreSubclassWindow
                  SelectObject(pLVCustomDraw->nmcd.hdc, m_Font);
                  // ��-��������� ���� ������
				  pLVCustomDraw->clrText = RGB(0,0,0);
				  // ������ ������� �������� ����� ������
				  if(ci==0){ pLVCustomDraw->clrText = RGB(0,0,220); }
                  // ���� ����������(���) ���������- �������� �����
                  strcpy(mySTR,CListCtrl::GetItemText(pLV.dwItemSpec,4));
				  if(mySTR[0]=='M'){
                     pLVCustomDraw->clrText = RGB(0,110,5);
                  }
                  // ���� � ������� Quality �������� BAD- 
				  // ���������� ������ ������� ������
                  strcpy(mySTR,CListCtrl::GetItemText(pLV.dwItemSpec,8));
				  if(mySTR[0]=='B'){
                     pLVCustomDraw->clrText = RGB(200,0,0);
				  }
			   }
			   //------------------------------------------------------------
               if(pLV.hdr.idFrom==IDC_LIST2){ 
                     pLVCustomDraw->clrText = RGB(90,60,0);
               }    
               //------------------------------------------------------------
               break; 
		} 
        default:
			   ci=6;
              *pResult = CDRF_DODEFAULT;
			   break;
	}
} 
//----------------------------------------------------------------------------
 

//----------------------------------------------------------------------------
// ����������� ������ ��� ������ � ����� ��� ���������� ���������
void CMyListCtrl::PreSubclassWindow(){ 
    LOGFONT lf; 
    memset(&lf, 0, sizeof(LOGFONT)); 
    strcpy(lf.lfFaceName, FontNM);
    lf.lfHeight = CMyListCtrl::FontSZ;
    lf.lfWeight = CMyListCtrl::FontST; 
    VERIFY(m_Font.CreateFontIndirect(&lf));           

    CListCtrl::PreSubclassWindow(); 
} 
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
void CMyListCtrl::DoDataExchange(CDataExchange* pDX) 
{
	// TODO: Add your specialized code here and/or call the base class
	CListCtrl::DoDataExchange(pDX);
}
//----------------------------------------------------------------------------

