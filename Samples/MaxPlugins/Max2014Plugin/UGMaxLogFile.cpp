
#include "max.h"
#include "Stream/UGFile.h"
#include "Toolkit/UGMarkup.h"
#include "Toolkit/UGInter.h"
#include "UGMaxLogFile.h"
#include "Toolkit/UGLicenseEnvironment.h"
UGMaxLogFile* UGMaxLogFile::m_pMaxLogFile = NULL;

UGMaxLogFile* UGMaxLogFile::GetMaxLogFile()
{
	if(!m_pMaxLogFile)
	{		
		m_pMaxLogFile = new UGMaxLogFile();
	}
	return m_pMaxLogFile;		
}

UGMaxLogFile::UGMaxLogFile()
{
	UGLicenseEnvironment::SetLicCheckMode(_U("b8a93b98-89bf-4f4b-897b-7b520b0f1b02"));
	UGString strConfigFile =  GetCOREInterface()->GetDir(APP_MAX_SYS_ROOT_DIR);
	strConfigFile =strConfigFile + _U("SupermapMaxLog.dat");//   Modify by lina 2014-3-4 字符转换
		
	m_strLongtitude = _U("0.0");
	m_strLatitude = _U("0.0");
	m_strAltitude = _U("0.0");
	m_strPlaneX = _U("0.0");
	m_strPlaneY = _U("0.0");
	m_strPlaneZ = _U("0.0");
	m_nTextureScaling = 0;
	m_nMaxTextureSize = 8192;
	m_strBatchTextureSize = _U("2048x2048");
	m_strBatchTerrainSize = _U("600x600");
	m_strPrefix = _U("");

	if (UGFile::IsExist(strConfigFile))
	{			
		UGString strElemValue;
		UGMarkup markUp;
		if (markUp.Load(strConfigFile))
		{	
			UGString strElemName = _U("SuperMapCache");
			if (markUp.FindElem(strElemName))
			{
				strElemName = _U("sml:Longtitude");
				if (markUp.FindChildElem(strElemName))
				{
					m_strLongtitude = markUp.GetChildData();
				}
				
				strElemName = _U("sml:Latitude");
				if(markUp.FindChildElem(strElemName))
				{
					m_strLatitude = markUp.GetChildData();
				}

				strElemName = _U("sml:Altitude");
				if(markUp.FindChildElem(strElemName))
				{
					m_strAltitude = markUp.GetChildData();
				}

				strElemName = _U("sml:TextureScaling");
				if(markUp.FindChildElem(strElemName))
				{
					strElemValue = markUp.GetChildData();
					m_nTextureScaling = ugatoi(strElemValue);
					if(m_nTextureScaling>3||m_nTextureScaling<0)
					{
						m_nTextureScaling = 0;
					}
				}
				
				strElemName = _U("sml:MaxTextureSize");
				if(markUp.FindChildElem(strElemName))
				{
					strElemValue = markUp.GetChildData();
					m_nMaxTextureSize = ugatoi(strElemValue);
					if(m_nMaxTextureSize>8192||m_nMaxTextureSize<0)
					{
						m_nMaxTextureSize = 8192;
					}
				}

				strElemName = _U("sml:BatchTextureSize");
				if (markUp.FindChildElem(strElemName))
				{
					m_strBatchTextureSize = markUp.GetChildData();
				}

				strElemName = _U("sml:BatchTerrainSize");
				if (markUp.FindChildElem(strElemName))
				{
					m_strBatchTerrainSize = markUp.GetChildData();
				}
				
				//Add by lina 用于标记平面坐标 2014-3-31
				strElemName =  _U("sml:PlaneX");
				if (markUp.FindChildElem(strElemName))
				{
					m_strPlaneX = markUp.GetChildData();
				}

				strElemName =  _U("sml:PlaneY");
				if (markUp.FindChildElem(strElemName))
				{
					m_strPlaneY = markUp.GetChildData();
				}

				strElemName =  _U("sml:PlaneZ");
				if (markUp.FindChildElem(strElemName))
				{
					m_strPlaneZ = markUp.GetChildData();
				}
			}
		}
	}
}

UGMaxLogFile::~UGMaxLogFile()
{
	UGMarkup markUp;
	UGString strElemValue;
	UGString strElemName = _U("SuperMapCache");//   Modify by lina 2014-3-4 字符转换
	markUp.AddElem(strElemName);

	strElemName =  _U("sml:Longtitude");
	markUp.AddChildElem(strElemName,m_strLongtitude);

	strElemName =  _U("sml:Latitude");
	markUp.AddChildElem(strElemName,m_strLatitude);

	strElemName =  _U("sml:Altitude");
	markUp.AddChildElem(strElemName,m_strAltitude);

	strElemName =  _U("sml:TextureScaling");
	strElemValue.Format(_U("%d"), m_nTextureScaling);
	markUp.AddChildElem(strElemName, strElemValue);

	strElemName =  _U("sml:MaxTextureSize");
	strElemValue.Format(_U("%d"), m_nMaxTextureSize);
	markUp.AddChildElem(strElemName, strElemValue);

	strElemName =  _U("sml:BatchTextureSize");
	markUp.AddChildElem(strElemName, m_strBatchTextureSize);

	strElemName =  _U("sml:BatchTerrainSize");
	markUp.AddChildElem(strElemName, m_strBatchTerrainSize);

	//Add by lina 用于标记平面坐标 2014-3-31
	strElemName =  _U("sml:PlaneX");
	markUp.AddChildElem(strElemName, m_strPlaneX);

	strElemName =  _U("sml:PlaneY");
	markUp.AddChildElem(strElemName, m_strPlaneY);

	strElemName =  _U("sml:PlaneZ");
	markUp.AddChildElem(strElemName, m_strPlaneZ);

	UGString strConfigFile =  GetCOREInterface()->GetDir(APP_MAX_SYS_ROOT_DIR);
	strConfigFile = strConfigFile+_U("SupermapMaxLog.dat");
	markUp.Save(strConfigFile);
}

void UGMaxLogFile::SetLontitude(UGString strLongtitude)
{
	m_strLongtitude = strLongtitude;
}
void UGMaxLogFile::SetLatitude(UGString strLatitude)
{
	m_strLatitude = strLatitude;
}
void UGMaxLogFile::SetAltitude(UGString strAltitude)
{
	m_strAltitude = strAltitude;
}
UGString UGMaxLogFile::GetLontitude()
{
	return m_strLongtitude;
}
UGString UGMaxLogFile::GetLatitude()
{
	return m_strLatitude;
}
UGString UGMaxLogFile::GetAltitude()
{
	return m_strAltitude;
}

void UGMaxLogFile::SetPlaneX(UGString strPlaneX)
{
	m_strPlaneX = strPlaneX;
}
void UGMaxLogFile::SetPlaneY(UGString strPlaneY)
{
	m_strPlaneY = strPlaneY;
}
void UGMaxLogFile::SetPlaneZ(UGString strPlaneZ)
{
	m_strPlaneZ = strPlaneZ;
}
UGString UGMaxLogFile::GetPlaneX()
{
	return m_strPlaneX;
}
UGString UGMaxLogFile::GetPlaneY()
{
	return m_strPlaneY;
}
UGString UGMaxLogFile::GetPlaneZ()
{
	return m_strPlaneZ;
}
//! \brief 支持的最大纹理
UGint UGMaxLogFile::GetMaxTextureSize(void) const
{
	return m_nMaxTextureSize;
}
void UGMaxLogFile::SetMaxTextureSize(UGint nMaxTextureSize)
{
	m_nMaxTextureSize = nMaxTextureSize;
}
//! \brief 支持的最大纹理
UGint UGMaxLogFile::GetTextureScaling(void) const
{
	return m_nTextureScaling;
}
void UGMaxLogFile::SetTextureScaling(UGint nTextureScaling)
{
	m_nTextureScaling = nTextureScaling;
}

UGDict<UGString,UGint>& UGMaxLogFile::GetMapName()
{
	return m_dicttMapName;
}

void UGMaxLogFile::SetBatchTextureSize(UGString& nBatchTextureSize)
{
	m_strBatchTextureSize = nBatchTextureSize;
}

UGString UGMaxLogFile::GetBatchTextureSize(void) const
{
	return m_strBatchTextureSize;
}

void UGMaxLogFile::SetBatchTerrainSize(UGString& nBatchTerrainSize)
{
	m_strBatchTerrainSize = nBatchTerrainSize;
}

UGString UGMaxLogFile::GetBatchTerrainSize(void) const
{
	return m_strBatchTerrainSize;
}
UGString UGMaxLogFile::GetTempDir()
{
	UGString strMapTemp = _U("MaxTmp");
	UGString strFileName = GetCOREInterface()->GetCurFilePath();
	if (!strFileName.IsEmpty())
	{
		UGuint nDirID = GetMapCode(strFileName);
		strMapTemp.Format(_U("%d"),nDirID);
	}
	m_strPrefix = GetMaxFilePathMapCode() + _U("_");

	return UGFile::GetTemporaryPath() + strMapTemp + _U("\\");
}
UGString UGMaxLogFile::GetTexPerfix()
{
	return m_strPrefix;
}
UGString UGMaxLogFile::GetMaxFilePathMapCode()
{
	UGString strMapCode = _U("");
	UGString strFileName = GetCOREInterface()->GetCurFilePath();
	if (!strFileName.IsEmpty())
	{
		UGuint nDirID = GetMapCode(strFileName);
		strMapCode.Format(_U("%d"),nDirID);
	}
	return strMapCode;
}
UGuint UGMaxLogFile::GetMapCode( const UGString& strkey )
{
	UGMBString strMB;
#ifdef _UGUNICODE
	strkey.ToMBString(strMB);
#else
	strMB = strkey;
#endif
	UGint nSize = strMB.GetLength();
	const UGachar* pch = strMB.Cstr();

	UGint num = 0x15051505;
	UGint num2 = num;
	for (UGint i = nSize; i > 0; i -= 2)
	{
		num = (((num << 5) + num) + (num >> 0x1b)) ^ pch[0];
		if (i <= 1)
		{
			break;
		}
		num2 = (((num2 << 5) + num2) + (num2 >> 0x1b)) ^ pch[1];
		pch += 2;
	}
	return (num + (num2 * 0x5d588b65));
}
