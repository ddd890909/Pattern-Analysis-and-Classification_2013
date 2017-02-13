// UExpressorView.h : interface of the CUExpressorView class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef UEXPRESSORVIEW_H
#define UEXPRESSORVIEW_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Classifier.h"
#include "Trainer.h"


class CUExpressorView : public CView
{
protected: // create from serialization only
	CUExpressorView();
	DECLARE_DYNCREATE(CUExpressorView)

// Attributes
public:
	CUExpressorDoc* GetDocument();

// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUExpressorView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CUExpressorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	
	IplImage *image; //image to be dealed with
	IplImage *image2;
	int isdone;
	CImage cimg;     //image to be drawn
	CvCapture *capture;// capture used in video or camera

	int drawtype; //1: image  2:video/camera
	
	/* serve for multi-thread*/
	BOOL m_bKillThread;
	CWinThread *m_pThread;

	BOOL *lpKillThread;

	/* end */

	Classifier classify;
	Trainer train;

// Generated message map functions
protected:
	//{{AFX_MSG(CUExpressorView)
	afx_msg void OnOpenImage();//open image file
	afx_msg void OnOpenVideo();//open video file
	afx_msg void OnModelTrain();//train model
	afx_msg void OnPause();
	afx_msg void OnPlay();
	afx_msg void OnUpdatePause(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePlay(CCmdUI* pCmdUI);
	afx_msg void OnModelLoad();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in UExpressorView.cpp
inline CUExpressorDoc* CUExpressorView::GetDocument()
   { return (CUExpressorDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // UEXPRESSORVIEW_H