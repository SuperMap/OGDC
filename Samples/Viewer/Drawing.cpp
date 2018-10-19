//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file Drawing.cpp
//!  \brief implementation of the CDrawing class.
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "Drawing.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawing::CDrawing()
{
	m_dCoordRatio = 1.0;
	m_curStyle.m_nMarkerSize = 10;
}

CDrawing::~CDrawing()
{

}

BOOL CDrawing::DrawDataset(CDC* pDC, OgdcDataset* pDataset)
{
	if(OGDCIS0(m_dCoordRatio) || m_rcClient.IsRectEmpty() ||
		pDataset == NULL)
	{
		return FALSE;
	}

	if(!pDataset->IsOpen())
	{
		pDataset->Open();
	}

	if(!pDataset->IsOpen())
	{
		return FALSE;
	}
	
	OgdcRect2D rcDraw = pDataset->GetBounds();
	if(!m_rcViewBounds.IsIntersect(rcDraw))
	{
		return FALSE;
	}

	if(pDataset->IsRaster())
	{
		return DrawRaster(pDC, (OgdcDatasetRaster*)pDataset);
	}
	return DrawVector(pDC,(OgdcDatasetVector*)pDataset);
}

//! 显示几何对象。
BOOL CDrawing::DrawElement(CDC* pDC, OgdcElement* pElement, BOOL bSelected)
{
	OGDCASSERT(!OGDCIS0(m_dCoordRatio) && !m_rcClient.IsRectEmpty() && pElement != NULL);	
	switch(pElement->GetType())
	{	
	case OgdcElement::ElemPoint:
		{
			return DrawElement(pDC,(OgdcElemPoint*)pElement, bSelected);
		}
	case OgdcElement::ElemMultiPoint:
		{
			return DrawElement(pDC,(OgdcElemMultiPoint*)pElement, bSelected);
		}
	case OgdcElement::ElemLine:
		{
			return DrawElement(pDC,(OgdcElemLine*)pElement, bSelected);
		}
	case OgdcElement::ElemRegion:
		{
			return DrawElement(pDC,(OgdcElemRegion*)pElement, bSelected);
		}
	case OgdcElement::ElemText:
		{
			return DrawElement(pDC,(OgdcElemText*)pElement, bSelected);
		}
	case OgdcElement::ElemPoint3D:
		{
			return DrawElement(pDC,(OgdcElemPoint3D*)pElement, bSelected);
		}
	case OgdcElement::ElemLine3D:
		{
			return DrawElement(pDC,(OgdcElemLine3D*)pElement, bSelected);
		}
	case OgdcElement::ElemRegion3D:
		{
			return DrawElement(pDC,(OgdcElemRegion3D*)pElement, bSelected);
		}
	default:
		{
			break;
		}
	}
	return FALSE;
}

BOOL CDrawing::DrawElement(CDC* pDC, UGC::UGGeometry* pGeometry, BOOL bSelected)
{
	OGDCASSERT(!OGDCIS0(m_dCoordRatio) && !m_rcClient.IsRectEmpty() && pGeometry != NULL);
	switch(pGeometry->GetType())
	{
	case UGC::UGGeometry::GeoPointEPS:
		{
			return DrawElement(pDC,(UGC::UGGeoPointEPS*)pGeometry, bSelected);
		}
	case UGC::UGGeometry::GeoLineEPS:
		{
			return DrawElement(pDC,(UGC::UGGeoLineEPS*)pGeometry, bSelected);
		}
	case UGC::UGGeometry::GeoRegionEPS:
		{
			return DrawElement(pDC,(UGC::UGGeoRegionEPS*)pGeometry, bSelected);
		}
	case UGC::UGGeometry::GeoTextEPS:
		{
			return DrawElement(pDC,(UGC::UGGeoTextEPS*)pGeometry, bSelected);
		}
	default:
		{
			break;
		}
	}
	return FALSE;
}

//! 全幅。
void CDrawing::ViewEntire()
{
	if(!m_rcBounds.IsEmpty() && !m_rcClient.IsRectEmpty() &&
		m_rcViewBounds != m_rcBounds)
	{
		m_rcViewBounds = m_rcBounds;
		m_dCoordRatio = m_rcBounds.Width()/m_rcClient.Width();
		if(m_dCoordRatio < m_rcBounds.Height()/m_rcClient.Height())
		{
			m_dCoordRatio = m_rcBounds.Height()/m_rcClient.Height();
		}
		SetCoordRatio(m_dCoordRatio);
	}
}

//! 平移。
void CDrawing::Pan(double dOffsetX, double dOffsetY)
{
	OGDCASSERT(!OGDCIS0(m_dCoordRatio) && !m_rcClient.IsRectEmpty());
	m_rcViewBounds.Offset(-dOffsetX, -dOffsetY);
}

//! 缩放。
void CDrawing::Zoom(double dRatio)
{
	OGDCASSERT(!OGDCIS0(m_dCoordRatio) && !m_rcClient.IsRectEmpty());

	if(dRatio < 0 || OGDCIS0(dRatio) || OGDCIS0(dRatio-1.0))
	{
		return;
	}

	if (m_dCoordRatio > 5e-10) 
	{
		m_dCoordRatio /= dRatio;
	}
	
	SetCoordRatio(m_dCoordRatio);
}

void CDrawing::SetCoordRatio(double dCoordRatio)
{
	OGDCASSERT(!OGDCIS0(m_dCoordRatio) && !m_rcClient.IsRectEmpty());

	if(dCoordRatio < 0 || OGDCIS0(dCoordRatio))
	{
		return;
	}

	m_dCoordRatio = dCoordRatio;

	OgdcPoint2D pntCenter = m_rcViewBounds.CenterPoint();
	OgdcDouble dHMargin = m_rcClient.Width()*m_dCoordRatio*0.5;
	OgdcDouble dVMargin = m_rcClient.Height()*m_dCoordRatio*0.5;

	m_rcViewBounds.left = pntCenter.x - dHMargin;
	m_rcViewBounds.right = pntCenter.x + dHMargin;
	m_rcViewBounds.top = pntCenter.y + dVMargin;
	m_rcViewBounds.bottom = pntCenter.y - dVMargin;
}

//! 改变客户区大小。
void CDrawing::ChangeClient(const CRect& rcClient)
{
	if(m_rcClient == rcClient)
	{
		return;
	}

	m_rcClient = rcClient;
	if(OGDCIS0(m_dCoordRatio) || m_rcClient.IsRectEmpty() )
	{
		return;
	}
	SetCoordRatio(m_dCoordRatio);
}

//! 显示数据集。
BOOL CDrawing::DrawVector(CDC* pDC, OgdcDatasetVector* pDatasetV)
{
	OGDCASSERT(!OGDCIS0(m_dCoordRatio) && !m_rcClient.IsRectEmpty());
	
	OgdcQueryDef queryDef;
	queryDef.m_nType = OgdcQueryDef::Bounds;
	queryDef.m_nOptions = OgdcQueryDef::Geometry;
	queryDef.m_nCursorType = OgdcQueryDef::OpenStatic;
	queryDef.m_nCursorLocation = OgdcQueryDef::UseClient;			
	queryDef.m_rc2Bounds = m_rcViewBounds;

	OgdcRect2D rcDtBounds = pDatasetV->GetBounds();	
	double dSearchArea = m_rcViewBounds.Width() * m_rcViewBounds.Height();
	double dDatasetArea = rcDtBounds.Width() * rcDtBounds.Height();
	if(dSearchArea >= dDatasetArea*0.6)
	{
		queryDef.m_rc2Bounds = rcDtBounds;
	}

	OgdcRecordset* pRecordset = pDatasetV->Query(queryDef);
	if(pRecordset != NULL)
	{
		pRecordset->Move(OgdcRecordset::Begin, 0);
		if (pDatasetV->GetType() == OgdcDataset::PointEPS ||
			pDatasetV->GetType() == OgdcDataset::LineEPS ||
			pDatasetV->GetType() == OgdcDataset::RegionEPS ||
			pDatasetV->GetType() == OgdcDataset::TextEPS)
		{
			UGC::UGGeometry* pGeometry = NULL;
			while (!pRecordset->IsEOF())
			{
				if (pRecordset->GetElement(pGeometry) && pGeometry != NULL)
				{
					DrawElement(pDC, pGeometry);
				}
				else//断网情况下避免长时间等待 http://192.168.115.2:8090/browse/UGC-4322
				{
					break;
				}

				pRecordset->Move();

				if (pGeometry != NULL)
				{
					delete pGeometry;
					pGeometry = NULL;
				}
			}
		}
		else
		{
			OgdcElement* pElement = NULL;
			while(!pRecordset->IsEOF())
			{
				if(pRecordset->GetElement(pElement) && pElement != NULL)
				{
					DrawElement(pDC, pElement);
				}
				else//断网情况下避免长时间等待 http://192.168.115.2:8090/browse/UGC-4322
				{
					break;
				}

				pRecordset->Move();

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
	return TRUE;
}

BOOL CDrawing::DrawRaster(CDC* pDC, OgdcDatasetRaster* pDatasetR)
{
	OGDCASSERT(!OGDCIS0(m_dCoordRatio) && !m_rcClient.IsRectEmpty());

	OgdcRect2D rcDraw = pDatasetR->GetBounds();
	if(!rcDraw.IntersectRect(m_rcViewBounds, rcDraw))
	{
		return FALSE;
	}
			
	BYTE *lp = new BYTE[sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*256];
	if(lp == NULL)
	{
		return FALSE;
	}

	int i = 0;
	BITMAPINFO *pBitMapInfo = (BITMAPINFO*)lp;
	PixelFormat ePixelFormat = pDatasetR->GetPixelFormat();
	pBitMapInfo->bmiHeader.biBitCount = BitCount(ePixelFormat);

	if(pDatasetR->GetBandCount() <= 1)
	{
		if(pDatasetR->GetType() == OgdcDataset::Image && ePixelFormat <= IPF_BYTE)
		{
			OgdcColorset colorset = pDatasetR->m_info.m_arrBandInfo[0].m_colorset;
			if(colorset.GetSize() == 0)
			{
				if(ePixelFormat == IPF_MONO)
				{				
					pBitMapInfo->bmiColors[0].rgbBlue = 0;
					pBitMapInfo->bmiColors[0].rgbGreen = 0;
					pBitMapInfo->bmiColors[0].rgbRed =  0;
					pBitMapInfo->bmiColors[0].rgbReserved = 0;

					pBitMapInfo->bmiColors[1].rgbBlue = 255;
					pBitMapInfo->bmiColors[1].rgbGreen = 255;
					pBitMapInfo->bmiColors[1].rgbRed =  255;
					pBitMapInfo->bmiColors[1].rgbReserved = 0;
				}
				else if(ePixelFormat == IPF_FBIT)
				{		
					for(i = 0; i < 16; i++)
					{
						pBitMapInfo->bmiColors[i].rgbBlue = (16*i);
						pBitMapInfo->bmiColors[i].rgbGreen = (16*i);
						pBitMapInfo->bmiColors[i].rgbRed =  (16*i);
						pBitMapInfo->bmiColors[i].rgbReserved = 0;
					}		
				}
				else
				{					
					for(i = 0; i < 256; i++)
					{
						pBitMapInfo->bmiColors[i].rgbBlue = i;
						pBitMapInfo->bmiColors[i].rgbGreen = i;
						pBitMapInfo->bmiColors[i].rgbRed =  i;
						pBitMapInfo->bmiColors[i].rgbReserved = 0;
					}
				}
			}
			else
			{
				for(i=0; i<256 && i<colorset.GetSize(); i++)
				{
					//! 因为读进来的r和b是反的，所以要再反一次。
					OgdcColor color = colorset.GetAt(i);	
					pBitMapInfo->bmiColors[i].rgbRed = GetBValue(color);	
					pBitMapInfo->bmiColors[i].rgbGreen = GetGValue(color);	
					pBitMapInfo->bmiColors[i].rgbBlue =  GetRValue(color);
					pBitMapInfo->bmiColors[i].rgbReserved = 0;
				}
			}
		}
		else if(pDatasetR->GetType() == OgdcDataset::Grid)
		{	
			pBitMapInfo->bmiHeader.biBitCount = 24;
			if(pDatasetR->m_colorTable.m_Colorset.GetSize() <= 0)
			{
				pDatasetR->m_colorTable.CreateDefault();
			}		
		}
	}
	else
	{
		OgdcArray<OgdcInt> aryBand;
		aryBand.Add(0);
		aryBand.Add(1);
		aryBand.Add(pDatasetR->GetBandCount() > 2 ? 2:1);

		ImgColorSpace nColorSpace = ICS_RGB;
// 		pDatasetR->SetBandCombineMode(aryBand, nColorSpace);
		pBitMapInfo->bmiHeader.biBitCount = 24;
	}


	pBitMapInfo->bmiHeader.biPlanes = 1;	
	pBitMapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pBitMapInfo->bmiHeader.biCompression = BI_RGB;
	pBitMapInfo->bmiHeader.biSizeImage = 0L;
	pBitMapInfo->bmiHeader.biXPelsPerMeter = 0L;
	pBitMapInfo->bmiHeader.biYPelsPerMeter = 0L;
	pBitMapInfo->bmiHeader.biClrUsed = 0L;
	pBitMapInfo->bmiHeader.biClrImportant = 0L;
	
	CRect rcBlock;
	CRgn *pClipRgn = NULL;
	OgdcElemRegion *pDTRasterClipRegion = pDatasetR->m_pClipRgn;
	if(pDTRasterClipRegion != NULL && !pDTRasterClipRegion->m_rcBounds.IsEmpty())
	{	
		OgdcPoint2D *pPoint2Ds = pDTRasterClipRegion->m_points.GetData();
		CPoint *pClipPoints = NULL;
		if(GeoToDevice(pPoint2Ds, pDTRasterClipRegion->m_points.GetSize(), pClipPoints))
		{
			int nSubCount = pDTRasterClipRegion->m_polyCounts.GetSize();
			int *lpPolyCount = pDTRasterClipRegion->m_polyCounts.GetData();
		
			pClipRgn = new CRgn;
			if(pClipRgn != NULL)
			{
				pClipRgn->CreatePolyPolygonRgn(pClipPoints, lpPolyCount, nSubCount, ALTERNATE);
				pDC->SelectClipRgn(pClipRgn, RGN_AND);
			}
			delete[] pClipPoints;
			pClipPoints = NULL;
		}			
	}		
	
	OgdcSize szBMP(OGDCROUND(rcDraw.Width()/m_dCoordRatio), OGDCROUND(rcDraw.Height()/m_dCoordRatio));	
	OgdcRasterBlock* pImgBlock = GetViewBlock(pDatasetR, rcDraw, szBMP.cx, szBMP.cy);
	if(pImgBlock != NULL)
	{
		DrawRasterBlock(pDC, pImgBlock, pDatasetR, pBitMapInfo);
	
		delete pImgBlock;
		pImgBlock = NULL;
	}
	pDatasetR->ReleaseAllBlocks();

	if(lp != NULL)
	{
		delete[] lp;
		lp = NULL;
	}

	if(pClipRgn != NULL)
	{
		pDC->SelectClipRgn(NULL, RGN_COPY);
		delete pClipRgn;
		pClipRgn = NULL;
	}	
	return TRUE;
}

void CDrawing::XYToImg(OgdcDatasetRaster*pDatasetRaster, const OgdcRect2D& rectXY,OgdcRect& rectImg)
{
	OgdcRect2D rcViewGeoRef = pDatasetRaster->GetBounds();
	OgdcSize2D imgRatio;
	//分辨率
	//水平方向地理坐标与影像映射比例.
	imgRatio.cx = (rcViewGeoRef.Width()) / (OgdcDouble)pDatasetRaster->GetWidth();	
	//垂直方向地理坐标与影像映射比例.
	imgRatio.cy = (rcViewGeoRef.Height()) / (OgdcDouble)pDatasetRaster->GetHeight(); 

	//地理坐标转化成像素坐标
	rectImg.left =	(OgdcInt)floor(( rectXY.left - rcViewGeoRef.left ) 
		/ imgRatio.cx);
	rectImg.right = (OgdcInt)ceil(( rectXY.right - rcViewGeoRef.left ) 
		/ imgRatio.cx);
	//翻转垂直朝向.
	rectImg.top	= (OgdcInt)floor(pDatasetRaster->GetHeight() 
		- (rectXY.top - rcViewGeoRef.bottom) / imgRatio.cy);	
	//翻转垂直朝向.
	rectImg.bottom = (OgdcInt)ceil(pDatasetRaster->GetHeight() 
		- (rectXY.bottom - rcViewGeoRef.bottom) / imgRatio.cy);	
}

void CDrawing::ImgToXY(OgdcDatasetRaster*pDatasetRaster, const OgdcRect& rectImg,OgdcRect2D& rectXY)
{
	OgdcRect2D rcViewGeoRef = pDatasetRaster->GetBounds();
	OgdcSize2D imgRatio;
	//分辨率
	//水平方向地理坐标与影像映射比例.
	imgRatio.cx = (rcViewGeoRef.Width()) / (OgdcDouble)pDatasetRaster->GetWidth();	
	//垂直方向地理坐标与影像映射比例.
	imgRatio.cy = (rcViewGeoRef.Height()) / (OgdcDouble)pDatasetRaster->GetHeight(); 

	//像素坐标转化成地理坐标	
	rectXY.left = (rectImg.left * imgRatio.cx + rcViewGeoRef.left);
	rectXY.right = (rectImg.right * imgRatio.cx + rcViewGeoRef.left) ;
	//翻转垂直朝向.
	rectXY.top =  ((pDatasetRaster->GetHeight() - rectImg.top) * imgRatio.cy 
		+ rcViewGeoRef.bottom);
	//翻转垂直朝向.
	rectXY.bottom = ((pDatasetRaster->GetHeight() - rectImg.bottom) * imgRatio.cy 
		+ rcViewGeoRef.bottom);	
}
OgdcRasterBlock* CDrawing::GetViewBlock(OgdcDatasetRaster*pDatasetRaster, OgdcRect2D rcView, int nDeviceWidth, int nDeviceHeight)
{
	OgdcRect2D rcBounds = rcView;
	rcBounds.Normalize();
	rcBounds.IntersectRect(rcBounds, pDatasetRaster->GetBounds());
	
	nDeviceWidth = OGDCROUND(rcBounds.Width()/rcView.Width()*nDeviceWidth);
	nDeviceHeight = OGDCROUND(rcBounds.Height()/rcView.Height()*nDeviceHeight);

	OgdcRect rcImage;
	//地理坐标转化成像素坐标
	XYToImg(pDatasetRaster, rcBounds, rcImage);
	
	if (rcImage.Width() < 2 && rcImage.Height() < 2)
	{
		return NULL;
	}

	OgdcDatasetRaster* pDtR = pDatasetRaster->GetPyramidTier(rcImage.Width(), nDeviceWidth);

	if (pDtR != NULL && pDtR != pDatasetRaster)
	{
		if (!pDtR->IsOpen())
		{
			pDtR->Open();
		}
		XYToImg(pDtR, rcBounds, rcImage);
		if (rcImage.Width() < 2 && rcImage.Height() < 2)
		{
			return NULL;
		}
		nDeviceWidth = rcImage.Width();
		nDeviceHeight = rcImage.Height();
		ImgToXY(pDtR, rcImage, rcBounds);
	}
	else
	{
		pDtR = pDatasetRaster;
		if (nDeviceWidth > rcImage.Width() || nDeviceHeight > rcImage.Height())
		{
			nDeviceWidth = rcImage.Width();
			nDeviceHeight = rcImage.Height();
			ImgToXY(pDtR,rcImage, rcBounds);
		}
		if (nDeviceWidth < 2 && nDeviceHeight < 2)
		{
			return NULL;
		}
	}	

	OgdcRasterBlock* pResBlock = new OgdcRasterBlock;
	pResBlock->m_nWidth = nDeviceWidth;
	pResBlock->m_nHeight = nDeviceHeight;
	pResBlock->m_nValidWidth = pResBlock->m_nWidth;
	pResBlock->m_nValidHeight = pResBlock->m_nHeight;
	pResBlock->m_nPixelFormat = pDatasetRaster->GetPixelFormat();
	pResBlock->m_rcBounds = rcBounds;
	pResBlock->m_nWidthBytes = ((pResBlock->m_nWidth 
		* OgdcInt(pDatasetRaster->GetPixelFormat())) + 31) / 32 * 4 ;
	
	if (pDatasetRaster->GetType() == OgdcDataset::Grid)
	{
		pResBlock->m_nWidthBytes = ((pResBlock->m_nWidth * 32) + 31) / 32 * 4 ;
		pResBlock->InitGrid((OgdcInt)pDatasetRaster->GetNoValue());
	}
	else
	{
		pResBlock->Init();
	}

	OgdcInt nBlockRowStart =  rcImage.top / pDatasetRaster->m_info.m_nBlockSize;
	OgdcInt nBlockRowEnd =  (rcImage.bottom - 1) / pDatasetRaster->m_info.m_nBlockSize;
	OgdcInt nBlockColtart = rcImage.left / pDatasetRaster->m_info.m_nBlockSize;
	OgdcInt nBlockColEnd = (rcImage.right - 1) / pDatasetRaster->m_info.m_nBlockSize;

	OgdcDouble dMaxZ = 0;
	OgdcDouble dMinZ = 0;
	
	OgdcRasterBlock* pSrcBlock = NULL;
	for (OgdcInt i = nBlockRowStart; i <= nBlockRowEnd; i++)
	{
		for (OgdcInt j = nBlockColtart; j <= nBlockColEnd; j++)
		{
			OgdcInt nIndex = i * pDtR->m_info.m_nColBlocks + j;
			pSrcBlock = pDtR->LoadBlock(i, j);

			if (pSrcBlock != NULL)
			{
				//Load一个栅格块来填充可视栅格块
				/////////////////////////////////////////////////

				if (NULL == pResBlock || pSrcBlock == NULL)
				{
					return FALSE;
				}
				
				PixelFormat srcIPF = pResBlock->m_nPixelFormat;
				PixelFormat destIPF = pSrcBlock->m_nPixelFormat;
				
				if (srcIPF != destIPF)
				{
					return FALSE;
				}
				
				OgdcRect2D rcDestBounds = pResBlock->m_rcBounds;
				OgdcRect2D rcSrcBounds = pSrcBlock->m_rcBounds;
				
				if (!rcSrcBounds.IsIntersect(rcDestBounds))
				{
					return FALSE; 
				}
				
				OgdcDouble dXDestRatio = rcDestBounds.Width() 
					/ pResBlock->m_nValidWidth;
				OgdcDouble dYDestRatio = rcDestBounds.Height() 
					/ pResBlock->m_nValidHeight;
				
				OgdcDouble dXSrcRatio = rcSrcBounds.Width() 
					/ pSrcBlock->m_nValidWidth;
				OgdcDouble dYSrcRatio = rcSrcBounds.Height() 
					/ pSrcBlock->m_nValidHeight;
				
				OgdcRect2D rcIntersectBounds = rcSrcBounds;
				rcIntersectBounds.IntersectRect(rcIntersectBounds, rcDestBounds);
				
				// 算出相交区域在当前数据集的像素的左上角坐标和右下角坐标
				OgdcInt nResLeft = (OgdcInt)floor(
					(rcIntersectBounds.left - rcDestBounds.left) / dXDestRatio);
				OgdcInt nResTop = (OgdcInt)floor(
					(rcDestBounds.top - rcIntersectBounds.top) / dYDestRatio);
				OgdcInt nResRight = (OgdcInt)ceil(
					(rcIntersectBounds.right - rcDestBounds.left) / dXDestRatio);
				OgdcInt nResBottom = (OgdcInt)ceil(
					(rcDestBounds.top - rcIntersectBounds.bottom) / dYDestRatio);
				nResRight = OGDCMIN(nResRight, pResBlock->m_nValidWidth - 1);
				nResBottom = OGDCMIN(nResBottom, pResBlock->m_nValidHeight - 1);
				
				OgdcInt nSrcX = 0;
				OgdcInt nSrcY = 0;
				OgdcBool bRes = FALSE;
		
				for (OgdcInt row = nResTop; row <= nResBottom; row++)
				{
					nSrcY = (OgdcInt)floor((rcSrcBounds.top 
						- (rcDestBounds.top - (row+0.5)*dYDestRatio)) / dYSrcRatio);
					if(nSrcY < 0 || nSrcY >= pSrcBlock->m_nValidHeight)
					{
						continue;
					}
					for (OgdcInt col = nResLeft; col <= nResRight; col++)
					{
						nSrcX = (OgdcInt)floor(((rcDestBounds.left
							+ (col+0.5) * dXDestRatio) - rcSrcBounds.left) / dXSrcRatio);
						if (nSrcX < 0 || nSrcX >= pSrcBlock->m_nValidWidth)
						{
							continue;
						}
						pResBlock->SetValue(col, row, pSrcBlock->GetValue(nSrcX, nSrcY));
						bRes = TRUE;
					}
				}
				/////////////////////////////////////////////////
			}
		}
	}
	
	if (pDtR != pDatasetRaster)
	{
		pDtR->Close();
	}
	return pResBlock;
}
BOOL CDrawing::DrawRasterBlock(CDC* pDC, OgdcRasterBlock* pIBlock, OgdcDatasetRaster* pDatasetR, BITMAPINFO* pBitMapInfo)
{
	OGDCASSERT(pIBlock != NULL);
	if(pIBlock->m_nValidWidth < 2 || pIBlock->m_nValidHeight < 2)
	{
		return FALSE; 
	}

	pBitMapInfo->bmiHeader.biWidth = pIBlock->m_nWidth;
	pBitMapInfo->bmiHeader.biHeight = pIBlock->m_nValidHeight;

	OgdcRect2D rcBlockBounds = pIBlock->m_rcBounds;
	OgdcPoint2D pnts[2];
	pnts[0] = rcBlockBounds.TopLeft();
	pnts[1] = rcBlockBounds.BottomRight();

	CPoint* pPoints = NULL;
	GeoToDevice(pnts, 2, pPoints);	
	CRect rcBounds(pPoints[0], pPoints[1]);
	delete[] pPoints;
	pPoints = NULL;
	rcBounds.NormalizeRect();

	OgdcRasterBlock* pDemBlock = NULL;	
	BYTE* pByte = NULL;
	// DEM或者GRID
	if(pDatasetR->GetType() == OgdcDataset::Grid
		&& pDatasetR->GetPixelFormat() != IPF_MONO)
	{		
		pDemBlock = new OgdcRasterBlock;

		// 象素信息 ImgBlockInfo
		pDemBlock->m_nPixelFormat = IPF_RGB;			
		pDemBlock->m_nWidth = pIBlock->m_nWidth;				// 宽度.
		pDemBlock->m_nHeight = pIBlock->m_nHeight;				// 高度.
		pDemBlock->m_nWidthBytes = ((3*pIBlock->m_nWidth+3)/4)*4;
		pDemBlock->m_nValidWidth = pIBlock->m_nValidWidth;		// 有效宽度	
		pDemBlock->m_nValidHeight = pIBlock->m_nValidHeight;	// 有效高度			
		pDemBlock->m_rcBounds = pIBlock->m_rcBounds;			// 地理坐标参照.		
		pDemBlock->m_bInited = pIBlock->m_bInited;				// 是否正在初始化,如是,将不装入数据.

		OgdcInt nSize = pDemBlock->m_nWidthBytes * pDemBlock->m_nHeight;
		pDemBlock->SetByteSize(nSize);
		
		COLORREF clrPixel = 0;
		OgdcDouble dZ = 0.0;
		OgdcInt nValue = 0;
		OgdcInt i = 0;
		OgdcInt y = 0;
		OgdcDouble dNoValue = OGDCROUND(pDatasetR->m_info.m_arrBandInfo[0].m_dNoValue);
		OgdcColorTable *pColorTable = &pDatasetR->m_colorTable;

		for(y = 0; y < pDemBlock->m_nValidHeight; y++)
		{
			OgdcInt x = 0;					
			for(x = 0; x < pDemBlock->m_nValidWidth; x++)
			{	
				dZ = pIBlock->GetValue(x,y);
				if(OGDCIS0(dZ - dNoValue))
				{
					clrPixel = RGB(255,255,255);
				}
				else
				{
					clrPixel = pColorTable->GetColor(dZ);
				}
				pDemBlock->SetValue(x,y,clrPixel);
			}
		}		
		pBitMapInfo->bmiHeader.biBitCount = IPF_RGB;
		pByte = (BYTE*)pDemBlock->GetBlockData();
	}
	else // 普通的Image
	{
		pByte = (BYTE*)pIBlock->GetBlockData();
	}
	
	int  nOldBltMode = pDC->SetStretchBltMode(COLORONCOLOR);	
	BOOL bResult = StretchDIBits(pDC->m_hDC, rcBounds.left, rcBounds.bottom, rcBounds.Width(), -rcBounds.Height(),
						0, 0, pIBlock->m_nValidWidth, pIBlock->m_nValidHeight, pByte, pBitMapInfo, DIB_RGB_COLORS, SRCCOPY);	
	
	pDC->SetStretchBltMode(nOldBltMode);
	
	if(pDemBlock != NULL)
	{
		delete pDemBlock;
		pDemBlock = NULL;
	}
	pByte = NULL;
	return bResult;
}

//! 显示几何对象。
BOOL CDrawing::DrawElement(CDC* pDC, OgdcElemPoint* pElemPoint, BOOL bSelected)
{
	OGDCASSERT(!OGDCIS0(m_dCoordRatio) && !m_rcClient.IsRectEmpty());

	OgdcPoint2D* pPoint2Ds = &pElemPoint->m_point;
	CPoint* pPoints = NULL;
	if(!GeoToDevice(pPoint2Ds, 1, pPoints))
	{
		return FALSE;
	}

	CPen* pPen = pDC->GetCurrentPen();
	COLORREF color = RGB(0,0,0);
	if(pPen != NULL)
	{		
		LOGPEN logPen;
		pPen->GetLogPen(&logPen);
		color = logPen.lopnColor;			
	}

	CBrush brush(color);
	CBrush* pOldBrush = pDC->SelectObject(&brush);

	int nRadius = m_curStyle.m_nMarkerSize / 2;
	pDC->Ellipse(CRect(pPoints[0].x-nRadius, pPoints[0].y-nRadius, pPoints[0].x+nRadius, pPoints[0].y+nRadius));

	pDC->SelectObject(pOldBrush);

	delete[] pPoints;
	pPoints = NULL;
	return TRUE;
}

BOOL CDrawing::DrawElement(CDC* pDC, OgdcElemMultiPoint* pElemMultiPoint, BOOL bSelected)
{
	OGDCASSERT(!OGDCIS0(m_dCoordRatio) && !m_rcClient.IsRectEmpty());

	OgdcPoint2D* pPoint2Ds = pElemMultiPoint->m_points.GetData();
	CPoint* pPoints = NULL;
	if(!GeoToDevice(pPoint2Ds, pElemMultiPoint->m_points.GetSize(), pPoints))
	{
		return FALSE;
	}

	CPen* pPen = pDC->GetCurrentPen();
	COLORREF color = RGB(0,0,0);
	if(pPen != NULL)
	{		
		LOGPEN logPen;
		pPen->GetLogPen(&logPen);
		color = logPen.lopnColor;			
	}

	CBrush brush(color);
	CBrush* pOldBrush = pDC->SelectObject(&brush);

	for(int i=0;i<pElemMultiPoint->m_points.GetSize();i++)
	{
		pDC->Ellipse(CRect(pPoints[i].x-3, pPoints[i].y-3, pPoints[i].x+3, pPoints[i].y+3));
	}

	pDC->SelectObject(pOldBrush);

	delete[] pPoints;
	pPoints = NULL;
	return TRUE;
}

BOOL CDrawing::DrawElement(CDC* pDC, OgdcElemLine* pElemLine, BOOL bSelected)
{
	OGDCASSERT(!OGDCIS0(m_dCoordRatio) && !m_rcClient.IsRectEmpty());

	OgdcPoint2D* pPoint2Ds = pElemLine->m_points.GetData();
	CPoint* pPoints = NULL;
	if(!GeoToDevice(pPoint2Ds, pElemLine->m_points.GetSize(), pPoints))
	{
		return FALSE;
	}

	pDC->PolyPolyline(pPoints, (unsigned long *)pElemLine->m_polyCounts.GetData(), pElemLine->m_polyCounts.GetSize());

	delete[] pPoints;
	pPoints = NULL;

	return TRUE;
}

BOOL CDrawing::DrawElement(CDC* pDC, OgdcElemRegion* pElemRegion, BOOL bSelected)
{
	OGDCASSERT(!OGDCIS0(m_dCoordRatio) && !m_rcClient.IsRectEmpty());

	OgdcPoint2D* pPoint2Ds = pElemRegion->m_points.GetData();
	CPoint* pPoints = NULL;
	if(!GeoToDevice(pPoint2Ds, pElemRegion->m_points.GetSize(), pPoints))
	{
		return FALSE;
	}

	pDC->PolyPolygon(pPoints, pElemRegion->m_polyCounts.GetData(), pElemRegion->m_polyCounts.GetSize());

	delete[] pPoints;
	pPoints = NULL;

	return TRUE;
}

BOOL CDrawing::DrawElement(CDC* pDC, OgdcElemText* pElemText, BOOL bSelected)
{
	OGDCASSERT(!OGDCIS0(m_dCoordRatio) && !m_rcClient.IsRectEmpty());

	long nFontHeight = 10, nFontWidth = 5;
	OgdcTextStyle textStyle = pElemText->m_textStyle;
	if(textStyle.m_bFixedSize)
	{
		nFontHeight = OGDCROUND(textStyle.m_dHeight*3);
		if(nFontHeight == 0)
		{
			return TRUE;
		}
		nFontWidth =  OGDCROUND(textStyle.m_dWidth*3);
	}
	else
	{
		nFontHeight = OGDCROUND(textStyle.m_dHeight/m_dCoordRatio);
		if(nFontHeight == 0)
		{
			return TRUE;
		}
		nFontWidth =  OGDCROUND(textStyle.m_dWidth/m_dCoordRatio);
	}

	int nOldBkMode = 0;
	if( textStyle.m_bBackOpaque )
	{
		nOldBkMode = pDC->SetBkMode(OPAQUE);
	}
	else
	{
		nOldBkMode = pDC->SetBkMode(TRANSPARENT);
	}

	CString strFontName = textStyle.m_strFaceName;
	strFontName.TrimLeft();
	strFontName.TrimRight(); 

	LOGFONT stFont;
	memset(&stFont,0,sizeof(LOGFONT));
	stFont.lfWeight=FW_NORMAL;	
	stFont.lfItalic = textStyle.m_bItalic;

	stFont.lfEscapement = OGDCROUND(textStyle.m_dAngle*10);
	
	stFont.lfOutPrecision = OUT_TT_PRECIS;
	stFont.lfClipPrecision = CLIP_LH_ANGLES;
	stFont.lfQuality = DRAFT_QUALITY;// PROOF_QUALITY;
	stFont.lfPitchAndFamily = TMPF_TRUETYPE|FF_SCRIPT;

	if(textStyle.m_bBold)
	{
		stFont.lfWeight = 700;//设置为粗体
	}
	else
	{
		stFont.lfWeight = 0;//400
	}	

	if(textStyle.m_bUnderline)
		stFont.lfUnderline = TRUE;//设置为下画线
	else
		stFont.lfUnderline = FALSE;

	if(textStyle.m_bStrikeOut)
		stFont.lfStrikeOut = TRUE;//设置为删除线
	else
		stFont.lfStrikeOut = FALSE;

	stFont.lfHeight = nFontHeight;
	stFont.lfWidth = nFontWidth;
	strcpy((char*)stFont.lfFaceName,(LPSTR)(LPCTSTR)strFontName);
	
	COLORREF color = textStyle.m_clrFore;
	if(bSelected)
	{
		CPen* pPen = pDC->GetCurrentPen();
		if(pPen != NULL)
		{		
			LOGPEN logPen;
			pPen->GetLogPen(&logPen);
			color = logPen.lopnColor;			
		}
	}

	COLORREF corForeOld = pDC->SetTextColor(color);
	COLORREF corBackOld = pDC->SetBkColor(textStyle.m_clrBack);

	for(int i=0;i<pElemText->m_strTexts.GetSize();i++)
	{
		stFont.lfEscapement = OGDCROUND(pElemText->m_angles[i]*10);

		CString strText = pElemText->m_strTexts[i];
		OgdcPoint2D pnt2D = pElemText->m_points[i];		
		CPoint pntAnchor;
		CPoint* pPoints = NULL;
		GeoToDevice(&pnt2D, 1, pPoints);
		pntAnchor = pPoints[0];
		delete[] pPoints;
		pPoints = NULL;
			
		CFont fontCurrent;
		fontCurrent.CreateFontIndirect(&stFont);
		CFont *pOldFont = pDC->SelectObject(&fontCurrent);

		pDC->TextOut(pntAnchor.x, pntAnchor.y, strText);

		pDC->SelectObject(pOldFont);
	}

	pDC->SetTextColor(corForeOld);
	pDC->SetBkColor(corBackOld);
	return TRUE;
}

BOOL CDrawing::DrawElement(CDC* pDC, OgdcElemPoint3D* pElemPoint3D, BOOL bSelected)
{
	OGDCASSERT(!OGDCIS0(m_dCoordRatio) && !m_rcClient.IsRectEmpty());

	OgdcPoint3D* pPoint3Ds = &pElemPoint3D->m_point;
	OgdcPoint2D pnt;
	pnt.x = pPoint3Ds->x;
	pnt.y = pPoint3Ds->y;
	CPoint* pPoints = NULL;
	if(!GeoToDevice(&pnt, 1, pPoints))
	{
		return FALSE;
	}

	CPen* pPen = pDC->GetCurrentPen();
	COLORREF color = RGB(0,0,0);
	if(pPen != NULL)
	{		
		LOGPEN logPen;
		pPen->GetLogPen(&logPen);
		color = logPen.lopnColor;			
	}

	CBrush brush(color);
	CBrush* pOldBrush = pDC->SelectObject(&brush);

	int nRadius = m_curStyle.m_nMarkerSize / 2;
	pDC->Ellipse(CRect(pPoints[0].x-nRadius, pPoints[0].y-nRadius, pPoints[0].x+nRadius, pPoints[0].y+nRadius));

	pDC->SelectObject(pOldBrush);

	delete[] pPoints;
	pPoints = NULL;
	return TRUE;
}

BOOL CDrawing::DrawElement(CDC* pDC, OgdcElemLine3D* pElemLine3D, BOOL bSelected)
{
	OGDCASSERT(!OGDCIS0(m_dCoordRatio) && !m_rcClient.IsRectEmpty());

	OgdcPoint3D* pPoint3Ds = pElemLine3D->m_points.GetData();
	CPoint* pPoints = NULL;
	if(!GeoToDevice(pPoint3Ds, pElemLine3D->m_points.GetSize(), pPoints))
	{
		return FALSE;
	}

	pDC->PolyPolyline(pPoints, (unsigned long *)pElemLine3D->m_polyCounts.GetData(), pElemLine3D->m_polyCounts.GetSize());

	delete[] pPoints;
	pPoints = NULL;

	return TRUE;
}
BOOL CDrawing::DrawElement(CDC* pDC, OgdcElemRegion3D* pElemRegion3D, BOOL bSelected)
{
	OGDCASSERT(!OGDCIS0(m_dCoordRatio) && !m_rcClient.IsRectEmpty());

	OgdcPoint3D* pPoint3Ds = pElemRegion3D->m_points.GetData();
	CPoint* pPoints = NULL;
	if(!GeoToDevice(pPoint3Ds, pElemRegion3D->m_points.GetSize(), pPoints))
	{
		return FALSE;
	}

	pDC->PolyPolygon(pPoints, pElemRegion3D->m_polyCounts.GetData(), pElemRegion3D->m_polyCounts.GetSize());

	delete[] pPoints;
	pPoints = NULL;

	return TRUE;
}

BOOL CDrawing::DrawElement(CDC* pDC, UGC::UGGeoPointEPS* pGeoPointEPS, BOOL bSelected)
{
	OGDCASSERT(!OGDCIS0(m_dCoordRatio) && !m_rcClient.IsRectEmpty());

	OgdcPoint2D pnt;
	pnt.x = pGeoPointEPS->GetX();
	pnt.y = pGeoPointEPS->GetY();
	CPoint* pPoints = NULL;
	if(!GeoToDevice(&pnt, 1, pPoints))
	{
		return FALSE;
	}

	CPen* pPen = pDC->GetCurrentPen();
	COLORREF color = RGB(0,0,0);
	if(pPen != NULL)
	{
		LOGPEN logPen;
		pPen->GetLogPen(&logPen);
		color = logPen.lopnColor;
	}

	CBrush brush(color);
	CBrush* pOldBrush = pDC->SelectObject(&brush);

	int nRadius = m_curStyle.m_nMarkerSize / 2;
	pDC->Ellipse(CRect(pPoints[0].x-nRadius, pPoints[0].y-nRadius, pPoints[0].x+nRadius, pPoints[0].y+nRadius));

	pDC->SelectObject(pOldBrush);

	delete[] pPoints;
	pPoints = NULL;
	return TRUE;
}

BOOL CDrawing::DrawElement(CDC* pDC, UGC::UGGeoLineEPS* pGeoLineEPS, BOOL bSelected)
{
	OGDCASSERT(!OGDCIS0(m_dCoordRatio) && !m_rcClient.IsRectEmpty());

	const UGC::UGPointEPS* pPointEPS = pGeoLineEPS->GetPoints();
	OgdcInt nEPSPointCount = pGeoLineEPS->GetPointCount();
	OgdcPoint2D* pPoint2Ds = new OgdcPoint2D[nEPSPointCount];
	for (OgdcInt i=0; i<nEPSPointCount; i++)
	{
		pPoint2Ds[i].x = pPointEPS[i].x;
		pPoint2Ds[i].y = pPointEPS[i].y;
	}

	CPoint* pPoints = NULL;
	if(!GeoToDevice(pPoint2Ds, nEPSPointCount, pPoints))
	{
		return FALSE;
	}
	delete[] pPoint2Ds;
	pPoint2Ds = NULL;

	pDC->PolyPolyline(pPoints, (unsigned long *)&nEPSPointCount, 1);

	delete[] pPoints;
	pPoints = NULL;

	return TRUE;
}

BOOL CDrawing::DrawElement(CDC* pDC, UGC::UGGeoRegionEPS* pGeoRegionEPS, BOOL bSelected)
{
	OGDCASSERT(!OGDCIS0(m_dCoordRatio) && !m_rcClient.IsRectEmpty());

	const UGC::UGPointEPS* pPointEPS = pGeoRegionEPS->GetPoints();
	OgdcInt nEPSPointCount = pGeoRegionEPS->GetPointCount();
	OgdcPoint2D* pPoint2Ds = new OgdcPoint2D[nEPSPointCount];
	for (OgdcInt i=0; i<nEPSPointCount; i++)
	{
		pPoint2Ds[i].x = pPointEPS[i].x;
		pPoint2Ds[i].y = pPointEPS[i].y;
	}

	CPoint* pPoints = NULL;
	if(!GeoToDevice(pPoint2Ds, nEPSPointCount, pPoints))
	{
		return FALSE;
	}
	delete[] pPoint2Ds;
	pPoint2Ds = NULL;

	pDC->PolyPolygon(pPoints, &nEPSPointCount, 1);

	delete[] pPoints;
	pPoints = NULL;

	return TRUE;
}

BOOL CDrawing::DrawElement(CDC* pDC, UGC::UGGeoTextEPS* pGeoTextEPS, BOOL bSelected)
{
	OGDCASSERT(!OGDCIS0(m_dCoordRatio) && !m_rcClient.IsRectEmpty());

	long nFontHeight = 10, nFontWidth = 5;
	UGC::UGTextStyle textStyle = pGeoTextEPS->GetTextStyle();
	if(textStyle.IsFixedSize())
	{
		nFontHeight = OGDCROUND(textStyle.GetHeight()*3);
		if(nFontHeight == 0)
		{
			return TRUE;
		}
		nFontWidth =  OGDCROUND(textStyle.GetWidth()*3);
	}
	else
	{
		nFontHeight = OGDCROUND(textStyle.GetHeight()/m_dCoordRatio);
		if(nFontHeight == 0)
		{
			return TRUE;
		}
		nFontWidth =  OGDCROUND(textStyle.GetWidth()/m_dCoordRatio);
	}

	int nOldBkMode = 0;
	if(textStyle.IsBackOpaque())
	{
		nOldBkMode = pDC->SetBkMode(OPAQUE);
	}
	else
	{
		nOldBkMode = pDC->SetBkMode(TRANSPARENT);
	}

	CString strFontName = textStyle.GetFaceName();
	strFontName.TrimLeft();
	strFontName.TrimRight();

	LOGFONT stFont;
	memset(&stFont,0,sizeof(LOGFONT));
	stFont.lfWeight=FW_NORMAL;	
	stFont.lfItalic = textStyle.IsItalic();

	stFont.lfOutPrecision = OUT_TT_PRECIS;
	stFont.lfClipPrecision = CLIP_LH_ANGLES;
	stFont.lfQuality = DRAFT_QUALITY;// PROOF_QUALITY;
	stFont.lfPitchAndFamily = TMPF_TRUETYPE|FF_SCRIPT;

	if(textStyle.IsBold())
	{
		stFont.lfWeight = 700;//设置为粗体
	}
	else
	{
		stFont.lfWeight = 0;//400
	}	

	if(textStyle.IsUnderline())
		stFont.lfUnderline = TRUE;//设置为下画线
	else
		stFont.lfUnderline = FALSE;

	if(textStyle.IsStrikeOut())
		stFont.lfStrikeOut = TRUE;//设置为删除线
	else
		stFont.lfStrikeOut = FALSE;

	stFont.lfHeight = nFontHeight;
	stFont.lfWidth = nFontWidth;
	strcpy((char*)stFont.lfFaceName,(LPSTR)(LPCTSTR)strFontName);

	COLORREF color = textStyle.GetForeColor();
	if(bSelected)
	{
		CPen* pPen = pDC->GetCurrentPen();
		if(pPen != NULL)
		{		
			LOGPEN logPen;
			pPen->GetLogPen(&logPen);
			color = logPen.lopnColor;			
		}
	}

	COLORREF corForeOld = pDC->SetTextColor(color);
	COLORREF corBackOld = pDC->SetBkColor(textStyle.GetBackColor());

	stFont.lfEscapement = OGDCROUND(textStyle.GetAngle()*10);
	CString strText = pGeoTextEPS->GetStringText();
	OgdcPoint2D pnt2D = pGeoTextEPS->GetInnerPoint();
	CPoint pntAnchor;
	CPoint* pPoints = NULL;
	GeoToDevice(&pnt2D, 1, pPoints);
	pntAnchor = pPoints[0];
	delete[] pPoints;
	pPoints = NULL;
	CFont fontCurrent;
	fontCurrent.CreateFontIndirect(&stFont);
	CFont *pOldFont = pDC->SelectObject(&fontCurrent);
	pDC->TextOut(pntAnchor.x, pntAnchor.y, strText);
	pDC->SelectObject(pOldFont);

	pDC->SetTextColor(corForeOld);
	pDC->SetBkColor(corBackOld);
	return TRUE;
}


//! 地理坐标到设备坐标转换。
BOOL CDrawing::GeoToDevice(OgdcPoint2D* pPoint2Ds, int nCount, CPoint*& pPoints)
{
	OGDCASSERT(!OGDCIS0(m_dCoordRatio) && !m_rcClient.IsRectEmpty() && 
		!m_rcViewBounds.IsEmpty());

	if(nCount <=0 || pPoint2Ds == NULL)
	{
		return FALSE;
	}

	if(pPoints != NULL)
	{
		delete[] pPoints;
		pPoints = NULL;
	}

	pPoints = new CPoint[nCount];
	for(int i=0;i<nCount;i++)
	{
		pPoints[i].x = OGDCROUND((pPoint2Ds[i].x - m_rcViewBounds.left)/m_dCoordRatio);
		pPoints[i].y = OGDCROUND((m_rcViewBounds.top - pPoint2Ds[i].y)/m_dCoordRatio);
	}
	return TRUE;
}

BOOL CDrawing::GeoToDevice(OgdcPoint3D* pPoint3Ds, int nCount, CPoint*& pPoints)	
{
	OGDCASSERT(!OGDCIS0(m_dCoordRatio) && !m_rcClient.IsRectEmpty() && 
		!m_rcViewBounds.IsEmpty());

	if(nCount <=0 || pPoint3Ds == NULL)
	{
		return FALSE;
	}

	if(pPoints != NULL)
	{
		delete[] pPoints;
		pPoints = NULL;
	}

	pPoints = new CPoint[nCount];
	for(int i=0;i<nCount;i++)
	{
		pPoints[i].x = OGDCROUND((pPoint3Ds[i].x - m_rcViewBounds.left)/m_dCoordRatio);
		pPoints[i].y = OGDCROUND((m_rcViewBounds.top - pPoint3Ds[i].y)/m_dCoordRatio);
	}
	return TRUE;
}

//! 设备坐标到地理坐标转换。
BOOL CDrawing::DeviceToGeo(CPoint* pPoints, int nCount, OgdcPoint2D*& pPoint2Ds)
{
	OGDCASSERT(!OGDCIS0(m_dCoordRatio) && !m_rcClient.IsRectEmpty() && 
		!m_rcViewBounds.IsEmpty());

	if(nCount <=0 || pPoints == NULL)
	{
		return FALSE;
	}

	if(pPoint2Ds != NULL)
	{
		delete[] pPoint2Ds;
		pPoint2Ds = NULL;
	}

	pPoint2Ds = new OgdcPoint2D[nCount];
	for(int i=0;i<nCount;i++)
	{
		pPoint2Ds[i].x = m_rcViewBounds.left + pPoints[i].x*m_dCoordRatio;
		pPoint2Ds[i].y = m_rcViewBounds.top - pPoints[i].y*m_dCoordRatio;
	}
	return TRUE;
}

unsigned short CDrawing::BitCount(PixelFormat pixelFormat)
{
	switch (pixelFormat)	
	{
	case IPF_MONO:
		return 1;
	case IPF_FBIT:
		return 4;
	case IPF_BYTE:
		return 8;
	case IPF_TBYTE:
		return 16;
	case IPF_RGB:
		return 24;
	case IPF_RGBA:
		return 32;
	case IPF_TRGB:
		return 48;
	case IPF_LONGLONG:
		return 64;
	case IPF_LONG:
		return 32;
	case IPF_FLOAT:
		return 32;
	case IPF_DOUBLE:
		return 64;
	default:
		return 1;
	}
}
