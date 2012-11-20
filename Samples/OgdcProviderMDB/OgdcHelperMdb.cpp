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
	fieldInfo.m_strName			  = daoInfo.m_strName.GetBuffer(
		daoInfo.m_strName.GetLength()); 
	fieldInfo.m_nType			  = (OgdcFieldInfo::FieldType)daoInfo.m_nType;
	fieldInfo.m_nSize			  = daoInfo.m_lSize;                  
	fieldInfo.m_nAttributes		  = daoInfo.m_lAttributes;            
	fieldInfo.m_nOrdinalPosition  = daoInfo.m_nOrdinalPosition;      
	fieldInfo.m_bRequired		  = daoInfo.m_bRequired;              
	fieldInfo.m_bAllowZeroLength  = daoInfo.m_bAllowZeroLength;       
	fieldInfo.m_nCollatingOrder	  = daoInfo.m_lCollatingOrder;        
	fieldInfo.m_strForeignName	  = (LPCTSTR)daoInfo.m_strForeignName;      
	fieldInfo.m_strSourceField	  = (LPCTSTR)daoInfo.m_strSourceField;      
	fieldInfo.m_strSourceTable	  = (LPCTSTR)daoInfo.m_strSourceTable;      
	fieldInfo.m_strValidationRule = (LPCTSTR)daoInfo.m_strValidationRule;   
	fieldInfo.m_strValidationText = (LPCTSTR)daoInfo.m_strValidationText;   	
	fieldInfo.m_bSystem           = IsSystemField(fieldInfo.m_strName);	
	fieldInfo.SetDefaultValue((LPCTSTR)daoInfo.m_strDefaultValue);  
}

OgdcBool OgdcHelperMdb::IsGeoField(OgdcString strName)
{
	strName.MakeUpper();
	if(strName.Find("OGGEOMETRY") == 0 ||
		strName.Find(".OGGEOMETRY") > 0 ||
		strName.Find("OGX") == 0 ||
		strName.Find(".OGX") > 0 ||
		strName.Find("OGY") == 0 ||
		strName.Find(".OGY") > 0 )
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
	if(strName.Left(2).CompareNoCase("OG")==0)
	{
		if(strName.CompareNoCase("OGUserID")==0)
		{
			return FALSE;
		}
		else if ( strName.CompareNoCase("OGID") == 0 )
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
	if (strCheckName.Left(2).CompareNoCase(OGDCPCTSTR("OG")) == 0) 
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
			OgdcString str = OgdcString(OGDCPCTSTR(coleVariant.bstrVal));
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
	OgdcString strValue = "";
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
	g_keywords.SetAt("ADD","ADD");
	g_keywords.SetAt("ALL","ALL");
	g_keywords.SetAt("ALPHANUMERIC","ALPHANUMERIC");
	g_keywords.SetAt("ALTER","ALTER");
	g_keywords.SetAt("AND","AND");
	g_keywords.SetAt("ANY","ANY");
	g_keywords.SetAt("APPLICATION","APPLICATION");
	g_keywords.SetAt("AS","AS");
	g_keywords.SetAt("ASC","ASC");
	g_keywords.SetAt("ASSISTANT","ASSISTANT");
	g_keywords.SetAt("AT","AT");
	g_keywords.SetAt("AUTOINCREMENT","AUTOINCREMENT");
	g_keywords.SetAt("AVG","AVG");
	g_keywords.SetAt("BETWEEN","BETWEEN");
	g_keywords.SetAt("BINARY","BINARY");
	g_keywords.SetAt("BIT","BIT");
	g_keywords.SetAt("BOOLEAN","BOOLEAN");
	g_keywords.SetAt("BY","BY");
	g_keywords.SetAt("BYTE","BYTE");
	g_keywords.SetAt("CHAR","CHAR");
	g_keywords.SetAt("CHARACTER","CHARACTER");
	g_keywords.SetAt("COLUMN","COLUMN");
	g_keywords.SetAt("COMPACTDATABASE","COMPACTDATABASE");
	g_keywords.SetAt("CONSTRAINT","CONSTRAINT");
	g_keywords.SetAt("CONTAINER","CONTAINER");
	g_keywords.SetAt("COUNT","COUNT");
	g_keywords.SetAt("COUNTER","COUNTER");
	g_keywords.SetAt("CREATE","CREATE");
	g_keywords.SetAt("CREATEDATABASE","CREATEDATABASE");
	g_keywords.SetAt("CREATEFIELD","CREATEFIELD");
	g_keywords.SetAt("CREATEGROUP","CREATEGROUP");
	g_keywords.SetAt("CREATEINDEX","CREATEINDEX");
	g_keywords.SetAt("CREATEOBJECT","CREATEOBJECT");
	g_keywords.SetAt("CREATEPROPERTY","CREATEPROPERTY");
	g_keywords.SetAt("CREATERELATION","CREATERELATION");
	g_keywords.SetAt("CREATETABLEDEF","CREATETABLEDEF");
	g_keywords.SetAt("CREATEUSER","CREATEUSER");
	g_keywords.SetAt("CREATEWORKSPACE","CREATEWORKSPACE");
	g_keywords.SetAt("CURRENCY","CURRENCY");
	g_keywords.SetAt("CURRENTUSER","CURRENTUSER");
	g_keywords.SetAt("DATABASE","DATABASE");
	g_keywords.SetAt("DATE","DATE");
	g_keywords.SetAt("DATETIME","DATETIME");
	g_keywords.SetAt("DELETE","DELETE");
	g_keywords.SetAt("DESC","DESC");
	g_keywords.SetAt("DESCRIPTION","DESCRIPTION");
	g_keywords.SetAt("DISALLOW","DISALLOW");
	g_keywords.SetAt("DISTINCT","DISTINCT");
	g_keywords.SetAt("DISTINCTROW","DISTINCTROW");
	g_keywords.SetAt("DOCUMENT","DOCUMENT");
	g_keywords.SetAt("DOUBLE","DOUBLE");
	g_keywords.SetAt("DROP","DROP");
	g_keywords.SetAt("ECHO","ECHO");
	g_keywords.SetAt("ELSE","ELSE");
	g_keywords.SetAt("END","END");
	g_keywords.SetAt("EQV","EQV");
	g_keywords.SetAt("ERROR","ERROR");
	g_keywords.SetAt("EXISTS","EXISTS");
	g_keywords.SetAt("EXIT","EXIT");
	g_keywords.SetAt("FALSE","FALSE");
	g_keywords.SetAt("FIELD","FIELD");
	g_keywords.SetAt("FIELDS","FIELDS");
	g_keywords.SetAt("FILLCACHE","FILLCACHE");
	g_keywords.SetAt("FLOAT","FLOAT");
	g_keywords.SetAt("FLOAT4","FLOAT4");
	g_keywords.SetAt("FLOAT8","FLOAT8");
	g_keywords.SetAt("FOREIGN","FOREIGN");
	g_keywords.SetAt("FORM","FORM");
	g_keywords.SetAt("FORMS","FORMS");
	g_keywords.SetAt("FROM","FROM");
	g_keywords.SetAt("FULL","FULL");
	g_keywords.SetAt("FUNCTION","FUNCTION");
	g_keywords.SetAt("GENERAL","GENERAL");
	g_keywords.SetAt("GETOBJECT","GETOBJECT");
	g_keywords.SetAt("GETOPTION","GETOPTION");
	g_keywords.SetAt("GOTOPAGE","GOTOPAGE");
	g_keywords.SetAt("GROUP","GROUP");
	g_keywords.SetAt("GROUPBY","GROUPBY");
	g_keywords.SetAt("GUID","GUID");
	g_keywords.SetAt("HAVING","HAVING");
	g_keywords.SetAt("IDLE","IDLE");
	g_keywords.SetAt("IEEEDOUBLE","IEEEDOUBLE");
	g_keywords.SetAt("IEEESINGLE","IEEESINGLE");
	g_keywords.SetAt("IF","IF");
	g_keywords.SetAt("IGNORE","IGNORE");
	g_keywords.SetAt("IMP","IMP");
	g_keywords.SetAt("IN","IN");
	g_keywords.SetAt("INDEX","INDEX");
	g_keywords.SetAt("INDEX","INDEX");
	g_keywords.SetAt("INDEXES","INDEXES");
	g_keywords.SetAt("INNER","INNER");
	g_keywords.SetAt("INSERT","INSERT");
	g_keywords.SetAt("INSERTTEXT","INSERTTEXT");
	g_keywords.SetAt("INT","INT");
	g_keywords.SetAt("INTEGER","INTEGER");
	g_keywords.SetAt("INTEGER1","INTEGER1");
	g_keywords.SetAt("INTEGER2","INTEGER2");
	g_keywords.SetAt("INTEGER4","INTEGER4");
	g_keywords.SetAt("INTO","INTO");
	g_keywords.SetAt("IS","IS");
	g_keywords.SetAt("JOIN","JOIN");
	g_keywords.SetAt("KEY","KEY");
	g_keywords.SetAt("LASTMODIFIED","LASTMODIFIED");
	g_keywords.SetAt("LEFT","LEFT");
	g_keywords.SetAt("LEVEL","LEVEL");
	g_keywords.SetAt("LIKE","LIKE");
	g_keywords.SetAt("LOGICAL","LOGICAL");
	g_keywords.SetAt("LOGICAL1","LOGICAL1");
	g_keywords.SetAt("LONG","LONG");
	g_keywords.SetAt("LONGBINARY","LONGBINARY");
	g_keywords.SetAt("LONGTEXT","LONGTEXT");
	g_keywords.SetAt("MACRO","MACRO");
	g_keywords.SetAt("MATCH","MATCH");
	g_keywords.SetAt("MAX","MAX");
	g_keywords.SetAt("MIN","MIN");
	g_keywords.SetAt("MOD","MOD");
	g_keywords.SetAt("MEMO","MEMO");
	g_keywords.SetAt("MODULE","MODULE");
	g_keywords.SetAt("MONEY","MONEY");
	g_keywords.SetAt("MOVE","MOVE");
//	g_keywords.SetAt("NAME","NAME");
	g_keywords.SetAt("NEWPASSWORD","NEWPASSWORD");
	g_keywords.SetAt("NO","NO");
	g_keywords.SetAt("NOT","NOT");
	g_keywords.SetAt("NOTE","NOTE");
	g_keywords.SetAt("NULL","NULL");
	g_keywords.SetAt("NUMBER","NUMBER");
	g_keywords.SetAt("NUMERIC","NUMERIC");
	g_keywords.SetAt("OBJECT","OBJECT");
	g_keywords.SetAt("OLEOBJECT","OLEOBJECT");
	g_keywords.SetAt("OFF","OFF");
	g_keywords.SetAt("ON","ON");
	g_keywords.SetAt("OPENRECORDSET","OPENRECORDSET");
	g_keywords.SetAt("OPTION","OPTION");
	g_keywords.SetAt("OR","OR");
	g_keywords.SetAt("ORDER","ORDER");
	g_keywords.SetAt("ORIENTATION","ORIENTATION");
	g_keywords.SetAt("OUTER","OUTER");
	g_keywords.SetAt("OWNERACCESS","OWNERACCESS");
	g_keywords.SetAt("PARAMETER","PARAMETER");
	g_keywords.SetAt("PARAMETERS","PARAMETERS");
	g_keywords.SetAt("PARTIAL","PARTIAL");
	g_keywords.SetAt("PASSWORD","PASSWORD");
	g_keywords.SetAt("PERCENT","PERCENT");
	g_keywords.SetAt("PIVOT","PIVOT");
	g_keywords.SetAt("PRIMARY","PRIMARY");
	g_keywords.SetAt("PROCEDURE","PROCEDURE");
	g_keywords.SetAt("PROPERTY","PROPERTY");
	g_keywords.SetAt("QUERIES","QUERIES");
	g_keywords.SetAt("QUERY","QUERY");
	g_keywords.SetAt("QUIT","QUIT");
	g_keywords.SetAt("REAL","REAL");
	g_keywords.SetAt("RECALC","RECALC");
	g_keywords.SetAt("RECORDSET","RECORDSET");
	g_keywords.SetAt("REFERENCES","REFERENCES");
	g_keywords.SetAt("REFRESH","REFRESH");
	g_keywords.SetAt("REFRESHLINK","REFRESHLINK");
	g_keywords.SetAt("REGISTERDATABASE","REGISTERDATABASE");
	g_keywords.SetAt("RELATION","RELATION");
	g_keywords.SetAt("REPAINT","REPAINT");
	g_keywords.SetAt("REPAIRDATABASE","REPAIRDATABASE");
	g_keywords.SetAt("REPORT","REPORT");
	g_keywords.SetAt("REPORTS","REPORTS");
	g_keywords.SetAt("REQUERY","REQUERY");
	g_keywords.SetAt("RIGHT","RIGHT");
	g_keywords.SetAt("SCREEN","SCREEN");
	g_keywords.SetAt("SECTION","SECTION");
	g_keywords.SetAt("SELECT","SELECT");
	g_keywords.SetAt("SET","SET");
	g_keywords.SetAt("SETFOCUS","SETFOCUS");
	g_keywords.SetAt("SETOPTION","SETOPTION");
	g_keywords.SetAt("SHORT","SHORT");
	g_keywords.SetAt("SINGLE","SINGLE");
	g_keywords.SetAt("SIZE","SIZE");
	g_keywords.SetAt("SMALLINT","SMALLINT");
	g_keywords.SetAt("SOME","SOME");
	g_keywords.SetAt("SQL","SQL");
	g_keywords.SetAt("STDEV","STDEV");
	g_keywords.SetAt("STDEVP","STDEVP");
	g_keywords.SetAt("STRING","STRING");
	g_keywords.SetAt("SUM","SUM");
	g_keywords.SetAt("TABLE","TABLE");
	g_keywords.SetAt("TABLEDEF","TABLEDEF");
	g_keywords.SetAt("TABLEDEFS","TABLEDEFS");
	g_keywords.SetAt("TABLEID","TABLEID");
	g_keywords.SetAt("TEXT","TEXT");
	g_keywords.SetAt("TIME","TIME");
	g_keywords.SetAt("TIMESTAMP","TIMESTAMP");
	g_keywords.SetAt("TOP","TOP");
	g_keywords.SetAt("TRANSFORM","TRANSFORM");
	g_keywords.SetAt("TRUE","TRUE");
	g_keywords.SetAt("TYPE","TYPE");
	g_keywords.SetAt("UNION","UNION");
	g_keywords.SetAt("UNIQUE","UNIQUE");
	g_keywords.SetAt("UPDATE","UPDATE");
	g_keywords.SetAt("USER","USER");
	g_keywords.SetAt("VALUE","VALUE");
	g_keywords.SetAt("VALUES","VALUES");
	g_keywords.SetAt("VAR","VAR");
	g_keywords.SetAt("VARP","VARP");
	g_keywords.SetAt("VARBINARY","VARBINARY");
	g_keywords.SetAt("VARCHAR","VARCHAR");
	g_keywords.SetAt("WHERE","WHERE");
	g_keywords.SetAt("WITH","WITH");
	g_keywords.SetAt("WORKSPACE","WORKSPACE");
	g_keywords.SetAt("XOR","XOR");
	g_keywords.SetAt("YEAR","YEAR");
	g_keywords.SetAt("YES","YES");
	g_keywords.SetAt("YESNO","YESNO");

	m_bInited = TRUE;
}

}