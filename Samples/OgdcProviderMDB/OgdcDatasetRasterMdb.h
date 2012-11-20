//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)         
//                                                           
//!  \file OgdcDatasetRasterMdb.h
//!  \brief 栅格数据集的接口定义。
//!  \details 栅格数据集的各种接口
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Limited Company.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCDATASETRASTERMDB_H__4FE7AE19_5388_451A_9C07_FC96405F8E5A__INCLUDED_)
#define AFX_OGDCDATASETRASTERMDB_H__4FE7AE19_5388_451A_9C07_FC96405F8E5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OGDC/OgdcDatasetRaster.h"
#include "Base/OgdcDict.h"
#include "OgdcDataSourceMdb.h"
#include "Base/OgdcRect.h"

namespace OGDC
{

//! \brief 栅格数据集实现的类
//! \remarks 栅格数据集包括影像数据集和Grid数据集。
class OGDC_API OgdcDatasetRasterMdb : public OgdcDatasetRaster  
{
	//! \brief 声明友元类
	friend class OgdcDataSourceMdb;

public:
	//! \brief 栅格数据集的默认构造函数
	OgdcDatasetRasterMdb();

	//! \brief 带指向数据源指针的构造函数
	//! \param pDataSource 指向数据源的指针[in]。
	OgdcDatasetRasterMdb(OgdcDataSourceMdb *pDataSource);

	//! \brief 栅格数据集默认析构函数
	virtual ~OgdcDatasetRasterMdb();
	
public:
	//! \brief 判断数据集是否已经打开。
	//! \return 打开返回TRUE，否则返回FALSE。
	virtual OgdcBool IsOpen() const ;

	//! \brief 打开数据集。
	//! \return 是否成功打开数据集，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool Open() ;

	//! \brief 关闭数据集。
	//! \return 是否成功关闭数据集，成功返回TRUE，失败返回FALSE。
	virtual void Close() ;	
	
	//! \brief 重命名数据集。
	//! \param strNewName 新的数据集名[in]。
	//! \return 是否成功重命名数据集，成功返回TRUE，失败返回FALSE。
	//! \remarks 只改变数据集名,表名不变。
	virtual OgdcBool Rename(const OgdcString& strNewName) ;
	
	//! \brief 判断当前数据集是否可编辑。
	//! \return 可编辑返回TRUE，不可返回FALSE。
	virtual OgdcBool CanUpdate() ;
	
	//! \brief 将数据集各种信息保存到数据库中。
	//! \return 是否成功保存，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool SaveInfo() ;
	
	//! \brief 刷新数据集信息。
	//! \return 是否成功刷新，成功返回TRUE，失败返回FALSE。
	//! \remarks 从数据库中读取数据集的各种属性，刷新当前数据集的信息。
	virtual OgdcBool RefreshInfo() ;	

	//! \brief 创建影像金字塔。
	//! \param bShhowProgress 是否显示进度条[in]。
	//! \return 是否创建成功，是返回TRUE，否则返回FALSE。
	virtual OgdcBool BuildPyramid();	

	//! \brief 移除影像金字塔。
	//! \return 是否移除成功，是返回TRUE，否则返回FALSE。
	virtual OgdcBool DropPyramid() ;
	
	//! \brief 判断是否已经建立影像金字塔。
	virtual OgdcBool IsBuildPyramid() const;

	//! \brief 得到扫描线
	virtual OgdcBool GetScanline(OgdcInt nRowIndex,OgdcRasterScanline& ScanLine, OgdcInt nStartCol = 0);

	//! \brief 通过行列索引设置扫描线
	virtual OgdcBool SetScanline(OgdcInt nRowIndex,OgdcInt nColIndex,
		const OgdcRasterScanline& ScanLine);

	//! \brief 读入指定块数据。
	//! \param nRowBlock 影像块行号[in]。
	//! \param nColBlock 影像块列号[in]。
	//! \return 影像块指针。
	//! \attention 返回的的栅格块指针由外面负责释放
	virtual OgdcRasterBlock* LoadBlock(OgdcInt nRowBlock, OgdcInt nColBlock);	
    
	//! \brief 将一内存中的影像块存入磁盘。
	//! \param pImgBlock 影像块指针[in]。
	//! \return 是否成功保存，成功返回TRUE，失败返回FALSE。
	virtual OgdcBool SaveBlock(OgdcRasterBlock* pImgBlock);	
	
	//! \brief 释放在内存中的所有栅格块
	virtual void ReleaseAllBlocks();
	
	//! \brief 获取影像数据集指定行列某一像素点的颜色值。
	//! \param nCol 像素点列号[in]。
	//! \param nRow 像素点行号[in]。
	//! \return 颜色值。
	//! \remarks 颜色值的数值因数据集的像素格式不同而不同。
	virtual OgdcColor GetPixel(OgdcInt nCol, OgdcInt nRow);	
	
	//! \brief  设置影像数据集指定行列某一像素点的颜色值。
	//! \param nCol 像素点列号[in]。
	//! \param nRow 像素点行号[in]。
	//! \param nColor 颜色值[in]。
	//! \remarks 要根据数据集的像素格式正确设置像素点的颜色值。
	virtual void SetPixel(OgdcInt nCol, OgdcInt nRow, OgdcColor nColor);	
	
	//! \brief 获取Grid数据集指定行列某一点的特性值。
	//! \param nCol 网格列号[in]。
	//! \param nRow 网格行号[in]。
	//! \return 特性值。
	virtual OgdcDouble GetValue(OgdcInt nCol, OgdcInt nRow);
	
	//! \brief 设置Grid数据集指定行列某一点的特性值。
	//! \param nCol 网格列号[in]。
	//! \param nRow 网格行号[in]。
	//! \param dValue 特性值[in]。
	//! \remarks 要根据数据集的白化值正确设置网格点的特性值。
	virtual void SetValue(OgdcInt nCol, OgdcInt nRow, OgdcDouble dValue);
	
	//! \brief 获取可见栅格块
	//! \param rcView 可视范围[in]。
	//! \param nDeviceWidth 设备宽度，主要指当前窗口的宽度[in]。
	//! \param nDeviceHeight 设备高度，主要指当前窗口的高度[in]。
	//! \return 返回可见栅格块指针。
	//! \attention 返回的指针由外面管理，负责释放。
	virtual OgdcRasterBlock* GetViewBlock(const OgdcRect2D& rcView, 
		OgdcInt nDeviceWidth, OgdcInt nDeviceHeight);

	//! \brief 返回数据源指针
	virtual OgdcDataSource* GetDataSource() const;

public:

	//! \brief 判断是否为栅格数据集
	OgdcBool IsRaster();
				
	//! \brief 判断数据集是否在打开后有过修改。
	//! \return 有过修改返回TRUE，没有返回FALSE。
	virtual OgdcBool IsModified() const;

	//! \brief 设置数据集是否有过修改。
	//! \param bModified 数据集是否有过修改，默认有修改[in]。
	virtual void SetModifiedFlag(OgdcBool bModified = TRUE);

	//! \brief 地理坐标换成像素坐标。
	//! \param rectXY [in]。
	//! \param rectImg [out]。
	void XYToImg(const OgdcRect2D& rectXY, OgdcRect& rectImg);

	//! \brief 像素坐标换成地理坐标
	//! \param rectImg [in]。
	//! \param rectXY [out]。
	void ImgToXY(const OgdcRect& rectImg, OgdcRect2D& rectXY);	
	
	//! \brief 获取相应层的金字塔
	//! \param nImgWidth [in]。
	//! \param nDeviceWidth [in]。
	//! \return 金字塔图层的指针。
	OgdcDatasetRasterMdb* GetPyramidTier(OgdcInt nImgWidth, OgdcInt nDeviceWidth);
	
	//! \brief 创建金字塔
	//! \param rInfo 栅格数据集描述信息对象[in]。
	//! \param nLevel 要创建的金字塔的层数[in]。
	//! \return 创建好的金字塔图层的指针。
	OgdcDatasetRasterMdb* CreateDatasetPyramid(OgdcDatasetRasterInfo& rInfo, OgdcInt nLevel);

	//! \brief 设置Grid数据集的调色板
	//! \param pColorTable=NULL [in]。
	//! \return 返回是否设置成功
	OgdcBool SetGridColorTable(OgdcColorTable* pColorTable=NULL);

private:

	//! \brief 释放所有金字塔数据集
	//! \return 返回是否删除成功
	//! \remarks 删除数据集在内存中的所有金字塔图层的指针
	OgdcBool ReleasePyramidDatasets();	

	//! 计算像素边长的移位操作位数
	OgdcBool ComDivAndMod();

protected:
	//! \brief 数据集的ID	
 	OgdcLong m_nID;

	//! \brief 指向数据源的指针
	OgdcDataSourceMdb *m_pDataSource;
	
	//! \brief 数据集是否打开的标志
 	OgdcBool m_bOpened;
	
	//! \brief 是否修改的标志	
	OgdcBool m_bModified;

	//! \brief 用来装加载到内存中栅格块的字典
	//! \remarks 字典的nKey为栅格块索引号
	OgdcDict<OgdcInt,OgdcRasterBlock*> m_rasterDict;

	//! \brief 显示参考坐标
	OgdcRect2D m_rcViewGeoRef;	

	//! \brief 地理坐标与影像映射比例
	OgdcSize2D m_imgRatio;
	
	//! \brief 存放各层金字塔的指针数组
	OgdcArray<OgdcDatasetRasterMdb*> m_pyramidDatasets;

	//! \brief 计算块行列位移操作
	OgdcUint m_nDiv;

	//! 计算块行列取余操作
	OgdcUint m_nMod;

	//! 记录着m_nDiv,m_nMod是否准备好
	OgdcUint m_bInitImg;


};

}
#endif 
