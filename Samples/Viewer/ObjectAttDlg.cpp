//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file ObjectAttDlg.cpp
//!  \brief implementation file
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "viewer.h"
#include "ObjectAttDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObjectAttDlg dialog


CObjectAttDlg::CObjectAttDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CObjectAttDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CObjectAttDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pRecordset = NULL;
}


void CObjectAttDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjectAttDlg)
	DDX_Control(pDX, IDC_LIST_OBJECT_ATT, m_lstObject);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CObjectAttDlg, CDialog)
	//{{AFX_MSG_MAP(CObjectAttDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjectAttDlg message handlers

BOOL CObjectAttDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(m_pRecordset != NULL)
	{		
		m_lstObject.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_INFOTIP|LVS_EX_FULLROWSELECT);
		LVCOLUMN lvColumn;
		lvColumn.mask = LVCF_TEXT|LVCF_WIDTH|LVCF_FMT;
		lvColumn.fmt = LVCFMT_LEFT;
		
		CString strText;
		lvColumn.cx = 100;
		
		strText.LoadString(IDS_FIELD_NAME);
		OgdcString strTmp = (OgdcString)strText;
		lvColumn.pszText = (OgdcChar*)strTmp.Cstr();
		
		m_lstObject.InsertColumn(0, &lvColumn);

		lvColumn.cx = 100;
		strText.LoadString(IDS_FIELD_VALUE);
		OgdcString strTmp2 = (OgdcString)strText;
		lvColumn.pszText = (OgdcChar*)strTmp2.Cstr();
		m_lstObject.InsertColumn(1, &lvColumn);

		OgdcFieldInfos fieldInfos;
		m_pRecordset->GetFieldInfos(fieldInfos);
		for(int i=0;i<fieldInfos.GetSize();i++)
		{
			OgdcFieldInfo fieldInfo = fieldInfos.GetAt(i);
			OgdcString strName = fieldInfo.m_strForeignName;
			if(strName.IsEmpty())
			{
				strName = fieldInfo.m_strName;
			}
			m_lstObject.InsertItem(i, strName);
			m_lstObject.SetItemText(i, 0, fieldInfo.m_strName);
			OgdcVariant var;
			m_pRecordset->GetFieldValue(fieldInfo.m_strName, var);
			m_lstObject.SetItemText(i, 1, var.ToString());
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
