
/*!
   **************************************************************************************
    \file     UGTextStyle.h
   **************************************************************************************
    \author   陈国雄
    \brief    文本风格类                                                             
    \attention   
   ----------------------------------------------------------------------------------<br>
    Copyright (c) 2000-2010 SuperMap Software Co., Ltd.                           <br>
    All Rights Reserved.                                                             <br>
   ----------------------------------------------------------------------------------<br> 
   
   **************************************************************************************
    \version 2005-07-06  陈国雄    初始化版本.                                       <br>
   **************************************************************************************
*/

#ifndef UGTEXTSTYLE_H
#define UGTEXTSTYLE_H

#include "Stream/ugdefs.h"

namespace UGC {

//! \brief 文本风格类
class TOOLKIT_API UGTextStyle  
{
public:
	//! \brief
	UGTextStyle();
	//! \brief
	UGTextStyle(const UGTextStyle& textStyle);
	//! \brief
	~UGTextStyle();

	//! \brief
	UGTextStyle& operator=(const UGTextStyle& textStyle);
	//! \brief
	UGbool operator==(const UGTextStyle& textStyle) const;
	//! \brief
	UGbool operator!=(const UGTextStyle& textStyle) const;

public:
	//! \brief
	UGbool FromOgdcTextStyle(const OgdcTextStyle& ogdcTextStyle);
	//! \brief
	UGbool ToOgdcTextStyle(OgdcTextStyle& ogdcTextStyle) const;
	
	//! \brief  得到字体颜色
	UGColor GetForeColor() const;	
	//! \brief  设置字体颜色
	void SetForeColor(UGColor foreColor);	
	  
	//! \brief  背景色，阴影色，外框色
	UGColor	GetBackColor() const;
	//! \brief  设置背景色,同时也是阴影色，外框色
	void SetBackColor(UGColor backColor);	
	  
	//! \brief  得到旋转角度，以度为单位，逆时针方向
	UGdouble GetAngle() const;	
	//! \brief  设置旋转角度，以度为单位，逆时针方向
	void SetAngle(UGdouble dAngle);

	//! \brief  得到字体宽度，地理单位
	UGdouble GetWidth() const;
	//! \brief  设置字体宽度，地理单位
	void SetWidth(UGdouble dWidth);

	//! \brief  得到字体高度，地理单位
	UGdouble GetHeight() const;	
	//! \brief  设置字体高度，地理单位
	void SetHeight(UGdouble dHeight);
	// added by lidl 2008-9-10 16:43:20
	  
	//! \brief  得到字号,单位：磅 
	UGdouble GetSize() const;
	//! \brief  设置字号，单位：磅
	void SetSize(UGdouble dSize);	
	
	//! \brief 得到字体名称	
	UGString GetFaceName() const;
     //! \brief  设置字体名称
	void SetFaceName(const UGString& strFaceName);
	
	//文本的风格,   0x BYTE4 BYTE3 BYTE2 BYTE1 
	//由几部分组成,	     |     |     |     |
	//具体如右图:        |     |     |     +--Text Align
	//				     |     |     +--Text Style
	//				     |     +--Text Font Weight
	//                   +--Text Other Attribute 
	//========================================================
	//BYTE1 文本的对齐方式
	//	   左  中  右
	// 上	0	1	2
	// 中	3	4	5
	// 下	6	7	8
	//--------------------------------------------------------
	//BYTE2 文本的一般风格	   本字节     在m_lStyle中
	//粗体	(Bold)				0x01     ( 0x00000100 )
	//斜体  (Italic)			0x02     ( 0x00000200 )
	//下划线(Underline)			0x04     ( 0x00000400 )
	//删除线(Stroke)			0x08     ( 0x00000800 )
	//--------------------------------------------------------
	//BYTE2 文本的高级风格	   本字节     在m_lStyle中
	//固定大小(FixedSize)		0x10     ( 0x00001000 )
	//背景填充(Opaque)			0x20     ( 0x00002000 )
	//轮廓背景(Outline)			0x40     ( 0x00004000 )    
	//显示阴影(Shadow)			0x80     ( 0x00008000 )    
	//--------------------------------------------------------
	//BYTE3 文本的笔划粗细 --> 与windows一致
	//--------------------------------------------------------
	//BYTE4固定大小的文本的尺寸 本字节    在m_lStyle中
	//固定的大小(FixedSize)		0xff     ( 0xff000000 )
	//========================================================
	//! \brief  得到所有风格信息:粗体，斜体，透明，黑体,排列等
	UGint GetStyle() const;			
	//! \brief 设置所有的风格，高级用户使用!
	void SetStyle(UGint nStyle);	

	//! \brief Toolkit
	//! \brief  是否粗体
	UGbool IsBold() const;	
	//! \brief  设置是否粗体
	void SetBold(UGbool bBold = true);				
	
	//! \brief  是否下划线
	UGbool IsUnderline() const;
	//! \brief  设置是否下划线
	void SetUnderline(UGbool bUnderline = true);		
	  
	//! \brief  是否斜体
	UGbool IsItalic() const;	
	//! \brief  设置是否斜体
	void SetItalic(UGbool bItalic = true);			
	
	//! \brief  是否删除线
	UGbool IsStrikeOut() const;		
	//! \brief 设置是否删除线
	void SetStrikeOut(UGbool bStrikeOut = true);
	
	//! \brief  得到对齐方式
	UGbyte GetAlign() const;				
	//! \brief  设置对齐方式,详细的见SetStyle方法备注
	void SetAlign(UGbyte nAlign = 0);
	
	//! \brief  得到对齐方式
	UGbyte GetStringAlign() const;				
	//! \brief  设置对齐方式
	void SetStringAlign(UGbyte nAlign = 0);
	  
	//! \brief  得到文本的笔划宽度
	UGint GetWeight() const;					
	//! \brief  设置文本的笔划宽度
	void SetWeight(UGint nWeight);	
	  
	//! \brief  是否固定大小
	UGbool IsFixedSize() const;			
	//! \brief 固定大小，字体不随地图的缩放而缩放
	void SetFixedSize(UGbool bFixedSize=true);			
	
	//! \brief  固定大小的尺寸，mm单位
	UGdouble GetSizeForFixed() const;			
	//! \brief 设置固定大小的尺寸，mm单位
	void SetSizeForFixed(UGdouble dSize);		

	  
	//!  \brief 是否背景不透明, 注意了,true表示不透明!!
	UGbool IsBackOpaque() const;				
	//! \brief  设置是否背景不透明
	void SetBackOpaque(UGbool bBackOpaque = false);				

	//! \brief 是否显示轮廓
	UGbool IsHalo() const;			
	//! \brief  设置是否显示轮廓
	void SetHalo(UGbool bHalo = true);	

	//! \brief  设置轮廓宽度，像素坐标单位，范围0~5
	void SetHaloWidth(UGint nWidth);
	//! \brief  得到轮廓宽度
	UGint GetHaloWidth() const;

	//! \brief  设置文字背景矩形边缘与文字边缘间隔宽度，像素坐标单位，
	void SetBorderSpacingWidth(UGint nWidth);
	//! \brief  得到文字背景矩形边缘与文字边缘间隔宽度
	UGint GetBorderSpacingWidth() const;

	//{{added by mahb at 20120716
	//! \brief  设置背景的透明度
	//! \params ucTransparency 透明度，0~255；0为透明，255为不透明
	void SetBackTransparency(UGuchar ucTransparency);
	//! \brief  获取背景的透明度
	UGuchar GetBackTransparency() const;
	
	//! \brief  是否显示阴影
	UGbool IsShadow() const;	
	//! \brief  设置是否显示阴影
	void SetShadow(UGbool bShadow = true);			
	
	UGbool FromXML(const UGString& strXML, UGint nVersion = 0);

	UGString ToXML(UGint nVersion = 0) const;

		//! \brief 获取文本风格对象的hashcode
	UGint GetHashCode() const;

	//! \brief 获取/设置耸肩效果,正负60度之间，目前只对标签专题图有效，传入的是度为单位
	UGdouble GetItalicAngle() const;
	void SetItalicAngle(UGdouble nItalicAngle);

	//! \brief  得到3D文本的透明度
	//! \remarks 该参数仅在三维中使用，支持保存为XMLUGC信息,SFC版本不保存，数据库中不保存此信息，默认为100,不透明,OGDC中不保存此风格
	UGint GetText3DOpaque() const;		
	//! \brief 设置3D文本的不透明度
	void SetText3DOpaque(UGint nOpaque);
	
	//! \brief 得到3D文本的缩放比例
	//! \remarks 该参数仅在三维中临时使用，不保存，OGDC中不保存此风格
	UGdouble  GetText3DScale() const;		
	//! \brief 设置文本的缩放比例
	void SetText3DScale(UGdouble dScale);	

	//! \brief 获取阴影X方向偏移量
	 UGdouble GetShadowOffsetX() const;	
	//! \brief 设置阴影X方向偏移量
	void SetShadowOffsetX(UGdouble nValue);

	//! \brief 获取阴影X方向偏移量
	UGdouble GetShadowOffsetY() const;	
	//! \brief 设置阴影X方向偏移量
	void SetShadowOffsetY(UGdouble nValue);

	//! \brief 获取阴影颜色
	UGColor GetShadowColor() const;
	//! \brief 设置阴影颜色
	//! \param clrColor 阴影颜色
	void SetShadowColor(UGColor clrColor);

	//! \brief 设置Style的默认值,和组件接口的默认值保持一致
	//! 目前仅UGMap的m_showTimeTextStyle使用，其他慎用
	void SetDefault();

	//! \brief 获取字体是否向左倾斜
	UGbool IsLeftItalic() const;
	void SetLeftItalic(UGbool isLeftItalic);

	// 内部给标签绘制背景时使用的，不做任何FromXML和ToXML，不要用来做其他用途 by gouyu 2014-8-18
public:
	UGint		m_backMode;				// 标签背景样式，目前只支持矩形和圆角矩形，且纯色的背景
	UGColor		m_backStyleLineColor;	// 标签背景的边线颜色
	UGColor		m_backStyleRectColor;	// 标签背景的背景颜色	
	UGbool		m_drawBack;				// 是否绘制背景，当填充ID为1时设置为false，就不绘制背景
	UGbool		m_drawLine;				// 是否绘制边线，当线型ID为5时设置为false，就不绘制边线
	UGbyte		m_fillOpaqueRate;		// 背景透明度

	// 清华山维 UGGeoTextEPS 类型风格中的剩余风格成员
	UGint		m_nFontInterval;		//字隔
	UGdouble	m_dDownAngle;			//耸肩,字的左下角到字的右下角与底线的夹角(度)
	UGdouble	m_dWordAngle;			//字角度,字底线与正东的夹角(度)
	UGint		m_nBackBorderStyle;		//边框样式
	UGint		m_nBackLineWidth;		//边框线宽

private:
	UGColor		m_ForeColor;			//! 字体颜色
	UGColor		m_BackColor;			//! 背景色，阴影色，外框色
	UGint		m_nAngle;				//! 旋转角度，以0.1度为单位,x坐标正方向为起始，逆时针方向
	UGint		m_nItalicAngle;			//! 旋转角度，以0.1度为单位,x坐标正方向为起始，逆时针方向
	UGdouble	m_dWidth;				//! 字体宽度，地理单位
	UGdouble	m_dHeight;				//! 字体高度，地理单位
	UGString	m_strFaceName;			//! 字体名字
	UGint		m_nStyle;				//! 风格:粗体，斜体，透明，黑体,排列等	
	UGbyte      m_nText3DOpaque;
	UGdouble    m_dText3DScale;
	//UGdouble    m_dSize;				//! 字体字号

	//{{ added by mahb at 20120716
	UGuchar     m_ucBackTransparency;   //! 背景透明度

	UGint		m_nHaloWidth;			//! 轮廓的宽度，像素单位，范围是从0~5像素

	UGint		m_nBorderSpacingWidth;	//! 背景矩形框边缘与文字边缘的间隔距离，像素单位

	//! \brief 阴影X方向偏移量
	UGdouble	m_dShadowOffsetX;//像素单位
	//! \brief 阴影Y方向偏移量
	UGdouble	m_dShadowOffsetY;
	//! \brief 阴影的颜色
	UGColor		m_clrShadowColor;   

};

}

#endif

