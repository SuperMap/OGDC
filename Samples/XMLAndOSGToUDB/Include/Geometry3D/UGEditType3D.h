//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file UGEditType3D.h
//!  \brief 定义三维中的编辑常数。
//!  \author UGCTeam。
//!  \attention 
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_UGEDITTYPE3D_H__9969CA39_1F0D_4AD2_920F_E860971C502C__INCLUDED_)
#define AFX_UGEDITTYPE3D_H__9969CA39_1F0D_4AD2_920F_E860971C502C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base3D/UGPrerequisites3D.h"

namespace UGC
{
class GEOMETRY3D_API UGEditType3D  
{
public:
	//! \brief 三维编辑工具形状。
	enum EditToolShape3D
	{	
		ET3D_NULL					= 0,			// 空
        ET3D_Selection				= 1,            // 选择
        ET3D_Point					= 21,           // 点 
		ET3D_LineSect				= 22,           // 直线
		
        ET3D_EllipseArc				= 23,			// 椭圆弧
        ET3D_PolyLine				= 24,			// 折线,支持subaction
        ET3D_BezierCurve			= 25,			// 贝兹曲线,支持subaction
        ET3D_Rectangle				= 26,			// 矩形
        ET3D_RoundRect				= 27,			// 圆角矩形
        ET3D_Parallelogram			= 28,			// 平行四边形
        ET3D_Circle					= 29,			// 圆心圆
        ET3D_Ellipse				= 30,			// 椭圆	
        ET3D_ObliqueEllipse			= 31,			// 斜椭圆
        ET3D_PolyGon				= 32,			// 多边形,支持subaction
		ET3D_MulitiPoint			= 33,			// 多点
		//ET_ARC3P					= 33,	        // 三点弧（圆弧）
        ET3D_Text					= 34,			// 注记
        ET3D_BSpline				= 35,			// B样条,支持subaction
        //sET_SameDistline			= 36,			//等值线
        ET3D_StreamText				= 37,			//沿线注记	
		//ET_Mulitiline				= 38,           //多段线,已废除，请使用ET_PolyLineEx、ET_PolygonEx代替
		ET3D_Path					= 39,           //路径（不知道怎么翻译）
		ET3D_PolyLineEx				= 42,			// 
		ET3D_PolygonEx				= 43,			// 
		ET3D_Arc3P					= 44,           //三点弧
		ET3D_Circle3P				= 45,           //三点圆
		ET3D_Pie					= 46,           //派对象 
		ET3D_Circle2P				= 47,           //两点圆
		ET3D_Parallel				= 48,			//平行线,支持subaction
		//ET_Custom					= 49,	        //扩展对象
		
		ET3D_TrackPolyLine			= 55,			//半自动跟踪线
		ET3D_TrackBreak				= 56,			//半自动跟踪回退
		ET3D_TrackPolyPolygon		= 57,			//跟踪面
		ET3D_FreeLine				= 58,			//画笔，自由画线，支持subaction		
        ET3D_Cardinal				= 59,			//Cardinal曲线，支持subaction
		ET3D_Hatch					= 60,			//Hatch面
		ET3D_Model					= 61,			// 模型
		ET3D_Overlay				= 62,			// OverLay
		
		ET3D_SetSnapPoint			= 70,			//设置选中对象的捕捉点，移动的时候就用设置的这个捕捉点捕捉，否则就用鼠标点
		
		ET3D_Picture				= 1101,			//图片对象(GeoPicture)
		
		ET3D_Map					= 2001,			//geomap对象，布局中
		//ET_MapRegion				= 2004,			//geomapRegion对象，布局中
		ET3D_Scale					= 2005,			//geoscale对象，布局中
		ET3D_Direction				= 2008,			//geomap对象，布局中
		
		ET3D_ArtText				= 2010,			//GeoArtText对象, 布局中

		ET3D_Move                   = 3000,        // 平移 
		ET3D_ScaleXYZ               = 3001,        // XYZ方向缩放
		ET3D_ScaleXorY              = 3002,        // X或Y方向缩放
		ET3D_RotateZ                = 3003,        // Z绕Z旋转
		ET3D_ScaleZ                 = 3004,        // Z方向缩放
		ET3D_ScaleX					= 3005,        // X或Y方向缩放
		ET3D_ScaleY					= 3006,        // X或Y方向缩放
		ET3D_ScaleXYZ_NWSE          = 3007,		   // XYZ方向缩放,沿西北东南方向
		ET3D_ScaleXYZ_NESW			= 3008,		   // XYZ方向缩放,沿东北西南方向

		ET3D_MoveZ                  = 3009,        // 高度平移 

		
		ET3D_RegionPoint            = 3010,        // 节点（节点平移）
		ET3D_RegionGroundPoint      = 3011,        // 底面节点（节点高度）
		ET3D_RegionLine             = 3012,        // 线（添加节点）
		
		ET3D_RotateX                = 9997,        // X绕X旋转
		ET3D_RotateY                = 9998,        // Y绕Y旋转
		ET3D_GraphicObject			= 9999,		   //标绘对象
	};

	//编辑交互方式
	enum EditAccessibilityAction
	{
		//无
		EAA_None,	
		//编辑节点
		EAA_EditPoint,			
		//添加节点
		EAA_AddPoint,
		//面
		EAA_RgnMode,			
		//线
		EAA_LineMode
	};

public:
	UGEditType3D();
	virtual ~UGEditType3D();
};

}

#endif // !defined(AFX_UGEDITTYPE3D_H__9969CA39_1F0D_4AD2_920F_E860971C502C__INCLUDED_)


