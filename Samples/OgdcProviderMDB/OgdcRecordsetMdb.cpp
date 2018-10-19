//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)         
//                                                           
//!  \file OgdcRecordsetMdb.cpp
//!  \brief OGDC记录集的接口实现。
//!  \details OGDC记录集各种操作接口的实现
//!  \author yangchao。
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Limited Company.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#include "OgdcRecordsetMdb.h"
#include "OgdcHelperMdb.h"
#include "OgdcDataSourceMdb.h"
#include "OgdcMacroDefineMdb.h"
#include "OgdcDatasetVectorMdb.h"
#include "OgdcFeatureMdb.h"
#include "Element/OgdcElemPoint.h"
#include "Element/OgdcElemLine.h"
#include "Element/OgdcElemText.h"
#include "Element/OgdcElemPoint3D.h"
#include "Element/OgdcElemLine3D.h"
#include "Element/OgdcElemRegion3D.h"

#include "Geometry/UGGeometryManager.h"
#include "GeometryCAD/UGGeoPointEPS.h"
#include "GeometryCAD/UGGeoLineEPS.h"
#include "GeometryCAD/UGGeoRegionEPS.h"
#include "GeometryCAD/UGGeoTextEPS.h"
#include "Geometry3D/UGGeoModelEntity.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
namespace OGDC
{

OgdcRecordsetMdb::OgdcRecordsetMdb()
{
	m_nID = 0;
	m_bEdited = FALSE;
	m_pDataset = NULL;
	m_nRecordCount = 0;
	m_nEditMode = dbEditNone;
	m_bFirstAddNew = FALSE; 
	m_pElement = NULL;
	m_pDaoRecordset = NULL;
	m_pGeometry = NULL;
}

OgdcRecordsetMdb::OgdcRecordsetMdb( OgdcDatasetVectorMdb *pDataset )
{
	m_nID = 0;
	m_bEdited = FALSE;
	m_pDataset = pDataset;
	m_strTableName = pDataset->m_info.m_strName;
	m_nRecordCount = 0;
	m_nEditMode = dbEditNone;
	m_pElement = NULL;
	m_bFirstAddNew = FALSE; 
	m_pDaoRecordset = NULL;
	m_pGeometry = NULL;
}

OgdcRecordsetMdb::~OgdcRecordsetMdb()
{
	Close();
}

OgdcBool OgdcRecordsetMdb::IsEmpty()
{
	if (!m_pDaoRecordset->IsOpen())
	{
		return TRUE;
	}
	return	(m_pDaoRecordset->GetRecordCount()<=0);
}

OgdcBool OgdcRecordsetMdb::IsBOF()
{
	if (!m_pDaoRecordset->IsOpen())
		return FALSE;
	try
	{
		return m_pDaoRecordset->IsBOF ();
		
	}
	catch (CDaoException* e)
	{
		e->Delete();
		return FALSE;
	}	
}

OgdcBool OgdcRecordsetMdb::IsEOF()
{
	if (!m_pDaoRecordset->IsOpen())
		return FALSE;
	try
	{
		return m_pDaoRecordset->IsEOF();
		
	}
	catch (CDaoException* e)
	{
		e->Delete();
		return FALSE;
	}
}

void OgdcRecordsetMdb::Close()
{
	if(m_pDaoRecordset == NULL)
	{
		return;
	}

	if (m_pDaoRecordset->IsOpen())
	{
		try
		{
			if(m_bEdited && m_nEditMode != dbEditNone)
			{
				m_pDaoRecordset->Update ();
				//在不使用 AddNew 或 Edit 的情况下，更新 (Update) 
			}
			m_pDaoRecordset->Close();
		}
		catch (CDaoException* e)
		{
			e->Delete();
		}
	}
	
	delete m_pDaoRecordset;
	m_pDaoRecordset = NULL;

	m_bEdited = FALSE;
	m_nEditMode = dbEditNone;
	
	if (m_pElement != NULL)
	{
		//m_pElement 是当前编辑对象，由外部传入的对象指针
		m_pElement = NULL;
	}
	if (m_pGeometry != NULL)
	{
		m_pGeometry = NULL;
	}
}

OgdcBool OgdcRecordsetMdb::GetFieldValue( const OgdcString& strName, OgdcVariant& varVal )
{
 	if (!m_pDaoRecordset->IsOpen())
 	{
 		return FALSE;
 	}
	
	if (IsBOF())
	{
		return FALSE;
	}
	if (IsEOF())
	{
		return FALSE;
	}
	COleVariant cOleVar;
	try
	{
		m_pDaoRecordset->GetFieldValue(strName, cOleVar);
	}
	catch (CDaoException* e)
	{
		e->Delete();
		return FALSE;
	}
	OgdcHelperMdb::COleVariant2OgdcVariant(cOleVar, varVal);

	return TRUE;
}

OgdcBool OgdcRecordsetMdb::GetFieldValue( OgdcInt nIndex,OgdcVariant& varVal )
{
	if (!m_pDaoRecordset->IsOpen())
	{
		return FALSE;
	}
	//当前记录无效
    if (IsBOF())
	{
		return FALSE;
	}
	if (IsEOF())
	{
		return FALSE;
	}
	COleVariant cOleVar;
	try
	{
		m_pDaoRecordset->GetFieldValue(nIndex, cOleVar);
	}
	catch (CDaoException* e)
	{
		e->Delete();
		return FALSE;
	}
	
	OgdcHelperMdb::COleVariant2OgdcVariant(cOleVar, varVal);
	
	return TRUE;
}

OgdcBool OgdcRecordsetMdb::SetFieldValue( const OgdcString& strName, const OgdcVariant& varVal )
{
	if (OgdcHelperMdb::IsSystemField(strName))
		return FALSE;

	OgdcVariant myVarValue = varVal;
	COleVariant var;
	OgdcString strSQL;
	strSQL.Format(_U("UPDATE [%s] set %s = '%s' WHERE OGID = %d "),
		m_strTableName.Cstr(), strName.Cstr(),
		myVarValue.ToString().Cstr(), GetID());

	try
	{
		m_pDataset->m_pDataSource->m_daoDatabase.Execute(strSQL);
	}
	catch(CDaoException* e)
	{
		e->Delete();
		return FALSE;
	}
	
	return TRUE;
}

OgdcBool OgdcRecordsetMdb::SetFieldValue( OgdcInt nIndex,const OgdcVariant& varVal )
{
	if (!m_pDaoRecordset->IsOpen())
	{
		return FALSE;
	}
	
	if (!Edit())
	{
		return FALSE;
	}
	CDaoFieldInfo fieldInfo;
	m_pDaoRecordset->GetFieldInfo(nIndex,fieldInfo);
	OgdcVariant myVarValue = varVal;
	COleVariant var;
	try
	{
		OgdcHelperMdb::OgdcVariant2COleVariant(myVarValue, var);
		m_pDaoRecordset->SetFieldValue(nIndex, var);
		m_pDaoRecordset->Update();
	}
	catch (CDaoException* e)
	{
		e->Delete();
		return FALSE;
	}
	
	
	return TRUE;
}

OgdcBool OgdcRecordsetMdb::Find( const OgdcString& strFilter, OgdcRecordset::FindMode nFindMode /*= OgdcRecordset::First*/ )
{
	if (!m_pDaoRecordset->IsOpen())
	{
		return FALSE;
	}
	//移动时当前的AddNew或Edit取消
	CancelUpdate();
	//为了防止找不到，先纪录当前的位置以便恢复
	COleVariant varValue;
	try
	{
		m_pDaoRecordset->GetFieldValue(0, varValue);
	}
	catch (CDaoException* e)
	{
		e->Delete();
		return FALSE;
	}
	OgdcBool bResult;
	if (nFindMode == FindMode::First)
	{
		try
		{
			bResult = m_pDaoRecordset->FindFirst(strFilter);
			bResult = TRUE;
		}
		catch (CDaoException* e)
		{
			e->Delete();
			bResult = FALSE;
		}
	}
	else if (nFindMode==FindMode::Last)
	{
		try
		{
			bResult = m_pDaoRecordset->FindLast(strFilter);
			bResult = TRUE;
		}
		catch (CDaoException* e)
		{
			e->Delete();
			bResult =  FALSE;
		}
	}
	else if (nFindMode == FindMode::Next)
	{
		try
		{
			bResult = m_pDaoRecordset->FindNext(strFilter);
			bResult = TRUE;
		}
		catch (CDaoException* e)
		{
			e->Delete();
			bResult = FALSE;
		}
	}
	else if (nFindMode==FindMode::Prev)
	{
		try
		{
			bResult = m_pDaoRecordset->FindPrev(strFilter);
			bResult = TRUE;
		}
		catch (CDaoException* e)
		{
			e->Delete();
			bResult = FALSE;
		}
	}


	//如果找不到的话，恢复Recordset的指针
	if (!bResult) 
	{
		SeekID(varValue.lVal);
	}
	
	return bResult;
}

OgdcBool OgdcRecordsetMdb::Move( OgdcRecordset::MovePosition nFromPosition /*= OgdcRecordset::Current*/, 
	OgdcInt nRows /*= 1*/ )
{
	if( !m_pDaoRecordset->IsOpen())
	{
		return FALSE;
	}
	//当前记录集为空
	if (IsEmpty ())
	{
		return FALSE;
	}
		
	//移动时当前的AddNew或Edit取消
	CancelUpdate();
	
	//if (!m_pDaoRecordset->IsBOF() && !m_pDaoRecordset->IsEOF())
	try
	{
		if (nFromPosition == MovePosition::Begin)
		{
			m_pDaoRecordset->MoveFirst();
			if (nRows)
			{
				m_pDaoRecordset->Move(nRows);
			}
		}
		else if (nFromPosition == MovePosition::End)
		{
			m_pDaoRecordset->MoveLast();
			if (nRows)
			{
				//如果move负数，则是往反方向移动
				m_pDaoRecordset->Move(-nRows);
			}

		}
		else if (nFromPosition == MovePosition::Current)
		{
			m_pDaoRecordset->Move(nRows);	
		}
		else
		{
			return FALSE;
		}
		
	}
	catch (CDaoException* e)
	{
		e->Delete();
		return FALSE;
	}

	return TRUE;
}

OgdcInt OgdcRecordsetMdb::GetRecordCount()
{
// 	if( !m_pDaoRecordset->IsOpen())
// 	{
// 		return -1;
// 	}
// 	if (m_pDaoRecordset->IsEOF()&&m_pDaoRecordset->IsBOF())
// 	{
// 		return 0;
// 	}
// 	m_pDaoRecordset->MoveLast();
// 	m_nRecordCount = m_pDaoRecordset->GetRecordCount();
// 	m_pDaoRecordset->MoveFirst();
	return OgdcInt(m_nRecordCount);
}

void OgdcRecordsetMdb::SetRecordCount()
{
	if( !m_pDaoRecordset->IsOpen())
	{
		return;
	}
	if (m_pDaoRecordset->IsEOF()&&m_pDaoRecordset->IsBOF())
	{
		return;
	}
	m_pDaoRecordset->MoveLast();
	m_nRecordCount = m_pDaoRecordset->GetRecordCount();
	m_pDaoRecordset->MoveFirst();
}

OgdcBool OgdcRecordsetMdb::GetFieldInfos( OgdcFieldInfos& fieldInfos )
{
	if (!m_pDaoRecordset->IsOpen())
	{
		return FALSE;
	}
	
	fieldInfos.RemoveAll();
	
	OgdcInt m_nFields = m_pDaoRecordset->GetFieldCount();
	CDaoFieldInfo info;
	OgdcFieldInfo fieldInfo;
	fieldInfos.SetSize(m_nFields);
	for (OgdcInt i = 0; i < m_nFields; i++)
	{	
		try
		{
			m_pDaoRecordset->GetFieldInfo( OgdcInt(i), info);
			OgdcHelperMdb::CDaoFieldInfo2OgdcFieldInfo(info, fieldInfo);
		}
		catch (CDaoException* e)
		{
			e->Delete();
			return FALSE;
		}
		
		//OgdcHelperMdb::CDaoFieldInfo2OgdcFieldInfo(info, fieldInfo);
		OgdcHelperMdb::SetFieldSign(fieldInfo);
		fieldInfo.m_strForeignName = fieldInfo.m_strName;
		fieldInfos.SetAt(i, fieldInfo);
		fieldInfo.m_nFieldSign = OgdcFieldInfo::signNone;
		fieldInfo.m_bSystem = FALSE;
	}
	return TRUE;
}

OgdcBool OgdcRecordsetMdb::GetFieldInfo( const OgdcString& strName,OgdcFieldInfo& fieldInfo )
{
	if (!m_pDaoRecordset->IsOpen())
	{
		return FALSE;
	}
	CDaoFieldInfo info;
	try
	{
		if (strName.CompareNoCase(_U("SMID")) == 0)
		{
			m_pDaoRecordset->GetFieldInfo(_U("OGID"), info);
		}
		else
		{
			m_pDaoRecordset->GetFieldInfo(strName, info);
		}
		OgdcHelperMdb::CDaoFieldInfo2OgdcFieldInfo(info, fieldInfo);
		OgdcHelperMdb::SetFieldSign(fieldInfo);
	}
	catch (CDaoException* e)
	{
		e->Delete();
		return FALSE;
	}
	return TRUE;	
}

OgdcVariant OgdcRecordsetMdb::Statistic( const OgdcString& strFieldName, OgdcStatisticMode nMode )
{

	OgdcVariant ogdcVar;
	OgdcVariant errorVar = 0;
	
	OgdcString strOGFieldName = _U("OGID");
	if (strFieldName.CompareNoCase(_U("SMID")) == 0)
		strOGFieldName = _U("OGID");
	else
		strOGFieldName = strFieldName;

	OgdcFieldInfo fieldInfo;
	GetFieldInfo(strFieldName, fieldInfo);
	if (!(fieldInfo.m_nType == OgdcFieldInfo::INT16
		|| fieldInfo.m_nType == OgdcFieldInfo::INT32
		|| fieldInfo.m_nType == OgdcFieldInfo::INT64
		|| fieldInfo.m_nType == OgdcFieldInfo::Float
		|| fieldInfo.m_nType == OgdcFieldInfo::Double))
	{
		return errorVar;
	}

	OgdcString strSQL;
	switch(nMode)
	{
	case OgdcStatisticMode::smMax:
		if(!m_strFilter.IsEmpty ()) 
		{
			strSQL.Format(_U("SELECT Max(%s) FROM [%s] WHERE %s "),
				strOGFieldName.Cstr(), m_strTableName.Cstr(),m_strFilter.Cstr());
		}
		else
		{
			strSQL.Format(_U("SELECT Max(%s) FROM [%s] "),
				strOGFieldName.Cstr(), m_strTableName.Cstr());
		}
		break;
	case OgdcStatisticMode::smMin:
		if (!m_strFilter.IsEmpty ())
		{
			strSQL.Format(_U("SELECT MIN(%s) FROM [%s] WHERE %s "),
				strOGFieldName.Cstr(), m_strTableName.Cstr(),
				m_strFilter.Cstr());
		}
		else
		{
			strSQL.Format(_U("SELECT MIN(%s) FROM [%s] "), 
				strOGFieldName.Cstr(), m_strTableName.Cstr());
		}
		break;
	case OgdcStatisticMode::smAvg:
		if (!m_strFilter.IsEmpty ())
		{
			strSQL.Format(_U("SELECT AVG(%s) FROM [%s] WHERE %s"), 
				strOGFieldName.Cstr(), m_strTableName.Cstr(), 
				m_strFilter.Cstr());
		}
		else
		{
			strSQL.Format(_U("SELECT AVG(%s) FROM [%s] "), 
				strOGFieldName.Cstr(), m_strTableName.Cstr());
		}
		break;
	case OgdcStatisticMode::smSum:
		if (!m_strFilter.IsEmpty ())
		{
			strSQL.Format(_U("SELECT SUM(%s) FROM [%s] WHERE %s"),
				strOGFieldName.Cstr(), m_strTableName.Cstr(), 
				m_strFilter.Cstr());
		}
		else
		{
			strSQL.Format(_U("SELECT SUM(%s) FROM [%s] "),
				strOGFieldName.Cstr(), m_strTableName.Cstr());
		}
		break;
	case OgdcStatisticMode::smStdev:
		if (!m_strFilter.IsEmpty ())
		{
			strSQL.Format(_U("SELECT StDev(%s) FROM [%s] WHERE %s "), 
				strOGFieldName.Cstr(), m_strTableName.Cstr(),
				m_strFilter.Cstr());
		}
		else
		{
			strSQL.Format(_U("SELECT StDev(%s) FROM [%s] "), 
				strOGFieldName.Cstr(), m_strTableName.Cstr());
		}
		break;
	case OgdcStatisticMode::smVar:
		if (!m_strFilter.IsEmpty ())
		{
			strSQL.Format(_U("SELECT Var(%s) FROM [%s] WHERE %s "),
				strOGFieldName.Cstr(), m_strTableName.Cstr(),
				m_strFilter.Cstr());
		}
		else
		{
			strSQL.Format(_U("SELECT Var(%s) FROM [%s] "), 
				strOGFieldName.Cstr(), m_strTableName.Cstr());
		}
		break;
	default:
		return ogdcVar;
	}
	
	CDaoRecordset recordset(&(m_pDataset->m_pDataSource->m_daoDatabase));

	try
	{
		if (m_pDaoRecordset->IsOpen())
			m_pDaoRecordset->Close();
		m_pDaoRecordset->Open(dbOpenDynaset, strSQL);
		COleVariant varlue;
		m_pDaoRecordset->GetFieldValue (0, varlue);
		OgdcHelperMdb::COleVariant2OgdcVariant(varlue, ogdcVar);
		m_pDaoRecordset->Close();
	}
	catch (CDaoException* e)
	{
		e->Delete();
		m_pDaoRecordset->Close();
		return errorVar;
	}

	return ogdcVar;
}

OgdcBool OgdcRecordsetMdb::ReQuery()
{
	if (!m_pDaoRecordset->IsOpen())
		return	FALSE;
	
	else
	{
		try
		{
			m_pDaoRecordset->Requery();		
		}
		catch (CDaoException* e)
		{
			e->Delete();
			return FALSE;
		}
		
	}
	return TRUE;

}

OgdcBool OgdcRecordsetMdb::ComputeBounds( OgdcRect2D &rc2Bounds, OgdcDouble &dMinZ, OgdcDouble &dMaxZ )
{
	//初值
	rc2Bounds.SetEmpty();
	dMinZ = dMaxZ = 0;
	//条件
	if (!(m_pDataset)->IsOpen() 
		|| m_pDataset->m_info.m_nType == OgdcDataset::Tabular
		|| IsEmpty())
	{
		return FALSE;
	}
	
	OgdcDouble MinW, MaxE, MinS, MaxN;
	if (m_pDataset->GetType() == OgdcDataset::Point ||
		m_pDataset->GetType() == OgdcDataset::Point3D)
	{
		//如果是点，初始化bounds为点周围的距离10的矩形
		m_pDaoRecordset->MoveFirst();
		COleVariant varValue;
		m_pDaoRecordset->GetFieldValue(OG_X, varValue);
		MinW = varValue.dblVal - 10;
		m_pDaoRecordset->GetFieldValue(OG_Y, varValue);
		MaxN = varValue.dblVal + 10;
		m_pDaoRecordset->GetFieldValue(OG_X, varValue);
		MaxE = varValue.dblVal + 10;
		m_pDaoRecordset->GetFieldValue(OG_Y, varValue);
		MinS = varValue.dblVal - 10;
		
		while (!m_pDaoRecordset->IsEOF())
		{
			m_pDaoRecordset->GetFieldValue(OG_X, varValue);
			MinW = varValue.dblVal < MinW ? varValue.dblVal : MinW;
			m_pDaoRecordset->GetFieldValue(OG_Y, varValue);
			MaxN = varValue.dblVal > MaxN ? varValue.dblVal : MaxN;
			m_pDaoRecordset->GetFieldValue(OG_X, varValue);
			MaxE = varValue.dblVal > MaxE ? varValue.dblVal : MaxE;
			m_pDaoRecordset->GetFieldValue(OG_Y, varValue);
			MinS = varValue.dblVal < MinS ? varValue.dblVal : MinS;
			m_pDaoRecordset->MoveNext();
		}
		m_pDaoRecordset->MoveFirst();
	}
	else
	{
		m_pDaoRecordset->MoveFirst();
		COleVariant varValue;
		m_pDaoRecordset->GetFieldValue(OG_SDRI_W, varValue);
		MinW = varValue.dblVal;
		m_pDaoRecordset->GetFieldValue(OG_SDRI_N, varValue);
		MaxN = varValue.dblVal;
		m_pDaoRecordset->GetFieldValue(OG_SDRI_E, varValue);
		MaxE = varValue.dblVal;
		m_pDaoRecordset->GetFieldValue(OG_SDRI_S, varValue);
		MinS = varValue.dblVal;
		
		while (!m_pDaoRecordset->IsEOF())
		{
			m_pDaoRecordset->GetFieldValue(OG_SDRI_W, varValue);
			MinW = varValue.dblVal < MinW ? varValue.dblVal : MinW;
			m_pDaoRecordset->GetFieldValue(OG_SDRI_N, varValue);
			MaxN = varValue.dblVal > MaxN ? varValue.dblVal : MaxN;
			m_pDaoRecordset->GetFieldValue(OG_SDRI_E, varValue);
			MaxE = varValue.dblVal > MaxE ? varValue.dblVal : MaxE;
			m_pDaoRecordset->GetFieldValue(OG_SDRI_S, varValue);
			MinS = varValue.dblVal < MinS ? varValue.dblVal : MinS;
			m_pDaoRecordset->MoveNext();
		}
		m_pDaoRecordset->MoveFirst();
	}

	rc2Bounds.left = MinW;
	rc2Bounds.right = MaxE;
	rc2Bounds.top = MaxN;
	rc2Bounds.bottom = MinS;
	return TRUE;
}

OgdcBool OgdcRecordsetMdb::CopyField( const OgdcString& strSrcExpression, const OgdcString& strDestFieldName )
{
	if (!m_pDaoRecordset->IsOpen())
	{
		//请打开先
		return FALSE;
	}
	OgdcFieldInfo fiSrc,fiDest;
	CDaoFieldInfo daoFirst,daoEest;
	try
	{
		//判断字段是否存在
		m_pDaoRecordset->GetFieldInfo(strSrcExpression, daoFirst, AFX_DAO_ALL_INFO);
		m_pDaoRecordset->GetFieldInfo(strDestFieldName, daoEest, AFX_DAO_ALL_INFO);
		if (daoFirst.m_strName != strSrcExpression || daoEest.m_strName != strDestFieldName)
		{
			return FALSE;
		}
	}
	catch (CDaoException* e)
	{
		e->Delete();
		return FALSE;
	}
	
	OgdcString strSQL;

	if (m_strFilter.IsEmpty())
	{
		strSQL.Format(_U("UPDATE %s SET %s = %s"),
			m_strTableName.Cstr(), strDestFieldName.Cstr(),
			strSrcExpression.Cstr());
	}
	else
	{	
		strSQL.Format(_U("UPDATE %s SET %s = %s WHERE %s"), 
			m_strTableName.Cstr(), strDestFieldName.Cstr(),
			strSrcExpression.Cstr(),m_strFilter.Cstr());
	}
	try
	{
		m_pDataset->m_pDataSource->m_daoDatabase.Execute (strSQL);
	}
	catch (CDaoException* e)
	{
		e->Delete();
		return FALSE;
	}
	return TRUE;
}

OgdcInt OgdcRecordsetMdb::GetID()
{
	if( !m_pDaoRecordset->IsOpen())
	{
		//请打开先
		return -1;
	}

	COleVariant varValue;
	try
	{
		m_pDaoRecordset->GetFieldValue(0, varValue);
		m_nID = varValue.lVal;
	}
	catch (CDaoException* e)
	{
		e->Delete();
		return 0;
	}

	return m_nID;	
}

OgdcBool OgdcRecordsetMdb::GetElement( OgdcElement*& pElement )
{
	if (IsBOF())
	{
		return FALSE;
	}
	if (m_pDataset->GetType() == OgdcDataset::Tabular)
	{
		return FALSE;
	}

	//得到当前的ID
	OgdcInt nID = GetID();
	if (nID == 0) 
	{
		return FALSE;
	}

	COleVariant var;

	//从数据库里取二进制数据
	COleVariant varArray;
	BSTR HUGEP *pbstr = NULL;
	long lLBound,lUBound;
	OgdcByte *pData = NULL;
	OgdcByte *pDataMirror = NULL;

	if (m_pDataset->GetType() != OgdcDataset::Point &&
		m_pDataset->GetType() != OgdcDataset::Point3D)
	{
		m_pDaoRecordset->GetFieldValue(OG_GEOMETRY,varArray);
		
		if (FAILED(SafeArrayGetLBound(varArray.parray, 1, &lLBound)))
		{
			return FALSE;
		}
		if (FAILED(SafeArrayGetUBound(varArray.parray, 1, &lUBound)))
		{
			return FALSE;
		}
		if (FAILED(SafeArrayAccessData(varArray.parray, (void HUGEP* FAR*)&pbstr)))
		{
			return FALSE;
		}
		
		long dataSize = lUBound - lLBound + 1;
		pData = new BYTE[dataSize];
		memcpy( pData, pbstr, dataSize);

		SafeArrayUnaccessData(varArray.parray);

		pDataMirror = pData;
	}

	OgdcInt i = 0;
	switch (m_pDataset->GetType())
	{
	case OgdcDataset::Point:
		{
			if (NULL == pElement || pElement->GetType() != OgdcElement::ElemPoint)
			{
				delete pElement;
				pElement = new OgdcElemPoint();
			}
			m_pDaoRecordset->GetFieldValue(OG_OBJ_ID, var);
			((OgdcElemPoint*)pElement)->m_nID = var.lVal;
			m_pDaoRecordset->GetFieldValue(OG_X, var);
			((OgdcElemPoint*)pElement)->m_point.x = var.dblVal;
			m_pDaoRecordset->GetFieldValue(OG_Y, var);
			((OgdcElemPoint*)pElement)->m_point.y = var.dblVal;
			pElement->m_nID = nID;
		}
		break;
	case OgdcDataset::Point3D:
		{
			if (NULL == pElement || pElement->GetType() != OgdcElement::ElemPoint3D)
			{
				delete pElement;
				pElement = new OgdcElemPoint3D();
			}
			m_pDaoRecordset->GetFieldValue(OG_OBJ_ID, var);
			((OgdcElemPoint3D*)pElement)->m_nID = var.lVal;
			m_pDaoRecordset->GetFieldValue(OG_X, var);
			((OgdcElemPoint3D*)pElement)->m_point.x = var.dblVal;
			m_pDaoRecordset->GetFieldValue(OG_Y, var);
			((OgdcElemPoint3D*)pElement)->m_point.y = var.dblVal;
			m_pDaoRecordset->GetFieldValue(OG_Z, var);
			((OgdcElemPoint3D*)pElement)->m_point.z = var.dblVal;
			pElement->m_nID = nID;
		}
		break;
	case OgdcDataset::Line:
		{
			if (NULL == pElement || m_pDataset->GetType() != OgdcElement::ElemLine)
			{
				delete pElement;
				pElement = new OgdcElemLine();
			}

			OgdcInt nPointCount=0, nSubCount=0;
			
			((OgdcElemLine*)pElement)->m_nID = nID;
			m_pDaoRecordset->GetFieldValue(OG_SDRI_W, var);
			((OgdcElemLine*)pElement)->m_rcBounds.left = var.dblVal;
			m_pDaoRecordset->GetFieldValue(OG_SDRI_N, var);
			((OgdcElemLine*)pElement)->m_rcBounds.top = var.dblVal;
			m_pDaoRecordset->GetFieldValue(OG_SDRI_E, var);
			((OgdcElemLine*)pElement)->m_rcBounds.right = var.dblVal;
			m_pDaoRecordset->GetFieldValue(OG_SDRI_S, var);
			((OgdcElemLine*)pElement)->m_rcBounds.bottom = var.dblVal;
			
			memcpy(&nPointCount, pDataMirror, sizeof(OgdcInt));
			pDataMirror += sizeof(OgdcInt);
			memcpy(&nSubCount, pDataMirror, sizeof(OgdcInt));
			pDataMirror += sizeof(OgdcInt);
			((OgdcElemLine*)pElement)->m_polyCounts.SetSize(nSubCount);
			((OgdcElemLine*)pElement)->m_points.SetSize(nPointCount);	
			for (i = 0; i< nSubCount; i++)
			{
				memcpy(&((OgdcElemLine*)pElement)->m_polyCounts[i], pDataMirror, 
					sizeof(OgdcInt));
				pDataMirror += sizeof(OgdcInt);
			}
			for (i = 0; i < nPointCount; i++)
			{
				memcpy(&((OgdcElemLine*)pElement)->m_points[i].x, pDataMirror, 
					sizeof(OgdcDouble));
				pDataMirror += sizeof(OgdcDouble);
				memcpy(&((OgdcElemLine*)pElement)->m_points[i].y, pDataMirror, 
					sizeof(OgdcDouble));
				pDataMirror += sizeof(OgdcDouble);
			}
		}
		break;
	case OgdcDataset::Line3D:
		{
			if (NULL == pElement || m_pDataset->GetType() != OgdcElement::ElemLine3D)
			{
				delete pElement;
				pElement = new OgdcElemLine3D();
			}

			OgdcInt nPointCount=0, nSubCount=0;

			((OgdcElemLine3D*)pElement)->m_nID = nID;
			m_pDaoRecordset->GetFieldValue(OG_SDRI_W, var);
			((OgdcElemLine3D*)pElement)->m_rcBounds.left = var.dblVal;
			m_pDaoRecordset->GetFieldValue(OG_SDRI_N, var);
			((OgdcElemLine3D*)pElement)->m_rcBounds.top = var.dblVal;
			m_pDaoRecordset->GetFieldValue(OG_SDRI_E, var);
			((OgdcElemLine3D*)pElement)->m_rcBounds.right = var.dblVal;
			m_pDaoRecordset->GetFieldValue(OG_SDRI_S, var);
			((OgdcElemLine3D*)pElement)->m_rcBounds.bottom = var.dblVal;

			memcpy(&nPointCount, pDataMirror, sizeof(OgdcInt));
			pDataMirror += sizeof(OgdcInt);
			memcpy(&nSubCount, pDataMirror, sizeof(OgdcInt));
			pDataMirror += sizeof(OgdcInt);
			((OgdcElemLine3D*)pElement)->m_polyCounts.SetSize(nSubCount);
			((OgdcElemLine3D*)pElement)->m_points.SetSize(nPointCount);
			for (i = 0; i< nSubCount; i++)
			{
				memcpy(&((OgdcElemLine3D*)pElement)->m_polyCounts[i], pDataMirror, 
					sizeof(OgdcInt));
				pDataMirror += sizeof(OgdcInt);
			}
			for (i = 0; i < nPointCount; i++)
			{
				memcpy(&((OgdcElemLine3D*)pElement)->m_points[i].x, pDataMirror, 
					sizeof(OgdcDouble));
				pDataMirror += sizeof(OgdcDouble);
				memcpy(&((OgdcElemLine3D*)pElement)->m_points[i].y, pDataMirror, 
					sizeof(OgdcDouble));
				pDataMirror += sizeof(OgdcDouble);
				memcpy(&((OgdcElemLine3D*)pElement)->m_points[i].z, pDataMirror, 
					sizeof(OgdcDouble));
				pDataMirror += sizeof(OgdcDouble);
			}
		}
		break;
	case OgdcDataset::Region:
		{
			if (NULL == pElement || m_pDataset->GetType() != OgdcElement::ElemRegion)
			{
				delete pElement;
				pElement = new OgdcElemRegion;
			}

			OgdcInt nPointCount=0, nSubCount=0;

			((OgdcElemRegion*)pElement)->m_nID = nID;
			m_pDaoRecordset->GetFieldValue(OG_SDRI_W, var);
			((OgdcElemRegion*)pElement)->m_rcBounds.left = var.dblVal;
			m_pDaoRecordset->GetFieldValue(OG_SDRI_N, var);
			((OgdcElemRegion*)pElement)->m_rcBounds.top = var.dblVal;
			m_pDaoRecordset->GetFieldValue(OG_SDRI_E, var);
			((OgdcElemRegion*)pElement)->m_rcBounds.right = var.dblVal;
			m_pDaoRecordset->GetFieldValue(OG_SDRI_S, var);
			((OgdcElemRegion*)pElement)->m_rcBounds.bottom = var.dblVal;
			
			memcpy(&nPointCount, pDataMirror, sizeof(OgdcInt));
			pDataMirror += sizeof(OgdcInt);
			memcpy(&nSubCount, pDataMirror, sizeof(OgdcInt));
			pDataMirror += sizeof(OgdcInt);
			((OgdcElemRegion*)pElement)->m_polyCounts.SetSize(nSubCount);
			((OgdcElemRegion*)pElement)->m_points.SetSize(nPointCount);	
			for(i = 0; i < nSubCount; i++)
			{
				memcpy(&((OgdcElemRegion*)pElement)->m_polyCounts[i], pDataMirror, 
					sizeof(OgdcInt));
				pDataMirror += sizeof(OgdcInt);
			}
			for (i = 0; i < nPointCount; i++)
			{
				memcpy(&((OgdcElemRegion*)pElement)->m_points[i].x, pDataMirror, 
					sizeof(OgdcDouble));
				pDataMirror += sizeof(OgdcDouble);
				memcpy(&((OgdcElemRegion *)pElement)->m_points[i].y, pDataMirror, 
					sizeof(OgdcDouble));
				pDataMirror += sizeof(OgdcDouble);
			}
		}
		break;
	case OgdcDataset::Region3D:
		{
			if (NULL == pElement || m_pDataset->GetType() != OgdcElement::ElemRegion3D)
			{
				delete pElement;
				pElement = new OgdcElemRegion3D();
			}

			OgdcInt nPointCount=0, nSubCount=0;

			((OgdcElemRegion3D*)pElement)->m_nID = nID;
			m_pDaoRecordset->GetFieldValue(OG_SDRI_W, var);
			((OgdcElemRegion3D*)pElement)->m_rcBounds.left = var.dblVal;
			m_pDaoRecordset->GetFieldValue(OG_SDRI_N, var);
			((OgdcElemRegion3D*)pElement)->m_rcBounds.top = var.dblVal;
			m_pDaoRecordset->GetFieldValue(OG_SDRI_E, var);
			((OgdcElemRegion3D*)pElement)->m_rcBounds.right = var.dblVal;
			m_pDaoRecordset->GetFieldValue(OG_SDRI_S, var);
			((OgdcElemRegion3D*)pElement)->m_rcBounds.bottom = var.dblVal;

			memcpy(&nPointCount, pDataMirror, sizeof(OgdcInt));
			pDataMirror += sizeof(OgdcInt);
			memcpy(&nSubCount, pDataMirror, sizeof(OgdcInt));
			pDataMirror += sizeof(OgdcInt);
			((OgdcElemRegion3D*)pElement)->m_polyCounts.SetSize(nSubCount);
			((OgdcElemRegion3D*)pElement)->m_points.SetSize(nPointCount);	
			for(i = 0; i < nSubCount; i++)
			{
				memcpy(&((OgdcElemRegion3D*)pElement)->m_polyCounts[i], pDataMirror, 
					sizeof(OgdcInt));
				pDataMirror += sizeof(OgdcInt);
			}
			for (i = 0; i < nPointCount; i++)
			{
				memcpy(&((OgdcElemRegion3D*)pElement)->m_points[i].x, pDataMirror, 
					sizeof(OgdcDouble));
				pDataMirror += sizeof(OgdcDouble);
				memcpy(&((OgdcElemRegion3D*)pElement)->m_points[i].y, pDataMirror, 
					sizeof(OgdcDouble));
				pDataMirror += sizeof(OgdcDouble);
				memcpy(&((OgdcElemRegion3D*)pElement)->m_points[i].z, pDataMirror, 
					sizeof(OgdcDouble));
				pDataMirror += sizeof(OgdcDouble);
			}
		}
		break;
	case OgdcDataset::Text:
		{
		
			OgdcElemText *pElementText = new OgdcElemText();
			
			((OgdcElemText*)pElementText)->m_nID = nID;
			m_pDaoRecordset->GetFieldValue(OG_SDRI_W, var);
			((OgdcElemText*)pElementText)->m_rcBounds.left = var.dblVal;
			m_pDaoRecordset->GetFieldValue(OG_SDRI_N, var);
			((OgdcElemText*)pElementText)->m_rcBounds.top = var.dblVal;
			m_pDaoRecordset->GetFieldValue(OG_SDRI_E, var);
			((OgdcElemText*)pElementText)->m_rcBounds.right = var.dblVal;
	 		m_pDaoRecordset->GetFieldValue(OG_SDRI_S, var);
	 		((OgdcElemText*)pElementText)->m_rcBounds.bottom = var.dblVal;
			OgdcInt i = 0;
			//{{取数字标志
			//记录点的个数
			OgdcInt nPointCount;
			memcpy( &nPointCount, pDataMirror, 4 );
			pDataMirror += 4;
			//记录子文本的个数，Angles的个数即是子对象个数
			OgdcInt nStrCount;
			memcpy( &nStrCount, pDataMirror, 4 );
			pDataMirror += 4;
			//记录每个子文本的长度
			OgdcArray<OgdcInt> nStrLength;
			nStrLength.SetSize(nStrCount);
			//获取每个子文本的长度
			for (i = 0; i < nStrCount; i++)
			{
				memcpy( &nStrLength.ElementAt(i), pDataMirror, 4 );
				pDataMirror += 4;
			}
			//记录文本风格中，字体名字的长度
			OgdcInt nStrFaceNameLength;
			memcpy( &nStrFaceNameLength, pDataMirror, 4 );
			pDataMirror += 4;
			//}}取数字标志

			//以下才是取文本风格的参数
			((OgdcElemText*)pElementText)->m_points.SetSize(nPointCount);
			for (i = 0; i < nPointCount; i++)
			{
				memcpy(&((OgdcElemText*)pElementText)->m_points[i].x, pDataMirror, 
					sizeof(OgdcDouble));
				pDataMirror += sizeof(OgdcDouble);

				memcpy(&((OgdcElemText*)pElementText)->m_points[i].y, pDataMirror, 
					sizeof(OgdcDouble));
				pDataMirror += sizeof(OgdcDouble);
			}

			((OgdcElemText*)pElementText)->m_strTexts.SetSize(nStrCount);
			OgdcString *str = new OgdcString;
			OgdcChar* ch = new OgdcChar;
			for (i = 0; i < nStrCount; i++)
			{
				((OgdcElemText*)pElementText)->m_strTexts[i].SetLength(nStrLength.GetAt(i));
				for (OgdcInt j = 0; j < nStrLength.GetAt(i); j++)
				{
					memcpy(ch, pDataMirror ,sizeof(OgdcChar));
					pDataMirror += sizeof(OgdcChar);
					((OgdcElemText*)pElementText)->m_strTexts[i].SetAt(j, *ch);
				}
			}

			delete str;
			str = NULL;

			((OgdcElemText*)pElementText)->m_angles.SetSize(nStrCount);
			OgdcDouble *pDouble=new OgdcDouble;
			for (i = 0; i < nStrCount; i++)
			{
			
				memcpy(pDouble, pDataMirror, sizeof(OgdcDouble));
				((OgdcElemText*)pElementText)->m_angles.SetAt(i, *pDouble);
				pDataMirror += sizeof(OgdcDouble);
			}
			delete pDouble;
			pDouble = NULL;
			////////////////
			//1
			memcpy(&((OgdcElemText*)pElementText)->m_textStyle.m_clrFore, pDataMirror, 4);
			pDataMirror += 4;
			//2
			memcpy(&((OgdcElemText*)pElementText)->m_textStyle.m_clrBack, pDataMirror, 4);
			pDataMirror += 4;
			//3
			memcpy(&((OgdcElemText*)pElementText)->m_textStyle.m_dAngle, pDataMirror, 8);
			pDataMirror += 8;
			//4
			memcpy(&((OgdcElemText*)pElementText)->m_textStyle.m_dItalicAngle, pDataMirror, 8);
			pDataMirror += 8;
			//5
			memcpy(&((OgdcElemText*)pElementText)->m_textStyle.m_dWidth, pDataMirror, 8);
			pDataMirror += 8;
			//6
			memcpy(&((OgdcElemText*)pElementText)->m_textStyle.m_dHeight, pDataMirror, 8);
			pDataMirror += 8;
			//7
			memcpy(&((OgdcElemText*)pElementText)->m_textStyle.m_nWeight, pDataMirror, 4);
			pDataMirror += 4;
			//8
			memcpy(&((OgdcElemText*)pElementText)->m_textStyle.m_nStyle, pDataMirror, 4);
			pDataMirror += 4;
			//9
			memcpy(&((OgdcElemText*)pElementText)->m_textStyle.m_nAlign, pDataMirror, 1);
			pDataMirror += 1;
			//10
			memcpy(&((OgdcElemText*)pElementText)->m_textStyle.m_bBold, pDataMirror, 1);
			pDataMirror += 1;
			//11
			memcpy(&((OgdcElemText*)pElementText)->m_textStyle.m_bUnderline, pDataMirror, 1);
			pDataMirror += 1;
			//12
			memcpy(&((OgdcElemText*)pElementText)->m_textStyle.m_bItalic, pDataMirror, 1);
			pDataMirror += 1;
			//13
			memcpy(&((OgdcElemText*)pElementText)->m_textStyle.m_bStrikeOut, pDataMirror, 1);
			pDataMirror += 1;
			//14
			memcpy(&((OgdcElemText*)pElementText)->m_textStyle.m_bFixedSize, pDataMirror, 1);
			pDataMirror += 1;
			//15
			memcpy(&((OgdcElemText*)pElementText)->m_textStyle.m_bBackOpaque, pDataMirror, 1);
			pDataMirror += 1;
			//16
			memcpy(&((OgdcElemText*)pElementText)->m_textStyle.m_bHalo, pDataMirror, 1);
			pDataMirror += 1;
			//17
			memcpy(&((OgdcElemText*)pElementText)->m_textStyle.m_bShadow, pDataMirror, 1);
			pDataMirror += 1;
			//18
			((OgdcElemText*)pElementText)->m_textStyle.m_strFaceName.SetLength(nStrFaceNameLength);
			for (i = 0; i < nStrFaceNameLength; i++)
			{				
				memcpy(ch, pDataMirror , sizeof(OgdcChar));
 				pDataMirror +=  sizeof(OgdcChar);
				((OgdcElemText*)pElementText)->m_textStyle.m_strFaceName.SetAt(i, *ch);
			}
			delete ch;
			ch = NULL;
			///////////////////////
			pElement = pElementText;
			pElementText = NULL;
		}
		break;
	default:
		return FALSE;
		break;
	}

	if (m_pDataset->GetType() != OgdcDataset::Point &&
		m_pDataset->GetType() != OgdcDataset::Point3D)
	{
		delete[] pData;
		pData = NULL;
	}

	return TRUE;	
}

OgdcBool OgdcRecordsetMdb::GetElement(UGC::UGGeometry*& pGeometry)
{
	if (IsBOF())
	{
		return FALSE;
	}
	if (m_pDataset->GetType() != OgdcDataset::PointEPS &&
		m_pDataset->GetType() != OgdcDataset::LineEPS &&
		m_pDataset->GetType() != OgdcDataset::RegionEPS &&
		m_pDataset->GetType() != OgdcDataset::TextEPS &&
		m_pDataset->GetType() != OgdcDataset::Model &&
		m_pDataset->GetType() != OgdcDataset::Texture)
	{
		OgdcElement *pElement = NULL;
		if (GetElement(pElement) && pElement != NULL)
		{
			if (pGeometry != NULL && pGeometry->GetType() != pElement->GetType())
			{
				delete pGeometry;
				pGeometry = NULL;
			}
			if (pGeometry == NULL)
			{
				pGeometry = UGC::UGGeometryManager::CreateGeometry(pElement->GetType());
			}
			if (pGeometry->FromElement(pElement))
			{
				delete pElement;
				pElement = NULL;
				return TRUE;
			}
			delete pGeometry;
			pGeometry = NULL;
		}
		delete pElement;
		pElement = NULL;
		return FALSE;
	}

	//得到当前的ID
	OgdcInt nID = GetID();
	if (nID == 0) 
	{
		return FALSE;
	}

	//从数据库里取二进制数据
	COleVariant varArray;
	m_pDaoRecordset->GetFieldValue(OG_GEOMETRY,varArray);
	long lLBound,lUBound;
	BSTR HUGEP *pbstr = NULL;
	if (FAILED(SafeArrayGetLBound(varArray.parray, 1, &lLBound)))
	{
		return FALSE;
	}
	if (FAILED(SafeArrayGetUBound(varArray.parray, 1, &lUBound)))
	{
		return FALSE;
	}
	if (FAILED(SafeArrayAccessData(varArray.parray, (void HUGEP* FAR*)&pbstr)))
	{
		return FALSE;
	}
	long dataSize = lUBound - lLBound + 1;
	OgdcByte *pData = new BYTE[dataSize];
	memcpy(pData, pbstr, dataSize);
	SafeArrayUnaccessData(varArray.parray);

	UGC::UGMemoryStream dataStream;
	dataStream.Open(UGC::UGStreamLoad, dataSize, pData);

	switch (m_pDataset->GetType())
	{
	case OgdcDataset::PointEPS:
		{
			if (NULL == pGeometry || pGeometry->GetType() != UGC::UGGeometry::GeoPointEPS)
			{
				delete pGeometry;
				pGeometry = new UGC::UGGeoPointEPS();
			}
		}
		break;
	case OgdcDataset::LineEPS:
		{
			if (NULL == pGeometry || pGeometry->GetType() != UGC::UGGeometry::GeoLineEPS)
			{
				delete pGeometry;
				pGeometry = new UGC::UGGeoLineEPS();
			}
		}
		break;
	case OgdcDataset::RegionEPS:
		{
			if (NULL == pGeometry || pGeometry->GetType() != UGC::UGGeometry::GeoRegionEPS)
			{
				delete pGeometry;
				pGeometry = new UGC::UGGeoRegionEPS();
			}
		}
		break;
	case OgdcDataset::TextEPS:
		{
			if (NULL == pGeometry || pGeometry->GetType() != UGC::UGGeometry::GeoTextEPS)
			{
				delete pGeometry;
				pGeometry = new UGC::UGGeoTextEPS();
			}
		}
		break;
	case OgdcDataset::Model:
	case OgdcDataset::Texture:
		{
			OgdcInt nGeoType = 0;
			dataStream >> nGeoType;
			if (NULL == pGeometry || pGeometry->GetType() != nGeoType)
			{
				delete pGeometry;
				pGeometry = UGC::UGGeometryManager::CreateGeometry((UGC::UGGeometry::Type)nGeoType);
			}
			if (NULL == pGeometry)
			{
				delete[] pData;
				pData = NULL;
				dataStream.Close();
				return FALSE;
			}
		}
		break;
	default:
		{
			delete[] pData;
			pData = NULL;
			dataStream.Close();
		}
		return FALSE;
	}

	OgdcBool bResult = FALSE;
	switch (m_pDataset->GetType())
	{
	case OgdcDataset::Texture:
		{
			bResult = pGeometry->Load(dataStream);
		}
		break;
	default:
		{
			if (m_pDataset->GetType() == OgdcDataset::Model)
			{
				bResult = pGeometry->Load(dataStream);
			}
			else
			{
				bResult = pGeometry->LoadGeoData(dataStream,UGC::UGDataCodec::encNONE);
			}
			if (bResult)
			{
				UGC::UGRect2D boundRect2D;
				COleVariant var;
				m_pDaoRecordset->GetFieldValue(OG_SDRI_W, var);
				boundRect2D.left = var.dblVal;
				m_pDaoRecordset->GetFieldValue(OG_SDRI_N, var);
				boundRect2D.top = var.dblVal;
				m_pDaoRecordset->GetFieldValue(OG_SDRI_E, var);
				boundRect2D.right = var.dblVal;
				m_pDaoRecordset->GetFieldValue(OG_SDRI_S, var);
				boundRect2D.bottom = var.dblVal;
				pGeometry->SetBounds(boundRect2D);
			}
		}
		break;
	}

	delete[] pData;
	pData = NULL;
	dataStream.Close();

	if (!bResult)
	{
		delete pGeometry;
		pGeometry = NULL;
		return FALSE;
	}
	pGeometry->SetID(nID);
	return TRUE;
}

OgdcBool OgdcRecordsetMdb::SetElement( OgdcElement* pElement )
{	
	if (!Edit())
	{
		return FALSE;
	}
	if (m_pDataset->GetType() == OgdcDataset::Point)
	{
		try
		{
			m_pDaoRecordset->SetFieldValue (OG_X, ((OgdcElemPoint *)pElement)->m_point.x);
			m_pDaoRecordset->SetFieldValue (OG_Y, ((OgdcElemPoint *)pElement)->m_point.y);
			m_pDaoRecordset->Update();
		}
		catch (CDaoException* e)
		{
			e->Delete();
			return FALSE;
		}		
	}
	else if (m_pDataset->GetType() == OgdcDataset::Point3D)
	{
		try
		{
			m_pDaoRecordset->SetFieldValue (OG_X, ((OgdcElemPoint3D *)pElement)->m_point.x);
			m_pDaoRecordset->SetFieldValue (OG_Y, ((OgdcElemPoint3D *)pElement)->m_point.y);
			m_pDaoRecordset->SetFieldValue (OG_Z, ((OgdcElemPoint3D *)pElement)->m_point.z);
			m_pDaoRecordset->Update();
		}
		catch (CDaoException* e)
		{
			e->Delete();
			return FALSE;
		}
	}
	else
	{
		
		OgdcRect2D bound = pElement->GetBounds();
		double dGranule = (bound.Width() > bound.Height() ? bound.Width() : bound.Height());
		CByteArray arr;

		if (m_pDataset->GetType() == OgdcDataset::Text)
		{
			OgdcInt i = 0;
			//记录点的个数
			OgdcInt nPointCount = ((OgdcElemText*)pElement)->m_points.GetSize();
			//记录子文本的个数，Angles的个数即是子对象个数
			OgdcInt nStrCount = ((OgdcElemText*)pElement)->m_strTexts.GetSize();
			//记录每个子文本的长度
			//OgdcInt nStrLength[nStrCount];
			OgdcArray<OgdcInt> nStrLength;
			nStrLength.SetSize(nStrCount);
			//记录文本风格中，字体名字的长度
			OgdcInt nStrFaceNameLength = ((OgdcElemText*)pElement)->m_textStyle .m_strFaceName.GetLength();
			//获取每个子文本的长度
			for (i = 0; i < nStrCount; i++)
			{
				nStrLength[i]=((OgdcElemText*)pElement)->m_strTexts.GetAt(i).GetLength();
			}
			//所有Count的字节数
			OgdcInt lSize = 4 + 4 + 4 * nStrCount + 4;
			//所有点的字节数
			lSize += nPointCount * 16;
			//所有子文本的长度
			for (i = 0; i < nStrCount; i++)
			{
				lSize += nStrLength[i];
			}
			//所有angele的字节数
			lSize += nPointCount * 8;
			//文本风格中，字体名字的字节数
			lSize += nStrFaceNameLength;
			//文本风格中，其他参数的字节数
			lSize += 57;
			
			BYTE *pData = new BYTE[lSize];
			BYTE *pDataMirror = pData;
			//{{存数字标志
			memcpy(pDataMirror, &nPointCount, 4);
			pDataMirror += 4;
			memcpy(pDataMirror, &nStrCount, 4);
			pDataMirror += 4;
			for (i = 0; i < nStrCount; i++)
			{
				OgdcInt *pOgdcInt = new OgdcInt;
				*pOgdcInt = nStrLength.GetAt(i);
				memcpy(pDataMirror, pOgdcInt, 4);
				pDataMirror += 4;
				delete pOgdcInt;
				pOgdcInt = NULL;
			}
			memcpy(pDataMirror, &nStrFaceNameLength, 4);
			pDataMirror += 4;
			//}}存数字标志

			//以下才是存文本风格的参数
			for (i = 0; i < nPointCount; i++)
			{
				memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_points[i].x,
					sizeof(OgdcDouble));
				pDataMirror += sizeof(OgdcDouble);
				memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_points[i].y,
					sizeof(OgdcDouble));
				pDataMirror += sizeof(OgdcDouble);
			}
			for (i = 0; i < nStrCount; i++)
			{
				for (OgdcInt j = 0; j < nStrLength.GetAt(i); j++)
				{
					OgdcChar ch = ((OgdcElemText*)pElement)->m_strTexts[i].GetAt(j);
					memcpy(pDataMirror, &ch, 1);
					pDataMirror += 1;
				}
			}
			for (i = 0; i < nStrCount; i++)
			{
				OgdcDouble *pDouble = new OgdcDouble;
				*pDouble = ((OgdcElemText*)pElement)->m_angles.GetAt(i);
				memcpy(pDataMirror, pDouble, sizeof(OgdcDouble));
				pDataMirror += sizeof(OgdcDouble);
				delete pDouble;
				pDouble = NULL;
			}
			////////////////
			//1
			memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_textStyle.m_clrFore, 4);
			pDataMirror += 4;
			//2
			memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_textStyle.m_clrBack, 4);
			pDataMirror += 4;
			//3
			memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_textStyle.m_dAngle, 8);
			pDataMirror += 8;
			//4
			memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_textStyle.m_dItalicAngle, 8);
			pDataMirror += 8;
			//5
			memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_textStyle.m_dWidth, 8);
			pDataMirror += 8;
			//6
			memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_textStyle.m_dHeight, 8);
			pDataMirror += 8;
			//7
			memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_textStyle.m_nWeight, 4);
			pDataMirror += 4;
			//8
			memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_textStyle.m_nStyle, 4);
			pDataMirror += 4;
			//9
			memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_textStyle.m_nAlign, 1);
			pDataMirror += 1;
			//10
			memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_textStyle.m_bBold, 1);
			pDataMirror += 1;
			//11
			memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_textStyle.m_bUnderline, 1);
			pDataMirror += 1;
			//12
			memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_textStyle.m_bItalic, 1);
			pDataMirror += 1;
			//13
			memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_textStyle.m_bStrikeOut, 1);
			pDataMirror += 1;
			//14
			memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_textStyle.m_bFixedSize, 1);
			pDataMirror += 1;
			//15
			memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_textStyle.m_bBackOpaque, 1);
			pDataMirror += 1;
			//16
			memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_textStyle.m_bHalo, 1);
			pDataMirror += 1;
			//17
			memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_textStyle.m_bShadow, 1);
			pDataMirror += 1;
			//18
			for (i = 0; i < nStrFaceNameLength; i++)
			{
				OgdcChar ch = ((OgdcElemText*)pElement)->m_textStyle.m_strFaceName.GetAt(i);
				memcpy(pDataMirror, &ch, 1);
				pDataMirror += 1;
			}
			///////////////////////
			//将pdata一个字节一个字节的装进CBYteArray数组中，再用ole来构造它
			
			arr.SetSize(lSize);
			for (i = 0; i < lSize; i++)
			{
				arr.SetAt(i, pData[i]);
			}
			delete pData;
			pData = NULL;
			nStrLength.RemoveAll();

		}
		else if(m_pDataset->GetType() == OgdcDataset::Line)
		{
			//ogdc中没有getdata函数，以下是自己计算几何对象在内存中的大小，并且拷贝数据进pdata
			OgdcInt nPointCount = ((OgdcElemLine*)pElement)->m_points.GetSize();
			OgdcInt nSubCount = ((OgdcElemLine*)pElement)->m_polyCounts.GetSize();
			OgdcInt lSize = 4 * 2; 
			lSize += nPointCount * 16;
			lSize += nSubCount * 4;
			
			int i = 0;
			BYTE *pData = new BYTE[lSize];
			BYTE *pDataMirror = pData;
			//用一个int型数存点的个数
			memcpy(pDataMirror,&nPointCount, sizeof(OgdcInt));
			pDataMirror += sizeof(OgdcInt);
			//用一个int型数存子对象的个数
 	    	memcpy(pDataMirror,&nSubCount, sizeof(OgdcInt));
			pDataMirror += sizeof(OgdcInt);
			for(i = 0; i < nSubCount; i++)
			{
				memcpy(pDataMirror, &(((OgdcElemLine*)pElement)->m_polyCounts[i]), 
					sizeof(OgdcInt));
				pDataMirror += sizeof(OgdcInt);
			}
			for (i = 0; i < nPointCount; i++)
			{
				memcpy(pDataMirror, &(((OgdcElemLine*)pElement)->m_points[i].x), 
					sizeof(OgdcDouble));
				pDataMirror += sizeof(OgdcDouble);
				memcpy(pDataMirror, &(((OgdcElemLine*)pElement)->m_points[i].y), 
					sizeof(OgdcDouble));
				pDataMirror += sizeof(OgdcDouble);
			}

			//将pdata一个字节一个字节的装进CBYteArray数组中，再用ole来构造它
		
			arr.SetSize(lSize);
			for (i = 0; i < lSize; i++)
			{
				arr.SetAt(i, pData[i]);
			}

			delete pData;
			pData = NULL;
						
		}
		else if(m_pDataset->GetType() == OgdcDataset::Line3D)
		{
			//ogdc中没有getdata函数，以下是自己计算几何对象在内存中的大小，并且拷贝数据进pdata
			OgdcInt nPointCount = ((OgdcElemLine3D*)pElement)->m_points.GetSize();
			OgdcInt nSubCount = ((OgdcElemLine3D*)pElement)->m_polyCounts.GetSize();
			OgdcInt lSize = sizeof(OgdcInt) * 2;
			lSize += nSubCount * sizeof(OgdcInt);
			lSize += 3 * nPointCount * sizeof(OgdcDouble);

			int i = 0;
			BYTE *pData = new BYTE[lSize];
			BYTE *pDataMirror = pData;
			//用一个int型数存点的个数
			memcpy(pDataMirror,&nPointCount, sizeof(OgdcInt));
			pDataMirror += sizeof(OgdcInt);
			//用一个int型数存子对象的个数
			memcpy(pDataMirror,&nSubCount, sizeof(OgdcInt));
			pDataMirror += sizeof(OgdcInt);
			for(i = 0; i < nSubCount; i++)
			{
				memcpy(pDataMirror, &(((OgdcElemLine3D*)pElement)->m_polyCounts[i]), 
					sizeof(OgdcInt));
				pDataMirror += sizeof(OgdcInt);
			}
			for (i = 0; i < nPointCount; i++)
			{
				memcpy(pDataMirror, &(((OgdcElemLine3D*)pElement)->m_points[i].x), 
					sizeof(OgdcDouble));
				pDataMirror += sizeof(OgdcDouble);
				memcpy(pDataMirror, &(((OgdcElemLine3D*)pElement)->m_points[i].y), 
					sizeof(OgdcDouble));
				pDataMirror += sizeof(OgdcDouble);
				memcpy(pDataMirror, &(((OgdcElemLine3D*)pElement)->m_points[i].z), 
					sizeof(OgdcDouble));
				pDataMirror += sizeof(OgdcDouble);
			}

			//将pdata一个字节一个字节的装进CBYteArray数组中，再用ole来构造它

			arr.SetSize(lSize);
			for (i = 0; i < lSize; i++)
			{
				arr.SetAt(i, pData[i]);
			}

			delete pData;
			pData = NULL;

		}
		else if (m_pDataset->GetType() == OgdcDataset::Region)
		{
			//ogdc中没有getdata函数，以下是自己计算几何对象在内存中的大小，并且拷贝数据进pdata
			OgdcInt nPointCount = ((OgdcElemRegion*)pElement)->m_points.GetSize();
			OgdcInt nSubCount = ((OgdcElemRegion*)pElement)->m_polyCounts.GetSize();
			OgdcInt lSize = 4 * 2; 
			lSize += nPointCount * 16;
			lSize += nSubCount * 4;
			
			OgdcInt i = 0;
			BYTE *pData = new BYTE[lSize];
			BYTE *pDataMirror = pData;
			//用一个int型数存点的个数
			memcpy(pDataMirror, &nPointCount, sizeof(OgdcInt));
			pDataMirror += sizeof(OgdcInt);
			//用一个int型数存子对象的个数
			memcpy(pDataMirror,&nSubCount,sizeof(OgdcInt));
			pDataMirror += sizeof(OgdcInt);
			for (i = 0; i < nSubCount; i++)
			{
				memcpy(pDataMirror, &((OgdcElemRegion*)pElement)->m_polyCounts[i], 
					sizeof(OgdcInt));
				pDataMirror += sizeof(OgdcInt);
			}
			for (i = 0; i < nPointCount; i++)
			{
				memcpy(pDataMirror, &((OgdcElemRegion*)pElement)->m_points[i].x, 
					sizeof(OgdcDouble));
				pDataMirror += sizeof(OgdcDouble);
				memcpy(pDataMirror, &((OgdcElemRegion*)pElement)->m_points[i].y, 
					sizeof(OgdcDouble));
				pDataMirror += sizeof(OgdcDouble);
			}
			//将pdata一个字节一个字节的装进CBYteArray数组中，再用ole来构造它
			
			arr.SetSize(lSize);
			for (i = 0; i < lSize; i++)
			{
				arr.SetAt(i, pData[i]);
			}

			delete pData;
			pData = NULL;
			
		}
		else if (m_pDataset->GetType() == OgdcDataset::Region3D)
		{
			//ogdc中没有getdata函数，以下是自己计算几何对象在内存中的大小，并且拷贝数据进pdata
			OgdcInt nPointCount = ((OgdcElemRegion3D*)pElement)->m_points.GetSize();
			OgdcInt nSubCount = ((OgdcElemRegion3D*)pElement)->m_polyCounts.GetSize();
			OgdcInt lSize = sizeof(OgdcInt) * 2;
			lSize += nSubCount * sizeof(OgdcInt);
			lSize += 3 * nPointCount * sizeof(OgdcDouble);

			OgdcInt i = 0;
			BYTE *pData = new BYTE[lSize];
			BYTE *pDataMirror = pData;
			//用一个int型数存点的个数
			memcpy(pDataMirror, &nPointCount, sizeof(OgdcInt));
			pDataMirror += sizeof(OgdcInt);
			//用一个int型数存子对象的个数
			memcpy(pDataMirror,&nSubCount,sizeof(OgdcInt));
			pDataMirror += sizeof(OgdcInt);
			for (i = 0; i < nSubCount; i++)
			{
				memcpy(pDataMirror, &((OgdcElemRegion3D*)pElement)->m_polyCounts[i], 
					sizeof(OgdcInt));
				pDataMirror += sizeof(OgdcInt);
			}
			for (i = 0; i < nPointCount; i++)
			{
				memcpy(pDataMirror, &((OgdcElemRegion3D*)pElement)->m_points[i].x, 
					sizeof(OgdcDouble));
				pDataMirror += sizeof(OgdcDouble);
				memcpy(pDataMirror, &((OgdcElemRegion3D*)pElement)->m_points[i].y, 
					sizeof(OgdcDouble));
				pDataMirror += sizeof(OgdcDouble);
				memcpy(pDataMirror, &((OgdcElemRegion3D*)pElement)->m_points[i].z, 
					sizeof(OgdcDouble));
				pDataMirror += sizeof(OgdcDouble);
			}
			//将pdata一个字节一个字节的装进CBYteArray数组中，再用ole来构造它

			arr.SetSize(lSize);
			for (i = 0; i < lSize; i++)
			{
				arr.SetAt(i, pData[i]);
			}

			delete pData;
			pData = NULL;

		}
		COleVariant varOle(arr);

		//以上是获取element数据的部分，需要写在一个函数中，等待改进
		try
		{
			m_pDaoRecordset->SetFieldValue (OG_GEOMETRY, varOle);
			m_pDaoRecordset->SetFieldValue (OG_SDRI_W, bound.left);
			m_pDaoRecordset->SetFieldValue (OG_SDRI_N, bound.top);
			m_pDaoRecordset->SetFieldValue (OG_SDRI_E, bound.right);
			m_pDaoRecordset->SetFieldValue (OG_SDRI_S, bound.bottom);
			m_pDaoRecordset->Update();
		}
		catch (CDaoException* e)
		{
			e->Delete();
			//没有成功，Cancel
			m_pDaoRecordset->CancelUpdate();
			return -1;
		}
	
	/*	m_recordset->SetFieldValue (LF_GRANULE, dGranule);*/	
	}
	

	//修改EditMode
	m_pElement = pElement;
	
	m_bEdited = TRUE;
	m_nEditMode = EditSetGeometry;
	m_pDataset->SetModifiedFlag(TRUE);
	return Update();	
}

OgdcBool OgdcRecordsetMdb::SetElement(UGC::UGGeometry* pGeometry)
{
	if (NULL == pGeometry)
	{
		return FALSE;
	}
	if (!Edit())
	{
		return FALSE;
	}
	if (m_pDataset->GetType() != OgdcDataset::PointEPS &&
		m_pDataset->GetType() != OgdcDataset::LineEPS &&
		m_pDataset->GetType() != OgdcDataset::RegionEPS &&
		m_pDataset->GetType() != OgdcDataset::TextEPS &&
		m_pDataset->GetType() != OgdcDataset::Model &&
		m_pDataset->GetType() != OgdcDataset::Texture)
	{
		OgdcElement *pElement = NULL;
		if (!pGeometry->ToElement(pElement))
		{
			delete pElement;
			pElement = NULL;
			return FALSE;
		}
		return SetElement(pElement);
	}

	OgdcBool bResult = FALSE;
	UGC::UGMemoryStream dataStream;
	dataStream.Open(UGC::UGStreamSave, 256, NULL);
	OgdcInt nHashCode = 0;
	UGC::UGRect2D bound;
	switch (m_pDataset->GetType())
	{
	case OgdcDataset::Texture:
		{
			OgdcInt nGeoType = pGeometry->GetType();
			dataStream << nGeoType;
			bResult = pGeometry->Save(dataStream, UGC::UGDataCodec::encNONE, FALSE);
			if (bResult)
			{
				nHashCode = ((UGC::UGGeoModelEntity*)pGeometry)->GeoHashCode();
			}
		}
		break;
	default:
		{
			if (m_pDataset->GetType() == OgdcDataset::Model)
			{
				OgdcInt nGeoType = pGeometry->GetType();
				dataStream << nGeoType;
				bResult = pGeometry->Save(dataStream, UGC::UGDataCodec::encNONE, FALSE);
			}
			else
			{
				bResult = pGeometry->SaveGeoData(dataStream,UGC::UGDataCodec::encNONE);
			}
			if (bResult)
			{
				bound = pGeometry->GetBounds();
			}
		}
		break;
	}

	if (!bResult)
	{
		dataStream.Close();
		return FALSE;
	}

	CByteArray arr;
	UGC::UGulong dataLength = dataStream.GetLength();
	arr.SetSize(dataLength);
	UGC::UGuchar* pData = dataStream.GetData();
	BYTE *pDataMirror = arr.GetData();
	memcpy(pDataMirror,pData,dataLength);
	dataStream.Close();
	COleVariant varOle(arr);

	try
	{
		m_pDaoRecordset->SetFieldValue (OG_GEOMETRY, varOle);
		if (m_pDataset->GetType() == OgdcDataset::Texture)
		{
			m_pDaoRecordset->SetFieldValue (OG_HASHCODE, long(nHashCode));
		}
		else
		{
			m_pDaoRecordset->SetFieldValue (OG_SDRI_W, bound.left);
			m_pDaoRecordset->SetFieldValue (OG_SDRI_N, bound.top);
			m_pDaoRecordset->SetFieldValue (OG_SDRI_E, bound.right);
			m_pDaoRecordset->SetFieldValue (OG_SDRI_S, bound.bottom);
		}
		m_pDaoRecordset->Update();
	}
	catch (CDaoException* e)
	{
		e->Delete();
		m_pDaoRecordset->CancelUpdate();
		return FALSE;
	}

	m_pGeometry = pGeometry;
	m_bEdited = TRUE;
	m_nEditMode = EditSetGeometry;
	m_pDataset->SetModifiedFlag(TRUE);
	return Update();
}

OgdcInt OgdcRecordsetMdb::AddNew( OgdcElement *pElement, OgdcBool bReturnID/*=FALSE*/ )
{
	if(pElement == NULL)
	{
		return -1;
	}

	OgdcString strSQL;
	strSQL.Format(_U("SELECT MAX(%s) FROM [%s]"),OG_OBJ_ID, m_strTableName.Cstr());

	CDaoRecordset recordset1(&(m_pDataset->m_pDataSource->m_daoDatabase));
	recordset1.Open (dbOpenDynaset,strSQL);
	COleVariant varlue;
	recordset1.GetFieldValue ((short)0,varlue);
	OgdcLong nID = varlue.lVal + 1;
	pElement->m_nID=OgdcInt(nID);
	if (nID == 1)
	{
		m_bFirstAddNew = TRUE;
	}
	recordset1.Close();
	try
	{	
		m_pDaoRecordset->AddNew();
	}
	catch (CDaoException* e)
	{
		e->Delete();
		m_pDaoRecordset->Close();
		return -1;
	}

	//******************************************
	//纯属性表的处理(不含空间数据
	if (pElement->GetType() == 0)
	{
		try
		{
			m_pDaoRecordset->SetFieldValue(OG_OBJ_ID, long(nID));				
		}
		catch (CDaoException* e)
		{
			e->Delete();
			m_pDaoRecordset->CancelUpdate();
			return -1;
		}
		m_bEdited = TRUE;
		m_nEditMode = dbEditAdd;
		m_pDataset->SetModifiedFlag(TRUE);
		m_pElement = pElement;
		Update();
		return OgdcInt(nID);
	}
	//}纯属性表的处理(不含空间数据
	//******************************************

	//****************************

	//{{空间数据表的处理，把数据转化成二进制装进数据库
	if (m_pDataset->GetType() == OgdcDataset::Point)
	{
		try
		{
			m_pDaoRecordset->SetFieldValue (OG_OBJ_ID,long(nID));
			m_pDaoRecordset->SetFieldValue (OG_X,((OgdcElemPoint *)pElement)->m_point.x);
			m_pDaoRecordset->SetFieldValue (OG_Y,((OgdcElemPoint *)pElement)->m_point.y);
			m_pDaoRecordset->Update();
	//		m_recordset->Close();
//			table.Close();
		}
		catch (CDaoException* e)
		{
			e->Delete();
			//没有成功，Cancel
			m_pDaoRecordset->CancelUpdate();
			return -1;
		}

	}
	else if (m_pDataset->GetType() == OgdcDataset::Point3D)
	{
		try
		{
			m_pDaoRecordset->SetFieldValue (OG_OBJ_ID,long(nID));
			m_pDaoRecordset->SetFieldValue (OG_X,((OgdcElemPoint3D *)pElement)->m_point.x);
			m_pDaoRecordset->SetFieldValue (OG_Y,((OgdcElemPoint3D *)pElement)->m_point.y);
			m_pDaoRecordset->SetFieldValue (OG_Z,((OgdcElemPoint3D *)pElement)->m_point.z);
			m_pDaoRecordset->Update();
		}
		catch (CDaoException* e)
		{
			e->Delete();
			m_pDaoRecordset->CancelUpdate();
			return -1;
		}
	}
	else
	{
		
		OgdcRect2D bound = pElement->GetBounds();
		double dGranule = (bound.Width() > bound.Height() ? bound.Width() : bound.Height());
		CByteArray arr;
		BYTE *pDataMirror = NULL;
		if (m_pDataset->GetType() == OgdcDataset::Text)
		{
			OgdcInt i = 0;
			//记录点的个数
			OgdcInt nPointCount = ((OgdcElemText*)pElement)->m_points.GetSize();
			//记录子文本的个数，Angles的个数即是子对象个数
			OgdcInt nStrCount = ((OgdcElemText*)pElement)->m_strTexts.GetSize();
			//记录每个子文本的长度
			//OgdcInt nStrLength[nStrCount];
			OgdcArray<OgdcInt> nStrLength;
			nStrLength.SetSize(nStrCount);
			//记录文本风格中，字体名字的长度
			OgdcInt nStrFaceNameLength = ((OgdcElemText*)pElement)->m_textStyle .m_strFaceName.GetLength();
			//获取每个子文本的长度
			for (i = 0; i < nStrCount; i++)
			{
				nStrLength[i]=((OgdcElemText*)pElement)->m_strTexts.GetAt(i).GetLength();
			}
			//所有Count的字节数
			OgdcInt lSize = 4 + 4 + 4 * nStrCount + 4;
			//所有点的字节数
			lSize += nPointCount * 16;
			//所有子文本的长度
			for (i = 0; i < nStrCount; i++)
			{
				lSize += nStrLength[i] * sizeof(OgdcChar);
			}
			//所有angele的字节数
			lSize += nPointCount*8;
			//文本风格中，字体名字的字节数
			lSize += nStrFaceNameLength * sizeof(OgdcChar);
			//文本风格中，其他参数的字节数
			lSize += 57;
			
			arr.SetSize(lSize);
			BYTE *pDataMirror = arr.GetData();
			//{{存数字标志
			memcpy(pDataMirror, &nPointCount, 4);
			pDataMirror += 4;
			memcpy(pDataMirror, &nStrCount, 4);
			pDataMirror += 4;
			for (i = 0; i < nStrCount; i++)
			{
				OgdcInt nLenTemp = nStrLength.GetAt(i);
				memcpy(pDataMirror, &nLenTemp, 4);
				pDataMirror += 4;
			}
			memcpy(pDataMirror, &nStrFaceNameLength, 4);
			pDataMirror += 4;
			//}}存数字标志

			//以下才是存文本风格的参数
			for (i = 0; i < nPointCount; i++)
			{
				memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_points[i].x, 
					sizeof(OgdcDouble));
				pDataMirror += sizeof(OgdcDouble);
				memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_points[i].y, 
					sizeof(OgdcDouble));
				pDataMirror += sizeof(OgdcDouble);
			}
			for (i = 0; i < nStrCount; i++)
			{
				for (OgdcInt j = 0; j < nStrLength.GetAt(i); j++)
				{
					OgdcChar ch = ((OgdcElemText*)pElement)->m_strTexts[i].GetAt(j);
					memcpy(pDataMirror, &ch, sizeof(OgdcChar));
					pDataMirror += sizeof(OgdcChar);
				}
			}
			for (i = 0; i < nStrCount; i++)
			{
				OgdcDouble *pDouble=new OgdcDouble;
				*pDouble=((OgdcElemText*)pElement)->m_angles.GetAt(i);
				memcpy(pDataMirror, pDouble, sizeof(OgdcDouble));
				pDataMirror += sizeof(OgdcDouble);
				delete pDouble;
				pDouble = NULL;
			}
			////////////////
			//1
			memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_textStyle.m_clrFore, 4);
			pDataMirror += 4;
			//2
			memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_textStyle.m_clrBack, 4);
			pDataMirror += 4;
			//3
			memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_textStyle.m_dAngle, 8);
			pDataMirror += 8;
			//4
			memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_textStyle.m_dItalicAngle, 8);
			pDataMirror += 8;
			//5
			memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_textStyle.m_dWidth, 8);
			pDataMirror += 8;
			//6
			memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_textStyle.m_dHeight, 8);
			pDataMirror += 8;
			//7
			memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_textStyle.m_nWeight, 4);
			pDataMirror += 4;
			//8
			memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_textStyle.m_nStyle, 4);
			pDataMirror += 4;
			//9
			memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_textStyle.m_nAlign, 1);
			pDataMirror += 1;
			//10
			memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_textStyle.m_bBold, 1);
			pDataMirror += 1;
			//11
			memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_textStyle.m_bUnderline, 1);
			pDataMirror += 1;
			//12
			memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_textStyle.m_bItalic, 1);
			pDataMirror += 1;
			//13
			memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_textStyle.m_bStrikeOut, 1);
			pDataMirror += 1;
			//14
			memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_textStyle.m_bFixedSize, 1);
			pDataMirror += 1;
			//15
			memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_textStyle.m_bBackOpaque, 1);
			pDataMirror += 1;
			//16
			memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_textStyle.m_bHalo, 1);
			pDataMirror += 1;
			//17
			memcpy(pDataMirror, &((OgdcElemText*)pElement)->m_textStyle.m_bShadow, 1);
			pDataMirror += 1;
			//18
			for (i = 0; i < nStrFaceNameLength; i++)
			{
				OgdcChar ch = ((OgdcElemText*)pElement)->m_textStyle.m_strFaceName.GetAt(i);
				memcpy(pDataMirror, &ch, sizeof(OgdcChar));
				pDataMirror +=  sizeof(OgdcChar);
			}
			///////////////////////

			nStrLength.RemoveAll();
		}
		else if (m_pDataset->GetType() == OgdcDataset::Line)
		{
			//ogdc中没有getdata函数，以下是自己计算几何对象在内存中的大小，并且拷贝数据进pdata
			OgdcInt nPointCount = ((OgdcElemLine*)pElement)->m_points.GetSize();
			OgdcInt nSubCount = ((OgdcElemLine*)pElement)->m_polyCounts.GetSize();
			OgdcInt lSize = 4*2; 
			lSize += nPointCount * 16;
			lSize += nSubCount * 4;
			
			OgdcInt i=0;
			arr.SetSize(lSize);
			BYTE *pDataMirror = arr.GetData();
			//用一个int型数存点的个数
			memcpy(pDataMirror,&nPointCount,sizeof(OgdcInt));
			pDataMirror+=sizeof(OgdcInt);
			//用一个int型数存子对象的个数
 	    	memcpy(pDataMirror,&nSubCount,sizeof(OgdcInt));
			pDataMirror+=sizeof(OgdcInt);
			for (i = 0; i < nSubCount; i++)
			{
				memcpy(pDataMirror, &(((OgdcElemLine*)pElement)->m_polyCounts[i]), 
					sizeof(OgdcInt));
				pDataMirror += sizeof(OgdcInt);
			}
			for (i = 0; i < nPointCount; i++)
			{
				memcpy(pDataMirror, &(((OgdcElemLine*)pElement)->m_points[i].x), 
					sizeof(OgdcDouble));
				pDataMirror += sizeof(OgdcDouble);
				memcpy(pDataMirror, &(((OgdcElemLine*)pElement)->m_points[i].y), 
					sizeof(OgdcDouble));
				pDataMirror += sizeof(OgdcDouble);
			}
		}
		else if (m_pDataset->GetType() == OgdcDataset::Line3D)
		{
			//ogdc中没有getdata函数，以下是自己计算几何对象在内存中的大小，并且拷贝数据进pdata
			OgdcInt nPointCount = ((OgdcElemLine3D*)pElement)->m_points.GetSize();
			OgdcInt nSubCount = ((OgdcElemLine3D*)pElement)->m_polyCounts.GetSize();
			OgdcInt lSize = sizeof(OgdcInt)*2; 
			lSize += nSubCount * sizeof(OgdcInt);
			lSize += 3 * nPointCount * sizeof(OgdcDouble);

			OgdcInt i=0;
			arr.SetSize(lSize);
			BYTE *pDataMirror = arr.GetData();
			//用一个int型数存点的个数
			memcpy(pDataMirror,&nPointCount,sizeof(OgdcInt));
			pDataMirror+=sizeof(OgdcInt);
			//用一个int型数存子对象的个数
			memcpy(pDataMirror,&nSubCount,sizeof(OgdcInt));
			pDataMirror+=sizeof(OgdcInt);
			for (i = 0; i < nSubCount; i++)
			{
				memcpy(pDataMirror, &(((OgdcElemLine3D*)pElement)->m_polyCounts[i]), 
					sizeof(OgdcInt));
				pDataMirror += sizeof(OgdcInt);
			}
			for (i = 0; i < nPointCount; i++)
			{
				memcpy(pDataMirror, &(((OgdcElemLine3D*)pElement)->m_points[i].x), 
					sizeof(OgdcDouble));
				pDataMirror += sizeof(OgdcDouble);
				memcpy(pDataMirror, &(((OgdcElemLine3D*)pElement)->m_points[i].y), 
					sizeof(OgdcDouble));
				pDataMirror += sizeof(OgdcDouble);
				memcpy(pDataMirror, &(((OgdcElemLine3D*)pElement)->m_points[i].z), 
					sizeof(OgdcDouble));
				pDataMirror += sizeof(OgdcDouble);
			}
		}
		else if (m_pDataset->GetType() == OgdcDataset::Region)
		{
			//ogdc中没有getdata函数，以下是自己计算几何对象在内存中的大小，并且拷贝数据进pdata
			OgdcInt nPointCount = ((OgdcElemRegion*)pElement)->m_points.GetSize();
			OgdcInt nSubCount = ((OgdcElemRegion*)pElement)->m_polyCounts.GetSize();
			OgdcInt lSize = 4 * 2; 
			lSize += nPointCount * 16;
			lSize += nSubCount * 4;
			
			OgdcInt i = 0;
			arr.SetSize(lSize);
			BYTE *pDataMirror = arr.GetData();

			//用一个int型数存点的个数
			memcpy(pDataMirror, &nPointCount, sizeof(OgdcInt));
			pDataMirror += sizeof(OgdcInt);
			//用一个int型数存子对象的个数
			memcpy(pDataMirror, &nSubCount, sizeof(OgdcInt));
			pDataMirror += sizeof(OgdcInt);
			for(i=0; i<nSubCount; i++)
			{
				memcpy(pDataMirror, &((OgdcElemRegion*)pElement)->m_polyCounts[i], 
					sizeof(OgdcInt));

				pDataMirror += sizeof(OgdcInt);
			}
			for(i = 0; i<nPointCount; i++)
			{
				memcpy(pDataMirror, &((OgdcElemRegion*)pElement)->m_points[i].x,
					sizeof(OgdcDouble));

				pDataMirror += sizeof(OgdcDouble);

				memcpy(pDataMirror, &((OgdcElemRegion*)pElement)->m_points[i].y, 
					sizeof(OgdcDouble));

				pDataMirror += sizeof(OgdcDouble);
			}	
		}
		else if (m_pDataset->GetType() == OgdcDataset::Region3D)
		{
			//ogdc中没有getdata函数，以下是自己计算几何对象在内存中的大小，并且拷贝数据进pdata
			OgdcInt nPointCount = ((OgdcElemRegion3D*)pElement)->m_points.GetSize();
			OgdcInt nSubCount = ((OgdcElemRegion3D*)pElement)->m_polyCounts.GetSize();
			OgdcInt lSize = sizeof(OgdcInt) * 2;
			lSize += nSubCount * sizeof(OgdcInt);
			lSize += 3 * nPointCount * sizeof(OgdcDouble);

			OgdcInt i = 0;
			arr.SetSize(lSize);
			BYTE *pDataMirror = arr.GetData();
			//用一个int型数存点的个数
			memcpy(pDataMirror, &nPointCount, sizeof(OgdcInt));
			pDataMirror += sizeof(OgdcInt);
			//用一个int型数存子对象的个数
			memcpy(pDataMirror, &nSubCount, sizeof(OgdcInt));
			pDataMirror += sizeof(OgdcInt);
			for(i=0; i<nSubCount; i++)
			{
				memcpy(pDataMirror, &((OgdcElemRegion3D*)pElement)->m_polyCounts[i], 
					sizeof(OgdcInt));
				pDataMirror += sizeof(OgdcInt);
			}
			for(i = 0; i<nPointCount; i++)
			{
				memcpy(pDataMirror, &((OgdcElemRegion3D*)pElement)->m_points[i].x,
					sizeof(OgdcDouble));
				pDataMirror += sizeof(OgdcDouble);
				memcpy(pDataMirror, &((OgdcElemRegion3D*)pElement)->m_points[i].y, 
					sizeof(OgdcDouble));
				pDataMirror += sizeof(OgdcDouble);
				memcpy(pDataMirror, &((OgdcElemRegion3D*)pElement)->m_points[i].z, 
					sizeof(OgdcDouble));
				pDataMirror += sizeof(OgdcDouble);
			}	
		}
		COleVariant varValue = arr;
		try
		{
			m_pDaoRecordset->SetFieldValue (OG_OBJ_ID, long(nID));
			m_pDaoRecordset->SetFieldValue (OG_GEOMETRY, varValue);
			m_pDaoRecordset->SetFieldValue (OG_SDRI_W, bound.left);
			m_pDaoRecordset->SetFieldValue (OG_SDRI_N, bound.top);
			m_pDaoRecordset->SetFieldValue (OG_SDRI_E, bound.right);
			m_pDaoRecordset->SetFieldValue (OG_SDRI_S, bound.bottom);
			m_pDaoRecordset->Update();
			pDataMirror = NULL;
		}
		catch (CDaoException* e)
		{
			e->Delete();
			//没有成功，Cancel
			m_pDaoRecordset->CancelUpdate();
			pDataMirror = NULL;
			return -1;
		}
	}

	//修改EditMode

	m_bEdited = TRUE;
	m_nEditMode = dbEditAdd;
	m_pDataset->SetModifiedFlag(TRUE);
	m_pElement = pElement;
	Update();
	m_nID = OgdcInt(nID);

	// 直接Seek就行了
	SeekID(nID);
	return OgdcInt(nID);
}

OgdcInt OgdcRecordsetMdb::AddNew(UGC::UGGeometry* pGeometry, OgdcBool bReturnID)
{
	if (pGeometry == NULL)
	{
		return -1;
	}

	if (m_pDataset->GetType() != OgdcDataset::PointEPS &&
		m_pDataset->GetType() != OgdcDataset::LineEPS &&
		m_pDataset->GetType() != OgdcDataset::RegionEPS &&
		m_pDataset->GetType() != OgdcDataset::TextEPS &&
		m_pDataset->GetType() != OgdcDataset::Model &&
		m_pDataset->GetType() != OgdcDataset::Texture)
	{
		OgdcElement *pElement = NULL;
		if (!pGeometry->ToElement(pElement))
		{
			delete pElement;
			pElement = NULL;
			return -1;
		}
		return AddNew(pElement, bReturnID);
	}

	OgdcString strSQL;
	strSQL.Format(_U("SELECT MAX(%s) FROM [%s]"),OG_OBJ_ID, m_strTableName.Cstr());

	CDaoRecordset recordset1(&(m_pDataset->m_pDataSource->m_daoDatabase));
	recordset1.Open (dbOpenDynaset,strSQL);
	COleVariant varlue;
	recordset1.GetFieldValue ((short)0,varlue);
	OgdcLong nID = varlue.lVal + 1;
	pGeometry->SetID(nID);
	if (nID == 1)
	{
		m_bFirstAddNew = TRUE;
	}
	recordset1.Close();
	try
	{	
		m_pDaoRecordset->AddNew();
	}
	catch (CDaoException* e)
	{
		e->Delete();
		m_pDaoRecordset->Close();
		return -1;
	}

	OgdcBool bResult = FALSE;
	UGC::UGMemoryStream dataStream;
	dataStream.Open(UGC::UGStreamSave, 256, NULL);
	OgdcInt nHashCode = 0;
	UGC::UGRect2D bound;
	switch (m_pDataset->GetType())
	{
	case OgdcDataset::Texture:
		{
			OgdcInt nGeoType = pGeometry->GetType();
			dataStream << nGeoType;
			bResult = pGeometry->Save(dataStream, UGC::UGDataCodec::encNONE, FALSE);
			if (bResult)
			{
				nHashCode = ((UGC::UGGeoModelEntity*)pGeometry)->GeoHashCode();
			}
		}
		break;
	default:
		{
			if (m_pDataset->GetType() == OgdcDataset::Model)
			{
				OgdcInt nGeoType = pGeometry->GetType();
				dataStream << nGeoType;
				bResult = pGeometry->Save(dataStream, UGC::UGDataCodec::encNONE, FALSE);
			}
			else
			{
				bResult = pGeometry->SaveGeoData(dataStream,UGC::UGDataCodec::encNONE);
			}
			if (bResult)
			{
				bound = pGeometry->GetBounds();
			}
		}
		break;
	}

	if (!bResult)
	{
		m_pDaoRecordset->CancelUpdate();
		dataStream.Close();
		return -1;
	}

	CByteArray arr;
	UGC::UGulong dataLength = dataStream.GetLength();
	arr.SetSize(dataLength);
	UGC::UGuchar* pData = dataStream.GetData();
	BYTE *pDataMirror = arr.GetData();
	memcpy(pDataMirror,pData,dataLength);
	dataStream.Close();
	COleVariant varOle(arr);

	try
	{
		m_pDaoRecordset->SetFieldValue (OG_OBJ_ID, long(nID));
		m_pDaoRecordset->SetFieldValue (OG_GEOMETRY, varOle);
		if (m_pDataset->GetType() == OgdcDataset::Texture)
		{
			m_pDaoRecordset->SetFieldValue (OG_HASHCODE, long(nHashCode));
		}
		else
		{
			m_pDaoRecordset->SetFieldValue (OG_SDRI_W, bound.left);
			m_pDaoRecordset->SetFieldValue (OG_SDRI_N, bound.top);
			m_pDaoRecordset->SetFieldValue (OG_SDRI_E, bound.right);
			m_pDaoRecordset->SetFieldValue (OG_SDRI_S, bound.bottom);
		}
		m_pDaoRecordset->Update();
		pDataMirror = NULL;
	}
	catch (CDaoException* e)
	{
		e->Delete();
		m_pDaoRecordset->CancelUpdate();
		pDataMirror = NULL;
		return -1;
	}

	m_bEdited = TRUE;
	m_nEditMode = dbEditAdd;
	m_pDataset->SetModifiedFlag(TRUE);
	m_pGeometry = pGeometry;
	Update();
	m_nID = OgdcInt(nID);

	// 直接Seek就行了
	SeekID(nID);
	return OgdcInt(nID);
}

OgdcFeature* OgdcRecordsetMdb::GetFeature()
{
	if (!((OgdcDatasetVectorMdb*)m_pDataset)->IsOpen())
	{
		return FALSE;
	}
	
	//当前记录集为空，直接返回FALSE
	if (IsEmpty())
	{
		return FALSE;
	}
	if (IsBOF() || IsEOF())
	{
		return FALSE;
	}
	
	OgdcElement *pElement = NULL;
	if (!GetElement(pElement) && m_pDataset->m_info.m_nType == OgdcDataset::Tabular)
	{
		delete pElement;
		pElement = NULL;
		return NULL;
	}
	
	OgdcFeatureMdb *pFeature = new OgdcFeatureMdb();
	pFeature->SetElement(pElement);
	pFeature->SetID(GetID());
	pFeature->SetCharset(m_pDataset->GetCharset());

	OgdcFieldInfos filedInfos;
	OgdcFieldInfo info;
	CDaoFieldInfo daoFieldInfo;
	COleVariant var;	
	OgdcVariant varValue;
	pFeature->m_fieldDefines.SetSize(m_pDaoRecordset->GetFieldCount());
	pFeature->m_fieldValues.SetSize(m_pDaoRecordset->GetFieldCount());
	for (OgdcInt i = 0; i < m_pDaoRecordset->GetFieldCount(); i++)
	{
		m_pDaoRecordset->GetFieldInfo(i, daoFieldInfo);
		OgdcHelperMdb::CDaoFieldInfo2OgdcFieldInfo(daoFieldInfo, info);
		pFeature->m_fieldDefines[i].m_strName = info.m_strName;
		pFeature->m_fieldDefines[i].m_nType = info.m_nType;
		pFeature->m_fieldDefines[i].m_nSize = info.m_nSize;
		filedInfos.Add(info);
		m_pDaoRecordset->GetFieldValue(i, var);
		OgdcHelperMdb::COleVariant2OgdcVariant(var, varValue);
		pFeature->SetValue(i, varValue);
	}
	
	return pFeature;
}

UGC::UGFeature* OgdcRecordsetMdb::GetFeatureEx()
{
	if (!((OgdcDatasetVectorMdb*)m_pDataset)->IsOpen())
	{
		return NULL;
	}

	if (m_pDataset->GetType() != OgdcDataset::PointEPS &&
		m_pDataset->GetType() != OgdcDataset::LineEPS &&
		m_pDataset->GetType() != OgdcDataset::RegionEPS &&
		m_pDataset->GetType() != OgdcDataset::TextEPS &&
		m_pDataset->GetType() != OgdcDataset::Model &&
		m_pDataset->GetType() != OgdcDataset::Texture)
	{
		OgdcFeature *pOFeature = GetFeature();
		if (pOFeature == NULL)
		{
			return NULL;
		}

		UGC::UGFeature *pFeature = new UGC::UGFeature();
		pFeature->SetOgdcFeature(pOFeature);
		return pFeature;
	}

	//当前记录集为空，直接返回NULL
	if (IsEmpty())
	{
		return NULL;
	}
	if (IsBOF() || IsEOF())
	{
		return NULL;
	}

	UGC::UGGeometry *pGeometry = NULL;
	if (!GetElement(pGeometry))
	{
		delete pGeometry;
		pGeometry = NULL;
		return NULL;
	}

	UGC::UGFeature *pFeature = new UGC::UGFeature();
	pFeature->SetGeometry(pGeometry);
	pFeature->SetID(GetID());
	pFeature->SetCharset(m_pDataset->GetCharset());

	CDaoFieldInfo daoFieldInfo;
	OgdcFieldInfo info;
	OgdcFieldInfos filedInfos;
	for (OgdcInt i=0; i<m_pDaoRecordset->GetFieldCount(); i++)
	{
		m_pDaoRecordset->GetFieldInfo(i, daoFieldInfo);
		OgdcHelperMdb::CDaoFieldInfo2OgdcFieldInfo(daoFieldInfo, info);
		filedInfos.Add(info);
	}
	pFeature->SetFieldInfos(filedInfos);
	COleVariant var;
	OgdcVariant varValue;
	for (OgdcInt i=0; i<m_pDaoRecordset->GetFieldCount(); i++)
	{
		m_pDaoRecordset->GetFieldValue(i, var);
		OgdcHelperMdb::COleVariant2OgdcVariant(var, varValue);
		pFeature->SetValue(i, varValue);
	}

	return pFeature;
}

OgdcBool OgdcRecordsetMdb::UpdateFeature( const OgdcFeature* pFeature )
{
	if (pFeature == NULL || m_nEditMode != EditNone)
	{
		return FALSE;	
	}

	if ( !Edit() )
	{
		return FALSE;
	}

	OgdcElement* pElement = ((OgdcFeatureMdb*)pFeature)->GetElement();
	if (pElement->GetType() != OgdcDataset::Tabular && pElement == NULL)
	{
		return FALSE;
	}
	//设置几何对象
	SetElement(pElement);

	OgdcVariant val;
	OgdcString strField;
	OgdcFieldInfo fieldInfo;
	for (OgdcInt i = 0; i < pFeature->m_fieldDefines.GetSize(); i++)
	{
		//更新属性
		strField = pFeature->m_fieldDefines.GetAt(i).m_strName;
		if (OgdcHelperMdb::IsSystemField(strField))
		{
			continue;
		}
		
		if (!((OgdcFeatureMdb*)pFeature)->GetValue(strField, val))
		{
			continue;
		}
		SetFieldValue(strField, val);
		val.Clear();
	}
	m_pElement = pElement;
	return Update();
}

OgdcBool OgdcRecordsetMdb::UpdateFeature(const UGC::UGFeature* pFeature)
{
	if (pFeature == NULL || m_nEditMode != EditNone)
	{
		return FALSE;
	}

	if (m_pDataset->GetType() != OgdcDataset::PointEPS &&
		m_pDataset->GetType() != OgdcDataset::LineEPS &&
		m_pDataset->GetType() != OgdcDataset::RegionEPS &&
		m_pDataset->GetType() != OgdcDataset::TextEPS &&
		m_pDataset->GetType() != OgdcDataset::Model &&
		m_pDataset->GetType() != OgdcDataset::Texture)
	{
		return UpdateFeature(pFeature->GetOgdcFeature());
	}

	if (!Edit())
	{
		return FALSE;
	}

	UGC::UGFeature *pFeatureTemp = (UGC::UGFeature*)pFeature;
	UGC::UGGeometry* pGeometry = pFeatureTemp->GetGeometry();
	if (pGeometry == NULL || (pGeometry->GetType() != UGC::UGGeometry::GeoPointEPS &&
		pGeometry->GetType() != UGC::UGGeometry::GeoLineEPS &&
		pGeometry->GetType() != UGC::UGGeometry::GeoRegionEPS &&
		pGeometry->GetType() != UGC::UGGeometry::GeoTextEPS &&
		pGeometry->GetType() != UGC::UGGeometry::GeoModelPro &&
		pGeometry->GetType() != UGC::UGGeometry::GeoModelEntitySkeleton &&
		pGeometry->GetType() != UGC::UGGeometry::GeoModelEntityTexture &&
		pGeometry->GetType() != UGC::UGGeometry::GeoModelEntityMaterial3D))
	{
		return FALSE;
	}

	//设置几何对象
	SetElement(pGeometry);

	OgdcString strField;
	OgdcVariant val;
	UGArray<OgdcFeature::OgdcFieldDefine>* fieldDefine = pFeatureTemp->GetFieldDefines();
	for (OgdcInt i=0; i<fieldDefine->GetSize(); i++)
	{
		//更新属性
		strField = fieldDefine->GetAt(i).m_strName;
		if (OgdcHelperMdb::IsSystemField(strField))
		{
			continue;
		}

		if (!pFeatureTemp->GetValue(strField, val))
		{
			continue;
		}

		SetFieldValue(strField, val);
		val.Clear();
	}

	m_pGeometry = pGeometry;
	return Update();
}

OgdcBool OgdcRecordsetMdb::AddFeature( const OgdcFeature* pFeature )
{
	if (pFeature == NULL || m_nEditMode != EditNone)
	{
		return FALSE;	
	}
	OgdcElement *pElement = ((OgdcFeature*)pFeature)->GetElement();
	if (m_pDataset->GetType() == OgdcDataset::Tabular && pElement == NULL)
	{
		return FALSE;
	}

	m_nID = AddNew(pElement);
	
	OgdcVariant val;
	OgdcString strField;
	OgdcFieldInfo fieldInfo;
	COleVariant varValue;
	for (OgdcInt i = 0; i < pFeature->m_fieldDefines.GetSize(); i++)
	{
		//更新属性
		strField = pFeature->m_fieldDefines.GetAt(i).m_strName;
		if (OgdcHelperMdb::IsSystemField(strField))
		{
			continue;
		}

		if (strField.Left(2).CompareNoCase(_U("Sm")) == 0) 
		{
			continue;
		}
		
		if (!((OgdcFeatureMdb*)pFeature)->GetValue(strField, val))
		{
			continue;
		}

		SetFieldValue(strField, val);
	}
	
//	return Update();
	return TRUE;
}

OgdcBool OgdcRecordsetMdb::AddFeature(const UGC::UGFeature* pFeature)
{
	if (pFeature == NULL || m_nEditMode != EditNone)
	{
		return FALSE;
	}

	if (m_pDataset->GetType() != OgdcDataset::PointEPS &&
		m_pDataset->GetType() != OgdcDataset::LineEPS &&
		m_pDataset->GetType() != OgdcDataset::RegionEPS &&
		m_pDataset->GetType() != OgdcDataset::TextEPS &&
		m_pDataset->GetType() != OgdcDataset::Model &&
		m_pDataset->GetType() != OgdcDataset::Texture)
	{
		return AddFeature(pFeature->GetOgdcFeature());
	}

	UGC::UGFeature *pFeatureTemp = (UGC::UGFeature*)pFeature;
	UGC::UGGeometry* pGeometry = pFeatureTemp->GetGeometry();
	if (pGeometry == NULL || (pGeometry->GetType() != UGC::UGGeometry::GeoPointEPS &&
		pGeometry->GetType() != UGC::UGGeometry::GeoLineEPS &&
		pGeometry->GetType() != UGC::UGGeometry::GeoRegionEPS &&
		pGeometry->GetType() != UGC::UGGeometry::GeoTextEPS &&
		pGeometry->GetType() != UGC::UGGeometry::GeoModelPro &&
		pGeometry->GetType() != UGC::UGGeometry::GeoModelEntitySkeleton &&
		pGeometry->GetType() != UGC::UGGeometry::GeoModelEntityTexture &&
		pGeometry->GetType() != UGC::UGGeometry::GeoModelEntityMaterial3D))
	{
		return FALSE;
	}

	m_nID = AddNew(pGeometry);

	OgdcString strField;
	OgdcVariant val;
	UGArray<OgdcFeature::OgdcFieldDefine>* fieldDefine = pFeatureTemp->GetFieldDefines();
	for (OgdcInt i=0; i<fieldDefine->GetSize(); i++)
	{
		//更新属性
		strField = fieldDefine->GetAt(i).m_strName;
		if (OgdcHelperMdb::IsSystemField(strField))
		{
			continue;
		}

		if (strField.Left(2).CompareNoCase(_U("Sm")) == 0)
		{
			continue;
		}

		if (!pFeatureTemp->GetValue(strField, val))
		{
			continue;
		}

		SetFieldValue(strField, val);
		val.Clear();
	}

	return TRUE;
}

OgdcBool OgdcRecordsetMdb::SeekID( OgdcInt nID )
{
	if( !m_pDaoRecordset->IsOpen())
	{
		return FALSE;
	}
	
	//移动时当前的AddNew或Edit取消
	
	CancelUpdate();

	OgdcString strFilter;
	strFilter.Format(_U("OGID = %d"), nID);
	OgdcBool bFind;
	try			
	{
		//先移动指针到首记录，再往下找
		m_pDaoRecordset->MoveFirst();
		bFind = m_pDaoRecordset->Find(AFX_DAO_FIRST, strFilter);
		m_nID = nID;
	}
	catch (CDaoException* e)
	{
		e->Delete();
		return FALSE;
	}
	
	return bFind;	
}

OgdcInt OgdcRecordsetMdb::Update()
{
	//如果没有首先Addnew或Edit,返回FALSE
	if (m_nEditMode == dbEditNone)
	{
		return FALSE;
	}
	try
	{	
		if (m_nEditMode == dbEditAdd)
		{	
			m_nRecordCount ++;		
			m_pDataset->m_nObjectCount += 1;
		}
		OgdcRect2D rcBounds;
		rcBounds.SetEmpty();

		if (m_pElement != NULL)
		{
			rcBounds = m_pDataset->GetBounds();
			if(rcBounds.IsNull() || m_bFirstAddNew)
			{
				if (m_pDataset->GetType() == OgdcDataset::Point)
				{
					rcBounds.left = ((OgdcElemPoint*)m_pElement)->m_point.x;
					rcBounds.right = ((OgdcElemPoint*)m_pElement)->m_point.x;
					rcBounds.top = ((OgdcElemPoint*)m_pElement)->m_point.y;
					rcBounds.bottom = ((OgdcElemPoint*)m_pElement)->m_point.y;
				}
				else if (m_pDataset->GetType() == OgdcDataset::Point3D)
				{
					rcBounds.left = ((OgdcElemPoint3D*)m_pElement)->m_point.x;
					rcBounds.right = ((OgdcElemPoint3D*)m_pElement)->m_point.x;
					rcBounds.top = ((OgdcElemPoint3D*)m_pElement)->m_point.y;
					rcBounds.bottom = ((OgdcElemPoint3D*)m_pElement)->m_point.y;
				}
				else
				{
					rcBounds = m_pElement->GetBounds();
				}
				m_bFirstAddNew = FALSE;
			}
			else
			{
				if (m_pDataset->GetType() == OgdcDataset::Point)
				{
					rcBounds.left = rcBounds.left < ((OgdcElemPoint*)m_pElement)->m_point.x
						? rcBounds.left : ((OgdcElemPoint*)m_pElement)->m_point.x;
					rcBounds.right = rcBounds.right > ((OgdcElemPoint*)m_pElement)->m_point.x
						? rcBounds.right : ((OgdcElemPoint*)m_pElement)->m_point.x;
					rcBounds.top = rcBounds.top > ((OgdcElemPoint*)m_pElement)->m_point.y
						? rcBounds.top : ((OgdcElemPoint*)m_pElement)->m_point.y;
					rcBounds.bottom = rcBounds.bottom < ((OgdcElemPoint*)m_pElement)->m_point.y
						? rcBounds.bottom : ((OgdcElemPoint*)m_pElement)->m_point.y;
				}
				else if (m_pDataset->GetType() == OgdcDataset::Point3D)
				{
					rcBounds.left = rcBounds.left < ((OgdcElemPoint3D*)m_pElement)->m_point.x
						? rcBounds.left : ((OgdcElemPoint3D*)m_pElement)->m_point.x;
					rcBounds.right = rcBounds.right > ((OgdcElemPoint3D*)m_pElement)->m_point.x
						? rcBounds.right : ((OgdcElemPoint3D*)m_pElement)->m_point.x;
					rcBounds.top = rcBounds.top > ((OgdcElemPoint3D*)m_pElement)->m_point.y
						? rcBounds.top : ((OgdcElemPoint3D*)m_pElement)->m_point.y;
					rcBounds.bottom = rcBounds.bottom < ((OgdcElemPoint3D*)m_pElement)->m_point.y
						? rcBounds.bottom : ((OgdcElemPoint3D*)m_pElement)->m_point.y;
				}
				else
				{
					rcBounds.Union(m_pElement->GetBounds());
				}
			}
			m_pDataset->SetBounds(rcBounds);
			m_pElement = NULL;
		}
		else if (m_pGeometry != NULL)
		{
			if (m_pDataset->GetType() != OgdcDataset::Texture)
			{
				rcBounds = m_pDataset->GetBounds();
				if(rcBounds.IsNull() || m_bFirstAddNew)
				{
					rcBounds = m_pGeometry->GetBounds();
					m_bFirstAddNew = FALSE;
				}
				else
				{
					rcBounds.Union(m_pGeometry->GetBounds());
				}
				m_pDataset->SetBounds(rcBounds);
			}
			m_pGeometry = NULL;
		}
		//添加四至到注册表

		OgdcString strSQL;
		strSQL.Format (_U("UPDATE OGRegister SET OGLeft = %.16f, OGRight = %.16f, OGTop = %.16f, OGBottom = %.16f WHERE OGDatasetName = '%s'"), 
			rcBounds.left, rcBounds.right, rcBounds.top, rcBounds.bottom, m_strTableName.Cstr()); 
		try
		{
			m_pDataset->m_pDataSource->m_daoDatabase.Execute(strSQL);
			if (m_nEditMode == dbEditAdd)
			{
			
				strSQL.Format(_U("UPDATE OGRegister SET OGRecordCount = OGRecordCount + 1 WHERE OGDatasetName = '%s'"), m_strTableName.Cstr());

				m_pDataset->m_pDataSource->m_daoDatabase.Execute(strSQL);

			}

			COleDateTime dt = COleDateTime::GetCurrentTime();
			OgdcVariant var = OgdcTime(dt.GetYear(), dt.GetMonth(), 
				dt.GetDay(), dt.GetHour(),dt.GetMinute(),dt.GetSecond());

			strSQL.Format(_U("UPDATE OGRegister SET OGLastUpdate = '%s' WHERE OGDatasetName = '%s'"),
				var.ToString().Cstr(), m_strTableName.Cstr());

			m_pDataset->m_pDataSource->m_daoDatabase.Execute(strSQL);	
		}
		catch (CDaoException* e)
		{
			e->Delete();
			return FALSE;
		}
		
	}
	catch (CDaoException* e)
	{
		CancelUpdate();
		m_nEditMode = dbEditNone;
		m_bEdited = FALSE;
		e->Delete();
		return FALSE;
	}
	m_nEditMode = dbEditNone;
	m_bEdited = FALSE;
	return TRUE;	
}

OgdcBool OgdcRecordsetMdb::CancelUpdate()
{
	if (!m_pDaoRecordset->IsOpen())
	{
		return FALSE;
	}
	
	if (m_nEditMode == dbEditNone)
	{
		return TRUE;
	}
	try
	{
		m_nEditMode = dbEditNone;
		m_pDaoRecordset->CancelUpdate();
	}
	catch (CDaoException* e)
	{
		e->Delete();
		return FALSE;
	}
	m_bEdited = FALSE;
	return TRUE;	
}

OgdcBool OgdcRecordsetMdb::Edit()
{	
	if (!m_pDaoRecordset->IsOpen() || !CanUpdate())
	{
		return FALSE;
	}
	
	if (m_nEditMode != dbEditNone)
	{
		return TRUE;
	}
// 	m_recordset->Close();
// 	CDaoTableDef tableDef(&m_pDataset->m_pDataSource->m_daoDatabase);
// 	tableDef.Open()
	try
	{
		m_pDaoRecordset->Edit();
		m_bEdited = TRUE;
	}
	catch (CDaoException* e)
	{	
		e->Delete();
		return FALSE;
	}	
	//但是如果Edit失败则不应该设置edit标志变量，所有移动到这里。
	m_nEditMode = dbEditInProgress;
	return TRUE;
}

OgdcBool OgdcRecordsetMdb::CanUpdate()
{
	if(!m_pDaoRecordset->IsOpen())
	{
		return FALSE;
	}
	else
	{
		return m_pDataset->CanUpdate();
	}
}

OgdcBool OgdcRecordsetMdb::Delete()
{

	if (m_bEdited && (!Update()))
	{
		return FALSE;
	}
	
	if (!CanUpdate())	
	{
		return FALSE;
	}

	try
	{

	//	m_recordset->Delete ();	
		//m_nRecordCount = OgdcRecordsetMdb::GetRecordCount()-1;
		m_nRecordCount -= 1;
		m_pDataset->m_nObjectCount -= 1;
		OgdcString strSQL;
		strSQL.Format(_U("UPDATE OGRegister SET OGRecordCount = OGRecordCount-1 WHERE OGDatasetName = '%s'"),
			m_strTableName.Cstr());
		m_pDataset->m_pDataSource->m_daoDatabase.Execute(strSQL);

		strSQL.Format(_U("DELETE FROM [%s] WHERE OGID = %d"),
			m_strTableName.Cstr(), GetID());
		//记录指针指向下一条记录
		m_pDaoRecordset->MoveNext();
		m_pDataset->m_pDataSource->m_daoDatabase.Execute(strSQL);
		m_pDaoRecordset->Requery();
	}
	catch (CDaoException* e)
	{
		e->Delete();
		return FALSE;
	}
	return TRUE;
}

OgdcBool OgdcRecordsetMdb::DeleteAll()
{
	if (!CanUpdate()) 
	{
		return FALSE;	//不能编辑
	}
	
	if (m_nEditMode != dbEditNone) //正在编辑
	{
		return FALSE;
	}
	OgdcInt nCount = GetRecordCount();
	OgdcString strSQL;
	if (m_strFilter.IsEmpty())
	{
		strSQL.Format(_U("DELETE * FROM [%s] "), m_strTableName.Cstr());
	}
	else
	{
		strSQL.Format(_U("DELETE * FROM [%s] WHERE %s"), 
			m_strTableName.Cstr(), m_strFilter.Cstr());
	}
	try
	{
		m_pDataset->m_pDataSource->m_daoDatabase.Execute (strSQL);

		m_nRecordCount = 0;
		m_pDataset->m_nObjectCount -= nCount;
		strSQL.Format(_U("UPDATE OGRegister SET OGRecordCount = OGRecordCount - %d WHERE OGDatasetName = '%s'"), 
			nCount, m_strTableName.Cstr());

		m_pDataset->m_pDataSource->m_daoDatabase.Execute(strSQL);
	}
	catch (CDaoException* e)
	{
		e->Delete();
		return FALSE;
	}

	return TRUE;
}

OgdcDatasetVector* OgdcRecordsetMdb::GetDataset()
{
	return m_pDataset;	
}	


}