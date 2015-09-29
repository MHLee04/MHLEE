// ChildView.cpp : implementation of the CChildView class
//
#include "stdafx.h"
#include "PROJECT.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildView
CChildView::CChildView()
{
	dibData     = NULL;
	dibImage    = NULL;
	dstData     = NULL;
	srcData     = NULL;
}

CChildView::~CChildView()
{
	if(dibData)
		delete[] dibData;

	if(dstData)
		delete[] dstData;

	if(srcData)
		delete[] srcData;
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_COMMAND(ID_FILE_OPEN,	OnFileOpen)
	ON_WM_CREATE()
	ON_WM_HSCROLL()
END_MESSAGE_MAP()
// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle	|= WS_EX_CLIENTEDGE;
	cs.style		&= ~WS_BORDER;
	cs.lpszClass	= AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	if(dibData == NULL)
		return;

	CString str;

	//str.Format(_T("Select Slice: "));
	//dc.DrawText(str, CRect(150, 580, 150+100, 600), DT_VCENTER | DT_SINGLELINE );

	if (name ==_T("DTI"))
	{
		str.Format(_T("Slice Num: %d"), gamma % 57 + 1);
		dc.DrawText(str, CRect(256, 530, 256 + 100, 550), DT_VCENTER | DT_SINGLELINE );
		
		str.Format(_T("Volume Num: %d"), gamma / 57 + 1);
		dc.DrawText(str, CRect(256, 555, 256 + 200, 575), DT_VCENTER | DT_SINGLELINE );
			
		//str.Format(_T("Slice Num: %d"),gamma);
		//dc.DrawText(str, CRect(256, 530, 256+100, 550), DT_VCENTER | DT_SINGLELINE );
		
		::SetDIBitsToDevice(dc.m_hDC,
			256/2, 256/2, width, height,   
			0, 0, 0, height,       
			dibImage, bitmapInfo, DIB_RGB_COLORS);	
	}

	if (name ==_T("FLAIR"))
	{
		str.Format(_T("Slice Num: %d"),gamma + 1);
		dc.DrawText(str, CRect(256 * 3, 530, 256 * 3 + 100, 550), DT_VCENTER | DT_SINGLELINE );
		
		::SetDIBitsToDevice(dc.m_hDC,
			522, 0, width, height,   
			0, 0, 0, height,       
			dibImage, bitmapInfo, DIB_RGB_COLORS);	
	}
}

void CChildView::OnFileOpen()
{
	CFolderPickerDialog dlg;

	if(dlg.DoModal() == IDCANCEL)
		return;
	
	FindFile(dlg.GetPathName());
	CLASSIFY(strFullPath);
}

CString CChildView::FindFile(CString strParentPath)
{
	int i = 0;
	CFileFind finder;

	BOOL bWorking = finder.FindFile(strParentPath + _T("\\*.dcm"));

	while(bWorking)
	{
		bWorking		= finder.FindNextFile();

		if (finder.IsDots())
			continue;

		strFullPath[i]	= finder.GetFilePath();	

		i++;		
	}

	finder.Close();
	return * strFullPath;
}

void CChildView::OpenDTIFile(CString * path, int gamma)
{
	// Allocate KDicomDS object
	KDicomDS * pDTI		 = new KDicomDS[928];

	pDTI[gamma].LoadDS(DTI[gamma]);

	samplePerPixel       = pDTI[gamma].m_nSamplePerPixel;
	photometric          = pDTI[gamma].m_nPhotometric;
	width                = pDTI[gamma].m_nWidth;
	height               = pDTI[gamma].m_nHeight;
	bitsAllocated        = pDTI[gamma].m_nBitsAllocated;
	bitsStored           = pDTI[gamma].m_nBitsStored;
	pixelRepresentation  = pDTI[gamma].m_nRepresentation;
	windowCenter         = pDTI[gamma].m_dWindowCenter;
	windowWidth          = pDTI[gamma].m_dWindowWidth;

	if(bitsAllocated == 8)
		srcStep = width * samplePerPixel;
	else
		srcStep = width * 2;

	if(srcData)
		delete[] srcData;
	// Allocate image processing source memory
	srcData  = new unsigned char[srcStep * height];
	pDTI[gamma].GetImageData(srcData);

	
	if(dstData)
		delete[] dstData;
	// Allocate image processing dest memory
	dstData  = new unsigned char[srcStep * height];
	memcpy(dstData, srcData, srcStep * height);

	// Delete DicomDS
	delete[] pDTI;
	name = _T("DTI");
	CreateDIB();
	Trans16to8();
	Invalidate(FALSE);
}

void CChildView::OpenFLAIRFile(CString * path, int gamma)
{
	// Allocate KDicomDS object
	KDicomDS * pFLAIR	 = new KDicomDS[20];

	pFLAIR[gamma].LoadDS(FLAIR[gamma]);

	samplePerPixel       = pFLAIR[gamma].m_nSamplePerPixel;
	photometric          = pFLAIR[gamma].m_nPhotometric;
	width                = pFLAIR[gamma].m_nWidth;
	height               = pFLAIR[gamma].m_nHeight;
	bitsAllocated        = pFLAIR[gamma].m_nBitsAllocated;
	bitsStored           = pFLAIR[gamma].m_nBitsStored;
	pixelRepresentation  = pFLAIR[gamma].m_nRepresentation;
	windowCenter         = pFLAIR[gamma].m_dWindowCenter;
	windowWidth          = pFLAIR[gamma].m_dWindowWidth;

	if(bitsAllocated == 8)
		srcStep = width * samplePerPixel;
	else
		srcStep = width * 2;

	if(srcData)
		delete[] srcData;
	// Allocate image processing source memory
	srcData  = new unsigned char[srcStep * height];
	pFLAIR[gamma].GetImageData(srcData);

	if(dstData)
		delete[] dstData;

	// Allocate image processing dest memory
	dstData  = new unsigned char[srcStep * height];
	memcpy(dstData, srcData, srcStep * height);

	// Delete DicomDS
	delete[] pFLAIR;
	name =_T("FLAIR");
	CreateDIB();
	Trans16to8();
	Invalidate(FALSE);
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//edit.Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_NUMBER, CRect(256, 580, 256+100, 600),this, 1234);
			
	scrollBar.Create(SBS_HORZ | WS_CHILD | WS_VISIBLE, CRect(0,512, 512,512+10), this, 9999);
	scrollBar.SetScrollRange(0, 927);
	scrollBar.SetScrollPos(0);

	tscrollBar.Create(SBS_HORZ | WS_CHILD | WS_VISIBLE, CRect(522, 512,1034,512+10), this, 9999);
	tscrollBar.SetScrollRange(0, 19);
	tscrollBar.SetScrollPos(0);

	return 0;
}

BOOL CChildView::CreateDIB()
{
	int colorNum = 256;
	dibStep		 = GetRealWidth(width);

	// Calculate DIB size
	int dibSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * colorNum + dibStep * height;

	// Allocate DIB memory
	if(dibData)
		delete[] dibData;

	dibData									  = new unsigned char[dibSize];
	bitmapInfo								  = (BITMAPINFO *) dibData;

	// Make BITMAPINFOHEADER
	bitmapInfo->bmiHeader.biSize              = sizeof(BITMAPINFOHEADER);
	bitmapInfo->bmiHeader.biWidth			  = width;
	bitmapInfo->bmiHeader.biHeight			  = height;
	bitmapInfo->bmiHeader.biPlanes			  = 1;
	bitmapInfo->bmiHeader.biBitCount	      = WORD(samplePerPixel * 8);
	bitmapInfo->bmiHeader.biCompression	      = 0;
	bitmapInfo->bmiHeader.biSizeImage		  = dibStep * height;
	bitmapInfo->bmiHeader.biXPelsPerMeter	  = 0;
	bitmapInfo->bmiHeader.biYPelsPerMeter	  = 0;
	bitmapInfo->bmiHeader.biClrUsed		      = colorNum;
	bitmapInfo->bmiHeader.biClrImportant	  = 0;

	dibImage								  = dibData + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * colorNum;

	if(photometric == MONOCHROME1)
	{
		for(int i=0;i<colorNum;i++)
		{
			bitmapInfo->bmiColors[i].rgbRed        = 255-i;
			bitmapInfo->bmiColors[i].rgbGreen      = 255-i;
			bitmapInfo->bmiColors[i].rgbBlue       = 255-i;
			bitmapInfo->bmiColors[i].rgbReserved   = 0;
		}
	}
	else if(photometric == MONOCHROME2)
	{
		for(int i=0;i<colorNum;i++)
		{
			bitmapInfo->bmiColors[i].rgbRed        = i;
			bitmapInfo->bmiColors[i].rgbGreen      = i;
			bitmapInfo->bmiColors[i].rgbBlue       = i;
			bitmapInfo->bmiColors[i].rgbReserved   = 0;
		}
	}

	return TRUE;
}

void CChildView::Trans16to8()
{
	if(bitsAllocated == 8)
		return;

	short * src    = (short *) dstData;
	int low        = windowCenter - windowWidth / 2;
	int high       = windowCenter + windowWidth / 2;
	double ratio   = 255 / windowWidth;
	short value;

	for(int i = 0;i < height;i++)
	{
		for(int j = 0;j < width;j++)
		{
			value									     = src[i * width + j];

			if(value < low)
				dibImage[(height - 1 - i)*dibStep + j]   = 0;
			else if(value > high)
				dibImage[(height - 1 - i)*dibStep + j]   = 255;
			else
			{
				dibImage[(height - 1 - i)*width + j]     = (value - low) * ratio;
			}
		}
	}
}

int CChildView::GetRealWidth(int width)
{
	int real_width;
	div_t r;

	r = div(width,4);

	if(r.rem != 0)
	{
		real_width = ((r.quot + 1) * 4);
		return real_width;
	}
	else
		return width;
}

void CChildView::CLASSIFY(CString * path)
{
	// Allocate KDicomDS object
	KDicomDS * dicomDS	= new KDicomDS[1192];
	int a				= 0;
	int b				= 0;
	KDicomElement * de[1192];

	for (int i = 0;i < 1192;i++)
	{
		dicomDS[i].LoadDS(path[i]);
		de[i]				= dicomDS[i].GetElement(0x0018,0x0020);

		if(de[i])
		{
			Seq				= de[i]->GetValueCS(0);

			if(Seq ==_T("EP"))
			{
				DTI[a]		= path[i];	
				a++;
			}		
			if(Seq ==_T("SE"))
			{
				FLAIR[b]	= path[i];	
				b++;
			}	
		}
	}
	delete[] dicomDS;
}

void CChildView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CString strText;

	if(&scrollBar ==pScrollBar)
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

		gamma = pScrollBar->GetScrollPos();	
		OpenDTIFile(DTI,gamma);		
	}

	if(&tscrollBar ==pScrollBar)
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

		gamma = pScrollBar->GetScrollPos();
		
		OpenFLAIRFile(FLAIR, gamma);	
	}

	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}