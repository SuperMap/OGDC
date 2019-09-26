

#ifndef UGLICENSEENV_H
#define UGLICENSEENV_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Stream/ugdefs.h"

namespace UGC
{			
	
	class TOOLKIT_API UGLicenseEnvironment
	{

	public:
	enum ChartLicenseState
	{
		UnEnable			= -1,	// 不可用
		iServerEnable		= 0,	// iServer许可可用
		DeskproEnable		= 1,	// 桌面许可可用
		ObjectEnable		= 2,	// 组件许可可用
	};									

	protected:
		UGLicenseEnvironment();		
	public:	

		// 设置是否试用许可, 0是正式许可，1为地图试用许可，2为布局试用许可，3为三维试用许可
		static void SetIsUnregistered(UGint nType, UGbool bUnregistered);
		static UGbool GetIsUnregistered(UGint nType);

		// 设置或获取当前海图许可是否可用 [8/31/2011 zhoujt]
		static void SetChartLicenseState(UGint nCode);
		static UGLicenseEnvironment::ChartLicenseState GetChartLicenseState();

		// 获取和设置当前产品是否允许绘制海图图层 [9/2/2011 zhoujt]
		static void SetDrawChartLayer(UGbool bEnable = false);
		static UGbool GetDrawChartLayer();

		static UGbool GetLicCheckMode();
	
		static void SetLicCheckMode(UGString mode);

		// 是否使用云许可
		static void SetUseCloudLicense(UGbool bUse = false);
		static UGbool IsUseCloudLicense();

		// 设置云许可水印的文字
		static void SetCloudLicenseText(UGString text);
		static UGString GetCloudLicenseText();
	private:

		// 是否试用许可，默认false
		static UGbool m_bUnregisteredMap;
		static UGbool m_bUnregisteredLayout;
		static UGbool m_bUnregisteredSpace;

		// 当前海图许可的状态
		// iServer许可可用 --- 0; 桌面许可可用 --- 1; 组件许可可用 -- 2; 没有许可 --- -1;
		static UGLicenseEnvironment::ChartLicenseState m_eChartLicenseState;
		static UGbool m_bDrawCharLayer;
		static UGString g_checkMode;

		static UGbool g_bIsUseCloudLicense;
		static UGString g_strCloudLicenseText;
	};	
	
}
#endif //UGLICENSE
