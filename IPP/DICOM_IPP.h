
// DICOM_IPP.h : main header file for the DICOM_IPP application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CDICOM_IPPApp:
// See DICOM_IPP.cpp for the implementation of this class
//

class CDICOM_IPPApp : public CWinApp
{
public:
	CDICOM_IPPApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CDICOM_IPPApp theApp;
