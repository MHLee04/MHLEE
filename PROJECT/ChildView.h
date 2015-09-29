// ChildView.h : interface of the CChildView class
//
#pragma once

// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:
	BITMAPINFO *      bitmapInfo;
	KD_PHOTOMETRIC		photometric;
	CScrollBar			scrollBar, tscrollBar;
	//CEdit				edit;
	BOOL				CreateDIB();

	unsigned char *		dibData;
	unsigned char *		dibImage;
	unsigned char *		srcData;
	unsigned char *		dstData;

	int					samplePerPixel, width, height, srcStep, dibStep, bitsAllocated, bitsStored;
	int					pixelRepresentation, gamma;
	int					GetRealWidth(int width);

	double				windowWidth, windowCenter, windowWidthTemp, windowCenterTemp;

	void				Trans16to8();
	void				OpenDTIFile(CString		* path, int gamma);
	void				OpenFLAIRFile(CString	* path, int gamma);
	void				CLASSIFY(CString		* path);

	CString				FindFile(CString strParentPath);
	CString				Seq;
	CString				strFullPath[1192];
	CString				DTI[928];
	CString				FLAIR[20];
	CString				name;	

// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();

// Generated message map functions
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void	OnFileOpen();
	afx_msg int		OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

