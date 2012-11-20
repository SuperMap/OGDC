//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file LayersManager.cpp
//!  \brief implementation file
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "viewer.h"
#include "LayersManager.h"
#include "DatasetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLayersManager dialog


CLayersManager::CLayersManager(CWnd* pParent /*=NULL*/)
	: CDialog(CLayersManager::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLayersManager)
	m_nLineWidth = 2;
	m_nMarkerSize = 10;
	//}}AFX_DATA_INIT
	m_pDataSources = NULL;
}


void CLayersManager::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLayersManager)
	DDX_Control(pDX, IDC_LIST_LAYERS, m_listLayers);
	DDX_Text(pDX, IDC_EDIT_LINEWIDTH, m_nLineWidth);
	DDX_Text(pDX, IDC_EDIT_MARKERSIZE, m_nMarkerSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLayersManager, CDialog)
	//{{AFX_MSG_MAP(CLayersManager)
	ON_BN_CLICKED(IDC_BUTTON_LAYER_ADD, OnButtonLayerAdd)
	ON_BN_CLICKED(IDC_BUTTON_LAYER_REMOVE, OnButtonLayerRemove)
	ON_BN_CLICKED(IDC_BUTTON_LINECOLOR, OnButtonLinecolor)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_BOTTOM, OnButtonMoveBottom)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_NEXT, OnButtonMoveNext)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_PREV, OnButtonMovePrev)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_UP, OnButtonMoveUp)
	ON_EN_CHANGE(IDC_EDIT_LINEWIDTH, OnChangeEditLinewidth)
	ON_BN_CLICKED(IDC_BUTTON_FILLCOLOR, OnButtonFillcolor)
	ON_LBN_SELCHANGE(IDC_LIST_LAYERS, OnSelchangeListLayers)
	ON_BN_CLICKED(IDC_CHECK_NULL_FILL, OnCheckNullFill)
	ON_EN_CHANGE(IDC_EDIT_MARKERSIZE, OnChangeEditMarkersize)
	ON_BN_CLICKED(IDC_BUTTON_MARKERCOLOR, OnButtonMarkercolor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLayersManager message handlers

void CLayersManager::OnButtonLayerAdd() 
{
	if(m_pDataSources == NULL || m_pDataSources->GetCount() == 0)
	{
		return;
	}

	CDatasetDlg dlg;
	dlg.m_bOnlyAdd = TRUE;
	dlg.m_pDataSources = m_pDataSources;	
	if(dlg.DoModal() == IDOK && dlg.m_pDataset != NULL && dlg.m_pDataSource != NULL)
	{
		OgdcStyle style;
		style.m_clrFillFore = dlg.m_corFill;
		style.m_clrLine = dlg.m_corLine;
		style.m_nLineWidth = dlg.m_nLineWidth;
		if(dlg.m_bNULLFill)
		{
			style.m_nFillStyle = 1;
		}

		OgdcString strName = dlg.m_pDataset->GetName() + "@" + dlg.m_pDataSource->GetAlias();		
		for(int i=0;i<m_datasets.GetSize();i++)
		{
			if(dlg.m_pDataset == m_datasets[i])
			{
				return;
			}
		}

		m_datasets.Add(dlg.m_pDataset);
		m_styles.Add(style);
		m_names.Add(strName);

		m_listLayers.InsertString(0, strName);
		m_listLayers.SetCurSel(0);
		OnSelchangeListLayers();
	}
}

void CLayersManager::OnButtonLayerRemove() 
{
	LONG nSel = m_listLayers.GetCurSel();
	LONG nCount = m_names.GetSize();
	if( nSel >= 0)
	{
		m_listLayers.DeleteString(nSel);
		m_datasets.RemoveAt(nCount-1-nSel);
		m_styles.RemoveAt(nCount-1-nSel);
		m_names.RemoveAt(nCount-1-nSel);

		nCount--;	
		if(nCount == 0)
		{
			nSel = -1;
		}
		if(nSel > nCount-1)
		{
			nSel--;
		}
		m_listLayers.SetCurSel(nSel);
		OnSelchangeListLayers();
	}		
}

void CLayersManager::OnButtonLinecolor() 
{
	LONG nSel = m_listLayers.GetCurSel();
	LONG nCount = m_names.GetSize();
	if( nSel >= 0)
	{
		OgdcStyle style = m_styles[nCount-1-nSel];
		CColorDialog dlg(style.m_clrLine);
		if(dlg.DoModal() == IDOK)
		{
			style.m_clrLine = dlg.GetColor();
			m_styles[nCount-1-nSel] = style;
		}
	}		
}

void CLayersManager::OnButtonMoveBottom() 
{
	LONG nSel = m_listLayers.GetCurSel();
	LONG nCount = m_names.GetSize();
	if( nSel >= 0 && nSel < nCount-1)
	{
		OgdcString strName = m_names[nCount-1-nSel];
		OgdcStyle style = m_styles[nCount-1-nSel];
		OgdcDataset* pDt = m_datasets[nCount-1-nSel];

		m_listLayers.DeleteString(nSel);
		m_datasets.RemoveAt(nCount-1-nSel);
		m_styles.RemoveAt(nCount-1-nSel);
		m_names.RemoveAt(nCount-1-nSel);
		
		m_datasets.InsertAt(0, pDt);
		m_styles.InsertAt(0, style);
		m_names.InsertAt(0, strName);
	
		m_listLayers.AddString(strName);
		m_listLayers.SetCurSel(m_listLayers.GetCount()-1);
		OnSelchangeListLayers();
	}		
}

void CLayersManager::OnButtonMoveNext() 
{
	LONG nSel = m_listLayers.GetCurSel();
	LONG nCount = m_names.GetSize();
	if( nSel >= 0 && nSel < nCount-1)
	{
		OgdcString strName = m_names[nCount-1-nSel];
		OgdcStyle style = m_styles[nCount-1-nSel];
		OgdcDataset* pDt = m_datasets[nCount-1-nSel];

		m_listLayers.DeleteString(nSel);
		m_datasets.RemoveAt(nCount-1-nSel);
		m_styles.RemoveAt(nCount-1-nSel);
		m_names.RemoveAt(nCount-1-nSel);
		
		m_datasets.InsertAt(nCount-nSel-2, pDt);
		m_styles.InsertAt(nCount-nSel-2, style);
		m_names.InsertAt(nCount-nSel-2, strName);
	
		m_listLayers.InsertString(nSel+1,strName);
		m_listLayers.SetCurSel(nSel+1);
		OnSelchangeListLayers();
	}		
}

void CLayersManager::OnButtonMovePrev() 
{
	LONG nSel = m_listLayers.GetCurSel();
	LONG nCount = m_names.GetSize();
	if( nSel > 0 && nSel <= nCount-1)
	{
		OgdcString strName = m_names[nCount-1-nSel];
		OgdcStyle style = m_styles[nCount-1-nSel];
		OgdcDataset* pDt = m_datasets[nCount-1-nSel];

		m_listLayers.DeleteString(nSel);
		m_datasets.RemoveAt(nCount-1-nSel);
		m_styles.RemoveAt(nCount-1-nSel);
		m_names.RemoveAt(nCount-1-nSel);
		
		m_datasets.InsertAt(nCount-nSel, pDt);
		m_styles.InsertAt(nCount-nSel, style);
		m_names.InsertAt(nCount-nSel, strName);
	
		m_listLayers.InsertString(nSel-1,strName);
		m_listLayers.SetCurSel(nSel-1);
		OnSelchangeListLayers();
	}		
}

void CLayersManager::OnButtonMoveUp() 
{
	LONG nSel = m_listLayers.GetCurSel();
	LONG nCount = m_names.GetSize();
	if( nSel > 0 && nSel <= nCount-1)
	{
		OgdcString strName = m_names[nCount-1-nSel];
		OgdcStyle style = m_styles[nCount-1-nSel];
		OgdcDataset* pDt = m_datasets[nCount-1-nSel];

		m_listLayers.DeleteString(nSel);
		m_datasets.RemoveAt(nCount-1-nSel);
		m_styles.RemoveAt(nCount-1-nSel);
		m_names.RemoveAt(nCount-1-nSel);
		
		m_datasets.Add(pDt);
		m_styles.Add(style);
		m_names.Add(strName);
	
		m_listLayers.InsertString(0,strName);
		m_listLayers.SetCurSel(0);
		OnSelchangeListLayers();
	}		
}

void CLayersManager::OnChangeEditLinewidth() 
{	
	LONG nSel = m_listLayers.GetCurSel();
	LONG nCount = m_names.GetSize();
	if( nSel >= 0)
	{
		OgdcStyle style = m_styles[nCount-1-nSel];
		CString str;
		GetDlgItem(IDC_EDIT_LINEWIDTH)->GetWindowText(str);
		style.m_nLineWidth = atoi(str);
		m_styles[nCount-1-nSel] = style;
	}		
}

void CLayersManager::OnButtonFillcolor() 
{
	LONG nSel = m_listLayers.GetCurSel();
	LONG nCount = m_names.GetSize();
	if( nSel >= 0)
	{
		OgdcStyle style = m_styles[nCount-1-nSel];
		CColorDialog dlg(style.m_clrFillFore);
		if(dlg.DoModal() == IDOK)
		{
			style.m_clrFillFore = dlg.GetColor();
			m_styles[nCount-1-nSel] = style;
		}
	}		
}

void CLayersManager::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CLayersManager::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CLayersManager::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_listLayers.ResetContent();
	for(int i=m_names.GetSize()-1;i>=0;i--)
	{
		m_listLayers.AddString(m_names[i]);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLayersManager::OnSelchangeListLayers() 
{
	LONG nSel = m_listLayers.GetCurSel();
	LONG nCount = m_names.GetSize();
	if( nSel >= 0)
	{
		OgdcStyle style = m_styles[nCount-1-nSel];
		CString str;
		str.Format("%d", style.m_nLineWidth);
		GetDlgItem(IDC_EDIT_LINEWIDTH)->SetWindowText(str);

		BOOL bNullFill = FALSE;
		if(style.m_nFillStyle == 1)
		{
			bNullFill = TRUE;
		}
		((CButton*)GetDlgItem(IDC_CHECK_NULL_FILL))->SetCheck(bNullFill);
	}	
}

void CLayersManager::OnCheckNullFill() 
{
	LONG nSel = m_listLayers.GetCurSel();
	LONG nCount = m_names.GetSize();
	if( nSel >= 0)
	{
		BOOL bNullFill = ((CButton*)GetDlgItem(IDC_CHECK_NULL_FILL))->GetCheck();
		OgdcStyle style = m_styles[nCount-1-nSel];
		style.m_nFillStyle = 0;
		if(bNullFill)
		{
			style.m_nFillStyle = 1;
		}
		m_styles[nCount-1-nSel] = style;
	}		
}

void CLayersManager::OnChangeEditMarkersize() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	LONG nSel = m_listLayers.GetCurSel();
	LONG nCount = m_names.GetSize();
	if( nSel >= 0)
	{
		OgdcStyle style = m_styles[nCount-1-nSel];
		CString str;
		GetDlgItem(IDC_EDIT_MARKERSIZE)->GetWindowText(str);
		style.m_nMarkerSize = atoi(str);
		m_styles[nCount-1-nSel] = style;
	}
	
}

void CLayersManager::OnButtonMarkercolor() 
{
	LONG nSel = m_listLayers.GetCurSel();
	LONG nCount = m_names.GetSize();
	if( nSel >= 0)
	{
		OgdcStyle style = m_styles[nCount-1-nSel];
		CColorDialog dlg(style.m_clrLine);
		if(dlg.DoModal() == IDOK)
		{
			style.m_clrLine = dlg.GetColor();
			m_styles[nCount-1-nSel] = style;
		}
	}
	
}
