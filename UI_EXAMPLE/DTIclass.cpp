#include "stdafx.h"
#include "UI_EXAMPLE.h"
#include "DTIclass.h"
#include "MainFrm.h"
#include "DTIDIB.h"
#include "FILEPROC.h"

IMPLEMENT_DYNAMIC(DTIclass, CWnd)

DTIclass::DTIclass()
{
	dstData     = NULL;
	srcData     = NULL;
}

DTIclass::~DTIclass()
{
	if(dstData)
		delete[] dstData;

	if(srcData)
		delete[] srcData;
}

BEGIN_MESSAGE_MAP(DTIclass, CWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

void DTIclass::OpenDTIFile(CString * path, int gamma)
{
	// Allocate KDicomDS object
	KDicomDS * pDTI		 = new KDicomDS[928];
	CMainFrame * pFrame  = (CMainFrame *) AfxGetApp()->m_pMainWnd;

	pDTI[gamma].LoadDS(pFrame->fileproc->DTI[gamma]);

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

	dstData = new unsigned char[srcStep * height];

	memcpy(dstData, srcData, srcStep * height);
	// Delete DicomDS
	delete[] pDTI;
}

int DTIclass::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}
