//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file  CopyDatasetDlg.h
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////
#if !defined(AFX_COPYDATASETDLG_H__8CC35145_F1CD_48E3_A638_2CB0E94B9DAA__INCLUDED_)
#define AFX_COPYDATASETDLG_H__8CC35145_F1CD_48E3_A638_2CB0E94B9DAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CopyDatasetDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCopyDatasetDlg dialog

class CCopyDatasetDlg : public CDialog
{
// Construction
public:
	CCopyDatasetDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCopyDatasetDlg)
	enum { IDD = IDD_DIALOG_COPYDATASET };
	CEdit	m_editDTName;
	CComboBox	m_cmbObjDatasource;
	CComboBox	m_cmbSrcDataset;
	CComboBox	m_cmbSrcDatasource;
	CString	m_strSrcDTName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCopyDatasetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	OgdcDict<OgdcString, OgdcDataSource*>* m_pDataSources;
    //! \brief 复制到的目标数据源
	OgdcDataSource* m_pObjDataSource;
	//! \brief 被复制的数据集
	OgdcDataset* m_pDataset;
	//! \brief 被复制数据集所在的数据源
	OgdcDataSource* m_pSrcDataSource;
	
	BOOL bChangeDS;
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCopyDatasetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeDataset();
	afx_msg void OnSelchangeSrcdatasource();
	afx_msg void OnSelchangeObjdatasource();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COPYDATASETDLG_H__8CC35145_F1CD_48E3_A638_2CB0E94B9DAA__INCLUDED_)
