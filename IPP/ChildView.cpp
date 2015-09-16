#include "stdafx.h"
#include "DICOM_IPP.h"
#include "ChildView.h"
#include "Math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CChildView::CChildView()
{
	dibData     = NULL;
	dibImage    = NULL;
	dstData     = NULL;
	srcData     = NULL;
	m_bDown     = FALSE;
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
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_FILE_OPEN,                OnFileOpen)
	ON_COMMAND(ID_ATRITHMETIC_ADD,			OnArithmeticAdd)
	ON_COMMAND(ID_ATRITHMETIC_SUB,			OnArithmeticSub)
	ON_COMMAND(ID_ATRITHMETIC_MULTIPLY,		OnArithmeticMultiply)
	ON_COMMAND(ID_ATRITHMETIC_DIVIDE,		OnArithmeticDivide)
	ON_COMMAND(ID_ATRITHMETIC_NEGATIVE,		OnArithmeticNegative)
	ON_COMMAND(ID_GEOMETRIC_FLIPH,			OnGeometricFlipH)
	ON_COMMAND(ID_GEOMETRIC_FLIPV,			OnGeometricFlipV)
	ON_COMMAND(ID_GEOMETRIC_ROTATELEFT,		OnGeometricRotateLeft)
	ON_COMMAND(ID_GEOMETRIC_ROTATERIGHT,	OnGeometricRotateRight)
	ON_COMMAND(ID_FILTER_BLUR,				OnFilterBlur)
	ON_COMMAND(ID_FILTER_SHARPEN,			OnFilterSharpen)
END_MESSAGE_MAP()

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this);

	if(dibData == NULL)
		return;

	::SetDIBitsToDevice(dc.m_hDC,
		0, 0, width, height,   
		0, 0, 0, height,       
		dibImage, bitmapInfo, DIB_RGB_COLORS);	
}

void CChildView::OnFileOpen()
{
	// Show File Dialog Box
	CString szFilter = _T("DICOM Files (*.dcm)|*.dcm|bitmap Files (*.bmp)|*.bmp|All Files (*.*)|*.*|");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, szFilter,this);

	if(dlg.DoModal() == IDCANCEL)
		return;

	CString ext = dlg.GetFileExt();
	ext.MakeUpper();

	if(ext == L"DCM")
		OpenDICOMFile(dlg.GetPathName());
}

void CChildView::OpenDICOMFile(CString path)
{
	// Allocate KDicomDS object
	KDicomDS * dicomDS = new KDicomDS;

	// Load File
	dicomDS->LoadDS(path);

	// Get Parameters
	samplePerPixel       = dicomDS->m_nSamplePerPixel;
	photometric          = dicomDS->m_nPhotometric;
	width                = dicomDS->m_nWidth;
	height               = dicomDS->m_nHeight;
	bitsAllocated        = dicomDS->m_nBitsAllocated;
	bitsStored           = dicomDS->m_nBitsStored;
	pixelRepresentation  = dicomDS->m_nRepresentation;
	windowCenter         = dicomDS->m_dWindowCenter;
	windowWidth          = dicomDS->m_dWindowWidth;

	if(bitsAllocated == 8)
		srcStep = width * samplePerPixel;
	else
		srcStep = width * 2;

	// Allocate image processing source memory
	if(srcData)
		delete[] srcData;

	srcData  = new unsigned char[srcStep * height];
	dicomDS->GetImageData(srcData);

	// Allocate image processing dest memory
	if(dstData)
		delete[] dstData;

	dstData  = new unsigned char[srcStep * height];
	memcpy(dstData, srcData, srcStep * height);

	// Delete DicomDS
	delete dicomDS;

	CreateDIB();
	Trans16to8();
	Invalidate(FALSE);
}

BOOL CChildView::CreateDIB()
{
	int colorNum = 256;
	dibStep = GetRealWidth(width);

	// Calculate DIB size
	int dibSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * colorNum + dibStep * height;

	// Allocate DIB memory
	if(dibData)
		delete[] dibData;

	dibData = new unsigned char[dibSize];
	bitmapInfo = (BITMAPINFO *) dibData;

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

	dibImage = dibData + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * colorNum;

	if(photometric == MONOCHROME1){
		for(int i=0;i<colorNum;i++){

			bitmapInfo->bmiColors[i].rgbRed        = 255-i;
			bitmapInfo->bmiColors[i].rgbGreen      = 255-i;
			bitmapInfo->bmiColors[i].rgbBlue       = 255-i;
			bitmapInfo->bmiColors[i].rgbReserved   = 0;

		}
	}
	else if(photometric == MONOCHROME2){
		for(int i=0;i<colorNum;i++){

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

	for(int i=0;i<height;i++){
		for(int j=0;j<width;j++){

			value = src[i*width + j];

			if(value < low)
				dibImage[(height - 1 - i)*dibStep + j]   = 0;
			else if(value > high)
				dibImage[(height - 1 - i)*dibStep + j]   = 255;
			else{

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

	if(r.rem != 0){

		real_width = ((r.quot + 1) * 4);
		return real_width;

	}
	else
		return width;
}

int CChildView::Clip(int value, int low, int high)
{
	if(value < low)
		return low;
	else if(value > high)
		return high;
	else 
		return value;
}

void CChildView::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_bDown               = TRUE;
	m_ptDown              = point;
	windowCenterTemp      = windowCenter;
	windowWidthTemp       = windowWidth;

	CWnd::OnLButtonDown(nFlags, point);
}


void CChildView::OnRButtonUp(UINT nFlags, CPoint point)
{
	m_bDown = FALSE;

	CWnd::OnLButtonUp(nFlags, point);
}


void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_bDown){
		if(bitsAllocated == 16){

			int dx         = (point.x - m_ptDown.x) * 100;
			int dy         = (point.y - m_ptDown.y) * 100;
			windowWidth    = windowWidthTemp + dx;
			windowCenter   = windowCenterTemp + dy;

			Trans16to8();
			Invalidate(FALSE);

		}
	}  

	CWnd::OnMouseMove(nFlags, point);
}



