// UGImagePlugIn.h: interface for the UGImagePulgIn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UGIMAGEPLUGIN_H__8C978A6D_1011_4B0F_A221_C9BEDA07CCD8__INCLUDED_)
#define AFX_UGIMAGEPLUGIN_H__8C978A6D_1011_4B0F_A221_C9BEDA07CCD8__INCLUDED_
//////////////////////////////////////////////////////////////////////////
//!  \file UGImagePlugin.h
//!  \brief UGImagePlugin 的接口定义。
//!  \details 关于影像插件的接口定义。
//!  \Modified Date: 2013-4-7
//!  \Modified Author:Conversion-zby
//!  \Details Comments:调整UGImagePlugin接口参数以及接口组成，主要目的是在UGImagePlugin接口中添加多波段栅格数据集的扩展支持;
//!  \Details Comments:本次调整主要包含两方面内容:(1) 对于原有接口增加波段索引参数支持;(2) 增加多波段栅格数据集特有接口
//!  \Details Comments:此次调整涉及整个栅格数据集结构的调整，由于技术人员水平所限以及时间的仓促，难免会产生这样那样的问题,有问题
//!  \Details Comments:请与Conversion-zby联系!
//!  \Version OGDC-1-1(2013-4-7)
#define MAXIMGAEPALETTEENTRY 256
#ifdef WIN32
	#include <Windows.h>
	#define IMAGEEXPORT __declspec(dllexport)
#else
	#define IMAGEEXPORT
#endif

#include "Stream/ugplatform.h"
#include "Stream/ugdefs.h"
#include "Toolkit/UGPalette.h"
#include "NetToolkit/UGDownloadListener.h"
#include "Geometry/UGDataCodec.h"
#include "FileParser/UGExchangeFileType.h"
#include "FileParser/UGRasterConfigParams.h"
#include "FileParser/UGFileParseRaster.h"
#include "CacheFile/UGCacheFileCommon.h"
#include "CacheFile/UGMapCacheFile.h"


//! \brief 象素格式类型.
enum ImagePixelFormat 
{
	PF_ERROR = 0,		// 0位,不支持的象素格式
	PF_MONO = 1,		// 1位,单色
	PF_FBIT = 4,		// 4位,16色
	PF_BYTE = 8,		// 8位,256色
	PF_TBYTE= 16,		// 16位,彩色
	PF_RGB = 24,		// 24位,真彩色
	PF_RGBA = 32,		// 32位,增强真彩色
	PF_LONGLONG = 64,	// 64位,长整型
	PF_LONG = 320,
	PF_FLOAT = 3200,	// 32位,浮点型
	PF_DOUBLE = 6400,	// 64位,双精度浮点型
};

//! \brief 定义不同象素格式的位数.
enum ImagePixelBits
{
	PB_MONO = 1,
	PB_FBIT = 4,
	PB_BYTE = 8,
	PB_TBYTE = 16,
	PB_RGB = 24,
	PB_RGBA = 32,
	PB_FLOAT = 3200,
};

//! \brief 色彩空间格式类型.
enum ImageColorSpace
{
	CS_RGB = 0,		// 显示系统使用.
	CS_RGBA = 1,	// 显示系统使用.
	CS_CMY = 2,		// 印刷系统使用(Cyan,Magenta,Yellow).
	CS_CMYK = 3,	// 印刷系统使用(Cyan,Magenta,Yellow,Black)
	CS_YIQ = 4,		// 北美电视系统使用(NTSC,Y:Luminance,Chrominance).
	CS_YUV = 5,		// 欧洲电视系统使用(PAL).
	CS_YCC = 6		// 与YUV类似,JPEG使用.
} ;

//! \brief 坐标与距离单位定义 - 标准方式
enum  ImageUnit
{
	EU_DEGREE		= 0,		// Radian弧度
	EU_MILIMETER	= 10,		// mm毫米
	EU_CENTIMETER	= 100,		// 厘米
	EU_DECIMETER	= 1000,		// 分米
	EU_METER		= 10000,	// 米
	EU_KILOMETER	= 10000000,	// 公里
	EU_MILE			= 16090000,	// 英里
	EU_INCH			= 254,		// 英寸
	EU_YARD			= 9144,		// Yard码
	EU_FOOT			= 3048,		// Feet英尺
};

//! \brief 影像数据调色板类型
struct IMAGEEXPORT UGImagePaletteEntry
{
	unsigned char peRed;
	unsigned char peGreen;
	unsigned char peBlue;
	unsigned char peAlpha;
};

//! \breif 影像插件类型
enum PluginClass {
	PC_UNKNOWN,
	PC_MAPCACHE,
	PC_MAPCACHE50,
	PC_MAPCACHEV70,
	PC_SIT,
	PC_GLOBALCACHE,
	PC_GRIDCACHE,
	PC_COMMON,
	PC_GECACHE,
	PC_WEBCACHE,
	PC_ECW,
	PC_SPI,
};

using namespace UGC;
namespace UGC
{	
	typedef void (UGSTDCALL * DownloadResponseFunc) (UGDownloadTileInfo result, UGlong pWnd);
	typedef void (UGSTDCALL * DataChangedProc) (const UGDataChangeInfo& ChangeInfo, UGlong pWnd);

	typedef void (UGSTDCALL * CheckStatusProc) (UGlong pWnd);// 连续十次下载失败，请求检查网络状态

	typedef void (UGSTDCALL * MapCacheProgressProc) (UGlong pWnd, UGint nStep);// 进度信息

	typedef void (UGSTDCALL * MapCachePerGroupCompleteProc) (UGlong pWnd);// 瓦片组完成信息

	typedef void (UGSTDCALL * MapCacheStatusProc) (UGlong pWnd, UGint nCompleteCount, UGlong nTotalCount);// 缓存完成情况，
}


class IMAGEEXPORT UGImagePlugin : public UGC::UGDownloadListener
{
public:

	//! \brief 图层服务类型。
	enum UGImageServiceType
	{	
		//! \brief 空图层
		IST_NONE				= 0,	
		//! \brief SuperMap 影像文件缓存数据图层
		IST_FILE				= 1, 
		//! \brief WMS图层
		IST_WMS					= 2,
		//! \brief WMTS图层
		IST_WMTS				= 3,
		//! \brief TMS图层
		IST_TMS                 = 4,
	};
    
	//! \brief 插件类构造函数
	UGImagePlugin()
	{
		m_nCurBandIndex = -1;
		m_dNoValue = -9999; 
		m_nPluginClass = PC_UNKNOWN;
		m_bBackTransparent = FALSE; 
		m_nImageServiceType = IST_NONE;
		m_bIsOpenAsOneBand  = FALSE;
		m_bSaveLocalCache = TRUE;
	}

	//! \brief 插件类析构函数
	virtual ~UGImagePlugin(){}

public:

	//! \brief 关闭文件。
	virtual void Close() = 0;
	//! \brief  获取图片最大值
	//! \param 
	//! \attention 
	virtual UGdouble GetMax(){return 0.0;}
	//! \brief 获取图片最小值
	//! \param  
	//! \attention 
	virtual UGdouble GetMin(){return 0.0;}
	//! \brief 输出影像文件的象素格式。
	//! \param  nPixelformat 象素格式，如RGB,灰度等[out]
	virtual UGbool GetPixelFormat(ImagePixelFormat& nPixelformat) = 0;

	//! \brief 输出象素位数
	//! \param  nPixelbits,象素格式位数，如RGB一般是24位[out]
	virtual UGbool GetPixelBits(ImagePixelBits& nPixelbits)= 0;

	//! \brief 得到影像的大小
	//! \param  nWidth 影像宽度[out]
	//! \param  nHeight 影像高度[out]
	virtual UGbool GetImageSize(UGlong&  nWidth, UGlong&  nHeight)= 0;

	//!  \brief 得到当前比例尺影像大小
	//!  \param nWidth 影像宽度
	//!  \param nHeight 影像高度
	//!  \param dScale 缓存当前比例尺
	//!  \remark 此函数主要应用于获取本地剖分缓存对应比例尺的影像宽高
	virtual UGbool GetImageSize(UGint& nWidth,UGint& nHeight,UGdouble dScale){return FALSE;}

	//! \brief 得到影像图片格式
	virtual UGFileType::EmType GetImageType(){return UGFileType::none;}

	//!  \brief  是否是全球缓存
	//! \return true
	virtual UGbool IsGlobalCache(){return TRUE;}

	//! \brief 判断插件是否打开
	virtual UGbool IsOpen(){return FALSE;}

	//! \brief 判断插件是否打开网络数据
	virtual UGbool IsNetCache(){return FALSE;}

	//! \return 插件类型
	virtual PluginClass GetPluginClass() { return m_nPluginClass; }

	//! \brief  输出插件的名称，比如 ECW 文件插件.
	//! \param  strName,字符串,函数外分配空间.[out]
	//! \param  nMaxLength 字符串长度[in]
	virtual UGbool GetPluginName(UGchar* strName, UGint nMaxLength) = 0;

	//! \brief  
	//! \param 
	//! \attention
	virtual void GetExtremum(UGdouble &dMaxZ, UGdouble &dMinZ, UGint nBandIndex = 0){return;}

	//! \brief  得到本插件支持的影像文件的扩展名，如".BMP",".JPG"等。
	//! \param  nIndex 第几个扩展名。可以通过GetExtNameCount取得支持的扩展名数目。[in]
	//! \param  strExtName 扩展名字符串，函数外分配空间。[out]
	//! \param  nMaxLength 字符串长度。[in]
	virtual UGbool GetExtName(UGchar* strExtName, UGint nMaxLength, UGint nExtNameIndex = 0) = 0;



	//! \brief 设置当前显示范围后,返回显示范围内影像数据的宽度。
	//! \remarks 对于多分辨率图象，可能从另外一层读取数据，所以输出的宽度可能和显示范围的宽度不一样
	//! \param  nViewWidth 显示数据影像宽度。[out]
	//! \param  nViewHeight 显示数据影像高度。[out]
	virtual UGbool GetViewImageSize(UGint&  nViewWidth, UGint&  nViewHeight)= 0;

	//! \brief 设置当前显示范围后,返回显示范围内数据的大小。
	virtual UGint GetViewImageDataSize() = 0; 

	//! \brief 设置当前显示范围后,读取显示范围内的影像数据,外部取得数据以后，可以用来显示
	//! \param  pDataBuffer 影像数据，函数外分配空间。[in]
	//! \param  nSize 数据大小(字节数），一般就是GetViewImageDataSize（）返回值[in]
	virtual UGbool GetViewImageData(UGbyte* pDataBuffer, UGint nSize)= 0;

	//! \brief 设置地图的固定比例尺。某些数据源，例如地图缓存插件要求设置地图的固定比例尺

	//! \brief 打开文件。
	//! \param  strFileName,文件路径名。[in]
	//! \param  nFlags,打开方式，如只读，共享等，参看CFile的标志。[in]
	virtual UGbool Open(const UGchar* strFileName, UGuint nFlags, const UGchar * strPassword) = 0;

	//! \brief 设置当前显示范围，数据随后通过 GetViewImageData() 获得
	//! \param  nLeft, nTop,显示范围的左边和上边。nLeft < nRight，nTop < nBottom.[in]
	//! \param  nRight, nBottom 显示范围的右边和下边。这是个值给的是象素坐标[in]
	//! \param  nDeviceWidth, nDeviceHeight 真实显示窗口的大小，屏幕象素单位。[in]
	//! \param  bLoadEveryPicEntirely load的每张图片是完整的，全部加载后整体偏移，缺省是false[in]
	//! \remarks 对于多分辨率的图象，可以按照窗口大小和显示范围大小，确定图象的分辨率,从而决定从那一层取得影像数据
	virtual UGbool SetViewPort(UGint nLeft,UGint nTop, UGint nRight, UGint nBottom, UGint nDeviceWidth, UGint nDeviceHeight) = 0;

	//! \brief 设置当前显示范围，数据随后通过 GetViewImageData() 获得
	//! \param  dLeft, dTop,显示范围的左边和上边。dLeft < dRight，dTop < dBottom.[in]
	//! \param  dRight, dBottom 显示范围的右边和下边。这是个值给的是地理坐标[in]
	//! \param  nDeviceWidth, nDeviceHeight,真实显示窗口的大小，屏幕象素单位。[in]
	//! \remarks 对于多分辨率的图象，可以按照窗口大小和显示范围大小，确定图象的分辨率,从而决定从那一层取得影像数据
	virtual UGbool SetViewPort(UGdouble dLeft,UGdouble dTop, UGdouble dRight, UGdouble dBottom, UGint nDeviceWidth, UGint nDeviceHeight,UGbool bLoadEveryPicEntirely = false) = 0;

	//! \brief 得到影像的地理范围以及坐标单位,参看enumUnit
	//! \param  dLeft, dRight 地理范围的左边和右边，一般dRight > dLeft.[out]
	//! \param  dButtom, dTop 地理范围的下边和上边，一般dTop > dBottom.[out]
	//! \param  nUnits 地理单位，如公里，米或经纬度。[out]
	virtual UGbool GetImageGeoRefrence(UGdouble& dLeft, UGdouble& dTop, UGdouble& dRight, UGdouble& dBottom, ImageUnit& nUnits)= 0;  

	//! \brief pMap 所需要设置固定比例尺的地图 [in]
	//! \brief pCallBack 用来设置固定比例尺的回调函数。该回调函数第一个参数为 UGMap 指针，第二个参数为所需要设置的固定比例尺 [in]
	virtual void ConfigureMap(void *pMap, void (*pCallBack) (void *, std::vector<UGdouble>)) { /* do nothing */ }

	//! \brief 设置当前影像插件数据集所在金子塔级别
	//! \param  viewWidth 显示窗口的像素范围
	//! \param  nDeviceWidth 窗口设备宽度
	//! \remarks 对于多分辨率的图象(代有金字塔的数据),可以按照窗口宽度和显示范围，确定图象的分辨率,从而决定从那一层(金字塔)取得影像数据
	virtual UGbool setPymidLevel(UGint nTierIndex/*UGint viewWidth,UGint nDeviceWidth*/){return TRUE;};

	// ******** 因为以上的结构不支持多线程 故增加几个接口 [ by jiangrs 2012/10/12 ]*********  

	//! \brief 通过设置当前显示范围，获取显示范围内需要的影像数据的宽高
	//! \param  dLeft, dTop,显示范围的左边和上边。dLeft < dRight，dTop < dBottom.[in]
	//! \param  dRight, dBottom 显示范围的右边和下边。这是个值给的是地理坐标[in]
	//! \param  nDeviceWidth, nDeviceHeight,真实显示窗口的大小，屏幕象素单位。[in]
	//! \param  nViewWidth 显示数据影像宽度。 [out]
	//! \param  nViewHeight 显示数据影像高度。[out]
	//! \param  nTierIndex 显示数据影像金字塔索引号。 [out]
	//! \remarks 即SetViewPort() + GetViewImageSize()的合体
	virtual UGbool GetViewImageSize(UGint dLeft, UGint dTop, UGint dRight, UGint dBottom, UGint nDeviceWidth, UGint nDeviceHeight, UGint& nViewWidth, UGint& nViewHeight, UGint& nTierIndex){return FALSE;}

	//! \brief 通过获取的影像数据的宽高和显示范围  构造ImgBlock后 然后在用此ImgBlock去获取数据
	//! \param  pResBlock 影像数据，函数外分配空间。[out]
	//! \remarks 即GetViewImageData(unsigned char* pDataBuffer, int nSize)的改版
	virtual UGbool GetViewImageData(UGImgBlock* pResBlock, UGint nTierIndex = -1){return FALSE;}

	//  ******** 因为以上的结构不支持多线程 故增加几个接口 [ by jiangrs 2012/10/12 ]*********  
	//! \brief 得到扫描线
	virtual UGbool GetScanline(UGint nRowIndex,UGImgScanline& ScanLine, UGint nStartCol = 0, UGint nWidth = 0,UGint nBandIndex=0){return FALSE;}

	//===================================================
	//! \brief  获取图片最大值
	//! \attention 
	virtual UGdouble GetMaxZ(UGint nBandIndex = 0){return 0.0;}

	//===================================================
	//! \brief 获取图片最小值
	//! \attention 
	virtual UGdouble GetMinZ(UGint nBandIndex = 0){return 0.0;}

	//! \brief  计算统计直方图
	//! \param 
	//! \return   
	//! \remarks 
	//! \attention
	virtual UGHistogram* GetHistogram( 
		UGdouble dMin, 
		UGdouble dMax,
		UGint nBandIndex = 0,
		UGint nBuckets = 256){return FALSE;}

	//===================================================
	//! \brief  获取图片的宽
	//! \param 
	//! \return   
	//! \remarks 
	//! \attention
	virtual UGint GetWidth() const{return 0;}

	//===================================================
	//! \brief  获取图片高
	//! \param 
	//! \return   
	//! \remarks 
	//! \attention
	virtual UGint GetHeight() const{return 0;}
    
    //===================================================
	//! \brief  统计栅格信息
	//! \param 
	//! \return   
	//! \remarks 
	//! \attention
 	virtual UGbool BuildStatistic(UGint nBandIndex, UGRasterStatisticsInfo& inof){return FALSE;}
    	//===================================================
	//! \brief  得到栅格信息
	//! \param 
	//! \return   
	//! \remarks 
	//! \attention
 	virtual UGbool GetStatisticsInfo(UGint nBandIndex, UGRasterStatisticsInfo &info){return FALSE;}

	//===================================================
	//! \brief  获取波段信息列表。
	//! \param  bandInfos[out]  要输出的波段信息列表。
	//! \return   
	//! \remarks 
	//! \attention 导入多波段时用。
	virtual UGbool GetBandInfos(UGArray<BandInfo>& bandInfos){return FALSE;}	

	//! \brief获取请求块返回的准确bounds
	virtual  UGRect2D GetViewImageBounds(){return m_rcViewImgBounds;} 

	//===================================================
	//! \brief   分波段计算极值
	//! \param   dMax[O]       最大值
	//! \param   dMin[O]       最小值
	//! \param   nBandIndex[I] 波段索引
	//! \return   
	//! \remarks 
	//! \attention
	virtual UGbool CalculateExtremum(UGdouble &dMax, UGdouble &dMin, UGint nBandIndex = 0){return FALSE;}

	//! \brief   获取FileParse的类型
	virtual UGFileType::EmType GetFileParseType(){return UGFileType::none;}

	//! \brief   获取数据集的类型
	virtual UGint GetDatasetType(){return 88/*UGDataset::MBImage*/;}

	//! \brief 保存数据
	virtual UGbool SaveImageFile(UGdouble dScale, UGint nRow, UGint nCol, UGByteArray* pByteArray) {return FALSE;}

	//得到数据的高度范围
	virtual UGdouble GetTop(){return 0.0;}
	virtual UGdouble GetBottom(){return 0.0;}

	//得到数据的层范围
	virtual UGint GetBeginLevel(){return 0;}

	virtual UGint GetEndLevel(){return 0;}
public:
	/*
		网络地图服务插件加载方式
	*/
	//! \brief 设置Web服务的用户名 密码使用Open()里头的接口吧！！！
	virtual void SetUser(const UGString strUser){return ;}

	//! \brief 设置取到数据并把数据区域的外接矩形返回给用户的回调函数
	//! \param pFunc 回调函数[in]。
	//! \param pWnd 句柄[in]。
	//! \remarks 二维使用的啊！！！
	virtual void SetDataChangedFunc(DataChangedProc pFunc,UGlong pWnd){return; }

	//! \brief 设置回调函数
	//! \remarks 三维使用的啊！！！
	virtual void SetDownloadResponseFunc(DownloadResponseFunc pFunc,UGlong pWnd){return; }

	//! \brief 设置请求参数
	//! \dScale param[in] 当前地图的比例尺
	//! \bound param[in] 当前地图范围
	virtual UGbool Request(const UGdouble& dScale, const UGRect2D& bound, UGbool bCallBack = TRUE){return FALSE;}

	//! \brief 设置请求参数
	//! \dScale param[in] 当前地图的比例尺
	//! \bound param[in] 当前地图范围
	virtual UGbool GetRequests(UGArray<void*> &arrRequest, const UGdouble& dScale, const UGRect2D& bound, UGString strToken = _U("")){return FALSE;}

	//! \brief 通过比例尺转换瓦片边长对应的地理大小
	//! \param dMapScale 地图比例尺
	virtual UGSize2D GetTileMPSize(UGdouble dMapScale){return UGSize2D();}

	//! \brief 获取瓦片的DP大小
	virtual UGSize GetTileSize(){return UGSize();}

	//! \brief 获取瓦片的DP大小
	virtual UGuint GetTileDepth(){return 1;}

	//! \brief 返回所有的比例尺级别
	virtual const UGArray<UGdouble> GetScales() const {return UGArray<UGdouble>();}	
	
	//! \brief 判断一次请求是否已经完成，Request之后调用
	//! \remarks 和IsFinished()不一样  IsFinished()是判断GetViewBlock()加载的是否完全
	//! \return 完成返回True，否则返回False
	virtual UGbool IsCompleteRequest() {return TRUE;}

	//! \brief 根据范围和比例尺，查询记录集。
	//! \param rcMapBounds [in] 范围。
	//! \param dScale [in] 比例尺。
	//! \return 返回范围内所有瓦片的信息 
	virtual UGTiles* QueryWithBounds(const UGRect2D& rcMapBounds, UGdouble dScale) const {return NULL;}

	//! \brief 加载缓存影像块数据
	//! \tile param 单个瓦片信息[in]
	//! \return 瓦片的ImageData
	virtual UGImageData* LoadTileImageData(const UGTile& tile){return NULL;}

	//! \brief 获取最接近的比例尺
	//! \param dScale 传入比例尺
	//! \return 返回最接近的固定比例尺
	virtual UGdouble ScaleToNearFixScale(UGdouble dScale){return 0.0;}

	//! \brief 计算一个范围扩张几个瓦片之后的范围
	//! \param dMapScale 地图比例尺
	//! \param rcMapBounds 地图范围
	//! \param nExtTileSize 扩张瓦片的个数
	//! \return 扩张之后的地图范围
	virtual UGRect2D ComputeTilesBounds(UGdouble dMapScale, const UGRect2D& rcMapBounds, UGint nExtTileSize = 1){return UGRect2D();}

	//! \brief 获取当前服务瓦片DP-->LP的转换比率
	virtual UGdouble GetCoordRatio() {return 3.0;}

	//! \brief 根据范围和比例尺，判断该范围内的瓦片是否全都存在。
	//! \param rcMapBounds [in] 范围。
	//! \param dScale [in] 比例尺。
	//! \return 
	virtual UGbool IsAllTilesExist(const UGRect2D& rcMapBounds, UGdouble dScale) const {return FALSE;}

	//! \brief 获取本地缓存的版本号(嵌入式使用)
	virtual UGint GetCacheVersion() const {return 0;}

	//! \brief 获取当前服务SCI中DP-->LP的转换比率(嵌入式使用，主要是使用GetCoordRatio有误差)
	virtual UGdouble GetCacheCoordRatio(){return 0.0;}

	//! \brief 获取像素值
	//!\param nBandIndex 存储的波段索引,默认的波段索引启始值为0
	virtual UGColor GetPixel(UGint X,UGint Y,UGint nBandIndex=0){return UGColor(0);};

	//! \brief 获取数值
	//!\param nBandIndex 存储的波段索引,默认的波段索引启始值为0
	virtual UGdouble GetValue(UGint X,UGint Y, UGbool bIsNeedLock = TRUE,UGint nBandIndex=0){return UGdouble(0.0);};

	//! \brief 保护继承获取块儿内存数据函数
	//! \param nBlockRow 块儿行编号
	//! \Param nBblockCol 块儿列编号
	//! \param pImgBlock 指向块儿内存地址
	//! \param  nlevel[int] 层级 
	//! \Param nBandIndex 块儿所在波段索引位置
	virtual UGbool GetBlockAt(UGint nBlockRow,UGint nBlockCol,
		UGImgBlock*& pImgBlock,UGint m_nLevel,UGint nBandIndex=0){return FALSE;};

	// 获取IndexBounds, added by XingJun, 2016.03.16
	virtual UGRect2D GetIndexBounds() const {return UGRect2D();};
public:

	//! \brief 释放自己。加这个方法主要是一个动态库内分配的堆内存，
	//! \remarks 在另外一个动态库中释放可能会出错，所以给各方法释放自己。
	virtual UGbool BuildPyramid(UGbool bShhowProgress){return FALSE;}
    
	//! \brief 删除金字塔
	//! \param 
	//! \return 删除成功返回TRUE，否则返回FALSE
	virtual UGbool RemovePyramids(){return FALSE;}
    
	//! \brief 是否存在金子塔
	//! \param  
	//! \return 存在金子塔返回TRUE，不存在金字塔返回FALSE
	virtual UGbool IsExistPyramid() const {return FALSE;}
    
	//! \brief 为栅格文件修改密码
	//! \param  bool 修改密码操作是否成功[out]
	//! \param  strNewPassword 设置的新密码[in]
	virtual UGbool ChangePassword(const UGchar* strNewPassword){return FALSE;}

	//! \brief 为获取栅格文件的缓存访问密码
	//! \return 返回已设置的密码，若没有设置则返回空
	virtual const UGchar* GetPassword() {return NULL;}

	//! \brief 得到支持的影像文件扩展名数目。
	virtual UGint GetExtNameCount() const{return 1;}
	
	//! \brief  得到波段数目。
	virtual UGint GetBandCount() const {return 1;}

	//! \brief  设置当前取数据的波段索引。
	//! \param  nBandIndex 波段索引。[in]
	virtual void SetCurBandIndex(UGint nCurBandIndex){m_nCurBandIndex = nCurBandIndex;}

	//! \brief  得到当前取数据的波段索引。
	virtual UGint GetCurBandIndex() const{return m_nCurBandIndex;}

	//! \brief  设置波段组合模式。
	//! \param  pBandIndexs 波段索引数组。[in]
	//! \param  nColorSpace 颜色空间模式。[in]
	virtual void SetBandCombineMode(const int* pBandIndexs, ImageColorSpace nColorSpace){}

	//! \brief  得到波段组合模式。
	//! \param  pBandIndexs 波段索引数组，外面开辟内存。最大为4个int。根据颜色空间模式而定。[out]
	//! \param  nColorSpace 颜色空间模式。[out]
	virtual void GetBandCombineMode(int* pBandIndexs, ImageColorSpace& nColorSpace) const{}

	//! \brief 设置投影信息
	virtual void SetPrjCoordSys(const UGchar*strPrjCoordSys){}

	//! \brief XML字符串
	virtual const UGchar* GetPrjCoordSys(){return NULL;}

	//! \brief 设置文件版本信息
	virtual void SetVersion(UGint nVersion){}
	virtual UGint GetVersion(){return 0;}
	
	//! \brief 得到指定范围数据的版本信息
	virtual UGint GetDataVersion(UGdouble dLeft,UGdouble dTop, UGdouble dRight, UGdouble dBottom, UGint nDeviceWidth, UGint nDeviceHeight){return 0;}
	
	//! \brief 得到裁剪区域,不需要外面分配内存，但是需要外面释放。
	virtual UGbool GetClipRgn(UGdouble*& pXCoord, UGdouble*& pYCoord, UGint*& pPloyCounts, UGint& nSubCount){return FALSE;}

	//! \brief 释放临时数据。
	virtual void Release(){}

	//! \brief 释放缓存中的数据
	virtual void FreeCache() {}

	//! \brief 数据下载进度
	virtual float GetDataStreamingProgress(){return -1.0;}

	//! brief 得到显示的金字塔的所需要的内存块(字节)
	//! param nBand 波段
	//! param nTierIndex 金字塔索引
	virtual UGint GetViewPyramidDataSize(UGint nBand, UGint nTierIndex){return 0;}

	//! \brief 设置显示的地理范围
	//! \param 显示的范围
	//! \remarks 这个函数是针对UGPluginCommon类设置的，在分块读取RDAS的IMG格式文件时会用到
	//! 在UGDatasetRasterImg::GetViewBlock函数中调用
	virtual void SetViewBounds(UGdouble dLeft, UGdouble dTop, UGdouble dRight, UGdouble dBottom){};

	//! \brief GetViewImageData() 获取数据是否已经获取完毕，
	//! \brief如果没有，需要继续调用 GetViewImageData() 获取数据。当前只有 UGPluginMapCache 有可能一次性无法获取全部数据。
	virtual UGbool IsFinished() { return true; }

	//!	\brief	获取插件的全路径文件名
	virtual const UGchar* GetPluginFileName() { return NULL; }

	//! \brief 设置是否背景透明
	virtual void SetBackTransparent(UGbool bBackTransparent){ m_bBackTransparent = bBackTransparent; }

	// 获取是否背景透明
	virtual UGbool IsTransparentBack() { return m_bBackTransparent; }

	//! \brief图层类型
	virtual UGImageServiceType GetImageServiceType() { return m_nImageServiceType; }

	//! \brief 以下用中UGPluginGlobalCache中,支持下载WMS数据
	//! \brief 获取网络数据集所有图层
	virtual UGbool GetWebSubLayersName(UGStrings& aryLayerNames) const{ return false; }

	//! \brief 获取网络数据集所有图层标题
	virtual UGbool GetWebSubLayersTitle(UGStrings& aryLayerTitle) const{ return false; }

	//! \brief 设置网络数据集所有图层是否可见属性
	virtual UGbool SetWebSubLayersVisible(UGStrings& arrLayersVisible){ return false; }

	//! \brief 得到网络数据集所有图层是否可见属性
	virtual UGbool GetWebSubLayersVisible(UGStrings& arrLayersVisible) const{ return false; }

	//! \brief 设置缓存目录
	//! \param  [in]。
	//! \remarks 需要缓存的有WMS图层
	virtual void SetCachePath(const UGString& strCachePath) { }

	//! \brief 读取金字塔的个数
	//! \得到第几波段的金字塔的个数
	virtual UGint GetPyramidCount(UGint nBand){return 0;}

	//===================================================
	//! \brief  获取金字塔数据集信息列表。
	//! \param  rasterInfos[out]要输出的金字塔信息集合列表
	//! \return  获取金字塔信息成功返回TRUE,否则返回FALSE 
	//! \attention 导入多波段时用。
	virtual UGbool GetPyramidInfos(UGArray<PyramidInfo>& pyramidInfos){return FALSE;}

	//! \brief 获取纹理压缩格式
	//! \return 纹理压缩格式。
	//! \attention 。
	virtual UGDataCodec::CodecType GetTextureCodecType() {return UGDataCodec::encNONE;}

	//! \brief 设置DXT格式纹理是否需要解压
	//! \param  bIsDXTEncode[in]是否解压
	//! \attention 地图DXTZ缓存加载到二三维场景时使用
	virtual void SetIsTextureEncode(UGbool bIsDXTEncode) {;}

	virtual UGbool GetOpenAsOneBand(){return m_bIsOpenAsOneBand;}

	virtual void SetOpenAsOneBand(UGbool bIsOpenAsOneBand = FALSE){m_bIsOpenAsOneBand = bIsOpenAsOneBand;}
	
	virtual void RemoveCallbackHandle(long handle){};WhiteBox_Ignore
	virtual UGC::UGHPDataArea GetTileDataArea(){return UGHPDataArea();};
	//! \得到最左上位置瓦片的MPBounds
	virtual UGRect2D ComputeTileMPBounds(double dMapScale, int nRow, int nCol){return UGRect2D();};
	virtual UGdouble GetCurrentScale(){return 0;};
	virtual UGImagePlugin* ClonePlugin(){return NULL;};
	//清除下载队列
	virtual void RemoveRequest(){};

	//地形最大最小值
	virtual UGdouble GetMaxValue(){return 0.0;};
	virtual UGdouble GetMinValue(){return 0.0;};

	virtual UGMapCacheFile* GetMapCacheFile() {return NULL;};

	//! \brief 得到服务请求基类
	virtual void* GetServiceStub(){return NULL;}

	//! \brief 设置网络数据是否保存到本地
	virtual void SetSaveLocalCache(UGbool bSave){m_bSaveLocalCache = bSave;}

	//! \brief 获取网络数据是否保存到本地
	virtual UGbool GetSaveLocalCache(){return m_bSaveLocalCache;}

	//! \brief 设置是否使用内存驻留
	virtual void SetMemCacheEnable(UGbool bEnable){return;}
public:
	//! \brief 设置告诉用户瓦片连续下载失败的回调函数
	//! \param pFunc 回调函数[in]。
	//! \param pWnd 句柄[in]。
	virtual void SetCheckStatusFunc(CheckStatusProc pFunc,UGlong pWnd){}

	//! \brief 设置告诉用户预缓存某一任务组完成的回调函数
	//! \param pFunc 回调函数[in]。
	//! \param pWnd 句柄[in]。
	virtual void SetMapCachePerGroupCompleteFunc(MapCachePerGroupCompleteProc pFunc,UGlong pWnd){}

	virtual void SetTotalTiles(UGint nTotalCount){}

	virtual UGint GetFailedCount(){ return 0;}

	virtual void StopMapDownloadQueue(){}
public:

	//! \brief  调色板数据。
	UGArray<UGImagePaletteEntry> m_arrPalette;  

	//! \brief  无值数据
	UGdouble m_dNoValue;

protected:
	//! \brief 对应的文件解析类类型。
	UGint m_nFileType;

	//! \brief 当前波段索引。
	UGint m_nCurBandIndex;

	//! \brief 合成波段索引数组
	UGint m_bandCombine[4];
    
	//! \brief 颜色空间
	ImageColorSpace m_nColorSpace;

	//! \brief 是否背景透明
	UGbool m_bBackTransparent; 
    
	//! \brief 当前影像插件类型
	PluginClass m_nPluginClass;	
    
	//! \brief 图层服务类型
	UGImageServiceType m_nImageServiceType;
    
	//! \brief 影像的地理bounds
	UGRect2D m_rcViewImgBounds; 

	//! \breif 是否已合成波段的方式打开
	//! 本接口目前只用于数据集集合
	UGbool m_bIsOpenAsOneBand;

	//! \breif 网络数据是否下载保存到本地文件
	UGbool m_bSaveLocalCache;
};



// 插件需要实现一个导出的全局函数用来返回 UGImagePlugin 派生的类。
// 创建一个插件类，请new 一个从 UGImagePlugin 派生的类。

extern "C" IMAGEEXPORT UGImagePlugin* CreateImagePlugin();

#endif // !defined(AFX_UGIMAGEPLUGIN_H__8C978A6D_1011_4B0F_A221_C9BEDA07CCD8__INCLUDED_)

