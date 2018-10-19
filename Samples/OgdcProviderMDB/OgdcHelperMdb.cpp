//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)           
//                                                           
//!  \file OgdcHelperMdb.cpp
//!  \brief OGDC辅助函数库的接口实现。
//!  \details 含有一些OGDC数据类型与其他数据类型的转换
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Limited Company.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#include "OgdcHelperMdb.h"
#include "OgdcMacroDefineMdb.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace OGDC
{

OgdcDict<OgdcString, OgdcString> g_keywords;
OgdcBool OgdcHelperMdb::m_bInited = FALSE;

OgdcHelperMdb::OgdcHelperMdb()
{

}

OgdcHelperMdb::~OgdcHelperMdb()
{
	g_keywords.RemoveAll();
}

void OgdcHelperMdb::OgdcFieldInfo2CDaoFieldInfo(const OgdcFieldInfo& fieldInfo, CDaoFieldInfo& daoFieldInfo )
{
	daoFieldInfo.m_strName		     = fieldInfo.m_strName; 
	daoFieldInfo.m_nType			 = fieldInfo.m_nType;
	daoFieldInfo.m_lSize			 = fieldInfo.m_nSize;                  
	daoFieldInfo.m_lAttributes	     = fieldInfo.m_nAttributes;            
	daoFieldInfo.m_nOrdinalPosition  = fieldInfo.m_nOrdinalPosition;      
	daoFieldInfo.m_bRequired		 = fieldInfo.m_bRequired;              
	daoFieldInfo.m_bAllowZeroLength  = fieldInfo.m_bAllowZeroLength;       
	daoFieldInfo.m_lCollatingOrder   = fieldInfo.m_nCollatingOrder;        
	daoFieldInfo.m_strForeignName	 = fieldInfo.m_strForeignName;      
	daoFieldInfo.m_strSourceField    = fieldInfo.m_strSourceField;      
	daoFieldInfo.m_strSourceTable	 = fieldInfo.m_strSourceTable;      
	daoFieldInfo.m_strValidationRule = fieldInfo.m_strValidationRule;   
	daoFieldInfo.m_strValidationText = fieldInfo.m_strValidationText;  
	OgdcFieldInfo field				 = fieldInfo;
	daoFieldInfo.m_strDefaultValue   = field.GetDefaultValue();  
}

void OgdcHelperMdb::CDaoFieldInfo2OgdcFieldInfo( const CDaoFieldInfo& daoFieldInfo, OgdcFieldInfo& fieldInfo )
{
	CDaoFieldInfo daoInfo = daoFieldInfo;
	fieldInfo.m_strName			  = daoInfo.m_strName; 
	fieldInfo.m_strForeignName	  = daoFieldInfo.m_strName	; 
	fieldInfo.m_nType			  = (OgdcFieldInfo::FieldType)daoInfo.m_nType;
	fieldInfo.m_nSize			  = daoInfo.m_lSize;                  
	fieldInfo.m_nAttributes		  = daoInfo.m_lAttributes;            
	fieldInfo.m_nOrdinalPosition  = daoInfo.m_nOrdinalPosition;      
	fieldInfo.m_bRequired		  = daoInfo.m_bRequired;              
	fieldInfo.m_bAllowZeroLength  = daoInfo.m_bAllowZeroLength;       
	fieldInfo.m_nCollatingOrder	  = daoInfo.m_lCollatingOrder;        
	fieldInfo.m_strForeignName	  = daoInfo.m_strForeignName;      
	fieldInfo.m_strSourceField	  = daoInfo.m_strSourceField;      
	fieldInfo.m_strSourceTable	  = daoInfo.m_strSourceTable;      
	fieldInfo.m_strValidationRule = daoInfo.m_strValidationRule;   
	fieldInfo.m_strValidationText = daoInfo.m_strValidationText;   	
	fieldInfo.m_bSystem           = IsSystemField(fieldInfo.m_strName);	
	fieldInfo.SetDefaultValue((LPCTSTR)daoInfo.m_strDefaultValue);  
}

OgdcBool OgdcHelperMdb::IsGeoField(OgdcString strName)
{
	strName.MakeUpper();
	if(strName.Find(_U("OGGEOMETRY")) == 0 ||
		strName.Find(_U(".OGGEOMETRY")) > 0 ||
		strName.Find(_U("OGX")) == 0 ||
		strName.Find(_U(".OGX")) > 0 ||
		strName.Find(_U("OGY")) == 0 ||
		strName.Find(_U(".OGY")) > 0 ||
		strName.Find(_U("OGZ")) == 0 ||
		strName.Find(_U(".OGZ")) > 0 )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

OgdcBool OgdcHelperMdb::IsOGID(OgdcString strName)
{
	OgdcInt nPos = strName.Find((_U(".")));
	if ( nPos > 0 )
	{
		strName = strName.Right(strName.GetLength() - nPos - 1);
	}

	if(strName.Left(2).CompareNoCase(_U("OG"))==0 )
	{
		if(strName.CompareNoCase(_U("OGUserID"))==0)
		{
			return FALSE;
		}
		else if ( strName.CompareNoCase(_U("OGID")) == 0 )
		{
			return TRUE;
		}
	}
	return FALSE;
}

OgdcBool OgdcHelperMdb::IsSystemField(const OgdcString& strName)
{
	//在这，OGDC里定义系统字段以OG开头
	OgdcString strCheckName = strName;
	strCheckName.MakeUpper();
	if (strCheckName.Left(2).CompareNoCase(_U("OG")) == 0) 
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void OgdcHelperMdb::SetFieldSign(OgdcFieldInfo& fieldInfo)
{	
	if(OgdcHelperMdb::IsOGID(fieldInfo.m_strName))
	{
		fieldInfo.m_nFieldSign = OgdcFieldInfo::signID;
	}
	else if(OgdcHelperMdb::IsGeoField(fieldInfo.m_strName))
	{
		fieldInfo.m_nFieldSign = OgdcFieldInfo::signGeometry;
	}
	if(OgdcHelperMdb::IsSystemField(fieldInfo.m_strName))
	{
		fieldInfo.m_bSystem = TRUE;
	}
}

void OgdcHelperMdb::COleVariant2OgdcVariant( const COleVariant& coleVariant, OgdcVariant& ogdcVariant )
{
	switch (coleVariant.vt) 
	{
	case VT_BOOL:

		if ( coleVariant.boolVal )
		{
			ogdcVariant = OgdcChar(TRUE);
		}
		else 
		{
			ogdcVariant = OgdcChar(FALSE);
		}
		break;
	case VT_UI1:
		ogdcVariant = coleVariant.bVal;
		break;
	case VT_I2:
		ogdcVariant = coleVariant.iVal;
		break;
	case VT_I4:
		ogdcVariant = coleVariant.lVal;
		break;
	case VT_R4:
		ogdcVariant = coleVariant.fltVal;
		break;
	case VT_R8:
		ogdcVariant = coleVariant.dblVal;
		break;
		
	case VT_DATE:
		{
			COleDateTime dt(coleVariant.date);
			ogdcVariant = OgdcTime(dt.GetYear(), dt.GetMonth(), dt.GetDay(), 
				dt.GetHour(), dt.GetMinute(), dt.GetSecond());
		}
		break;
	case VT_BSTR:
		{
			OgdcString str = OgdcString(coleVariant.bstrVal);
			OgdcString ugstr = str;
			ogdcVariant = OgdcVariant(ugstr);
		}
		break;
	case VT_NULL:
	case VT_EMPTY:
		ogdcVariant.Clear();
		break;
	case VT_DECIMAL:
		{
			double dVal = 0;
			VarR8FromDec((DECIMAL *)&coleVariant.decVal, &dVal);
			ogdcVariant = dVal;
	}
	default:
		break;
	}	
}

void OgdcHelperMdb::OgdcVariant2COleVariant( const OgdcVariant& ogdcVariant, COleVariant& coleVariant )
{
	switch (ogdcVariant.GetType()) 
	{
	case OgdcVariant::Byte:
		coleVariant = ogdcVariant.GetValue().bVal;
		break;
	case OgdcVariant::Short:
		coleVariant = ogdcVariant.GetValue().sVal;
		break;
	case OgdcVariant::Integer:
		coleVariant = (long)ogdcVariant.GetValue().iVal;
		break;
 	case OgdcVariant::Long:
 		coleVariant = (long)ogdcVariant.GetValue().lVal;
 		break;
	case OgdcVariant::Float:
		coleVariant = ogdcVariant.GetValue().fVal;
		break;
	case OgdcVariant::Double:
		coleVariant = ogdcVariant.GetValue().dVal;
		break;
	case OgdcVariant::Date:
	case OgdcVariant::Time:
	case OgdcFieldInfo::TimeStamp:
		{
			COleDateTime time(ogdcVariant.GetValue().tmVal);
			coleVariant = time;
		}
		break;
	case OgdcVariant::String:
		{
			OgdcString* pOgdcString = (OgdcString *)ogdcVariant.GetValue().binVal.pVal;
			if (pOgdcString != NULL)
			{
				CString str(pOgdcString->Cstr());
				coleVariant = str;
			}
			else
			{
				return;
			}
		}
		break;
	default:
		coleVariant.Clear();
		return;
		break;
	}
}

OgdcBool OgdcHelperMdb::IsKeyWord( const OgdcString& strName )
{
	OgdcString strKey = strName;
	strKey.MakeUpper();
	OgdcString strValue;
	if (!m_bInited)
	{
		InitKeyWords();
	}
	if (g_keywords.Lookup(strKey,strValue))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

void OgdcHelperMdb::InitKeyWords()
{
	g_keywords.RemoveAll();
	g_keywords.SetAt(_U("ADD"),_U("ADD"));
	g_keywords.SetAt(_U("ALL"),_U("ALL"));
	g_keywords.SetAt(_U("ALPHANUMERIC"),_U("ALPHANUMERIC"));
	g_keywords.SetAt(_U("ALTER"),_U("ALTER"));
	g_keywords.SetAt(_U("AND"),_U("AND"));
	g_keywords.SetAt(_U("ANY"),_U("ANY"));
	g_keywords.SetAt(_U("APPLICATION"),_U("APPLICATION"));
	g_keywords.SetAt(_U("AS"),_U("AS"));
	g_keywords.SetAt(_U("ASC"),_U("ASC"));
	g_keywords.SetAt(_U("ASSISTANT"),_U("ASSISTANT"));
	g_keywords.SetAt(_U("AT"),_U("AT"));
	g_keywords.SetAt(_U("AUTOINCREMENT"),_U("AUTOINCREMENT"));
	g_keywords.SetAt(_U("AVG"),_U("AVG"));
	g_keywords.SetAt(_U("BETWEEN"),_U("BETWEEN"));
	g_keywords.SetAt(_U("BINARY"),_U("BINARY"));
	g_keywords.SetAt(_U("BIT"),_U("BIT"));
	g_keywords.SetAt(_U("BOOLEAN"),_U("BOOLEAN"));
	g_keywords.SetAt(_U("BY"),_U("BY"));
	g_keywords.SetAt(_U("BYTE"),_U("BYTE"));
	g_keywords.SetAt(_U("CHAR"),_U("CHAR"));
	g_keywords.SetAt(_U("CHARACTER"),_U("CHARACTER"));
	g_keywords.SetAt(_U("COLUMN"),_U("COLUMN"));
	g_keywords.SetAt(_U("COMPACTDATABASE"),_U("COMPACTDATABASE"));
	g_keywords.SetAt(_U("CONSTRAINT"),_U("CONSTRAINT"));
	g_keywords.SetAt(_U("CONTAINER"),_U("CONTAINER"));
	g_keywords.SetAt(_U("COUNT"),_U("COUNT"));
	g_keywords.SetAt(_U("COUNTER"),_U("COUNTER"));
	g_keywords.SetAt(_U("CREATE"),_U("CREATE"));
	g_keywords.SetAt(_U("CREATEDATABASE"),_U("CREATEDATABASE"));
	g_keywords.SetAt(_U("CREATEFIELD"),_U("CREATEFIELD"));
	g_keywords.SetAt(_U("CREATEGROUP"),_U("CREATEGROUP"));
	g_keywords.SetAt(_U("CREATEINDEX"),_U("CREATEINDEX"));
	g_keywords.SetAt(_U("CREATEOBJECT"),_U("CREATEOBJECT"));
	g_keywords.SetAt(_U("CREATEPROPERTY"),_U("CREATEPROPERTY"));
	g_keywords.SetAt(_U("CREATERELATION"),_U("CREATERELATION"));
	g_keywords.SetAt(_U("CREATETABLEDEF"),_U("CREATETABLEDEF"));
	g_keywords.SetAt(_U("CREATEUSER"),_U("CREATEUSER"));
	g_keywords.SetAt(_U("CREATEWORKSPACE"),_U("CREATEWORKSPACE"));
	g_keywords.SetAt(_U("CURRENCY"),_U("CURRENCY"));
	g_keywords.SetAt(_U("CURRENTUSER"),_U("CURRENTUSER"));
	g_keywords.SetAt(_U("DATABASE"),_U("DATABASE"));
	g_keywords.SetAt(_U("DATE"),_U("DATE"));
	g_keywords.SetAt(_U("DATETIME"),_U("DATETIME"));
	g_keywords.SetAt(_U("DELETE"),_U("DELETE"));
	g_keywords.SetAt(_U("DESC"),_U("DESC"));
	g_keywords.SetAt(_U("DESCRIPTION"),_U("DESCRIPTION"));
	g_keywords.SetAt(_U("DISALLOW"),_U("DISALLOW"));
	g_keywords.SetAt(_U("DISTINCT"),_U("DISTINCT"));
	g_keywords.SetAt(_U("DISTINCTROW"),_U("DISTINCTROW"));
	g_keywords.SetAt(_U("DOCUMENT"),_U("DOCUMENT"));
	g_keywords.SetAt(_U("DOUBLE"),_U("DOUBLE"));
	g_keywords.SetAt(_U("DROP"),_U("DROP"));
	g_keywords.SetAt(_U("ECHO"),_U("ECHO"));
	g_keywords.SetAt(_U("ELSE"),_U("ELSE"));
	g_keywords.SetAt(_U("END"),_U("END"));
	g_keywords.SetAt(_U("EQV"),_U("EQV"));
	g_keywords.SetAt(_U("ERROR"),_U("ERROR"));
	g_keywords.SetAt(_U("EXISTS"),_U("EXISTS"));
	g_keywords.SetAt(_U("EXIT"),_U("EXIT"));
	g_keywords.SetAt(_U("FALSE"),_U("FALSE"));
	g_keywords.SetAt(_U("FIELD"),_U("FIELD"));
	g_keywords.SetAt(_U("FIELDS"),_U("FIELDS"));
	g_keywords.SetAt(_U("FILLCACHE"),_U("FILLCACHE"));
	g_keywords.SetAt(_U("FLOAT"),_U("FLOAT"));
	g_keywords.SetAt(_U("FLOAT4"),_U("FLOAT4"));
	g_keywords.SetAt(_U("FLOAT8"),_U("FLOAT8"));
	g_keywords.SetAt(_U("FOREIGN"),_U("FOREIGN"));
	g_keywords.SetAt(_U("FORM"),_U("FORM"));
	g_keywords.SetAt(_U("FORMS"),_U("FORMS"));
	g_keywords.SetAt(_U("FROM"),_U("FROM"));
	g_keywords.SetAt(_U("FULL"),_U("FULL"));
	g_keywords.SetAt(_U("FUNCTION"),_U("FUNCTION"));
	g_keywords.SetAt(_U("GENERAL"),_U("GENERAL"));
	g_keywords.SetAt(_U("GETOBJECT"),_U("GETOBJECT"));
	g_keywords.SetAt(_U("GETOPTION"),_U("GETOPTION"));
	g_keywords.SetAt(_U("GOTOPAGE"),_U("GOTOPAGE"));
	g_keywords.SetAt(_U("GROUP"),_U("GROUP"));
	g_keywords.SetAt(_U("GROUPBY"),_U("GROUPBY"));
	g_keywords.SetAt(_U("GUID"),_U("GUID"));
	g_keywords.SetAt(_U("HAVING"),_U("HAVING"));
	g_keywords.SetAt(_U("IDLE"),_U("IDLE"));
	g_keywords.SetAt(_U("IEEEDOUBLE"),_U("IEEEDOUBLE"));
	g_keywords.SetAt(_U("IEEESINGLE"),_U("IEEESINGLE"));
	g_keywords.SetAt(_U("IF"),_U("IF"));
	g_keywords.SetAt(_U("IGNORE"),_U("IGNORE"));
	g_keywords.SetAt(_U("IMP"),_U("IMP"));
	g_keywords.SetAt(_U("IN"),_U("IN"));
	g_keywords.SetAt(_U("INDEX"),_U("INDEX"));
	g_keywords.SetAt(_U("INDEX"),_U("INDEX"));
	g_keywords.SetAt(_U("INDEXES"),_U("INDEXES"));
	g_keywords.SetAt(_U("INNER"),_U("INNER"));
	g_keywords.SetAt(_U("INSERT"),_U("INSERT"));
	g_keywords.SetAt(_U("INSERTTEXT"),_U("INSERTTEXT"));
	g_keywords.SetAt(_U("INT"),_U("INT"));
	g_keywords.SetAt(_U("INTEGER"),_U("INTEGER"));
	g_keywords.SetAt(_U("INTEGER1"),_U("INTEGER1"));
	g_keywords.SetAt(_U("INTEGER2"),_U("INTEGER2"));
	g_keywords.SetAt(_U("INTEGER4"),_U("INTEGER4"));
	g_keywords.SetAt(_U("INTO"),_U("INTO"));
	g_keywords.SetAt(_U("IS"),_U("IS"));
	g_keywords.SetAt(_U("JOIN"),_U("JOIN"));
	g_keywords.SetAt(_U("KEY"),_U("KEY"));
	g_keywords.SetAt(_U("LASTMODIFIED"),_U("LASTMODIFIED"));
	g_keywords.SetAt(_U("LEFT"),_U("LEFT"));
	g_keywords.SetAt(_U("LEVEL"),_U("LEVEL"));
	g_keywords.SetAt(_U("LIKE"),_U("LIKE"));
	g_keywords.SetAt(_U("LOGICAL"),_U("LOGICAL"));
	g_keywords.SetAt(_U("LOGICAL1"),_U("LOGICAL1"));
	g_keywords.SetAt(_U("LONG"),_U("LONG"));
	g_keywords.SetAt(_U("LONGBINARY"),_U("LONGBINARY"));
	g_keywords.SetAt(_U("LONGTEXT"),_U("LONGTEXT"));
	g_keywords.SetAt(_U("MACRO"),_U("MACRO"));
	g_keywords.SetAt(_U("MATCH"),_U("MATCH"));
	g_keywords.SetAt(_U("MAX"),_U("MAX"));
	g_keywords.SetAt(_U("MIN"),_U("MIN"));
	g_keywords.SetAt(_U("MOD"),_U("MOD"));
	g_keywords.SetAt(_U("MEMO"),_U("MEMO"));
	g_keywords.SetAt(_U("MODULE"),_U("MODULE"));
	g_keywords.SetAt(_U("MONEY"),_U("MONEY"));
	g_keywords.SetAt(_U("MOVE"),_U("MOVE"));
//	g_keywords.SetAt(_U("NAME"),_U("NAME"));
	g_keywords.SetAt(_U("NEWPASSWORD"),_U("NEWPASSWORD"));
	g_keywords.SetAt(_U("NO"),_U("NO"));
	g_keywords.SetAt(_U("NOT"),_U("NOT"));
	g_keywords.SetAt(_U("NOTE"),_U("NOTE"));
	g_keywords.SetAt(_U("NULL"),_U("NULL"));
	g_keywords.SetAt(_U("NUMBER"),_U("NUMBER"));
	g_keywords.SetAt(_U("NUMERIC"),_U("NUMERIC"));
	g_keywords.SetAt(_U("OBJECT"),_U("OBJECT"));
	g_keywords.SetAt(_U("OLEOBJECT"),_U("OLEOBJECT"));
	g_keywords.SetAt(_U("OFF"),_U("OFF"));
	g_keywords.SetAt(_U("ON"),_U("ON"));
	g_keywords.SetAt(_U("OPENRECORDSET"),_U("OPENRECORDSET"));
	g_keywords.SetAt(_U("OPTION"),_U("OPTION"));
	g_keywords.SetAt(_U("OR"),_U("OR"));
	g_keywords.SetAt(_U("ORDER"),_U("ORDER"));
	g_keywords.SetAt(_U("ORIENTATION"),_U("ORIENTATION"));
	g_keywords.SetAt(_U("OUTER"),_U("OUTER"));
	g_keywords.SetAt(_U("OWNERACCESS"),_U("OWNERACCESS"));
	g_keywords.SetAt(_U("PARAMETER"),_U("PARAMETER"));
	g_keywords.SetAt(_U("PARAMETERS"),_U("PARAMETERS"));
	g_keywords.SetAt(_U("PARTIAL"),_U("PARTIAL"));
	g_keywords.SetAt(_U("PASSWORD"),_U("PASSWORD"));
	g_keywords.SetAt(_U("PERCENT"),_U("PERCENT"));
	g_keywords.SetAt(_U("PIVOT"),_U("PIVOT"));
	g_keywords.SetAt(_U("PRIMARY"),_U("PRIMARY"));
	g_keywords.SetAt(_U("PROCEDURE"),_U("PROCEDURE"));
	g_keywords.SetAt(_U("PROPERTY"),_U("PROPERTY"));
	g_keywords.SetAt(_U("QUERIES"),_U("QUERIES"));
	g_keywords.SetAt(_U("QUERY"),_U("QUERY"));
	g_keywords.SetAt(_U("QUIT"),_U("QUIT"));
	g_keywords.SetAt(_U("REAL"),_U("REAL"));
	g_keywords.SetAt(_U("RECALC"),_U("RECALC"));
	g_keywords.SetAt(_U("RECORDSET"),_U("RECORDSET"));
	g_keywords.SetAt(_U("REFERENCES"),_U("REFERENCES"));
	g_keywords.SetAt(_U("REFRESH"),_U("REFRESH"));
	g_keywords.SetAt(_U("REFRESHLINK"),_U("REFRESHLINK"));
	g_keywords.SetAt(_U("REGISTERDATABASE"),_U("REGISTERDATABASE"));
	g_keywords.SetAt(_U("RELATION"),_U("RELATION"));
	g_keywords.SetAt(_U("REPAINT"),_U("REPAINT"));
	g_keywords.SetAt(_U("REPAIRDATABASE"),_U("REPAIRDATABASE"));
	g_keywords.SetAt(_U("REPORT"),_U("REPORT"));
	g_keywords.SetAt(_U("REPORTS"),_U("REPORTS"));
	g_keywords.SetAt(_U("REQUERY"),_U("REQUERY"));
	g_keywords.SetAt(_U("RIGHT"),_U("RIGHT"));
	g_keywords.SetAt(_U("SCREEN"),_U("SCREEN"));
	g_keywords.SetAt(_U("SECTION"),_U("SECTION"));
	g_keywords.SetAt(_U("SELECT"),_U("SELECT"));
	g_keywords.SetAt(_U("SET"),_U("SET"));
	g_keywords.SetAt(_U("SETFOCUS"),_U("SETFOCUS"));
	g_keywords.SetAt(_U("SETOPTION"),_U("SETOPTION"));
	g_keywords.SetAt(_U("SHORT"),_U("SHORT"));
	g_keywords.SetAt(_U("SINGLE"),_U("SINGLE"));
	g_keywords.SetAt(_U("SIZE"),_U("SIZE"));
	g_keywords.SetAt(_U("SMALLINT"),_U("SMALLINT"));
	g_keywords.SetAt(_U("SOME"),_U("SOME"));
	g_keywords.SetAt(_U("SQL"),_U("SQL"));
	g_keywords.SetAt(_U("STDEV"),_U("STDEV"));
	g_keywords.SetAt(_U("STDEVP"),_U("STDEVP"));
	g_keywords.SetAt(_U("STRING"),_U("STRING"));
	g_keywords.SetAt(_U("SUM"),_U("SUM"));
	g_keywords.SetAt(_U("TABLE"),_U("TABLE"));
	g_keywords.SetAt(_U("TABLEDEF"),_U("TABLEDEF"));
	g_keywords.SetAt(_U("TABLEDEFS"),_U("TABLEDEFS"));
	g_keywords.SetAt(_U("TABLEID"),_U("TABLEID"));
	g_keywords.SetAt(_U("TEXT"),_U("TEXT"));
	g_keywords.SetAt(_U("TIME"),_U("TIME"));
	g_keywords.SetAt(_U("TIMESTAMP"),_U("TIMESTAMP"));
	g_keywords.SetAt(_U("TOP"),_U("TOP"));
	g_keywords.SetAt(_U("TRANSFORM"),_U("TRANSFORM"));
	g_keywords.SetAt(_U("TRUE"),_U("TRUE"));
	g_keywords.SetAt(_U("TYPE"),_U("TYPE"));
	g_keywords.SetAt(_U("UNION"),_U("UNION"));
	g_keywords.SetAt(_U("UNIQUE"),_U("UNIQUE"));
	g_keywords.SetAt(_U("UPDATE"),_U("UPDATE"));
	g_keywords.SetAt(_U("USER"),_U("USER"));
	g_keywords.SetAt(_U("VALUE"),_U("VALUE"));
	g_keywords.SetAt(_U("VALUES"),_U("VALUES"));
	g_keywords.SetAt(_U("VAR"),_U("VAR"));
	g_keywords.SetAt(_U("VARP"),_U("VARP"));
	g_keywords.SetAt(_U("VARBINARY"),_U("VARBINARY"));
	g_keywords.SetAt(_U("VARCHAR"),_U("VARCHAR"));
	g_keywords.SetAt(_U("WHERE"),_U("WHERE"));
	g_keywords.SetAt(_U("WITH"),_U("WITH"));
	g_keywords.SetAt(_U("WORKSPACE"),_U("WORKSPACE"));
	g_keywords.SetAt(_U("XOR"),_U("XOR"));
	g_keywords.SetAt(_U("YEAR"),_U("YEAR"));
	g_keywords.SetAt(_U("YES"),_U("YES"));
	g_keywords.SetAt(_U("YESNO"),_U("YESNO"));

	m_bInited = TRUE;
}

}