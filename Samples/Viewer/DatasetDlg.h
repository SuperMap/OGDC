//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file DatasetDlg.h
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATASETDLG_H__0E2951ED_64DA_4510_AE9B_940272E2D4E3__INCLUDED_)
#define AFX_DATASETDLG_H__0E2951ED_64DA_4510_AE9B_940272E2D4E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CDatasetDlg dialog

class CDatasetDlg : public CDialog
{
// Construction
public:
	CDatasetDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDatasetDlg)
	enum { IDD = IDD_DLG_DATASET_SELECT };
	CComboBox	m_cmbDatasets;
	CComboBox	m_cmbDataSources;
	int		m_nLineWidth;
	BOOL	m_bNULLFill;
	int		m_nMarkerSize;
	//}}AFX_DATA

	BOOL m_bOnlyAdd;
	LONG m_nType;
	COLORREF m_corLine;
	COLORREF m_corFill;
	OgdcDataset* m_pDataset;
	OgdcDataSource* m_pDataSource;
	OgdcDict<OgdcString, OgdcDataSource*>* m_pDataSources;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDatasetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDatasetDlg)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeDataset();
	afx_msg void OnSelchangeDataSource();
	afx_msg void OnRadioOpenDataset();
	afx_msg void OnRadioAddDataset();
	afx_msg void OnButtonFillcolor();
	afx_msg void OnButtonLinecolor();
	afx_msg void OnButtonMarkcolor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATASETDLG_H__0E2951ED_64DA_4510_AE9B_940272E2D4E3__INCLUDED_)
