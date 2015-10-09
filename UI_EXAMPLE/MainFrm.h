#pragma once

class ViewWnd;
class ToolWnd;
class DTIclass;
class DTIDIB;
class FLAIRDIB;
class FLAIRclass;
class FACAL;
class FADIB;
class FILEPROC;

class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// Attributes
public:
	ViewWnd			* viewWnd;
	ToolWnd			* toolWnd;
	DTIclass		* fileopen;
	DTIDIB			* dib;
	FLAIRDIB		* tdib;
	FLAIRclass		* tfileopen;
	FACAL			* fa;
	FADIB			* fadib;
	FILEPROC		* fileproc;
	int				a,b,c;
	CScrollBar		scrollBar, tscrollBar;
// Operations

public:
	CRect clientRect, viewRect, toolRect;

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:	
	void				CLASSIFY(CString		* path);
	CString				FindFile(CString strParentPath);
	CString				Seq;
	CString				strFullPath[1192];
	CString				DTI[928];
	CString				FLAIR[20];
	CString				name;	

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};


