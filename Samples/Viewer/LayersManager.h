//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file  LayersManager.h
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////


#if !defined(AFX_LAYERSMANAGER_H__B9C954B5_7939_4637_B371_38949394ACF9__INCLUDED_)
#define AFX_LAYERSMANAGER_H__B9C954B5_7939_4637_B371_38949394ACF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CLayersManager dialog

class CLayersManager : public CDialog
{
// Construction
public:
	CLayersManager(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLayersManager)
	enum { IDD = IDD_DIALOG_LAYERS };
	CListBox	m_listLayers;
	int		m_nLineWidth;
	int		m_nMarkerSize;
	//}}AFX_DATA

	OgdcArray<OgdcDataset*> m_datasets;
	OgdcArray<OgdcStyle> m_styles;
	OgdcArray<OgdcString> m_names;
	OgdcDict<OgdcString, OgdcDataSource*>* m_pDataSources;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLayersManager)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLayersManager)
	afx_msg void OnButtonLayerAdd();
	afx_msg void OnButtonLayerRemove();
	afx_msg void OnButtonLinecolor();
	afx_msg void OnButtonMoveBottom();
	afx_msg void OnButtonMoveNext();
	afx_msg void OnButtonMovePrev();
	afx_msg void OnButtonMoveUp();
	afx_msg void OnChangeEditLinewidth();
	afx_msg void OnButtonFillcolor();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListLayers();
	afx_msg void OnCheckNullFill();
	afx_msg void OnChangeEditMarkersize();
	afx_msg void OnButtonMarkercolor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAYERSMANAGER_H__B9C954B5_7939_4637_B371_38949394ACF9__INCLUDED_)
