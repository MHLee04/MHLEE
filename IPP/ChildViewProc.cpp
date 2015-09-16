#include "stdafx.h"
#include "DICOM_IPP.h"
#include "ChildView.h"
#include "Math.h"
#include "ippi.h"
#include "ipps.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CChildView::OnArithmeticAdd()
{
   if(dibData == NULL)
      return;

   short * src = (short *) srcData;
   short * dst = (short *) dstData;


   IppiSize  roiSize;

   roiSize.width  = width;
   roiSize.height = height;

   ippiAddC_16s_C1RSfs(src, srcStep, 100, 
	                   dst, srcStep, roiSize, 0);

   Trans16to8();
   Invalidate(FALSE);
}

void CChildView::OnArithmeticSub()
{
	if(dibData == NULL)
		return;

	short * src = (short *) srcData;
	short * dst = (short *) dstData;


	IppiSize  roiSize;

	roiSize.width  = width;
	roiSize.height = height;

	ippiSubC_16s_C1RSfs(src, srcStep, 100, 
		                dst, srcStep, roiSize, 0);

	Trans16to8();
	Invalidate(FALSE);
}

void CChildView::OnArithmeticMultiply()
{
	if(dibData == NULL)
		return;

	short * src = (short *) srcData;
	short * dst = (short *) dstData;


	IppiSize  roiSize;

	roiSize.width  = width;
	roiSize.height = height;

	ippiMulC_16s_C1RSfs(src, srcStep, 10, 
						dst, srcStep, roiSize, 0);

	Trans16to8();
	Invalidate(FALSE);
}

void CChildView::OnArithmeticDivide()
{
	if(dibData == NULL)
		return;

	short * src = (short *) srcData;
	short * dst = (short *) dstData;


	IppiSize  roiSize;

	roiSize.width  = width;
	roiSize.height = height;

	ippiDivC_16s_C1RSfs(src, srcStep, 10, 
		                dst, srcStep, roiSize, 0);

	Trans16to8();
	Invalidate(FALSE);
}

void CChildView::OnArithmeticNegative()
{
   if(dibData == NULL)
      return;


   Invalidate(FALSE);
}

void CChildView::OnGeometricFlipV()
{
   if(dibData == NULL)
      return;

   short * src = (short *) srcData;
   short * dst = (short *) dstData;


   IppiSize  roiSize;

   roiSize.width  = width;
   roiSize.height = height;

   ippiMirror_16s_C1R(src, srcStep, 
	                  dst, srcStep, roiSize, 
					  ippAxsVertical);

   Trans16to8();
   Invalidate(FALSE);
}

void CChildView::OnGeometricFlipH()
{
	if(dibData == NULL)
		return;

	short * src = (short *) srcData;
	short * dst = (short *) dstData;


	IppiSize  roiSize;

	roiSize.width  = width;
	roiSize.height = height;

	ippiMirror_16s_C1R(src, srcStep, 
		               dst, srcStep, roiSize, 
		               ippAxsHorizontal);

	Trans16to8();
	Invalidate(FALSE);
}

void CChildView::OnGeometricRotateLeft()
{
	if(dibData == NULL)
		return;

	short * src = (short *) srcData;
	short * dst = (short *) dstData;


	IppiSize  roiSize;

	roiSize.width  = width;
	roiSize.height = height;

	ippiTranspose_16s_C1R(src, srcStep, 
		                  dst, srcStep, roiSize);

	Trans16to8();
	Invalidate(FALSE);
}

void CChildView::OnGeometricRotateRight()
{
	if(dibData == NULL)
		return;

	short * src = (short *) srcData;
	short * dst = (short *) dstData;


	IppiSize  roiSize;

	roiSize.width  = width;
	roiSize.height = height;

	ippiMirror_16s_C1R(src, srcStep, 
		               dst, srcStep, roiSize, 
					   ippAxs135);

	Trans16to8();
	Invalidate(FALSE);
}

void CChildView::OnFilterBlur()
{
	if(dibData == NULL)
		return;

	short * src = (short *) srcData;
	short * dst = (short *) dstData;

	IppiSize  roiSize;

	roiSize.width         = width;
	roiSize.height        = height;

	IppiSize maskSize     = {5,5};
	Ipp16s borderValue    = 32767;
	Ipp8u * pBuffer       = NULL;

	int  pBufferSize;

	ippiFilterBoxBorderGetBufferSize(roiSize, maskSize,
		                             ipp16s, 1, &pBufferSize);

	pBuffer = ippsMalloc_8u(pBufferSize);

	ippiFilterBoxBorder_16s_C1R(src, srcStep, 
		                        dst, srcStep, 
								roiSize, maskSize,
								ippBorderRepl, &borderValue, pBuffer);

	ippsFree(pBuffer);

	Trans16to8();
	Invalidate(FALSE);
}


void CChildView::OnFilterSharpen()
{
	if(dibData == NULL)
		return;

	short * src = (short *) srcData;
	short * dst = (short *) dstData;

	IppiSize  roiSize;

	roiSize.width  = width;
	roiSize.height = height;

	ippiFilterSharpen_16s_C1R(src, srcStep, 
		                      dst, srcStep, roiSize);

	Trans16to8();
	Invalidate(FALSE);
}
