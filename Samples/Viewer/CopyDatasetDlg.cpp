//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file  CopyDatasetDlg.cpp
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////
// CopyDatasetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "viewer.h"
#include "CopyDatasetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCopyDatasetDlg dialog


CCopyDatasetDlg::CCopyDatasetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCopyDatasetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCopyDatasetDlg)
	m_strSrcDTName = _U("");
	//}}AFX_DATA_INIT
	

	m_pDataSources = NULL;
	m_pObjDataSource = NULL;
	m_pDataset = NULL;
	m_pSrcDataSource = NULL;
}


void CCopyDatasetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCopyDatasetDlg)
	DDX_Control(pDX, IDC_EDIT_DTNAME, m_editDTName);
	DDX_Control(pDX, IDC_COMBO_OBJDATASOURCE, m_cmbObjDatasource);
	DDX_Control(pDX, IDC_COMBO_SRCDATASET, m_cmbSrcDataset);
	DDX_Control(pDX, IDC_COMBO_SRCDATASOURCE, m_cmbSrcDatasource);
	DDX_Text(pDX, IDC_EDIT_DTNAME, m_strSrcDTName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCopyDatasetDlg, CDialog)
	//{{AFX_MSG_MAP(CCopyDatasetDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_SRCDATASET, OnSelchangeDataset)
	ON_CBN_SELCHANGE(IDC_COMBO_SRCDATASOURCE, OnSelchangeSrcdatasource)
	ON_CBN_SELCHANGE(IDC_COMBO_OBJDATASOURCE, OnSelchangeObjdatasource)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCopyDatasetDlg message handlers

BOOL CCopyDatasetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if (m_pDataSources != NULL)
	{
		int i = 0;
		OgdcString strAlias;
		OgdcDataSource* pDS = NULL;
		OgdcDict<OgdcString, OgdcDataSource*>::POSITION pos = m_pDataSources->GetStartPosition();
		while (!m_pDataSources->IsEOF(pos))
		{
			m_pDataSources->GetNextAssoc(pos, strAlias, pDS);
			m_cmbSrcDatasource.AddString(strAlias); 
			m_cmbObjDatasource.AddString(strAlias); 
		}
	}

	m_cmbSrcDatasource.SetCurSel(0);
	m_cmbObjDatasource.SetCurSel(0);
	OnSelchangeSrcdatasource();
	OnSelchangeObjdatasource();


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCopyDatasetDlg::OnSelchangeDataset() 
{
	// TODO: Add your control notification handler code here
	LONG nSelect = m_cmbSrcDataset.GetCurSel();		
	if(nSelect >= 0)
	{
		CString strName;
		m_cmbSrcDataset.GetLBText(nSelect, strName);
		OgdcString strDtName = (OgdcString)strName;
		m_pDataset = m_pSrcDataSource->GetDataset(strDtName);
		
		m_editDTName.SetWindowText(strDtName);

	}
	else
	{
		m_pDataset = NULL;
	}
	
}

void CCopyDatasetDlg::OnSelchangeSrcdatasource() 
{
	// TODO: Add your control notification handler code here
	LONG nSelect = m_cmbSrcDatasource.GetCurSel();
	if(nSelect >= 0)
	{
		CString strAlias;
		m_cmbSrcDatasource.GetLBText(nSelect, strAlias);
		OgdcString strTmp = strAlias;
		m_pDataSources->Lookup(strTmp, m_pSrcDataSource);
		
		m_cmbSrcDataset.ResetContent();

		for(int j = 0; j < m_pSrcDataSource->GetDatasetCount();j++)
		{
			m_pDataset = m_pSrcDataSource->GetDataset(j);
			OgdcString strName;
			if (m_pDataset!=NULL) {
				strName = m_pDataset->GetName();
			}
			else
				strName = _U("");
			m_cmbSrcDataset.AddString(strName);				
		}
		m_cmbSrcDataset.SetCurSel(0);
		OnSelchangeDataset();
	}
	else
	{
		m_pSrcDataSource = NULL;
	}
	
}

void CCopyDatasetDlg::OnSelchangeObjdatasource() 
{
	// TODO: Add your control notification handler code here
	LONG nSelect = m_cmbObjDatasource.GetCurSel();
	if(nSelect >= 0)
	{
		CString strAlias;
		m_cmbObjDatasource.GetLBText(nSelect, strAlias);
		OgdcString strTmp = strAlias;
		m_pDataSources->Lookup(strTmp, m_pObjDataSource);
		OgdcString strDtName;
		if (m_pDataset!=NULL) {
			strDtName = m_pDataset->GetName();
		}
		else strDtName = _U("");
		CString m_strObjDTName = m_pObjDataSource->GetUnoccupiedDatasetName(strDtName);
		m_editDTName.SetWindowText(m_strObjDTName);		
	}
	else
	{
		m_pObjDataSource = NULL;
	}
	
}

