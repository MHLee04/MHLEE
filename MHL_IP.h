
// MHL_IP.h : main header file for the MHL_IP application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMHL_IPApp:
// See MHL_IP.cpp for the implementation of this class
//

class CMHL_IPApp : public CWinApp
{
public:
	CMHL_IPApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMHL_IPApp theApp;
