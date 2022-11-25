#if !defined(AFX_MYLISTCTRL_H__7730F92E_B1FD_46BF_80EB_0A8D0BD687EE__INCLUDED_)
#define AFX_MYLISTCTRL_H__7730F92E_B1FD_46BF_80EB_0A8D0BD687EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl window

class CMyListCtrl : public CListCtrl
{
// Construction
public:
	CMyListCtrl();
	static char  FontNM[200];
	static int   FontSZ;
	static DWORD FontCL;
	static int   FontST;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyListCtrl)
	protected:
	virtual void PreSubclassWindow();
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyListCtrl();
	// Generated message map functions

private:

protected:
    CFont           m_Font;

	//{{AFX_MSG(CMyListCtrl)
    afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult); 
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


};
//----------------------------------------------------------------------------


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYLISTCTRL_H__7730F92E_B1FD_46BF_80EB_0A8D0BD687EE__INCLUDED_)
