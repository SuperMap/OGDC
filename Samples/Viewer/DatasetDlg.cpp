//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file DatasetDlg.cpp
//!  \brief implementation file
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "viewer.h"
#include "DatasetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDatasetDlg dialog


CDatasetDlg::CDatasetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDatasetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDatasetDlg)
	m_nLineWidth = 0;
	m_bNULLFill = FALSE;
	m_nMarkerSize = 10;
	//}}AFX_DATA_INIT
	m_pDataSources = NULL;
	m_pDataSource = NULL;
	m_pDataset = NULL;
	m_nType = 1;	
	m_corLine = RGB(0,0,0);
	m_corFill = RGB(255,255,255);
	m_bOnlyAdd = FALSE;
}

void CDatasetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDatasetDlg)
	DDX_Control(pDX, IDC_COMBO_DATASETS, m_cmbDatasets);
	DDX_Control(pDX, IDC_COMBO_DATASOURCES, m_cmbDataSources);
	DDX_Text(pDX, IDC_EDIT_LINEWIDTH, m_nLineWidth);
	DDV_MinMaxInt(pDX, m_nLineWidth, 0, 1000);
	DDX_Check(pDX, IDC_CHECK_NULL_FILL, m_bNULLFill);
	DDX_Text(pDX, IDC_EDIT_MARKERSIZE, m_nMarkerSize);
	DDV_MinMaxInt(pDX, m_nMarkerSize, 0, 100);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDatasetDlg, CDialog)
	//{{AFX_MSG_MAP(CDatasetDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_DATASETS, OnSelchangeDataset)
	ON_CBN_SELCHANGE(IDC_COMBO_DATASOURCES, OnSelchangeDataSource)
	ON_BN_CLICKED(IDC_RADIO_OPENDT, OnRadioOpenDataset)
	ON_BN_CLICKED(IDC_RADIO_ADDDT, OnRadioAddDataset)
	ON_BN_CLICKED(IDC_BUTTON_FILLCOLOR, OnButtonFillcolor)
	ON_BN_CLICKED(IDC_BUTTON_LINECOLOR, OnButtonLinecolor)
	ON_BN_CLICKED(IDC_BUTTON_MARKCOLOR, OnButtonMarkcolor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDatasetDlg message handlers

void CDatasetDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CDatasetDlg::OnOK() 
{
	UpdateData(TRUE);
	CDialog::OnOK();
}

BOOL CDatasetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	

	if(m_bOnlyAdd)
	{
		GetDlgItem(IDC_RADIO_OPENDT)->EnableWindow(false);
	}
	
	if(m_pDataSources != NULL)
	{		
		INT i = 0;
		LONG nCurSel = 0;			
		OgdcString strAlias;
		OgdcDataSource* pDS = NULL;
		OgdcDict<OgdcString, OgdcDataSource*>::POSITION pos = m_pDataSources->GetStartPosition();
		while(!m_pDataSources->IsEOF(pos))
		{
			m_pDataSources->GetNextAssoc(pos, strAlias, pDS);
			m_cmbDataSources.AddString(strAlias);
			if(pDS == m_pDataSource)
			{
				nCurSel = i;
			}
			i++;
		}

		if(m_cmbDataSources.GetCount() > 0)
		{
			m_cmbDataSources.SetCurSel(nCurSel);
			OnSelchangeDataSource();
		}
		((CButton*)GetDlgItem(IDC_RADIO_ADDDT))->SetCheck(true);
		OnRadioAddDataset();

		UpdateData(FALSE);
	}	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDatasetDlg::OnSelchangeDataset() 
{
	LONG nSelect = m_cmbDatasets.GetCurSel();
	if(nSelect >= 0 && m_pDataSource != NULL)
	{
		CString strName;
		m_cmbDatasets.GetLBText(nSelect, strName);
		OgdcString strDtName = (OgdcString)strName;
		m_pDataset = m_pDataSource->GetDataset(strDtName);
	}
	else
	{
		m_pDataset = NULL;
	}
}

void CDatasetDlg::OnSelchangeDataSource() 
{
	LONG nSelect = m_cmbDataSources.GetCurSel();
	if(nSelect >= 0)
	{
		CString strAlias;
		m_cmbDataSources.GetLBText(nSelect, strAlias);
		OgdcString strTmp = strAlias;
		m_pDataSources->Lookup(strTmp, m_pDataSource);
	}
	else
	{
		m_pDataSource = NULL;
	}

	m_cmbDatasets.ResetContent();
	if(m_pDataSource != NULL)
	{
		LONG nSize = 0;
		for(int i=0;i<m_pDataSource->GetDatasetCount();i++)
		{
			OgdcDataset* pDataset = m_pDataSource->GetDataset(i);
			if(pDataset != NULL && pDataset->GetType() != OgdcDataset::Tabular)
			{
				m_cmbDatasets.AddString(pDataset->GetName());
				nSize++;
			}
		}
		if(nSize > 0)
		{
			m_cmbDatasets.SetCurSel(0);
			OnSelchangeDataset();
		}
	}
}

void CDatasetDlg::OnRadioOpenDataset() 
{
	m_nType = 2;
}

void CDatasetDlg::OnRadioAddDataset() 
{
	m_nType = 1;
}

void CDatasetDlg::OnButtonFillcolor() 
{
	CColorDialog dlg(m_corFill);
	if(dlg.DoModal() == IDOK)
	{
		m_corFill = dlg.GetColor();
	}	
}

void CDatasetDlg::OnButtonLinecolor() 
{
	CColorDialog dlg(m_corLine);
	if(dlg.DoModal() == IDOK)
	{
		m_corLine = dlg.GetColor();
	}	
}

void CDatasetDlg::OnButtonMarkcolor() 
{
	CColorDialog dlg(m_corLine);
	if(dlg.DoModal() == IDOK)
	{
		m_corLine = dlg.GetColor();
	}
	
}
