#pragma once


// FILEPROC

class FILEPROC : public CWnd
{
	DECLARE_DYNAMIC(FILEPROC)

public:

	FILEPROC();
	virtual ~FILEPROC();

public:

	void				CLASSIFY(CString		* path);
	CString				FindFile(CString strParentPath);
	CString				Seq;
	CString				strFullPath[1192];
	CString				DTI[928];
	CString				FLAIR[20];


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


