#include "stdafx.h"
#include "UI_EXAMPLE.h"
#include "MainFrm.h"
#include "ViewWnd.h"
#include "ToolWnd.h"
#include "DTIclass.h"
#include "DTIDIB.h"
#include "FLAIRDIB.h"
#include "FLAIRclass.h"
#include "FACAL.h"
#include "FADIB.h"
#include "FILEPROC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

CMainFrame::CMainFrame()
{
	a			= 0;
	b			= 0;
	c			= 0;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	viewWnd		= new ViewWnd;
	toolWnd		= new ToolWnd;
	tfileopen	= new FLAIRclass;
	tdib		= new FLAIRDIB;
	fileopen	= new DTIclass;
	dib			= new DTIDIB;
	fa			= new FACAL;
	fadib		= new FADIB;
	fileproc	= new FILEPROC;
	if (! viewWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, ID_VIEWWND,NULL))
	{
		TRACE0("Failed to creat view window\n");
			return -1;
	}

	if (! toolWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, ID_TOOLWND,NULL))
	{
		TRACE0("Failed to creat tool window\n");
			return -1;
	}
	if (! dib->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, ID_VIEWWND,NULL))
	{
		TRACE0("Failed to creat view window\n");
		return -1;
	}
	if (! tdib->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, ID_DIBWND,NULL))
	{
		TRACE0("Failed to creat view window\n");
		return -1;
	}

	if (! tfileopen->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, ID_OPENWND,NULL))
	{
		TRACE0("Failed to creat tool window\n");
		return -1;
	}
	if (! fa->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, ID_VIEWWND,NULL))
	{
		TRACE0("Failed to creat view window\n");
		return -1;
	}
	if (! fadib->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, ID_VIEWWND,NULL))
	{
		TRACE0("Failed to creat view window\n");
		return -1;
	}
	if (! fileopen->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, ID_VIEWWND,NULL))
	{
		TRACE0("Failed to creat view window\n");
		return -1;
	}
	if (! fileproc->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, ID_VIEWWND,NULL))
	{
		TRACE0("Failed to creat view window\n");
		return -1;
	}

	return 0;

}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.cx = GetSystemMetrics(SM_CXSCREEN);
	cs.cy = GetSystemMetrics(SM_CYSCREEN);
	

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	SetTitle(_T("DICOM"));
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG

// CMainFrame message handlers

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{

}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{

	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CMainFrame::OnEraseBkgnd(CDC* pDC)
{
	return CFrameWnd::OnEraseBkgnd(pDC);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);
	GetClientRect(clientRect);

	viewRect		= toolRect		= clientRect;
	viewRect.right	= toolRect.left = clientRect.Width()*4/5;

	viewWnd->MoveWindow(viewRect);
	toolWnd->MoveWindow(toolRect);
}

void CMainFrame::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	dc.FillSolidRect(toolRect, RGB(0,0,70));
	dc.FillSolidRect(viewRect, RGB(200,200,200));
						
	::SetDIBitsToDevice(dc.m_hDC,
		356/2, 356/2, fileopen->width, fileopen->height,   
		0, 0, 0, fileopen->height,       
		dib->dibImage, dib->bitmapInfo, DIB_RGB_COLORS);

	::SetDIBitsToDevice(dc.m_hDC,
		622, 0, tfileopen->width, tfileopen->height,   
		0, 0, 0, tfileopen->height,       
		tdib->dibImage, tdib->bitmapInfo, DIB_RGB_COLORS);

	::SetDIBitsToDevice(dc.m_hDC,
		476, 530, fileopen->width, fileopen->height,   
		0, 0, 0, fileopen->height,       
		fadib->faImage, fadib->bitmapInfo, DIB_RGB_COLORS);
}

BOOL CMainFrame::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(pt.x <612)
	{
		if(pt.y <512)
		{
			if(zDelta < 0)
				if (a > 0)
					a--;

			if(zDelta > 0)
				if(a < 927)
					a++;
			
			fileopen->OpenDTIFile(fileproc->DTI,a);		
			dib->CreateDIB();
			Invalidate(FALSE);
		}
	}

	if((pt.x > 620) & (pt.x < 620+512))
	{
		if(pt.y <520)
		{
			if(zDelta < 0)
				if (b > 0)
					b--;


				if(zDelta > 0)
					if(b < 19)
						b++;

			tfileopen->OpenFLAIRFile(fileproc->FLAIR,b);
			tdib->CreateDIB();
			Invalidate(FALSE);
		}
	}

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}