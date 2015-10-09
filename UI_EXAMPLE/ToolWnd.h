#pragma once

class ToolWnd : public CWnd
{
	DECLARE_DYNAMIC(ToolWnd)

public:
	ToolWnd();
	virtual ~ToolWnd();

public:

	CButton testButton;
	CButton AddButton;
	CButton SubButton;
	CButton BvecButton;
	CButton FAButton;
	int		ind;
	float	BV[3][15];

protected:

	DECLARE_MESSAGE_MAP()

public:

	afx_msg int		OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL	OnEraseBkgnd(CDC* pDC);
	afx_msg void	OnSize(UINT nType, int cx, int cy);
	afx_msg void	OnPaint();
	afx_msg void	OnButtonTest();
	afx_msg void	OnBvecButton();
	afx_msg void	OnFAButton();
};


