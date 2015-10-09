#include "stdafx.h"
#include "UI_EXAMPLE.h"
#include "FADIB.h"
#include "MainFrm.h"
#include "FACAL.h"
#include "DTIclass.h"

IMPLEMENT_DYNAMIC(FADIB, CWnd)

	FADIB::FADIB()
{
	dibData     = NULL;
	faImage		= NULL;
}

FADIB::~FADIB()
{
	if(dibData)
		delete[] dibData;
}


BEGIN_MESSAGE_MAP(FADIB, CWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()


BOOL FADIB::CreateDIB()
{
	CMainFrame * pFrame = (CMainFrame *) AfxGetApp()->m_pMainWnd;

	int colorNum		= 256;
	dibStep				= GetRealWidth(pFrame->fileopen->width);

	// Calculate DIB size
	int dibSize			= sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * colorNum + dibStep * pFrame->fileopen->height;

	// Allocate DIB memory
	if(dibData)
		delete[] dibData;

	dibData									  = new unsigned char[dibSize];
	bitmapInfo								  = (BITMAPINFO *) dibData;

	// Make BITMAPINFOHEADER
	bitmapInfo->bmiHeader.biSize              = sizeof(BITMAPINFOHEADER);
	bitmapInfo->bmiHeader.biWidth			  = pFrame->fileopen->width;
	bitmapInfo->bmiHeader.biHeight			  = pFrame->fileopen->height;
	bitmapInfo->bmiHeader.biPlanes			  = 1;
	bitmapInfo->bmiHeader.biBitCount	      = WORD(pFrame->fileopen->samplePerPixel *8);
	bitmapInfo->bmiHeader.biCompression	      = 0;
	bitmapInfo->bmiHeader.biSizeImage		  = dibStep * pFrame->fileopen->height;
	bitmapInfo->bmiHeader.biXPelsPerMeter	  = 0;
	bitmapInfo->bmiHeader.biYPelsPerMeter	  = 0;
	bitmapInfo->bmiHeader.biClrUsed		      = colorNum;
	bitmapInfo->bmiHeader.biClrImportant	  = 0;

	faImage								  = dibData + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * colorNum;

	if(pFrame->fileopen->photometric == MONOCHROME1)
	{
		for(int i=0;i<colorNum;i++)
		{
			bitmapInfo->bmiColors[i].rgbRed        = 255-i;
			bitmapInfo->bmiColors[i].rgbGreen      = 255-i;
			bitmapInfo->bmiColors[i].rgbBlue       = 255-i;
			bitmapInfo->bmiColors[i].rgbReserved   = 0;
		}
	}
	else if(pFrame->fileopen->photometric == MONOCHROME2)
	{
		for(int i=0;i<colorNum;i++)
		{
			bitmapInfo->bmiColors[i].rgbRed        = i;
			bitmapInfo->bmiColors[i].rgbGreen      = i;
			bitmapInfo->bmiColors[i].rgbBlue       = i;
			bitmapInfo->bmiColors[i].rgbReserved   = 0;
		}
	}

	FATrans16to8();

	return TRUE;
}

void FADIB::FATrans16to8()
{
	CMainFrame * pFrame = (CMainFrame *) AfxGetApp()->m_pMainWnd;

	if(pFrame->fileopen->bitsAllocated == 8)
		return;

	double * src = (double *)pFrame->fa->rfa;
	double ratio   = 255 ;

	int low        = 0;
	int high       = 1;

	for(int i = 0;i < pFrame->fileopen->height;i++)
	{
		for(int j = 0;j < pFrame->fileopen->width;j++)
		{
			if(src[i * pFrame->fileopen->width + j] < low)
				faImage[(pFrame->fileopen->height - 1 - i)*dibStep + j]						= 0;
			else if(src[i * pFrame->fileopen->width + j] > high)
				faImage[(pFrame->fileopen->height - 1 - i)*dibStep + j]						= 255;
			else
			{
				faImage[(pFrame->fileopen->height - 1 - i)*pFrame->fileopen->width + j]     = src[i * pFrame->fileopen->width + j] * ratio;
			}
		}
	}	
}

int FADIB::GetRealWidth(int width)
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

int FADIB::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}
