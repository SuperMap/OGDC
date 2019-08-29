//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file headerfile.h
//!  \brief 
//!  \details 
//!  \author zhangshuai。
//!  \attention 
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////
#if !defined(AFX_UGMAXTOOLKIT_H__E89D0670_EB32_4782_8964_7F60F1421BCF__INCLUDED_)
#define AFX_UGMAXTOOLKIT_H__E89D0670_EB32_4782_8964_7F60F1421BCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <ShlObj.h>
#include <Stream/ugdefs.h>
#include "Base3D/UGRenderOperationGroup.h"
#include "Base3D/UGCommon3D.h"

#undef SelectFont
#undef SelectBrush
#undef SelectPen

//!  \导出参数单独一个结构体吧
typedef struct exportParameter
{
	UGC::UGbool RemoveDupVertex;
	UGC::UGbool IsPlanar;
	UGC::UGbool IgnoreMaterialColor;
	UGC::UGbool WithoutNormal;
	UGC::UGdouble X;
	UGC::UGdouble Y;
	UGC::UGdouble Z;
	UGC::UGString DatasourceName;
	UGC::UGString DatasetName;
}ExportParameter;
//using namespace UGC;

enum FileType
{
	origin,
	compress
};

class  UGMaxToolkit 
{
public:
	//! \brief 弹出文件夹对话框，dialogName为对话框的名字，返回选择的结果。
	//! \param dialogName 动画名称[in]。
	static UGC::UGString GetInputPath(TCHAR* dialogName,int IDC,HWND hDlg,bool setText,TCHAR* pInitialPath =NULL);

	//!  \所有的目录应该已斜杠结尾，该函数使目录路径带有斜杠
	static void TransStandDir(TCHAR* pDespath);

	//!  \ 判定字符串是否为合法的整型和浮点型。
	static bool IsInt(LPTSTR intStr);

	static bool IsFloat(LPTSTR floStr);

	//!  \ 创建UDB，里面只有m_arrmodelProp所描述的模型索引，不含模型本身的数据信息。
	//!   \cmpathName文件生成路径，
	//!   \cfileName不带后缀名的文件名，
	//!   \m_arrmodelProp模型信息表，
	//!   \longitude 插入点经度，
	//!   \latitude  插入点纬度，
	//!   \altitude  插入点高度，

	//static void CreateUDB(UGC::UGString cmpathName,UGC::UGString cfileName,UGArray<ModelProp>m_arrmodelProp,double longitude,double latitude,double altitude);

	//static void CreateUDBWithFilePath(UGC::UGString cmpathName,UGC::UGString cfileName,UGArray<ModelProp>m_arrmodelProp,double longitude,double latitude,double altitude);


	//!  \ 删除给定目录下所有的文件。
	//!  \strDeleteDir 给定的目录
	static void DeleteAllMyDirFiles(UGC::UGString strDeleteDir);

	//!  \ 将相对插入点的直角坐标系的坐标点转换为经纬度坐标点 (注意，高度不变 wangxinhe 20120607)
	//!   \dLatitude 插入点经度，
	//!   \dLatitude  插入点纬度，
	//!   \dAltitude  插入点高度，
	//!   \p  输入为直角坐标系，输出为球面坐标
	static void CartesianToSphericalD(double dLongitude,double dLatitude,double dAltitude,UGC::UGPoint3D& p);

	//!  \ 是否是C语言标志符的命名规则
	//!   \strName 标识符名称
	static bool IslegalName(UGC::UGString strName);
	

	//!  \是否在正确的经纬度表示范文
	//!   \pStrLonti 经度
	//!   \pLati 纬度
	static UGC::UGbool IsCorrectSphericalD(char* pStrLonti,char* pLati);


	//!  \拷贝纹理，并对纹理进行适当的缩放
	//!   \strSrcTexPath 源文件路径
	//!   \strDesTexPath 目标拷贝文件路径
	static void CopyTexture(const UGC::UGString& strSrcTexPath,const UGC::UGString& strDesTexPath);


	//! \brief 保存标识为水面对象的名称列表
	static std::set<UGC::UGString> m_setRegionZ;


	//! \brief 保存标识为BillBoard对象的名称列表
	static std::set<UGC::UGString> m_setBillboardList;

	static UGC::UGRenderOperationNode::TextureDataInfo* ImageTOTextureDataInfo(UGC::UGString strTex);

	//! \brief
	static UGC::UG3DPixelFormat GlPixelFormat2UG3DFormat(UGC::UGuint osgPixelFormat);


};

#endif //AFX_UGMAXTOOLKIT_H__E89D0670_EB32_4782_8964_7F60F1421BCF__INCLUDED_
