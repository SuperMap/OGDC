/*! \file	 UGGeoPointEPS.h
 *  \brief	 清华山维 点 几何对象类
 *  \author	 ugc team
 *  \attention 
 *  Copyright(c) 2000-2010 SuperMap Software Co., Ltd.<br>
 *  All Rights Reserved
 *  \version $Id: UGGeoPoint.h,v 1.8 2009/09/07 10:33:28 duxm Exp $
 */


#ifndef UGGEOPOINTEPS_H
#define UGGEOPOINTEPS_H

#include "GeometryCAD/UGGeometryEPS.h"
#include "Geometry/UGGeoPoint.h"

namespace UGC {

//! \brief 点几何对象类
class GEOMETRYCAD_API UGGeoPointEPS : public UGGeometryEPS
{
private:
	UGdouble m_xyzScale[3]; //图形缩放比例，xScale, yScale, zScale
public:
	//! \brief 构造函数
	UGGeoPointEPS();
	
	//! \brief 析构函数
	virtual ~UGGeoPointEPS();
	
public: //pure virtual 

	//获得横向坐标比例
	//返回值: 横向坐标比例
	double GetScaleX();

	//获得纵向坐标比例
	//返回值: 纵向坐标比例
	double GetScaleY();

	//获得竖向坐标比例
	//返回值: 竖向坐标比例
	double GetScaleZ();
	//设置横向坐标比例
 	//进口参数: dScaleX(横向坐标比例)
    void SetScaleX( double dScaleX);
	//设置纵向坐标比例
 	//进口参数: dScaleY(纵向坐标比例)
    void SetScaleY( double dScaleY);
	//设置竖向坐标比例
 	//进口参数: dScaleZ(竖向坐标比例)
	void SetScaleZ( double dScaleZ);

	//设置获取图形缩放比例( xScale=0, yScale=1, zScale=2 )
	double GetScale( int iScale );
	void   SetScale( int iScale , double dScaleZ );


	//! \brief 清除Geometry内部数据
	virtual void Clear();
	
	//! \brief 得到维度, 固定为0
	virtual UGint GetDimension() const;
	
	//! \brief 得到类型, 固定为 GeoPoint
	virtual Type GetType() const;
	
	//! \brief 得到内点（在Geometry内部，靠近中央的点）, 即为本身的点
	//! \return 返回内点
	virtual UGPoint2D GetInnerPoint() const;

	//! \brief 点击测试, 用于对象点选
	//! \param pntHitTest 用来测试选中的点
	//! \param dTolerance 容限
	//! \return 选中返回true, 否则返回false
	virtual UGbool HitTest( const UGPoint2D &pntHitTest, UGdouble dTolerance) const;

	//! \brief 点击测试, 用于对象点选(精度比较高,用于地图比例尺比较大,两个点相距非常近的情况)
	//! \param pntHitTest 用来测试选中的点
	//! \param dTolerance 容限
	//! \return 选中返回true, 否则返回false
	// 修改缺陷UGOVI-1047(added by hp 2011-5-31)
	virtual UGbool HitTestH( const UGPoint2D &pntHitTest, UGdouble dTolerance) const;
	
	//! \brief 得到空间数据, 主要用于需要按照维度相同时
	//! 来做统一处理的时候, 例如一些空间判断和操作的算法
	//! \param pPoints 传出的点坐标数组[out]。
	//! \param pPolyCount 传出的每个子对象点个数的数组[out]。
	//! \param nSubCount 传出的子对象个数[out]。
	//! \param nSegmentCount 对参数化对象, 每个弧段分割点的个数[in]。
	//! \remarks 参数采用了智能指针的技术,调用者不用释放内存空间
	//! \return 成功返回true,否则返回false。
	virtual UGbool GetSpatialData(UGAutoPtr<UGPoint2D>& pPoints, 
		UGAutoPtr<UGint>& pPolyCount, UGint& nSubCount, UGint nSegmentCount=0) const;
	
	//! \brief 几何对象是否有效
	//! \return 返回true; 因为无法判断是否无效
	virtual UGbool IsValid()const;
	
	//! \brief 缩放
	//! \param dRatioX X方向缩放比率
	//! \param dRatioY Y方向缩放比率
	virtual void Inflate( UGdouble dRatioX, UGdouble dRatioY);
	
	//! \brief 缩放
	//! \param dRatio 缩放比率(X,Y方向相同)
	virtual void Inflate( UGdouble dRatio);
	
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
	virtual void Rotate(const UGPoint2D& pntOrigin, UGdouble dCosAngle,
						UGdouble dSinAngle, double dAngle = 0);
	
	//! \brief 调整几何数据,限定在新的Bounds中
	//! \param rcNewBounds 新的,限定范围的Bounds
	//! \remarks 坐标点变为Bounds的中心点
	//! \return 成功返回true,失败返回false
	virtual UGbool Resize(const UGRect2D& rcNewBounds); 
	
	//! \brief 两点镜像
	//! \param pntMirror1 构成镜面的第一个点
	//! \param pntMirror2 构成镜面的第二个点
	//! \return 成功返回true,失败返回false
	virtual UGbool Mirror(const UGPoint2D &pntMirror1, const UGPoint2D &pntMirror2);

	virtual UGbool ConvertToPoint( UGGeoPoint &geoPoint) const;	

	virtual UGPoint3D Get3DPoint();
	
public: // 得到和设置信息

	//! \brief 构造点对象
	//! \param pnt [in] 点坐标。
	//! \return 返回true。
	UGbool Make(const UGPointEPS& pnt);

	//! \brief 拷贝构造点对象。
	//! \param geoPoint [in] 已有点对象。
	//! \return 返回true。
	UGbool Make(const UGGeoPointEPS& geoPoint);	

	//! \brief 得到子对象个数
	//! \return 返回子对象个数
	//! \remarks 固定为1。
	virtual UGint GetSubCount() const;	
	
	virtual UGint GetDataSize() const;
public:
	
	//! \brief 设置X值
	//! \param dX 点坐标的X值
	void SetX(UGdouble dX);
	
	//! \brief 得到X值
	//! \return 返回X值
	UGdouble GetX() const;
	
	//! \brief 设置Y值
	//! \param dY 点坐标的Y值
	void SetY(UGdouble dY);
	
	//! \brief 得到Y值
	//! \return 返回Y值
	UGdouble GetY() const;

	//! \brief 设置Z值
	//! \param dZ 点坐标的Z值
	void SetZ(UGdouble dZ);

	//! \brief 得到Z值
	//! \return 返回Z值
	UGdouble GetZ() const;

public:
	
	////! \brief 根据投影转化器的设置和转化方向要求, 进行投影转化
	////! \param pPJTranslator 投影转化器
	////! \param bForward 转化方向, true代表从UGRefTranslator的源(Src)转化到目的(Des), false则相反
	//virtual void PJConvert( UGRefTranslator *pPJTranslator, UGbool bForward = true);
	//virtual UGbool PJConvert( UGRefTranslator *pPJTranslator, UGbool bForward ,UGbool bClip );

	////! \brief 从经纬坐标系统转化为投影坐标系统
	////! \param pCoordSys 投影坐标系统
	//virtual void PJForward( UGPrjCoordSys *pCoordSys );

	////! \brief 从投影坐标系统转化为经纬坐标
	////! \param pCoordSys 投影坐标系统
	//virtual void PJInverse( UGPrjCoordSys *pCoordSys );

#if PACKAGE_TYPE==PACKAGE_UGC

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
	virtual UGbool FromXML(const UGString& strXML, GMLVersionType versionType = SML);
#if !defined SYMBIAN60 && !defined  OS_ANDROID && !defined (IOS)
	//! \brief Geometry与Element的转换
	//! \param pElement,返回的pElement指针在函数内分配内存,外面负责删除，否则会有内存泄漏
	virtual UGbool ToElement(OgdcElement*& pElement)  const;

	//! \brief Geometry与Element的转换
	//! \param pElement,传入的Element，如果无法转换则返回False，否则返回True
	virtual UGbool FromElement(const OgdcElement* pElement);
#endif

#endif // end declare ugc sdk

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
	
protected: //! 子类需要实现的一些不公开函数
	//! \brief 
	virtual UGRect2D ComputeBounds() const;
	/*virtual UGRect2D GetBoundsInside() const;
	virtual void SetBoundsInside(const UGRect2D & rcBounds);*/
	
	//! \brief 存储到Stream中
	virtual UGbool SaveGeoData(UGStream& stream, UGDataCodec::CodecType eCodecType,UGbool bSDBPlus = FALSE)  const;
	//! \brief 从Stream中生成Geometry
	virtual UGbool LoadGeoData( UGStream& stream , UGDataCodec::CodecType eCodecType = UGDataCodec::encNONE,UGbool bSDBPlus = FALSE );
	//! \brief 只转载几何数据部分
	//! \param stream 流 
	//! \param iGuge 用户采用的Load方式的nGuge判断参数
	//! \param eCodecType 编码方式
	//! \attention 还需要增加 UGbool bSDBPlus = FALSE 的参数
	//! \return 成功返回true,失败返回false
	virtual UGbool LoadGeoData( UGStream& stream , UGint iGuge, UGDataCodec::CodecType eCodecType = UGDataCodec::encNONE,UGbool bSDBPlus = FALSE);
};

}

#endif

