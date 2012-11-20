//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file  ViewerView.h
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////


#if !defined(AFX_VIEWERVIEW_H__4832D293_1E09_48BC_A3BB_8803F70654D9__INCLUDED_)
#define AFX_VIEWERVIEW_H__4832D293_1E09_48BC_A3BB_8803F70654D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ViewerWnd.h"

class CViewerView : public CView
{
protected: // create from serialization only
	CViewerView();
	DECLARE_DYNCREATE(CViewerView)

// Attributes
public:
	CViewerDoc* GetDocument();
	void Close();

// Operations
public:
	CViewerWnd m_viewerWnd;
	OgdcDict<OgdcString, OgdcDataSource*> m_dataSources;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewerView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CViewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CViewerView)
	afx_msg void OnFileOpen();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnPan();
	afx_msg void OnRefresh();
	afx_msg void OnSelect();
	afx_msg void OnViewEntire();
	afx_msg void OnZoomin();
	afx_msg void OnZoomout();
	afx_msg void OnMapOpen();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnFileClose();
	afx_msg void OnObjectAtt();
	afx_msg void OnLayersManager();
	afx_msg void OnOpenMapChina();
	afx_msg void OnOpenMapGeologic();
	afx_msg void OnCopyDataset();
	afx_msg void OnFileNewDs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ViewerView.cpp
inline CViewerDoc* CViewerView::GetDocument()
   { return (CViewerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWERVIEW_H__4832D293_1E09_48BC_A3BB_8803F70654D9__INCLUDED_)
