//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file  ObjectAttDlg.h
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////



#if !defined(AFX_OBJECTATTDLG_H__DA53EBE8_463C_43D8_889D_31A1F059A947__INCLUDED_)
#define AFX_OBJECTATTDLG_H__DA53EBE8_463C_43D8_889D_31A1F059A947__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CObjectAttDlg dialog

class CObjectAttDlg : public CDialog
{
// Construction
public:
	CObjectAttDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CObjectAttDlg)
	enum { IDD = IDD_DIALOG_OBJECT_ATT };
	CListCtrl	m_lstObject;
	//}}AFX_DATA

	OgdcRecordset* m_pRecordset;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectAttDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CObjectAttDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJECTATTDLG_H__DA53EBE8_463C_43D8_889D_31A1F059A947__INCLUDED_)
