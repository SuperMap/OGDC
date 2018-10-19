/*! \file	 UGGeoTextEPS.h
 *  \brief	 清华山维 文本对象类
 *  \author	 ugc team
 *  \attention 
 *  Copyright(c) 2000-2010 SuperMap Software Co., Ltd.<br>
 *  All Rights Reserved
 *  \version $Id: UGGeoTextEPS.h,v 1.11 2010/03/17 06:16:11 majt Exp $
 */


#ifndef UGGEOTEXTEPS_H
#define UGGEOTEXTEPS_H

#include "GeometryCAD/UGGeometryEPS.h"
#include "Geometry/UGGeoText.h"
#include "Toolkit/UGTextStyle.h"

namespace UGC {

class GEOMETRYCAD_API UGGeoTextEPS : public UGGeometryEPS
{
private: //! 内部地理数据
	UGString  m_strNote;			//注记内容
	UGfloat	  m_fStringAngle;		//串的角度,单字时等于字角度(度)//m_nAngle
	UGbyte	  m_byteHS;			//对齐方式//m_nStyle

	//以下为原m_FontInfo中记录信息
	UGString  m_szNumber;		//分类号
	UGString  m_strFontName;	//字体名
	UGshort   m_unFontWidth;	//字宽
	UGshort   m_unFontHeight;	//字高
	UGshort   m_unFontInterval;	//字隔
	UGshort   m_unWeight;		//字磅值
	UGfloat   m_fIlaticAngle;	//斜体,字的右下角到字的右上角与底线的夹角(度) 
	UGfloat   m_fDownAngle;		//耸肩,字的左下角到字的右下角与底线的夹角(度)
	/*受操作系统功能限制，斜体只能实现斜72度，耸肩在Windows98还不能实现*/
	UGfloat	  m_fWordAngle;		//字角度,字底线与正东的夹角(度)
    UGbool	  m_bUnderLine;		//是否画下画线
	
	//COLORREF  m_rgbColor;		    //文字颜色
	//使用基类中的m_Color
	UGColor   m_ColorBorder;		//边框颜色
	UGColor   m_ColorFill;		//填充颜色
	
	UGshort   m_nBorderStyle;       //边框样式
	UGshort   m_nLineWidth;         //边框线宽
	UGshort   m_nThickness;         //边框厚度(棱台, 阴影等)

	UGString  m_strMemo;	     	//备注

	UGTextStyle	m_textStyle;

public:
	//! \brief 构造函数
	UGGeoTextEPS();
	UGGeoTextEPS(const UGGeoTextEPS& geoText);
	
	//! \brief 析构函数
	virtual ~UGGeoTextEPS();
public: 

	//设置对齐方式
 	//进口参数: Alignment(对齐方式)
	void SetAlignment(UGshort Alignment);
	//获得对齐方式
	//返回值: 对齐方式
	UGshort GetAlignment() const;

	// 设置多点注记定位方式
 	//进口参数: nPosType(定位方式) 
	//          0 沿路径均匀分布
	//          1 路径逐点分布
	void SetPosType( UGint nPosType );
	//获得点注记定位方式
	//返回值: 定位方式
	UGint  GetPosType() const;

	//获得注记内容
	//返回值: 注记内容
	UGString GetStringText() const;
	//设置注记内容
 	//进口参数: strNote(新注记内容)
	//返回值: 原注记内容
	UGString SetStringText( UGString strNote);

	//获得注记串的角度
	//返回值: 注记串的角度
	UGfloat		GetStringAngle() const;
	//设置注记串的角度
 	//进口参数: fAngle(串的角度)
	//返回值: 原串的角度
	UGfloat		SetStringAngle(UGfloat fAngle );

	//获得字体名称
	//返回值: 字体名称
	UGString     GetFontName() const;
	//设置字体名称
 	//进口参数: strName(字体名称)
	//返回值: 原字体名称
	UGString     SetFontName(UGString strName);

	//获得字体字宽
	//返回值: 字体字宽
	UGshort		GetFontWidth() const;
	//设置字体字宽
 	//进口参数: nWidth(字体字宽)
	//返回值: 原字体字宽
	UGshort		SetFontWidth(UGshort nWidth);

	//获得字体字高
	//返回值: 字体字高
	UGshort		GetFontHeight() const;
	//设置字体字高
 	//进口参数: nHeight(字体字高)
	//返回值: 原字体字高
	UGshort		SetFontHeight(UGshort nHeight);

	//获得字体字磅值
	//返回值: 字体字磅值
	UGshort       GetFontWeight() const;
	//设置字体字磅值
 	//进口参数: nWeight(字体字磅值)
	//返回值: 原字体字磅值
	UGshort       SetFontWeight(UGshort nWeight);
 
	//获得字体字隔
	//返回值: 字体字隔
	UGshort		GetFontInterval() const;
	//设置字体字隔
 	//进口参数: nInterval(字体字隔)
	//返回值: 原字体字隔
	UGshort		SetFontInterval(UGshort nInterval);

	//获得字角度
	//返回值: 字角度
	UGfloat		GetWordAngle() const;
	//设置字体字角度
 	//进口参数: fAngle(字角度)
	//返回值: 原字角度
	UGfloat		SetWordAngle(UGfloat fAngle);

	//获得字体分类号
	//返回值: 字体分类号
	UGString GetFontClass() const;
	//设置字体分类号
 	//进口参数: szClass(字体分类号)
	void SetFontClass(UGString szClass);
	//设置字体分类号
 	//进口参数: nNumber(字体分类号)
	void SetFontClass( UGuint nNumber);

	//获得斜体(字的右下角到字的右上角与底线的夹角)
	//返回值: 斜体
	UGfloat GetIlaticAngle() const;
	//设置斜体(字的右下角到字的右上角与底线的夹角)
 	//进口参数: fIlaticAngle(斜体)
	//返回值: 原斜体
	UGfloat SetIlaticAngle(UGfloat fIlaticAngle);

	//获得耸肩(字的左下角到字的右下角与底线的夹角)
	//返回值: 耸肩
	UGfloat GetDownAngle() const; 
	//设置耸肩(字的左下角到字的右下角与底线的夹角)
 	//进口参数: fWownAngle(耸肩)
	//返回值: 原耸肩
	UGfloat SetDownAngle(UGfloat fWownAngle) ;  
	//获得是否画下画线
	//返回值: TRUE表示画下划线,FALSE表示不画下划线
	UGbool GetUnderLine() const;
	//设置是否画下画线
 	//进口参数: nFlags(标识)
	//返回值: 原是否画下画线标识
	UGbool SetUnderLine(UGbool nFlags); 

	//获得备注信息
	//返回值: 备注信息
	UGString GetFontMemo() const;
	//设置备注信息
 	//进口参数: strMemo(备注信息)
	void SetFontMemo(const UGString& strMemo );

	
	//! \brief 清空地理数据
	virtual void Clear();
	
	//! \brief 得到维度, 固定为-1
	virtual UGint GetDimension() const;
	
	//! \brief 得到类型,固定为GeoText
	virtual Type GetType() const;
	
	//! \brief 得到内点（在Geometry内部，靠近中央的点）
	//! \return 返回第一个子对象的锚点
	virtual UGPoint2D GetInnerPoint() const;
	
	//! \brief 点击测试, 用于对象点选
	//! \param pntHitTest 用来测试选中的点
	//! \param dTolerance 容限
	//! \return 选中返回true, 否则返回false
	virtual UGbool HitTest( const UGPoint2D &pntHitTest, UGdouble dTolerance) const;	

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
	//! \brief 得到子对象的个数
	//! \return 返回子对象的个数
	virtual UGint GetSubCount() const;
	
	//! \brief 几何对象是否有效
	//! \return 有效返回true,无效返回false
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
	virtual void Rotate( const UGPoint2D& pntOrigin, UGdouble dCosAngle, UGdouble dSinAngle, double dAngle = 0);
	
	//! \brief 调整几何数据,限定在新的Bounds中
	//! \param rcNewBounds 新的,限定范围的Bounds
	//! \return 成功返回true,失败返回false
	virtual UGbool Resize(const UGRect2D& rcNewBounds);
	
//public:
//	//! \brief 根据投影转化器的设置和转化方向要求, 进行投影转化
//	//! \param pPJTranslator 投影转化器
//	//! \param bForward 转化方向, true代表从UGRefTranslator的源(Src)转化到目的(Des), false则相反
//	virtual void PJConvert( UGRefTranslator *pPJTranslator, UGbool bForward = true);
//	virtual UGbool PJConvert( UGRefTranslator *pPJTranslator, UGbool bForward ,UGbool bClip );
//
//	//! \brief 从经纬坐标系统转化为投影坐标系统
//	//! \param pCoordSys 投影坐标系统
//	virtual void PJForward( UGPrjCoordSys *pCoordSys );
//
//	//! \brief 从投影坐标系统转化为经纬坐标
//	//! \param pCoordSys 投影坐标系统
//	virtual void PJInverse( UGPrjCoordSys *pCoordSys );
	
public://需要重新实现的方法
	
	//! \brief 设置风格
	//! \param pStyle 风格指针
	//! \remarks 如果pStyle==NULL,则Geometry内部风格指针被delete; 否则会复制一份风格
	//! 对于文本对象等, 设置风格不起作用,因为文本对象有自己的TextStyle
	virtual void SetStyle(const UGStyle* pStyle);
	
	virtual UGint GetDataSize() const;

	const UGString& GetNote();
	void SetNote(UGString& strNote);

public: //! group 创建文本对象
	//! \brief 通过文本内容/锚点/旋转角度来构造文本对象。
	//! \param strText [in] 文本内容。
	//! \param pntAnchor [in] 锚点。
	//! \param dAngle [in] 旋转角度。
	//! \return 返回true。
	UGbool Make(const UGString& strText, const UGPointEPS& pntAnchor, UGdouble dAngle=0.0);

	//! \brief 通过复制构造文本对象。
	//! \param geotext [in] 已有的文本对象。
	//! \return 返回true。
	UGbool Make(const UGGeoTextEPS& geotext);	

	//! \brief 设置文本风格
	//! \param textStyle [in] 文本风格。
	void SetTextStyle(const UGTextStyle& textStyle, UGbool bChangeSubTextAngle = false);	

	//! \brief 得到文本风格。
	//! \return 返回文本风格。
	const UGTextStyle& GetTextStyle();	
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
	UGString GetIndexString() const;		
	
	//! \brief 转化为线对象
	//! \param geoLine[out] 得到的线对象
	//! \param nSegmentCount 当曲线转化为线时,每一段用多少个点来模拟
	//! \remarks 文本对象无法转化,固定返回false
	//! \return 返回false
	virtual UGbool ConvertToLine( UGGeoLine &geoLine, UGlong nSegmentCount = 0 ) const;
	
	//! \brief 转化为面对象
	//! \param geoRegion[out] 得到的面对象
	//! \param nSegmentCount 当曲面转化为面时,每一段用多少个点来模拟
	//! \remarks 文本对象无法转化,固定返回false
	//! \return 返回false
	virtual UGbool ConvertToRegion( UGGeoRegion &geoRegion, UGlong nSegmentCount = 0 ) const;

	UGbool ConvertToGeoText(UGGeoText &geoText, UGlong nSegmentCount = 0 );

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
	virtual UGbool FromXML(const UGString& strXML, GMLVersionType versionType);
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


	//! \brief 计算对象绘制时的辅助点，返回的是点的数目,nHandleType返回的是辅助点的类型,参考UGEditType::EditHandleType
	//! \param pPoints 点串
	//! \param nPntCount  点的个数
	//! \param eAssistantPoints 点串,返回的辅助点点串
	//! \param nEditType 绘制对象的类型，参考UGEditType
	 virtual void CalAssistantPoints(const UGPoint2D* pPoints,UGint nPntCount,UGArray<UGEditType::EditAssistantPoint>& eAssistantPoints,UGEditType::EditToolShape nEditType);

	 //! \brief 根据传入的点串生成对象，主要用于编辑中的参数化等没有内部数组保存点串的对象，
	 //也用于自定义对象,对于有内部数组保存点串的对象，该函数直接return既可，比如GeoLine
	 //GeoLine等对象就直接用AddHandle来生成，效率比较高 
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
	
private:
	
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

