// UExpressorDoc.cpp : implementation of the CUExpressorDoc class
//

#include "stdafx.h"
#include "UExpressor.h"
#include "UExpressorDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUExpressorDoc

IMPLEMENT_DYNCREATE(CUExpressorDoc, CDocument)

BEGIN_MESSAGE_MAP(CUExpressorDoc, CDocument)
	//{{AFX_MSG_MAP(CUExpressorDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUExpressorDoc construction/destruction

CUExpressorDoc::CUExpressorDoc()
{
	// TODO: add one-time construction code here
}

CUExpressorDoc::~CUExpressorDoc()
{

}

BOOL CUExpressorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CUExpressorDoc serialization

void CUExpressorDoc::Serialize(CArchive& ar)
{
//	int i, j, k, l;
	if (ar.IsStoring())
	{

	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CUExpressorDoc diagnostics

char** CUExpressorDoc::depicit = 0;

#ifdef _DEBUG
void CUExpressorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CUExpressorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CUExpressorDoc commands
