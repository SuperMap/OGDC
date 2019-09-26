//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file UGGeometry3D.h
//!  \brief 三维几何对象基类。
//!  \author UGCteam。
//!  \attention 
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_UGGEOMETRY3D_H__0D8126C6_CAE8_4AD5_91D7_47DF8BD75819__INCLUDED_)
#define AFX_UGGEOMETRY3D_H__0D8126C6_CAE8_4AD5_91D7_47DF8BD75819__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Geometry/UGGeometry.h"
#include "Base3D/UGPolySet.h"
#include "Base3D/UGVector3d.h"
#include "Base3D/UGBoundingBox.h"
#include "Geometry3D/UGEditType3D.h"
#include "Base3D/UGStyle3D.h"
#include "Base3D/UGRenderOperationGroup.h"

namespace UGC
{
//class UGStyle3D;
class UGGeoModel;
class UGGeoModelPro;

//! \brief 三维几何对象基类。
class GEOMETRY3D_API UGGeometry3D : public UGGeometry
{
public:
	//! \brief 缺省构造函数。
	UGGeometry3D();	
	
	//! \brief 默认析构函数。
	virtual ~UGGeometry3D();

	//! \brief 清空地理数据
	virtual void Clear();

	//! \brief 得到维度。
	//! \return 维数。
	//! \remarks 点为0维, 线/曲线等为1维, 面/曲面等为2维, GeoCompound/GeoText等为-1，在数据库存储时用。
	virtual UGint GetDimension() const;	

	//! \brief 得到几何对象类型。
	//! \return 固定为 Null。
	virtual UGGeometry::Type GetType() const;	
	
	//! \brief 是否是三维几何对象。
	//! \return 如果是返回TRUE。
	virtual UGbool Is3D() const;

	//! \brief 得到空间数据, 主要用于需要按照维度相同时
	//! 来做统一处理的时候, 例如一些空间判断和操作的算法
	//! \param pPoints [out]。
	//! \param pPolyCount 传出的每个子对象点个数的数组[out]。
	//! \param nSubCount 传出的子对象个数[out]。
	//! \param nSegmentCount 对参数化对象, 每个弧段分割点的个数[in]。
	//! \remarks 参数采用了智能指针的技术,调用者不用释放内存空间
	//! \return 成功返回true,否则返回false。
	//! 此接口暂未实现
	virtual UGbool GetSpatialData(UGAutoPtr<UGPoint2D>& pPoints, 
		UGAutoPtr<UGint>& pPolyCount, UGint& nSubCount, UGint nSegmentCount=0) const;
	
	//! \brief 得到内点（在Geometry3D相对应的Geometry内部，靠近中央的点）
	//! \return 返回内点
	virtual UGPoint2D GetInnerPoint() const;	

	//! \brief 得到内点（可能是Geometry3D的质点）
	//! \return 返回内点
	virtual UGPoint3D GetInnerPoint3D() const;

	//! \brief 几何对象是否有效
	//! \return 有效返回true,无效返回false
	virtual UGbool IsValid() const;
	
	//! \brief 点击测试, 用于对象点选
	//! \param pntHitTest 用来测试选中的点
	//! \param dTolerance 容限
	//! \return 选中返回true, 否则返回false
	virtual UGbool HitTest( const UGPoint2D &pntHitTest, UGdouble dTolerance) const;

      //! \brief 点击测试, 用于对象点选
	//! \param pntHitTest 用来测试选中的点
	//! \param dTolerance 容限
	//! \return 选中返回true, 否则返回false
	virtual UGbool HitTest3D( const UGPoint3D &pntHitTest, UGdouble dTolerance) const;


	//! \brief 点击测试, 用于三维对象在三维空间中点选。
	//! \param vecStart [in] 起始点。
	//! \param vecEnd [in] 终止点。
	//! \param dTolerance [in] 容限。
	//! \return 选中返回true, 否则返回false。
	//! \remarks 建议采用射线检测法，暂时没有实现。
	virtual UGbool HitTest3D(const UGVector3d& vecStart, const UGVector3d& vecEnd, UGdouble dTolerance) const;	
	
	//! \brief 缩放
	//! \param dRatioX [in] X方向缩放比率。
	//! \param dRatioY [in] Y方向缩放比率。
	//! \remarks 转成2维的Geometry后，调用它的方法实现。
	virtual void Inflate( UGdouble dRatioX, UGdouble dRatioY);	
	
	//! \brief 缩放
	//! \param dRatio [in] 缩放比率(X,Y方向相同)。
	virtual void Inflate( UGdouble dRatio);	
	
	//! \brief 平移。
	//! \param dX [in] 平移的X偏移量。
	//! \param dY [in] 平移的Y偏移量。
	virtual void Offset( UGdouble dX, UGdouble dY);	
	
	//! \brief 平移。
	//! \param dOffset [in] 偏移量(X,Y方向相同)。
	virtual void Offset( UGdouble dOffset);	
	
	//! \brief 平移
	//! \param szOffset [in] 平移的偏移量(分别用UGSize2D中的x,y代表X,Y方向的偏移量)。
	virtual void Offset( const UGSize2D &szOffset);	
	
	//! \brief 旋转
	//! \param pntOrigin [in] 旋转的中心点(锚点,不动点)。
	//! \param dAngle [in] 旋转角度。
	virtual void Rotate(const UGPoint2D& pntOrigin, UGdouble dAngle);
	
	//! \brief 旋转,增加一些参数,以便内部可以不用进行cos,sin, 在批量处理的时候有利于提高效率。
	//! \param pntOrigin [in] 旋转的中心点(锚点,不动点)。
	//! \param dCosAngle [in] 旋转的cos值。
	//! \param dSinAngle [in] 旋转的sin值。
	//! \param dAngle [in] 旋转角度。
	//! \remarks dAngle看似多余,但GeoText需要,所以要保留。
	virtual void Rotate( const UGPoint2D& pntOrigin, UGdouble dCosAngle, UGdouble dSinAngle,UGdouble dAngle = 0);	
	
	//! \brief 调整几何数据,限定在新的Bounds中
	//! \param rcNewBounds [in] 新的,限定范围的Bounds
	//! \return 成功返回true,失败返回false
	virtual UGbool Resize(const UGRect2D& rcNewBounds);

	//! \brief 两点镜像
	//! \param pntMirror1 [in] 构成镜面的第一个点
	//! \param pntMirror2 [in] 构成镜面的第二个点
	//! \return 成功返回true,失败返回false
	virtual UGbool Mirror(const UGPoint2D &pntMirror1, const UGPoint2D &pntMirror2);

	//! \brief 三点镜像
	//! \param pntMirror1 [in] 构成镜面的第一个点
	//! \param pntMirror2 [in] 构成镜面的第二个点
	//! \param pntMirror3 [in] 构成镜面的第三个点
	//! \return 成功返回true,失败返回false
	virtual UGbool Mirror3D(const UGPoint3D &pntMirror1, const UGPoint3D &pntMirror2, const UGPoint3D &pntMirror3);//liuy 2009-5-20

	//! \brief 镜像
	//! \param vecNormal [in] 镜面的法线向量	
	//! \return 成功返回true,失败返回false
	virtual UGbool Mirror3D(const UGVector3d &vecNormal); //liuy 2009-5-20

	
	
public:
	// 以下是Geometry3D相对应的方法...

	//! \brief 三维对象缩放
	//! \param dRatioX [in] X方向缩放比率
	//! \param dRatioY [in] Y方向缩放比率
	//! \param dRatioZ [in] Z方向缩放比率
	virtual void Inflate3D( UGdouble dRatioX, UGdouble dRatioY, UGdouble dRatioZ);

	//! \brief 三维对象缩放
	//! \param dRatio [in] 缩放比率(X,Y,Z方向相同)	
	virtual void Inflate3D( UGdouble dRatio);

	//! \brief 三维对象平移
	//! \param dX [in] 平移的X偏移量
	//! \param dY [in] 平移的Y偏移量	
	//! \param dZ [in] 平移的Z偏移量	
	virtual void Offset3D( UGdouble dX, UGdouble dY, UGdouble dZ);

	//! \brief 三维对象平移
	//! \param dOffset [in] 偏移量(X,Y,Z方向相同)	
	virtual void Offset3D( UGdouble dOffset);

	//! \brief 三维对象平移
	//! \param vecOffset [in] 平移的偏移量(分别用UGVector3d中的x,y,z代表X,Y,Z方向的偏移量)。
	virtual void Offset3D( const UGVector3d &vecOffset);	

	//! \brief 三维对象旋转
	//! \param pntOrigin [in] 旋转的中心点(锚点,不动点)
	//! \param dAngle [in] 旋转角度
	virtual void Rotate3D(const UGPoint3D& pntOrigin, UGdouble dAngle);
	
	//! \brief 三维对象旋转,增加一些参数,以便内部可以不用进行cos,sin, 在批量处理的时候有利于提高效率
	//! \param pntOrigin [in] 旋转的中心点(锚点,不动点)
	//! \param dCosAngle [in] 旋转的cos值
	//! \param dSinAngle [in] 旋转的sin值
	//! \param dAngle [in] 旋转角度	
	//! \remarks dAngle看似多余,但GeoText需要,所以要保留
	virtual void Rotate3D( const UGPoint3D& pntOrigin, UGdouble dCosAngle, UGdouble dSinAngle, UGdouble dAngle = 0);

	//! \brief 三维对象旋转
	//! \param vecAxis [in] 旋转轴 过原点
	//! \param dAngle [in] 旋转角度
	virtual void Rotate3D(const UGVector3d& vecAxis, UGdouble dAngle);

	//! \brief 三维对象调整几何数据，限定在新的BoundingBox中
	//! \param rcNewBox [in] 新的,限定范围的BoundingBox
	//! \return 成功返回true,失败返回false	
	virtual UGbool Resize3D(const UGBoundingBox& rcNewBox);

	//! \brief 只保存几何数据部分。
	//! \param stream [in] 存储流, 调用之前应该先Open。
	//! \param eCodecType [in] 存储的编码方式。
	//! \param FALSE [in] 是否存储为SDBPlus的格式。
	//! \return 成功返回true,失败返回false。
	//! \remarks 由于SDBPlus引擎和数据库引擎的存储上差别较大,所以必须直接调用该函数,并增加变量进行分别处理。
	//! \attention 被SDBPlus所迫,不得不public之。
	virtual UGbool SaveGeoData(UGStream& stream, 
		UGDataCodec::CodecType eCodecType,UGbool bSDBPlus = FALSE) const;

	//! \brief 只转载几何数据部分
	//! \param stream [in] 流 
	//! \param eCodecType [in] 编码方式
	//! \attention 还需要增加 UGbool bSDBPlus = FALSE 的参数
	//! \return 成功返回true,失败返回false	
	virtual UGbool LoadGeoData( UGStream& stream , 
		UGDataCodec::CodecType eCodecType = UGDataCodec::encNONE,UGbool bSDBPlus = FALSE );

	//! \brief 将Geometry生成XML字符串
	//! \param versionType [in] GML的版本
	//! \param nPace [in] 缩进的空格数目,使得xml字符串更加易于人工阅读; 
	//! 主要用于如果返回的xml字符串是一个大标签中的一部分, 通过指定空格数目,可以使层次清晰
	//! \return 返回指定GML版本的xml字符串
	//! \remarks 转成2维的Geometry后，调用它的方法实现
	virtual UGString ToXML(GMLVersionType versionType, UGint nPace = 0)  const;

	//! \brief 从XML字符串中生成Geometry
	//! \param strXML [in] xml格式字符串
	//! \param versionType [in] xml字符串的GML版本
	//! \return 返回指定GML版本的xml字符串
	virtual UGbool FromXML(const UGString& strXML, GMLVersionType versionType);

	//! \brief 将三维几何对象生成XML字符串
	//! \param versionType [in] GML的版本
	//! \param nPace [in] 缩进的空格数目,使得xml字符串更加易于人工阅读; 
	//! 主要用于如果返回的xml字符串是一个大标签中的一部分, 通过指定空格数目,可以使层次清晰
	//! \return 返回指定GML版本的xml字符串
	virtual UGString ToXML3D(GMLVersionType versionType, UGint nPace = 0)  const;

	//! \brief 从XML字符串中生成三维Geometry
	//! \param strXML [in] xml格式字符串
	//! \param versionType [in] xml字符串的GML版本
	//! \return 返回指定GML版本的xml字符串	
	virtual UGbool FromXML3D(const UGString& strXML, GMLVersionType versionType);
#if !defined (OPENGL_ES_VERSION2)
	//! \brief Geometry与Element的转换
	//! \param pElement [in] 返回的pElement指针在函数内分配内存,外面负责删除，否则会有内存泄漏
	//! \remarks 转成2维的Geometry后，调用它的方法实现
	virtual UGbool ToElement(OgdcElement*& pElement)  const;
	
	//! \brief Geometry与Element的转换
	//! \param pElement [in] 传入的Element，如果无法转换则返回False，否则返回True
	virtual UGbool FromElement(const OgdcElement* pElement);

	//! \brief Geometry3D与Element的转换
	//! \param pElement [in] 返回的pElement指针在函数内分配内存,外面负责删除，否则会有内存泄漏
	virtual UGbool ToElement3D(OgdcElement*& pElement)  const;

	//! \brief Geometry3D与Element的转换
	//! \param pElement [in] 传入的Element，如果无法转换则返回False，否则返回True	
	virtual UGbool FromElement3D(const OgdcElement* pElement);
#endif
	//! \brief 规则几何体对象三角化后生成几何模型。
	//! \param nStacks [in] 横向分几份。
	//! \param nSlices [in] 竖向分几段。
	//! \return 几何模型指针。
	virtual UGGeoModel* GetModel(UGint nStacks, UGint nSlices);

	//! \brief 参数化对象对象三角化后生成几何模型。
	//! \return 几何模型指针。
	virtual UGGeoModelPro* Convert2GeoModelPro(
		UGuint nResolution = 0, 
		UGuint nFragmentSize = 2, 
		UGuint nFragmentAngle = 12) const;

	//! \brief 获取模型三角面化的模型顶点和索引。
	//! \param pVertexDataPackage [out] 模型顶点数组。
	//! \param arrIndexPackage [out] 模型索引。
	//! \param nResolution [in] 分辨率。
	//! \param nFragmentSize [in] 片段大小，单位毫米。
	//! \param nFragmentAngle [in] 片段角度，单位度。
	//! \attention 传入参数pVertexDataPackage不能为NULL。
	//! \remarks 在UGGeoBox\UGGeoPyramid类中三参数无意义。
	virtual void GetTriMesh(
		UGVertexDataPackage* pVertexDataPackage, 
		UGArray<UGIndexPackage*>& arrIndexPackage, 
		UGuint nResolution = 0, 
		UGuint nFragmentSize = 2, 
		UGuint nFragmentAngle = 12) const;
	virtual void GetTriMesh(
		UGVertexDataPackageExact* pVertexDataPackage, 
		UGArray<UGIndexPackage*>& arrIndexPackage, 
		UGuint nResolution = 0, 
		UGuint nFragmentSize = 2, 
		UGuint nFragmentAngle = 12) const;

	//! \brief 获取模型构建的多面体集合。
	//! \param nResolution [in] 分辨率。
	//! \param nFragmentSize [in] 片段大小，单位毫米。
	//! \param nFragmentAngle [in] 片段角度，单位度。
	//! \attention 使用后需要用户释放内存。
	virtual UGPolySet* GetPolySet(
		UGuint nResolution = 0, 
		UGuint nFragmentSize = 2, 
		UGuint nFragmentAngle = 12) const;

	//! \brief 由建模对象构建Geometry。
	virtual UGbool MakeWithPolySet(const UGPolySet* pPolySet) { return FALSE; }

	//! \brief 曲面细分。
	//! \param dGranularity [in] 细分粒度。顶点为经纬度坐标时，参数默认为PI/180.0度；顶点为平面坐标时，参数默认为1.0米。
	//! \param bLonLat [in] 顶点坐标是否是经纬度。
	virtual UGbool ComputeSubDivision(UGdouble dGranularity, UGbool bLonLat) { return FALSE; }

public:
	
	// 编辑相关函数,把编辑和geometry统一...
	
	//! \brief 获得编辑对象的句柄的数目
	//! \remarks 对于固定句柄对象为9包括了改变大小的8个控制点和一个旋转控制点
	//! 转成2维的Geometry后，调用它的方法实现
	virtual UGint  GetHandleCount();
	
	//nHandle:的值与控制点的关系如右图                         
	//									1---+   	  +--2    	+----3	
	//                                      |         |         |  
	//                                      |         |         |  
	//										x---------x---------x     9
    //										|   ____________    |     |
    //										|  /            \   | x---+
	//									4---x |              |  x-----5
	//										|  \____________/   |
	//										|                   |
	//										x---------x---------x
	//										|		  |         |
	//										|		  |         |
	//								    6---+         +-----7   +-----8
	
	//! \brief 根据句柄获得相应控制点的值
	//! \param nHandle [in] 控制点句柄
	//! \param point [in] 句柄对应的控制点的坐标值
	//! \return 返回辅助点的类型
	//! \remarks 转成2维的Geometry后，调用它的方法实现
	virtual UGint GetHandle( UGint nHandle, UGPoint2D& point);
	
	//! \brief 移动控制点到指定坐标,主要用于绘制对象，修改对象节点等操作
	//! \param nHandle [in] 控制点句柄
	//! \param point [in] 控制点坐标
	//! \remarks 不能转成2维的Geometry后，调用它的方法实现
	virtual void MoveHandle( UGint nHandle, const UGPoint2D& pnt2D,
		UGEditType::EditToolShape nEditType=UGEditType::ET_Default);

	//! \brief 节点编辑时可用来插入一个控制点,也可以用于绘制对象时加入节点
	//! \param nHandle [in] 插入位置
	//! \param pnt2D [in] 插入点的地理坐标
	//! \remarks 不能转成2维的Geometry后，调用它的方法实现
	virtual void AddHandle( UGint nHandle, const UGPoint2D& pnt2D,
		UGEditType::EditToolShape nEditType=UGEditType::ET_Default);

	//! \brief 删除编辑对象的指定节点
	//! \param nHandle [in] 节点句柄
	//! \remarks 不能转成2维的Geometry后，调用它的方法实现
	virtual UGuint DelNode( UGint nHandle );

	//! \brief 计算对象绘制时的辅助线，返回的是一个复合对象
	//! \param pPoints [in] 点串
	//! \param nPntCount [in] 点的个数
	//! \param nEditType [in] 绘制对象的类型，参考UGEditType
	//! \param pStyle [in] 辅助线的风格，如果传入为NULL，用默认的风格
	//! \remarks 转成2维的Geometry后，调用它的方法实现
	virtual UGGeometry*  CalAssistantLine(const UGPoint2D* pPoints,
		UGint nPntCount,UGEditType::EditToolShape nEditType,UGStyle* pStyle=NULL);
	
	//! \brief 计算对象绘制时的辅助点，返回的是点的数目,
	//! nHandleType返回的是辅助点的类型,参考UGEditType::EditHandleType
	//! \param pPoints [in] 点串
	//! \param nPntCount [in] 点的个数
	//! \param eAssistantPoints [in] 点串,返回的辅助点点串
	//! \param nEditType [in] 绘制对象的类型，参考UGEditType
	//! \remarks 转成2维的Geometry后，调用它的方法实现
	virtual void CalAssistantPoints(const UGPoint2D* pPoints, UGint nPntCount,
		UGArray<UGEditType::EditAssistantPoint>& eAssistantPoints,
		UGEditType::EditToolShape nEditType);
	
	//! \brief 根据传入的点串生成对象，主要用于编辑中的参数化等没有内部数组保存点串的对象，
	//! 也用于自定义对象,对于有内部数组保存点串的对象，该函数直接return既可，
	//! 比如GeoLine等对象就直接用AddHandle来生成，效率比较高 
	//! \param pPoints [in] 点串
	//! \param nPntCount [in] 点的个数
	//! \remarks 不能转成2维的Geometry后，调用它的方法实现
	virtual void MakeWithPoints(UGPoint2D* pPoints,UGint nPntCount,
		UGEditType::EditToolShape nEditType=UGEditType::ET_Default);

	//! \brief 根据已有几何对象的指针，拷贝构造一个三维几何对象。
	//! \param pGeometry [in] 已有几何对象指针。
	//! \return 构造成功返回true。
	//! \remarks 根据geometry类型，内部调用各自的make方法去实现。
	virtual UGbool MakeWithGeometry(const UGGeometry* pGeometry);	

	//! \brief 根据三维点串构造一个三维几何对象
	//! \param pPoints [in] 点串指针。
	//! \param nPntCount [in] 点的个数。
	//! \param UGEditType3D::ET3D_NULL [in] 编辑工具形状（可选）。
	//! \return 构造成功返回true。
	//! \remarks 根据geometry类型，内部调用各自的make方法去实现，用于在编辑中多态构造对象。
	virtual UGbool MakeWithPoints3D(UGPoint3D* pPoints,UGint nPntCount,
		UGEditType3D::EditToolShape3D nEditType = UGEditType3D::ET3D_NULL);	

	//! \brief 获取对象需要捕捉的点串
	//! \param aryPnts [in] 点串，传出去的要捕捉的点串
	//! \return 如果是true表示有需要捕捉的点串，如果返回false表示该对象没有需要捕捉的点串
	//! \remarks 转成2维的Geometry后，调用它的方法实现
	virtual UGbool GetSnapPoints(UGArray<UGPoint2D>& aryPnts);
	
	//! \brief 获取对象需要捕捉的线段数组
	//! \param aryLines [in] 需要捕捉的线数组（如果有多个part，那么aryLines就应该有多少个count）
	//! \return 如果是true表示有需要捕捉的线，如果返回false表示该对象没有需要捕捉的线
	//! \remarks 转成2维的Geometry后，调用它的方法实现
	virtual UGbool GetSnapLines(UGLineArray& aryLines);
	
	//! \brief 编辑（拉伸，编辑节点）结束后，通知geometry，对你的编辑结束了，
	//! 主要针对geomap这样的对象，因为需要在结束编辑后去刷新地图内容
	//! \remarks 转成2维的Geometry后，调用它的方法实现
	virtual void AfterModified();
	
	//! \brief 设置几何对象中字符串的Charset,主要针对文本对象，复合对象中的文本字符串,只set，不转换字符串内容
	//! \param nCharset [in] 字符编码类型。
	//! \remarks 转成2维的Geometry后，调用它的方法实现
	virtual void SetTextCharset(UGString::Charset nCharset);	
	
	//! \brief 改变几何对象中字符串的Charset,主要针对文本对象，复合对象中的文本字符串,转换字符串内容
	//! \param nCharset [in] 字符编码类型。
	//! \remarks 转成2维的Geometry后，调用它的方法实现
	virtual void ChangeTextCharset(UGString::Charset nCharset);

public:		
	//! \brief 得到模型体积。
	//! \return 笛卡尔坐标系下的体积。
	virtual UGdouble GetVolume();	

	//! \brief 得到对象面积
	//! \attention Dimension为2的对象才支持
	//! \return 返回面积
	//! \remarks 转成2维的Geometry后，调用它的方法实现
	virtual UGdouble GetArea() const;

	//! \brief 得到三维对象的表面积
	//! \attention Dimension为2的对象才支持
	//! \return 返回面积
	virtual UGdouble GetArea3D() const;
	
	//! \brief 得到对象长度
	//! \attention Dimension为1or2的对象才支持
	//! \return 返回长度
	//! \remarks 转成2维的Geometry后，调用它的方法实现
	virtual UGdouble GetLength() const;

	//! \brief 得到三维对象长度
	//! \attention Dimension为1or2的对象才支持
	//! \return 返回长度
	virtual UGdouble GetLength3D() const;
	
	//! \brief 得到三维对象长度。
	//! \pjType 坐标类型
	//! \return 返回长度。
	virtual UGdouble GetLength3D(EmPrjCoordSysType pjType);

	//! \brief 得到子对象的个数
	//! \return 返回子对象的个数
	//! \remarks 参考2维实现
	virtual UGint GetSubCount() const;

public:	
	//! \brief 设置三维几何对象的风格。
	//! \param pStyle3D [in] 要设置的风格的指针，内部会拷贝一份，外面如果new的话记得释放。
	virtual void SetStyle3D(const UGStyle3D* pStyle3D);		
	
	//! \brief 得到三维几何对象的风格。
	//! \return 得到三维几何对象的风格的指针。
	virtual UGStyle3D* GetStyle3D() const;	

	//! \brief 设置对象的位置，这个函数只对模型和规则几何体意义，位置为模型等物体的底面中心点
	//! x,y为经纬度，z是指到球面的距离而不是到球心的距离
	//! \param value [in] 指定的位置点，含义见上。
	virtual void SetPosition(const UGPoint3D& value);	

	//! \brief 设置对象的缩放大小，这个函数只对模型和规则几何体意义
	//! \param fScaleX [in] X方向缩放比率。
	//! \param fScaleY [in] Y方向缩放比率。
	//! \param fScaleZ [in] Z方向缩放比率。
	virtual void SetScale(UGdouble fScaleX,UGdouble fScaleY,UGdouble fScaleZ);	

	//! \brief 设置对象的缩放大小，这个函数只对模型和规则几何体意义 
	//! \param fScaleXYZ [in] 物体在三个方向上进行同样大小的缩放 
	virtual void SetScale(UGdouble fScaleXYZ);

	//! \brief 设置对象的旋转量，这个函数只对模型和规则几何体意义 
	//! \param fRotateX [in] X方向的旋转角度。
	//! \param fRotateY [in] Y方向的旋转角度。
	//! \param fRotateZ [in] Z方向的旋转角度。
	virtual void SetRotation(UGdouble fRotateX,UGdouble fRotateY,UGdouble fRotateZ);	

	//! \brief 得到对象的位置，这个函数只对模型和规则几何体意义，位置为模型等物体的底面中心点
	//! \return 对象的位置。
	virtual UGPoint3D GetPosition() const;

	//! \brief 得到对象各方向的缩放比率，这个函数只对模型和规则几何体有意义
	//! \return 对象各方向的缩放比率。
	UGPoint3D GetScale() const;	
	

	//! \brief 得到对象各方向的旋转角度，这个函数只对模型和规则几何体有意义
	//! \return 对象各方向的旋转角度。
	UGPoint3D GetRotation() const;

	//! \brief 拷贝Geometry基类中的成员变量, 包括ID, Style, Bounds,以及Geometry3D中的成员变量,包括Style3D, BoundingBox, Point3D对象等
	//! \param geo3d [in] 提供信息的Gemetry3D
	//! \return 如果有对自身的拷贝，或者有指针为空，则返回false
	UGbool CopyAllBaseData(const UGGeometry3D& geo3d);

	//! \brief 设置三维对象是否改变，旋转、平移、缩放的时候这个变量会变为True
	//! \param bModified [in] 设置的状态。
	void SetModified(UGbool bModified);	
	
	//! \brief 得到三维对象是否改变
	//! \return 得到状态
	UGbool IsModified();

protected:

	//! \brief 真正计算Bounds
	//! \return 返回得到的Bounds
	//! \remarks 转成2维的Geometry后，调用它的方法实现
	virtual UGRect2D ComputeBounds() const;	

	//! \brief 真正计算BoundingBox
	//! \return 返回得到的BoundingBox
	virtual UGBoundingBox ComputeBoundingBox() const;

public:
	//! \brief 转化为线对象
	//! \param geoLine [out] 得到的线对象
	//! \param nSegmentCount [in] 当曲线转化为线时,每一段用多少个点来模拟
	//! \return 成功返回true,失败返回false
	virtual UGbool ConvertToLine( UGGeoLine &geoLine, UGint nSegmentCount = 0 ) const;
	
	//! \brief 转化为面对象
	//! \param geoRegion[out] 得到的面对象
	//! \param nSegmentCount [in] 当曲面转化为面时,每一段用多少个点来模拟
	//! \return 成功返回true,失败返回false
	virtual UGbool ConvertToRegion( UGGeoRegion &geoRegion, UGint nSegmentCount = 0 ) const;

	//! \brief 模型获取基底面
	virtual UGbool ConvertToRegion(EmSpatialRefType prjCoordType, UGGeoRegion &geoRegion, UGint nSegmentCount  = 0 ) const ;
	
	//! \brief 转换成多点对象，可以把3D点、3D多点对象转换成多点对象显示。
	//! \param geoMultiPoint [out] 输出多点几何对象。
	//! \return 成功返回true,失败返回false
	virtual UGbool ConvertToPoint( UGGeoMultiPoint &geoMultiPoint) const;	
	
	//! \brief 转换成图片，3D类型几何对象，以及用户类型几何对象可以通过先转换成图片，再显示。
	//! \param geoPicture [out] 输出图片几何对象。
	//! \param dResolution [in] 分辨率。
	//! \param dXAngle [in] 水平倾角。
	//! \param dZAngle [in] 旋转倾角。
	//! \return 成功返回true,失败返回false
	virtual UGbool ConvertToPicture( UGGeoPicture &geoPicture, UGdouble dResolution, 
								UGdouble dXAngle = 0, UGdouble dZAngle = 0) const;	
	
	//! \brief 转换成相对应的2维Geometry对象。
	//! \param geometry [out] 输出2维Geometry对象	
	//! \return 成功返回true,失败返回false
	virtual UGbool ConvertToGeometry2D( UGGeometry& geometry ) const;

public:
	//Projection..
	
	//! \brief 根据投影转化器的设置和转化方向要求, 进行投影转化
	//! \param pPJTranslator [in] 投影转化器
	//! \param bForward [in] 转化方向, true代表从UGRefTranslator的源(Src)转化到目的(Des), false则相反
	//! \remarks 转成2维的Geometry后，调用它的方法实现
	virtual void PJConvert( UGRefTranslator *pPJTranslator, UGbool bForward = true);

	//! \brief 进行三维投影坐标系统转化
	//! \attention 暂未实现。
	virtual void PJConvert3D( UGRefTranslator *pPJTranslator, UGbool bForward = true);	
	
	//! \brief 从经纬坐标系统转化为投影坐标系统
	//! \param pCoordSys [in] 投影坐标系统
	//! \remarks 转成2维的Geometry后，调用它的方法实现
	virtual void PJForward( UGPrjCoordSys *pCoordSys );

	//! \brief 从经纬坐标系统转化为三维投影坐标系统
	//! \attention 暂未实现。
	virtual void PJForward3D( UGPrjCoordSys *pCoordSys );
	
	//! \brief 从投影坐标系统转化为经纬坐标
	//! \param pCoordSys 投影坐标系统
	//! \remarks 转成2维的Geometry后，调用它的方法实现
	virtual void PJInverse( UGPrjCoordSys *pCoordSys );	

	//! \brief 从投影坐标系统转化为三维坐标系统
	//! \attention 暂未实现。
	virtual void PJInverse3D( UGPrjCoordSys *pCoordSys );	

	//! \brief 设置三维几何对象的包围盒。
	//! \param box [in] 新包围盒的值。
	virtual void SetBoundingBox(const UGBoundingBox& box)	{m_BoundingBox = box;};

	//! \brief 得到三维几何对象的包围盒。
	//! \return 返回包围盒的值。
	// virtual const UGBoundingBox& GetBoundingBox()	const;
	virtual const UGBoundingBox& GetBoundingBox()	const;

public:
#ifndef ANDROID_2D
	//! \brief 存储额外的附加数据,不和Geometry存储在一起的数据
	//! \brief 模型里边存储纹理和模型分开
	//! \return 成功返回TRUE,失败返回FLASE
	virtual UGbool SaveExtData(UGArray<UGTextureInfo>& arryTextureInfo);
#endif
	//! \brief 通过几何对象中保存的纹理路径初始化纹理信息
	//! \return 成功返回TRUE,失败返回FLASE
	virtual UGbool InitialTextureInfo(UGArray<UGTextureInfo>& arrayTextureInfos);
#ifndef ANDROID_2D
	//! \brief 加载额外纹理数据到全局的管理类中 和 UGGeoModel
	//! \return 成功返回TRUE,失败返回FLASE
	virtual UGbool LoadExtData(const UGArray<UGTextureInfo>& arrayTextureInfos);
#endif
protected:
	UGbool IsInLatLong() const; 
protected:

	//! \brief 三维几何对象的包围盒
	UGBoundingBox m_BoundingBox;

	//! \brief 三维对象的位置，对规则几何体，模型，粒子有效
	UGPoint3D m_pntPos;

	//! \brief 三维对象的缩放系数，对规则几何体，模型，粒子有效
	UGPoint3D m_pntScale;

	//! \brief 三维对象的旋转量，对规则几何体，模型，粒子有效
	UGPoint3D m_pntRotate;
};
}

#endif // !defined(AFX_UGGEOMETRY3D_H__0D8126C6_CAE8_4AD5_91D7_47DF8BD75819__INCLUDED_)


