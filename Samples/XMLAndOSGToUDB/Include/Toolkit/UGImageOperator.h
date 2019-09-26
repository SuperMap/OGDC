// 内存图像处理类,包括一些常用的图像处理
// 此类的所有方法均为静态方法
// 作者:王茂
// 日期:2010-12-15
// 版本:1.0.0

#ifndef  UGIMAGEOPERATOR_H
#define  UGIMAGEOPERATOR_H

#include "Stream/ugdefs.h"
#include "Toolkit/UGPalette.h"
#include "Toolkit/UGTextureInfo.h"
#include "Geometry/UGDataCodec.h"
#include <algorithm>

#define DXT1_ENCODE_SIZE(w,h) (((w-1)/4 + 1)*((h-1)/4 + 1)*8)
#define DXT3_ENCODE_SIZE(w,h) (((w-1)/4 + 1)*((h-1)/4 + 1)*16)
#define DXT5_ENCODE_SIZE DXT3_ENCODE_SIZE

#define PVRTC_RGBA2_ENCODE_SIZE(w,h) ((UGMAX((int)w,16) * UGMAX((int)h, 8) * 2 + 7)/8)

#define PVRTC_RGBA4_ENCODE_SIZE(w,h) ((UGMAX((int)w, 8) * UGMAX((int)h, 8) * 4 + 7)/8)
#define ETC1_RGB8_ENCODE_SIZE(w,h) ((w*h)>>1)
#define ETC1_RGBA8_ENCODE_SIZE ETC1_RGB8_ENCODE_SIZE

using namespace UGC;

#if defined OPENGL_ES_VERSION2
struct PVRFILEHEADER{
	UGuint	u32Version;			//Version of the file header, used to identify it.
	UGuint	u32Flags;			//Various format flags.
	UGulong	u64PixelFormat;		//The pixel format, 8cc value storing the 4 channel identifiers and their respective sizes.
	UGuint	u32ColourSpace;		//The Colour Space of the texture, currently either linear RGB or sRGB.
	UGuint	u32ChannelType;		//Variable type that the channel is stored in. Supports signed/unsigned int/short/byte or float for now.
	UGuint	u32Height;			//Height of the texture.
	UGuint	u32Width;			//Width of the texture.
	UGuint	u32Depth;			//Depth of the texture. (Z-slices)
	UGuint	u32NumSurfaces;		//Number of members in a Texture Array.
	UGuint	u32NumFaces;		//Number of faces in a Cube Map. Maybe be a value other than 6.
	UGuint	u32MIPMapCount;		//Number of MIP Maps in the texture - NB: Includes top level.
	UGuint	u32MetaDataSize;	//Size of the accompanying meta data.
};

struct KTXFILEHEADER{
	UGbyte identifier[12];
	UGuint endianness;
	UGuint glType;
	UGuint glTypeSize;
	UGuint glFormat;
	UGuint glInternalFormat;
	UGuint glBaseInternalFormat;
	UGuint pixelWidth;
	UGuint pixelHeight;
	UGuint pixelDepth;
	UGuint numberOfArrayElements;
	UGuint numberOfFaces;
	UGuint numberOfMipmapLevels;
	UGuint bytesOfKeyValueData;
};

#endif

class TOOLKIT_API UGImageOperator
{
public:
	// format 原图像的像素格式
	// width,height 原图像的宽度和高度
	// in 原图像的数据
	// comtype 图像压缩类型
	// out  压缩后的输出,内部分配空间
	// return 返回压缩后图像数据的大小
	static UGuint Encode( const UGuint pixsize, UGuint &nWidth, UGuint &nHeight,UGbyte* pBufferIn,UGbyte **ppBufferOut,UGuint eCodecType =UGDataCodec::enrS3TCDXTN,UGbool bGeneMipmaps =FALSE);

	// 解压缩dxtn
	static UGuint Decode( const UGuint pixsize, UGuint nWidth, UGuint nHeight,UGbyte** ppBufferOut,const UGbyte *pBufferIn ,UGuint eCodecType,UGbool bGeneMipmaps =FALSE);

	// 按照指定的大小缩放图片
	static void Scale(const UGuint components, UGuint widthin, UGuint heightin,UGbyte *pBufferIn,
					        UGuint widthout, UGuint heightout, UGbyte *pBufferOut);

	//将影像数据缩放成正方形
	static UGbyte* MakeSquareForPVR(UGuint& nWidth, UGuint& nHeight, UGbyte* pBufferIn);

	// RGB,RGBA 到 BGR,BRGA之间相互转换,直接改变原始数据
	// RGB时components = 3,RGBA components = 4
	static void ImageSwapRB(const UGuint components, UGuint widthin, UGuint heightin,UGbyte *pBufferIn,UGbool bIsContainMipmap =FALSE);

 	// 获取mipmap的数据
	static UGuint GetMipMapData(UGuint nWidth,UGuint nHeight,const UGbyte* pBufferIn,UGbyte** ppBufferOut,UGArray<UGuint>& arrByteOffset);

 	//! \brief  返回比a大的，并且是最接近a的2的次方的数。
 	static UGuint NextP2 (UGuint a);

	//! \brief 把ImageData转化成TextureInfo
	//! \brief bMipmapCompress带mipmap并且dxt压缩
	static void ImageData2TexInfo(const UGImageData& ImageData,UGTextureInfo &Info,UGbool bMipmapCompress=TRUE);

	//! \brief 获取指定压缩格式的纹理压缩后的大小
	//! \brief
	static UGuint GetCompressedTextureSize(const UGuint pixsize,UGuint nWidth,UGuint nHeight,UGuint eCodecType,UGbool bIsMipmaps =FALSE);

	//! \brief 根据宽高获取mipMap层级
	static UGuint GetMipMapLevel(const UGuint nWidth,const UGuint nHeight);


	/** 转化为纹理地图集
		@remarks
		将textInfo的宽高均扩大一倍
		@note
		新的纹理左上1/4存储原来纹理的RGB,左下角1/4存储alpha,新纹理右半部分为空		
    */
	static void TextureToAtlas(UGTextureInfo& textInfo);

	/** 纹理地图集转化为纹理
        @remarks
            将textInfo的宽高均缩小为原来的一半
        @note
            新的纹理的RGB来自原纹理左上1/4部分,alpha来自原纹理左下1/4部分  			
    */
	static void AltasToTexture(UGTextureInfo& textInfo);

	//! \是否是压缩纹理
	static UGbool IsCompressedTextureType(UGDataCodec::CodecType eType);
    
#if defined OPENGL_ES_VERSION2
    //! \brief get the pvr file contenie
    static void GetPvrTextureData(const UGString &strFilePath,UGTextureInfo& textData);
	//! \brief get the pvr file contenie
	static void GetKtxTextureData(const UGString &strFilePath,UGTextureInfo& textData);
#endif

private:


#if _MSC_VER > 1200
	//! \brief 根据UGC编码获取pvr像素格式
	static UGuint GetPVRPixelFormat(UGuint eCodecType, UGuint nComponent);
#endif
};
#endif

