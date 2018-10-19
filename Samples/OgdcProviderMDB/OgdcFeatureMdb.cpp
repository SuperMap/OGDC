//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)              
//                                                           
//!  \file OgdcFeatureMdb.cpp
//!  \brief OgdcFeature 的接口实现。
//!  \details OGDC特征要素类的接口实现
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Limited Company.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#include "OgdcFeatureMdb.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace OGDC
{

OgdcFeatureMdb::OgdcFeatureMdb()
{
	m_nID = 0;
	m_pElement = NULL;
}

OgdcFeatureMdb::~OgdcFeatureMdb()
{
	if(m_pElement != NULL)
	{
		delete m_pElement;
		m_pElement = NULL;
	}
	OgdcInt nSize = m_fieldValues.GetSize();
	if (nSize)
	{
		for (OgdcInt i = 0; i < nSize; i++)
		{
			delete m_fieldValues.ElementAt(i);
		}
		m_fieldValues.RemoveAll();
	}
}

OgdcInt OgdcFeatureMdb::GetID() const
{
	return m_nID;
}

void OgdcFeatureMdb::SetID( OgdcInt nID )
{
	m_nID = nID;
}

OgdcElement* OgdcFeatureMdb::GetElement()
{
	return m_pElement;
}

void OgdcFeatureMdb::SetElement( OgdcElement* pElement )
{
	m_pElement = pElement;
}

OgdcBool OgdcFeatureMdb::GetValue( OgdcInt nIndex, OgdcVariant& varValue )
{

	OgdcByte* pByte = m_fieldValues.GetAt(nIndex);
	if (pByte == NULL)
	{
		varValue.Clear();
		return TRUE;
	}
	OgdcInt nSize = m_fieldDefines.ElementAt(nIndex).m_nSize;
	OgdcByte* pData /*= new OgdcByte[nSize]*/;
	pData = m_fieldValues.GetAt(nIndex);
	OgdcByte* pDataMirror = pData;

	switch (m_fieldDefines.GetAt(nIndex).m_nType)
	{
	case OgdcFieldInfo::Boolean:
	case OgdcFieldInfo::Byte:
		{
			OgdcByte* nValue = new OgdcByte[nSize];
			memcpy(nValue, pDataMirror, nSize);
			varValue = *nValue;
			nValue = NULL;
		}
		break;
	case OgdcFieldInfo::INT16:
		{
			OgdcShort* nValue = new OgdcShort;
			memcpy(nValue, pDataMirror, nSize);
			varValue = *nValue;
			delete nValue;
			nValue = NULL;
		}
		break;
	case OgdcFieldInfo::INT32:
		{
			OgdcInt* nValue = new OgdcInt;
			memcpy(nValue, pDataMirror, nSize);
			varValue = *nValue;
			delete nValue;
			nValue = NULL;
		}
		break;
	case OgdcFieldInfo::INT64:
		{
			OgdcLong* nValue = new OgdcLong;
			memcpy(nValue, pDataMirror, nSize);
			varValue = *nValue;
			delete nValue;
			nValue = NULL;
		}
		break;
	case OgdcFieldInfo::Double:
		{
			OgdcDouble* dValue = new OgdcDouble;
			memcpy(dValue, pDataMirror, nSize);
			varValue = *dValue;
			delete dValue;
			dValue = NULL;
		}
		break;
	case OgdcFieldInfo::Float:
		{
			OgdcFloat* dValue = new OgdcFloat;
			memcpy(dValue, pDataMirror, nSize);
			varValue = *dValue;
			delete dValue;
			dValue = NULL;
		}
		break;
	case OgdcFieldInfo::Text:
	case OgdcFieldInfo::Char:
	case OgdcFieldInfo::NText:
		{
			OgdcString strTemp = OgdcString((OgdcChar*)pByte);
			strTemp.SetCharset(GetCharset());
			varValue = strTemp;
		}
		break;
	case OgdcFieldInfo::Date:
	case OgdcFieldInfo::Time:
	case OgdcFieldInfo::TimeStamp:
		{
			OgdcDouble* dValue = new OgdcDouble[nSize];
			memcpy(dValue, pDataMirror, nSize);
			OgdcTime dateTime = *dValue;
			varValue = dateTime;
			dValue = NULL;
		}
		break;
	case OgdcFieldInfo::LongBinary:
		{
			OgdcByte* pValues = new OgdcByte[nSize];
			memcpy(pValues, pDataMirror, nSize);
			varValue.Clear();
			varValue.Set(pValues, nSize);
			pValues = NULL;
		}
		break;
	case OgdcFieldInfo::Geometry:
	default:
		{
			varValue.Clear();
			return FALSE;
		}
		break;
	}

	pData = NULL;
	pDataMirror = NULL;
	return TRUE;
}

OgdcBool OgdcFeatureMdb::SetValue( OgdcInt nIndex, const OgdcVariant& varValue )
{
	if (nIndex >= m_fieldValues.GetSize())
	{
		return FALSE;
	}
			
	OgdcByte* pData = new OgdcByte[m_fieldDefines.ElementAt(nIndex).m_nSize];
	OgdcByte* pDataMirror = pData;
	switch (m_fieldDefines.ElementAt(nIndex).m_nType)
	{		
	case OgdcFieldInfo::Boolean:
	case OgdcFieldInfo::Byte:
		{
			OgdcByte nResult = (OgdcByte)varValue.ToInt();
			memcpy(pDataMirror, &nResult,sizeof(OgdcByte));
			break;
		}
	case OgdcFieldInfo::INT16:
		{
			OgdcShort nResult = (OgdcShort)varValue.ToInt();
			memcpy(pDataMirror, &nResult,sizeof(OgdcShort));
			break;
		}
	case OgdcFieldInfo::INT32:
		{
			OgdcInt nResult = varValue.ToInt();
			memcpy(pDataMirror, &nResult, sizeof(OgdcInt));
			break;
		}
	case OgdcFieldInfo::INT64:
		{
			OgdcLong nResult = varValue.ToLong();
			memcpy(pDataMirror, &nResult, sizeof(OgdcLong));
			break;
		}
	case OgdcFieldInfo::Float:
		{
			OgdcFloat fResult = (OgdcFloat)varValue.ToDouble();
			memcpy(pDataMirror, &fResult, sizeof(OgdcFloat));
			break;
		}
	case OgdcFieldInfo::Double:
		{
			OgdcDouble dResult = varValue.ToDouble();
			memcpy(pDataMirror, &dResult, sizeof(OgdcDouble));
			break;
		}
	case OgdcFieldInfo::Text:
	case OgdcFieldInfo::Char:
	case OgdcFieldInfo::NText:
		{
			OgdcString strTemp = varValue.ToString();
			strTemp.SetCharset(m_nCharset);
			OgdcInt nLength = strTemp.GetLength() + 1;
			OgdcInt nByteSize = nLength * sizeof(OgdcChar);
			OgdcByte* pByte1 = new OgdcByte[nByteSize];
			memset(pByte1, 0, nByteSize);
			strcpy((char*)pByte1, (char*)strTemp.Cstr());
			m_fieldValues.SetAt(nIndex, pByte1);
			delete pData;
			pData = NULL;
			pByte1 = NULL;
			return TRUE;
		}
	case OgdcFieldInfo::Date:
	case OgdcFieldInfo::Time:	
	case OgdcFieldInfo::TimeStamp:
		{
			if (varValue.GetType() == OgdcVariant::Time) 
			{	
				memcpy(pDataMirror, &varValue.GetValue().tmVal,sizeof(OgdcDouble));
			}			
			break;
		}
	case OgdcFieldInfo::LongBinary:
		{
			if (varValue.GetType() == OgdcVariant::Binary) 
			{	
				OgdcInt nLength = varValue.m_value.binVal.nSize;
				nLength = OGDCMIN(m_fieldDefines.ElementAt(nIndex).m_nSize, nLength);
			
				memcpy(pDataMirror, varValue.m_value.binVal.pVal, nLength);
				m_fieldValues.SetAt(nIndex, pData);
				delete pData;
				pData = NULL;
				return TRUE;
			}		
			break;
		} 
	case OgdcFieldInfo::Geometry:
	default:
		return FALSE;		
	}
	if (pData == NULL)
	{
		pData = new OgdcByte[m_fieldDefines.ElementAt(nIndex).m_nSize];
	}
	delete m_fieldValues.ElementAt(nIndex);
	m_fieldValues.SetAt(nIndex, pData);

	//!!这里不能删除，不然外面又没值
//	delete pData;
	pData = NULL;
	pDataMirror = NULL;
	return TRUE;
}

OgdcBool OgdcFeatureMdb::SetValue( const OgdcString& strFieldName, const OgdcVariant& varValue )
{
	OgdcInt nCount = m_fieldDefines.GetSize();
	for (OgdcInt i = 0; i < nCount; i++)
	{
		if (m_fieldDefines.ElementAt(i).m_strName.CompareNoCase(strFieldName) == 0)
		{
			return SetValue(i, varValue);
		}
	}
	return FALSE;
}
OgdcBool OgdcFeatureMdb::GetValue( const OgdcString& strFieldName, OgdcVariant& varValue )
{
	OgdcInt nCount = m_fieldDefines.GetSize();
	for (OgdcInt i = 0; i < nCount; i++)
	{
		if (m_fieldDefines.GetAt(i).m_strName.CompareNoCase(strFieldName) == 0)
		{
			return GetValue(i, varValue);
		}
	}
	return FALSE;
}

void OgdcFeatureMdb::SetFieldInfos( const OgdcFieldInfos& fieldInfos )
{
	OgdcInt nCount = fieldInfos.GetSize();
	m_fieldDefines.SetSize(nCount);
	m_fieldValues.SetSize(nCount);

	OgdcFieldDefine fieldDefine;
	OgdcFieldInfo fieldInfo;
	for (OgdcInt i = 0; i < nCount; i++)
	{
		fieldInfo = fieldInfos.GetAt(i);
		fieldDefine.m_nSize = fieldInfo.m_nSize;
		fieldDefine.m_nType = fieldInfo.m_nType;
		fieldDefine.m_strName = fieldInfo.m_strName;
		m_fieldDefines.SetAt(i, fieldDefine);
	}
}

OgdcString::Charset OgdcFeatureMdb::GetCharset() const
{
	return m_nCharset;
}

void OgdcFeatureMdb::SetCharset( OgdcString::Charset nCharset )
{
	m_nCharset = nCharset;	
}

}