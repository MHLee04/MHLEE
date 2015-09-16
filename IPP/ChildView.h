#pragma once

class CChildView : public CWnd
{
	// Construction
public:
	CChildView();

	// Attributes
public:
	BITMAPINFO *      bitmapInfo;

	unsigned char *   dibData;
	unsigned char *   dibImage;

	unsigned char *   srcData;
	unsigned char *   dstData;

	int               samplePerPixel, width, height, srcStep, dibStep, bitsAllocated, bitsStored;
	int               pixelRepresentation;
	double            windowWidth, windowCenter, windowWidthTemp, windowCenterTemp;
	KD_PHOTOMETRIC    photometric;

	int               GetRealWidth(int width);
	int               Clip(int value, int low, int high);

	void              GammaCorrection(double gamma);

	void              SpatialFilter3x3(double * mask);

	BOOL              m_bDown;
	CPoint            m_ptDown;

	void              OpenDICOMFile(CString path);

	BOOL              CreateDIB();
	void              Trans16to8();

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
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();

	afx_msg void OnArithmeticAdd();
	afx_msg void OnArithmeticSub();
	afx_msg void OnArithmeticMultiply();
	afx_msg void OnArithmeticDivide();
	afx_msg void OnArithmeticNegative();

	afx_msg void OnGeometricFlipV();
	afx_msg void OnGeometricFlipH();
	afx_msg void OnGeometricRotateLeft();
	afx_msg void OnGeometricRotateRight();

	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnFilterBlur();
	afx_msg void OnFilterSharpen();
};

