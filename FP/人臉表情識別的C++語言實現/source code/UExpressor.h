// UExpressor.h : main header file for the UEXPRESSOR application
//

#if !defined(AFX_UEXPRESSOR_H__051DC47D_B7AE_4430_9880_EE0F4F9118C9__INCLUDED_)
#define AFX_UEXPRESSOR_H__051DC47D_B7AE_4430_9880_EE0F4F9118C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CUExpressorApp:
// See UExpressor.cpp for the implementation of this class
//

class CUExpressorApp : public CWinApp
{
public:
	CUExpressorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUExpressorApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CUExpressorApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UEXPRESSOR_H__051DC47D_B7AE_4430_9880_EE0F4F9118C9__INCLUDED_)
