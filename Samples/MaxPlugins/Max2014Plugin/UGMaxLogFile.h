//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file 记录最后一次导出时的屏幕投影信息,单例模式
//!  \brief 
//!  \details 
//!  \author sunye。
//!  \attention 
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////
#ifndef UGMAXLOGFILE_H
#define UGMAXLOGFILE_H
#include "Stream/ugdefs.h"
class UGMaxLogFile
{
public:
	~UGMaxLogFile();
	static UGMaxLogFile* GetMaxLogFile();

	//! \brief 设置上一次对话框上显示的经度值
	//! \param strLongtitude [in]
	void SetLontitude(UGC::UGString strLongtitude);

	//! \brief 设置上一次对话框上显示的维度值
	//! \param strLatitude [in]
	void SetLatitude(UGC::UGString strLatitude);

	//! \brief 设置上一次对话框上显示的高度值
	//! \param strAltitude [in]
	void SetAltitude(UGC::UGString strAltitude);

	//! \brief 获取上一次对话框上显示的经度值
	UGC::UGString GetLontitude();

	//! \brief 获取上一次对话框上显示的维度值
	UGC::UGString GetLatitude();

	//! \brief 获取上一次对话框上显示的高度值
	UGC::UGString GetAltitude();

	//-----------------------Add by lina 2014-3-31
	//! \brief 设置上一次对话框上显示的平面坐标X值
	//! \param strLongtitude [in]
	void SetPlaneX(UGC::UGString strPlaneX);

	//! \brief 设置上一次对话框上显示的平面坐标Y值
	//! \param strLatitude [in]
	void SetPlaneY(UGC::UGString strPlaneY);

	//! \brief 设置上一次对话框上显示的平面坐标Z值
	//! \param strAltitude [in]
	void SetPlaneZ(UGC::UGString strPlaneZ);

	//! \brief 获取上一次对话框上显示的平面坐标X值
	UGC::UGString GetPlaneX();

	//! \brief 获取上一次对话框上显示的平面坐标Y值
	UGC::UGString GetPlaneY();

	//! \brief 获取上一次对话框上显示的平面坐标Z值
	UGC::UGString GetPlaneZ();
	//--------------------------------------------
	//! \brief 获区支持的最大纹理
	UGC::UGint GetMaxTextureSize(void) const;

	//! \brief 设置支持的最大纹理
	//! \param nMaxTextureSize [in] 
	void SetMaxTextureSize(UGC::UGint nMaxTextureSize);

	//! \brief 纹理的缩放比
	UGC::UGint GetTextureScaling(void) const;

	//! \brief 设置纹理的缩放比
	//! \param nTextureScaling [in]
	void SetTextureScaling(UGC::UGint nTextureScaling);

	//! \brief 设置支持的最大纹理
	//! \param strBatchTextureSize [in] 
	void SetBatchTextureSize(UGC::UGString& strBatchTextureSize);

	//! \brief 获区支持的最大纹理
	UGC::UGString GetBatchTextureSize(void) const;

	//! \brief 设置支持的最大地形块数
	//! \param nBatchTerrainSize [in] 
	void SetBatchTerrainSize(UGC::UGString& strBatchTerrainSize);

	//! \brief 获区支持的最大地形块数
	UGC::UGString GetBatchTerrainSize(void) const;	

	//! \\导出tif的时候用于记录更改后的纹理名称
	UGDict<UGC::UGString,UGC::UGint>& GetMapName();

	//! \brief 生成纹理的前缀,导入数据库时用于标识纹理
	UGC::UGString GetTexPerfix();

	//! \brief 生成临时目录,只有导入数据库才需要
	UGC::UGString GetTempDir();

private:
	UGC::UGString GetMaxFilePathMapCode();

	UGC::UGuint GetMapCode( const UGC::UGString& strkey );
	
private:
	UGMaxLogFile();
	UGC::UGString m_strLongtitude;
	UGC::UGString m_strLatitude;
	UGC::UGString m_strAltitude;
	//add by lina 平面坐标
	UGC::UGString m_strPlaneX;
	UGC::UGString m_strPlaneY;
	UGC::UGString m_strPlaneZ;

	//! \brief 缩放比例
	UGC::UGuint m_nTextureScaling;

	//! \brief 支持的最大纹理
	UGC::UGuint m_nMaxTextureSize;

	//! \批处理地形时，对应的地形中纹理与地形块顶点的个数大小
	UGC::UGString m_strBatchTextureSize;
	UGC::UGString m_strBatchTerrainSize;

	UGDict<UGC::UGString,UGC::UGint> m_dicttMapName;

	UGC::UGString m_strPrefix;

	static UGMaxLogFile* m_pMaxLogFile;

};
#endif
