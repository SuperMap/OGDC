#ifndef SUPERMAP_GEOMETRYCAD_UGGEOCOMPOUNDREGION_HEADER
#define SUPERMAP_GEOMETRYCAD_UGGEOCOMPOUNDREGION_HEADER


#include "Geometry/UGGeometry.h"

namespace UGC {

//! \brief 复合面几何对象


class  GEOMETRYCAD_API UGGeoCompoundRegion : public UGGeometry
{
public:
	UGGeoCompoundRegion(void);
	virtual ~UGGeoCompoundRegion(void);

public:
	//! \brief 清空地理数据
	virtual   void Clear();

	//! \brief 得到维度, 固定为2
	//! \return 得到维度,固定为2
	virtual   UGint GetDimension() const;


	//! \brief 得到类型, 固定为 GeoCompoundParamRegion
	virtual   Type GetType() const;


	//! \brief 得到内点（在Geometry内部，靠近中央的点）
	//! \remarks 为点数最多的子对象 的中间点(中间个数的点, 而不是中间距离的点)
	virtual   UGPoint2D GetInnerPoint() const;


	//! \brief 点击测试, 用于对象点选
	//! \param pntHitTest 用来测试选中的点
	//! \param dTolerance 容限
	//! \return 选中返回true, 否则返回false
	virtual   UGbool HitTest( const UGPoint2D &pntHitTest, UGdouble dTolerance) const;	

	//! \brief 得到空间数据, 主要用于需要按照维度相同时
	//! 来做统一处理的时候, 例如一些空间判断和操作的算法
	//! \param pPoints 传出的点坐标数组[out]。
	//! \param pPolyCount 传出的每个子对象点个数的数组[out]。
	//! \param nSubCount 传出的子对象个数[out]。
	//! \param nSegmentCount 对参数化对象, 每个弧段分割点的个数[in]。
	//! \remarks 参数采用了智能指针的技术,调用者不用释放内存空间
	//! \return 成功返回true,否则返回false。
	virtual   UGbool GetSpatialData( UGAutoPtr<UGPoint2D>& pPoints, 
		UGAutoPtr<UGint>& pPolyCount, UGint& nSubCount, UGint nSegmentCount=0) const;


	//! \brief 转化为面对象。
	//! \param geoRegion [out] 得到的面对象。
	//! \param nSegmentCount [in] 参数化对象每段的插值的点数。
	//! \return 面对象加入子对象成功,返回true。
	//! \remarks 内部实现就是把GeoParametricLine中的每一个子对象转化为Region,再加入到geoRegion引用参数中。
	virtual  UGbool ConvertToRegion( UGGeoRegion &geoRegion, UGint nSegmentCount = 0 ) const;		

	//! \brief 转化为线对象。
	//! \param geoLine [out] 得到的线对象。
	//! \param nSegmentCount [in] 参数化对象每段的插值的点数。
	//! \return 线对象加入子对象成功,返回true。
	//! \remarks 内部实现就是把GeoParametricLine中的每一个子对象转化为Line,再加入到geoLine引用参数中。
	virtual  UGbool ConvertToLine( UGGeoLine &geoLine, UGint nSegmentCount = 0 ) const;	



	//! \brief 缩放
	//! \param dRatioX X方向缩放比率
	//! \param dRatioY Y方向缩放比率
	virtual   void Inflate( UGdouble dRatioX, UGdouble dRatioY);

	//! \brief 缩放
	//! \param dRatio 缩放比率(X,Y方向相同)
	virtual   void Inflate( UGdouble dRatio);

	//! \brief 平移
	//! \param dX 平移的X偏移量
	//! \param dY 平移的Y偏移量
	virtual   void Offset( UGdouble dX, UGdouble dY);

	//! \brief 平移
	//! \param 偏移量(X,Y方向相同)
	virtual   void Offset( UGdouble dOffset);

	//! \brief 平移
	//! \param 平移的偏移量(分别用UGSize2D中的x,y代表X,Y方向的偏移量)
	virtual   void Offset( const UGSize2D &szOffset);


	//! \brief 旋转
	//! \param pntOrigin 旋转的中心点(锚点,不动点)
	//! \param dAngle 旋转角度
	virtual   void Rotate(const UGPoint2D& pntOrigin, UGdouble dAngle);

	//! \brief 旋转,增加一些参数,以便内部可以不用进行cos,sin, 在批量处理的时候有利于提高效率
	//! \remarks dAngle看似多余,但GeoText需要,所以要保留
	//! \param pntOrigin 旋转的中心点(锚点,不动点)
	//! \param dCosAngle 旋转的cos值
	//! \param dSinAngle 旋转的sin值
	//! \param dAngle 旋转角度
	virtual   void Rotate( const UGPoint2D& pntOrigin, UGdouble dCosAngle, UGdouble dSinAngle, double dAngle = 0);


	//! \brief 调整几何数据,限定在新的Bounds中
	//! \param rcNewBounds 新的,限定范围的Bounds
	//! \return 成功返回true,失败返回false
	virtual   UGbool Resize(const UGRect2D& rcNewBounds); 

	UGbool IsValidSubGeometry( const UGGeometry* pGeometry) const ;

	//! \brief 几何对象是否有效
	//! \return 有效返回true,无效返回false
	virtual   UGbool IsValid()const;

	//! \brief 得到子对象的个数
	//! \return 返回子对象的个数
	virtual   UGint GetSubCount() const;

//! \brief 生成XML格式的信息
	//! \param versionType GML的版本
	//! \param nPace 缩进的空格数目,使得xml字符串更加易于人工阅读; 
	//! 主要用于如果返回的xml字符串是一个大标签中的一部分, 通过指定空格数目,可以使层次清晰
	//! \return 返回指定GML版本的xml字符串
	virtual   UGString ToXML(GMLVersionType versionType, UGint nPace = 0) const;

	//! \brief 从XML字符串中生成Geometry
	//! \param strXML xml格式字符串
	//! \param versionType xml字符串的GML版本
	//! \return 返回指定GML版本的xml字符串
	virtual   UGbool FromXML(const UGString& strXML, GMLVersionType versionType);

	//! \brief 分解复合对象
	//! \param arrGeometry [out] 分解后得到的子对象的数组
	//! \param bToLeaf 是否分解到叶对象, 默认为false
	//! \remarks 复合对象是一颗树, 如果不分解到叶对象, 则传出的arrGeometry仍然可能存在复合对象(非叶)
	//  如果bToLeaf =true, 则传出的arrGeometry中肯定没有 复合对象
	//! \attention 取出来的几何对象指针数组, 这些指针仍然是指向GeoCompound内部的内存地址, 外面使用后不要delete;
	//! 在GeoCompound发生变化(如插入子对象,或者清空等)后,不要再使用这些指针,因为有可能为野指针
	  UGbool Divide(UGArray<UGGeometry *> &arrGeometry, UGbool bToLeaf=false);

	//! \brief 按照子对象Bounds的大小来重新排列复合对象中各个子对象的上下顺序。
	//! \param bMoveUptheMinObj [in] 为true时,小对象在上面;为false时小对象在下面。
	  void Adjust(UGbool bMoveUptheMinObj = true);  


	//! \brief 复制构造复合对象。
	//! \param geoCompound [in] 已有的复合对象。
	//! \return 已有复合对象非法,返回false。
	  UGbool Make(const UGGeoCompoundRegion& geoCompound);

	//! \brief 在最后增加一个子对象。
	//! \param pSubRegion [in] 子对象指针。
	//! \return pSubRegion非法, 返回false。
	  UGbool AddSub( UGGeometry *pGeometry );	


	//! \brief 插入子对象。
	//! \param nIndex [in] 要插入的位置索引。
	//! \param pPoints [in] 点串指针。
	//! \param nPntCount [in] 点串中点的个数。
	//! \return 点个数小于2或者索引位置非法,返回false。
	  UGbool InsertSub(UGint nSubIndex, UGGeometry *pGeometry);	

	//! \brief 给指定的位置设置结合对象。
	//! \param nIndex [in] 指定的位置索引。
	//! \param pGeometry [in] 几何对象指针。
	//! \return 几何对象指针不为NULL,返回true。
	//! \remarks 指定位置上原有的几何对象将被delete掉。
	  UGbool SetSub(UGint nIndex, UGGeometry *pGeometry);


	//! \brief 删除子对象。
	//! \param nIndex [in] 要删除的子对象的位置索引。
	//! \return 索引非法, 返回false。
	  UGbool DelSub( UGint nIndex );	

	//! \brief 得到某个子对象, 并构造得到GeoParametricLine。
	//! \param nSubIndex [in] 子对象索引
	//! \return 子对象，索引非法, 返回Null。
	  UGGeometry* GetSub(UGint nSubIndex ) ;	
	  UGGeometry* GetSubPtr( UGint nSubIndex ) ;

	//! \brief 得到对象面积
	//! \return 返回面积
	virtual   UGdouble GetArea() const;

	//! \brief 得到对象周长
	//! \return 返回周长
	virtual   UGdouble GetLength() const;	

	//protected: //! 子类需要实现的一些不公开函数
	virtual   UGRect2D ComputeBounds() const;

	virtual   UGint GetDataSize() const;

	//! \brief 保存到存储流重
	//! \param stream 存储用的流, 调用之前应该先Open
	//! \param eCodecType 存储的编码方式
	//! \param bIgnoreStyle 是否忽略对象风格
	//! \return 成功返回true,失败返回false
	virtual  UGbool Save(UGStream& stream, UGDataCodec::CodecType eCodecType=UGDataCodec::encNONE, 
		UGbool bIgnoreStyle = TRUE) const;

	UGbool SaveGeoData(UGStream& stream, UGDataCodec::CodecType eCodecType,UGbool bIgnoreStyle,UGbool bSDBPlus /* = FALSE */) const;

	virtual   UGbool SaveGeoData(UGStream& stream, UGDataCodec::CodecType eCodecType,UGbool bSDBPlus /* = FALSE */) const;

	virtual   UGbool LoadGeoData( UGStream& stream , UGDataCodec::CodecType eCodecType /* = UGDataCodec::encNONE */,UGbool bSDBPlus /* = FALSE */ );


private:

	//存储其子对象
	UGArray<UGGeometry*> m_ChildRegion;

	//预留字段
	UGuint m_SRID;

};

}
#endif