#include "stdafx.h"
#include "UI_EXAMPLE.h"
#include "FILEPROC.h"
#include "MainFrm.h"
IMPLEMENT_DYNAMIC(FILEPROC, CWnd)

FILEPROC::FILEPROC()
{

}

FILEPROC::~FILEPROC()
{
	if(DTI)
		delete[] DTI;

	if(FLAIR)
		delete[] FLAIR;
	if(strFullPath)
		delete[] strFullPath;
}

BEGIN_MESSAGE_MAP(FILEPROC, CWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

int FILEPROC::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

CString FILEPROC::FindFile(CString strParentPath)
{
	int i = 0;

	CFileFind finder;

	BOOL bWorking = finder.FindFile(strParentPath + _T("\\*.dcm"));

	while(bWorking)
	{

		bWorking			= finder.FindNextFile();

		if (finder.IsDots())
			continue;

		*(strFullPath + i)	= finder.GetFilePath();	

		i++;		
	}

	finder.Close();

	return * strFullPath;
}

void FILEPROC::CLASSIFY(CString * path)
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