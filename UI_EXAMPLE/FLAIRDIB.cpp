#include "stdafx.h"
#include "UI_EXAMPLE.h"
#include "FLAIRDIB.h"
#include "MainFrm.h"
#include "FLAIRclass.h"

IMPLEMENT_DYNAMIC(FLAIRDIB, CWnd)

FLAIRDIB::FLAIRDIB()
{
	dibData     = NULL;
	dibImage    = NULL;
}

FLAIRDIB::~FLAIRDIB()
{
	if(dibData)
		delete[] dibData;
}


BEGIN_MESSAGE_MAP(FLAIRDIB, CWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

int FLAIRDIB::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL FLAIRDIB::CreateDIB()
{
	CMainFrame * pFrame = (CMainFrame *) AfxGetApp()->m_pMainWnd;

	int colorNum		= 256;
	dibStep				= GetRealWidth(pFrame->tfileopen->width);

	// Calculate DIB size
	int dibSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * colorNum + dibStep * pFrame->tfileopen->height;

	// Allocate DIB memory
	if(dibData)
		delete[] dibData;

	dibData									  = new unsigned char[dibSize];
	bitmapInfo								  = (BITMAPINFO *) dibData;

	// Make BITMAPINFOHEADER
	bitmapInfo->bmiHeader.biSize              = sizeof(BITMAPINFOHEADER);
	bitmapInfo->bmiHeader.biWidth			  = pFrame->tfileopen->width;
	bitmapInfo->bmiHeader.biHeight			  = pFrame->tfileopen->height;
	bitmapInfo->bmiHeader.biPlanes			  = 1;
	bitmapInfo->bmiHeader.biBitCount	      = WORD(pFrame->tfileopen->samplePerPixel * 8);
	bitmapInfo->bmiHeader.biCompression	      = 0;
	bitmapInfo->bmiHeader.biSizeImage		  = dibStep * pFrame->tfileopen->height;
	bitmapInfo->bmiHeader.biXPelsPerMeter	  = 0;
	bitmapInfo->bmiHeader.biYPelsPerMeter	  = 0;
	bitmapInfo->bmiHeader.biClrUsed		      = colorNum;
	bitmapInfo->bmiHeader.biClrImportant	  = 0;

	dibImage								  = dibData + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * colorNum;

	if(pFrame->tfileopen->photometric == MONOCHROME1)
	{
		for(int i=0;i<colorNum;i++)
		{
			bitmapInfo->bmiColors[i].rgbRed        = 255-i;
			bitmapInfo->bmiColors[i].rgbGreen      = 255-i;
			bitmapInfo->bmiColors[i].rgbBlue       = 255-i;
			bitmapInfo->bmiColors[i].rgbReserved   = 0;
		}
	}
	else if(pFrame->tfileopen->photometric == MONOCHROME2)
	{
		for(int i=0;i<colorNum;i++)
		{
			bitmapInfo->bmiColors[i].rgbRed        = i;
			bitmapInfo->bmiColors[i].rgbGreen      = i;
			bitmapInfo->bmiColors[i].rgbBlue       = i;
			bitmapInfo->bmiColors[i].rgbReserved   = 0;
		}
	}

	Trans16to8();

	return TRUE;
}

void FLAIRDIB::Trans16to8()
{
	CMainFrame * pFrame = (CMainFrame *) AfxGetApp()->m_pMainWnd;

	if(pFrame->tfileopen->bitsAllocated == 8)
		return;

	short * src    = (short *) pFrame->tfileopen->dstData;
	int low        = pFrame->tfileopen->windowCenter - pFrame->tfileopen->windowWidth / 2;
	int high       = pFrame->tfileopen->windowCenter + pFrame->tfileopen->windowWidth / 2;
	double ratio   = 255 / pFrame->tfileopen->windowWidth;
	short value;

	for(int i = 0;i < pFrame->tfileopen->height;i++)
	{
		for(int j = 0;j < pFrame->tfileopen->width;j++)
		{
			value																			    = src[i * pFrame->tfileopen->width + j];

			if(value < low)
				dibImage[(pFrame->tfileopen->height - 1 - i)*dibStep + j]						= 0;
			else if(value > high)
				dibImage[(pFrame->tfileopen->height - 1 - i)*dibStep + j]						= 255;
			else
			{
				dibImage[(pFrame->tfileopen->height - 1 - i)*pFrame->tfileopen->width + j]     = (value - low) * ratio;
			}
		}
	}
}

int FLAIRDIB::GetRealWidth(int width)
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
