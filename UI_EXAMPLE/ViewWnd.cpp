#include "stdafx.h"
#include "UI_EXAMPLE.h"
#include "ViewWnd.h"
#include "ToolWnd.h"
#include "MainFrm.h"
#include "DTIclass.h"
#include "FLAIRclass.h"
#include "FACAL.h"
#include "FILEPROC.h"
#include "DTIDIB.h"
#include "FLAIRDIB.h"
#include "FADIB.h"
IMPLEMENT_DYNAMIC(ViewWnd, CWnd)

ViewWnd::ViewWnd()
{
}

ViewWnd::~ViewWnd()
{	
}

BEGIN_MESSAGE_MAP(ViewWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

int ViewWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	scrollBar.Create(SBS_HORZ | WS_CHILD | WS_VISIBLE, CRect(50, 512, 570, 522), this, ID_LSCROLL);
	scrollBar.SetScrollRange(0, 927);
	scrollBar.SetScrollPos(0);

	tscrollBar.Create(SBS_HORZ | WS_CHILD | WS_VISIBLE, CRect(622, 512, 1134, 522), this, ID_RSCROLL);
	tscrollBar.SetScrollRange(0, 19);
	tscrollBar.SetScrollPos(0);

	fascrollBar.Create(SBS_HORZ | WS_CHILD | WS_VISIBLE, CRect(330, 810, 882, 820), this, ID_BSCROLL);
	fascrollBar.SetScrollRange(0, 57);
	fascrollBar.SetScrollPos(0);

	return 0;
}

void ViewWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
}

BOOL ViewWnd::OnEraseBkgnd(CDC* pDC)
{
	return CWnd::OnEraseBkgnd(pDC);
}

void ViewWnd::OnPaint()
{
	CPaintDC dc(this); 
	// device context for painting
}

void ViewWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CMainFrame * pFrame = (CMainFrame *) AfxGetApp()->m_pMainWnd;

	if(&scrollBar == pScrollBar)
	{		
		if(nSBCode == SB_LINELEFT)
		{
			if (pScrollBar->GetScrollPos() != 0)
				pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() - 1);
		}

		if(nSBCode == SB_LINERIGHT)
			pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() + 1);

		if (nSBCode == SB_PAGELEFT)
		{
			if (pScrollBar->GetScrollPos() >= 58)
				pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() - 58);
		}

		if (nSBCode == SB_PAGERIGHT)
			pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() + 58);

		if (nSBCode == SB_THUMBTRACK)
			pScrollBar->SetScrollPos(nPos);

		gamma = pScrollBar->GetScrollPos();	

		pFrame->fileopen->OpenDTIFile(pFrame->fileproc->DTI,gamma);		
		pFrame->dib->CreateDIB();

		pFrame->Invalidate(FALSE);
	}

	if(&tscrollBar == pScrollBar)
	{
		if(nSBCode == SB_LINELEFT)
		{
			if (pScrollBar->GetScrollPos() != 0)
				pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() - 1);
		}

		if(nSBCode == SB_LINERIGHT)
			pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() + 1);

		if (nSBCode == SB_PAGELEFT)
		{
			if (pScrollBar->GetScrollPos() >= 5)
				pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() - 5);
		}

		if (nSBCode == SB_PAGERIGHT)
			pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() + 5);
		
		if (nSBCode == SB_THUMBTRACK)
			pScrollBar->SetScrollPos(nPos);

		gamma = pScrollBar->GetScrollPos();
		
		pFrame->tfileopen->OpenFLAIRFile(pFrame->fileproc->FLAIR,gamma);
		pFrame->tdib->CreateDIB();

		pFrame->Invalidate(FALSE);
	}

	if(&fascrollBar == pScrollBar)
	{		
		if(nSBCode == SB_LINELEFT)
		{
			if (pScrollBar->GetScrollPos() != 0)
				pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() - 1);
		}

		if(nSBCode == SB_LINERIGHT)
			pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() + 1);

		if (nSBCode == SB_PAGELEFT)
		{
			if (pScrollBar->GetScrollPos() >= 5)
				pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() - 5);
		}

		if (nSBCode == SB_PAGERIGHT)
			pScrollBar->SetScrollPos(pScrollBar->GetScrollPos() + 5);

		if (nSBCode == SB_THUMBTRACK)
			pScrollBar->SetScrollPos(nPos);

		gamma = pScrollBar->GetScrollPos();	

		pFrame->fa->MG(gamma);
		pFrame->fa->eigen();
		pFrame->fadib->CreateDIB();

		pFrame->Invalidate(FALSE);
	}

	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}