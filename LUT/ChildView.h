
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
	int GetRealWidth(int width);
	unsigned char Clip(int value, int low, int high);
	unsigned char * dibData;
	BITMAPINFO * bitmapInfo;
	int samplePerPixel;
	int imageWidth;
	int imageHeight;
	int imageStep;
	unsigned char * srcData;
	unsigned char * dstData;

	CScrollBar	scrollBar;

	void GammaCorrection(double gamma);
	
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

	afx_msg void OnFileOpen();
	afx_msg void OnArithmeticAdd();
	afx_msg void OnArithmeticSubtract();
	afx_msg void OnArithmeticMultiply();
	afx_msg void OnArithmeticDivide();
	afx_msg void OnArithmeticNegative();
	afx_msg void OnArithmeticLogtransform();
	afx_msg void OnGeometryFlipv();
	afx_msg void OnGeometryFliph();
	afx_msg void OnGeometryLRotate90();
	afx_msg void OnGeometryRRotate90();
	afx_msg void OnLutAdd();
	afx_msg void OnUpdateLutAdd(CCmdUI *pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLutNegative();
	afx_msg void OnUpdateLutNegative(CCmdUI *pCmdUI);
	afx_msg void OnUpdateLutGamma(CCmdUI *pCmdUI);
	afx_msg void OnLutGamma();
};

