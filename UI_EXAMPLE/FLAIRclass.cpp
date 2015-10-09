#include "stdafx.h"
#include "UI_EXAMPLE.h"
#include "FLAIRclass.h"
#include "MainFrm.h"
#include "FLAIRDIB.h"
#include "FILEPROC.h"
IMPLEMENT_DYNAMIC(FLAIRclass, CWnd)

FLAIRclass::FLAIRclass()
{
	dstData     = NULL;
	srcData     = NULL;
}

FLAIRclass::~FLAIRclass()
{
	if(dstData)
		delete[] dstData;

	if(srcData)
		delete[] srcData;
}


BEGIN_MESSAGE_MAP(FLAIRclass, CWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()


int FLAIRclass::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}
void FLAIRclass::OpenFLAIRFile(CString * path, int gamma)
{
	// Allocate KDicomDS object
	KDicomDS * pFLAIR	 = new KDicomDS[20];
	CMainFrame * pFrame  = (CMainFrame *) AfxGetApp()->m_pMainWnd;

	pFLAIR[gamma].LoadDS(pFrame->fileproc->FLAIR[gamma]);

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
}