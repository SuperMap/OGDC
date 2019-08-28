//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file ViewerView.cpp
//!  \brief implementation of the CViewerView class
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "Viewer.h"
#include "OpenDBDlg.h"
#include "DatasetDlg.h"
#include "ObjectAttDlg.h"
#include "LayersManager.h"

#include "ViewerDoc.h"
#include "ViewerView.h"
#include "Base/OgdcRect.h"
#include "CopyDatasetDlg.h"

//#ifdef _DEBUG
//#define new DEBNEW
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#endif

/////////////////////////////////////////////////////////////////////////////
// CViewerView

IMPLEMENT_DYNCREATE(CViewerView, CView)

BEGIN_MESSAGE_MAP(CViewerView, CView)
	//{{AFX_MSG_MAP(CViewerView)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_EDIT_PAN, OnPan)
	ON_COMMAND(ID_EDIT_REFRESH, OnRefresh)
	ON_COMMAND(ID_EDIT_SELECT, OnSelect)
	ON_COMMAND(ID_EDIT_VIEWENTIRE, OnViewEntire)
	ON_COMMAND(ID_EDIT_ZOOMIN, OnZoomin)
	ON_COMMAND(ID_EDIT_ZOOMOUT, OnZoomout)
	ON_COMMAND(ID_FILE_NEW, OnMapOpen)
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_COMMAND(ID_OBJECT_ATT, OnObjectAtt)
	ON_COMMAND(ID_LAYERS_MANAGER, OnLayersManager)
	ON_COMMAND(ID_OPEN_MAP_CHINA, OnOpenMapChina)
	ON_COMMAND(ID_OPEN_MAP_GEOLOGIC, OnOpenMapGeologic)
	ON_COMMAND(ID_COPY_DATASET, OnCopyDataset)
	ON_COMMAND(ID_FILE_NEW_DS, OnFileNewDs)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewerView construction/destruction

CViewerView::CViewerView()
{
	// TODO: add construction code here
}

CViewerView::~CViewerView()
{
	Close();
}

BOOL CViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CViewerView drawing

void CViewerView::OnDraw(CDC* pDC)
{
	CViewerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	CRect rc;
	GetClientRect(&rc);
	m_viewerWnd.OnDraw(pDC, rc);
}

/////////////////////////////////////////////////////////////////////////////
// CViewerView printing

BOOL CViewerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CViewerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

//************************************
// Method:    OnEndPrinting
// FullName:  CViewerView::OnEndPrinting
// Access:    virtual protected 
// Returns:   void
// Qualifier:
// Parameter: CDC *
// Parameter: CPrintInfo *
//************************************
void CViewerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CViewerView diagnostics

#ifdef _DEBUG
void CViewerView::AssertValid() const
{
	CView::AssertValid();
}

void CViewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CViewerDoc* CViewerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CViewerDoc)));
	return (CViewerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewerView message handlers

void CViewerView::Close()
{
	m_viewerWnd.OnClose();

	OgdcDict<OgdcString, OgdcDataSource*>::POSITION pos = m_dataSources.GetStartPosition();
	OgdcString strAlias;
	OgdcDataSource* pDS = NULL;
	while(!m_dataSources.IsEOF(pos))
	{
		m_dataSources.GetNextAssoc(pos, strAlias, pDS);
		if(pDS != NULL)
		{
			pDS->Close();
			delete pDS;
			pDS = NULL;
		}
	}
	m_dataSources.RemoveAll();
}

void CViewerView::OnFileOpen() 
{
	COpenDBDlg dlg;
	dlg.m_pDataSources = &m_dataSources;
	dlg.m_bOpen = true;
	if(dlg.DoModal() == IDOK)
	{
		OgdcDataSource* pDS = OgdcProviderManager::CreateOgdcDataSource(dlg.m_nEngineType);
		if(pDS != NULL)
		{
			pDS->m_connection.m_nType = dlg.m_nEngineType;
			pDS->m_connection.m_strAlias = (OgdcString)dlg.m_strAlias;
			pDS->m_connection.m_strServer = (OgdcString)dlg.m_strServer;
			pDS->m_connection.m_strDatabase = (OgdcString)dlg.m_strDatabase;
			pDS->m_connection.m_strUser = (OgdcString)dlg.m_strUser;
			pDS->m_connection.m_strPassword = (OgdcString)dlg.m_strPassWord;
			if (pDS->m_connection.m_nType==OGDC::oeSQLServer) {
				pDS->m_connection.m_strDriver =  _U("SQL Server");
			}
										
			CWaitCursor cursor;
			if(!pDS->Open())
			{
				delete pDS;
				pDS = NULL;
			}
			else 
			{
				m_dataSources.SetAt(pDS->m_connection.m_strAlias, pDS);
				CDatasetDlg dlg;
				dlg.m_pDataSource = pDS;
				dlg.m_pDataSources = &m_dataSources;
				if(dlg.DoModal() == IDOK && dlg.m_pDataset != NULL && dlg.m_pDataSource != NULL)
				{
					OgdcStyle style;
					style.m_clrFillFore = dlg.m_corFill;
					style.m_clrLine = dlg.m_corLine;
					style.m_nLineWidth = dlg.m_nLineWidth;
					style.m_nMarkerSize = dlg.m_nMarkerSize;
					if(dlg.m_bNULLFill)
					{
						style.m_nFillStyle = 1;
					}

					OgdcString strName = dlg.m_pDataset->GetName() + _U("@") + dlg.m_pDataSource->GetAlias();
					m_viewerWnd.AddDataset(dlg.m_pDataset, style, strName,(dlg.m_nType != 1));					
					OnRefresh();
				}
			}
		}
		if(pDS == NULL)
		{
			AfxMessageBox(IDS_OPEN_DATASOURCE_FAILED);
		}
		
	}
}

void CViewerView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	OnObjectAtt();
	CView::OnLButtonDblClk(nFlags, point);
}

void CViewerView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_viewerWnd.OnLButtonDown(nFlags, point);	
	CView::OnLButtonDown(nFlags, point);
}

void CViewerView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_viewerWnd.OnLButtonUp(nFlags, point);
	CView::OnLButtonUp(nFlags, point);
}

void CViewerView::OnMouseMove(UINT nFlags, CPoint point) 
{
	m_viewerWnd.OnMouseMove(nFlags, point);
	CView::OnMouseMove(nFlags, point);
}

BOOL CViewerView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if(nFlags & MK_CONTROL )//平移
	{
		CSize sz(0, 50);
		if(zDelta < 0)
		{
			sz.cy = -sz.cy;
		}
		m_viewerWnd.m_drawing.Pan(sz.cx*m_viewerWnd.m_drawing.m_dCoordRatio, sz.cy*m_viewerWnd.m_drawing.m_dCoordRatio);
		OnRefresh();
		return TRUE;
	}
	else if(nFlags & MK_SHIFT)//平移
	{
		CSize sz(-50, 0);
		if(zDelta < 0)
		{
			sz.cx = -sz.cx;
		}
		m_viewerWnd.m_drawing.Pan(sz.cx*m_viewerWnd.m_drawing.m_dCoordRatio, sz.cy*m_viewerWnd.m_drawing.m_dCoordRatio);
		OnRefresh();
		return TRUE;
	}
	else//放大缩小
	{
		double dScale = 0.0;
		if(zDelta < 0)//缩小
		{
			dScale = 0.5;
		}
		else
		{
			dScale = 2.0;
		}

		CRect rc;
		GetWindowRect(&rc);
		CPoint point(pt.x-rc.left, pt.y-rc.top);

		OgdcPoint2D* pPoints = NULL;
		if(m_viewerWnd.m_drawing.DeviceToGeo(&point, 1, pPoints))
		{
			OgdcPoint2D pnt2D = pPoints[0];
			delete[] pPoints;
			pPoints = NULL;

			m_viewerWnd.m_drawing.Zoom(dScale);
			if(m_viewerWnd.m_drawing.DeviceToGeo(&point, 1, pPoints))
			{
				m_viewerWnd.m_drawing.Pan(pPoints[0].x-pnt2D.x, pPoints[0].y-pnt2D.y);
				delete[] pPoints;
				pPoints = NULL;
				OnRefresh();
				return TRUE;
			}
		}	
	}	
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

BOOL CViewerView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	switch(m_viewerWnd.GetViewAction())
	{
	case CViewerWnd::uaPan:
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_PAN));
		break;
	case CViewerWnd::uaZoomIn:
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_ZOOMIN));
		break;
	case CViewerWnd::uaZoomOut:
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_ZOOMOUT));
		break;
	case CViewerWnd::uaPointSelect:
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_SELECT));
		break;
	default:
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		break;
	}	
	return TRUE;
}

void CViewerView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();	
	m_viewerWnd.InitialUpdate(this);	
}

void CViewerView::OnPan() 
{
	m_viewerWnd.SetViewAction(CViewerWnd::uaPan);
}

void CViewerView::OnRefresh() 
{
	m_viewerWnd.Refresh();
}

void CViewerView::OnSelect() 
{
	m_viewerWnd.SetViewAction(CViewerWnd::uaPointSelect);
}

void CViewerView::OnViewEntire() 
{
	m_viewerWnd.m_drawing.ViewEntire();
	m_viewerWnd.Refresh();
}

void CViewerView::OnZoomin() 
{
	m_viewerWnd.SetViewAction(CViewerWnd::uaZoomIn);
}

void CViewerView::OnZoomout() 
{
	m_viewerWnd.SetViewAction(CViewerWnd::uaZoomOut);
}

void CViewerView::OnMapOpen() 
{
	if(m_dataSources.GetCount() == 0)
	{
		AfxMessageBox(IDS_PLEASE_OPEN_DATASOURCE);
		return;
	}
	CDatasetDlg dlg;
	dlg.m_pDataSources = &m_dataSources;
	
	if(dlg.DoModal() == IDOK && dlg.m_pDataset != NULL && dlg.m_pDataSource != NULL)
	{
		OgdcStyle style;
		style.m_clrFillFore = dlg.m_corFill;
		style.m_clrLine = dlg.m_corLine;
		style.m_nLineWidth = dlg.m_nLineWidth;
		style.m_nMarkerSize = dlg.m_nMarkerSize;
		if(dlg.m_bNULLFill)
		{
			style.m_nFillStyle = 1;
		}

		OgdcString strName = dlg.m_pDataset->GetName() + _U("@") + dlg.m_pDataSource->GetAlias();
		m_viewerWnd.AddDataset(dlg.m_pDataset, style, strName,(dlg.m_nType != 1));
		OnRefresh();
	}
}

void CViewerView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	m_viewerWnd.SetViewAction(CViewerWnd::uaPointSelect);	
	CView::OnRButtonDown(nFlags, point);
}

void CViewerView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{	
	CSize sz(0, -40);
	switch(nChar)
	{
	case VK_ESCAPE:
		m_viewerWnd.m_nSelectID = -1;
		m_viewerWnd.m_pSelectDataset = NULL;
		OnRefresh();
		break;
	case VK_HOME:
		m_viewerWnd.m_drawing.Pan(m_viewerWnd.m_drawing.m_rcViewBounds.Width()*2.0/3.0, 0.0);
		OnRefresh();
		break;
	case VK_END:
		m_viewerWnd.m_drawing.Pan(-m_viewerWnd.m_drawing.m_rcViewBounds.Width()*2.0/3.0, 0.0);
		OnRefresh();
		break;
	case VK_PRIOR:
		m_viewerWnd.m_drawing.Pan(0.0, -m_viewerWnd.m_drawing.m_rcViewBounds.Height()*2.0/3.0);
		OnRefresh();
		break;
	case VK_NEXT:
		m_viewerWnd.m_drawing.Pan(0.0, m_viewerWnd.m_drawing.m_rcViewBounds.Height()*2.0/3.0);
		OnRefresh();
		break;
	case VK_LEFT:	
		sz = CSize(40, 0);
		if(GetKeyState(VK_UP) < 0)
		{
			sz.cy = -40;
		}
		else if(GetKeyState(VK_DOWN) < 0)
		{
			sz.cy = 40;
		}
		m_viewerWnd.m_drawing.Pan(sz.cx*m_viewerWnd.m_drawing.m_dCoordRatio, sz.cy*m_viewerWnd.m_drawing.m_dCoordRatio);
		OnRefresh();
		break;
	case VK_RIGHT:
		sz = CSize(-40, 0);
		if(GetKeyState(VK_UP) < 0)
		{
			sz.cy = -40;
		}
		else if(GetKeyState(VK_DOWN) < 0)
		{
			sz.cy = 40;
		}
		m_viewerWnd.m_drawing.Pan(sz.cx*m_viewerWnd.m_drawing.m_dCoordRatio, sz.cy*m_viewerWnd.m_drawing.m_dCoordRatio);
		OnRefresh();
		break;
	case VK_UP:	
		sz = CSize(0, -40);
		if(GetKeyState(VK_LEFT) < 0)
		{
			sz.cx = 40;
		}
		else if(GetKeyState(VK_RIGHT) < 0)
		{
			sz.cx = -40;
		}
		m_viewerWnd.m_drawing.Pan(sz.cx*m_viewerWnd.m_drawing.m_dCoordRatio, sz.cy*m_viewerWnd.m_drawing.m_dCoordRatio);
		OnRefresh();
		break;
	case VK_DOWN:
		sz = CSize(0, 40);
		if(GetKeyState(VK_LEFT) < 0)
		{
			sz.cx = 40;
		}
		else if(GetKeyState(VK_RIGHT) < 0)
		{
			sz.cx = -40;
		}
		m_viewerWnd.m_drawing.Pan(sz.cx*m_viewerWnd.m_drawing.m_dCoordRatio, sz.cy*m_viewerWnd.m_drawing.m_dCoordRatio);
		OnRefresh();
		break;
	default:
		break;
	}	
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CViewerView::OnFileClose() 
{
	Close();
	OnRefresh();	
}

void CViewerView::OnObjectAtt() 
{
	if(m_viewerWnd.GetSelectID() >= 0 && m_viewerWnd.GetSelectDataset() != NULL)
	{
		OgdcDatasetVector* pDtV = m_viewerWnd.GetSelectDataset();
		OgdcQueryDef queryDef;
		queryDef.m_nType = OgdcQueryDef::IDs;
		queryDef.m_nOptions = OgdcQueryDef::Attribute;
		queryDef.m_nCursorType = OgdcQueryDef::OpenKeyset;
		queryDef.m_nCursorLocation = OgdcQueryDef::UseClient;	
		queryDef.m_IDs.Add(m_viewerWnd.GetSelectID());

		OgdcRecordset* pRecordset = pDtV->Query(queryDef);
		if(pRecordset != NULL)
		{
			CObjectAttDlg dlg;
			dlg.m_pRecordset = pRecordset;
			dlg.DoModal();
			pDtV->ReleaseRecordset(pRecordset);
			pRecordset = NULL;
		}
	}
}

void CViewerView::OnLayersManager() 
{
	CLayersManager dlg;
	dlg.m_pDataSources = &m_dataSources;
	dlg.m_datasets.Copy(m_viewerWnd.m_datasets);
	dlg.m_styles.Copy(m_viewerWnd.m_styles);
	dlg.m_names.Copy(m_viewerWnd.m_names);

	if(dlg.DoModal() == IDOK)
	{
		m_viewerWnd.m_datasets.Copy(dlg.m_datasets);
		m_viewerWnd.m_styles.Copy(dlg.m_styles);
		m_viewerWnd.m_names.Copy(dlg.m_names);
		m_viewerWnd.m_nSelectID = -1;
		m_viewerWnd.m_pSelectDataset = NULL;
		m_viewerWnd.Refresh();
	}	
}

void CViewerView::OnOpenMapChina() 
{
	// TODO: Add your command handler code here

	//引擎类型为SDBPlus类型
	OgdcInt m_nEngineType;
	m_nEngineType = 219;
	//别名
	OgdcString m_strAlias;
	m_strAlias = _U("China400");
	//////////////////////////////////////////////////////////////////////////
	//获取数据所在的绝对路径
	HMODULE module = GetModuleHandle(0);
	OgdcChar path[MAX_PATH];
	GetModuleFileName(module,path,MAX_PATH);

	OgdcString strApplicationPath = path;
	int nPos = strApplicationPath.Find(_U("Builds"));
	OgdcString strDataPath = strApplicationPath.Left(nPos) + _U("SampleData\\UDB\\China400.udb");
	//////////////////////////////////////////////////////////////////////////
			
	//数据源所在路径
	OgdcString m_strServer = strDataPath;
	
	//创建数据源指针，数据源类型为SDBPlus
	OgdcDataSource* pDS = NULL;
	pDS = OgdcProviderManager::CreateOgdcDataSource(m_nEngineType);
	
	if(pDS != NULL)
	{
		//给数据源连接信息赋值
		pDS->m_connection.m_nType = m_nEngineType;
		pDS->m_connection.m_strAlias = (OgdcString)m_strAlias;
		pDS->m_connection.m_strServer = (OgdcString)m_strServer;
		
		CWaitCursor cursor;
		if(!pDS->Open())
		{
			MessageBox(_U("China400.udb打开失败!"));
			delete pDS;
			pDS = NULL;
		}
		else 
		{			
			m_dataSources.SetAt(m_strAlias,pDS);
			BOOL bClear = true;
			BOOL bAdd = false;
			
			//设置数据集风格，包括面填充颜色、线的颜色、线宽
			OgdcString strName;
			OgdcDataset* pDataset = pDS->GetDataset(_U("Provinces_R"));
			OgdcStyle style;
			if(pDataset)
			{
				style.m_clrFillFore = OGDCRGB(228,255,211);
				style.m_clrLine = OGDCRGB(63,188,0);
				style.m_nLineWidth = 1;

				strName = pDataset->GetName() + _U("@") + pDS->GetAlias();
				m_viewerWnd.AddDataset(pDataset,style, strName,true);
				OnViewEntire();
			}
			pDataset = pDS->GetDataset(_U("MainRiver"));
			if(pDataset)
			{
				style.m_clrLine = OGDCRGB(0,170,231);
				style.m_nLineWidth = 2;

				strName = pDataset->GetName() + _U("@") + pDS->GetAlias();
				m_viewerWnd.AddDataset(pDataset,style, strName,false);
				OnViewEntire();
			}

			pDataset = pDS->GetDataset(_U("Railway_L"));
			if(pDataset)
			{
				style.m_clrLine = OGDCRGB(173,113,0);
				style.m_nLineWidth = 2;
				
				strName = pDataset->GetName() + _U("@") + pDS->GetAlias();
				m_viewerWnd.AddDataset(pDataset,style, strName,false);
				OnViewEntire();
			}

			pDataset = pDS->GetDataset(_U("Capital_P"));
			if(pDataset)
			{
				style.m_bSymbolScale = true;
				style.m_nMarkerSize = 5;
				style.m_clrLine = OGDCRGB(115,0,0);	
				
				strName = pDataset->GetName() + _U("@") + pDS->GetAlias();
				m_viewerWnd.AddDataset(pDataset,style, strName,false);
				OnViewEntire();
			}		
		}
	}	
}


void CViewerView::OnOpenMapGeologic() 
{
	//引擎类型为SDBPlus类型
	OgdcInt m_nEngineType;
	m_nEngineType = 219;
	//别名
	OgdcString m_strAlias;
	m_strAlias = _U("Geologic");

	//////////////////////////////////////////////////////////////////////////
	//获取数据所在的绝对路径
	HMODULE module = GetModuleHandle(0);
	OgdcChar path[MAX_PATH];
	GetModuleFileName(module,path,MAX_PATH);

	OgdcString strApplicationPath = path;
	int nPos = strApplicationPath.Find(_U("Builds"));
	OgdcString strDataPath = strApplicationPath.Left(nPos) + _U("SampleData\\UDB\\Geologic.udb");
	//////////////////////////////////////////////////////////////////////////
	
	//数据源所在路径
	OgdcString m_strServer;
	m_strServer = strDataPath;
	
	//创建数据源指针，数据源类型为SDBPlus
	OgdcDataSource* pDS = NULL;
	pDS = OgdcProviderManager::CreateOgdcDataSource(m_nEngineType);
	
	if(pDS != NULL)
	{
		//给数据源连接信息赋值
		pDS->m_connection.m_nType = m_nEngineType;
		pDS->m_connection.m_strAlias = (OgdcString)m_strAlias;
		pDS->m_connection.m_strServer = (OgdcString)m_strServer;
		
		CWaitCursor cursor;
		if(!pDS->Open())
		{
			MessageBox(_U("Geologic.udb打开失败!"));
			delete pDS;
			pDS = NULL;
		}
		else 
		{			
			m_dataSources.SetAt(m_strAlias,pDS);
			BOOL bClear = true;
			//设置数据集风格，包括面填充颜色、线的颜色、线宽

			for (int i = 0;i< pDS->GetDatasetCount();i++)
			{
				OgdcDataset* pDataset = pDS->GetDataset(i);
				OgdcStyle style;
				OgdcString strName;
				OgdcInt nType = pDataset->GetType();
				if(nType == OgdcDataset::Type::Line)
				{
					style.m_clrLine = OGDCRGB(0,170,231);
					style.m_nLineWidth = 2;									
				}
				else if (nType == OgdcDataset::Type::Point)
				{
					style.m_clrLine = OGDCRGB(115,233,255);					 
					style.m_nMarkerSize = 5;
				}
				strName = pDataset->GetName() + _U("@") + pDS->GetAlias();
				m_viewerWnd.AddDataset(pDataset,style, strName,bClear);
				OnViewEntire();
				bClear = false;
			}			
		}
	}	
}

void CViewerView::OnCopyDataset() 
{
	if(m_dataSources.GetCount() == 0)
	{
		AfxMessageBox(IDS_PLEASE_OPEN_DATASOURCE);
		return;
	}
	CCopyDatasetDlg dlg;
	dlg.m_pDataSources = &m_dataSources;

	if(dlg.DoModal() == IDOK && dlg.m_pDataset != NULL && dlg.m_pSrcDataSource != NULL)
	{
		OgdcDataset* pDataset =  dlg.m_pObjDataSource->CopyDataset(dlg.m_pDataset,OgdcString(dlg.m_strSrcDTName));
		if (pDataset == NULL) 
		{
			MessageBox(_U("复制数据集失败"));
		}
		else
		{
			MessageBox(_U("复制数据集成功!"));
		}
	}
	

}

void CViewerView::OnFileNewDs() 
{
	COpenDBDlg dlg;
	dlg.m_pDataSources = &m_dataSources;
	dlg.m_bOpen = false;
	if(dlg.DoModal() == IDOK)
	{
		OgdcDataSource* pDS = OgdcProviderManager::CreateOgdcDataSource(dlg.m_nEngineType);
		if(pDS != NULL)
		{
			pDS->m_connection.m_nType = dlg.m_nEngineType;
			pDS->m_connection.m_strAlias = (OgdcString)dlg.m_strAlias;
			pDS->m_connection.m_strServer = (OgdcString)dlg.m_strServer;
			pDS->m_connection.m_strDatabase = (OgdcString)dlg.m_strDatabase;
			pDS->m_connection.m_strUser = (OgdcString)dlg.m_strUser;
			pDS->m_connection.m_strPassword = (OgdcString)dlg.m_strPassWord;

			if (dlg.m_nEngineType==OGDC::oeSQLServer) {
				pDS->m_connection.m_strDriver = _U("SQL Server");
			}
							
			pDS->Create();
			CWaitCursor cursor;
			if (pDS->IsOpen()) 
			{
				m_dataSources.SetAt(pDS->m_connection.m_strAlias, pDS);	
			}
			else
			{
				delete pDS;
				pDS = NULL;		 		
			}
						
		}
		if(pDS == NULL)
		{
			AfxMessageBox(_U("新建数据源失败"));
		}	
	}
	
}
