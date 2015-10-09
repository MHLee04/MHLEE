#include "stdafx.h"
#include "UI_EXAMPLE.h"
#include "ToolWnd.h"
#include "MainFrm.h"
#include "ViewWnd.h"
#include "DTIclass.h"
#include "DTIDIB.h"
#include "FLAIRclass.h"
#include "FLAIRDIB.h"
#include "FACAL.h"
#include "FADIB.h"
#include "FILEPROC.h"

// ToolWnd
#define ID_BTNCLK	ID_TOOLWND + 1
#define ID_BVECBTN  ID_TOOLWND + 4
#define ID_FABTN	ID_TOOLWND + 5

IMPLEMENT_DYNAMIC(ToolWnd, CWnd)

ToolWnd::ToolWnd()
{
}

ToolWnd::~ToolWnd()
{
}

BEGIN_MESSAGE_MAP(ToolWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_BN_CLICKED(ID_BTNCLK,	OnButtonTest)
	ON_BN_CLICKED(ID_BVECBTN,	OnBvecButton)
	ON_BN_CLICKED(ID_FABTN,		OnFAButton)
END_MESSAGE_MAP()

int ToolWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	testButton.Create(L"DICOM FILE OPEN",	WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(100,10,300,30),	this, ID_BTNCLK);
	BvecButton.Create(L"B-vector table",	WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(100,100,300,120),	this, ID_BVECBTN);
	FAButton.Create(L"FA IMAGE",			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(100,130,300,150),	this, ID_FABTN);

	return 0;
}

void ToolWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
}

BOOL ToolWnd::OnEraseBkgnd(CDC* pDC)
{
	return CWnd::OnEraseBkgnd(pDC);
}

void ToolWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
}

void ToolWnd::OnButtonTest()
{
	CMainFrame * pFrame = (CMainFrame *) AfxGetApp()->m_pMainWnd;
	CFolderPickerDialog dlg;

	if(dlg.DoModal() == IDCANCEL)
		return;

	pFrame->fileproc->FindFile(dlg.GetPathName());
	pFrame->fileproc->CLASSIFY(pFrame->fileproc->strFullPath);

	pFrame->fileopen->OpenDTIFile(pFrame->fileproc->DTI,0);
	pFrame->dib->CreateDIB();

	pFrame->tfileopen->OpenFLAIRFile(pFrame->fileproc->FLAIR,0);
	pFrame->tdib->CreateDIB();

	pFrame->Invalidate(FALSE);
}

void ToolWnd::OnBvecButton()
{
	CMainFrame * pFrame = (CMainFrame *) AfxGetApp()->m_pMainWnd;

	CString szFilter = _T("B-vector table (*.txt)|*.txt| All Files (*.*)|*.*|");

	CFileDialog dlg (TRUE, NULL, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, szFilter, this);

	if(dlg.DoModal() == IDCANCEL)
		return;

	CStdioFile		file;
	CFileException	e;
	CString			str, temp;
	int				i = 0;
	
	if(!file.Open(dlg.GetPathName() , CFile::modeRead, &e))
	{
		e.ReportError();
		return;
	}
	
	for (int i =0; i <3; i++)
	{
		for(int j =0; j <15; j++)
		{
			if(file.ReadString(str))
			{
				AfxExtractSubString(temp,str, 0,'\n');
				BV[i][j] =_ttof(temp);
			}
		}
	}

	file.Close();

	AfxMessageBox(_T("Complete Load B-vec"));
}

void ToolWnd::OnFAButton()
{
	CMainFrame * pFrame = (CMainFrame *) AfxGetApp()->m_pMainWnd;

	pFrame->fa->MG(0);
	pFrame->fa->eigen();
	pFrame->fadib->CreateDIB();

	pFrame->Invalidate(FALSE);
}