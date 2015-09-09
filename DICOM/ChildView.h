
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

	int             GetRealWidth(int width);
	unsigned char   Clip(int value, int low, int high);
	unsigned char   * dibData;
	BITMAPINFO      * bitmapInfo;
	int             samplePerPixel;
	int             imageWidth;
	int             imageHeight;
	int             imageStep;
	CString         photometric;
	int             BitsAllocated, BitsStored, PixelRepresentation;
	double          windowWidth, windowCenter;
	unsigned char   * srcData;
	unsigned char   * dstData;
	unsigned char   * temData;
	//CScrollBar	scrollBar;
	void            GammaCorrection(double gamma);
	void            SpatialFilter3x3(double * mask);
	void			twobytetoonebyte();
	
// Operations
public:

	BOOL			leftButtonDown;
	CPoint			leftButtonPoint;
	BOOL			rightButtonDown;
	CPoint			rightButtonPoint;
	BOOL            CreateDIB();
	void            OpenBMPFile(CString path);
	void            OpenDCMFile(CString path);

// Overrides
protected:

	virtual BOOL    PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:

	virtual ~CChildView();

	// Generated message map functions
protected:

	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

public:

	afx_msg void    OnFileOpen();
	afx_msg void    OnArithmeticAdd();
	afx_msg void    OnArithmeticSubtract();
	afx_msg void	OnArithmeticMultiply();
	afx_msg void	OnArithmeticDivide();
	afx_msg void	OnArithmeticNegative();
	afx_msg void	OnArithmeticLogtransform();
	afx_msg void	OnGeometryFlipv();
	afx_msg void	OnGeometryFliph();
	afx_msg void	OnGeometryLRotate90();
	afx_msg void	OnGeometryRRotate90();
	afx_msg void	OnLutAdd();
	afx_msg void	OnUpdateLutAdd(CCmdUI *pCmdUI);
	afx_msg int		OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void	OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void	OnLutNegative();
	afx_msg void	OnUpdateLutNegative(CCmdUI *pCmdUI);
	afx_msg void	OnUpdateLutGamma(CCmdUI *pCmdUI);
	afx_msg void	OnLutGamma();
	afx_msg void	OnFilterBlur();
	afx_msg void	OnFilterSharpen();
	afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void	OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void	OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void	OnRButtonUp(UINT nFlags, CPoint point);
};

