#pragma once


// DTIDIB

class DTIDIB : public CWnd
{
	DECLARE_DYNAMIC(DTIDIB)

public:

	DTIDIB();
	virtual ~DTIDIB();

public:

	BITMAPINFO *		bitmapInfo;
	BOOL				CreateDIB();

	unsigned char *		dibData;
	unsigned char *		dibImage;

	int					dibStep;
	int					GetRealWidth(int width);
	void				Trans16to8();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


