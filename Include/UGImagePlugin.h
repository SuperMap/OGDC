// UGImagePlugIn.h: interface for the UGImagePulgIn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UGIMAGEPLUGIN_H__8C978A6D_1011_4B0F_A221_C9BEDA07CCD8__INCLUDED_)
#define AFX_UGIMAGEPLUGIN_H__8C978A6D_1011_4B0F_A221_C9BEDA07CCD8__INCLUDED_

#define MAXIMGAEPALETTEENTRY 256
#ifdef WIN32
	#include <Windows.h>
	#define IMAGEEXPORT __declspec(dllexport)
#else
	#define IMAGEEXPORT
#endif

#include "Stream/ugdefs.h"
#include "NetToolkit/UGDownloadListener.h"

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

struct IMAGEEXPORT UGImagePaletteEntry
{
	unsigned char peRed;
	unsigned char peGreen;
	unsigned char peBlue;
	unsigned char peAlpha;
};

enum PluginClass {
	PC_UNKNOWN,
	PC_MAPCACHE,
	PC_HPMAPCACHE,
	PC_SIT,
	PC_GLOBALCACHE,
	PC_COMMON,
	PC_GECACHE,
};

using namespace UGC;

class IMAGEEXPORT UGImagePlugin : public UGC::UGDownloadListener
{
public:
	UGImagePlugin(){m_nCurBandIndex = -1; m_dNoValue = -9999; m_nPluginClass = PC_UNKNOWN; m_bBackTransparent = FALSE;}
	virtual ~UGImagePlugin(){}

public:
	//! \brief 输出插件类型
	virtual PluginClass GetPluginClass() { return m_nPluginClass; }

	//! \brief  输出插件的名称，比如 ECW 文件插件.
	//! \param  strName,字符串,函数外分配空间.[out]
	//! \param  nMaxLength 字符串长度[in]
	virtual bool GetPluginName(UGchar* strName, int nMaxLength) = 0;

	//! \brief  得到本插件支持的影像文件的扩展名，如".BMP",".JPG"等。
	//! \param  nIndex 第几个扩展名。可以通过GetExtNameCount取得支持的扩展名数目。[in]
	//! \param  strExtName 扩展名字符串，函数外分配空间。[out]
	//! \param  nMaxLength 字符串长度。[in]
	virtual bool GetExtName(UGchar* strExtName, int nMaxLength, int nExtNameIndex = 0) = 0;

	//! \brief 打开文件。
	//! \param  strFileName,文件路径名。[in]
	//! \param  nFlags,打开方式，如只读，共享等，参看CFile的标志。[in]
	virtual bool Open(const UGchar* strFileName, unsigned int nFlags, const UGchar * strPassword) = 0;

	//! \brief 关闭文件。
	virtual void Close() = 0;

	//! \brief 输出影像文件的象素格式。
	//! \param  nPixelformat 象素格式，如RGB,灰度等[out]
	virtual bool GetPixelFormat(ImagePixelFormat& nPixelformat) = 0;

	//! \brief 输出象素位数
	//! \param  nPixelbits,象素格式位数，如RGB一般是24位[out]
	virtual bool GetPixelBits(ImagePixelBits& nPixelbits)= 0;

	//! \brief 得到影像的大小
	//! \param  nWidth 影像宽度[out]
	//! \param  nHeight 影像高度[out]
	virtual bool GetImageSize(int& nWidth, int& nHeight)= 0;

	//! \brief 得到影像的地理范围以及坐标单位,参看enumUnit
	//! \param  dLeft, dRight 地理范围的左边和右边，一般dRight > dLeft.[out]
	//! \param  dButtom, dTop 地理范围的下边和上边，一般dTop > dBottom.[out]
	//! \param  nUnits 地理单位，如公里，米或经纬度。[out]
	virtual bool GetImageGeoRefrence(double& dLeft, double& dTop, double& dRight, double& dBottom, ImageUnit& nUnits)= 0;  

	//! \brief 设置当前显示范围，数据随后通过 GetViewImageData() 获得
	//! \param  nLeft, nTop,显示范围的左边和上边。nLeft < nRight，nTop < nBottom.[in]
	//! \param  nRight, nBottom 显示范围的右边和下边。这是个值给的是象素坐标[in]
	//! \param  nDeviceWidth, nDeviceHeight 真实显示窗口的大小，屏幕象素单位。[in]
	//! \remarks 对于多分辨率的图象，可以按照窗口大小和显示范围大小，确定图象的分辨率,从而决定从那一层取得影像数据
	virtual bool SetViewPort(int nLeft,int nTop, int nRight, int nBottom, int nDeviceWidth, int nDeviceHeight) = 0;

	//! \brief 设置当前显示范围，数据随后通过 GetViewImageData() 获得
	//! \param  dLeft, dTop,显示范围的左边和上边。dLeft < dRight，dTop < dBottom.[in]
	//! \param  dRight, dBottom 显示范围的右边和下边。这是个值给的是地理坐标[in]
	//! \param  nDeviceWidth, nDeviceHeight,真实显示窗口的大小，屏幕象素单位。[in]
	//! \remarks 对于多分辨率的图象，可以按照窗口大小和显示范围大小，确定图象的分辨率,从而决定从那一层取得影像数据
	virtual bool SetViewPort(double dLeft,double dTop, double dRight, double dBottom, int nDeviceWidth, int nDeviceHeight) = 0;

	//! \brief 设置当前显示范围后,返回显示范围内影像数据的宽度。
	//! \remarks 对于多分辨率图象，可能从另外一层读取数据，所以输出的宽度可能和显示范围的宽度不一样
	//! \param  nViewWidth 显示数据影像宽度。[out]
	//! \param  nViewHeight 显示数据影像高度。[out]
	virtual bool GetViewImageSize(int& nViewWidth, int& nViewHeight)= 0;
	
	//! \brief 设置当前显示范围后,返回显示范围内数据的大小。
	virtual int GetViewImageDataSize() = 0; 

	//! \brief 设置当前显示范围后,读取显示范围内的影像数据,外部取得数据以后，可以用来显示
	//! \param  pDataBuffer 影像数据，函数外分配空间。[in]
	//! \param  nSize 数据大小(字节数），一般就是GetViewImageDataSize（）返回值[in]
	virtual bool GetViewImageData(unsigned char* pDataBuffer, int nSize)= 0;

	//! \brief 设置地图的固定比例尺。某些数据源，例如地图缓存插件要求设置地图的固定比例尺
	//! \brief pMap 所需要设置固定比例尺的地图 [in]
	//! \brief pCallBack 用来设置固定比例尺的回调函数。该回调函数第一个参数为 UGMap 指针，第二个参数为所需要设置的固定比例尺 [in]
	virtual void ConfigureMap(void *pMap, void (*pCallBack) (void *, std::vector<double>)) { /* do nothing */ }

public:
	//! \brief 释放自己。加这个方法主要是一个动态库内分配的堆内存，
	//! \remarks 在另外一个动态库中释放可能会出错，所以给各方法释放自己。
	virtual bool BuildPyramid(bool bShhowProgress){return FALSE;}

	virtual bool RemovePyramids(){return FALSE;}

	virtual bool IsBuildPyramid() const{return FALSE;}

	//! \brief 为栅格文件修改密码
	//! \param  bool 修改密码操作是否成功[out]
	//! \param  strNewPassword 设置的新密码[in]
	virtual bool ChangePassword(const UGchar* strNewPassword){return false;}

	//! \brief 为获取栅格文件的缓存访问密码
	//! \return 返回已设置的密码，若没有设置则返回空
	virtual const UGchar* GetPassword() {return NULL;}

	//! \brief 得到支持的影像文件扩展名数目。
	virtual int GetExtNameCount() const{return 1;}
	
	//! \brief  得到波段数目。
	virtual int GetBandCount() const {return 0;}

	//! \brief  设置当前取数据的波段索引。
	//! \param  nBandIndex 波段索引。[in]
	virtual void SetCurBandIndex(int nCurBandIndex){m_nCurBandIndex = nCurBandIndex;}

	//! \brief  得到当前取数据的波段索引。
	virtual int GetCurBandIndex() const{return m_nCurBandIndex;}

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
	virtual const UGchar* GetPrjCoordSys(){return NULL;}

	//! \brief 设置文件版本信息
	virtual void SetVersion(int nVersion){}
	virtual int GetVersion(){return 0;}
	
	//! \brief 得到指定范围数据的版本信息
	virtual int GetDataVersion(double dLeft,double dTop, double dRight, double dBottom, int nDeviceWidth, int nDeviceHeight){return 0;}
	
	//! \brief 得到裁剪区域,不需要外面分配内存，但是需要外面释放。
	virtual bool GetClipRgn(double*& pXCoord, double*& pYCoord, int*& pPloyCounts, int& nSubCount){return false;}
	
	//! \brief 释放临时数据。
	virtual void Release(){}

	//! \brief 释放缓存中的数据
	virtual void FreeCache() {}

	//! \brief 数据下载进度
	virtual float GetDataStreamingProgress(){return -1.0;}

	//! brief 得到显示的金字塔的所需要的内存块(字节)
	//! param nBand 波段
	//! param nTierIndex 金字塔索引
	virtual int GetViewPyramidDataSize(int nBand, int nTierIndex){return 0;}

	//! \brief 设置显示的地理范围
	//! \param 显示的范围
	//! \remarks 这个函数是针对UGPluginCommon类设置的，在分块读取RDAS的IMG格式文件时会用到
	//! 在UGDatasetRasterImg::GetViewBlock函数中调用
	virtual void SetViewBounds(double dLeft, double dTop, double dRight, double dBottom){};

	//! \brief GetViewImageData() 获取数据是否已经获取完毕，如果没有，需要继续调用 GetViewImageData() 获取数据。当前只有 UGPluginMapCache 有可能一次性无法获取全部数据。
	virtual bool IsFinished() { return true; }

	//!	\brief	获取插件的全路径文件名
	virtual const UGchar* GetPluginFileName() { return NULL; }

	//! \brief 设置是否背景透明
	virtual void SetBackTransparent(UGbool bBackTransparent){ m_bBackTransparent = bBackTransparent; }

	// 获取是否背景透明
	virtual bool IsTransparentBack() { return m_bBackTransparent; }

	//! \brief判断影像插件是否解析的是OGC服务
	virtual UGbool IsOGCServerMap(){ return false; };

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
	//! \return 。
	//! \remarks 需要缓存的有WMS图层
	//! \attention 。
	virtual void SetCachePath(const UGString& strCachePath) { }

public:
	//! \brief  调色板数据。
	UGImagePaletteEntry m_palette[MAXIMGAEPALETTEENTRY];  
	//! \brief  无值数据
	double m_dNoValue;

protected:
	//! \brief  当前波段索引。
	int m_nCurBandIndex;
	int m_bandCombine[4];
	ImageColorSpace m_nColorSpace;

	//! \brief 是否背景透明
	UGbool m_bBackTransparent; 

	PluginClass m_nPluginClass;		// 当前影像插件类型
};

// 插件需要实现一个导出的全局函数用来返回 UGImagePlugin 派生的类。
// 创建一个插件类，请new 一个从 UGImagePlugin 派生的类。
extern "C" IMAGEEXPORT UGImagePlugin* CreateImagePlugin();

#endif // !defined(AFX_UGIMAGEPLUGIN_H__8C978A6D_1011_4B0F_A221_C9BEDA07CCD8__INCLUDED_)

