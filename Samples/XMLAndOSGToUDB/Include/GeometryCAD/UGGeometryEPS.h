/*! \file	 UGGeometryEPS.h
 *  \brief	 清华山维 CGeoBase 类型
 *  \author	 ugc team
 *  \attention 
 *  Copyright(c) 2000-2010 SuperMap Software Co., Ltd.<br>
 *  All Rights Reserved
 *  \version $Id: UGGeoPoint.h,v 1.8 2009/09/07 10:33:28 duxm Exp $
 */


#ifndef UGGEOMETRYEPS_H
#define UGGEOMETRYEPS_H

#include "Geometry/UGGeometry.h"

namespace UGC {

//float:
#define  ZERO_FLOAT (+1.401298E-45)
#define  MIN_FLOAT	(-3.402823E+38)
#define  MAX_FLOAT	(+3.402823E+38)

//double:
#define  ZERO_DOUBLE   (+4.94065645841247E-324)
#define  MIN_DOUBLE	 (-1.79769313486232E+308)
#define  MAX_DOUBLE	 (+1.79769313486232E+308)

//int:
#define  MIN_SHORT	-32768
#define  MAX_SHORT	+32767

//long:
#define  MIN_LONG	-2147483648
#define  MAX_LONG   +2147483647

//坐标容差、浮点计算容差
#define MINI_COORDINATE	0.001
#define MINI_FLOAT	0.000001
#define MINI_DOUBLE 0.0000000001

typedef enum ObjStatusEnum
{
	e_Valid          =  0x0001,   //是否有效
	e_Explode        =  0x0002,   //是否已打散
	e_Screen         =  0x0004,   //是否被屏蔽
	e_Duplicate		 =  0x0008,   //是否被复制品
	e_Quote          =  0x0010,   //是否索引
	e_Hidden         =  0x0020,   //是否隐藏
	e_Selected       =  0x0040,   //是否已选择 
	e_Editable       =  0X0080,   //是否可编辑
	e_Highlight      =  0X0100,   //是否加亮
	e_Gray           =  0X0200,   //是否变灰
	e_Edited         =  0X0400,   //是否被修改
	e_Refrence       =  0X0800,   //参考图形
	e_Opaque         =  0X1000,   //是否压盖
	e_Mirror         =  0X2000,   //是否镜向
	e_Virtual        =  0X4000,   //是否是虚拟地物
};


class GEOMETRYCAD_API UGGeometryEPS : public UGGeometry
{
protected:

	UGString 			m_strLayer;           //地物所在用户层名
	UGshort				m_Status;	        //地物状态---加锁,被删除
	UGuint				m_SymbolRef;        //符号化参考

	UGString		    m_strObjName;			//对象名称
	UGPointEPSs			m_Points;			//点列
	UGArray<UGint>		m_relGeoIDs;			//关系

	UGColor				m_Color;			//颜色
	UGColor             m_FillColor;        //填充颜色

	UGint		        m_Code;             //地物分类编码
	UGshort		        m_LineWidth;	    //线宽
	UGshort		        m_LineType;	        //连接类型：0；组代理、1；折线、2；曲线、3；圆弧、4；圆。
									        //若为点地物；0：组代理。1：一般独立点，2：有向点，3：结点。

	UGString            m_strMemoData;        //外部属性数据
	UGString            m_strDataMark;        //数据标识字符串
	UGString            m_strByname;          //别名

	UGuint              m_nGroupID;			//组标识
	UGdouble            m_dCreateTime;      //创建时间
	UGdouble            m_dEditTime;        //修改时间
	UGdouble            m_dReference;       //参照

	UGArray<UGbyte>     m_GraphicInfo;     //图形特征


	//! \brief 构造函数
	UGGeometryEPS();
	
	//! \brief 析构函数
	virtual ~UGGeometryEPS();
	
public: //pure virtual 

	//! \brief 清除Geometry内部数据
	virtual void Clear() = 0;
	
	//! \brief 得到维度, 固定为0
	virtual UGint GetDimension() const = 0;
	
	//! \brief 得到类型, 固定为 GeoPoint
	virtual Type GetType() const = 0;
	
	//! \brief 得到内点（在Geometry内部，靠近中央的点）, 即为本身的点
	//! \return 返回内点
	virtual UGPoint2D GetInnerPoint() const = 0;

	//对象组ID
	void SetGroupID( UGuint id );
	UGuint GetGroupID( ) const;

	//对象特征编码
	void SetCode( UGint code );
	UGint GetCode( ) const;

	//用户使用的分类标志
	//进口参数: strDataMark(用户使用的分类标志)
	void    SetUserID( const UGString& strUserID );
	//返回值: 用户使用的分类标志
	UGString GetUserID();

	//设置状态
	//进口参数: bFlag(是否是此状态),eInfo(状态信息)
	void SetStatus(UGbool bFlag, ObjStatusEnum eInfo) ;
	//进口参数: eInfo(状态信息)
	//返回值: TRUE表示是此状态,FALSE表示不是此状态
    UGbool GetStatus( ObjStatusEnum eInfo) const;

	//获取与设置地物状态成员
	UGshort GetObjStatus() const;
    void SetObjStatus( UGshort eInfo) ;

	//获得时间
	//进口参数: nType(0创建时间,1修改时间)
	//返回值: 时间对象
	UGTime GetDateTime( UGuint nType = 1 );
	//设置时间
	//进口参数: DateTime(时间对象),nType(0创建时间,1修改时间)
	void SetDateTime( UGTime& DateTime , UGuint nType = 1 ) ;

	//获得时间
	//进口参数: nType(0创建时间,1修改时间)
	//返回值: 时间数值
    UGdouble GetDblDateTime( UGuint nType = 1 );

	//设置时间
	//进口参数: DateTime(时间数值),nType(0创建时间,1修改时间)
	void SetDblDateTime( UGdouble DateTime ,UGuint nType = 1 );

	// 设置线宽
	//进口参数: lineWidth(线宽)
	UGshort SetLineWidth(UGshort lineWidth);
	//获得线宽
	//返回值: 线宽
	UGshort GetLineWidth() const;

	//设置线型方向成员
 	//进口参数: nStyle(线型方向成员)
	void SetStyleDirection( UGshort nType );
	//获得线型方向成员
	//返回值: 线型方向成员
	UGshort GetStyleDirection() const ;

	// 设置线型
	//进口参数: lineType(线型)
	UGshort SetLineType(UGshort lineType) ;
	//获得线型
	//返回值: 线型
	UGshort GetLineType() const ;

	//设置线样式 ( 简单线, 多义线, 变宽线... )
	//进口参数: lineStyle(线样式)
	UGshort SetLineStyle(UGshort lineStyle) ;
	//获得线样式
	//返回值: 线样式
	UGshort GetLineStyle() const ;

	//设置线形游动比例(可选值(0,1,2,3~16)/16 LC类符号相对一个周期, Y, P类相对一线段长度)
	//进口参数: scale(游动比例)
	UGshort SetLShapeOffset(UGshort scale) ;
	//获得游动比例
	//返回值: 游动比例
	UGshort GetLShapeOffset() const ;

	//面符号填充密度比例
	//当scale <= 1/8 时, 为实心面 
	//当scale >=  8  时, 不填充(空面) 

	//设置面符号填充密度比例
	//进口参数: scale(填充密度比例)
	UGdouble SetHatchDensityScale( UGdouble scale );
	//获得面符号填充密度比例
	//返回值: 面符号填充密度比例
	UGdouble GetHatchDensityScale() const ;

	//设置系统填充符号样式
	//进口参数: nStyle(填充符号样式)
	UGshort  SetSysHatchStyle( UGshort nStyle );
	//获得面符号填充符号样式
	//返回值: 填充符号样式
	UGshort  GetSysHatchStyle() const;

	// 设置颜色
 	//进口参数: color(颜色)
	UGColor SetColor(UGColor color) ;
	//获得颜色
	//返回值: 颜色
	UGColor GetColor() const;

	//设置填充色
 	//进口参数: color(填充色)
	UGColor SetFillColor(UGColor color) ;
	//获得填充色
	//返回值: 填充色
	UGColor GetFillColor() const ;

	//获取点列的点数
	UGint   GetPointSize() const;
	//获取点列
	//出口参数: Points(定位点列)
	void  GetPointList( UGPointEPSs& Points);
	//设置点列
 	//进口参数: pointList(对象新点列)
	void  SetPointList(const UGPointEPSs &pointList) ;

	// 得到指定对象的点串的指针。
	// 返回对象的点串的指针。
	const UGPointEPS* GetPoints() const;

	//获取一个点拷贝
	//进口参数: index(点列中指定下标), index = -1 指最后一个点
	//返回值: 点
	UGPointEPS& GetPoint(UGint index ) ;

	//向点列中加一个点	
	//进口参数: Point(新增的点)
	bool    SetPoint( UGint index, UGPointEPS point );

	//向点列中加一个点	
	//进口参数: Point(新增的点)
	int    AddPoint( UGPointEPS Point ) ;

	//向点列中插入一个点	
	//进口参数: Point(新增的点)
	bool InsertPoint(UGint index, UGPointEPS point);

	//移出点列中指定的点	
	bool RemovePoint(UGint index);

	//移出点列中所有的点	
	void    RemoveAllPoint();

	//获得横向坐标比例
	//返回值: 横向坐标比例
	virtual UGdouble GetScaleX();
	//获得纵向坐标比例
	//返回值: 纵向坐标比例
	virtual UGdouble GetScaleY() ;
	//获得竖向坐标比例
	//返回值: 竖向坐标比例
	virtual UGdouble GetScaleZ() ;
	//设置横向坐标比例
 	//进口参数: dScaleX(横向坐标比例)
	virtual void SetScaleX( UGdouble dScaleX) ;
	//设置纵向坐标比例
 	//进口参数: dScaleY(纵向坐标比例)
    virtual void SetScaleY( UGdouble dScaleY) ;
	//设置竖向坐标比例
 	//进口参数: dScaleZ(竖向坐标比例)
	virtual void SetScaleZ( UGdouble dScaleZ) ;

	//获得角度(点地物的旋转角)
	//返回值: 角度
	UGfloat GetAngle() const;
	//设置角度(点地物的旋转角)
	//进口参数: newAngle(新角度)
	//返回值: 原角度
	UGfloat SetAngle( UGfloat newAngle); 

	//名称
	//进口参数: strName(地物名称)
	void SetName( UGString& strName );
	//返回值: 地物名称
	UGString GetName() const;

	//参照
	//进口参数: dReference(参照)
	void SetReference( UGdouble dReference );
	//返回值: 参照
	UGdouble GetReference( ) const;

	//符号化参考
	//进口参数: SymbolRef(符号化参考)
	void SetSymbolRef( UGuint SymbolRef );
	//返回值: 符号化参考
	UGuint GetSymbolRef( ) const;

	//设置符号朝向
	//进口参数: shDirection(符号朝向)
	void SetDirection( UGshort shDirection );
	//获得符号朝向
	//返回值: 符号朝向
    UGshort GetDirection() const;

	//设置属性信息
	void SetMemoData( const UGStrings& strFields, const UGStrings& strValues );
	void SetMemoData( UGStrings& strValues );
	void SetMemoData( UGString& strValues );

	int  GetMemoData( UGStrings& strFields, UGStrings& strValues );
	UGbool GetMemoData( UGStrings& strValues ) const;
	UGString GetMemoData() const;
	UGString GetMemoData( const UGString& strField );

	void SetGraphicInfo( UGArray<UGbyte>& bytes );
 	UGbool GetGraphicInfo( UGArray<UGbyte>& bytes ) const;

	UGbool IsContainsPointType(PointEPSType pointType) const;

	//设置RelationID数组
 	//进口参数: relArr(RelationID数组)
	void    SetRelation( const UGArray<UGint>& relArr );

	//获得RelationID数组
	//返回值: RelationID数组
	void    GetRelations( UGArray<UGint>& relArr ) const;

	//! \brief 点击测试, 用于对象点选
	//! \param pntHitTest 用来测试选中的点
	//! \param dTolerance 容限
	//! \return 选中返回true, 否则返回false
	virtual UGbool HitTest( const UGPoint2D &pntHitTest, UGdouble dTolerance) const = 0;
	
	//! \brief 得到空间数据, 主要用于需要按照维度相同时
	//! 来做统一处理的时候, 例如一些空间判断和操作的算法
	//! \param pPoints 传出的点坐标数组[out]。
	//! \param pPolyCount 传出的每个子对象点个数的数组[out]。
	//! \param nSubCount 传出的子对象个数[out]。
	//! \param nSegmentCount 对参数化对象, 每个弧段分割点的个数[in]。
	//! \remarks 参数采用了智能指针的技术,调用者不用释放内存空间
	//! \return 成功返回true,否则返回false。
	virtual UGbool GetSpatialData(UGAutoPtr<UGPoint2D>& pPoints, 
		UGAutoPtr<UGint>& pPolyCount, UGint& nSubCount, UGint nSegmentCount=0) const = 0;
	
	//! \brief 几何对象是否有效
	//! \return 返回true; 因为无法判断是否无效
	virtual UGbool IsValid()const = 0;
	
	//! \brief 缩放
	//! \param dRatioX X方向缩放比率
	//! \param dRatioY Y方向缩放比率
	virtual void Inflate( UGdouble dRatioX, UGdouble dRatioY) = 0;
	
	//! \brief 缩放
	//! \param dRatio 缩放比率(X,Y方向相同)
	virtual void Inflate( UGdouble dRatio) = 0;
	
	//! \brief 平移
	//! \param dX 平移的X偏移量
	//! \param dY 平移的Y偏移量
	virtual void Offset( UGdouble dX, UGdouble dY) = 0;
	
	//! \brief 平移
	//! \param 偏移量(X,Y方向相同)
	virtual void Offset( UGdouble dOffset) = 0;
	
	//! \brief 平移
	//! \param 平移的偏移量(分别用UGSize2D中的x,y代表X,Y方向的偏移量)
	virtual void Offset( const UGSize2D &szOffset) = 0;
	
	//! \brief 旋转
	//! \param pntOrigin 旋转的中心点(锚点,不动点)
	//! \param dAngle 旋转角度
	virtual void Rotate(const UGPoint2D& pntOrigin, UGdouble dAngle) {};
	
	//! \brief 旋转,增加一些参数,以便内部可以不用进行cos,sin, 在批量处理的时候有利于提高效率
	//! \remarks dAngle看似多余,但GeoText需要,所以要保留
	//! \param pntOrigin 旋转的中心点(锚点,不动点)
	//! \param dCosAngle 旋转的cos值
	//! \param dSinAngle 旋转的sin值
	//! \param dAngle 旋转角度
	virtual void Rotate(const UGPoint2D& pntOrigin, UGdouble dCosAngle,
						UGdouble dSinAngle, UGdouble dAngle = 0) = 0;
	
	//! \brief 调整几何数据,限定在新的Bounds中
	//! \param rcNewBounds 新的,限定范围的Bounds
	//! \remarks 坐标点变为Bounds的中心点
	//! \return 成功返回true,失败返回false
	virtual UGbool Resize(const UGRect2D& rcNewBounds) = 0; 
	
	//! \brief 两点镜像
	//! \param pntMirror1 构成镜面的第一个点
	//! \param pntMirror2 构成镜面的第二个点
	//! \return 成功返回true,失败返回false
	virtual UGbool Mirror(const UGPoint2D &pntMirror1, const UGPoint2D &pntMirror2) {return FALSE;};
	
public: // 得到和设置信息

	//! \brief 得到子对象个数
	//! \return 返回子对象个数
	//! \remarks 固定为1。
	virtual UGint GetSubCount() const;	
	
	virtual UGint GetDataSize() const;

public:
	
	//! \brief 根据投影转化器的设置和转化方向要求, 进行投影转化
	//! \param pPJTranslator 投影转化器
	//! \param bForward 转化方向, true代表从UGRefTranslator的源(Src)转化到目的(Des), false则相反
	virtual void PJConvert( UGRefTranslator *pPJTranslator, UGbool bForward = true);
	virtual UGbool PJConvert( UGRefTranslator *pPJTranslator, UGbool bForward ,UGbool bClip );

	//! \brief 从经纬坐标系统转化为投影坐标系统
	//! \param pCoordSys 投影坐标系统
	virtual void PJForward( UGPrjCoordSys *pCoordSys );

	//! \brief 从投影坐标系统转化为经纬坐标
	//! \param pCoordSys 投影坐标系统
	virtual void PJInverse( UGPrjCoordSys *pCoordSys );

#if PACKAGE_TYPE==PACKAGE_UGC

	//! \brief 生成XML格式的信息
	//! \param versionType GML的版本
	//! \param nPace 缩进的空格数目,使得xml字符串更加易于人工阅读; 
	//! 主要用于如果返回的xml字符串是一个大标签中的一部分, 通过指定空格数目,可以使层次清晰
	//! \return 返回指定GML版本的xml字符串
	virtual UGString ToXML(GMLVersionType versionType, UGint nPace = 0) const {return _U("");};

	//! \brief 从XML字符串中生成Geometry
	//! \param strXML xml格式字符串
	//! \param versionType xml字符串的GML版本
	//! \return 返回指定GML版本的xml字符串
	virtual UGbool FromXML(const UGString& strXML, GMLVersionType versionType = SML) {return FALSE;};
#if !defined SYMBIAN60 && !defined  OS_ANDROID && !defined (IOS)
	//! \brief Geometry与Element的转换
	//! \param pElement,返回的pElement指针在函数内分配内存,外面负责删除，否则会有内存泄漏
	virtual UGbool ToElement(OgdcElement*& pElement)  const {return FALSE;};

	//! \brief Geometry与Element的转换
	//! \param pElement,传入的Element，如果无法转换则返回False，否则返回True
	virtual UGbool FromElement(const OgdcElement* pElement) { return FALSE;};
#endif

#endif // end declare ugc sdk

public:
	
	//! \brief 获得编辑对象的句柄的数目
	//! \remarks 对于固定句柄对象为9包括了改变大小的8个控制点和一个旋转控制点
	virtual UGint  GetHandleCount(){return 0;};

	
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
	virtual UGint GetHandle( UGint nHandle, UGPoint2D& point){return 0;};


	//! \brief 移动控制点到指定坐标,主要用于绘制对象，修改对象节点等操作
	//! \param nHandle 控制点句柄
	//! \param point 控制点坐标
	virtual void MoveHandle( UGint nHandle, const UGPoint2D& pnt2D,UGEditType::EditToolShape nEditType=UGEditType::ET_Default){};

	//! \brief 节点编辑时可用来插入一个控制点,也可以用于绘制对象时加入节点
	//! \param nHandle 插入位置
	//! \param pnt2D 插入点的地理坐标
	virtual void AddHandle( UGint nHandle, const UGPoint2D& pnt2D,UGEditType::EditToolShape nEditType=UGEditType::ET_Default){};
	
	//! \brief 删除编辑对象的指定节点
	//! \param nHandle 节点句柄
	virtual UGuint DelNode( UGint nHandle ) {return 0;};

	//! \brief 计算对象绘制时的辅助线，返回的是一个复合对象
	//! \param pPoints 点串
	//! \param nPntCount  点的个数
	//! \param nEditType 绘制对象的类型，参考UGEditType
	//! \param pStyle	 辅助线的风格，如果传入为NULL，用默认的风格
	virtual UGGeometry*  CalAssistantLine(const UGPoint2D* pPoints,UGint nPntCount,UGEditType::EditToolShape nEditType,UGStyle* pStyle=NULL){return NULL;};
	
protected: //! 子类需要实现的一些不公开函数
	//! \brief 
	virtual UGRect2D ComputeBounds() const = 0;

	virtual void CopyBaseData(const UGGeometryEPS& geo);
	
	//! \brief 存储到Stream中
	virtual UGbool SaveGeoData(UGStream& stream, UGDataCodec::CodecType eCodecType,UGbool bSDBPlus = FALSE) const;
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

