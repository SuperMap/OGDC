//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)             
//                                                           
//!  \file OgdcDatasetVectorMdb.cpp
//!  \brief 矢量数据集的接口实现
//!  \details 矢量数据集各种接口的实现
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Limited Company.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#include "OgdcDatasetVectorMdb.h"
#include "OgdcHelperMdb.h"
#include "OgdcMacroDefineMdb.h"
#include "OgdcRecordsetMdb.h"
#include "OgdcFeatureMdb.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace OGDC
{

OgdcDatasetVectorMdb::OgdcDatasetVectorMdb()
{
	m_nObjectCount = 0;
	m_nID = 0;
	m_bModified = FALSE;
	m_pDataSource = NULL;
	m_pDaoTableDef = NULL;
	m_bOpened = FALSE;
	m_nRecordCount = 0;
}

OgdcDatasetVectorMdb::OgdcDatasetVectorMdb( OgdcDataSourceMdb *pDataSource )
{
	m_nObjectCount = 0;
	m_nID = 0;
	m_bModified = FALSE;
	m_pDataSource = pDataSource;
	m_pDaoTableDef = NULL;
	m_bOpened = FALSE;
	m_nRecordCount = 0;
}

OgdcDatasetVectorMdb::~OgdcDatasetVectorMdb()
{
	Close();
}

OgdcInt OgdcDatasetVectorMdb::GetObjectCount()
{
	if (m_nObjectCount > 0)
	{
		return m_nObjectCount;
	}
	OgdcString strSQL;
	COleVariant varValue;
	CDaoRecordset recordset(&(m_pDataSource->m_daoDatabase));
	strSQL.Format( OGDCPCTSTR("SELECT OGRecordCount FROM OGRegister "
		"WHERE OGDatasetName = '%s'"), OGDCPCTSTR(m_info.m_strName));
	try
	{
		recordset.Open(dbOpenSnapshot, strSQL);
		recordset.GetFieldValue(0, varValue);
		recordset.Close();
	}
	catch(CDaoException* e)
	{
		e->Delete();
		recordset.Close();
		return -1;
	}

	m_nObjectCount = varValue.lVal;
	return OgdcInt(m_nObjectCount);
}

OgdcInt OgdcDatasetVectorMdb::GetRecordsetCount()
{
	if (m_nRecordCount)
	{
		return OgdcInt(m_nRecordCount);
	}
	OgdcString strSQL;
	COleVariant varValue;
	CDaoRecordset recordset(&(m_pDataSource->m_daoDatabase));
	strSQL.Format( OGDCPCTSTR("SELECT COUNT(*) FROM OGRegister "));
	try
	{
		recordset.Open(dbOpenSnapshot, strSQL);
		recordset.GetFieldValue(0, varValue);
		recordset.Close();
	}
	catch (CDaoException* e)
	{
		e->Delete();
		recordset.Close();
		return -1;
	}
	m_nRecordCount = varValue.lVal;
	return OgdcInt(m_nRecordCount);			
}

OgdcBool OgdcDatasetVectorMdb::ReleaseRecordset( OgdcRecordset *pRecordset )
{
	OgdcInt nCount = m_recordsets.GetSize();
	OgdcInt i = 0;
	//看看m_pRecordsets数组里是否有该指针，有就删除
	for (i = 0; i < nCount; i++)
	{
		if (pRecordset == m_recordsets.GetAt(i))
		{
			if (((OgdcRecordsetMdb*)pRecordset)->m_pDaoRecordset != NULL)
			{
				((OgdcRecordsetMdb*)pRecordset)->m_pDaoRecordset->Close();
				delete ((OgdcRecordsetMdb*)pRecordset)->m_pDaoRecordset;
				((OgdcRecordsetMdb*)pRecordset)->m_pDaoRecordset = NULL;
			}
			pRecordset->Close();
			delete pRecordset;
			pRecordset = NULL;
			m_recordsets.RemoveAt(i,1);
			return TRUE;
		}
	}
	//直接删除
	if (((OgdcRecordsetMdb*)pRecordset)->m_pDaoRecordset != NULL)
	{
		((OgdcRecordsetMdb*)pRecordset)->m_pDaoRecordset->Close();
		delete ((OgdcRecordsetMdb*)pRecordset)->m_pDaoRecordset;
		((OgdcRecordsetMdb*)pRecordset)->m_pDaoRecordset = NULL;
	}
	pRecordset->Close();
	delete pRecordset;
	pRecordset = NULL;
	return TRUE;
}

void OgdcDatasetVectorMdb::ReleaseAllRecordsets()
{
	OgdcLong nCount = m_recordsets.GetSize();
	OgdcInt i = 0;
	for (i = 0; i < nCount; i++)
	{
		delete m_recordsets.GetAt(i);
	}
	m_recordsets.RemoveAll();
}

OgdcFeature* OgdcDatasetVectorMdb::GetFeature( OgdcInt nID )
{
	OgdcFeatureMdb* pFeature = NULL;
	OgdcQueryDef queryDef;
	queryDef.m_nType = OgdcQueryDef::IDs;
	queryDef.m_IDs.Add(nID);
	queryDef.m_nOptions = OgdcQueryDef::Both;
	OgdcRecordsetMdb *pRecordset = (OgdcRecordsetMdb*)Query(queryDef);
	if (pRecordset == NULL)
	{
		return FALSE;
	}

	if(pRecordset->SeekID(nID))
	{
		pFeature = (OgdcFeatureMdb*)pRecordset->GetFeature();
	}

	ReleaseRecordset(pRecordset);
	pRecordset = NULL;
	return pFeature;
}

OgdcBool OgdcDatasetVectorMdb::UpdateFeature( const OgdcFeature* pFeature )
{
	if (pFeature == NULL)
	{
		return FALSE;
	}
	
	OgdcBool bResult = FALSE;
	OgdcFeatureMdb *pFeatureTemp = (OgdcFeatureMdb*)pFeature;
	OgdcInt nID = pFeatureTemp->GetID();
	OgdcQueryDef queryDef;
	queryDef.m_nCursorType = OgdcQueryDef::OpenDynamic;
	queryDef.m_IDs.Add(nID);
	queryDef.m_nType = OgdcQueryDef::IDs;
	
	if (pFeatureTemp->GetElement() == NULL)
	{
		queryDef.m_nOptions = OgdcQueryDef::Attribute;
	}
	else
	{
		queryDef.m_nOptions = OgdcQueryDef::Both;
	}
	
	OgdcRecordsetMdb *pRecordset = (OgdcRecordsetMdb*)Query(queryDef);
	if (pRecordset == NULL)
	{
		return FALSE;
	}
	
	if(pRecordset->SeekID(nID))
	{
		bResult = pRecordset->UpdateFeature(pFeature);
	}

	pFeatureTemp = NULL;
	ReleaseRecordset(pRecordset);
 	pRecordset = NULL;
	return bResult;
}

OgdcBool OgdcDatasetVectorMdb::AddFeature( const OgdcFeature* pFeature )
{
	if(pFeature == NULL)
	{
		return FALSE;
	}
	
	if (!IsOpen())
	{
		Open();
	}

	OgdcBool bResult = FALSE;
	OgdcFeatureMdb *pFeatureTemp = (OgdcFeatureMdb*)pFeature;

	OgdcQueryDef queryDef;
	queryDef.m_nType = OgdcQueryDef::General;
	queryDef.m_strFilter = "";
	queryDef.m_nOptions = OgdcQueryDef::Both;

	OgdcRecordsetMdb *pRecordset = (OgdcRecordsetMdb*)Query(queryDef);
	bResult = pRecordset->AddFeature(pFeatureTemp);
// 	if (bResult)
// 		this->m_nObjectCount++;
	pFeatureTemp = NULL;
	ReleaseRecordset(pRecordset);
	pRecordset = NULL;

	return bResult;
}

OgdcBool OgdcDatasetVectorMdb::DeleteFeature( OgdcInt nID )
{
	OgdcBool bResult = FALSE;
	OgdcQueryDef queryDef;
	queryDef.m_nCursorType = OgdcQueryDef::OpenDynamic;
	queryDef.m_IDs.Add(nID);
	queryDef.m_nType = OgdcQueryDef::IDs;
	if (GetType() != Tabular)
	{
		queryDef.m_nOptions = OgdcQueryDef::Both;
	}
	else
	{
		queryDef.m_nOptions = OgdcQueryDef::Attribute;
	}
	
	OgdcRecordsetMdb *pRecordset = (OgdcRecordsetMdb*)Query(queryDef);
	if (pRecordset == NULL)
	{
		return FALSE;
	}
	if (pRecordset->SeekID(nID))
	{
		bResult = pRecordset->Delete();
	}
	ReleaseRecordset(pRecordset);
	pRecordset = NULL;
	return bResult;	
}

OgdcBool OgdcDatasetVectorMdb::DeleteFeatures( const OgdcInt* pIDs, OgdcInt nCount )
{
	OgdcBool bResult = FALSE;
	for (OgdcInt i = 0; i < nCount; i++)
	{
		if (!DeleteFeature(*pIDs))
		{
			return FALSE;
		}
		pIDs++;
	}

	return TRUE;
}

OgdcBool OgdcDatasetVectorMdb::Truncate()
{
	if (!IsOpen()) 
	{
		return FALSE;
	}
	//删除数据
	OgdcString strSQL;
	strSQL.Format(OGDCPCTSTR("DELETE * FROM [%s]"), OGDCPCTSTR(m_info.m_strName));
	try
	{
		m_pDataSource->m_daoDatabase.Execute(strSQL);
		strSQL.Format(OGDCPCTSTR("UPDATE OGRegister SET OGRecordCount = 0 "
			"WHERE OGDatasetName = '%s'"), OGDCPCTSTR(m_info.m_strName),
			GetName());
		m_pDataSource->m_daoDatabase.Execute(strSQL);
	}
	catch (CDaoException* e)
	{
		e->Delete();
		return FALSE;
	}
	m_nObjectCount = 0;
	return TRUE;
}

OgdcBool OgdcDatasetVectorMdb::BuildSpatialIndex()
{
	//接口暂未实现...
	m_info.m_nSpatialIndexType = 2;
	return TRUE;
}

OgdcBool OgdcDatasetVectorMdb::DropSpatialIndex()
{
	//接口暂未实现...
	m_info.m_nSpatialIndexType = 1;
	return TRUE;
}

OgdcBool OgdcDatasetVectorMdb::IsSpatialIndexDirty()
{
	return FALSE;	
}

OgdcRecordset *OgdcDatasetVectorMdb::Query( const OgdcQueryDef& querydef )
{
	if (!IsOpen())
	{
		return NULL;
	}

 	OgdcRecordsetMdb *pRecordset = new OgdcRecordsetMdb(this);
	pRecordset->m_pDaoRecordset = new CDaoRecordset(&m_pDataSource->m_daoDatabase);
	
	// {{ 构建查询字段
	OgdcString strField = _U("");
	OgdcBool bStat = FALSE;
	OgdcString subStr = "";
	OgdcString strFieldName = "";
	OgdcInt i = 0;
	if (querydef.m_Fields.GetSize() > 0)
	{
		OgdcString strSort, strGroup;
		strSort = querydef.m_strSort;
		strGroup = querydef.m_strGroup;
		strSort.MakeUpper();

		for(i=0;i<querydef.m_Fields.GetSize();i++)
		{
			strFieldName = querydef.m_Fields[i];
			strFieldName.MakeUpper();

			if(	strFieldName.Find("MIN(")>-1 || 
				strFieldName.Find("MAX(")>-1 || 
				strFieldName.Find("SUM(")>-1 || 
				strFieldName.Find("AVG(")>-1 ||
				strFieldName.Find("COUNT(")> -1 ||
				strFieldName.Find("STDDEV(")>-1 ||
				strFieldName.Find("VARIANCE(")>-1 ||
				strFieldName.Find("DISTINCT ")>-1)
			{
				bStat = TRUE;
				break;
			}
		}

		if (bStat || strSort.Find("GROUP BY")>-1 || !strGroup.IsEmpty())
		{
			for(i=0;i<querydef.m_Fields.GetSize();i++)
			{
				subStr.Format(OGDCPCTSTR("%s,"), querydef.m_Fields.GetAt(i).Cstr());
				strField = strField + subStr;					
			}
		}
		else
		{
			for (i = 0; i < m_fieldInfos.GetSize(); i ++)
			{
				subStr.Format(OGDCPCTSTR("%s.%s,"), m_info.m_strName.Cstr(), 
					m_fieldInfos.GetAt(i).m_strName.Cstr());
				strField = strField + subStr;					
			}

			OgdcString strFieldName = "";
			for (i = 0; i < querydef.m_Fields.GetSize(); i++)
			{
				strFieldName = querydef.m_Fields.GetAt(i);
				if (strField.Find(strFieldName) > -1)
					continue;
				subStr.Format(OGDCPCTSTR("%s,"), strFieldName.Cstr());
				strField = strField + subStr;
			}
		}
		strField.SetAt(strField.GetLength() - 1, '\0');
	}
	// }}

	OgdcVariant varValue;
	OgdcBool bSucceed = FALSE;
	OgdcString strSQL;
	switch (querydef.m_nType)
	{
	case OgdcQueryDef::General:
		{
			if (querydef.m_Fields.GetSize())
			{
// 				OgdcString subStr = "";
// 				for (OgdcInt i = 0; i < querydef.m_Fields.GetSize(); i++)
// 				{
// 					subStr.Format("%s,", querydef.m_Fields.GetAt(i).Cstr());
// 					strField = strField + subStr;
// 				}
// 				strField.SetAt(strField.GetLength() - 1, '\0');
			}
			else
			{
				strField = "*";
			}
			
			if (querydef.m_strFilter.IsEmpty())
			{
				strSQL.Format(OGDCPCTSTR("SELECT %s FROM [%s] %s %s"),
					OGDCPCTSTR(strField),
					OGDCPCTSTR(m_info.m_strName), 
					OGDCPCTSTR(querydef.m_strGroup),
					OGDCPCTSTR(querydef.m_strSort));
			}
			else
			{
				strSQL.Format(OGDCPCTSTR("SELECT %s FROM [%s] WHERE %s %s %s"),
					OGDCPCTSTR(strField),
					OGDCPCTSTR(m_info.m_strName), 
					OGDCPCTSTR(querydef.m_strFilter),
					OGDCPCTSTR(querydef.m_strGroup),
					OGDCPCTSTR(querydef.m_strSort));

				
				pRecordset->m_strFilter.Format(OGDCPCTSTR(" %s"), 
				OGDCPCTSTR(querydef.m_strFilter));
			}
			
			
			
			try
			{
				pRecordset->m_pDaoRecordset->Open(AFX_DAO_USE_DEFAULT_TYPE, strSQL);
				pRecordset->SetRecordCount();
			}
			catch (CDaoException* e)
			{
				e->Delete();
				pRecordset->Close();
				delete pRecordset;
				pRecordset = NULL;
				return FALSE;
			}
			
			if (!pRecordset->m_pDaoRecordset->IsEOF())
			{	
				pRecordset->GetFieldValue(0, varValue);
				pRecordset->SeekID(varValue.ToInt());
			}
			
			m_recordsets.Add(pRecordset);
			return pRecordset;
		}
	
	case OgdcQueryDef::Bounds:
		{
			OgdcRect2D rcBounds = querydef.m_rc2Bounds;
			rcBounds.Normalize();
			
			if (querydef.m_Fields.GetSize())
			{
// 				strField.Format("*,");
// 				OgdcString subStr = "";
// 				for (OgdcInt i = 0; i < querydef.m_Fields.GetSize(); i++)
// 				{
// 					subStr.Format("%s,", querydef.m_Fields.GetAt(i).Cstr());
// 					strField = strField + subStr;
// 				}
// 				strField.SetAt(strField.GetLength() - 1, '\0');
			}
			else
			{
				strField.Format("*");
			}

			if (GetType() == OgdcDataset::Point)
			{
				pRecordset->m_strFilter.Format(OGDCPCTSTR(
					"OGX >= %lf AND OGY <= %lf AND OGX <= %lf AND OGY >= %lf"),
					rcBounds.left, rcBounds.top, rcBounds.right, rcBounds.bottom);
			}
			else
			{
				pRecordset->m_strFilter.Format("OGID NOT IN "
					"( SELECT OGID FROM [%s] "
					"where OGSdriW > %lf or OGSdriN < %lf "
					"or OGSdriE < %lf or OGSdriS > %lf ) ",
					OGDCPCTSTR(m_info.m_strName),rcBounds.right, 
					rcBounds.bottom, rcBounds.left, rcBounds.top);
			}

			if(!querydef.m_strFilter.IsEmpty())
			{
				pRecordset->m_strFilter += " AND " + querydef.m_strFilter;
			}

			strSQL.Format("SELECT %s FROM [%s] WHERE %s",OGDCPCTSTR(strField),
				OGDCPCTSTR(m_info.m_strName),OGDCPCTSTR(pRecordset->m_strFilter));

			try
			{
				pRecordset->m_pDaoRecordset->Open(AFX_DAO_USE_DEFAULT_TYPE, strSQL);
				pRecordset->SetRecordCount();
			}
			catch (CDaoException* e)
			{
				e->Delete();
				pRecordset->Close();
				delete pRecordset;
				pRecordset = NULL;
				return FALSE;
			}
			if (!pRecordset->m_pDaoRecordset->IsEOF())
			{	
				pRecordset->GetFieldValue(0, varValue);
				pRecordset->SeekID(varValue.ToInt());
			}

			m_recordsets.Add(pRecordset);
			return pRecordset;	
		}
	case OgdcQueryDef::IDs:	
		{
			strSQL.Format(OGDCPCTSTR("SELECT * FROM [%s] WHERE "),
				OGDCPCTSTR(m_info.m_strName));

			OgdcString strFilter;
			strFilter.Format(OGDCPCTSTR(" OGID IN ("));
			OgdcString strSQL1;
			for (OgdcInt i = 0; i < querydef.m_IDs.GetSize(); i++)
			{
				strSQL1.Format("%d,", querydef.m_IDs.GetAt(i));
				strFilter = strFilter + strSQL1;
			}
			OgdcChar ch = ')';
			strFilter.SetAt(strFilter.GetLength() - 1, ch);
			strSQL = strSQL + strFilter;
			pRecordset->m_strFilter = strFilter;

			try
			{
				pRecordset->m_pDaoRecordset->Open(AFX_DAO_USE_DEFAULT_TYPE, strSQL);
				pRecordset->SetRecordCount();
			}
			catch (CDaoException* e)
			{
				e->Delete();
				pRecordset->Close();
				delete pRecordset;
				pRecordset = NULL;
				return FALSE;
			}

			if (!pRecordset->m_pDaoRecordset->IsEOF())
			{	
				pRecordset->GetFieldValue(0, varValue);
				pRecordset->SeekID(varValue.ToInt());
			}
			
			m_recordsets.Add(pRecordset);
			return pRecordset;	
		}
	}
		
	return NULL;
}

OgdcBool OgdcDatasetVectorMdb::UpdateField( const OgdcString& strFieldName, 
	const OgdcString& strExpress, const OgdcString& strFilter /*= ""*/ )
{
    if (!IsOpen()) 
	{
		return FALSE;
	}
	OgdcString strSQL;
	if (strFilter.IsEmpty())
	{
		strSQL.Format("UPDATE [%s] SET %s = %s ",
			OGDCPCTSTR(m_info.m_strName), OGDCPCTSTR(strFieldName),
		OGDCPCTSTR(strExpress));
	}
	else
	{
		strSQL.Format("UPDATE [%s] SET %s = %s WHERE %s",
			OGDCPCTSTR(m_info.m_strName), OGDCPCTSTR(strFieldName),
		OGDCPCTSTR(strExpress), OGDCPCTSTR(strFilter));
	}

	try
	{
		m_pDataSource->m_daoDatabase.Execute(strSQL);
	}
	catch (CDaoException* e)
	{
		e->Delete();
		return FALSE;
	}
	
	return TRUE;
}

OgdcBool OgdcDatasetVectorMdb::CopyField( const OgdcString& strSrcFieldName, const OgdcString& strDestFieldName )
{
	if (!IsOpen())
	{
		return FALSE;
	}
	OgdcFieldInfo fieldInfo;
	OgdcFieldInfos fielInfos;

	//如果目标字段不存在就创建一下
	if (!GetFieldInfo(strDestFieldName, fieldInfo))
	{
		if (GetFieldInfo(strSrcFieldName, fieldInfo))
		{
			fieldInfo.m_strName = strDestFieldName;
			fielInfos.Add(fieldInfo);
			if (!CreateFields(fielInfos))
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
	}

	OgdcString strSQL;
	strSQL.Format(OGDCPCTSTR("UPDATE %s SET %s = %s"),
		OGDCPCTSTR(m_info.m_strName), OGDCPCTSTR(strDestFieldName),
		OGDCPCTSTR(strSrcFieldName));
	try
	{
		m_pDataSource->m_daoDatabase.Execute(strSQL);
	}
	catch (CDaoException* e)
	{
		e->Delete();
		return FALSE;
	}
	return TRUE;
}

OgdcBool OgdcDatasetVectorMdb::CreateFieldIndex( const OgdcString& strFieldNameList, const OgdcString& strIndexName )
{

	return	FALSE;
}

OgdcBool OgdcDatasetVectorMdb::DropFieldIndex( const OgdcString& strIndexName )
{
	return	FALSE;	
}

OgdcBool OgdcDatasetVectorMdb::CreateFields( const OgdcFieldInfos& fieldInfos )
{
	if (!Open())
	{
		return FALSE;
	}
	if (!CanUpdate())
	{
		return FALSE;
	}

	OgdcInt nFields = fieldInfos.GetSize();
	CDaoFieldInfo info;
	for (OgdcInt i = 0; i < nFields; i++)
	{

		// 检查字段名是否合法,不能为系统字段(OG开头)
		if (!IsAvailableFieldName(fieldInfos[i].m_strName))
		{
			continue;
		}

		if (fieldInfos[i].m_strName.Left(2).CompareNoCase(OGDCPCTSTR("Sm")) == 0) 
		{
			continue;
		}

		OgdcHelperMdb::OgdcFieldInfo2CDaoFieldInfo(fieldInfos.GetAt(i),info);
		try
		{
			
			info.m_bRequired = FALSE;
			info.m_lCollatingOrder = 0;
			info.m_nOrdinalPosition = 0;
			if (fieldInfos.GetAt(i).m_nType != OgdcFieldInfo::Text)
			{
				info.m_bAllowZeroLength = FALSE;
			}

			//不能创建系统字段
			m_pDaoTableDef->CreateField(info);
			//将创建的字段，加入内存m_fieldInfos集合。
			m_fieldInfos.Add(fieldInfos.GetAt(i));
		}
		catch (CDaoException* e)
		{
			e->Delete();
			m_pDaoTableDef->Close();
			delete m_pDaoTableDef;
			m_pDaoTableDef = NULL;
			return FALSE;
		}	
		
	}
	//设置修改标志
	SetModifiedFlag(TRUE);
	return TRUE;
}

OgdcBool OgdcDatasetVectorMdb::DeleteField( const OgdcString& strFieldName )
{
	for (OgdcInt i =0; i < m_fieldInfos.GetSize(); i++)
	{
		if (strFieldName.CompareNoCase(m_fieldInfos[i].m_strName) == 0)
		{ 
			if (i < 0 || i >= m_pDaoTableDef->GetFieldCount())
			{
				return FALSE;
			}
			if (!Open())
			{
				return FALSE;
			}
			if (!CanUpdate())
			{
				return FALSE;
			}
			
			CDaoFieldInfo daoFieldinfo;
			
			//删除字段
			try
			{
				m_pDaoTableDef->GetFieldInfo(i, daoFieldinfo);
				
				//系统字段不能删除,系统字段以"OG"开头
				if (OgdcHelperMdb::IsSystemField(OGDCPCTSTR(daoFieldinfo.m_strName))) 
				{
					return FALSE;
				}
				m_pDaoTableDef->DeleteField(i);
				m_fieldInfos.RemoveAt(i);
			}
			catch (CDaoException* e)
			{
				e->Delete();
				m_pDaoTableDef->Close();
				delete m_pDaoTableDef;
				m_pDaoTableDef = NULL;
				return FALSE;
			}

			return TRUE;
		}
	}
	return FALSE;
}

OgdcBool OgdcDatasetVectorMdb::GetFieldInfos( OgdcFieldInfos& fieldInfos, OgdcBool bSystemFieldExcluded /*= FALSE*/ )
{
	if (!Open())
	{
		return FALSE;
	}
	
	OgdcFieldInfo info;
	OgdcInt nFields= m_fieldInfos.GetSize();
	for (OgdcInt i = 0; i < nFields; i++)
	{
		info = m_fieldInfos.GetAt(i);
		if (OgdcHelperMdb::IsSystemField(info.m_strName))
		{
			//是否获取系统字段
			if (!bSystemFieldExcluded)
			{
				fieldInfos.Add(info);
			}
		}	
		else
		{
			fieldInfos.Add(info);
		}
	}

	return TRUE;
}

OgdcBool OgdcDatasetVectorMdb::GetFieldInfo( const OgdcString& strFieldName, OgdcFieldInfo& fieldInfo )
{
	if (!Open())
	{
		return FALSE;
	}
	CDaoFieldInfo daoFieldInfo;
	try
	{
		m_pDaoTableDef->GetFieldInfo(strFieldName, daoFieldInfo, AFX_DAO_PRIMARY_INFO);
		OgdcHelperMdb::CDaoFieldInfo2OgdcFieldInfo(daoFieldInfo, fieldInfo);
	}
	catch (CDaoException* e)
	{
		e->Delete();
		return FALSE;
	}

	return TRUE;
}

OgdcBool OgdcDatasetVectorMdb::SetFieldInfo( const OgdcString& strFieldName, const OgdcFieldInfo& fieldInfo )
{
	if (!Open())
	{
		return FALSE;
	}
	CDaoFieldInfo daoFieldInfo;
	OgdcFieldInfo desFieldInfo;
	//如果源字段信息跟要设置的信息一样，返回，即什么也没改变
	GetFieldInfo(strFieldName, desFieldInfo);
	if (desFieldInfo == fieldInfo)
	{
		return TRUE;
	}
	//系统字段不允许修改
	if (OgdcHelperMdb::IsSystemField(strFieldName)
		|| OgdcHelperMdb::IsSystemField(fieldInfo.m_strName))
	{
		return FALSE;
	}

	OgdcFieldInfo tempInfo = fieldInfo;
	tempInfo.m_strName = GetUnoccupiedFieldName("tempField");
	OgdcFieldInfos tempFieldInfos;
	tempFieldInfos.Add(tempInfo);
	
	try
	{
		//用fieldInfo创建一个临时字段，拷贝strFieldName字段到临时字段
		//再删除strFieldName字段，将临时字段改名为strFieldName
		if (!CreateFields(tempFieldInfos))
		{
			return FALSE;
		}
		//将strFieldName字段的值拷贝到 tempField
		CopyField(strFieldName, tempFieldInfos[0].m_strName);
		tempFieldInfos.RemoveAt(0);
		tempFieldInfos.Add(fieldInfo);
		if (!CreateFields(tempFieldInfos))
		{
			DeleteField(tempInfo.m_strName);
			return FALSE;
		}
		CopyField(tempInfo.m_strName, fieldInfo.m_strName);
		DeleteField(tempInfo.m_strName);
		DeleteField(strFieldName);
	}
	catch(CDaoException *e)
	{
		e->Delete();
		tempFieldInfos.RemoveAll();
		return FALSE;
	}

	return TRUE;	
}

OgdcVariant OgdcDatasetVectorMdb::Statistic( const OgdcString& strFieldName, OgdcRecordset::OgdcStatisticMode nMode )
{
	OgdcVariant ogdcVar = 0;
	if (!IsOpen())
	{
		return ogdcVar;
	}
	OgdcFieldInfo fieldInfo;
	GetFieldInfo(strFieldName, fieldInfo);
	if (!(fieldInfo.m_nType == OgdcFieldInfo::INT16
		|| fieldInfo.m_nType == OgdcFieldInfo::INT32
		|| fieldInfo.m_nType == OgdcFieldInfo::INT64
		|| fieldInfo.m_nType == OgdcFieldInfo::Float
		|| fieldInfo.m_nType == OgdcFieldInfo::Double))
	{
		return ogdcVar;
	}
	OgdcString strSQL;
	switch (nMode)
	{
	case OgdcRecordset::smMax:
		strSQL.Format(OGDCPCTSTR("SELECT Max(%s) FROM [%s]"),
			OGDCPCTSTR(strFieldName), OGDCPCTSTR(m_info.m_strName));
		break;
	case OgdcRecordset::smMin:
		strSQL.Format(OGDCPCTSTR("SELECT Min(%s) FROM [%s]"),
			OGDCPCTSTR(strFieldName), OGDCPCTSTR(m_info.m_strName));
		break;
	case OgdcRecordset::smAvg:
		strSQL.Format(OGDCPCTSTR("SELECT Avg(%s) FROM [%s]"),
			OGDCPCTSTR(strFieldName), OGDCPCTSTR(m_info.m_strName));
		break;
	case OgdcRecordset::smSum:
		strSQL.Format(OGDCPCTSTR("SELECT Sum(%s) FROM [%s]"),
			OGDCPCTSTR(strFieldName), OGDCPCTSTR(m_info.m_strName));
		break;
	case OgdcRecordset::smStdev:
		strSQL.Format(OGDCPCTSTR("SELECT StDev(%s) FROM [%s]"),
			OGDCPCTSTR(strFieldName), OGDCPCTSTR(m_info.m_strName));
		break;
	case OgdcRecordset::smVar:
		strSQL.Format(OGDCPCTSTR("SELECT Var(%s) FROM [%s]"),
			OGDCPCTSTR(strFieldName), OGDCPCTSTR(m_info.m_strName));
		break;
	default:
		return ogdcVar;
	}
	
	CDaoRecordset* recordset = new CDaoRecordset(&(m_pDataSource->m_daoDatabase));
	if (recordset == NULL) 
	{
		delete recordset;
		recordset = NULL;
		return ogdcVar;
	}
	try
	{
		recordset->Open(dbOpenDynaset, strSQL);
		COleVariant varValue;
		recordset->MoveFirst();
		recordset->GetFieldValue(0, varValue);
		OgdcHelperMdb::COleVariant2OgdcVariant(varValue, ogdcVar);
	}
	catch(CDaoException* e)
	{
		recordset->Close();
		delete recordset;
		recordset = NULL;
		e->Delete();
		return ogdcVar;
	}

	recordset->Close();
	delete recordset;
	recordset = NULL;

	return ogdcVar;
}

OgdcBool OgdcDatasetVectorMdb::IsRaster()
{
	if (m_info.m_nType == OgdcDataset::Grid 
		|| m_info.m_nType == OgdcDataset::Image)
	{
		return	TRUE;
	}
	else
	{
		return	FALSE;	
	}
}

OgdcRect2D OgdcDatasetVectorMdb::GetBounds()
{
	OgdcRect2D rcBounds; 
	if (!IsOpen())
	{
		return m_info.m_rcBounds;
	}
	if (!m_info.m_rcBounds.IsEmpty())
	{
		return m_info.m_rcBounds;
	}

	OgdcString strSQL;
	strSQL.Format(OGDCPCTSTR("SELECT OGLeft,OGRight,OGTop,OGBottom "
		"FROM OGRegister WHERE OGDatasetName = '%s'"),
		OGDCPCTSTR(m_info.m_strName));

	CDaoRecordset tempRecord(&m_pDataSource->m_daoDatabase);
	COleVariant varValue;
	try
	{
		tempRecord.Open(dbOpenSnapshot, strSQL);
		tempRecord.GetFieldValue(0, varValue);
		rcBounds.left = varValue.dblVal;
		tempRecord.GetFieldValue(1, varValue);
		rcBounds.right = varValue.dblVal;
		tempRecord.GetFieldValue(2, varValue);
		rcBounds.top = varValue.dblVal;
		tempRecord.GetFieldValue(3, varValue);
		rcBounds.bottom = varValue.dblVal;
		tempRecord.Close();
	}
	catch (CDaoException* e)
	{
		e->Delete();
		tempRecord.Close();
		return OgdcRect2D();
	}
	return rcBounds;
}

OgdcString OgdcDatasetVectorMdb::GetUnoccupiedFieldName( const OgdcString& strFieldName )
{
	OgdcString strDsName = strFieldName;
	OgdcString strName = strFieldName;
	//如果没有传进字符串，则用默认的"T_"命名
	if (strName.IsEmpty())
	{
		strName = "T";
	}
	OgdcString	strVSQL,strRSQL;
	OgdcString strNull = "";
	OgdcString subStr = "";
	OgdcInt i = 1;	
	if (!IsAvailableFieldName(strFieldName))
	{
		return	strNull;
	}

	for (OgdcInt nIndex = 0; nIndex < m_fieldInfos.GetSize(); nIndex++)
	{
		if (m_fieldInfos.GetAt(nIndex).m_strName == strName)
		{
			subStr.Format("_%d", i++);
			strName = strDsName + subStr;
			//重新判断是否有重名字段
			nIndex = -1;
		}
	}

	return strName;		
}

OgdcBool OgdcDatasetVectorMdb::IsAvailableFieldName( const OgdcString& strFieldName )
{
	if (strFieldName.IsEmpty())
	{
		return FALSE;
	}

	if (OgdcHelperMdb::IsSystemField(strFieldName)) 
	{
		return FALSE;
	}

	if (OgdcHelperMdb::IsKeyWord(strFieldName))
	{
		return FALSE;
	}

	if(strFieldName.GetLength() > 30 ) 
	{
		//名称>30个字符
		return FALSE;
	}

	OgdcBool bResult = TRUE;
	OgdcBool bNumber = FALSE;//是否是数字
	OgdcBool bLetter = FALSE; //是否是字母
	OgdcBool bUnderline = FALSE;//是否是下划线

	OgdcString strTemp(strFieldName);
	OgdcInt nSize = strTemp.GetLength();
	OgdcChar *pChar = NULL;

	for(OgdcInt i=0;i<nSize;i++)
	{
		OgdcWchar charW = strTemp.GetAt(i);
		pChar = (OgdcChar *) &charW;
		if ( pChar[0] == 0 || pChar[1] == 0 )
		{
			OgdcInt n = 0;
			if( pChar[0] == 0 )	// UGC_ISBIGENDIAN
			{
			  if(sizeof(OgdcWchar) ==2)
			  {
				  n = 1;
			  }
			  else if(sizeof(OgdcWchar) == 4)
			  {
				  n = 3;
			  }
			}
			bNumber = (pChar[n] >= 48 && pChar[n] <= 57);//数字0-9的ASCII码:48-57
			bLetter = (pChar[n] >= 65 && pChar[n] <= 90)||(pChar[n] >= 97 && pChar[n] <= 122);//A-z,a-z的ASCII码
			bUnderline = (pChar[n] == 95);//'_'的ASCII码是95
			if( (bNumber||bLetter||bUnderline)  == FALSE )
			{//有非法字符
				bResult = FALSE;
				break;
			}
			else if(i == 0)
			{
				if(bNumber||bUnderline)
				{//第一个字符不能是数字或下划线
					bResult = FALSE;
					break;
				}
			}
		}
	}
	
	return bResult;		
}

void OgdcDatasetVectorMdb::Close()
{
	if (!IsOpen())
	{ 
		return;
	}

	if (IsModified())
	{
		SaveInfo();	
	}

	//释放所有相关Recordset
	ReleaseAllRecordsets();
	
	if (m_pDaoTableDef != NULL)
	{
		m_pDaoTableDef->Close();
		delete m_pDaoTableDef;
		m_pDaoTableDef = NULL;
	}
	m_bOpened = FALSE;
	m_bOpened = FALSE;
}

OgdcBool OgdcDatasetVectorMdb::Rename( const OgdcString& strNewName )
{
	if (!m_pDataSource->IsAvailableDatasetName(strNewName))
	{
		return FALSE;
	}
	OgdcString strName = m_pDataSource->GetUnoccupiedDatasetName(strNewName);
	if (strName != strNewName)
	{
		return FALSE;
	}
	OgdcString strSQL;
	try
	{
		strSQL.Format(OGDCPCTSTR("UPDATE OGRegister "
			"SET OGDatasetName = '%s' WHERE OGDatasetName = '%s'"), 
			OGDCPCTSTR(strNewName), OGDCPCTSTR(m_info.m_strName));
		m_pDataSource->m_daoDatabase.Execute(strSQL);
		m_pDaoTableDef->SetName(strNewName);

	}
	catch(CDaoException* e)
	{
		e->Delete ();
		m_pDaoTableDef->Close();
		m_pDataSource->m_daoDatabase.Close();
		return FALSE;
	}
	m_info.m_strName = strNewName;

	return TRUE;	
}

OgdcBool OgdcDatasetVectorMdb::CanUpdate()
{
	if (m_pDataSource->m_connection.m_bReadOnly == TRUE)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

OgdcBool OgdcDatasetVectorMdb::SaveInfo()
{
 	if (!m_pDataSource->m_daoDatabase.IsOpen())
 	{
 		return	FALSE;
 	}
	if (!IsModified())
	{
		return TRUE;
	}
	CDaoRecordset recordset(&m_pDataSource->m_daoDatabase);
	OgdcString strSQL;
	strSQL.Format(OGDCPCTSTR("SELECT * FROM OGRegister "
		"WHERE OGDatasetName = '%s'"), OGDCPCTSTR(m_info.m_strName));	
 	CDaoTableDef tableDef(&m_pDataSource->m_daoDatabase);
	try
	{
		recordset.Open(AFX_DAO_USE_DEFAULT_TYPE, strSQL);
		recordset.Edit();
		recordset.SetFieldValue(OGRF_DATASET_NAME,
			COleVariant(m_info.m_strName, VT_BSTRT));	
		recordset.SetFieldValue(OGRF_DATASET_TYPE, long(m_info.m_nType));
		recordset.SetFieldValue(OGRF_LEFT, m_info.m_rcBounds.left);
		recordset.SetFieldValue(OGRF_RIGHT, m_info.m_rcBounds.right);
		recordset.SetFieldValue(OGRF_TOP, m_info.m_rcBounds.top);
		recordset.SetFieldValue(OGRF_BOTTOM, m_info.m_rcBounds.bottom);
		recordset.SetFieldValue(OGRF_LASTUPDATE, COleDateTime::GetCurrentTime()); 
		recordset.SetFieldValue(OGRF_RECORDCOUNT, long(m_nObjectCount));
		recordset.Update ();
		recordset.Close();
	}
	catch (CDaoException* e)
	{
		e->Delete();
		recordset.Close();
		return FALSE;	
	}

	return TRUE;	
}

OGDC::OgdcBool OgdcDatasetVectorMdb::RefreshInfo()
{

	if (IsModified())
	{
		SaveInfo();
	}
	OgdcString strSQL;
	strSQL.Format(OGDCPCTSTR("SELECT * FROM OGRegister "
		"WHERE OGDatasetName = '%s'"), OGDCPCTSTR(m_info.m_strName));	
	//先释放所有查询记录集
	ReleaseAllRecordsets();

	CDaoRecordset recordset(&(m_pDataSource->m_daoDatabase));	
	OgdcDouble dLeft = 0.0;
	OgdcDouble dTop = 0.0;
	OgdcDouble dRight = 0.0;
	OgdcDouble dBottom = 0.0;	
	COleVariant varValue;
	try
	{
		recordset.Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);
		recordset.GetFieldValue(OGRF_ID, varValue);
		m_nID = varValue.lVal;
		recordset.GetFieldValue(OGRF_LEFT, varValue);
		dLeft = varValue.dblVal;
		recordset.GetFieldValue(OGRF_RIGHT, varValue);
		dRight = varValue.dblVal;
		recordset.GetFieldValue(OGRF_TOP, varValue);
		dTop = varValue.dblVal;
		recordset.GetFieldValue(OGRF_BOTTOM, varValue);
		dBottom = varValue.dblVal;
		recordset.GetFieldValue(OGRF_RECORDCOUNT, varValue);
		m_nObjectCount = varValue.lVal;

		recordset.Close();
	}
	catch (CDaoException* e)
	{
		e->Delete();
		recordset.Close();
		return FALSE;
	}
	m_info.m_rcBounds.SetRect(dLeft, dTop, dRight, dBottom);
	return TRUE;
}

OgdcBool OgdcDatasetVectorMdb::IsOpen() const
{
	return m_bOpened;
}

OgdcBool OgdcDatasetVectorMdb::Open()
{
	if (IsOpen())
	{
		return TRUE;
	}

	m_pDaoTableDef = new CDaoTableDef(&(m_pDataSource->m_daoDatabase));	
	OgdcString strSQL;
	strSQL.Format(OGDCPCTSTR("SELECT * FROM [%s] "), OGDCPCTSTR(m_info.m_strName));
	try
	{
		m_pDaoTableDef->Open(m_info.m_strName);

		m_fieldInfos.RemoveAll ();
		long fCount = m_pDaoTableDef->GetFieldCount();
		
		for (OgdcInt i = 0; i < fCount; i++)
		{
			OgdcFieldInfo tempFieldinfo;
			CDaoFieldInfo daoFiledInfo;
			m_pDaoTableDef->GetFieldInfo (i, daoFiledInfo, AFX_DAO_PRIMARY_INFO);
			OgdcHelperMdb::CDaoFieldInfo2OgdcFieldInfo(daoFiledInfo, tempFieldinfo);
			OgdcHelperMdb::SetFieldSign(tempFieldinfo);
			m_fieldInfos.Add(tempFieldinfo);
		}
	}
	catch (CDaoException* e)
	{
		m_pDaoTableDef->Close();
		delete m_pDaoTableDef;
		m_pDaoTableDef = NULL;
		e->Delete();
		return FALSE;
	}		
	
	m_bOpened = TRUE;
	return TRUE;
}

OgdcBool OgdcDatasetVectorMdb::IsModified() const
{
	return m_bModified;
}

void OgdcDatasetVectorMdb::SetModifiedFlag( OgdcBool bModified /*= TRUE*/ )
{
	m_bModified = bModified;
}

OgdcDataSource* OgdcDatasetVectorMdb::GetDataSource() const
{
	return (OgdcDataSource*)m_pDataSource;
}

//##ModelId=48202C9D00A6
OgdcBool OgdcDatasetVectorMdb::SetCoordSys(const OgdcCoordSys& coordSys)
{
	m_cordsys = coordSys;
	return TRUE;
}

//##ModelId=48202C9D00A9
OgdcBool OgdcDatasetVectorMdb::GetCoordSys(OgdcCoordSys& coordSys) const
{
	coordSys = m_cordsys;
	return TRUE;
}

OgdcBool OgdcDatasetVectorMdb::IsSupportQueryType(OgdcQueryDef::QueryType queryType) const
{
	switch(queryType)
	{
	case OgdcQueryDef::General:
	case OgdcQueryDef::Bounds:
	case OgdcQueryDef::IDs:
		{
			return TRUE;
		}
	default:
		break;
	}
	return FALSE;
}
}