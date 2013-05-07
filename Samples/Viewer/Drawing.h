//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file  Drawing.h
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////


#if !defined(AFX_DRAWING_H__1B26EFAF_FF51_4B33_B1E2_4F440DB032C5__INCLUDED_)
#define AFX_DRAWING_H__1B26EFAF_FF51_4B33_B1E2_4F440DB032C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base/OgdcRect.h"
#include "Element/OgdcElemPoint.h"
#include "Element/OgdcElemMultiPoint.h"
#include "Element/OgdcElemLine.h"
#include "Element/OgdcElemText.h"
#include "Element/OgdcElemPoint3D.h"
#include "Element/OgdcElemLine3D.h"
#include "Element/OgdcElemRegion3D.h"

class CDrawing  
{
public:
	CDrawing();
	virtual ~CDrawing();

public:
	//! 显示数据集。
	BOOL DrawDataset(CDC* pDC, OgdcDataset* pDataset);

	//! 显示几何对象。
	BOOL DrawElement(CDC* pDC, OgdcElement* pElement, BOOL bSelected=FALSE);
	
	//! 全幅。
	void ViewEntire();
	
	//! 平移。
	void Pan(double dOffsetX, double dOffsetY);
	
	//! 缩放。
	void Zoom(double dRatio);

	//! 改变客户区大小。
	void ChangeClient(const CRect& rcClient);

	//! 设置坐标转换系数。
	void SetCoordRatio(double dCoordRatio);

	//! 地理坐标到设备坐标转换。
	BOOL GeoToDevice(OgdcPoint2D* pPoint2Ds, int nCount, CPoint*& pPoints);	

	//! 地理坐标到设备坐标转换。
	BOOL GeoToDevice(OgdcPoint3D* pPoint3Ds, int nCount, CPoint*& pPoints);	

	//! 设备坐标到地理坐标转换。
	BOOL DeviceToGeo(CPoint* pPoints, int nCount, OgdcPoint2D*& pPoint2Ds);

protected:
	//! 显示数据集。
	BOOL DrawVector(CDC* pDC, OgdcDatasetVector* pDatasetV);
	BOOL DrawRaster(CDC* pDC, OgdcDatasetRaster* pDatasetR);

	//! 显示几何对象。
	BOOL DrawElement(CDC* pDC, OgdcElemPoint* pElemPoint, BOOL bSelected=FALSE);
	BOOL DrawElement(CDC* pDC, OgdcElemMultiPoint* pElemMultiPoint, BOOL bSelected=FALSE);
	BOOL DrawElement(CDC* pDC, OgdcElemLine* pElemLine, BOOL bSelected=FALSE);
	BOOL DrawElement(CDC* pDC, OgdcElemRegion* pElemRegion, BOOL bSelected=FALSE);
	BOOL DrawElement(CDC* pDC, OgdcElemText* pElemText, BOOL bSelected=FALSE);
	BOOL DrawElement(CDC* pDC, OgdcElemPoint3D* pElemPoint3D, BOOL bSelected=FALSE);
	BOOL DrawElement(CDC* pDC, OgdcElemLine3D* pElemLine3D, BOOL bSelected=FALSE);
	BOOL DrawElement(CDC* pDC, OgdcElemRegion3D* pElemRegion3D, BOOL bSelected=FALSE);

	unsigned short BitCount(PixelFormat pixelFormat);
	BOOL DrawRasterBlock(CDC* pDC, OgdcRasterBlock* pIBlock, OgdcDatasetRaster* pDatasetR, BITMAPINFO* pBitmapInfo);

public:
	OgdcRect2D m_rcBounds;			// 全幅范围，地理坐标。
	OgdcRect2D m_rcViewBounds;		// 显示范围，地理坐标。
	CRect m_rcClient;				// 客户区范围，设备坐标。
	double m_dCoordRatio;			// 地理坐标到设备坐标的转换系数。
	OgdcStyle m_curStyle;

};

#endif // !defined(AFX_DRAWING_H__1B26EFAF_FF51_4B33_B1E2_4F440DB032C5__INCLUDED_)
