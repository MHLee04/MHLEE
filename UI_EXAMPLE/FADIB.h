#pragma once


// FADIB

class FADIB : public CWnd
{
	DECLARE_DYNAMIC(FADIB)

public:
	FADIB();
	virtual ~FADIB();

public:

	BITMAPINFO *		bitmapInfo;
	unsigned char *		dibData;
	unsigned char *		faImage;

	BOOL				CreateDIB();
	int					GetRealWidth(int width);
	void				FATrans16to8();
	
	int					dibStep;

protected:

	DECLARE_MESSAGE_MAP()

public:

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


