//////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file UGGeoLine3D.h
//!  \brief 三维线几何对象。
//!  \author UGCTeam。
//!  \attention 
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_UGGEOLINE3D_H__C9F9464F_CCB3_4297_B3CB_B7D5114B9CBE__INCLUDED_)
#define AFX_UGGEOLINE3D_H__C9F9464F_CCB3_4297_B3CB_B7D5114B9CBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Geometry3D/UGGeometry3D.h"
#include "Base3D/UGRenderOperationGroup.h"

namespace UGC
{
class UGRenderOperationGeometry;

//! \brief 内部使用的保存线的阴影体单个顶点数据的结构
struct LineShadowVolumeVertex
{
	// 阴影体顶点数据
	UGVector3d Vertex;
	// 阴影体顶点处与走向垂直的水平方向
	UGVector3d CrossDir;
	// 阴影体顶点处垂直拉伸方向
	UGVector3d VerticalDir;
};

class UGGeoLine;
class UGGeoLineEPS;
//! \brief 三维线几何对象
class GEOMETRY3D_API UGGeoLine3D : public UGGeometry3D
{
public:
	
	//! \brief 缺省构造函数。
	UGGeoLine3D();	

	//! \brief 默认析构函数。
	virtual ~UGGeoLine3D();	

public:
	
	//! \brief 得到维度。
	//! \return 返回维度，点为0维, 线/曲线等为1维, 面/曲面等为2维, GeoCompound/GeoText等为-1。
	virtual UGint GetDimension() const;	
	
	//! \brief 得到内点（在Geometry内部，靠近中央的点）
	//! \remarks 为点数最多的子对象 的中间点(中间个数的点, 而不是中间距离的点)
	virtual UGPoint2D GetInnerPoint() const;

	//! \brief 得到对象面积
	//! \return 返回面积
	virtual UGdouble GetArea() const;
	
	//! \brief 得到对象面积
	//! \return 返回面积
	virtual UGdouble GetArea3D() const;
	
	//! \brief 选择，用于三维线对象的点选。
	//! \param &pntHitTest [in] 用来测试选中的点。
	//! \param dTolerance [in] 容限。
	//! \return 选中返回true, 否则返回false。
	//! \remarks 采用二维的HitTest()进行判断。
	virtual UGbool HitTest( const UGPoint2D &pntHitTest, UGdouble dTolerance) const;	
	
	//! \brief 几何对象是否有效
	//! \return 有效返回true,无效返回false
	//! \remarks 不是转成2维的Geometry后，调用它的方法实现，但参考2维的实现。
	virtual UGbool IsValid()const;

	//! \brief 根据已有线对象的指针，拷贝构造一个Geoline3D。
	//! \param pGeometry [in] 已有线对象指针。
	//! \return 构造成功返回true。
	//! \remarks 内部调用make方法去实现。
	virtual UGbool MakeWithGeometry(const UGGeometry* pGeometry);	

	//! \brief 根据三维点串构造一个GeoLine
	//! \param pPoints [in] 点串指针。
	//! \param nPntCount [in] 点的个数。
	//! \param UGEditType3D::ET3D_NULL [in] 编辑工具形状（可选）。
	//! \return 构造成功返回true。
	//! \remarks 内部调用make方法去实现，用于在编辑中多态构造对象GeoLine。
	virtual UGbool MakeWithPoints3D(UGPoint3D* pPoints,UGint nPntCount,
		UGEditType3D::EditToolShape3D nEditType = UGEditType3D::ET3D_NULL);	

	//! \brief 根据传入的点串生成对象，主要用于编辑中的参数化等没有内部数组保存点串的对象，
	//! 也用于自定义对象,对于有内部数组保存点串的对象，该函数直接return既可，
	//! 比如GeoLine等对象就直接用AddHandle来生成，效率比较高 
	//! \param pPoints 3维点串
	//! \param nPntCount  点的个数
	virtual void MakeWithPoints(UGPoint3D* pPoints,UGint nPntCount,
				UGEditType::EditToolShape nEditType=UGEditType::ET_Default);

	//! \brief 得到对象的位置，这个函数只对模型和规则几何体意义，位置为模型等物体的底面中心点
	//! \return 对象的位置。
	virtual UGPoint3D GetPosition() const;
	 
	//! \brief 获取数据存储时的大小
	//! \return 返回存储大小
	//! \remarks 暂未考虑编码情况，与SaveGeoData()一起维护
	//! \attention 字符串的长度要+4
	virtual UGint GetDataSize() const;

	//! \brief 获取模型构建的多面体集合。
	//! \param nResolution [in] 分辨率。
	//! \param nFragmentSize [in] 片段大小，单位毫米。
	//! \param nFragmentAngle [in] 片段角度，单位度。
	//! \attention 使用后需要用户释放内存。
	virtual UGPolySet* GetPolySet(
		UGuint nResolution = 0, 
		UGuint nFragmentSize = 2, 
		UGuint nFragmentAngle = 12) const;

	//! \brief 使用UGPolySet重新构造GeoRegion3D。
	virtual UGbool MakeWithPolySet(const UGPolySet* pPolySet);

	//! \brief 曲面细分。
	//! \param dGranularity [in] 细分粒度。顶点为经纬度坐标时，参数默认为PI/180.0度；顶点为平面坐标时，参数默认为1.0米。
	//! \param bLonLat [in] 顶点坐标是否是经纬度。
	virtual UGbool ComputeSubDivision(UGdouble dGranularity, UGbool bLonLat);

protected:
	//! \brief 真正计算Bounds
	//! \return 返回得到的Bounds
	virtual UGRect2D ComputeBounds() const;	

	//! \brief 只保存几何数据部分
	//! \param stream [in] 存储流, 调用之前应该先Open
	//! \param eCodecType [in] 存储的编码方式
	//! \param bSDBPlus [in] 是否存储为SDBPlus的格式
	//! \remarks 由于SDBPlus引擎和数据库引擎的存储上差别较大,所以必须直接调用该函数,
	//! 并增加变量进行分别处理
	//! \return 成功返回true,失败返回false
	virtual UGbool SaveGeoData(UGStream& stream, 
		UGDataCodec::CodecType eCodecType,UGbool bSDBPlus = FALSE) const;
	
	//! \brief 只转载几何数据部分
	//! \param stream [in] 流 
	//! \param eCodecType [in] 编码方式
	//! \attention [in] 还需要增加 UGbool bSDBPlus = FALSE 的参数
	//! \return 成功返回true,失败返回false
	virtual UGbool LoadGeoData( UGStream& stream , 
	UGDataCodec::CodecType eCodecType = UGDataCodec::encNONE,UGbool bSDBPlus = FALSE );

	
	//!\brief 得到Boundingbox
	virtual UGBoundingBox ComputeBoundingBox() const;

public:


	//! \brief 缩放
	//! \param dRatioX X方向缩放比率
	//! \param dRatioY Y方向缩放比率
	//! \param dRatioY Z方向缩放比率
	//! \remarks M无缩放
	virtual void Inflate3D( UGdouble dRatioX, UGdouble dRatioY, UGdouble dRatioZ);
 	
	//! \brief 缩放
	//! \param dRatio 缩放比率(X,Y,Z相同)
	virtual void Inflate3D( UGdouble dRatio);

	//! \brief 缩放
	//! \param dRatioX [in] X方向缩放比率。
	//! \param dRatioY [in] Y方向缩放比率。
	virtual void Inflate( UGdouble dRatioX, UGdouble dRatioY);	

	//! \brief 平移
	//! \param dX 平移的X偏移量
	//! \param dY 平移的Y偏移量
	virtual void Offset( UGdouble dX, UGdouble dY);
	
	//! \brief 平移
	//! \param 偏移量(X,Y方向相同)
	virtual void Offset( UGdouble dOffset);
	
	//! \brief 平移
	//! \param 平移的偏移量(分别用UGSize2D中的x,y代表X,Y方向的偏移量)
	virtual void Offset( const UGSize2D &szOffset);

	//! \brief 旋转
	//! \param pntOrigin 旋转的中心点(锚点,不动点)
	//! \param dAngle 旋转角度
	virtual void Rotate(const UGPoint2D& pntOrigin, UGdouble dAngle);

	//! \brief 旋转,增加一些参数,以便内部可以不用进行cos,sin, 在批量处理的时候有利于提高效率
	//! \remarks dAngle看似多余,但GeoText需要,所以要保留
	//! \param pntOrigin 旋转的中心点(锚点,不动点)
	//! \param dCosAngle 旋转的cos值
	//! \param dSinAngle 旋转的sin值
	//! \param dAngle 旋转角度
	virtual void Rotate( const UGPoint2D& pntOrigin, UGdouble dCosAngle, UGdouble dSinAngle, double dAngle = 0);

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
	//! \param vecAxis [in] 旋转轴 过原点
	//! \param dAngle [in] 旋转角度
	virtual void Rotate3D(const UGVector3d& vecAxis, UGdouble dAngle);

	//! \brief 三点镜像
	//! \param pntMirror1 [in] 构成镜面的第一个点
	//! \param pntMirror2 [in] 构成镜面的第二个点
	//! \param pntMirror3 [in] 构成镜面的第三个点
	//! \return 成功返回true,失败返回false
	virtual UGbool Mirror3D(const UGPoint3D &pntMirror1, const UGPoint3D &pntMirror2, const UGPoint3D &pntMirror3);
	
	
	//! \brief 镜像
	//! \param vecNormal [in] 镜面的法线向量	
	//! \return 成功返回true,失败返回false
	virtual UGbool Mirror3D(const UGVector3d &vecNormal);

	//! \brief 得到对象长度。
	//! \attention Dimension为1or2的对象才支持。
	//! \return 返回长度。
	//! \remarks 转成2维的Geometry后，调用它的方法实现。
	virtual UGdouble GetLength() const;	
	
	//! \brief 得到三维对象长度。
	//! \attention Dimension为1or2的对象才支持。
	//! \return 返回长度。
	//! \remarks 内部还是按照2维的Geometry对长度的求算进行实现的。
	virtual UGdouble GetLength3D() const;

	//! \brief 得到三维对象长度。
	//! \pjType 坐标类型
	//! \return 返回长度。
	virtual UGdouble GetLength3D(EmPrjCoordSysType pjType);

	//! \brief 转化为线对象。
	//! \param geoLine[out] 得到的线对象。
	//! \param nSegmentCount 当曲线转化为线时,每一段用多少个点来模拟。
	//! \return 成功返回true,失败返回false。
	virtual UGbool ConvertToLine( UGGeoLine &geoLine, UGint nSegmentCount = 0 ) const;
	
	//! \brief 转化为面对象。
	//! \param geoRegion[out] 得到的面对象。
	//! \param nSegmentCount 当曲面转化为面时,每一段用多少个点来模拟。
	//! \return 成功返回true,失败返回false。
	virtual UGbool ConvertToRegion( UGGeoRegion &geoRegion, UGint nSegmentCount = 0 ) const;
	
	//! \brief 转换成多点对象，可以把3D点、3D多点对象转换成多点对象显示。
	//! \param geoMultiPoint [out] 输出多点几何对象。
	//! \return 成功返回true,失败返回false。
	virtual UGbool ConvertToPoint( UGGeoMultiPoint &geoMultiPoint) const;	
	
	//! \brief 转换成图片，3D类型几何对象，以及用户类型几何对象可以通过先转换成图片，再显示。
	//! \param geoPicture [out] 输出图片几何对象。
	//! \param dResolution [in] 分辨率。
	//! \param dXAngle [in] 水平倾角。
	//! \param dZAngle [in] 旋转倾角。
	//! \return 成功返回true,失败返回false
	virtual UGbool ConvertToPicture( UGGeoPicture &geoPicture, UGdouble dResolution, 
								UGdouble dXAngle = 0, UGdouble dZAngle = 0) const;	
		
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
	
	//! \brief 从投影坐标系统转化为经纬坐标
	//! \param pCoordSys 投影坐标系统
	//! \remarks 转成2维的Geometry后，调用它的方法实现
	virtual void PJInverse( UGPrjCoordSys *pCoordSys );

	//! \brief 从经纬坐标系统转化为三维投影坐标系统
	//! \param pCoordSys 投影坐标系统
	virtual void PJForward3D( UGPrjCoordSys *pCoordSys );
	
	//! \brief 从投影坐标系统转化为经纬坐标系
	//! \param pCoordSys 投影坐标系统
	virtual void PJInverse3D( UGPrjCoordSys *pCoordSys );

	//! \brief 生成XML格式的信息
	//! \param versionType GML的版本
	//! \param nPace 缩进的空格数目,使得xml字符串更加易于人工阅读; 
	//! 主要用于如果返回的xml字符串是一个大标签中的一部分, 通过指定空格数目,可以使层次清晰
	//! \return 返回指定GML版本的xml字符串
	virtual UGString ToXML(GMLVersionType versionType, UGint nPace = 0) const;
	
	//! \brief 从XML字符串中生成Geometry
	//! \param strXML xml格式字符串
	//! \param versionType xml字符串的GML版本
	//! \return 返回指定GML版本的xml字符串
	virtual UGbool FromXML(const UGString& strXML, GMLVersionType versionType);
#ifndef OPENGL_ES_VERSION2
	//! \brief Geometry与Element的转换
	//! \param pElement [in] 返回的pElement指针在函数内分配内存,外面负责删除，否则会有内存泄漏
	//! \remarks 转成3维的Geometry后，调用它的方法实现
	virtual UGbool ToElement(OgdcElement*& pElement)  const;


	//! \brief Geometry与Element的转换
	//! \param pElement [in] 传入的Element，如果无法转换则返回False，否则返回True
	virtual UGbool FromElement(const OgdcElement* pElement);
#endif
public:

	//! \brief 得到几何对象类型。
	//! \return 固定为 GeoLine3D。
	virtual UGGeometry::Type GetType() const;	

	//! \brief 获取线中心三维点。
	//! \return 三维点。
	virtual UGPoint3D GetInnerPoint3D() const;	

	//! \brief 清空地理数据
	virtual void Clear();

	//! \brief 得到指定子对象的点串的指针。
	//! \param nSubIndex [in] 子对象索引。
	//! \return 返回子对象的点串的指针。
	virtual const UGPoint3D* GetPoints(UGint nSubIndex=0) const;		
	
	//! \brief 构造一个子对象的GeoLine
	//! \param pPoints [in] 点串指针。
	//! \param nPntCount [in] 点串的点数目。
	//! \return 点数小于2个,返回false。
	virtual UGbool Make( const UGPoint3D* pPoints, UGint nPntCount );

	//! \brief 构造多个子对象的复杂GeoLine
	//! \param pPoints [in] 点串指针。
	//! \param pPolyCount [in] 子对象中点数的数组。
	//! \param nSubCount [in] 子对象数目。
	//! \return 所有子对象的点数都小于2个,返回false。
	virtual	UGbool Make( const UGPoint3D* pPoints, const UGint* pPolyCount, UGint nSubCount );	
	virtual	UGbool Make( const UGPoint2D* pPoints, const UGint* pPolyCount, UGint nSubCount );	

	//! \brief 拷贝构造GeoLine3D。
	//! \param geoLine3D [in] 已有线对象。
	//! \return 已有线对象非法, 返回false。
	virtual UGbool Make( const UGGeoLine3D& geoLine3D);

	//! \brief 构造GeoLine3D。
	//! \param geoLine [in] 已有线对象。
	//! \return 已有线对象非法, 返回false。
	virtual UGbool Make( const UGGeoLine& geoLine);

	//! \brief 构造GeoLine3D。
	//! \param geoLineEPS [in] 已有线对象。
	//! \return 已有线对象非法, 返回false。
	virtual UGbool Make( const UGGeoLineEPS& geoLineEPS);

	//! \brief 在最后增加一个子对象。
	//! \param pPoints [in] 点串指针。
	//! \param nPntCount [in] 点串的点数目。
	//! \return 点串个数小于2个, 返回false。
	UGbool AddSub( const UGPoint3D* pPoints, UGint nPntCount );

	//! \brief 插入子对象。
	//! \param nIndex [in] 要插入的位置索引。
	//! \param pPoints [in] 点串指针。
	//! \param nPntCount [in] 点串中点的个数。
	//! \return 点个数小于2或者索引位置非法,返回false。
	UGbool InsertSub(UGint nIndex, const UGPoint3D* pPoints, UGint nPntCount);	

	//! \brief 删除子对象。
	//! \param nIndex [in] 要删除的子对象的位置索引。
	//! \return 索引非法, 返回false。
	UGbool DelSub( UGint nIndex );	

	//! \brief 从GeoLine上找到距离开始点指定长度的点。
	//! \param dDistance [in] 指定的距离长度[in]。
	//! \return 找到的点。
	//! \remarks dDistance小于0时，会返回起始点；如果dDistance大于线的长度，返回线的终止点；
	//! 对于有多个子对象的线，在前一个子对象中dDistance没有用完，会在下一个子对象中继续找（使用剩余的dDistance）
	//! 直到dDistance用完，找到对应的点，或者返回最后一个子对象的终止点
	//! \attention 查找的点不一定是线的节点（vertex），可以是在线上，计算出来的点
	UGPoint3D FindPointOnLineByDistance(UGdouble dDistance);	

	//! \brief 逆转坐标点的次序
	//! \return 成功返回true,失败返回false
	//! \remarks 只逆转每个子对象中的点的次序,不会逆转子对象的顺序
	UGbool Reverse();

	//! \brief 得到子对象数目的数组指针。
	//! \return 返回子对象数目的数组指针。
	virtual const UGint* GetPolyCounts() const;	

	//! \brief 得到所有子对象的点的个数总和。
	//! \return 返回个数。
	virtual UGint GetPointCount() const;	

	//! \brief 得到子对象的个数
	//! \return 返回个数
	virtual UGint GetSubCount() const;

	//! \brief 得到某个子对象的点的个数。
	//! \param nSubIndex [in] 子对象索引。
	//! \return 返回个数。
	virtual	UGint GetSubPntCount(UGint nSubIndex) const;	

	//! \brief 在指定的位置上设置点的坐标。
	//! \param nIndex [in] 位置索引。
	//! \param point [in] 点坐标。
	//! \return 索引非法（即索引不在原始点串内）, 返回false。
	//! \remarks 所有子对象的点在一个整体的数组中, 本函数没有区分子对象, 从头开始计算索引。
	UGbool SetPoint( UGint nIndex, const UGPoint3D& point );	

	//! \brief 通过指定子线的索引和点在子线中的索引位置进行插入。
	//! \param nSubIndex [in] 子对象索引。
	//! \param nPointIndex [in] 要插入点的位置。
	//! \param &pnt [in] 要插入的点。
	UGbool InsertPoint(UGint nSubIndex, UGint nPointIndex, UGPoint3D &pnt);	


	// fansc 2012-11-8
	//! \brief 连接一个GeoLine3D
	//! \param pGeoLine3D 要被连接的GeoLine3D
	//! \remarks 两个GeoLine3D都必须只有一个子对象; 连接后形成一个连续的子对象
	//!			连接时,把两个子对象两头距离比较近的连接起来; 
	//! \return 成功返回true,失败返回false
	virtual UGbool Joint(UGGeoLine3D* pGeoLine3D);
	
	virtual UGint GetXYZAtPos(UGdouble dPos, UGPoint3D& pntXYZ);

	//! \brief 将一个geometry转换为一个GeoLine3D，并复制属性
	//! \return 成功返回true,失败返回false
	UGbool ConvertFromGeometry(UGGeometry* pGeometry);
public:
	//added by xielin ^_^ 编辑相关函数,把编辑和geometry统一
	
	//! \brief 获得编辑对象的句柄的数目
	//! \remarks 对于固定句柄对象为9包括了改变大小的8个控制点和一个旋转控制点
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
	//! \param nHandle 控制点句柄
	//! \param point 句柄对应的控制点的坐标值
	//! \return 返回辅助点的类型
	 virtual UGint GetHandle( UGint nHandle, UGPoint2D& point);


	//! \brief 移动控制点到指定坐标,主要用于绘制对象，修改对象节点等操作
	//! \param nHandle 控制点句柄
	//! \param point 控制点坐标
	virtual void MoveHandle( UGint nHandle, const UGPoint2D& pnt2D,UGEditType::EditToolShape nEditType=UGEditType::ET_Default);

	//! \brief 节点编辑时可用来插入一个控制点,也可以用于绘制对象时加入节点
	//! \param nHandle 插入位置
	//! \param pnt2D 插入点的地理坐标
	virtual void AddHandle( UGint nHandle, const UGPoint2D& pnt2D,UGEditType::EditToolShape nEditType=UGEditType::ET_Default);
	
	//! \brief 删除编辑对象的指定节点
	//! \param nHandle 节点句柄
     virtual UGuint DelNode( UGint nHandle );

	//! \brief 计算对象绘制时的辅助线，返回的是一个复合对象
	//! \param pPoints 点串
	//! \param nPntCount  点的个数
	//! \param nEditType 绘制对象的类型，参考UGEditType
	//! \param pStyle	 辅助线的风格，如果传入为NULL，用默认的风格
     virtual UGGeometry*  CalAssistantLine(const UGPoint2D* pPoints,UGint nPntCount,UGEditType::EditToolShape nEditType,UGStyle* pStyle=NULL);


	 //! \brief 计算对象绘制时的辅助点，返回的是点的数目,nHandleType返回的是辅助点的类型,参考UGEditType::EditHandleType
	//! \param pPoints 点串
	//! \param nPntCount  点的个数
	//! \param eAssistantPoints 点串,返回的辅助点点串
	//! \param nEditType 绘制对象的类型，参考UGEditType
	void CalAssistantPoints(const UGPoint2D* pPoints,UGint nPntCount,UGArray<UGEditType::EditAssistantPoint>& eAssistantPoints,UGEditType::EditToolShape nEditType);

	//! \param pPoints [out]。
	//! \param pPolyCount 传出的每个子对象点个数的数组[out]。
	//! \param nSubCount 传出的子对象个数[out]。
	//! \param nSegmentCount 对参数化对象, 每个弧段分割点的个数[in]。
	//! \remarks 参数采用了智能指针的技术,调用者不用释放内存空间
	//! \return 成功返回true,否则返回false。
	virtual UGbool GetSpatialData(UGAutoPtr<UGPoint2D>& pPoints, 
		UGAutoPtr<UGint>& pPolyCount, UGint& nSubCount, UGint nSegmentCount=0) const;



	 //! \brief 根据传入的点串生成对象，主要用于编辑中的参数化等没有内部数组保存点串的对象，
	 //也用于自定义对象,对于有内部数组保存点串的对象，该函数直接return既可，比如GeoLine
	 //GeoLine等对象就直接用AddHandle来生成，效率比较高,但是如果画平行线，就用它来生成
	//! \param pPoints 点串
	//! \param nPntCount  点的个数
     virtual void MakeWithPoints(UGPoint2D* pPoints,UGint nPntCount,UGEditType::EditToolShape nEditType=UGEditType::ET_Default);
	
	 //! \brief 获取对象需要捕捉的点串
	//! \param aryPnts 点串，传出去的要捕捉的点串
	//! \return 如果是true表示有需要捕捉的点串，如果返回false表示该对象没有需要捕捉的点串
	virtual UGbool GetSnapPoints(UGArray<UGPoint2D>& aryPnts);

	//! \brief 获取对象需要捕捉的线段数组
	//! \param aryLines 需要捕捉的线数组（如果有多个part，那么aryLines就应该有多少个count）
	//! \return 如果是true表示有需要捕捉的线，如果返回false表示该对象没有需要捕捉的线
	virtual UGbool GetSnapLines(UGLineArray& aryLines);
	 
	 //added by xielin ^_^ 编辑相关函数,把编辑和geometry统一 end

	//! \brief 调整几何数据,限定在新的Bounds中
	//! \param rcNewBounds 新的,限定范围的Bounds
	//! \return 成功返回true,失败返回false
	virtual UGbool Resize(const UGRect2D& rcNewBounds); 

	//! \brief 计算固定的锚点
	void ComputeFixedAnchorPoints();

	//! \brief 三个固定的锚点
	UGArray<UGPoint3D> m_AnchorPoints;

	//! \brief 获取对象首坐标点
	UGPoint3D GetStartNode();

	//! \brief 获取对象尾坐标点
	UGPoint3D GetEndNode();

	//! \brief 获取GeoLine的阴影体，指针外部释放
	//! \param sceneType 场景类型[in]
	//! \param fBottomHeight 阴影体的底部高度，用于计算包围盒，如果没有不影响渲染，但可能被裁剪[in]
	//! \param fExtendHeight 阴影体的拉伸高度，用于计算包围盒，如果没有不影响渲染，但可能被裁剪[in]
	//! \param fReductionTolerance 简化容限，如果设置为默认值0.0则不简化[in]
	//! \return 成功返回阴影体
	//! \attention 指针请外部释放。这个阴影体是线的顶点按照矩形放样出几何形状。但没有指定高度和宽度，需要在着色器中指定[in]
	UGRenderOperationGeometry* GetBaseShadowVolume(SceneType sceneType, UGfloat fBottomHeight = 0.0f, UGfloat fExtendHeight = 0.0f, UGfloat fReductionTolerance = 0.0f);

	//! \brief 获取GeoLine的阴影体，指针外部释放
	//! \param sceneType 场景类型[in]
	//! \param centerMatrix 中心矩阵[in]
	//! \param fBottomHeight 阴影体的底部高度，用于计算包围盒，如果没有不影响渲染，但可能被裁剪[in]
	//! \param fExtendHeight 阴影体的拉伸高度，用于计算包围盒，如果没有不影响渲染，但可能被裁剪[in]
	//! \param fReductionTolerance 简化容限，如果设置为默认值0.0则不简化[in]
	//! \return 成功返回阴影体
	//! \attention 指针请外部释放。这个阴影体是线的顶点按照矩形放样出几何形状。但没有指定高度和宽度，需要在着色器中指定[in]
	UGRenderOperationGeometry* GetBaseShadowVolume(SceneType sceneType, const UGMatrix4d& centerMatrix, UGfloat fBottomHeight = 0.0f, UGfloat fExtendHeight = 0.0f, UGfloat fReductionTolerance = 0.0f);

	//! \brief 填充线型阴影体的IndexPackage
	//! \param pIndexPackage 待填充的索引数据[in]
	//! \param nPointCount 线的顶点个数[in]
	//! \param nSubCount 子线个数[in]
	//! \param pPolyCount 子线的顶点数[in]
	static void FillShadowVolumeIndexPackage(UGIndexPackage* pIndexPackage, UGint nPointCount, UGint nSubCount, const UGint* pPolyCount);

	//! \brief 获取获取阴影体的顶点坐标
	//! \param sceneType 场景类型[in]
	//! \param arrCenterVertex 输入线的顶点的在世界坐标系中的笛卡尔坐标数组[in]
	//! \param fReductionTolerance 简化容限，如果设置为默认值0.0则不简化[in]
	//! \param arrShadowVolumeVertex 输出阴影体顶点的数据[out]
	static void ConstructShadowVolumeVertex(SceneType sceneType, UGArray<UGVector3d>& arrCenterVertex, UGfloat fReductionTolerance, UGArray<LineShadowVolumeVertex>& arrShadowVolumeVertex);

	//! \brief 构建阴影体的顶点数据包
	//! \param arrShadowVolumeVertex 输入阴影体顶点的数据[in]
	//! \param pVertexDataPackage 输出阴影体的顶点数据包[out]
	static void FillShadowVolumeVertexDataPackage(UGArray<UGArray<LineShadowVolumeVertex> >& arrShadowVolumeVertex, UGVertexDataPackage* pVertexDataPackage);
private:
	//! \brief 构建阴影体的索引数据包
	//! \param nPointCount 这个index对应的线原始的顶点数量[in]
	//! \param nIndexOffset 当前的index的索引偏移量[in/out]
	//! \param nVertexOffset 当前的index的索引的顶点偏移量[in]
	//! \param pIndexPackage 输出阴影体的索引数据包[out]
	template<typename T>
	static void FillShadowVolumeIndex(UGint nPointCount, UGint& nIndexOffset, UGint nVertexOffset, T* pIndexes);
private:
	
	//! \brief 将内部的三维点数据转成二维点
	//! \return 返回二维点指针，外面注意使用后释放。	
	const UGPoint2D* GetPoint2D() const;

	//! \brief 构成线的所有点。
	UGArray<UGPoint3D> m_Points;

	//! \brief 线的子对象个数。
	UGArray<UGint> m_PolyCounts;
};

}

#endif // !defined(AFX_UGGEOLINE3D_H__C9F9464F_CCB3_4297_B3CB_B7D5114B9CBE__INCLUDED_)



