// UExpressorDoc.h : interface of the CUExpressorDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_UEXPRESSORDOC_H__02008E90_4E27_4942_9A27_997DB6127480__INCLUDED_)
#define AFX_UEXPRESSORDOC_H__02008E90_4E27_4942_9A27_997DB6127480__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AdaBoost.h"

class CUExpressorDoc : public CDocument
{
protected: // create from serialization only
	CUExpressorDoc();
	DECLARE_DYNCREATE(CUExpressorDoc)

// Attributes
public:
	static	char **depicit;
	Boosting boost;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUExpressorDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CUExpressorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Attributes
protected:
	
// Generated message map functions
protected:
	//{{AFX_MSG(CUExpressorDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UEXPRESSORDOC_H__02008E90_4E27_4942_9A27_997DB6127480__INCLUDED_)
