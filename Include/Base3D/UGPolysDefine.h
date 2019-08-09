// UGPolysDefine.h: interface for the UGPolysDefine class.
//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file UGPolysDefine.h
//!  \brief 参数化对象建模模型结构。
//!  \details 文件详细信息。
//!  \author 陈玉成。
//!  \attention 
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_UGPOLYSDEFINE_H__6065BF90_B191_4740_AAA0_0CD7AD8D8A50__INCLUDED_)
#define AFX_UGPOLYSDEFINE_H__6065BF90_B191_4740_AAA0_0CD7AD8D8A50__INCLUDED_
// [Guid("6065BF90-B191-4740-AAA0-0CD7AD8D8A50")]

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Toolkit/UGColorValue3DEx.h"
#include "Base3D/UGPrerequisites3D.h"

namespace UGC
{
	//! \brief 模型坐标类型。
	enum UGCoordinatesType
	{
		//! \brief 笛卡尔坐标系。
		//! \remark 顶点为局部笛卡尔坐标系下坐标(x, y, z)。
		Cartesian,

		//! \brief 球体ENU坐标系。
		//! \remark 局部笛卡尔坐标系和球极坐标系组合。
		ENU, 

		//! \brief 球体经纬度极坐标系。
		//! \remark 顶点为极坐标系下经纬度坐标(lon, lat, high)。
		LonLat,

		//! \brief 椭球体ENU坐标系。
		//! \remark 局部笛卡尔坐标系和椭球极坐标系组合。
		Ellipsoidal_ENU,

		//! \brief 投影坐标系。
		Projection,

		//! \brief 伪直角坐标系。
		Pseudo_Cartesian,
	};

	//! \brief 参数化对象类型。
	enum UGPrimitiveType
	{
		no_type,
		pt, line,
		rect, circle, ellipse, region,
		box, cylinder, ellipsoid, pyramid, sphere, polyhedron, tin, solid
	};

	//! \brief 建模操作。
	enum UGBuilderOperation
	{
		no_operation,
		linear_extrude, rotate_extrude, pipe_extrude, loft, blend, polygon_offsetting, straight_skeleton, envelop, builder,
		section_projection, plane_projection, flatten, shadow_volume, clip, mosaic, replace, cull, match
	};

	//! \brief 纹理坐标类型。
	enum UGUVWCoordinatesType { U, V, W, UV, VW, WU, UVW };

	//! \brief 纹理贴图映射模式。
	//! \remark 按模型实际大小贴图：4种(PLANAR, CYLINDRICAL(CYLINDRICAL_CAP), SPHERICAL，BOX)，非实际尺寸7种。
	enum UGUVWMappingMode { NOMAP, PLANAR, CYLINDRICAL, CYLINDRICAL_CAP, SPHERICAL, SHRINK_WRAP, BOX, FACE, XYZ2UVW };

	//! \brief 光照类型。
	enum UGShadowVolumeType
	{
		//! \brief 太阳光
		sunLight, 
		//! \brief 点光源
		pointLight,
		//! \brief 方向光
		directLight
		////! \brief 聚光灯
		//spotLight
	};

	//! \brief 投影后的结果返回形式
	enum UGProjectResultType
	{
		//! \brief 返回上表面
		topFace,
		//! \brief 返回下表面
		bottomFace,
		//! \brief 返回体数据
		shadowVolume
	};

	//! \brief 布尔运算类型。
	enum UGBooleanOperationType
	{
		OP_DIFFERENCE,             // Difference                        
		OP_INTERSECTION,              // Intersection                      
		OP_UNION,                  // Union 
		OP_XOR,                     // Exclusive or
		OP_MINKOWSKI,
		OP_HULL
	};

	//! \brief 保留类型
	enum UGKeepType
	{
		//! \brief 内裁，保留内部。
		KeepInside,
		//! \brief 外裁，保留外部。
		KeepOutside
	};

	//! \brief 约束类型。
	enum UGConstraintType
	{
		//! \brief 软约束，按照原始数据计算。
		SoftConstraint,
		//! \brief 硬约束，按照新数据计算。
		HardConstraint
	};

	//! \brief 衔接角类型。
	enum UGJoinType
	{
		//! \brief 方角衔接，连接角为方角。
		Square,
		//! \brief 圆角衔接，连接角为圆角。
		Round,
		//! \brief 直接衔接，连接角直连。
		Miter
	};

	//! \brief 点属性
	enum UGPolyPointAttribute
	{
		CommonPt			= 0x00000000, // 普通点
		PtCorner			= 0x00000001, // 四个角上的点
		PtInter			= 0x00000002, // 边上的交点
		PtByEdge			= 0x00000004, // 三角化后与边相邻的内部点
		PtEdge			= 0x00000008, // 边上的特征点
		InvalidPt		= 0x00000010, // 无效点
		PtInterInvalid	= 0x00000020, // 边上无效的交点
		PtNeedRemove		= 0x00000040, // 需要去除的点
		PtEdgeWithSkirt	= 0x00000080, // 有裙边的边点
		PtEdgeByClip		= 0x00000100, // 被裁剪出的点
		PtEdgeByBound		= 0x00000200, // 约束的边点

		SolfConstraintPt	= 0x00000400, // 软约束点
		HardConstraintPt	= 0x00000800, // 硬约束点
		ClipPt			= 0x00001000, // 裁剪点
	};

	struct UGParam
	{
		/* 参数基类 */
	};

	struct UGObject
	{
		/* 对象基类 */
	};

	//! \brief 参数化对象参数类。
	struct BASE3D_API UGPrimitiveParam
	{
		UGPrimitiveParam();
		~UGPrimitiveParam();

		//! \brief 重置。
		void Reset();

		//! \brief 参数化对象类型。
		UGPrimitiveType ePrimitiveType;

		//! \brief 模型构建类型。
		UGBuilderOperation eBuilderOperation;

		//! \brief 基本参数信息。
		UGdouble dX, dY, dZ;
		UGdouble dR1, dR2;

		// 特殊的三参数。
		//! \brief nResolution 分辨率。
		UGuint nResolution;
		//! \brief nFragmentSize 片段大小，单位毫米。
		UGuint nFragmentSize;
		//! \brief nFragmentAngle 片段角度，单位度。
		UGuint nFragmentAngle;

		//! \brief 是否创建LOD。
		UGbool bGenerateLOD;

		//! \brief 返回一个圆的数量切片。
		//! \param dRadius [in] 圆的半径。
		//! \param nResolution [in] 分辨率。
		//! \param nFragmentSize [in] 片段大小，单位毫米。
		//! \param nFragmentAngle [in] 片段角度，单位度。
		UGuint GetFragments(UGdouble dRadius);
	};

	//! \brief 纹理贴图参数类。
	struct BASE3D_API UGUVWMapParam
	{
		UGUVWMapParam();
		~UGUVWMapParam();

		//! \brief 重置。
		void Reset();

		//! \brief 贴图类型。
		UGUVWMappingMode eMappingMode;

		//! \brief 纹理坐标是否世界坐标系下贴图大小。
		UGbool bRealWorldMapSize;

		//! \brief 纹理坐标计算是否使用纹理图片实际尺寸计算。
		UGbool bRealTexMapSize;

		//! \brief 纹理坐标所放通道。
		//! \remark 介于[0, SMSCN_MAX_TEXTURE_COORD_SETS - 1]。
		UGuint uMapChannel;

		//! \brief 纹理坐标类型。
		UGUVWCoordinatesType eCoordType;

		//! \brief 投影模型长度、宽度、高度。
		//! \remark 当参数bRealWorldMapSize为TRUE时，以下参数无效。
		UGdouble dPrjModelLength;
		UGdouble dPrjModelWidth;
		UGdouble dPrjModelHeight;

		//! \brief 参数投影模型长度、宽度、高度是否由用户定义。
		UGbool bPrjLenghByUserDefined;
		UGbool bPrjWidthByUserDefined;
		UGbool bPrjHeightByUserDefined;

		//! \brief UVW方向位移距离。
		//! \remark 当参数bRealWorldMapSize为TRUE时，以下参数无效。
		UGdouble dUOffset;
		UGdouble dVOffset;
		UGdouble dWOffset;

		//! \brief UVW方向重复次数。
		//! \remark 当参数bRealWorldMapSize为TRUE时，以下参数无效。
		UGdouble dUTiling; // U Tile
		UGdouble dVTiling; // V Tile
		UGdouble dWTiling; // W Tile

		//! \brief UVW方向是否翻转。
		//! \remark 当参数bRealWorldMapSize为TRUE时，以下参数无效。
		UGbool bUFlip;
		UGbool bVFlip;
		UGbool bWFlip;

		//! \brief UVW方向旋转角度。
		UGdouble dUAngle;
		UGdouble dVAngle;
		UGdouble dWAngle;

		//! \brief 返回纹理矩阵。
		UGMatrix4d GetTexMatrix() const;
	};

	//! \brief 字段属性。
	template<typename T>
	class UGAttributeTemplate
	{
	public:
		UGAttributeTemplate() : m_iAttribute(0) {}
		UGAttributeTemplate(const UGAttributeTemplate<T>& other) { m_iAttribute = other.m_iAttribute; }
		~UGAttributeTemplate() {}

		UGAttributeTemplate(T iAttribute) { m_iAttribute = iAttribute; }

		UGbool operator==(const UGAttributeTemplate<T>& other) const { return m_iAttribute == other.m_iAttribute; }
		UGbool operator!=(const UGAttributeTemplate<T>& other) const { return m_iAttribute != other.m_iAttribute; }

		UGbool Has(T attr) const { return (m_iAttribute & attr) != 0; }
		void Add(T attr) { m_iAttribute |= attr; }
		void Remove(T attr) { m_iAttribute &= ~attr; }

		T Get() const { return m_iAttribute; }
		void Set(T attr) { m_iAttribute = attr; }

	private:
		T m_iAttribute;
	};
	typedef UGAttributeTemplate<UGint> UGAttribute;

	//! \brief 伪零值
#define PSEUDO_ZERO 0.0001

	//! \brief 地理上的高度极值
	//! \remark 世界上最高的是珠穆朗玛峰(8848.13米)，最深的是马里亚纳海沟(-11034.0米)
#define GEOGRAPHICAL_MAX_ALTITUDE 9000.0
#define GEOGRAPHICAL_MIN_ALTITUDE -12000.0

	//! \brief 操作结果
	//! \param failure代表操作失败，可能是参数问题。
	//! \param nothing代表没有未处理，数据完好未修改。
	//! \param success代表处理成功，数据已经修改。
	enum UGOperationResult
	{
		failure = -1, nothing = 0, success = 1
	};

	class UGPolySet;
	typedef std::vector<UGPolySet*> UGPolySetPtrs;
	typedef std::vector<UGPolySet > UGPolySetObjs;
	typedef UGPolySetPtrs UGPolySets;

	//! \brief 裁剪面集。
	struct UGClipPoly
	{
		UGPolySet*			pClipPoly;
		UGPolySet*			pRefSlopePoly;
		UGPolySet*			pSlopePoly;

		UGClipPoly() : pClipPoly(NULL), pRefSlopePoly(NULL), pSlopePoly(NULL) {}
	};
	typedef std::vector<UGClipPoly> UGClipPolys;

	struct UGOperationParam
	{
		virtual UGBuilderOperation GetType() const = 0;
	};

	//! \brief 裁剪操作参数集。
	struct UGClipOperationParam : public UGOperationParam
	{
		UGBuilderOperation	op;
		UGKeepType			keep;
		UGConstraintType		constraint;
		UGdouble				radius;
		UGColorValue3D		color;

		UGClipOperationParam() : op(clip), keep(KeepInside), constraint(SoftConstraint), radius(0.0) {}

		virtual UGBuilderOperation GetType() const { return clip; }
	};

	//! \brief 剔除操作参数类。
	struct UGCullOperationParam : public UGOperationParam
	{
		UGKeepType			keep;
		UGdouble				minz;
		UGdouble				maxz;

		UGCullOperationParam() : keep(KeepOutside), minz(GEOGRAPHICAL_MIN_ALTITUDE), maxz(GEOGRAPHICAL_MAX_ALTITUDE) {}

		virtual UGBuilderOperation GetType() const { return cull; }
	};

	//! \brief 拉伸参数类。
	struct UGLinearExtrudeParam : public UGOperationParam
	{
		//! \brief 面枚举
		enum Face { bottom = 0x1, top = 0x2, side = 0x4 };

		//! \brief 拉伸高度(沿z轴方向)，取值范围(0, ∞)。
		UGdouble dHeight;

		//! \brief 是否生成纹理坐标
		UGbool bCreateTexCoord;

		//! \brief 绕z轴旋转的角度，取值范围(-∞, ∞)。
		UGdouble dTwist;

		//! \brief 模型拉伸过程中在X和Y方向的缩放比例，取值范围(0, ∞)。
		UGdouble dScaleX;
		UGdouble dScaleY;

		//! \brief 构建过程中的采样切片数量，采用默认值即可，取值范围[1, ∞)。
		UGuint nSlices;

		//! \brief 是否成一组
		UGbool bGroup;

		//! \brief 是否有LOD层级
		UGbool bHasLOD;

		//! \brief 生成模型面属性
		UGAttribute attr;

		UGLinearExtrudeParam() : 
			dHeight(1.0), 
			bCreateTexCoord(FALSE), 
			dTwist(0.0), 
			dScaleX(1.0), dScaleY(1.0), 
			nSlices(1), 
			bGroup(TRUE), 
			bHasLOD(TRUE),
			attr(bottom | side | top) {}

		virtual UGBuilderOperation GetType() const { return linear_extrude; }
	};

	//! \brief 车削参数类。
	struct UGRotateExtrudeParam : public UGOperationParam
	{
		//! \brief 面枚举
		enum Face { start = 0x1, end = 0x2, ring = 0x4 };

		//! \brief 旋转角度，取值范围[-360, 0) U (0, 360]。
		UGdouble dAngle;

		//! \brief 构建过程中的采样切片数量，采用默认值即可，取值范围[1, ∞)。
		UGuint nSlices;

		//! \brief 是否成一组
		UGbool bGroup;

		//! \brief 是否有LOD层级
		UGbool bHasLOD;

		//! \brief 生成模型面属性
		UGAttribute attr;

		UGRotateExtrudeParam() : 
			dAngle(360.0), 
			nSlices(1), 
			bGroup(TRUE), 
			bHasLOD(TRUE),
			attr(ring | start | end) {}

		virtual UGBuilderOperation GetType() const { return rotate_extrude; }
	};

	//! \brief 放样参数类。
	struct UGPipeExtrudeParam : public UGOperationParam
	{
		//! \brief 面枚举
		enum Face { start = 0x1, end = 0x2, side = 0x4 };

		//! \brief 是否生成纹理坐标
		UGbool bCreateTexCoord;

		//! \brief 衔接角类型。
		UGJoinType eJoinType;

		//! \brief 构建过程中的采样切片数量，采用默认值即可，取值范围[1, ∞)。
		UGuint nSlices;

		//! \brief 是否成一组
		UGbool bGroup;

		//! \brief 是否有LOD层级
		UGbool bHasLOD;

		//! \brief 生成模型面属性
		UGAttribute attr;

		UGPipeExtrudeParam() : 
			bCreateTexCoord(FALSE), 
			eJoinType(Miter),
			nSlices(1), 
			bGroup(TRUE), 
			bHasLOD(TRUE),
			attr(side | start | end) {}

		virtual UGBuilderOperation GetType() const { return pipe_extrude; }
	};

	//! \brief 匹配参数类。
	struct UGMatchOperationParam : public UGOperationParam
	{
		//! \brief 依附类型。
		enum Attach { top, bottom };

		//! \brief 采样间隔。
		UGdouble dGranularity;

		//! \brief 依附类型。
		Attach eAttach;

		UGMatchOperationParam() :
			dGranularity(0.0),
			eAttach(top) {}

		virtual UGBuilderOperation GetType() const { return match; }
	};
}
#endif