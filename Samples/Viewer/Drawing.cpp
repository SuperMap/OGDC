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
		OgdcElement* pElement = NULL;		
		pRecordset->Move(OgdcRecordset::Begin, 0);
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
			OgdcColorset colorset = pDatasetR->m_info.m_colorset;
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
		pDatasetR->SetBandCombineMode(aryBand, nColorSpace);
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
	OgdcRasterBlock* pImgBlock = pDatasetR->GetViewBlock(rcDraw, szBMP.cx, szBMP.cy);
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
		OgdcDouble dNoValue = OGDCROUND(pDatasetR->m_info.m_dNoValue);
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
				pDemBlock->SetPixel(x,y,clrPixel);
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
	strcpy(stFont.lfFaceName,strFontName);
	
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
