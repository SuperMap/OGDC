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
}

OgdcDatasetVectorMdb::OgdcDatasetVectorMdb( OgdcDataSourceMdb *pDataSource )
{
	m_nObjectCount = 0;
	m_nID = 0;
	m_bModified = FALSE;
	m_pDataSource = pDataSource;
	m_pDaoTableDef = NULL;
	m_bOpened = FALSE;
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
	strSQL.Format( _U("SELECT OGRecordCount FROM OGRegister WHERE OGDatasetName = '%s'"), 
		m_info.m_strName.Cstr());
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
	return m_recordsets.GetSize();
	/*
	if (m_nRecordCount)
	{
		return OgdcInt(m_nRecordCount);
	}
	OgdcString strSQL;
	COleVariant varValue;
	CDaoRecordset recordset(&(m_pDataSource->m_daoDatabase));
	strSQL.Format( _U("SELECT COUNT(*) FROM OGRegister "));
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
	*/
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
		return NULL;
	}

	if(pRecordset->SeekID(nID))
	{
		pFeature = (OgdcFeatureMdb*)pRecordset->GetFeature();
	}

	ReleaseRecordset(pRecordset);
	pRecordset = NULL;
	return pFeature;
}

UGC::UGFeature* OgdcDatasetVectorMdb::GetFeatureEx(OgdcInt nID)
{
	if (GetType() != OgdcDataset::PointEPS &&
		GetType() != OgdcDataset::LineEPS &&
		GetType() != OgdcDataset::RegionEPS &&
		GetType() != OgdcDataset::TextEPS &&
		GetType() != OgdcDataset::Model &&
		GetType() != OgdcDataset::Texture)
	{
		OgdcFeature *pOFeature = GetFeature(nID);
		if (pOFeature == NULL)
		{
			return NULL;
		}

		UGC::UGFeature *pFeature = new UGC::UGFeature();
		pFeature->SetOgdcFeature(pOFeature);
		return pFeature;
	}

	UGC::UGFeature* pFeature = NULL;
	OgdcQueryDef queryDef;
	queryDef.m_nType = OgdcQueryDef::IDs;
	queryDef.m_IDs.Add(nID);
	queryDef.m_nOptions = OgdcQueryDef::Both;
	OgdcRecordsetMdb *pRecordset = (OgdcRecordsetMdb*)Query(queryDef);
	if (pRecordset == NULL)
	{
		return NULL;
	}

	if(pRecordset->SeekID(nID))
	{
		pFeature = pRecordset->GetFeatureEx();
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

OgdcBool OgdcDatasetVectorMdb::UpdateFeature(const UGC::UGFeature* pFeature)
{
	if (pFeature == NULL)
	{
		return FALSE;
	}

	if (GetType() != OgdcDataset::PointEPS &&
		GetType() != OgdcDataset::LineEPS &&
		GetType() != OgdcDataset::RegionEPS &&
		GetType() != OgdcDataset::TextEPS &&
		GetType() != OgdcDataset::Model &&
		GetType() != OgdcDataset::Texture)
	{
		return UpdateFeature(pFeature->GetOgdcFeature());
	}

	OgdcBool bResult = FALSE;
	OgdcInt nID = pFeature->GetID();
	OgdcQueryDef queryDef;
	queryDef.m_nCursorType = OgdcQueryDef::OpenDynamic;
	queryDef.m_IDs.Add(nID);
	queryDef.m_nType = OgdcQueryDef::IDs;
	queryDef.m_nOptions = OgdcQueryDef::Both;

	OgdcRecordsetMdb *pRecordset = (OgdcRecordsetMdb*)Query(queryDef);
	if (pRecordset == NULL)
	{
		return bResult;
	}

	if(pRecordset->SeekID(nID))
	{
		bResult = pRecordset->UpdateFeature(pFeature);
	}

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
	queryDef.m_strFilter = _U("");
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

OgdcBool OgdcDatasetVectorMdb::AddFeature(const UGC::UGFeature* pFeature)
{
	if(pFeature == NULL)
	{
		return FALSE;
	}

	if (GetType() != OgdcDataset::PointEPS &&
		GetType() != OgdcDataset::LineEPS &&
		GetType() != OgdcDataset::RegionEPS &&
		GetType() != OgdcDataset::TextEPS &&
		GetType() != OgdcDataset::Model &&
		GetType() != OgdcDataset::Texture)
	{
		return AddFeature(pFeature->GetOgdcFeature());
	}

	if (!IsOpen())
	{
		Open();
	}

	OgdcBool bResult = FALSE;
	OgdcQueryDef queryDef;
	queryDef.m_nType = OgdcQueryDef::General;
	queryDef.m_strFilter = _U("");
	queryDef.m_nOptions = OgdcQueryDef::Both;

	OgdcRecordsetMdb *pRecordset = (OgdcRecordsetMdb*)Query(queryDef);
	bResult = pRecordset->AddFeature(pFeature);
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
	strSQL.Format(_U("DELETE * FROM [%s]"), m_info.m_strName.Cstr());
	try
	{
		m_pDataSource->m_daoDatabase.Execute(strSQL);
		strSQL.Format(_U("UPDATE OGRegister SET OGRecordCount = 0 WHERE OGDatasetName = '%s'"), 
			m_info.m_strName.Cstr());
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
	OgdcString subStr = _U("");
	OgdcString strFieldName = _U("");
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

			if(	strFieldName.Find(_U("MIN("))>-1 || 
				strFieldName.Find(_U("MAX("))>-1 || 
				strFieldName.Find(_U("SUM("))>-1 || 
				strFieldName.Find(_U("AVG("))>-1 ||
				strFieldName.Find(_U("COUNT("))> -1 ||
				strFieldName.Find(_U("STDDEV("))>-1 ||
				strFieldName.Find(_U("VARIANCE("))>-1 ||
				strFieldName.Find(_U("DISTINCT "))>-1)
			{
				bStat = TRUE;
				break;
			}
		}

		if (bStat || strSort.Find(_U("GROUP BY"))>-1 || !strGroup.IsEmpty())
		{
			for(i=0;i<querydef.m_Fields.GetSize();i++)
			{
				subStr.Format(_U("%s,"), querydef.m_Fields.GetAt(i).Cstr());
				strField = strField + subStr;					
			}
		}
		else
		{
			for (i = 0; i < m_fieldInfos.GetSize(); i ++)
			{
				subStr.Format(_U("%s.%s,"), m_info.m_strName.Cstr(), 
					m_fieldInfos.GetAt(i).m_strName.Cstr());
				strField = strField + subStr;					
			}

			OgdcString strFieldName;
			for (i = 0; i < querydef.m_Fields.GetSize(); i++)
			{
				strFieldName = querydef.m_Fields.GetAt(i);
				if (strField.Find(strFieldName) > -1)
					continue;
				subStr.Format(_U("%s,"), strFieldName.Cstr());
				strField = strField + subStr;
			}
		}
		strField.SetAt(strField.GetLength() - 1, _U('\0'));
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
				strField = _U("*");
			}
			
			if (querydef.m_strFilter.IsEmpty())
			{
				strSQL.Format(_U("SELECT %s FROM [%s] %s %s"),
					strField.Cstr(),m_info.m_strName.Cstr(),
					querydef.m_strGroup.Cstr(),querydef.m_strSort.Cstr());
			}
			else
			{
				strSQL.Format(_U("SELECT %s FROM [%s] WHERE %s %s %s"),
					strField.Cstr(),m_info.m_strName.Cstr(), 
					querydef.m_strFilter.Cstr(),querydef.m_strGroup.Cstr(),
					querydef.m_strSort.Cstr());

				
				pRecordset->m_strFilter.Format(_U(" %s"), querydef.m_strFilter.Cstr());
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

			OgdcDouble dScale = rcBounds.Width() > rcBounds.Height() ? rcBounds.Width() : rcBounds.Height();
			dScale = dScale * EP + EP*10;
			rcBounds.Inflate(dScale);

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
				strField.Format(_U("*"));
			}

			if (GetType() == OgdcDataset::Point || GetType() == OgdcDataset::Point3D)
			{
				pRecordset->m_strFilter.Format(_U("OGX >= %.16f AND OGY <= %.16f AND OGX <= %.16f AND OGY >= %.16f"),
					rcBounds.left, rcBounds.top, rcBounds.right, rcBounds.bottom);
			}
			else
			{
				pRecordset->m_strFilter.Format(_U("OGID NOT IN ")
					_U("( SELECT OGID FROM [%s] ")
					_U("where OGSdriW > %.16f or OGSdriN < %.16f ")
					_U("or OGSdriE < %.16f or OGSdriS > %.16f ) "),
					m_info.m_strName.Cstr(),rcBounds.right, 
					rcBounds.bottom, rcBounds.left, rcBounds.top);
			}

			if(!querydef.m_strFilter.IsEmpty())
			{
				pRecordset->m_strFilter += _U(" AND ") + querydef.m_strFilter;
			}

			strSQL.Format(_U("SELECT %s FROM [%s] WHERE %s"),strField.Cstr(),
				m_info.m_strName.Cstr(),pRecordset->m_strFilter.Cstr());

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
			strSQL.Format(_U("SELECT * FROM [%s] WHERE "),m_info.m_strName.Cstr());

			OgdcString strFilter;
			if (querydef.m_strIDFieldName.CompareNoCase(_U("SmHashCode")) == 0 ||
				querydef.m_strIDFieldName.CompareNoCase(_U("OGHashCode")) == 0)
			{
				strFilter.Format(_U(" OGHashCode IN ("));
			}
			else
			{
				strFilter.Format(_U(" OGID IN ("));
			}
			OgdcString strSQL1;
			for (OgdcInt i = 0; i < querydef.m_IDs.GetSize(); i++)
			{
				strSQL1.Format(_U("%d,"), querydef.m_IDs.GetAt(i));
				strFilter = strFilter + strSQL1;
			}
			OgdcChar ch = _U(')');
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
		strSQL.Format(_U("UPDATE [%s] SET %s = %s "),
			m_info.m_strName.Cstr(), strFieldName.Cstr(),strExpress.Cstr());
	}
	else
	{
		strSQL.Format(_U("UPDATE [%s] SET %s = %s WHERE %s"),
			m_info.m_strName.Cstr(), strFieldName.Cstr(),strExpress.Cstr(), strFilter.Cstr());
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
	strSQL.Format(_U("UPDATE %s SET %s = %s"),
		m_info.m_strName.Cstr(), strDestFieldName.Cstr(),strSrcFieldName.Cstr());
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

		if (fieldInfos[i].m_strName.Left(2).CompareNoCase(_U("Sm")) == 0) 
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
	if (!Open())
	{
		return FALSE;
	}
	if (!CanUpdate())
	{
		return FALSE;
	}

	for (OgdcInt i =0; i < m_fieldInfos.GetSize(); i++)
	{
		if (strFieldName.CompareNoCase(m_fieldInfos[i].m_strName) == 0)
		{ 
			if (i < 0 || i >= m_pDaoTableDef->GetFieldCount())
			{
				return FALSE;
			}

			CDaoFieldInfo daoFieldinfo;
			
			//删除字段
			try
			{
				m_pDaoTableDef->GetFieldInfo(i, daoFieldinfo);
				
				//系统字段不能删除,系统字段以"OG"开头
				OgdcString strTemp(daoFieldinfo.m_strName);
				if (OgdcHelperMdb::IsSystemField(strTemp)) 
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
	tempInfo.m_strName = GetUnoccupiedFieldName(_U("tempField"));
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
		strSQL.Format(_U("SELECT Max(%s) FROM [%s]"),
			strFieldName.Cstr(), m_info.m_strName.Cstr());
		break;
	case OgdcRecordset::smMin:
		strSQL.Format(_U("SELECT Min(%s) FROM [%s]"),
			strFieldName.Cstr(), m_info.m_strName.Cstr());
		break;
	case OgdcRecordset::smAvg:
		strSQL.Format(_U("SELECT Avg(%s) FROM [%s]"),
			strFieldName.Cstr(), m_info.m_strName.Cstr());
		break;
	case OgdcRecordset::smSum:
		strSQL.Format(_U("SELECT Sum(%s) FROM [%s]"),
			strFieldName.Cstr(), m_info.m_strName.Cstr());
		break;
	case OgdcRecordset::smStdev:
		strSQL.Format(_U("SELECT StDev(%s) FROM [%s]"),
			strFieldName.Cstr(), m_info.m_strName.Cstr());
		break;
	case OgdcRecordset::smVar:
		strSQL.Format(_U("SELECT Var(%s) FROM [%s]"),
			strFieldName.Cstr(), m_info.m_strName.Cstr());
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
	strSQL.Format(_U("SELECT OGLeft,OGRight,OGTop,OGBottom FROM OGRegister WHERE OGDatasetName = '%s'"),
		m_info.m_strName.Cstr());

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
		strName = _U("T");
	}
	OgdcString	strVSQL,strRSQL;
	OgdcString strNull = _U("");
	OgdcString subStr = _U("");
	OgdcInt i = 1;	
	if (!IsAvailableFieldName(strFieldName))
	{
		return	strNull;
	}

	for (OgdcInt nIndex = 0; nIndex < m_fieldInfos.GetSize(); nIndex++)
	{
		if (m_fieldInfos.GetAt(nIndex).m_strName == strName)
		{
			subStr.Format(_U("_%d"), i++);
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

	if (GetType() == OgdcDataset::Model)
	{
		if (m_arrSubDataset.GetSize() > 0)
		{
			OgdcString strNewNameSub = strNewName + _U("_Texture");
			strNewNameSub = m_pDataSource->GetUnoccupiedDatasetName(strNewNameSub);
			if (!m_arrSubDataset[0]->Rename(strNewNameSub))
			{
				return FALSE;
			}
		}
	}

	OgdcString strSQL;
	try
	{
		strSQL.Format(_U("UPDATE OGRegister SET OGDatasetName = '%s' WHERE OGDatasetName = '%s'"), 
			strNewName.Cstr(), m_info.m_strName.Cstr());
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
	strSQL.Format(_U("SELECT * FROM OGRegister WHERE OGDatasetName = '%s'"), m_info.m_strName.Cstr());	
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
	strSQL.Format(_U("SELECT * FROM OGRegister WHERE OGDatasetName = '%s'"), m_info.m_strName.Cstr());	
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
	strSQL.Format(_U("SELECT * FROM [%s] "), m_info.m_strName.Cstr());
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
			tempFieldinfo.m_strForeignName = tempFieldinfo.m_strName;
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

OgdcBool OgdcDatasetVectorMdb::CreateSubDatasetVector(OgdcDatasetVectorInfo& vInfo)
{
	if (m_pDataSource == NULL)
	{
		return FALSE;
	}
	if (!m_pDataSource->IsOpen())
	{
		return FALSE;
	}

	vInfo.m_strName = m_pDataSource->GetUnoccupiedDatasetName(vInfo.m_strName);
	if (vInfo.m_strName.IsEmpty())
	{
		return FALSE;
	}

	OgdcFieldInfos fieldInfos;
	fieldInfos.AddField(OG_OBJ_ID, OgdcFieldInfo::INT32, sizeof(dbLong), 0, FALSE, FALSE);
	switch (vInfo.m_nType)
	{
	case OgdcDataset::Texture:
		fieldInfos.AddField(OG_HASHCODE, OgdcFieldInfo::INT32, sizeof(dbLong), 0, FALSE, FALSE);
		fieldInfos.AddField(OG_GEOMETRY, OgdcFieldInfo::LongBinary, 0, 0, FALSE, FALSE);
		break;
	default:
		break;
	}

	CDaoTableDef daoTableDef(&m_pDataSource->m_daoDatabase);
	try
	{
		daoTableDef.Create(vInfo.m_strName);
	}
	catch (CDaoException* e)
	{
		e->Delete();
		return FALSE;
	}
	try
	{
		OgdcInt nFields = fieldInfos.GetSize();
		CDaoFieldInfo info;
		for (OgdcInt i=0; i<nFields; i++)
		{
			OgdcHelperMdb::OgdcFieldInfo2CDaoFieldInfo(fieldInfos.GetAt(i),info);
			daoTableDef.CreateField(info);
		}

		//建立一个OGId字段的索引
		CDaoIndexInfo indexInfo;
		CDaoIndexFieldInfo indexFieldInfo;
		indexFieldInfo.m_strName = OG_OBJ_ID;
		indexFieldInfo.m_bDescending = FALSE;
		indexInfo.m_strName = _U("OGID_idx_") + vInfo.m_strName;
		indexInfo.m_pFieldInfos = &indexFieldInfo;
		indexInfo.m_nFields = 1;
		indexInfo.m_bPrimary = TRUE;
		daoTableDef.CreateIndex(indexInfo);

		if (vInfo.m_nType == OgdcDataset::Texture)
		{
			CDaoIndexInfo indexInfo1;
			CDaoIndexFieldInfo indexFieldInfo1;
			indexFieldInfo1.m_strName = OG_HASHCODE;
			indexFieldInfo1.m_bDescending = FALSE;
			indexInfo1.m_strName = _U("OGHashCode_idx_") + vInfo.m_strName;
			indexInfo1.m_pFieldInfos = &indexFieldInfo1;
			indexInfo1.m_nFields = 1;
			indexInfo1.m_bUnique = TRUE;
			daoTableDef.CreateIndex(indexInfo1);
		}

		daoTableDef.Append();
		daoTableDef.Close();
	}
	catch (CDaoException* e)
	{
		e->Delete();
		daoTableDef.Close();
		return FALSE;
	}

	//得到数据集的最大ID
	COleVariant lRecordMaxId;
	OgdcString strSQL = _U("SELECT MAX(OGDatasetID) FROM OGRegister");
	CDaoRecordset recordset(&m_pDataSource->m_daoDatabase);
	try
	{
		recordset.Open(dbOpenSnapshot, strSQL);
		lRecordMaxId = recordset.GetFieldValue(0);
		recordset.Close();
	}
	catch (CDaoException* e)
	{
		//获取最大ID，当为零条记录时，获取open异常，表示没有记录
		e->Delete();
	}

	OgdcDatasetVectorMdb *pDatasetVector = new OgdcDatasetVectorMdb(this->m_pDataSource);
	lRecordMaxId.intVal++;
	OgdcInt nID = lRecordMaxId.intVal;
	pDatasetVector->m_nID = nID;
	pDatasetVector->m_info = vInfo;
	pDatasetVector->m_info.m_rcBounds.left = 0.0;
	pDatasetVector->m_info.m_rcBounds.top = 0.0;
	pDatasetVector->m_info.m_rcBounds.right = 0.0;
	pDatasetVector->m_info.m_rcBounds.bottom = 0.0;

	//加入注册表
	CDaoTableDef tableDef(&m_pDataSource->m_daoDatabase);
	try
	{
		tableDef.Open(OG_REGISTER_TABLE);
	}
	catch (CDaoException* e)
	{
		e->Delete();
		delete pDatasetVector;
		pDatasetVector = NULL;
		try
		{
			m_pDataSource->m_daoDatabase.DeleteTableDef(vInfo.m_strName);
		}
		catch (CDaoException* e1)
		{
			e1->Delete();
		}
		return FALSE;
	}
	CDaoRecordset record;
	try
	{
		record.Open(&tableDef);
	}
	catch (CDaoException* e)
	{
		e->Delete();
		tableDef.Close();
		delete pDatasetVector;
		pDatasetVector = NULL;
		try
		{
			m_pDataSource->m_daoDatabase.DeleteTableDef(vInfo.m_strName);
		}
		catch (CDaoException* e1)
		{
			e1->Delete();
		}
		return FALSE;
	}
	try
	{
		record.AddNew();
		//如果setFieldValue后的数为OgdcInt或OgdcLong，需要用long强制转换
		record.SetFieldValue(OGRF_ID, long(pDatasetVector->m_nID));
		record.SetFieldValue(OGRF_DATASET_NAME, COleVariant(pDatasetVector->m_info.m_strName, VT_BSTRT));
		record.SetFieldValue(OGRF_PARENTDT_ID, long(this->m_nID));
		record.SetFieldValue(OGRF_DATASET_TYPE, long(pDatasetVector->m_info.m_nType));
		record.SetFieldValue(OGRF_LEFT, pDatasetVector->m_info.m_rcBounds.left);
		record.SetFieldValue(OGRF_RIGHT, pDatasetVector->m_info.m_rcBounds.right);
		record.SetFieldValue(OGRF_TOP, pDatasetVector->m_info.m_rcBounds.top);
		record.SetFieldValue(OGRF_BOTTOM, pDatasetVector->m_info.m_rcBounds.bottom);
		record.SetFieldValue(OGRF_LASTUPDATE, COleDateTime::GetCurrentTime());
		record.Update();
		record.Close();
		tableDef.Close();
	}
	catch (CDaoException* e)
	{
		e->Delete();
		record.Close();
		tableDef.Close();
		delete pDatasetVector;
		pDatasetVector = NULL;
		try
		{
			m_pDataSource->m_daoDatabase.DeleteTableDef(vInfo.m_strName);
		}
		catch (CDaoException* e1)
		{
			e1->Delete();
		}
		return FALSE;
	}

	pDatasetVector->Open();
	m_arrSubDataset.Add(pDatasetVector);
	return TRUE;
}

}