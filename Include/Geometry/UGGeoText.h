/*! \file	 UGGeoText.h
 *  \brief	 文本对象类
 *  \author	 ugc team
 *  \attention 
 *  Copyright(c) 2000-2010 SuperMap Software Co., Ltd.<br>
 *  All Rights Reserved
<<<<<<< UGGeoText.h
 *  \version $Id: UGGeoText.h,v 1.11 2010/03/17 06:16:11 majt Exp $
=======
 *  \version $Id: UGGeoText.h,v 1.11 2010/03/17 06:16:11 majt Exp $
>>>>>>> 1.34.2.1
 */


#ifndef UGGEOTEXT_H
#define UGGEOTEXT_H

#include "Geometry/UGGeometry.h"
#include "Toolkit/UGTextStyle.h"

namespace UGC {

//! \brief 文本子对象
#ifdef SYMBIAN60
class GEOMETRY_APIC UGSubText 
#else
class GEOMETRY_API UGSubText 
#endif
{
public:
	//! \brief 默认构造函数
	GEOMETRY_APIF UGSubText();

	//! \brief 拷贝构造函数。
	//! \param subText [in] 已有的文本子对象。
	GEOMETRY_APIF UGSubText(const UGSubText& subText);	

	//! \brief 用文本内容字符串进行构造
	//! \param strText [in] 文本内容字符串。
	//! \param dAngle [in] 旋转角度,默认为0
	GEOMETRY_APIF UGSubText(const UGString& strText, UGdouble dAngle=0);
	
	//! \brief 得到旋转角度。
	//! \return 返回旋转角度。
	GEOMETRY_APIF UGdouble GetAngle() const;	

	//! \brief 设置旋转角度。
	//! \param dAngle [in] 旋转角度。
	GEOMETRY_APIF void SetAngle(UGdouble dAngle);	
	
	//! \brief 得到文本内容。
	//! \return 返回文本内容。
	GEOMETRY_APIF UGString GetText() const;	

	//! \brief 设置文本内容。
	//! \param strText [in] 文本内容。
	GEOMETRY_APIF void SetText(const UGString& strText);	
	
	//! \brief 赋值函数。
	//! \param subText [in] 已有的文本子对象。
	GEOMETRY_APIF void operator=(const UGSubText& subText);
	
private:
	// modified by zengzm 2007-4-27 
	//UGint m_nAngle; //! 子文本的倾角，单位为 0.1度, 逆时针方向
	//! \brief 子文本的旋转角度, 单位为:度, 逆时针方向
	//! \remarks 内存中改为double,角度单位,存储时和SFC保持兼容
	UGdouble m_dAngle;
	
	//! \brief 预留
	//UGint m_nReserved;

	//! \brief 文本内容字符串
	UGString m_strText;
};



//! \brief 文本对象类
//! \remarks 对文本对象,可以有多个文本子对象,每个文本子对象有自己的锚点和旋转角度,
//! 但文本的风格是统一的,只有文本对象有一份.
#ifdef SYMBIAN60
class GEOMETRY_APIC UGGeoText : public UGGeometry
#else
class GEOMETRY_API UGGeoText : public UGGeometry
#endif
{
public:
	//! \brief 构造函数
	GEOMETRY_APIF UGGeoText();

#ifdef SYMBIAN60
	GEOMETRY_APIF UGGeoText(const UGGeoText& geoText);
	GEOMETRY_APIF UGGeoText& operator=(const UGGeoText& geoText);
#endif
	
	//! \brief 析构函数
	virtual GEOMETRY_APIF ~UGGeoText();
	
public: 
	
	//! \brief 清空地理数据
	virtual GEOMETRY_APIF void Clear();
	
	//! \brief 得到维度, 固定为-1
	virtual GEOMETRY_APIF UGint GetDimension() const;
	
	//! \brief 得到类型,固定为GeoText
	virtual GEOMETRY_APIF Type GetType() const;
	
	//! \brief 得到内点（在Geometry内部，靠近中央的点）
	//! \return 返回第一个子对象的锚点
	virtual GEOMETRY_APIF UGPoint2D GetInnerPoint() const;
	
	//! \brief 点击测试, 用于对象点选
	//! \param pntHitTest 用来测试选中的点
	//! \param dTolerance 容限
	//! \return 选中返回true, 否则返回false
	virtual GEOMETRY_APIF UGbool HitTest( const UGPoint2D &pntHitTest, UGdouble dTolerance) const;	

	//! \brief 得到空间数据, 主要用于需要按照维度相同时
	//! 来做统一处理的时候, 例如一些空间判断和操作的算法
	//! \param pPoints 传出的点坐标数组[out]。
	//! \param pPolyCount 传出的每个子对象点个数的数组[out]。
	//! \param nSubCount 传出的子对象个数[out]。
	//! \param nSegmentCount 对参数化对象, 每个弧段分割点的个数[in]。
	//! \remarks 参数采用了智能指针的技术,调用者不用释放内存空间
	//! \return 成功返回true,否则返回false。
	virtual GEOMETRY_APIF UGbool GetSpatialData(UGAutoPtr<UGPoint2D>& pPoints, 
		UGAutoPtr<UGint>& pPolyCount, UGint& nSubCount, UGint nSegmentCount=0) const;
	//! \brief 得到子对象的个数
	//! \return 返回子对象的个数
	virtual GEOMETRY_APIF UGint GetSubCount() const;
	
	//! \brief 几何对象是否有效
	//! \return 有效返回true,无效返回false
	virtual GEOMETRY_APIF UGbool IsValid()const;
	
	//! \brief 缩放
	//! \param dRatioX X方向缩放比率
	//! \param dRatioY Y方向缩放比率
	virtual GEOMETRY_APIF void Inflate( UGdouble dRatioX, UGdouble dRatioY);
	
	//! \brief 缩放
	//! \param dRatio 缩放比率(X,Y方向相同)
	virtual GEOMETRY_APIF void Inflate( UGdouble dRatio);
	
	//! \brief 平移
	//! \param dX 平移的X偏移量
	//! \param dY 平移的Y偏移量
	virtual GEOMETRY_APIF void Offset( UGdouble dX, UGdouble dY);
	
	//! \brief 平移
	//! \param 偏移量(X,Y方向相同)
	virtual GEOMETRY_APIF void Offset( UGdouble dOffset);
	
	//! \brief 平移
	//! \param 平移的偏移量(分别用UGSize2D中的x,y代表X,Y方向的偏移量)
	virtual GEOMETRY_APIF void Offset( const UGSize2D &szOffset);
	
	//! \brief 旋转
	//! \param pntOrigin 旋转的中心点(锚点,不动点)
	//! \param dAngle 旋转角度
	virtual GEOMETRY_APIF void Rotate(const UGPoint2D& pntOrigin, UGdouble dAngle);
	
	//! \brief 旋转,增加一些参数,以便内部可以不用进行cos,sin, 在批量处理的时候有利于提高效率
	//! \remarks dAngle看似多余,但GeoText需要,所以要保留
	//! \param pntOrigin 旋转的中心点(锚点,不动点)
	//! \param dCosAngle 旋转的cos值
	//! \param dSinAngle 旋转的sin值
	//! \param dAngle 旋转角度
	virtual GEOMETRY_APIF void Rotate( const UGPoint2D& pntOrigin, UGdouble dCosAngle, UGdouble dSinAngle, double dAngle = 0);
	
	//! \brief 调整几何数据,限定在新的Bounds中
	//! \param rcNewBounds 新的,限定范围的Bounds
	//! \return 成功返回true,失败返回false
	virtual GEOMETRY_APIF UGbool Resize(const UGRect2D& rcNewBounds);
	
public:
	//! \brief 根据投影转化器的设置和转化方向要求, 进行投影转化
	//! \param pPJTranslator 投影转化器
	//! \param bForward 转化方向, true代表从UGRefTranslator的源(Src)转化到目的(Des), false则相反
	virtual GEOMETRY_APIF void PJConvert( UGRefTranslator *pPJTranslator, UGbool bForward = true);
	virtual GEOMETRY_APIF UGbool PJConvert( UGRefTranslator *pPJTranslator, UGbool bForward ,UGbool bClip );

	//! \brief 从经纬坐标系统转化为投影坐标系统
	//! \param pCoordSys 投影坐标系统
	virtual GEOMETRY_APIF void PJForward( UGPrjCoordSys *pCoordSys );

	//! \brief 从投影坐标系统转化为经纬坐标
	//! \param pCoordSys 投影坐标系统
	virtual GEOMETRY_APIF void PJInverse( UGPrjCoordSys *pCoordSys );

	UGSize2D* m_pSize2D;
	
public://需要重新实现的方法
	
	//! \brief 设置风格
	//! \param pStyle 风格指针
	//! \remarks 如果pStyle==NULL,则Geometry内部风格指针被delete; 否则会复制一份风格
	//! 对于文本对象等, 设置风格不起作用,因为文本对象有自己的TextStyle
	virtual GEOMETRY_APIF void SetStyle(const UGStyle* pStyle);
	
	virtual GEOMETRY_APIF UGint GetDataSize() const;
public: //! group 创建文本对象
	//! \brief 通过文本内容/锚点/旋转角度来构造文本对象。
	//! \param strText [in] 文本内容。
	//! \param pntAnchor [in] 锚点。
	//! \param dAngle [in] 旋转角度。
	//! \return 返回true。
	GEOMETRY_APIF UGbool Make(const UGString& strText, const UGPoint2D& pntAnchor, UGdouble dAngle=0.0);	
	
	//! \brief 创建文本对象(沿线文本)
	//! \param strText [in] 文本内容。
	//! \param geoline [in] 要沿线的线。
	//! \param bFixedAngle [in] 是否固定角度。
	//! \param dAngle [in] 固定角度时的文本角度。
	//! \return 如果文本为空,或者线对象非法,返回false。
	GEOMETRY_APIF UGbool Make(const UGString& strText, const UGGeoLine& geoline, 
						UGbool bFixedAngle=FALSE, UGdouble dAngle=0.0);	
	
	//! \brief 创建文本对象(默认风格)
	//! \param pntAnchor [in] 锚点(文本对象的定位点)。
	//! \param subText [in] 文本子对象。
	//! \return 返回true
	GEOMETRY_APIF UGbool Make(const UGPoint2D& pntAnchor, const UGSubText& subText);	

	//! \brief 创建文本对象(指定风格,且有多个子对象)
	//! \param textStyle [in] 文本风格。
	//! \param pPoints [in] 文本子对象的定位点点串指针。
	//! \param pSubText [in] 文本子对象的指针。
	//! \param nSubCount [in] 文本子对象的数目。
	//! \return 返回true。
	GEOMETRY_APIF UGbool Make(const UGTextStyle& textStyle, const UGPoint2D* pPoints, const UGSubText* pSubText, UGint nSubCount);	

	//! \brief 通过复制构造文本对象。
	//! \param geotext [in] 已有的文本对象。
	//! \return 返回true。
	GEOMETRY_APIF UGbool Make(const UGGeoText& geotext);	
	
	//! \brief 增加文本子对象
	//! \param strText [in] 文本内容。
	//! \param pntAnchor [in] 子对象的锚点。
	//! \param dAngle=0.0 [in] 子对象的旋转角度。
	//! \return 返回true。
	GEOMETRY_APIF UGbool AddSub(const UGString& strText, const UGPoint2D& pntAnchor, UGdouble dAngle=0.0);	

	//! \brief 增加文本子对象。
	//! \param pntAnchor [in] 锚点。
	//! \param subText [in] 文本子对象。
	//! \return 返回true。
	GEOMETRY_APIF UGbool AddSub(const UGPoint2D& pntAnchor, const UGSubText& subText);	

	//! \brief 插入文本子对象。
	//! \param nIndex [in] 插入的位置索引。
	//! \param strText [in] 文本内容。
	//! \param pntAnchor [in] 锚点。
	//! \param dAngle [in] 旋转角度。
	//! \return 
	GEOMETRY_APIF UGbool InsertSub(UGint nIndex, const UGString& strText, const UGPoint2D& pntAnchor,UGdouble dAngle);	

	//! \brief 插入文本子对象。
	//! \param nIndex [in] 插入的位置索引。
	//! \param pntAnchor [in] 锚点。
	//! \param subText [in] 文本子对象。
	//! \return 。
	GEOMETRY_APIF UGbool InsertSub(UGint nIndex, const UGPoint2D& pntAnchor, const UGSubText& subText);	
	
	//! \brief 删除文本子对象
	//! \param nIndex [in] 要删除的子对象位置索引。
	//! \return 索引非法,返回false。
	GEOMETRY_APIF UGbool DelSub(UGint nIndex);	
	
	//! \brief 设置文本子对象。
	//! \param nIndex [in] 设置的索引。
	//! \param strText [in] 文本内容。
	//! \param pntAnchor [in] 锚点。
	//! \param dAngle [in] 旋转角度。
	//! \return 。
	GEOMETRY_APIF UGbool SetSub(UGint nIndex, const UGString& strText, 
				const UGPoint2D& pntAnchor, UGdouble dAngle=0.0);	
	
	//! \brief 设置文本子对象。
	//! \param nIndex [in] 设置的索引。
	//! \param pntAnchor [in] 锚点。
	//! \param subText [in] 文本子对象。
	//! \return 。
	GEOMETRY_APIF UGbool SetSub(UGint nIndex, const UGPoint2D& pntAnchor, const UGSubText& subText);	
	
	//! \brief 得到文本子对象
	//! \param nIndex [in] 位置索引。
	//! \return 返回文本子对象。
	const GEOMETRY_APIF UGSubText& GetSub(UGint nIndex) const {return m_SubTexts[nIndex];}	

	//! \brief 得到文本子对象
	//! \param nIndex [in] 位置索引。
	//! \return 返回文本子对象。
	GEOMETRY_APIF UGSubText& GetSub(UGint nIndex) {return m_SubTexts[nIndex];}
	
	//! \brief 得到文本子对象的锚点。
	//! \param nIndex [in] 要得到子对象的位置索引。
	//! \return 返回锚点坐标置。
	const GEOMETRY_APIF UGPoint2D& GetSubAnchor(UGint nIndex) const {return m_Points[nIndex];}	

	//! \brief 得到文本子对象的锚点。
	//! \param nIndex [in] 要得到子对象的位置索引。
	//! \return 返回锚点坐标置。
	GEOMETRY_APIF UGPoint2D& GetSubAnchor(UGint nIndex) {return m_Points[nIndex];}

	GEOMETRY_APIF void SetSubAnchor(UGint nIndex, const UGPoint2D& newPoint) { m_Points[nIndex] = newPoint;}
	
	//! \brief 设置文本风格
	//! \param textStyle [in] 文本风格。
	GEOMETRY_APIF void SetTextStyle(const UGTextStyle& textStyle, UGbool bChangeSubTextAngle = false);	

	//! \brief 得到文本风格。
	//! \return 返回文本风格。
	const GEOMETRY_APIF UGTextStyle& GetTextStyle() const;	
	//! \remarks 
	//! 1) 去掉前后的 空格/回车换行/Tab 等字符
	//! 2) 中间的 空格/回车换行/Tab , 如果前后是 中文/日文 等, 也去掉
	//! 3) 如果前后都是 英文(or数字等ACSII码), 则 保留一个空格(回车换行/Tab等也转换为一个空格)
	//! 原因: 这个接口就是用来 进行 字符串比较查询的, 如 整个文本数据集都 ToString 后, 
	//! 保存到文本字段中, 然后 用户就可以很方便的 在字段中进行 查找, 
	//! 如 在图形显示上 需要 写成" I am   \r\n a student.   ", 但 用户在属性表中查询时, 
	//! 只需要 输入 "I am a student.", 达到所想即所得 的效果
	//!	如:　" I am   \r\n a student.   " 变为 "I am a student."
    //!					 "Hello, \r中国 "  			  变为 "Hello,中国"
    //!					 "Abc\rABC"					  变为 "Abc ABC"
	//! \return 返回文本对象的合并结果
	//! \brief 取得文本对象的索引串(所有的子对象的文字串的合并,去掉空格、换行等)
	GEOMETRY_APIF UGString GetIndexString() const;		

	// 获取不考虑角度的文字的bounds，用于点和标签同时显隐，且文字不需要旋转的情况
	UGRect2D GetBoundsWithoutAngle() const;
	
	//! \brief 转化为线对象
	//! \param geoLine[out] 得到的线对象
	//! \param nSegmentCount 当曲线转化为线时,每一段用多少个点来模拟
	//! \remarks 文本对象无法转化,固定返回false
	//! \return 返回false
	virtual GEOMETRY_APIF UGbool ConvertToLine( UGGeoLine &geoLine, UGlong nSegmentCount = 0 ) const;
	
	//! \brief 转化为面对象
	//! \param geoRegion[out] 得到的面对象
	//! \param nSegmentCount 当曲面转化为面时,每一段用多少个点来模拟
	//! \remarks 文本对象无法转化,固定返回false
	//! \return 返回false
	virtual GEOMETRY_APIF UGbool ConvertToRegion( UGGeoRegion &geoRegion, UGlong nSegmentCount = 0 ) const;

#if PACKAGE_TYPE==PACKAGE_UGC
		
	//! \brief 生成XML格式的信息
	//! \param versionType GML的版本
	//! \param nPace 缩进的空格数目,使得xml字符串更加易于人工阅读; 
	//! 主要用于如果返回的xml字符串是一个大标签中的一部分, 通过指定空格数目,可以使层次清晰
	//! \return 返回指定GML版本的xml字符串
	virtual GEOMETRY_APIF UGString ToXML(GMLVersionType versionType, UGint nPace = 0) const;
	
	//! \brief 从XML字符串中生成Geometry
	//! \param strXML xml格式字符串
	//! \param versionType xml字符串的GML版本
	//! \return 返回指定GML版本的xml字符串
	virtual GEOMETRY_APIF UGbool FromXML(const UGString& strXML, GMLVersionType versionType);
#if !defined SYMBIAN60 && !defined  OS_ANDROID && !defined (IOS) 
	//! \brief Geometry与Element的转换
	//! \param pElement,返回的pElement指针在函数内分配内存,外面负责删除，否则会有内存泄漏
	virtual GEOMETRY_APIF UGbool ToElement(OgdcElement*& pElement)  const;

	//! \brief Geometry与Element的转换
	//! \param pElement,传入的Element，如果无法转换则返回False，否则返回True
	virtual GEOMETRY_APIF UGbool FromElement(const OgdcElement* pElement);
#endif
	
#endif // end declare ugc sdk

public:
	//added by xielin ^_^ 编辑相关函数,把编辑和geometry统一
	
	//! \brief 获得编辑对象的句柄的数目
	//! \remarks 对于固定句柄对象为9包括了改变大小的8个控制点和一个旋转控制点
     virtual GEOMETRY_APIF UGint  GetHandleCount();

	
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
	 virtual GEOMETRY_APIF UGint GetHandle( UGint nHandle, UGPoint2D& point);


	//! \brief 移动控制点到指定坐标,主要用于绘制对象，修改对象节点等操作
	//! \param nHandle 控制点句柄
	//! \param point 控制点坐标
	 virtual GEOMETRY_APIF void MoveHandle( UGint nHandle, const UGPoint2D& pnt2D,UGEditType::EditToolShape nEditType=UGEditType::ET_Default);

	//! \brief 节点编辑时可用来插入一个控制点,也可以用于绘制对象时加入节点
	//! \param nHandle 插入位置
	//! \param pnt2D 插入点的地理坐标
	virtual GEOMETRY_APIF void AddHandle( UGint nHandle, const UGPoint2D& pnt2D,UGEditType::EditToolShape nEditType=UGEditType::ET_Default);
	
	//! \brief 删除编辑对象的指定节点
	//! \param nHandle 节点句柄
     virtual GEOMETRY_APIF UGuint DelNode( UGint nHandle );

	//! \brief 计算对象绘制时的辅助线，返回的是一个复合对象
	//! \param pPoints 点串
	//! \param nPntCount  点的个数
	//! \param nEditType 绘制对象的类型，参考UGEditType
	//! \param pStyle	 辅助线的风格，如果传入为NULL，用默认的风格
     virtual GEOMETRY_APIF UGGeometry*  CalAssistantLine(const UGPoint2D* pPoints,UGint nPntCount,UGEditType::EditToolShape nEditType,UGStyle* pStyle=NULL);


	//! \brief 计算对象绘制时的辅助点，返回的是点的数目,nHandleType返回的是辅助点的类型,参考UGEditType::EditHandleType
	//! \param pPoints 点串
	//! \param nPntCount  点的个数
	//! \param eAssistantPoints 点串,返回的辅助点点串
	//! \param nEditType 绘制对象的类型，参考UGEditType
	 virtual GEOMETRY_APIF void CalAssistantPoints(const UGPoint2D* pPoints,UGint nPntCount,UGArray<UGEditType::EditAssistantPoint>& eAssistantPoints,UGEditType::EditToolShape nEditType);

	 //! \brief 根据传入的点串生成对象，主要用于编辑中的参数化等没有内部数组保存点串的对象，
	 //也用于自定义对象,对于有内部数组保存点串的对象，该函数直接return既可，比如GeoLine
	 //GeoLine等对象就直接用AddHandle来生成，效率比较高 
	//! \param pPoints 点串
	//! \param nPntCount  点的个数
     virtual GEOMETRY_APIF void MakeWithPoints(UGPoint2D* pPoints,UGint nPntCount,UGEditType::EditToolShape nEditType=UGEditType::ET_Default);

	//! \brief 获取对象需要捕捉的点串
	//! \param aryPnts 点串，传出去的要捕捉的点串
	//! \return 如果是true表示有需要捕捉的点串，如果返回false表示该对象没有需要捕捉的点串
	virtual GEOMETRY_APIF UGbool GetSnapPoints(UGArray<UGPoint2D>& aryPnts);

	//! \brief 获取对象需要捕捉的线段数组
	//! \param aryLines 需要捕捉的线数组（如果有多个part，那么aryLines就应该有多少个count）
	//! \return 如果是true表示有需要捕捉的线，如果返回false表示该对象没有需要捕捉的线
	virtual GEOMETRY_APIF UGbool GetSnapLines(UGLineArray& aryLines);

	//added by xielin ^_^ 编辑相关函数,把编辑和geometry统一 end

#if !defined _UGUNICODE
	//! \brief 设置几何对象中字符串的Charset,主要针对文本对象，复合对象中的文本字符串
	virtual GEOMETRY_APIF void SetTextCharset(UGString::Charset nCharset);

	//! \brief 改变几何对象中字符串的Charset,主要针对文本对象，复合对象中的文本字符串,转换字符串内容
	virtual GEOMETRY_APIF void ChangeTextCharset(UGString::Charset nCharset);
#endif
	UGbool IsBoundsAndAnchorCalculated() { return m_bBoundsAndAnchorCalculated; }
	void SetBoundsAndAnchorCalculated(UGbool value) { m_bBoundsAndAnchorCalculated = value; }

	//! \brief 设置当前文本对象的真实Bounds
	GEOMETRY_APIF void SetRealBounds(const UGRect2D& bounds);
	//! \brief 获取当前文本对象的真实Bounds
	GEOMETRY_APIF UGRect2D GetRealBounds();
	//! \brief 判断是否有真实的Bounds
	GEOMETRY_APIF UGbool HadRealBounds();
	//! \brief 锚点和实际Bounds直接的偏移量
	UGSize2D m_szAnchorOffset;

private: //! 内部地理数据
	//! \brief 文本头
	UGTextStyle m_textStyle;
	
	//! \brief 子对象定位点
	UGArray<UGPoint2D> m_Points;
	
	//! \brief 子对象数组
	UGArray<UGSubText> m_SubTexts;

	//! \brief 当前文本对象的真实Bounds，必须通过外部调用Graphics来计算
	UGRect2D m_rcRealBounds;

	//! \brief 返回当前文本对象是否有真实Bounds，方便外面判断是否获取
	// 如果当前是BoundsDirty的时候也认为是false的
	UGbool m_bHasRealBounds;

	UGbool m_bBoundsAndAnchorCalculated;

protected: //! 子类需要实现的一些不公开函数
	//! \brief 
	virtual GEOMETRY_APIF UGRect2D ComputeBounds() const;
	/*virtual GEOMETRY_APIF UGRect2D GetBoundsInside() const;
	virtual GEOMETRY_APIF void SetBoundsInside(const UGRect2D & rcBounds);*/
	
	//! \brief 存储到Stream中
	virtual GEOMETRY_APIF UGbool SaveGeoData(UGStream& stream, UGDataCodec::CodecType eCodecType,UGbool bSDBPlus = FALSE)  const;
	//! \brief 从Stream中生成Geometry
	virtual GEOMETRY_APIF UGbool LoadGeoData( UGStream& stream , UGDataCodec::CodecType eCodecType = UGDataCodec::encNONE,UGbool bSDBPlus = FALSE );
	
private:
	//! \brief 计算一个子对象的Bounds
	//! \param bConsiderRotation : 是否考虑旋转
	//! \param bAlignOffset : 是否考虑Align == 3,4,5 时, 文本显示位置的偏移	
	UGRect2D ComputeSubBounds(UGint nSubIndex, 
		UGbool bConsiderRotation=true, UGbool bAlignOffset=false) const;
	
	//! \brief 计算文本的Bounds（参数已经提供足够多的信息）
	static UGRect2D ComputeTextBounds(const UGString& strText,
		double dFontHeight, double dFontWidth, 
		const UGPoint2D& pntAnchor, 
		UGint nAlign, double dRadian=0, UGbool bAlignOffset=false);
	
	//! \brief 由于文本对齐方式带来的偏移
	//! \remarks 对齐方式为3 4 5 时，并不是中央对齐，而是Baseline
	static UGRect2D AlignOffset(const UGRect2D& rcBounds, 
		UGint nAlign, double dFontHeight);
	
	// 文本在 Align == 3 4 5 时, 显示并不是居中的, 而是BaseLine, 
	// 显示位置要向上 偏移一些, 这里用一个统一的函数控制偏移比例
	static double GetTextAlignOffset(UGbyte nAlign, double dFontHeight);
	
	//! \brief 得到最长一行的字符个数,  和整个字符串的行数
	//! \return 返回值:	UGSize::cx = 最长一行的字符个数
	//!					UGSize::cy = 整个字符串的行数	
	static UGSize GetTextSize(const UGString& strText);
	
	//!  自身不变, 返回旋转之后的矩形
	//! \param bAlign: 对齐方式, 同bCenterType
	//! \param dRadian: 单位是弧度
	static UGRect2D UnionRotate(const UGRect2D& rcBounds, UGbyte bAlign, UGdouble dRadian); 
	
	//! \brief 旋转一个Rect2D，结果通过pPntResult返回，pPntResult必须有4个点的空间
	static void RotateRect(const UGRect2D& rcBounds, UGbyte bCenterType, UGdouble dRadian,
							UGPoint2D *pPntResult);
	
	//! \brief 获取子对象的边界点
	void GetVertexes(UGint nIndex, UGPoint2D* pPoints) const;
};

}

#endif

