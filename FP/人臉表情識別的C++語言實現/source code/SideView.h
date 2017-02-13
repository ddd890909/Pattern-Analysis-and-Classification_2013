#if !defined(AFX_SIDEVIEW_H__8EC46575_DB48_47CF_BFA1_6CC08DDD9757__INCLUDED_)
#define AFX_SIDEVIEW_H__8EC46575_DB48_47CF_BFA1_6CC08DDD9757__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SideView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// SideView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class SideView : public CFormView
{
protected:
	DECLARE_DYNCREATE(SideView)

// Form Data
public:
		SideView();           // protected constructor used by dynamic creation

	//{{AFX_DATA(SideView)
	enum { IDD = IDD_FORMVIEW };
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
//	CUExpressorDoc* GetDocument();
	CDC *pdc;
	// Operations
public:
	void Display(char * str);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SideView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~SideView();	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(SideView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIDEVIEW_H__8EC46575_DB48_47CF_BFA1_6CC08DDD9757__INCLUDED_)
