// UGImgToolkit.h: interface for the UGImgToolkit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UGIMGTOOLKIT_H__21224E42_7983_49CB_99BC_9792CA94305F__INCLUDED_)
#define AFX_UGIMGTOOLKIT_H__21224E42_7983_49CB_99BC_9792CA94305F__INCLUDED_

#pragma warning (disable:4244)

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Toolkit/UGPalette.h"
#include "Toolkit/UGMarkup.h"
#include "Element/OgdcRasterBlock.h"
#include "Geometry/UGDataCodec.h"
#include "Geometry/UGGeoRegion.h"

#define IMAGECONFIGEFILE_HEADER			_U("SuperMapImageConfig")
#define IMAGECONFIGEFILE_PRJ			_U("SuperMapProjection")
#define IMAGECONFIGEFILE_CLIPREGION		_U("SuperMapClipRegion")
#define IMAGECONFIGEFILE_STATINFO		_U("SuperMapStatisticsInfo")

namespace UGC
{
//栅格数据集统计信息(为了给FileParser和Engine用，放Toolkit里)
class TOOLKIT_API UGRasterStatisticsInfo
{
	public:
		UGRasterStatisticsInfo();
		virtual ~UGRasterStatisticsInfo();
public:
		void SetMax(UGdouble max);
		UGdouble GetMax();
		void SetMin(UGdouble min);
		UGdouble GetMin();
		void SetMiddle(UGdouble mid);
		UGdouble GetMiddle();
		void SetAverage(UGdouble average);
		UGdouble GetAverage();
		void SetStddev(UGdouble stddev);
		UGdouble GetStddev();
		void SetVariance(UGdouble variance);
		UGdouble GetVariance();
		UGbool HasMax();
		UGbool HasMin();
		UGbool HasStddev();
		UGbool HasVariance();
		UGbool HasMiddle();
		UGbool HasAverage();
	private:
		UGdouble m_dMax;//最大值
		UGdouble m_dMin;//最小值
		UGdouble m_dMiddle;//中值
		UGdouble m_dAverage;//平均值
		UGdouble m_dStddev;//标准差
		UGdouble m_dVariance;//方差
		 
		//! \brief 统计信息不一定都能取到，所以设置以下标识，用来判断哪些成员的信息是有效的
		UGbool m_bHasMin;      // 最小值是否有效
		UGbool m_bHasMax ;     // 最大值是否有效
		UGbool m_bHasMiddle;   // 中值是否有效
		UGbool m_bHasAverage;  // 平均值是否有效
		UGbool m_bHasStddev;   // 标准差是否有效
		UGbool m_bHasVariance; // 方差是否有效
	public:
		UGArray<UGdouble> m_arrMost;//众数
		UGArray<UGdouble> m_arrLeast;//稀数
		UGbool IsDirty;
} ;

	  //!  \brief Toolkit
	struct UGCMYK 
	{
	  //!  \brief Toolkit
		UGuint CValue;
	  //!  \brief Toolkit
		UGuint MValue;
	  //!  \brief Toolkit
		UGuint YellowVal;
	  //!  \brief Toolkit
		UGuint KValue;
	};


class TOOLKIT_API UGImgToolkit  
{
public:
	  //!  \brief Toolkit
	enum ReflectType
	{
	  //!  \brief Toolkit
		XAXES,	//水平中线对称
	  //!  \brief Toolkit
		YAXES,	//垂直中线对称
	  //!  \brief Toolkit
		ORIGIN	//中点对称
	};
public:
	  //!  \brief Toolkit
	UGImgToolkit();
	  //!  \brief Toolkit
	virtual ~UGImgToolkit();

public:
	//==============================================================
	//根据象素格式和象素宽度计算DWORD对齐的按照BYTE计的实际需要宽度.
	//! \brief
	  //!  \brief Toolkit
	static UGint AlignWidth(OGDC::PixelFormat PixelFormat,UGint lPixelWidth);
	//! \brief
	  //!  \brief Toolkit
	static UGint AlignWidthByte(OGDC::PixelFormat PixelFormat,UGint lPixelWidth);
	//! \brief
	  //!  \brief Toolkit
	static UGint AlignWidthWord(OGDC::PixelFormat PixelFormat,UGint lPixelWidth);
	
	//! \brief
	  //!  \brief Toolkit
	static UGushort BitCount(OGDC::PixelFormat PixelFormat);		//根据格式计算位数.
	//! \brief
	  //!  \brief Toolkit
	static UGushort PaletteSize(OGDC::PixelFormat PixelFormat);	//得到需要的调色板大小.
	//! \brief
	  //!  \brief Toolkit
	static OGDC::PixelFormat Bit2Format(UGushort wBitCount);		//根据位数返回格式.
	
	//! \brief
	//!  \brief 将GIF文件解析问RGBA格式
	static UGbool ImageGIFToRGBA(UGImageData& imageData, UGbyte nAlpha = 255); 
	//! \brief
	  //!  \brief Toolkit
	static UGbool ImageDataToRGBA(UGImageData& imageData, UGbyte nAlpha = 255);  // 先给三维用吧以后再完善。
	//! \brief 32位Colors颜色转化为ImageData形式，供组件使用
	//! \param imageData	 要转成的ImageData格式[in/out]
	//! \param nWidth		 要转成的ImageData的宽度[in]
	//! \param nHeight		 要转成的ImageData的高度[in]
	//! \param pData		 传入的32位颜色串[in]
	//! \return 转换是否成功
	//! \remarks 传入的为32位字符串，长度为nWidth*32/4*nHeight,转换后的Imagedata也为232位格式
	//! \brief
	static UGbool ColorsToImageData(UGImageData& imageData, UGint nWidth,UGint nHeight,UGColor* pData);  // 先给三维用吧以后再完善。
	  //!  \brief Toolkit
	//! \brief
	static UGuint GetValue(const UGbyte* pByte, UGint nWidthBytes, OGDC::PixelFormat pixelFormat, UGuint x, UGuint y);
	//! \brief
	  //!  \brief Toolkit
	static void SetValue(UGbyte* pByte, UGint nWidthBytes, OGDC::PixelFormat pixelFormat, UGuint x, UGuint y, UGuint nValue);
	//! \brief 镜像ImageData。
	//! \param arySrcImgData [in]。
	//! \param nReflectType 镜像类型[in]。
	//! \return 。
	//! \remarks 。
	//! \attention 。
	static UGArray<UGImageData*> ReflectImageData(UGArray<UGImageData*>& arySrcImgData,ReflectType nReflectType);	
	
	//! \brief 线性拉伸算法 像素拉伸
	static UGbool BilinearInterpolation(UGColor& nColor ,UGArray<UGdouble>& arrValue
		, ImgColorSpace nColorSpace,UGArray<UGdouble>&dMinZs,
		UGArray<UGdouble>&dMaxZs,UGint nBandCount );

	//! \brief 线性拉伸算法 影像块拉伸
	//	BilinearInterpolation	CubicConvolution NearestNeighbor
	static UGbool BilinearInterpolation(UGImgBlock *pResBlock, UGArray<UGImgBlock*>& arrImgBlock
		, ImgColorSpace nColorSpace,UGArray<UGdouble>&dMinZs,
		UGArray<UGdouble>&dMaxZs,UGint nBandCount , UGbool bMaxMinStretch=true);

	//!\brief 线性拉伸的反算法
	static UGbool BilinearInterpolationR(UGArray<UGdouble>& arrValue,UGColor nColor, 
		ImgColorSpace nColorSpace,UGArray<UGdouble>&dMinZs,
		UGArray<UGdouble>&dMaxZs,UGint nBandCount );

	//##ModelId=483BB7090157
	static UGbool AppendBlock(UGImgBlock* pResBlock, UGImgBlock* pSrcBlock, UGint nOpenMPNum = 0, UGbool bHasNoValue = FALSE, UGdouble dNoValue = 0.0); 

	//add by kangweibo,解决移动端WMS叠加显示慢的问题,暂时先这么处理（只在移动端使用）
	static UGbool AppendBlockWMS(UGImgBlock* pResBlock, UGImgBlock* pSrcBlock);

	//! \brief 获得不同数值到像素256位之间的比率
	static UGdouble GetRatioPixel(UGdouble dMaxZ,UGdouble dMinZ,OGDC::PixelFormat ePixelFormat);
	//! \brief 通过nColor来获取实际的RGB值
	static UGColor GetColor(UGint nColor,ImgColorSpace nColorSpace);

	#if PACKAGE_TYPE==PACKAGE_UGC
	//色彩空间转换------------------------------------------------------------
	//! \brief
	  //!  \brief Toolkit
	static UGint RGB2CMY(UGint color);
	//! \brief
	  //!  \brief Toolkit
	static UGint CMY2RGB(UGint color);
	//! \brief
	  //!  \brief Toolkit
	static UGint RGB2YIQ(UGint color);
	//! \brief
	  //!  \brief Toolkit
	static UGint YIQ2RGB(UGint color);
	//! \brief
	  //!  \brief Toolkit
	static UGint RGB2YUV(UGint color);
	//! \brief
	  //!  \brief Toolkit
	static UGint YUV2RGB(UGint color);
	//! \brief
	  //!  \brief Toolkit
	static UGint RGB2YCC(UGint color);
	//! \brief
	  //!  \brief Toolkit
	static UGint YCC2RGB(UGint color);
	//! \brief CMYK与RGB转换 与SFC保持兼容
	static UGint CMYK2RGB(UGCMYK& cmyk);
	  //!  \brief Toolkit
	static UGCMYK RGB2CMYK(UGColor color);
	//! \brief
	static void RGBtoHSL(UGColor rgb, UGdouble& H, UGdouble& S, UGdouble& L);
	//! \brief
	static UGColor HLStoRGB(const UGdouble& H, const UGdouble& L, const UGdouble& S);
	//! \brief
	static UGdouble HuetoRGB(UGdouble m1, UGdouble m2, UGdouble h);
	#endif // end declare ugc API

	//! \brief 判断着色表是否8位灰度
	static UGbool IsGrayColorset(const UGColorset &colorset);


	//! \brief 图片处理成2的N次方，并可以避免三维中半个像素的问题。
	//! \param imageData 图片数据 [out]。
	//! \param nRealWidth 有效数据的宽度 [out]。
	//! \param nRealHeight 有效数据的高度 [out]。
	//! \return 。
	//! \remarks 。
	//! \attention 。
	static void ImageDataToPowerOfTwo(UGImageData& imageData, UGint& nRealWidth, UGint& nRealHeight, UGbyte chA = 255);


	//{{影像插件的配置文件(.xml)
	//准备写入配置文件，没有就生成一个，并写入最外层SuperMap标签内容;
	//读和写都可以用
	static UGbool PrepareImageConfigFile(const UGString strFilePath);
	//根据标签读写xml字符串
	static UGbool ElementToConfigFile(const UGString strFilePath, const UGString strElemTag, UGString strContent);
	static UGString ElementFromConfigFile(const UGString strFilePath, const UGString strElemTag );

	// 栅格信息格式化成XML
	static UGString StatisticsInfoToXML(UGDict<UGint, UGRasterStatisticsInfo> &dictRasterStatInfo, UGString strDTName);
	static void StatisticsInfoFromXML(const UGString& strXML, UGArray<UGint> arrBandIndex, UGDict<UGint, UGRasterStatisticsInfo> &dictRasterStatInfo);
		static void ParserStatInfoXML(UGMarkup& markup, UGRasterStatisticsInfo& statInfo, UGbool isToXML);
	//}}影像插件的配置文件(.xml)
};
}

#endif // !defined(AFX_UGIMGTOOLKIT_H__21224E42_7983_49CB_99BC_9792CA94305F__INCLUDED_)


