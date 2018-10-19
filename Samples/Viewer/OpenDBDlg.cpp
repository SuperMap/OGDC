//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file OpenDBDlg.cpp
//!  \brief implementation file
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Viewer.h"
#include "OpenDBDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COpenDBDlg dialog


COpenDBDlg::COpenDBDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COpenDBDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COpenDBDlg)
	m_strAlias = _T("");
	m_strDatabase = _T("");
	m_strPassWord = _T("");
	m_strServer = _T("");
	m_strUser = _T("");
	//}}AFX_DATA_INIT
	
	m_nEngineType = OGDC::oeNone;

	m_pDataSources = NULL;
	m_bOpen = true;
}


void COpenDBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COpenDBDlg)
	DDX_Control(pDX, IDC_EDIT_SERVER, m_ctrServer);
	DDX_Control(pDX, IDC_COMBO_ENGINETYPE, m_cmbEngine);
	DDX_Text(pDX, IDC_EDIT_ALIAS, m_strAlias);
	DDX_Text(pDX, IDC_EDIT_DATABASE, m_strDatabase);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassWord);
	DDX_Text(pDX, IDC_EDIT_SERVER, m_strServer);
	DDX_Text(pDX, IDC_EDIT_USER, m_strUser);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COpenDBDlg, CDialog)
	//{{AFX_MSG_MAP(COpenDBDlg)
	ON_BN_CLICKED(IDC_BUTTON_PATH, OnButtonPath)
	ON_CBN_SELCHANGE(IDC_COMBO_ENGINETYPE, OnSelchangeComboEnginetype)
	ON_EN_CHANGE(IDC_EDIT_SERVER, OnChangeEditServer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpenDBDlg message handlers

BOOL COpenDBDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CString strText = _T("");	
	if (m_bOpen) 
	{
		strText.LoadString(IDS_STRING_OPEN);
		SetWindowText(strText);
	}
	else
	{
		strText.LoadString(IDS_STRING_NEW);
		SetWindowText(strText);
		
	}
	GetDlgItem(IDC_BUTTON_PATH)->EnableWindow(FALSE);
	OgdcProviderManager::LoadAllProvider(); 

	m_EngineCount = OgdcProviderManager::GetProviderCount();
	for(int i=0;i<m_EngineCount;i++)
	{
		m_Provider = OgdcProviderManager::GetAt(i);
		OgdcString m_EngineName = OgdcProviderManager::GetProviderName(m_Provider.m_nType);
		m_cmbEngine.AddString(m_EngineName);
		
	}
	m_cmbEngine.SelectString(0,_U("Universal DataBase Engine"));
	OnSelchangeComboEnginetype();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COpenDBDlg::OnButtonPath() 
{	
	CString strFilter = _U("SuperMap UDB File(*.udb)|*.udb||");

	if(m_nEngineType == 5 )
	{
		strFilter = _U("SuperMap Image File(*.sit;*.bmp;*.jpg;*.png)|*.sit;*.bmp;*.jpg;*.png||");
	}
	else if( m_nEngineType == OGDC::oeFile)
	{
		strFilter = _U("SuperMap UDB File(*.udb)|*.udb||");
	}
	else
	{		
	    strFilter = m_Provider.m_strName+ _U("(") + m_Provider.GetPostfix()+ _U(")") +_U("|")+ m_Provider.GetPostfix()+_U("||" );	
	}
	

	CFileDialog FileDialog(m_bOpen, _U(""), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);
	//////////////////////////////////////////////////////////////////////////
	//获取数据所在的绝对路径
	HMODULE module = GetModuleHandle(0);
	OgdcChar path[MAX_PATH];
	GetModuleFileName(module,path,MAX_PATH);
	OgdcString strApplicationPath = path;
	int nPos = strApplicationPath.Find(_U("Builds"));
	OgdcString strDataPath = strApplicationPath.Left(nPos) + _U("SampleData\\");
	//////////////////////////////////////////////////////////////////////////

	FileDialog.m_ofn.lpstrInitialDir = strDataPath;
	if(IDOK == FileDialog.DoModal())
	{
		m_strDatabase = _U("");
		m_strPassWord = _U("");
		m_strUser = _U("");
		m_strServer = FileDialog.GetPathName();
		m_strAlias = FileDialog.GetFileTitle();
		UpdateData(false);
		
	}
	
}

void COpenDBDlg::OnOK() 
{	 
	UpdateData();	
	OgdcString strAlias = m_strAlias;
	if(strAlias.IsEmpty() ||
		(m_pDataSources != NULL && m_pDataSources->Lookup(strAlias) != NULL))
	{		
		AfxMessageBox(IDS_ALIAS_ISEXISTED);
		return;
	}
	OgdcInt n_Class = m_Provider.GetEngineClass();	
	if(n_Class == 2)
	{
		OgdcString strTmpServer = OgdcString(m_strServer);
		char cTmp = _U('\\');
		if (strTmpServer.Find(cTmp,0)==-1) 
		{
			AfxMessageBox(_U("请输入正确的路径！"));
		     return;
		}
	}
	CDialog::OnOK();
}

void COpenDBDlg::OnSelchangeComboEnginetype() 
{	
	m_strAlias = _U("");
	m_strDatabase = _U("");
	m_strPassWord = _U("");
	m_strServer = _U("");
	m_strUser = _U("");
	UpdateData(false);
	
	LONG nType = m_cmbEngine.GetCurSel();
	if (nType==LB_ERR)
	{
		MessageBox(_U("找不到Provider动态链接库文件（*.sdx、*.odx）"));
		return;
	}
	CString strName;
	m_cmbEngine.GetLBText(nType,strName);
	m_EngineCount = OgdcProviderManager::GetProviderCount();
	int i = 0;
	while(i < m_EngineCount)	
	{
		m_Provider = OgdcProviderManager::GetAt(i);
		if (strName.CompareNoCase(m_Provider.m_strName)==0) 
		{
			i = m_EngineCount;
			m_nEngineType = m_Provider.m_nType;
			OgdcInt n_Class = m_Provider.GetEngineClass();
			switch(n_Class) 
			{
			case 1:
				GetDlgItem(IDC_BUTTON_PATH)->EnableWindow(false);
				GetDlgItem(IDC_EDIT_DATABASE)->EnableWindow(true);
				GetDlgItem(IDC_EDIT_USER)->EnableWindow(true);
				break;
			case 2:
				GetDlgItem(IDC_BUTTON_PATH)->EnableWindow(true);
				GetDlgItem(IDC_EDIT_DATABASE)->EnableWindow(false);
				GetDlgItem(IDC_EDIT_USER)->EnableWindow(false);
				break;
			case 3:
				GetDlgItem(IDC_BUTTON_PATH)->EnableWindow(false);
				GetDlgItem(IDC_EDIT_DATABASE)->EnableWindow(true);
				GetDlgItem(IDC_EDIT_USER)->EnableWindow(true);
			default:
				break;
			}
		}
	
		i++;
	}	
}

void COpenDBDlg::OnChangeEditServer() 
{

}


