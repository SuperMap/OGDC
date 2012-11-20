//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file  OpenDBDlg.h
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////


#if !defined(AFX_OPENDBDLG_H__4F959AC6_5593_40DA_93B7_94C0B0F6BF67__INCLUDED_)
#define AFX_OPENDBDLG_H__4F959AC6_5593_40DA_93B7_94C0B0F6BF67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// COpenDBDlg dialog

class COpenDBDlg : public CDialog
{
// Construction
public:
	COpenDBDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COpenDBDlg)
	enum { IDD = IDD_DIALOG_DATASOURCE };
	CEdit	m_ctrServer;
	CComboBox	m_cmbEngine;
	CString	m_strAlias;
	CString	m_strDatabase;
	CString	m_strPassWord;
	CString	m_strServer;
	CString	m_strUser;
	//}}AFX_DATA
	CString m_strWindowText;
	LONG m_nEngineType;
	OgdcDict<OgdcString, OgdcDataSource*>* m_pDataSources;

	OgdcProvider m_Provider;
	OgdcInt m_EngineCount;
	//判断是打开数据源还是新建数据源，true 打开数据源，false 新建数据源
	OgdcBool m_bOpen;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpenDBDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COpenDBDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonPath();
	virtual void OnOK();
	afx_msg void OnSelchangeComboEnginetype();
	afx_msg void OnChangeEditServer();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENDBDLG_H__4F959AC6_5593_40DA_93B7_94C0B0F6BF67__INCLUDED_)
