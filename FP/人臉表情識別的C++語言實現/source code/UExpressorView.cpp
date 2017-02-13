// UExpressorView.cpp : implementation of the CUExpressorView class
//

#include "stdafx.h"
#include "UExpressor.h"

#include "UExpressorDoc.h"
#include "UExpressorView.h"
#include "MainFrm.h"
#include "SideView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct TagParam
{
	CvCapture *capture;
	BOOL *killthread;
	HWND hWnd;
	Classifier *classify;
}tagParam;

unsigned int ThreadFunction( void* param );

/////////////////////////////////////////////////////////////////////////////
// CUExpressorView

IMPLEMENT_DYNCREATE(CUExpressorView, CView)

BEGIN_MESSAGE_MAP(CUExpressorView, CView)
	//{{AFX_MSG_MAP(CUExpressorView)
	ON_COMMAND(ID_OPEN_IMAGE, OnOpenImage)
	ON_COMMAND(ID_OPEN_VIDEO, OnOpenVideo)
	ON_COMMAND(ID_MODEL_TRAIN, OnModelTrain)
	ON_COMMAND(ID_PAUSE, OnPause)
	ON_COMMAND(ID_PLAY, OnPlay)
	ON_UPDATE_COMMAND_UI(ID_PAUSE, OnUpdatePause)
	ON_UPDATE_COMMAND_UI(ID_PLAY, OnUpdatePlay)
	ON_COMMAND(ID_MODEL_LOAD, OnModelLoad)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUExpressorView construction/destruction

CUExpressorView::CUExpressorView()
{
	// TODO: add construction code here
	image = 0;
	image2 = 0;
	isdone = 0;
	capture = 0;
	//capture = cvCaptureFromCAM(0);//obtain from camera
	drawtype = (capture == 0) ? 0 : 2;
	
	m_bKillThread = false;
	m_pThread = 0;
	
}

CUExpressorView::~CUExpressorView()
{
	//OnFinalRelease();
}

BOOL CUExpressorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CUExpressorView drawing

void CUExpressorView::OnDraw(CDC* pDC)
{
	CUExpressorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	if(!classify.IsLoad())
	{
		AfxMessageBox("Model hasn't been loaded, load it first!\n",MB_OK);
		return;
	}
	
	HDC hDC = pDC->GetSafeHdc();
	CRect rect(0,0,640,480);
	float sx,sy;			
	switch(drawtype)
	{
	case 1://image
		if(!isdone)
		{
			CUExpressorDoc::depicit = classify.DoRecognition(image);
			cimg.CopyOf(image);
			isdone = 1;
		}
		else cimg.CopyOf(image2);
		
		sx = cimg.Width()/640.;
		sy = cimg.Height()/480.;		
		if(sx<1.0f&&sy<1.0f) rect = CRect(0,0,cimg.Width(),cimg.Height());
		else rect = CRect(0,0,cimg.Width()/max(sx,sy),cimg.Height()/max(sx,sy));
		
		cimg.DrawToHDC(hDC,&rect);

		break;

	case 2://video/camera
		if(!capture)
		{
			AfxMessageBox("Cannot obtain video!");
			return;
		}
		if(m_pThread == 0)
		{
			tagParam.hWnd = m_hWnd;
			tagParam.killthread = &m_bKillThread;
		    tagParam.capture = capture;
		    tagParam.classify = &classify;
			m_pThread = AfxBeginThread(ThreadFunction, (void*)&tagParam);
		}
		break;
	default:
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CUExpressorView diagnostics

#ifdef _DEBUG
void CUExpressorView::AssertValid() const
{
	CView::AssertValid();
}

void CUExpressorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CUExpressorDoc* CUExpressorView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUExpressorDoc)));
	return (CUExpressorDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CUExpressorView message handlers

void CUExpressorView::OnOpenImage() 
{
	// TODO: Add your command handler code here
	char szFilter[] = "Image Files (*.bmp;*.jpg;*.tiff;*.gif)|*.bmp;*.jpg;*.tiff;*.gif|All Files (*.*)|*.*||";
	CFileDialog dlg(true, "bmp|jpg|jpeg|gif", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	dlg.m_ofn.lpstrTitle = "Please select a test image";
	
	CString filename;
		
	if(dlg.DoModal() == IDOK)	
	{
	    drawtype = 1;
		isdone = 0;
		filename = dlg.GetPathName();
		image = cvLoadImage(filename,1);
	
		Invalidate();
	}
}

void CUExpressorView::OnOpenVideo() 
{
	// TODO: Add your command handler code here
	char szFilter[] = "video Files (*.avi)|*.avi|All Files (*.*)|*.*||";
	CFileDialog dlg(true, "avi", 
		NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	dlg.m_ofn.lpstrTitle = "Please select a video file";
	
	if(dlg.DoModal() == IDOK)
	{
		drawtype = 2;
		CString filename = dlg.GetPathName();	
		if(m_pThread){
			m_bKillThread = false;
			m_pThread->SuspendThread();//Suspend Old Thread
		}
		m_pThread = 0;
		capture = cvCaptureFromAVI(filename);
		
		Invalidate();	
	}	
}

void CUExpressorView::OnModelLoad() 
{
	// TODO: Add your command handler code here
	char szFilter[] = "Model Files (*.model)|*.model|All Files (*.*)|*.*||";
	CFileDialog dlg(true, "model", 
		NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
	dlg.m_ofn.lpstrTitle = "Please select the trained model";
	
	if(dlg.DoModal() == IDOK)	
	{
		classify.LoadModel(dlg.GetPathName());
	}
}

void CUExpressorView::OnModelTrain() 
{
	// TODO: Add your command handler code here
	LPMALLOC pMalloc;
	char *path;
	if (::SHGetMalloc(&pMalloc) == NOERROR)
	{
		BROWSEINFO		bi;
		char			pszBuffer[MAX_PATH];
		LPITEMIDLIST	pidl;

		bi.hwndOwner = GetSafeHwnd();
		bi.pidlRoot  = NULL;
		bi.pszDisplayName = pszBuffer;
		bi.lpszTitle = _T("Please select the directory to be trained");
		bi.ulFlags   = BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS;
		bi.lpfn      = NULL;
		bi.lParam    = 0;

		if ((pidl=::SHBrowseForFolder(&bi)) != NULL)
		{
			if (::SHGetPathFromIDList(pidl, pszBuffer))
			{ 
				path = pszBuffer;
				train.TrainModel(path);
				AfxMessageBox("Train Done");
			}
		}
	} 
	
}

void CUExpressorView::OnPause() 
{
	// TODO: Add your command handler code here
	m_bKillThread = TRUE;
	m_pThread->SuspendThread();
}

void CUExpressorView::OnPlay() 
{
	// TODO: Add your command handler code here
	m_bKillThread = FALSE;
	m_pThread->ResumeThread();
}

void CUExpressorView::OnUpdatePause(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(drawtype == 2 && m_bKillThread == FALSE);
	
}

void CUExpressorView::OnUpdatePlay(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(drawtype == 2 && m_bKillThread == TRUE);
}

unsigned int ThreadFunction( void* param )
{
	IplImage *image;
	IplImage *image0 ;
	CImage cimg;
	CRect rect;
	HDC hdc ;
	int create = 0;
	clock_t t;
	double e;
	char e_time[100];
	char *showresult;
	TagParam *m_tagparam = (TagParam*)param;
	HWND hWnd = m_tagparam->hWnd;
	CvCapture *capture = m_tagparam->capture;
	Classifier *classify = m_tagparam->classify;
	
	while( (*m_tagparam->killthread) == false )
	{
		hdc = ::GetDC( hWnd );
		cvGrabFrame( capture );
		image = cvRetrieveFrame(capture );	
		while(image)
		{
			t = clock();
			// Do with each frame
			if(!create)
			{
				//first, initialize
				image0 = cvCreateImage(cvSize(image->width,image->height),IPL_DEPTH_8U, image->nChannels );
				rect = CRect(0,0,image->width,image->height);
				create = 1;
			}
			cvFlip( image, image0, 0 );
			try
			{
				showresult = classify->DoRecognition2(image0);
			}
			catch (...) 
			{
				AfxMessageBox("Error");
				throw;
			}
			//draw to screen
			cimg.CopyOf(image0);
			cimg.DrawToHDC(hdc,&rect);
		
			//compute time cost
			e = clock() - t;
			sprintf(e_time, "%g fap/s", 1000.0/(e+1));
			SetTextColor(hdc, RGB(255,0,0));
			TextOut(hdc, 0, image->height+10, e_time, strlen(e_time));

			// get next frame
			image = cvQueryFrame( capture );
		}		
		*m_tagparam->killthread = TRUE;
		cvReleaseImage(&image0);
	}
	return( 0 );
}



void CUExpressorView::OnFinalRelease() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(drawtype == 1)
		cvReleaseImage(&image);
	else if(drawtype == 2)
		cvReleaseCapture(&capture);
	else ;

	// Set the flag to kill the thread to TRUE
	m_bKillThread = true;
	// Wait for the thread to end
	::WaitForSingleObject( m_pThread->m_hThread, 100 );
	delete m_pThread;

	train.Free();
	classify.Free();
	CView::OnFinalRelease();
}
