// SideView.cpp : implementation file
//
#include "stdafx.h"
#include "uexpressor.h"
#include "SideView.h"
#include "UExpressorDoc.h"
#include "UExpressorView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SideView

IMPLEMENT_DYNCREATE(SideView, CFormView)

SideView::SideView()
	: CFormView(SideView::IDD)
{
	//{{AFX_DATA_INIT(SideView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

SideView::~SideView()
{
}

void SideView::OnDraw(CDC* pDC) 
{
	CUExpressorDoc* pDoc =(CUExpressorDoc*) GetDocument();
	ASSERT_VALID(pDoc);

	
	
	/*
	CPen MyPen;
	CPen* pOldPen;
    CBrush MyBrush;
	CBrush* pOldBrush;

	CRect r (0,0,100,200);

	MyBrush.CreateSolidBrush(RGB(0,0,255));
	pOldBrush=pDC->SelectObject(&MyBrush);
	pDC->FillRect(r, &MyBrush);
	
	MyPen.CreatePen(PS_SOLID,1,RGB(255,0,255));
	pOldPen=pDC->SelectObject(&MyPen);	
	pDC->MoveTo(CPoint(100,100));
	pDC->LineTo(CPoint(200,300));

	pDC->SelectObject(pOldPen);
	pDC->MoveTo(CPoint(200,100));
    pDC->LineTo(CPoint(200,300));  
    */
	
	pDC->SetTextColor(RGB(255,0,0));
	
	char** depicit= CUExpressorDoc::depicit;
	int i = 0;
	if(depicit){
		while(depicit[i])
		{
			pDC->TextOut(0,i*20,depicit[i]);
			free(depicit[i]);
			i++;
		}
		free(depicit);
	}
	
}

void SideView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SideView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SideView, CFormView)
	//{{AFX_MSG_MAP(SideView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SideView diagnostics

#ifdef _DEBUG
void SideView::AssertValid() const
{
	CFormView::AssertValid();
}

void SideView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// SideView message handlers

void SideView::Display(char *str)
{
	//HDC hdc = ::GetDC( m_hWnd );
	
//	CDC* pDC = CDC::FromHandle(hdc);

//	::InvalidateRect( m_hWnd, NULL, TRUE );
//	::UpdateWindow( m_hWnd );

//	CClientDC   dc(this);  
//	OnPrepareDC(&dc);         //就得到与OnDraw中pDC一样的dc了 
	//DrawTextA(dc,str,10,CRect(600,600,10,100),0);
//	TextOut(dc,0,0,str,10);
/*
	  CDC   *   pDc;  
      CMainFrame   *pFrame   =   (CMainFrame*)(AfxGetApp()->m_pMainWnd);  
	  SideView   *pView   =   (SideView*)(pFrame->GetActiveView());  
	  pDc   =   pView->GetDC();
*/
//	CDC   *pDC=GetDC();
//	pDC->TextOut(10,10,str);

	CRect rect;
	CClientDC   dc(this);  
	GetClientRect(&rect);
	//CDC* pDC=GetDC();

    CBrush MyBrush;
	CBrush* pOldBrush;

	CRect r (0,0,100,200);

	MyBrush.CreateSolidBrush(RGB(0,0,255));
	pOldBrush=dc.SelectObject(&MyBrush);
	
	FillRect(dc,/*CRect(360,480,rect.bottom,rect.right)*/r, MyBrush);

}
