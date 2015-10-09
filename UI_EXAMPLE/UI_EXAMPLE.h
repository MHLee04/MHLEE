
// UI_EXAMPLE.h : main header file for the UI_EXAMPLE application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CUI_EXAMPLEApp:
// See UI_EXAMPLE.cpp for the implementation of this class
//

class CUI_EXAMPLEApp : public CWinApp
{
public:
	CUI_EXAMPLEApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CUI_EXAMPLEApp theApp;
