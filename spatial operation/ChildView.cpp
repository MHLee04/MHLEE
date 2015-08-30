// ChildView.cpp : implementation of the CChildView class

#include "stdafx.h"
#include "MHL_IP.h"
#include "ChildView.h"
#include "cmath"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildView

CChildView::CChildView()
{
	dibData = NULL;
	dstData = NULL;
	leftButtonDown = FALSE;
}

CChildView::~CChildView()
{
	if(dibData)
	delete[] dibData;

	if(dstData)
		delete[] dstData;
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_COMMAND(ID_FILE_OPEN, &CChildView::OnFileOpen)
	ON_COMMAND(ID_ARITHMETIC_ADD, &CChildView::OnArithmeticAdd)
	ON_COMMAND(ID_ARITHMETIC_SUBTRACT, &CChildView::OnArithmeticSubtract)
	ON_COMMAND(ID_ARITHMETIC_MULTIPLY, &CChildView::OnArithmeticMultiply)
	ON_COMMAND(ID_ARITHMETIC_DIVIDE, &CChildView::OnArithmeticDivide)
	ON_COMMAND(ID_ARITHMETIC_LOGTRANSFORM, &CChildView::OnArithmeticLogtransform)
	ON_COMMAND(ID_ARITHMETIC_NEGATIVE, &CChildView::OnArithmeticNegative)
	ON_COMMAND(ID_GEOMETRY_FLIPV, &CChildView::OnGeometryFlipv)
	ON_COMMAND(ID_GEOMETRY_FLIPH, &CChildView::OnGeometryFliph)
	ON_COMMAND(ID_GEOMETRY_LROTATE90, &CChildView::OnGeometryLRotate90)
	ON_COMMAND(ID_GEOMETRY_RROTATE90, &CChildView::OnGeometryRRotate90)
	ON_COMMAND(ID_LUT_ADD, &CChildView::OnLutAdd)
	ON_UPDATE_COMMAND_UI(ID_LUT_ADD, &CChildView::OnUpdateLutAdd)
	ON_WM_CREATE()
	ON_WM_HSCROLL()
	ON_COMMAND(ID_LUT_NEGATIVE, &CChildView::OnLutNegative)
	ON_UPDATE_COMMAND_UI(ID_LUT_NEGATIVE, &CChildView::OnUpdateLutNegative)
	ON_UPDATE_COMMAND_UI(ID_LUT_GAMMA, &CChildView::OnUpdateLutGamma)
	ON_COMMAND(ID_LUT_GAMMA, &CChildView::OnLutGamma)
	ON_COMMAND(ID_FILTER_BLUR, &CChildView::OnFilterBlur)
	ON_COMMAND(ID_FILTER_SHARPEN, &CChildView::OnFilterSharpen)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CChildView message handlers

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

void CChildView::OnPaint() 
{

	CPaintDC dc(this); // device context for painting	
	
	CPen Pen;
	Pen.CreatePen(PS_SOLID, 1, RGB(255,0,0));
	
	CPen *pOldPen;
	pOldPen = dc.SelectObject(&Pen);
	
	CBrush Brush;
	Brush.CreateSolidBrush(RGB(0,255,0));
	CBrush * pOldBrush = dc.SelectObject(&Brush);
	
	{
		
		dc.MoveTo(0,0);
		dc.LineTo(100,100);
		dc.Rectangle(100,100,200,200);

	}
	
	dc.SelectObject(pOldBrush);
	Brush.DeleteObject();

	dc.SelectObject(pOldPen);
	Pen.DeleteObject();

	::SetDIBitsToDevice(dc.m_hDC,0,0,bitmapInfo->bmiHeader.biWidth, bitmapInfo->bmiHeader.biHeight,
		0 , 0, 0, bitmapInfo->bmiHeader.biHeight, dstData, bitmapInfo, DIB_RGB_COLORS);

}

void CChildView::OnFileOpen()
{

	CString szFilter = _T("bitmap Files (*.bmp)|*.bmp| All Files (*.*)|*.*|");
	CFileDialog dlg (TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT, szFilter,this);
	if( dlg.DoModal () == IDCANCEL)
		return;
	
	FILE * file;
	_wfopen_s(&file, dlg.GetPathName(),L"rb");
	
	BITMAPFILEHEADER bitmapfileheader;
	fread(&bitmapfileheader, sizeof(BITMAPFILEHEADER), 1, file);

	int dibsize = bitmapfileheader.bfSize - sizeof(BITMAPFILEHEADER);

	if(dibData)
		delete[] dibData;

	dibData = new unsigned char[dibsize];

	fread(dibData, dibsize, 1, file);
	bitmapInfo = (BITMAPINFO *) dibData;
	samplePerPixel = bitmapInfo->bmiHeader.biBitCount/8;
	imageWidth = bitmapInfo->bmiHeader.biWidth;
	imageHeight = bitmapInfo->bmiHeader.biHeight;
	imageStep = GetRealWidth (imageWidth);
	srcData = dibData + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * bitmapInfo->bmiHeader.biClrUsed;

	if(dstData)
		delete[] dstData;

	dstData = new unsigned char[imageStep * imageHeight];
	memcpy(dstData, srcData,imageStep*imageHeight);

	fclose(file);

	Invalidate(FALSE);	

}

int CChildView::GetRealWidth( int width)
{

	int real_width;
	div_t r;	
	r = div(width,4);
	
	if(r.rem != 0)
	{
		real_width = ((r.quot + 1) * 4);
		return real_width ;
	}
	else
		return width ;

}

unsigned char CChildView::Clip(int value, int low, int high)
{

	if(value < low)
		return (unsigned char) low;
	else if(value > high)
		return (unsigned char) high;
	else
		return (unsigned char) value;

}

void CChildView::OnArithmeticAdd()
{

	if(dibData == NULL)
		return;

	for(int i = 0; i < imageStep * imageHeight; i++)
	{
		dstData[i] = Clip(srcData[i] +100,0,255);
	}

	Invalidate(FALSE);

}

void CChildView::OnArithmeticSubtract()
{

	if(dibData == NULL)
		return;

	for(int i = 0; i < imageStep * imageHeight; i++)
	{
		dstData[i] = Clip(srcData[i] - 100,0,255);
	}

	Invalidate(FALSE);

}

void CChildView::OnArithmeticMultiply()
{

	if(dibData == NULL)
		return;

	for(int i = 0; i < imageStep * imageHeight; i++)
	{
		dstData[i] = Clip(srcData[i] * 2,0,255);
	}

	Invalidate(FALSE);

}

void CChildView::OnArithmeticDivide()
{

	if(dibData == NULL)
		return;

	for(int i = 0; i < imageStep * imageHeight; i++)
	{
		dstData[i] = Clip(srcData[i] / 2,0,255);
	}

	Invalidate(FALSE);

}

void CChildView::OnArithmeticLogtransform()
{

	if(dibData == NULL)
		return;

	for(int i = 0; i < imageStep * imageHeight; i++)
	{
		dstData[i] = Clip(10*log(1+(double)srcData[i]),0,255);
		//Warning
	}

	Invalidate(FALSE);

}

void CChildView::OnArithmeticNegative()
{

	if(dibData == NULL)
		return;

	for(int i = 0; i < imageStep * imageHeight; i++)
	{
		dstData[i] = Clip(255- srcData[i],0,255);
	}

	Invalidate(FALSE);

}

void CChildView::OnGeometryFlipv()
{

	for(int i=0; i< imageHeight;i++)
	{
		memcpy(dstData + i*imageStep,srcData+(imageHeight-1-i)*imageStep, imageStep);
	}

	Invalidate(FALSE);

}

void CChildView::OnGeometryFliph()
{

	for(int i =0; i<imageHeight; i++)
	{
		for(int j = 0; j <imageWidth;j++)
		{
			dstData[(i*imageStep)+j] = srcData[(i*imageStep)+(imageWidth-1-j)];
		}
	}

	Invalidate(FALSE);

}

void CChildView::OnGeometryLRotate90()
{

	for(int i =0; i<imageHeight; i++)
	{
		for(int j = 0; j <imageWidth;j++)
		{
			dstData[j*imageStep+(imageStep-i-1)] = srcData[(i*imageStep)+j];
		}
	}

		Invalidate(FALSE);

}

void CChildView::OnGeometryRRotate90()
{

	for(int i =0; i<imageHeight; i++)
	{
		for(int j = 0; j <imageWidth;j++)
		{
			dstData[i*imageStep+j] = srcData[j*imageStep+(imageHeight-i-1)];
		}
	}

		Invalidate(FALSE);

}

void CChildView::OnUpdateLutAdd(CCmdUI *pCmdUI)
{
	if(dibData == NULL)
		pCmdUI->Enable(FALSE);
}

void CChildView::OnLutAdd()
{

	if(dibData == NULL)
		return;

	unsigned char lut[256];

	for(int i = 0; i <256;i++)
		lut[i] = Clip(i +100,0,255);

	for(int i = 0; i < imageStep * imageHeight; i++)
	{
		dstData[i] = lut[ srcData[i] ];
	}

	Invalidate(FALSE);

}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	scrollBar.Create(SBS_HORZ | WS_CHILD | WS_VISIBLE, CRect(0, 520, 500,520+10), this, 9999);
	scrollBar.SetScrollRange(0, 100);
	scrollBar.SetScrollPos(0);

	return 0;

}

void CChildView::OnUpdateLutGamma(CCmdUI *pCmdUI)
{
	if(dibData == NULL)
		pCmdUI->Enable(FALSE);
}

void CChildView::OnLutGamma()
{
	if(dibData == NULL)
		return;

	unsigned char lut[256];
	
	double r = 1. - 1/5.;

	double exp = 1./ r;

	for(int i = 0; i <256;i++)
		lut[i] = Clip(pow (i/255.,exp)*255,0,255);
	
	for(int i = 0; i < imageStep * imageHeight; i++)
	{
		dstData[i] = lut[ srcData[i] ];
	}
	
	Invalidate(FALSE);

}

void CChildView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	double gamma = 1. - nPos /200.;
	GammaCorrection(gamma);
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);

}

void CChildView::GammaCorrection(double gamma)
{

	if(dibData == NULL)
		return;

	unsigned char lut[256];

	double exp = 1./ gamma;

	for(int i = 0; i <256;i++)
		lut[i] = Clip(pow (i/255.,exp)*255,0,255);
	
	for(int i = 0; i < imageStep * imageHeight; i++)
	{
		dstData[i] = lut[ srcData[i] ];
	}
	
	Invalidate(FALSE);

}

void CChildView::OnUpdateLutNegative(CCmdUI *pCmdUI)
{
		if(dibData == NULL)
			pCmdUI->Enable(FALSE);
}

void CChildView::OnLutNegative()
{
	if(dibData == NULL)
		return;

	unsigned char lut[256];

	for(int i = 0; i < 256; i++)
		lut[i] = Clip(255 - i,0,255);
	
	for(int i = 0; i < imageStep * imageHeight; i++)
	{
		dstData[i] = lut[ srcData[i] ];
	}

	Invalidate(FALSE);

}

void CChildView::OnFilterBlur()
{
	
	double mask[9] = {1/9., 1/9., 1/9.,
		              1/9., 1/9., 1/9.,
		              1/9., 1/9., 1/9.};

	SpatialFilter3x3(mask);

}


void CChildView::OnFilterSharpen()
{
		double mask[9] = {0, -1, 0,
		              -1, 5, -1,
		              0, -1, 0};
		SpatialFilter3x3(mask);
}


void CChildView::SpatialFilter3x3(double * mask)
{

	for(int i = 0; i < imageHeight;i++){
		for(int j = 0; j < imageWidth;j++){
			
			double sum = 0;

			for(int k = 0; k < 3; k ++){
				for(int l = 0; l < 3; l ++){

					sum += srcData[(i-k-1) * imageStep + (j+l-1)] * mask[k * 3 + l];

				}
			}
			
			dstData[i*imageStep + j ] = Clip(int(sum),0,255);

		}
	}
	
	Invalidate(FALSE);

}


void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	leftButtonDown = TRUE;
	leftButtonPoint = point;

	CWnd::OnLButtonDown(nFlags, point);
}


void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	leftButtonDown = FALSE;// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CWnd::OnLButtonUp(nFlags, point);
}


void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	if(leftButtonDown)
	{
		int diff = leftButtonPoint.y-point.y;

		unsigned char lut[256];
		
		for(int i = 0; i <256;i++)
		lut[i] = Clip(i + diff,0,255);
		
		for(int i = 0; i < imageStep * imageHeight; i++)
		{
			dstData[i] = lut[ srcData[i] ];
		}

		Invalidate(FALSE);
	}

	CWnd::OnMouseMove(nFlags, point);

}