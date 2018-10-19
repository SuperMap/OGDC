//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file ViewerWnd.cpp
//!  \brief implementation of the CViewerWnd class
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#include "Stdafx.h"
#include "ViewerWnd.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CViewerWnd::CViewerWnd()
{
	m_nViewAction = CViewerWnd::uaPan;
	m_nSelectID = -1;

	m_bRefresh = FALSE;
	m_bDowning = FALSE;
	m_pntStart = CPoint(0,0);
	m_pSelectDataset = NULL;
	m_pOldBitmap = NULL;
	m_pWnd = NULL;
}

CViewerWnd::~CViewerWnd()
{
	OnClose();
}

void CViewerWnd::InitialUpdate(CWnd *pWnd)
{
	m_pWnd = pWnd;
	if(m_pWnd && m_pWnd->m_hWnd != NULL)
	{
		CRect rcClient;
		m_pWnd->GetClientRect(&rcClient);
		m_drawing.ChangeClient(rcClient);
	}
}

void CViewerWnd::AddDataset(OgdcDataset* pDataset, OgdcStyle& style, OgdcString& strName, BOOL bClear)
{	
	if(pDataset == NULL)
	{
		return;
	}

	if(bClear)
	{
		m_nSelectID = -1;
		m_pSelectDataset = NULL;
		m_datasets.RemoveAll();
		m_styles.RemoveAll();
		m_names.RemoveAll();
	}
	else if(FindDataset(pDataset) != -1)
	{
		return;
	}

	if(pDataset->GetType() == OgdcDataset::Tabular ||
		strName.IsEmpty())
	{
		return;
	}

	m_datasets.Add(pDataset);	
	m_styles.Add(style);
	m_names.Add(strName);

	m_drawing.m_rcBounds = ComputeBounds();
    if(m_datasets.GetSize() == 1)
	{
		m_drawing.ViewEntire();
		if(!pDataset->IsRaster())
		{
			OgdcDatasetVector* pDatasetV = (OgdcDatasetVector*)pDataset;
			long nCount = pDatasetV->GetObjectCount();
			if(nCount > 1000000)		//! 百万以上
			{
				m_drawing.Zoom(100);
			}
			else if(nCount > 100000)	//! 十万以上
			{
				m_drawing.Zoom(32);
			}
			else if(nCount > 10000)		//! 一万以上
			{
				m_drawing.Zoom(8);
			}
			else if(nCount > 3000)		//! 三千以上
			{
				m_drawing.Zoom(2);
			}
		}
		else
		{
			OgdcDatasetRaster *pDatasetR = (OgdcDatasetRaster *)pDataset;
			double dWidth = m_drawing.m_rcBounds.Width();
			double dHeight = m_drawing.m_rcBounds.Height();
			LONG nWidth = pDatasetR->GetWidth();
			LONG nHeight = pDatasetR->GetHeight();

			if(nWidth!=0 && nHeight!=0)
			{
				double dResolutionX = dWidth / nWidth;
				double dResolutionY = dHeight / nHeight;
				if(dResolutionX > dResolutionY)
				{
					m_drawing.SetCoordRatio(dResolutionX);
				}
				else
				{
					m_drawing.SetCoordRatio(dResolutionY);
				}
			}
		}
	}
}

void CViewerWnd::OnClose()
{
	m_nViewAction = CViewerWnd::uaPan;
	m_nSelectID = -1;
	m_pSelectDataset = NULL;
	m_datasets.RemoveAll();
	m_styles.RemoveAll();
	m_names.RemoveAll();

	m_bDowning = FALSE;
	m_pntStart = CPoint(0,0);

	if(m_pOldBitmap != NULL)
	{
		m_memDC.SelectObject(m_pOldBitmap);
		m_pOldBitmap = NULL;
		m_bitmap.DeleteObject();
		m_memDC.DeleteDC();
	}

	m_drawing.m_dCoordRatio = 1.0;
	InitialUpdate(m_pWnd);
}

//! 设置当前窗口浏览的操作方式
void CViewerWnd::SetViewAction(LONG nAction)
{
	m_nViewAction = nAction;
}

LONG CViewerWnd::GetViewAction() const
{
	return m_nViewAction;
}

LONG CViewerWnd::GetSelectID() const
{
	return m_nSelectID;
}

//! 鼠标操作。
void CViewerWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	switch(m_nViewAction)
	{
	case CViewerWnd::uaPan:
	case CViewerWnd::uaZoomIn:
	case CViewerWnd::uaZoomOut:
		m_pWnd->SetCapture();
		m_bDowning = TRUE;
		m_pntStart = point;
		break;
	case CViewerWnd::uaPointSelect:
		{
			OgdcPoint2D *pntHitTest = NULL;
			if(m_drawing.DeviceToGeo(&point, 1, pntHitTest))
			{
				HitTest(*pntHitTest);
				delete[] pntHitTest;
				pntHitTest = NULL;
			}
			m_pWnd->Invalidate(false);
		}
		break;
	default:
		break;
	}
}

void CViewerWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	switch(m_nViewAction)
	{
	case CViewerWnd::uaPan:
		if(m_bDowning)
		{
			ReleaseCapture();
			m_bDowning = FALSE;

			if(point != m_pntStart)
			{
				CPoint pnts[2];
				pnts[0] = m_pntStart;
				pnts[1] = point;

				OgdcPoint2D* pPoints = NULL;
				if(m_drawing.DeviceToGeo(pnts, 2, pPoints))
				{
					m_drawing.Pan(pPoints[1].x-pPoints[0].x, pPoints[1].y-pPoints[0].y);
				}
				if( pPoints != NULL)
				{
					delete[] pPoints;
					pPoints = NULL;
				}
				Refresh();
			}
		}
		break;
	case CViewerWnd::uaZoomIn:
	case CViewerWnd::uaZoomOut:
		{
			ReleaseCapture();
			m_bDowning = FALSE;
			CRect rcZoom(point, m_pntStart);
			rcZoom.NormalizeRect();
			
			OgdcPoint2D *pntHitTest = NULL;
			CPoint pntTmp = rcZoom.CenterPoint();
			if(!m_drawing.DeviceToGeo(&pntTmp, 1, pntHitTest))
			{
				return;
			}

			double dRatio = 1.0;
			if(rcZoom.Width() < 4 && rcZoom.Height() < 4)
			{
				dRatio = 2.0;
			}
			else
			{
				if(rcZoom.Width() != 0)
				{
					dRatio = m_drawing.m_rcClient.Width() / (double)rcZoom.Width();
					if(rcZoom.Height() != 0 && dRatio >  (m_drawing.m_rcClient.Height() / (double)rcZoom.Height()))
					{
						dRatio = m_drawing.m_rcClient.Height() / (double)rcZoom.Height();
					}
				}
				else
				{
					dRatio = m_drawing.m_rcClient.Height() / (double)rcZoom.Height();
				}
			}

			OgdcPoint2D pntCenter = m_drawing.m_rcViewBounds.CenterPoint();
			if(m_nViewAction == CViewerWnd::uaZoomIn)
			{
				m_drawing.Zoom(dRatio);
			}
			else
			{
				m_drawing.Zoom(1/dRatio);
			}

			if(pntHitTest != NULL)
			{
				m_drawing.Pan(pntCenter.x - pntHitTest->x , pntCenter.y - pntHitTest->y);
				delete[] pntHitTest;
				pntHitTest = NULL;
			}
			Refresh();
		}
		break;
	default:
		break;
	}
}

void CViewerWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rcClient = m_drawing.m_rcClient;
	switch(m_nViewAction)
	{
	case CViewerWnd::uaPan:
		{
			if(m_bDowning)
			{
				int nOffsetX = point.x - m_pntStart.x;
				int nOffsetY = point.y - m_pntStart.y;

				CRect rcClient = m_drawing.m_rcClient;				
				if((abs(nOffsetX)>2 || abs(nOffsetY)>2) && 
					!m_bRefresh && m_pOldBitmap != NULL)
				{					
					CRect rcOffset = rcClient;
					rcOffset.OffsetRect(nOffsetX, nOffsetY);
					rcOffset.IntersectRect(rcClient, rcOffset);
					if(rcOffset.Width()==0 || rcOffset.Height()==0)
					{
						break;
					}
					CPoint pntOffset(-nOffsetX, -nOffsetY);
					pntOffset.x = max(pntOffset.x, 0);
					pntOffset.y = max(pntOffset.y, 0);				
				
					CDC* pDC = m_pWnd->GetDC();	
					CDC memDC;
					CBitmap bitmap;
					if(memDC.CreateCompatibleDC(pDC) && 
						bitmap.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height()))
					{							
						CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);						
						//! 画背景矩形
						CBrush brush(RGB(255,255,255));
						memDC.FillRect(rcClient, &brush);
					
						memDC.BitBlt(rcOffset.left, rcOffset.top, rcOffset.Width(), rcOffset.Height(),
							&m_memDC, pntOffset.x, pntOffset.y, SRCCOPY);

						pDC->BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(),
							&memDC, rcClient.left, rcClient.top, SRCCOPY);
						
						memDC.SelectObject(pOldBitmap);
					}
					m_pWnd->ReleaseDC(pDC);
					pDC = NULL;
				}
			}
		}
		break;
	case CViewerWnd::uaZoomIn:
	case CViewerWnd::uaZoomOut:
		{
			if(m_bDowning)
			{
				int nOffsetX = point.x - m_pntStart.x;
				int nOffsetY = point.y - m_pntStart.y;			
				if((nOffsetX != 0 || nOffsetY != 0) && m_pOldBitmap != NULL)
				{
					CDC* pDC = m_pWnd->GetDC();	
					pDC->BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(),
						&m_memDC, rcClient.left, rcClient.top, SRCCOPY);
								
					CPen pen;
					pen.CreatePen(PS_DOT,1,RGB(0,0,0));
					CPen *pOldPen=pDC->SelectObject(&pen);
					int nBKMode = pDC->SetBkMode(OPAQUE);

					pDC->MoveTo(m_pntStart);
					pDC->LineTo(m_pntStart.x,point.y);
					pDC->LineTo(point);
					pDC->LineTo(point.x,m_pntStart.y);
					pDC->LineTo(m_pntStart);

					pDC->SetBkMode(nBKMode);
					pDC->SelectObject(pOldPen);

					m_pWnd->ReleaseDC(pDC);
					pDC = NULL;
				}
			}
		}
		break;
	default:
		break;
	}
}

//! 窗口大小改变。
void CViewerWnd::OnSize(UINT nType, int cx, int cy)
{
	if(m_pWnd)
	{
		CRect rcClient;
		m_pWnd->GetClientRect(&rcClient);
		m_drawing.ChangeClient(rcClient);
		Refresh();
	}
}

//! 显示数据
void CViewerWnd::OnDraw(CDC* pDC, const CRect& rcClient)
{
	if((rcClient.Width()<1) || (rcClient.Height()<1))
	{
		return;
	}

	if(OGDCIS0(m_drawing.m_dCoordRatio))
	{
		return;
	}

	if(m_drawing.m_rcClient != rcClient)
	{
		m_drawing.ChangeClient(rcClient);
		m_bRefresh = TRUE;
	}

	OgdcRect2D rcBounds = ComputeBounds();
	if(m_drawing.m_rcBounds != rcBounds)
	{
		m_drawing.m_rcBounds = rcBounds;
		m_bRefresh = TRUE;
	}

	BOOL bDeviceChange = FALSE;
	LONG ncxFullScreen = GetSystemMetrics( SM_CXFULLSCREEN );
	LONG ncyFullScreen = GetSystemMetrics( SM_CYFULLSCREEN );

	if(m_bitmap.m_hObject != NULL)
	{
		BITMAP BM;
		m_bitmap.GetBitmap(&BM);

		if(BM.bmWidth != ncxFullScreen || BM.bmHeight != ncyFullScreen ||
			pDC->GetDeviceCaps(BITSPIXEL) != BM.bmBitsPixel)
		{
			bDeviceChange = TRUE;
			m_bRefresh = TRUE;
		}
	}

	if(pDC->IsPrinting() || m_bRefresh)
	{
		if(!pDC->IsPrinting() && bDeviceChange && m_pOldBitmap != NULL)
		{
			m_memDC.SelectObject(m_pOldBitmap);
			m_pOldBitmap = NULL;
			m_bitmap.DeleteObject();
			m_memDC.DeleteDC();
		}

		if(!pDC->IsPrinting() && m_bitmap.m_hObject == NULL)
		{
			m_memDC.CreateCompatibleDC(pDC);
			if(!m_bitmap.CreateCompatibleBitmap(pDC, ncxFullScreen, ncyFullScreen))
			{		
				m_bRefresh = FALSE;
				return;
			}
			m_pOldBitmap = m_memDC.SelectObject(&m_bitmap);
		}

		CDC* pDeviceDC = pDC;
		if(!pDC->IsPrinting())
		{
	 		pDeviceDC = &m_memDC;
		}

		//! 画背景矩形
		CBrush brush(RGB(255,255,255));
		pDeviceDC->FillRect(rcClient, &brush);

		//! 画数据
		for(int i=0;i<m_datasets.GetSize();i++)
		{

			OgdcStyle style = m_styles[i];
			OgdcDataset* pDT = m_datasets.GetAt(i);
			CPen pen(PS_SOLID, style.m_nLineWidth, style.m_clrLine);
			CPen* pOldPen = pDeviceDC->SelectObject(&pen);
			CBrush brush(style.m_clrFillFore);
			CBrush* pOldBrush = NULL;

			if(style.m_nFillStyle == 1)
			{
				pOldBrush = (CBrush*)pDeviceDC->SelectStockObject(NULL_BRUSH);
			}
			else
			{
				pOldBrush = pDeviceDC->SelectObject(&brush);
			}

			m_drawing.m_curStyle = style;
			m_drawing.DrawDataset(pDeviceDC, m_datasets[i]);

			pDeviceDC->SelectObject(pOldPen);
			pDeviceDC->SelectObject(pOldBrush);
		}	
		m_bRefresh = FALSE;
	}

	if(!pDC->IsPrinting() && m_bitmap.m_hObject != NULL)
	{
		pDC->BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), 
			&m_memDC, rcClient.left, rcClient.top, SRCCOPY );
	}
	
	//! 画选择
	if(m_nSelectID >= 0 && m_pSelectDataset != NULL && !pDC->IsPrinting())
	{	
		OgdcDatasetVector* pDatasetV = m_pSelectDataset;
		OgdcQueryDef queryDef;
		queryDef.m_nType = OgdcQueryDef::IDs;
		queryDef.m_nOptions = OgdcQueryDef::Geometry;
		queryDef.m_nCursorType = OgdcQueryDef::OpenStatic;
		queryDef.m_nCursorLocation = OgdcQueryDef::UseClient;	
		queryDef.m_IDs.Add(m_nSelectID);

		OgdcRecordset* pRecordset = pDatasetV->Query(queryDef);
		if(pRecordset != NULL)
		{
			pRecordset->Move(OgdcRecordset::End, 0);
			if (pDatasetV->GetType() == OgdcDataset::PointEPS ||
				pDatasetV->GetType() == OgdcDataset::LineEPS ||
				pDatasetV->GetType() == OgdcDataset::RegionEPS ||
				pDatasetV->GetType() == OgdcDataset::TextEPS)
			{
				UGC::UGGeometry* pGeometry = NULL;
				while(!pRecordset->IsBOF())
				{
					if(pRecordset->GetElement(pGeometry) && pGeometry != NULL)
					{
						CPen pen(PS_SOLID, 2, RGB(0,0,255));
						CPen* pOldPen = pDC->SelectObject(&pen);

						CBrush brush(RGB(255,255,115));
						CBrush *pOldBrush = pDC->SelectObject(&brush);

						m_drawing.DrawElement(pDC, pGeometry, true);

						pDC->SelectObject(pOldPen);
						pDC->SelectObject(pOldBrush);
						delete pGeometry;
						pGeometry = NULL;
						break;
					}
					pRecordset->Move(OgdcRecordset::Current, -1);

					if(pGeometry != NULL)
					{
						delete pGeometry;
						pGeometry = NULL;
					}
				}
			}
			else
			{
				OgdcElement* pElement = NULL;
				while(!pRecordset->IsBOF())
				{
					if(pRecordset->GetElement(pElement) && pElement != NULL)
					{
						CPen pen(PS_SOLID, 2, RGB(0,0,255));
						CPen* pOldPen = pDC->SelectObject(&pen);

						CBrush brush(RGB(255,255,115));
						CBrush *pOldBrush = pDC->SelectObject(&brush);

						m_drawing.DrawElement(pDC, pElement, true);

						pDC->SelectObject(pOldPen);
						pDC->SelectObject(pOldBrush);
						delete pElement;
						pElement = NULL;
						break;
					}
					pRecordset->Move(OgdcRecordset::Current, -1);

					if(pElement != NULL)
					{
						delete pElement;
						pElement = NULL;
					}
				}
			}

			pDatasetV->ReleaseRecordset(pRecordset);
			pRecordset = NULL;
		}
	}
}

void CViewerWnd::Refresh()
{
	m_bRefresh = TRUE;
	if(m_pWnd != NULL)
	{
		m_pWnd->Invalidate(false);
	}
}

//! 得到选中对象ID。选不中返回-1。
LONG CViewerWnd::HitTest(const OgdcPoint2D& pntHitTest)	
{
	m_nSelectID = -1;
	m_pSelectDataset = NULL;
	for(int i=m_datasets.GetSize()-1;i>=0;i--)
	{
		OgdcDataset* pDataset = m_datasets[i];
		if(pDataset != NULL && !pDataset->IsRaster())
		{
			m_nSelectID = HitTest(pntHitTest, (OgdcDatasetVector*)pDataset);
			if(m_nSelectID != -1)
			{
				m_pSelectDataset = (OgdcDatasetVector*)pDataset;
				break;
			}
		}
	}
	return m_nSelectID;
}

LONG CViewerWnd::HitTest(const OgdcPoint2D& pntHitTest, OgdcDatasetVector* pDatasetV)
{
	if(!pDatasetV->IsOpen())
	{
		pDatasetV->Open();
	}

	double dTolerance = 3.0*m_drawing.m_dCoordRatio;

	OgdcQueryDef queryDef;
	queryDef.m_nType = OgdcQueryDef::Bounds;
	queryDef.m_nOptions = OgdcQueryDef::Geometry;
	queryDef.m_nCursorType = OgdcQueryDef::OpenStatic;
	queryDef.m_nCursorLocation = OgdcQueryDef::UseClient;
	queryDef.m_rc2Bounds = OgdcRect2D(pntHitTest.x-dTolerance,	
									pntHitTest.y+dTolerance,
									pntHitTest.x+dTolerance,
									pntHitTest.y-dTolerance);

	LONG nSelectID = -1;
	OgdcRecordset* pRecordset = pDatasetV->Query(queryDef);
	if(pRecordset != NULL)
	{
		pRecordset->Move(OgdcRecordset::End, 0);
		if (pDatasetV->GetType() == OgdcDataset::PointEPS ||
			pDatasetV->GetType() == OgdcDataset::LineEPS ||
			pDatasetV->GetType() == OgdcDataset::RegionEPS ||
			pDatasetV->GetType() == OgdcDataset::TextEPS)
		{
			UGC::UGGeometry* pGeometry = NULL;
			while(!pRecordset->IsBOF())
			{
				if(pRecordset->GetElement(pGeometry) && pGeometry != NULL)
				{
					if (pGeometry->HitTest(pntHitTest, dTolerance))
					{
						nSelectID = pGeometry->GetID();
					}

					if(nSelectID >= 0)
					{
						delete pGeometry;
						pGeometry = NULL;
						break;
					}
				}
				pRecordset->Move(OgdcRecordset::Current, -1);

				if(pGeometry != NULL)
				{
					delete pGeometry;
					pGeometry = NULL;
				}
			}
		}
		else
		{
			OgdcElement* pElement = NULL;
			while(!pRecordset->IsBOF())
			{
				if(pRecordset->GetElement(pElement) && pElement != NULL)
				{
					if(pElement->GetType() == OgdcElement::ElemLine)
					{
						if(HitTest(pntHitTest, (OgdcElemLine*)pElement, dTolerance))
						{
							nSelectID = pElement->m_nID;
						}
					}
					else if(pElement->GetType() == OgdcElement::ElemRegion)
					{
						if(HitTest(pntHitTest, (OgdcElemRegion*)pElement, dTolerance))
						{
							nSelectID = pElement->m_nID;
						}
					}
					else if (pElement->GetType() == OgdcElement::ElemLine3D)
					{
						if(HitTest(pntHitTest, (OgdcElemLine3D*)pElement, dTolerance))
						{
							nSelectID = pElement->m_nID;
						}
					}
					else if (pElement->GetType() == OgdcElement::ElemRegion3D)
					{
						if(HitTest(pntHitTest, (OgdcElemRegion3D*)pElement, dTolerance))
						{
							nSelectID = pElement->m_nID;
						}
					}
					else
					{
						nSelectID = pElement->m_nID;
					}

					if(nSelectID >= 0)
					{
						delete pElement;
						pElement = NULL;
						break;
					}
				}
				pRecordset->Move(OgdcRecordset::Current, -1);

				if(pElement != NULL)
				{
					delete pElement;
					pElement = NULL;
				}
			}
		}

		pDatasetV->ReleaseRecordset(pRecordset);
		pRecordset = NULL;
	}
	return nSelectID;
}

OgdcDatasetVector* CViewerWnd::GetSelectDataset() const
{
	return m_pSelectDataset;
}

LONG CViewerWnd::FindDataset(OgdcDataset* pDataset)
{
	for(int i=0;i<m_datasets.GetSize();i++)
	{
		if(pDataset == m_datasets[i])
		{
			return i;
		}
	}
	return -1;
}

OgdcRect2D CViewerWnd::ComputeBounds()
{
	OgdcRect2D rc;
	BOOL bInit = FALSE;
	for(int i=0;i<m_datasets.GetSize();i++)
	{
		OgdcDataset* pDataset = m_datasets[i];
		if(pDataset != NULL)
		{
			if(!bInit)
			{
				if(!pDataset->IsRaster())
				{
					if(((OgdcDatasetVector*)pDataset)->GetObjectCount() > 0)
					{
						rc = pDataset->GetBounds();
						bInit = TRUE;
					}
				}
				else
				{
					rc = pDataset->GetBounds();
					bInit = TRUE;
				}
			}
			else
			{			
				rc.Union(pDataset->GetBounds());
			}
		}
	}

	if(rc.IsEmpty())
	{
		rc = m_drawing.m_rcBounds;
	}
	return rc;
}

BOOL CViewerWnd::HitTest(const OgdcPoint2D& pntHitTest, OgdcElemRegion* pElemRegion, double dTolerance)	
{
	OGDCASSERT(pElemRegion != NULL);
	OgdcRect2D rcBounds = pElemRegion->GetBounds();
	rcBounds.Inflate(dTolerance, dTolerance);
	if (rcBounds.PtInRect(pntHitTest))
	{
		OgdcInt *plPolyCounts = pElemRegion->m_polyCounts.GetData();
		OgdcPoint2D *pPoints = pElemRegion->m_points.GetData();
		OgdcInt lCount = pElemRegion->m_polyCounts.GetSize();
		if(plPolyCounts == NULL || pPoints == NULL || lCount == 0)
		{
			return FALSE;
		}
	
		OgdcDouble dInter=0;
		OgdcInt nCount=0,counter = 0;		
		OgdcPoint2D* p1 = NULL;
		OgdcPoint2D* p2 = NULL;		
		for (OgdcInt k=0; k < lCount; k++)
		{
			nCount = plPolyCounts[k];
			///////////////////////////////////////////////////
			p1 = pPoints;
			for (OgdcInt i=1;i <= nCount; i++) 
			{
				p2 = pPoints+(i%nCount);
				if (OGDCIS0(pntHitTest.y - OGDCMIN(p1->y,p2->y)))
				{
					if (OGDCIS0(p1->y - p2->y))
					{
						if (( (pntHitTest.x < OGDCMAX(p1->x,p2->x) ) || OGDCEQUAL(pntHitTest.x,OGDCMAX(p1->x,p2->x))) 
							&& ( (pntHitTest.x > OGDCMIN(p1->x,p2->x) ) || OGDCEQUAL(pntHitTest.x,OGDCMIN(p1->x,p2->x))))
						{
							return TRUE;
						}
					}
					else
					{
						if ((pntHitTest == (*p1)) || (pntHitTest == (*p2)))
						{
							return TRUE;
						}
					}
				}			
				else if (pntHitTest.y > OGDCMIN(p1->y,p2->y)) 
				{
					//if (pntHitTest.y <= OGDCMAX(p1->y,p2->y)) 
					if ( (pntHitTest.y < OGDCMAX(p1->y,p2->y)) || OGDCEQUAL(pntHitTest.y , OGDCMAX(p1->y,p2->y))  )
					{        
						//if (pntHitTest.x <= OGDCMAX(p1->x,p2->x)) 
						if (  (pntHitTest.x < OGDCMAX(p1->x,p2->x)) || OGDCEQUAL(pntHitTest.x , OGDCMAX(p1->x,p2->x)) ) 
						{
							if (!OGDCIS0(p1->y - p2->y))
							{
								if (OGDCIS0(p1->x - p2->x))
								{
									if (OGDCIS0(pntHitTest.x - p1->x))
									{
										return TRUE;
									}
									counter++;
								}
								else
								{
									dInter = (pntHitTest.y-p1->y)*(p2->x-p1->x)/(p2->y-p1->y)+(p1->x-pntHitTest.x);
									dInter /= fabs((p2->x-p1->x));
									if (fabs(dInter) < 1.0e-8)
									{
										return TRUE;
									}
									if (dInter >  EP)
									{
										counter++;
									}
								}
							}
						}      
					}    
				}				
				p1 = p2;  
			}  
			///////////////////////////////////////////////////
			pPoints += nCount;
		}
		if (counter % 2 == 0)
		{
			return FALSE;  
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CViewerWnd::HitTest(const OgdcPoint2D& pntHitTest, OgdcElemLine* pElemLine,  double dTolerance)	
{
	OGDCASSERT(pElemLine != NULL);
	OgdcRect2D rcBounds = pElemLine->GetBounds();
	rcBounds.Inflate(dTolerance, dTolerance);
	if (rcBounds.PtInRect(pntHitTest))
	{
		OgdcInt *plPolyCounts = pElemLine->m_polyCounts.GetData();
		OgdcPoint2D *pPoints = pElemLine->m_points.GetData();
		OgdcInt nSubCount = pElemLine->m_polyCounts.GetSize();

		if(plPolyCounts == NULL || pPoints == NULL || nSubCount == 0)
		{
			return FALSE;
		}

		for(OgdcInt k=0; k<nSubCount; k++)
		{
			OgdcInt lCount = plPolyCounts[k];
			if(pPoints==NULL || lCount<=1)
			{
				continue;
			}
			
			OgdcInt i = 0,j = 0;
			OgdcDouble dDist = 0.0;	// 给定点到折线上的点或线段的距离
			OgdcDouble dA = 0.0,dB = 0.0;
			OgdcDouble dC = 0.0;	// 直线方程Ax+By+C=0的3参数
			OgdcDouble dToleranceSquare = dTolerance*dTolerance;
			OgdcDouble  dpntx = 0.0;
			OgdcDouble  dpnty = 0.0;
			
			dpntx=pntHitTest.x;
			dpnty=pntHitTest.y;
			
			dDist=(pPoints[lCount-1].x - dpntx) * (pPoints[lCount-1].x - dpntx)
				+(pPoints[lCount-1].y - dpnty) * (pPoints[lCount-1].y - dpnty);
			if(dDist<dToleranceSquare)
			{
				return TRUE;
			}
			
			for(j=0;j<5;j++)
			{
				i=j;
				while(i<lCount-1)
				{
					if((pPoints[i].x>dpntx-dTolerance)&&(pPoints[i].x<dpntx+dTolerance))
						// 利用x坐标排除一部分点;
					{
						dDist=(pPoints[i].x - dpntx) * (pPoints[i].x - dpntx)
							+(pPoints[i].y - dpnty) * (pPoints[i].y - dpnty);
						if(dDist<dToleranceSquare)
						{
							return TRUE;
						}// 如果有一个点满足条件,则返回真;
					}

					// 判断点到直线的垂线是否在线段上,跳过垂线不在线段上的点
					if(IsProjectOnLineSegment(pntHitTest,pPoints[i],pPoints[i+1]))
					{
						dA=pPoints[i].y - pPoints[i+1].y;
						dB=pPoints[i+1].x - pPoints[i].x;
						if(OGDCIS0(dA) && OGDCIS0(dB))
						{
							dDist = (pntHitTest.x-pPoints[i].x) * (pntHitTest.x-pPoints[i].x)
								+ (pntHitTest.y-pPoints[i].y) * (pntHitTest.y-pPoints[i].y);
						}
						else 
						{
							dC=-((dA * pPoints[i].x) + (dB * pPoints[i].y));
							dDist=(dA * pntHitTest.x) + (dB * pntHitTest.y )+ dC;
							dDist=dDist * dDist;
							dDist=dDist/((dA*dA) +(dB*dB));	
						}
						if(dDist<dToleranceSquare)
						{
							return TRUE;
						}
					}
					i=i+5;
				}// endwhile;
			}
			
			pPoints += *plPolyCounts;
			plPolyCounts++;
		}
	}
	return FALSE;
}

BOOL CViewerWnd::HitTest(const OgdcPoint2D& pntHitTest, OgdcElemLine3D* pElemLine3D, double dTolerance)
{
	OGDCASSERT(pElemLine3D != NULL);
	OgdcRect2D rcBounds = pElemLine3D->GetBounds();
	rcBounds.Inflate(dTolerance, dTolerance);
	if (rcBounds.PtInRect(pntHitTest))
	{
		OgdcInt *plPolyCounts = pElemLine3D->m_polyCounts.GetData();
		OgdcPoint3D *pPoints = pElemLine3D->m_points.GetData();
		OgdcInt nSubCount = pElemLine3D->m_polyCounts.GetSize();

		if(plPolyCounts == NULL || pPoints == NULL || nSubCount == 0)
		{
			return FALSE;
		}

		for(OgdcInt k=0; k<nSubCount; k++)
		{
			OgdcInt lCount = plPolyCounts[k];
			if(pPoints==NULL || lCount<=1)
			{
				continue;
			}

			OgdcInt i = 0,j = 0;
			OgdcDouble dDist = 0.0;	// 给定点到折线上的点或线段的距离
			OgdcDouble dA = 0.0,dB = 0.0;
			OgdcDouble dC = 0.0;	// 直线方程Ax+By+C=0的3参数
			OgdcDouble dToleranceSquare = dTolerance*dTolerance;
			OgdcDouble  dpntx = 0.0;
			OgdcDouble  dpnty = 0.0;

			dpntx=pntHitTest.x;
			dpnty=pntHitTest.y;

			dDist=(pPoints[lCount-1].x - dpntx) * (pPoints[lCount-1].x - dpntx)
				+(pPoints[lCount-1].y - dpnty) * (pPoints[lCount-1].y - dpnty);
			if(dDist<dToleranceSquare)
			{
				return TRUE;
			}

			for(j=0;j<5;j++)
			{
				i=j;
				while(i<lCount-1)
				{
					if((pPoints[i].x>dpntx-dTolerance)&&(pPoints[i].x<dpntx+dTolerance))
						// 利用x坐标排除一部分点;
					{
						dDist=(pPoints[i].x - dpntx) * (pPoints[i].x - dpntx)
							+(pPoints[i].y - dpnty) * (pPoints[i].y - dpnty);
						if(dDist<dToleranceSquare)
						{
							return TRUE;
						}// 如果有一个点满足条件,则返回真;
					}

					// 判断点到直线的垂线是否在线段上,跳过垂线不在线段上的点
					OgdcPoint2D startPoint;
					OgdcPoint2D endPoint;
					startPoint.x = pPoints[i].x;
					startPoint.y = pPoints[i].y;
					endPoint.x = pPoints[i+1].x;
					endPoint.y = pPoints[i+1].y;
					if(IsProjectOnLineSegment(pntHitTest,startPoint,endPoint))
					{
						dA=pPoints[i].y - pPoints[i+1].y;
						dB=pPoints[i+1].x - pPoints[i].x;
						if(OGDCIS0(dA) && OGDCIS0(dB))
						{
							dDist = (pntHitTest.x-pPoints[i].x) * (pntHitTest.x-pPoints[i].x)
								+ (pntHitTest.y-pPoints[i].y) * (pntHitTest.y-pPoints[i].y);
						}
						else 
						{
							dC=-((dA * pPoints[i].x) + (dB * pPoints[i].y));
							dDist=(dA * pntHitTest.x) + (dB * pntHitTest.y )+ dC;
							dDist=dDist * dDist;
							dDist=dDist/((dA*dA) +(dB*dB));	
						}
						if(dDist<dToleranceSquare)
						{
							return TRUE;
						}
					}
					i=i+5;
				}// endwhile;
			}

			pPoints += *plPolyCounts;
			plPolyCounts++;
		}
	}
	return FALSE;
}

BOOL CViewerWnd::HitTest(const OgdcPoint2D& pntHitTest, OgdcElemRegion3D* pElemRegion3D, double dTolerance)
{
	OGDCASSERT(pElemRegion3D != NULL);
	OgdcRect2D rcBounds = pElemRegion3D->GetBounds();
	rcBounds.Inflate(dTolerance, dTolerance);
	if (rcBounds.PtInRect(pntHitTest))
	{
		OgdcInt *plPolyCounts = pElemRegion3D->m_polyCounts.GetData();
		OgdcPoint3D *pPoints = pElemRegion3D->m_points.GetData();
		OgdcInt lCount = pElemRegion3D->m_polyCounts.GetSize();
		if(plPolyCounts == NULL || pPoints == NULL || lCount == 0)
		{
			return FALSE;
		}

		OgdcDouble dInter=0;
		OgdcInt nCount=0,counter = 0;		
		OgdcPoint3D* p1 = NULL;
		OgdcPoint3D* p2 = NULL;
		OgdcPoint2D startPoint;
		OgdcPoint2D endPoint;
		for (OgdcInt k=0; k < lCount; k++)
		{
			nCount = plPolyCounts[k];
			///////////////////////////////////////////////////
			p1 = pPoints;
			for (OgdcInt i=1;i <= nCount; i++) 
			{
				p2 = pPoints+(i%nCount);
				if (OGDCIS0(pntHitTest.y - OGDCMIN(p1->y,p2->y)))
				{
					if (OGDCIS0(p1->y - p2->y))
					{
						if (( (pntHitTest.x < OGDCMAX(p1->x,p2->x) ) || OGDCEQUAL(pntHitTest.x,OGDCMAX(p1->x,p2->x))) 
							&& ( (pntHitTest.x > OGDCMIN(p1->x,p2->x) ) || OGDCEQUAL(pntHitTest.x,OGDCMIN(p1->x,p2->x))))
						{
							return TRUE;
						}
					}
					else
					{
						startPoint.x = p1->x;
						startPoint.y = p1->y;
						endPoint.x = p2->x;
						endPoint.y = p2->y;
						if ((pntHitTest == startPoint) || (pntHitTest == endPoint))
						{
							return TRUE;
						}
					}
				}			
				else if (pntHitTest.y > OGDCMIN(p1->y,p2->y)) 
				{
					//if (pntHitTest.y <= OGDCMAX(p1->y,p2->y)) 
					if ( (pntHitTest.y < OGDCMAX(p1->y,p2->y)) || OGDCEQUAL(pntHitTest.y , OGDCMAX(p1->y,p2->y))  )
					{        
						//if (pntHitTest.x <= OGDCMAX(p1->x,p2->x)) 
						if (  (pntHitTest.x < OGDCMAX(p1->x,p2->x)) || OGDCEQUAL(pntHitTest.x , OGDCMAX(p1->x,p2->x)) ) 
						{
							if (!OGDCIS0(p1->y - p2->y))
							{
								if (OGDCIS0(p1->x - p2->x))
								{
									if (OGDCIS0(pntHitTest.x - p1->x))
									{
										return TRUE;
									}
									counter++;
								}
								else
								{
									dInter = (pntHitTest.y-p1->y)*(p2->x-p1->x)/(p2->y-p1->y)+(p1->x-pntHitTest.x);
									dInter /= fabs((p2->x-p1->x));
									if (fabs(dInter) < 1.0e-8)
									{
										return TRUE;
									}
									if (dInter >  EP)
									{
										counter++;
									}
								}
							}
						}      
					}    
				}				
				p1 = p2;  
			}  
			///////////////////////////////////////////////////
			pPoints += nCount;
		}
		if (counter % 2 == 0)
		{
			return FALSE;  
		}
		return TRUE;
	}
	return FALSE;
}

//! \brief 判断点在线段的垂足是否在线段上
BOOL CViewerWnd::IsProjectOnLineSegment(const OgdcPoint2D& pntHitTest, 
					const OgdcPoint2D& pntLinsectStart, const OgdcPoint2D& pntLinsectEnd)
{
	if(pntLinsectStart==pntLinsectEnd) return TRUE;

	double a2 = (pntHitTest.x - pntLinsectStart.x) * (pntHitTest.x - pntLinsectStart.x) 
		         + (pntHitTest.y - pntLinsectStart.y) * (pntHitTest.y - pntLinsectStart.y);
	double b2 = (pntHitTest.x - pntLinsectEnd.x) * (pntHitTest.x - pntLinsectEnd.x) 
		         + (pntHitTest.y - pntLinsectEnd.y) * (pntHitTest.y - pntLinsectEnd.y);
	double c2 = (pntLinsectEnd.x - pntLinsectStart.x) * (pntLinsectEnd.x - pntLinsectStart.x) 
		         + (pntLinsectEnd.y - pntLinsectStart.y) * (pntLinsectEnd.y - pntLinsectStart.y);

	if ((b2+c2<a2) || (a2+c2<b2))
	{
		return FALSE;
	}
	return TRUE;
}