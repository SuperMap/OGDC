//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)           
//                                                           
//!  \file OgdcDatasetRasterMdb.cpp
//!  \brief 栅格数据集的接口实现。
//!  \details 栅格数据集各种接口的实现
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Limited Company.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#include "OgdcDatasetRasterMdb.h"
#include "OgdcHelperMdb.h"
#include "OgdcMacroDefineMdb.h"
#include "OGDC/OgdcProviderManager.h"

#define PYRAMID_MIN_WIDTH			64		//创建金字塔影像库时候最小宽度
#define PYRAMID_MIN_HEIGHT			64		//创建金字塔影像库时候最小高度
#define PYRAMID_TIRE_SCALE			2		//层与层之间缩小的倍率

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace OGDC
{

OgdcDatasetRasterMdb::OgdcDatasetRasterMdb()
{
	m_nID = 0;
	m_pDataSource = NULL;
	m_bOpened = FALSE;
	m_bModified = FALSE;
	m_bInitImg = FALSE;
}

OgdcDatasetRasterMdb::OgdcDatasetRasterMdb( OgdcDataSourceMdb *pDataSource )
{
	m_nID = 0;
	m_pDataSource = pDataSource;
	m_bOpened = FALSE;
	m_bModified = FALSE;
	m_bInitImg = FALSE;
}
OgdcDatasetRasterMdb::~OgdcDatasetRasterMdb()
{	
	Close();
	ReleasePyramidDatasets();
	ReleaseAllBlocks();
}

OgdcBool OgdcDatasetRasterMdb::BuildPyramid()
{
	if (m_pDataSource == NULL)
	{
		return FALSE;
	}

	//自己打开的自己关闭
	OgdcBool bOpen = FALSE;
	if (!IsOpen())
	{
		if (Open())
		{
			bOpen = TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	
	if (this->IsBuildPyramid())
	{
		return TRUE;
	}

	OgdcInt nNewWidth = GetWidth();
	OgdcInt nNewHeight = GetHeight();
	OgdcDatasetRasterInfo::IBSizeOption blocksize = m_info.m_nBlockSize;
	OgdcInt nOptions = 0;	
	if (GetType() != OgdcDataset::Grid && GetType() != OgdcDataset::Image)
	{
		return FALSE;
	}
	
	OgdcDatasetRasterMdb* pDest = NULL;
	OgdcInt lProgress = nNewWidth < nNewHeight ? nNewWidth : nNewHeight;
	OgdcInt nLevel = 1;	
	OgdcDouble dScale = 1.0;
	//金字塔父图层指针
	OgdcDatasetRasterMdb* pParentDataset = NULL;
	PixelFormat nPixelFormat = GetPixelFormat();

	while (nNewWidth > PYRAMID_MIN_WIDTH || nNewHeight > PYRAMID_MIN_HEIGHT)
	{
		if (pParentDataset == NULL)
		{
			pParentDataset = this;
		}
		
		if (nNewWidth > PYRAMID_TIRE_SCALE) 
		{
			nNewWidth /= PYRAMID_TIRE_SCALE;
		}
		if (nNewHeight > PYRAMID_TIRE_SCALE) 
		{
			nNewHeight /= PYRAMID_TIRE_SCALE;
		}
		dScale *= PYRAMID_TIRE_SCALE;
  
		if (nNewWidth <= 0 || nNewHeight <= 0)
		{
			return FALSE;
		}
		
		OgdcDatasetRasterMdb* pDataset = new OgdcDatasetRasterMdb(this->m_pDataSource);	
		pDataset->m_info = this->m_info;
		pDataset->m_info.m_nWidth = nNewWidth;
		pDataset->m_info.m_nHeight = nNewHeight;
		pDataset->m_info.m_nBlockSize = blocksize;
		pDataset->m_info.m_dMinZ = this->m_info.m_dMinZ;
		pDataset->m_info.m_dMaxZ = this->m_info.m_dMaxZ;
		pDest = CreateDatasetPyramid(pDataset->m_info, nLevel++);
	
		delete pDataset;
		pDataset = NULL;
		
		if (!pDest->Open())
		{
			return FALSE;
		}
		
		OgdcInt X, Y;	
		OgdcColorset pal = m_info.m_colorset;
		OgdcBool bIndexedColor = pal.GetSize() > 0;
		for (Y = 0; Y < nNewHeight; Y++)
		{
			for (X = 0; X < nNewWidth; X++)
			{
				if (GetType() == OgdcDataset::Image)
				{
					OgdcInt nPixel[5];
					//多种格式下的处理方式是不尽相同的
					//在这里针对各种不同的格式进行一下特殊处理
					switch (nPixelFormat)
					{
					case IPF_RGB:
					case IPF_RGBA:
					case IPF_TBYTE:		//16位同RGB一样处理即可
						{
							nPixel[0] = pParentDataset->GetPixel(2*X,2*Y);
							nPixel[1] = pParentDataset->GetPixel(2*X+1,2*Y);
							nPixel[2] = pParentDataset->GetPixel(2*X,2*Y+1);
							nPixel[3] = pParentDataset->GetPixel(2*X+1,2*Y+1);
							
							nPixel[4] = 
								RGB(
								(GetRValue(nPixel[0])+
								GetRValue(nPixel[1])+
								GetRValue(nPixel[2])+
								GetRValue(nPixel[3]))/4,
								
								(GetGValue(nPixel[0])+
								GetGValue(nPixel[1])+
								GetGValue(nPixel[2])+
								GetGValue(nPixel[3]))/4,
								
								(GetBValue(nPixel[0])+
								GetBValue(nPixel[1])+
								GetBValue(nPixel[2])+
								GetBValue(nPixel[3]))/4
								);
						}
						break;
					case IPF_MONO:
					case IPF_FBIT:
					case IPF_BYTE:
						{
							if ( bIndexedColor )
							{
								nPixel[0] = pParentDataset->GetPixel(2*X,2*Y);
								nPixel[1] = pParentDataset->GetPixel(2*X+1,2*Y);
								nPixel[2] = pParentDataset->GetPixel(2*X,2*Y+1);
								nPixel[3] = pParentDataset->GetPixel(2*X+1,2*Y+1);
								
								OgdcDouble f,dltA,dltB,dltC,dltD;
								f = (nPixel[0]+nPixel[1]+nPixel[2]+nPixel[3])/4.0;
								dltA = abs(nPixel[0]-(OgdcInt)f);
								dltB = abs(nPixel[1]-(OgdcInt)f);
								dltC = abs(nPixel[2]-(OgdcInt)f);
								dltD = abs(nPixel[3]-(OgdcInt)f);
								if(dltA<=dltB && dltA<=dltC && dltA<=dltD)
								{
									nPixel[4]=nPixel[0];
								}
								else if(dltB<=dltC && dltB<=dltD)
								{
									nPixel[4]=nPixel[1];
								}
								else if(dltC<=dltD)
								{
									nPixel[4]=nPixel[2];
								}
								else 
								{
									nPixel[4]=nPixel[3];
								}
							}
							else
							{
								nPixel[0] = (OgdcInt)pParentDataset->GetValue(2*X,2*Y);
								nPixel[1] = (OgdcInt)pParentDataset->GetValue(2*X+1,2*Y);
								nPixel[2] = (OgdcInt)pParentDataset->GetValue(2*X,2*Y+1);
								nPixel[3] = (OgdcInt)pParentDataset->GetValue(2*X+1,2*Y+1);
								nPixel[4] = (OgdcInt)(nPixel[0]+nPixel[1]+
									nPixel[2]+nPixel[3])/4;
							}
						}
						break;
					default:
						{
							nPixel[4] = (OgdcInt)pParentDataset->GetValue(2*X,2*Y);
						}
						break;
					}
					
					pDest->SetPixel(X,Y,nPixel[4]);
				}
				else if (GetType() == OgdcDataset::Grid)
				{
					OgdcDouble dPixel[5];
					dPixel[0] = pParentDataset->GetValue(2*X,2*Y);
					dPixel[1] = pParentDataset->GetValue(2*X+1,2*Y);
					dPixel[2] = pParentDataset->GetValue(2*X,2*Y+1);
					dPixel[3] = pParentDataset->GetValue(2*X+1,2*Y+1);
					
					dPixel[4] = (dPixel[0]+dPixel[1]+dPixel[2]+dPixel[3])/4.0;
					pDest->SetValue(X,Y,dPixel[4]);
				}
				else 
				{
					return FALSE;
				}
		
			}

		}
		pParentDataset->ReleaseAllBlocks();
		pParentDataset = pDest;
		OgdcRasterBlock* pRasterBlock = NULL;
		pDest->SetModifiedFlag(TRUE);
		for (OgdcInt nKey = 0; nKey < pDest->m_rasterDict.GetCount(); nKey++)
		{
			pDest->m_rasterDict.Lookup(nKey, pRasterBlock);
			pDest->SaveBlock(pRasterBlock);
		}
		pDest->SetModifiedFlag(FALSE);
		//各层金字塔指针
		m_pyramidDatasets.Add(pDest);
	}

	if (bOpen)
	{
		Close();
	}

	return TRUE;
}

OgdcBool OgdcDatasetRasterMdb::DropPyramid()
{
	ReleaseAllBlocks();
	ReleasePyramidDatasets();

	OgdcString strSQL;
	strSQL.Format("SELECT OGDatasetName FROM OGImgRegister "
		"WHERE OGPyramid LIKE '%s'", m_info.m_strName.Cstr());
	CDaoRecordset rasterRecord(&m_pDataSource->m_daoDatabase);
	try
	{
		rasterRecord.Open(AFX_DAO_USE_DEFAULT_TYPE, strSQL);
	
		COleVariant varValue;
		OgdcString strPyramidName;
		while (!rasterRecord.IsEOF())  
		{			
			rasterRecord.GetFieldValue((short)0, varValue);
			strPyramidName = OgdcString((LPCTSTR)varValue.bstrVal);
			m_pDataSource->m_daoDatabase.DeleteTableDef(strPyramidName);
			strSQL.Format ("DELETE FROM OGImgRegister "
				"WHERE OGDatasetName LIKE '%s'", strPyramidName.Cstr());
			m_pDataSource->m_daoDatabase.Execute (strSQL);
			rasterRecord.MoveNext();
		}
		rasterRecord.Close();
	}
	catch (CDaoException* e)
	{
		e->Delete();
		rasterRecord.Close();
		return FALSE;
	}

	return TRUE;
}

OgdcBool OgdcDatasetRasterMdb::IsBuildPyramid() const
{	
	if (m_pyramidDatasets.GetSize() > 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

OgdcBool OgdcDatasetRasterMdb::GetScanline(OgdcInt nRowIndex,OgdcRasterScanline& ScanLine, OgdcInt nStartCol)
{
	if(!Open())
	{
		return FALSE;
	}

	if ((nRowIndex<0)||(nRowIndex>=GetHeight()))
	{
		return FALSE;
	}

	if(nStartCol<0)
		nStartCol = 0;
	if((nStartCol>=GetWidth()))
	{
		return FALSE;
	}

	//计算包含该像素的块的行列索引,以及该像素在该块中的偏移
	if (!m_bInitImg)
	{
		if(!ComDivAndMod())
		{
			return 0;
		}
	}

	OgdcInt lBlockIndex  = nRowIndex >> m_nDiv;
	OgdcInt lBlockRemain = nRowIndex & m_nMod;
	OgdcRasterBlock* pImgBlock = NULL;

	ScanLine.SetWidth(GetWidth()-nStartCol);	
	ScanLine.SetPixelFormat(GetPixelFormat()) ;		

	ScanLine.SetSize(ScanLine.GetWidthBytes());

	OgdcInt nSCLPos = 0;
	OgdcInt nColRemain = nStartCol & m_nMod;
	for (OgdcInt lCol=nStartCol>>m_nDiv; lCol<m_info.m_nColBlocks; lCol++)
	{
		OgdcInt nIndex = lBlockIndex * m_info.m_nColBlocks + lCol;
		if (m_rasterDict.Lookup(nIndex, pImgBlock))
		{
			if (pImgBlock == NULL || !pImgBlock->IsValid())
			{
				m_rasterDict.RemoveKey(nIndex);
				pImgBlock = this->LoadBlock(lBlockIndex, lCol);
				m_rasterDict.SetAt(nIndex, pImgBlock);
			}
		}
		else
		{
			pImgBlock = this->LoadBlock(lBlockIndex, lCol);
			m_rasterDict.SetAt(nIndex, pImgBlock);
		}

		if(pImgBlock == NULL)
		{		
			return FALSE;
		}
		pImgBlock->AddRefCount();

		if(nSCLPos == 0)
		{
			pImgBlock->GetScanline(lBlockRemain,ScanLine,nSCLPos,nColRemain);
			nSCLPos += m_info.m_nBlockSize-nColRemain;
		}
		else
		{
			pImgBlock->GetScanline(lBlockRemain,ScanLine,nSCLPos,0);
			nSCLPos += m_info.m_nBlockSize;
		}
		pImgBlock->ReduceRefCount();
	}
	return TRUE;
}

OgdcBool OgdcDatasetRasterMdb::SetScanline(OgdcInt nRowIndex,OgdcInt nColIndex, const OgdcRasterScanline& ScanLine)
{
	if(!Open())
	{
		return FALSE;
	}

	if ((nRowIndex<0)||(nRowIndex>GetHeight()))
	{
		return FALSE;
	}

	if((nColIndex<0) || (nColIndex+ScanLine.GetWidth())>GetWidth())
	{
		return FALSE;
	}

	//计算包含该像素的块的行列索引,以及该像素在该块中的偏移	
	if (!m_bInitImg)
	{
		if(!ComDivAndMod())
		{
			return FALSE;
		}
	}

	OgdcInt lRowBlockIndex  = nRowIndex >> m_nDiv;
	OgdcInt lRowBlockRemain = nRowIndex & m_nMod;
	OgdcInt lColBlockIndex  = nColIndex >> m_nDiv;
	OgdcInt lBlkSP = nColIndex & m_nMod;
	OgdcInt lSlSP = 0;
	
	OgdcRasterBlock* pBlock = NULL;

	OgdcInt lCol=lColBlockIndex;
	while (lSlSP<ScanLine.GetWidth())
	{
		OgdcInt nIndex = lRowBlockIndex * m_info.m_nColBlocks + lCol;
		if (m_rasterDict.Lookup(nIndex, pBlock))
		{
			if (pBlock == NULL || !pBlock->IsValid())
			{
				m_rasterDict.RemoveKey(nIndex);
				pBlock = this->LoadBlock(lRowBlockIndex, lCol);
				m_rasterDict.SetAt(nIndex, pBlock);
			}
		}
		else
		{
			pBlock = this->LoadBlock(lRowBlockIndex, lCol);
			m_rasterDict.SetAt(nIndex, pBlock);
		}

		if(pBlock == NULL) 
		{				
			return FALSE;
		}
		pBlock->AddRefCount();
		pBlock->SetScanline(lRowBlockRemain,ScanLine,lSlSP,lBlkSP);
		lSlSP += this->m_info.m_nBlockSize-lBlkSP;
		lBlkSP = 0;
		lCol++;
		pBlock->ReduceRefCount();
	}

	SetModifiedFlag(TRUE);
	return TRUE;
}

OgdcBool OgdcDatasetRasterMdb::ComDivAndMod()
{
	switch (this->m_info.m_nBlockSize)
	{

	case OgdcDatasetRasterInfo::IBS_64:			//! \brief 64*64
		{
			m_nDiv = 6;
			m_nMod = 63;
			break;
		}
	case OgdcDatasetRasterInfo::IBS_128:			//! \brief 128*128
		{
			m_nDiv = 7;
			m_nMod = 127;
			break;
		}
	case OgdcDatasetRasterInfo::IBS_256:			//! \brief 256*256
		{
			m_nDiv = 8;
			m_nMod = 255;
			break;
		}
	case OgdcDatasetRasterInfo::IBS_512:			//! \brief 512*512
		{
			m_nDiv = 9;
			m_nMod = 511;
			break;
		}
	case OgdcDatasetRasterInfo::IBS_1024:			//! \brief 1024*1024
		{
			m_nDiv = 10;
			m_nMod = 1023;
			break;
		}
	case OgdcDatasetRasterInfo::IBS_2048:			//! \brief 2048*2048
		{
			m_nDiv = 11;
			m_nMod = 2047;
			break;
		}
	case OgdcDatasetRasterInfo::IBS_4096:			//! \brief 4096*4096
		{
			m_nDiv = 12;
			m_nMod = 4095;
			break;
		}
	case OgdcDatasetRasterInfo::IBS_8192:			//! \brief 8192*8192
		{	
			m_nDiv = 13;
			m_nMod = 8191;
			break;
		}
	default:
		return FALSE;
	}

	m_bInitImg = TRUE;
	return m_bInitImg;
}

OgdcRasterBlock* OgdcDatasetRasterMdb::LoadBlock( OgdcInt nRowBlock, OgdcInt nColBlock )
{
	OgdcRasterBlock* pRasterBlock = new OgdcRasterBlock;
	
	OgdcString strSQL;
	strSQL.Format(OGDCPCTSTR("SELECT OGSize, OGRaster FROM [%s] "
		"WHERE OGRow = %d AND OGColumn = %d"),
		OGDCPCTSTR(this->m_info.m_strName), nRowBlock, nColBlock);

	CDaoRecordset recordset(&m_pDataSource->m_daoDatabase);
	try
	{
		recordset.Open(dbOpenSnapshot, strSQL, dbForwardOnly);
		COleVariant varValue;
		recordset.GetFieldValue(OG_DIRF_SIZE, varValue);
		DWORD dwSize = 0;
		if (varValue.vt != VT_EMPTY && varValue.vt != VT_NULL)
		{
			dwSize = varValue.lVal;
		}

		COleVariant varArray;
		recordset.GetFieldValue(OG_DIRF_RASTER, varArray);

		BSTR HUGEP *pbstr = NULL;
		long lLBound,lUBound;

		if (FAILED(SafeArrayGetLBound(varArray.parray, 1, &lLBound)))
		{
			return NULL;
		}

		if (FAILED(SafeArrayGetUBound(varArray.parray, 1, &lUBound)))
		{
			return NULL;
		}

		if (FAILED(SafeArrayAccessData(varArray.parray, (void HUGEP* FAR*)&pbstr)))
		{
			return NULL;
		}

		OgdcInt nDataSize = lUBound - lLBound + 1;
		pRasterBlock->Init();
		pRasterBlock->SetByteSize(nDataSize);
		memcpy((void*)pRasterBlock->GetBlockData(), pbstr, nDataSize);

		SafeArrayUnaccessData(varArray.parray);
		
		//填充ImgBlock中的BlockInfo相关信息
		pRasterBlock->m_nPixelFormat = m_info.m_nPixelFormat;
		pRasterBlock->m_nBlockRowIndex = nRowBlock;
		pRasterBlock->m_nBlockColIndex = nColBlock;
		pRasterBlock->m_nPixelRowIndex = nRowBlock * m_info.m_nBlockSize;
		pRasterBlock->m_nPixelColIndex = nColBlock * m_info.m_nBlockSize;
		pRasterBlock->m_nWidth = m_info.m_nBlockSize;
		pRasterBlock->m_nHeight = m_info.m_nBlockSize;

		pRasterBlock->m_nWidthBytes = ((pRasterBlock->m_nWidth 
			* OgdcInt(m_info.m_nPixelFormat)) + 31) / 32 * 4 ;

		//计算一下有效长度和有效的宽度
		OgdcInt blockSize = m_info.m_nBlockSize;
		OgdcInt rowOffset = pRasterBlock->m_nBlockRowIndex, colOffset = pRasterBlock->m_nBlockColIndex;
		OgdcInt width = m_info.m_nWidth, height = m_info.m_nHeight;

		pRasterBlock->m_nValidWidth = (blockSize*(colOffset+1)<width)?blockSize:width-blockSize*colOffset;
		pRasterBlock->m_nValidHeight = (blockSize*(rowOffset+1)<height)?blockSize:height-blockSize*rowOffset;
		
		if (m_info.m_nType == OgdcDataset::Grid)
		{
			pRasterBlock->m_nWidthBytes = ((pRasterBlock->m_nWidth * 32) + 31) 
				/ 32 * 4 ;
		}
		
		if (pRasterBlock->GetByteSize() <= 1) 
		{
			pRasterBlock->Init();
		}
	
		//计算Block的地理坐标
		OgdcDouble dWidthScale = m_info.m_rcBounds.Width()/m_info.m_nWidth;
		OgdcDouble dHeightScale = m_info.m_rcBounds.Height()/m_info.m_nHeight;
		
		OgdcRect2D rc2Bounds;
		rc2Bounds.left = m_info.m_rcBounds.left + (nColBlock * m_info.m_nBlockSize * dWidthScale);
		rc2Bounds.right = rc2Bounds.left + (HIWORD(dwSize) * dWidthScale);
		rc2Bounds.top = m_info.m_rcBounds.top - (nRowBlock * m_info.m_nBlockSize * dHeightScale);
		rc2Bounds.bottom = rc2Bounds.top - (LOWORD(dwSize) * dHeightScale);
		rc2Bounds.Normalize();
		pRasterBlock->m_rcBounds = rc2Bounds;
		
		pRasterBlock->m_bInited = TRUE;
		pRasterBlock->SetModifiedFlag(FALSE);
		recordset.Close();

		return pRasterBlock;
	}
	catch (CDaoException* e)
	{
		e->Delete();
		delete pRasterBlock;
		pRasterBlock = NULL;
		return NULL;
	}

	return NULL;
}

OgdcBool OgdcDatasetRasterMdb::SaveBlock( OgdcRasterBlock* pImgBlock )
{
	if (pImgBlock == NULL)
	{
		return FALSE;
	}
	//这里第二个IsModified为拷贝栅格数据存盘时用，这样就不用一个个栅格块的
	//SetModified，在拷贝数据集完时，修改 isModified为FALSE
	if( pImgBlock->IsModified() || IsModified())
	{
		try
		{
			//定位到指定的记录
			CString strSQL;
			CDaoRecordset recordset(&m_pDataSource->m_daoDatabase);
	
			strSQL.Format(OGDCPCTSTR("SELECT * FROM [%s] "
				"WHERE OGRow = %ld AND OGColumn = %ld"),
				OGDCPCTSTR(m_info.m_strName), pImgBlock->m_nBlockRowIndex, 
				pImgBlock->m_nBlockColIndex);

			recordset.Open(AFX_DAO_USE_DEFAULT_TYPE, strSQL);
			
			//有效矩形,懂了，用四字节装有效矩形，高两位装有效宽度，所以左移16位
			long nSize = (pImgBlock->m_nValidWidth << 16) + pImgBlock->m_nValidHeight;

			recordset.Edit();
			recordset.SetFieldValue(OG_DIRF_SIZE, nSize);
			
			OgdcByte* pData = (unsigned char*)pImgBlock->GetBlockData();
			CByteArray arr;
			arr.SetSize(pImgBlock->GetByteSize());
			for (OgdcInt i = 0; i < pImgBlock->GetByteSize(); i++)
			{
				arr.SetAt(i, pData[i]);
			}
			recordset.SetFieldValue(OG_DIRF_RASTER, arr);
	
			recordset.Update();
			recordset.Close();
			arr.RemoveAll();		
		}
		catch (CDaoException* e)
		{
			e->Delete();
			return FALSE;
		}
		
		pImgBlock->SetModifiedFlag(FALSE);
	}

	return TRUE;	
}

OgdcColor OgdcDatasetRasterMdb::GetPixel( OgdcInt nCol, OgdcInt nRow )
{
	if (nRow < 0 || nCol < 0)
	{
		return 0;
	}
	OgdcInt X = nCol % m_info.m_nBlockSize;
	OgdcInt subCol = (OgdcInt)floor(OgdcDouble(nCol / m_info.m_nBlockSize));
	OgdcInt Y = nRow % m_info.m_nBlockSize;
	OgdcInt subRow = (OgdcInt)floor(OgdcDouble(nRow / m_info.m_nBlockSize));
	OgdcInt nIndex = subRow * m_info.m_nColBlocks + subCol;
	OgdcRasterBlock* pBlock = NULL;

	if (m_rasterDict.Lookup(nIndex, pBlock))
	{
		if (pBlock == NULL || !pBlock->IsValid())
		{
			m_rasterDict.RemoveKey(nIndex);
			pBlock = this->LoadBlock(subRow, subCol);
			m_rasterDict.SetAt(nIndex, pBlock);
		}
	}
	else
	{
		pBlock = this->LoadBlock(subRow, subCol);
		m_rasterDict.SetAt(nIndex, pBlock);
	}

	if (pBlock != NULL)
	{
		return pBlock->GetPixel(X, Y);	
	}
	else
	{
		return 0;
	}
}

void OgdcDatasetRasterMdb::SetPixel( OgdcInt nCol, OgdcInt nRow, OgdcColor nColor )
{
	if (nRow < 0 || nCol < 0)
	{
		return;
	}
	OgdcInt X = nCol % m_info.m_nBlockSize;
	OgdcInt subCol = (OgdcInt)floor(OgdcDouble(nCol / m_info.m_nBlockSize));
	OgdcInt Y = nRow % m_info.m_nBlockSize;
	OgdcInt subRow = (OgdcInt)floor(OgdcDouble(nRow / m_info.m_nBlockSize));
	OgdcInt nIndex = subRow * m_info.m_nColBlocks + subCol;
	OgdcRasterBlock* pBlock = NULL;
	
	if (m_rasterDict.Lookup(nIndex, pBlock))
	{
		if (pBlock == NULL || !pBlock->IsValid())
		{
			m_rasterDict.RemoveKey(nIndex);
			pBlock = this->LoadBlock(subRow, subCol);
			m_rasterDict.SetAt(nIndex, pBlock);
		}
	}
	else
	{
		pBlock = this->LoadBlock(subRow, subCol);
		m_rasterDict.SetAt(nIndex, pBlock);
	}

	if (pBlock != NULL)
	{
		pBlock->SetPixel(X, Y, nColor);
		pBlock->SetModifiedFlag(TRUE);
	}
}

OgdcDouble OgdcDatasetRasterMdb::GetValue( OgdcInt nCol, OgdcInt nRow )
{
	if (nRow < 0 || nCol < 0)
	{
		return 0;
	}
	OgdcInt X = nCol % m_info.m_nBlockSize;
	OgdcInt subCol = (OgdcInt)floor(OgdcDouble(nCol / m_info.m_nBlockSize));
	OgdcInt Y = nRow % m_info.m_nBlockSize;
	OgdcInt subRow = (OgdcInt)floor(OgdcDouble(nRow / m_info.m_nBlockSize));
	OgdcInt nIndex = subRow * m_info.m_nColBlocks + subCol;
	OgdcRasterBlock* pBlock = NULL;
	
	if (m_rasterDict.Lookup(nIndex, pBlock))
	{
		if (pBlock == NULL || !pBlock->IsValid())
		{
			m_rasterDict.RemoveKey(nIndex);
			pBlock = this->LoadBlock(subRow, subCol);
			m_rasterDict.SetAt(nIndex, pBlock);
		}
	}
	else
	{
		pBlock = this->LoadBlock(subRow, subCol);
		m_rasterDict.SetAt(nIndex, pBlock);
	}

	if (pBlock != NULL)
	{
		return pBlock->GetValue(X, Y);
	}
	else
	{
		return 0;
	}
}

void OgdcDatasetRasterMdb::SetValue( OgdcInt nCol, OgdcInt nRow, OgdcDouble dValue )
{
	if (nRow < 0 || nCol < 0)
	{
		return;
	}
	OgdcInt X = nCol % m_info.m_nBlockSize;
	OgdcInt subCol = (OgdcInt)floor(OgdcDouble(nCol / m_info.m_nBlockSize));
	OgdcInt Y = nRow % m_info.m_nBlockSize;
	OgdcInt subRow = (OgdcInt)floor(OgdcDouble(nRow / m_info.m_nBlockSize));
	OgdcInt nIndex = subRow * m_info.m_nColBlocks + subCol;
	OgdcRasterBlock* pBlock = NULL;

	if (m_rasterDict.Lookup(nIndex, pBlock))
	{
		if (pBlock == NULL || !pBlock->IsValid())
		{
			m_rasterDict.RemoveKey(nIndex);
			pBlock = this->LoadBlock(subRow, subCol);
			m_rasterDict.SetAt(nIndex, pBlock);
		}
	}
	else
	{
		pBlock = this->LoadBlock(subRow, subCol);
		m_rasterDict.SetAt(nIndex, pBlock);
	}

	if (pBlock != NULL)
	{
		pBlock->SetValue(X, Y, dValue);
		pBlock->SetModifiedFlag(TRUE);
	}	
}

OgdcRasterBlock* OgdcDatasetRasterMdb::GetViewBlock( const OgdcRect2D& rcView, OgdcInt nDeviceWidth, OgdcInt nDeviceHeight )
{
	OgdcRect2D rcBounds = rcView;
	rcBounds.Normalize();
	rcBounds.IntersectRect(rcBounds, GetBounds());
	
	nDeviceWidth = OGDCROUND(rcBounds.Width()/rcView.Width()*nDeviceWidth);
	nDeviceHeight = OGDCROUND(rcBounds.Height()/rcView.Height()*nDeviceHeight);

	OgdcRect rcImage;
	//地理坐标转化成像素坐标
	XYToImg(rcBounds, rcImage);
	
	if (rcImage.Width() < 2 && rcImage.Height() < 2)
	{
		return NULL;
	}

	OgdcDatasetRasterMdb* pDtR = GetPyramidTier(rcImage.Width(), nDeviceWidth);

	if (pDtR != NULL && pDtR != this)
	{
		if (!pDtR->IsOpen())
		{
			pDtR->Open();
		}
		pDtR->XYToImg(rcBounds, rcImage);
		if (rcImage.Width() < 2 && rcImage.Height() < 2)
		{
			return NULL;
		}
		nDeviceWidth = rcImage.Width();
		nDeviceHeight = rcImage.Height();
		pDtR->ImgToXY(rcImage, rcBounds);
	}
	else
	{
		pDtR = this;
		if (nDeviceWidth > rcImage.Width() || nDeviceHeight > rcImage.Height())
		{
			nDeviceWidth = rcImage.Width();
			nDeviceHeight = rcImage.Height();
			pDtR->ImgToXY(rcImage, rcBounds);
		}
		if (nDeviceWidth < 2 && nDeviceHeight < 2)
		{
			return NULL;
		}
	}	

	OgdcRasterBlock* pResBlock = new OgdcRasterBlock;
	pResBlock->m_nWidth = nDeviceWidth;
	pResBlock->m_nHeight = nDeviceHeight;
	pResBlock->m_nValidWidth = pResBlock->m_nWidth;
	pResBlock->m_nValidHeight = pResBlock->m_nHeight;
	pResBlock->m_nPixelFormat = GetPixelFormat();
	pResBlock->m_rcBounds = rcBounds;
	pResBlock->m_nWidthBytes = ((pResBlock->m_nWidth 
		* OgdcInt(m_info.m_nPixelFormat)) + 31) / 32 * 4 ;
	
	if (GetType() == OgdcDataset::Grid)
	{
		pResBlock->m_nWidthBytes = ((pResBlock->m_nWidth * 32) + 31) / 32 * 4 ;
		pResBlock->InitGrid((OgdcInt)GetNoValue());
	}
	else
	{
		pResBlock->Init();
	}

	OgdcInt nBlockRowStart =  rcImage.top / m_info.m_nBlockSize;
	OgdcInt nBlockRowEnd =  (rcImage.bottom - 1) / m_info.m_nBlockSize;
	OgdcInt nBlockColtart = rcImage.left / m_info.m_nBlockSize;
	OgdcInt nBlockColEnd = (rcImage.right - 1) / m_info.m_nBlockSize;

	OgdcDouble dMaxZ = 0;
	OgdcDouble dMinZ = 0;
	
	OgdcRasterBlock* pSrcBlock = NULL;
	for (OgdcInt i = nBlockRowStart; i <= nBlockRowEnd; i++)
	{
		for (OgdcInt j = nBlockColtart; j <= nBlockColEnd; j++)
		{
			OgdcInt nIndex = i * pDtR->m_info.m_nColBlocks + j;
			if (pDtR->m_rasterDict.Lookup(nIndex, pSrcBlock))
			{
				if (pSrcBlock == NULL || !pSrcBlock->IsValid())
				{
					pDtR->m_rasterDict.RemoveKey(nIndex);
					pSrcBlock = pDtR->LoadBlock(i, j);
					pDtR->m_rasterDict.SetAt(nIndex, pSrcBlock);
				}
			}
			else
			{
				pSrcBlock = pDtR->LoadBlock(i, j);
				pDtR->m_rasterDict.SetAt(nIndex, pSrcBlock);
			}

			if (pSrcBlock != NULL)
			{
				//Load一个栅格块来填充可视栅格块
				/////////////////////////////////////////////////

				if (NULL == pResBlock || pSrcBlock == NULL)
				{
					return FALSE;
				}
				
				PixelFormat srcIPF = pResBlock->m_nPixelFormat;
				PixelFormat destIPF = pSrcBlock->m_nPixelFormat;
				
				if (srcIPF != destIPF)
				{
					return FALSE;
				}
				
				OgdcRect2D rcDestBounds = pResBlock->m_rcBounds;
				OgdcRect2D rcSrcBounds = pSrcBlock->m_rcBounds;
				
				if (!rcSrcBounds.IsIntersect(rcDestBounds))
				{
					return FALSE; 
				}
				
				OgdcDouble dXDestRatio = rcDestBounds.Width() 
					/ pResBlock->m_nValidWidth;
				OgdcDouble dYDestRatio = rcDestBounds.Height() 
					/ pResBlock->m_nValidHeight;
				
				OgdcDouble dXSrcRatio = rcSrcBounds.Width() 
					/ pSrcBlock->m_nValidWidth;
				OgdcDouble dYSrcRatio = rcSrcBounds.Height() 
					/ pSrcBlock->m_nValidHeight;
				
				OgdcRect2D rcIntersectBounds = rcSrcBounds;
				rcIntersectBounds.IntersectRect(rcIntersectBounds, rcDestBounds);
				
				// 算出相交区域在当前数据集的像素的左上角坐标和右下角坐标
				OgdcInt nResLeft = (OgdcInt)floor(
					(rcIntersectBounds.left - rcDestBounds.left) / dXDestRatio);
				OgdcInt nResTop = (OgdcInt)floor(
					(rcDestBounds.top - rcIntersectBounds.top) / dYDestRatio);
				OgdcInt nResRight = (OgdcInt)ceil(
					(rcIntersectBounds.right - rcDestBounds.left) / dXDestRatio);
				OgdcInt nResBottom = (OgdcInt)ceil(
					(rcDestBounds.top - rcIntersectBounds.bottom) / dYDestRatio);
				nResRight = OGDCMIN(nResRight, pResBlock->m_nValidWidth - 1);
				nResBottom = OGDCMIN(nResBottom, pResBlock->m_nValidHeight - 1);
				
				OgdcInt nSrcX = 0;
				OgdcInt nSrcY = 0;
				OgdcBool bRes = FALSE;
		
				for (OgdcInt row = nResTop; row <= nResBottom; row++)
				{
					nSrcY = (OgdcInt)floor((rcSrcBounds.top 
						- (rcDestBounds.top - (row+0.5)*dYDestRatio)) / dYSrcRatio);
					if(nSrcY < 0 || nSrcY >= pSrcBlock->m_nValidHeight)
					{
						continue;
					}
					for (OgdcInt col = nResLeft; col <= nResRight; col++)
					{
						nSrcX = (OgdcInt)floor(((rcDestBounds.left
							+ (col+0.5) * dXDestRatio) - rcSrcBounds.left) / dXSrcRatio);
						if (nSrcX < 0 || nSrcX >= pSrcBlock->m_nValidWidth)
						{
							continue;
						}
						pResBlock->SetValue(col, row, pSrcBlock->GetValue(nSrcX, nSrcY));
						bRes = TRUE;
					}
				}
				/////////////////////////////////////////////////
			}
		}
	}
	
	if (pDtR != this)
	{
		pDtR->Close();
	}
	return pResBlock;
}

OgdcBool OgdcDatasetRasterMdb::IsRaster()
{
	if (m_info.m_nType == OgdcDataset::Grid || m_info.m_nType == OgdcDataset::Image)
	{
		return	TRUE;
	}
	else
	{
		return	FALSE;	
	}
}

OgdcBool OgdcDatasetRasterMdb::IsOpen() const
{
	return m_bOpened;
}

OgdcBool OgdcDatasetRasterMdb::Open()
{

	if (IsOpen())
	{
		return TRUE;
	}

	if (GetType() == OgdcDataset::Grid)
	{
		this->SetGridColorTable(&m_colorTable);	
	}
	m_bOpened = TRUE;

	return TRUE;
}

OgdcBool OgdcDatasetRasterMdb::ReleasePyramidDatasets()
{
	//释放金字塔数据集
	if (IsBuildPyramid())
	{
		OgdcInt nSize = m_pyramidDatasets.GetSize();
		OgdcDatasetRasterMdb* pDataset = NULL;	
		for (OgdcInt i = 0; i < nSize; i++)
		{
			pDataset = m_pyramidDatasets.ElementAt(i);
			if (pDataset != NULL)
			{
				if (pDataset->IsOpen())
				{
					pDataset->Close();
				}
				delete pDataset;	
				pDataset = NULL;
			}
			
		}
		m_pyramidDatasets.RemoveAll();
	}
	return TRUE;
}

void OgdcDatasetRasterMdb::Close()
{
	if (!IsOpen())
	{
		return;
	}
	//非事务方式关闭时存盘
	if (m_pDataSource && (!m_pDataSource->IsTransacted()))
	{
		SaveInfo();
	}	
	
	ReleaseAllBlocks();
	
	m_bOpened = FALSE;	
}

void OgdcDatasetRasterMdb::ReleaseAllBlocks()
{
	OgdcRasterBlock *pBlock = NULL;
	OgdcInt nIndex=0;
	OgdcDict<OgdcInt,OgdcRasterBlock*>::POSITION pos = m_rasterDict.GetStartPosition();
	while(!m_rasterDict.IsEOF(pos))
	{
		m_rasterDict.GetNextAssoc(pos, nIndex, pBlock);
		if(pBlock != NULL)
		{
			if (pBlock->IsModified())
			{
				SaveBlock(pBlock);
			}
			pBlock->Empty();
			delete pBlock;
			pBlock = NULL;
		}
	}

	m_rasterDict.RemoveAll();
}

OgdcBool OgdcDatasetRasterMdb::CanUpdate()
{
	if (m_pDataSource->m_connection.m_bReadOnly == TRUE)
	{
		return	FALSE;
	}
	else
	{
		return TRUE;
	}
	
}
OgdcBool OgdcDatasetRasterMdb::Rename( const OgdcString& strNewName )
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
	CDaoTableDef rasterTable(&m_pDataSource->m_daoDatabase);
	try
	{

		//更新OGImgRegister里的表名
		strSQL.Format(OGDCPCTSTR("UPDATE OGImgRegister "
			"SET OGDatasetName = '%s' WHERE OGDatasetName = '%s'"),
			OGDCPCTSTR(strNewName), OGDCPCTSTR(m_info.m_strName));
		m_pDataSource->m_daoDatabase.Execute(strSQL);

		//更新金字塔名
		strSQL.Format(OGDCPCTSTR("UPDATE OGImgRegister "
			"SET OGPyramid = '%s' WHERE OGPyramid = '%s'"),
			OGDCPCTSTR(strNewName), OGDCPCTSTR(m_info.m_strName));
		m_pDataSource->m_daoDatabase.Execute(strSQL);
		rasterTable.Open(m_info.m_strName);
		rasterTable.SetName(strNewName);
		rasterTable.Close();
	}
	catch (CDaoException* e)
	{
		e->Delete();
		rasterTable.Close();
		m_pDataSource->m_daoDatabase.Close();
		return FALSE;
	}
	m_info.m_strName = strNewName;
	return TRUE;	
}

OgdcBool OgdcDatasetRasterMdb::SaveInfo()
{
 	if (!m_pDataSource->m_daoDatabase.IsOpen())
 	{
		return FALSE;
	}
	if (!IsModified())
	{
		return TRUE;
	}
	CDaoRecordset recordset(&m_pDataSource->m_daoDatabase);
	CString strSQL;
	strSQL.Format(OGDCPCTSTR("SELECT * FROM OGImgRegister "
		"WHERE OGDatasetName = '%s' OR OGDatasetID = %d"), 
		OGDCPCTSTR(m_info.m_strName), m_nID);

	CDaoTableDef tableDef(&m_pDataSource->m_daoDatabase);
	try
	{
		//保存注册表的信息
		recordset.Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);
		recordset.Edit();

		recordset.SetFieldValue (OGRF_DATASET_NAME,
			COleVariant(m_info.m_strName, VT_BSTRT));
		recordset.SetFieldValue (OGRF_DATASET_TYPE,long(m_info.m_nType));
		recordset.SetFieldValue (OGIRF_PIXELFORMAT,(long)(m_info.m_nPixelFormat));
		recordset.SetFieldValue (OGIRF_WIDTH,long(m_info.m_nWidth));	
		recordset.SetFieldValue (OGIRF_HEIGHT,long(m_info.m_nHeight));
		recordset.SetFieldValue (OGIRF_EBLOCKSIZE,(long)(m_info.m_nBlockSize));
		recordset.SetFieldValue (OGIRF_LEFT,m_info.m_rcBounds.left);
		recordset.SetFieldValue (OGIRF_TOP,m_info.m_rcBounds.top);
		recordset.SetFieldValue (OGIRF_RIGHT,m_info.m_rcBounds.right);
		recordset.SetFieldValue (OGIRF_BOTTOM,m_info.m_rcBounds.bottom);
		recordset.SetFieldValue (OGIRF_MAXZ, m_info.m_dMaxZ);
		recordset.SetFieldValue (OGIRF_MINZ, m_info.m_dMinZ);
		recordset.Update ();
		recordset.Close();
	}
	catch (CDaoException* e)
	{
		e->Delete();
		recordset.Close();
		SetModifiedFlag(FALSE);
		return FALSE;	
	}

	SetModifiedFlag(FALSE);
	return TRUE;	
}

OgdcBool OgdcDatasetRasterMdb::RefreshInfo()
{
	if (!m_pDataSource->m_daoDatabase.IsOpen())
	{
		return FALSE;
	}
	CDaoRecordset recordset(&m_pDataSource->m_daoDatabase);
	OgdcString strSQL;
	strSQL.Format(OGDCPCTSTR("SELECT * FROM OGImgRegister "
		"WHERE OGDatasetID = %d"), m_nID);
	CDaoTableDef tableDef(&m_pDataSource->m_daoDatabase);
	COleVariant varValue;
	try
	{
		recordset.Open(AFX_DAO_USE_DEFAULT_TYPE,strSQL);
		recordset.GetFieldValue(OGRF_DATASET_NAME,varValue);
		this->m_info.m_strName = OgdcString((LPCTSTR)varValue.bstrVal);
		recordset.GetFieldValue(OGRF_DATASET_TYPE,varValue);
		this->m_info.m_nType = (OgdcDataset::Type)varValue.lVal;
		recordset.GetFieldValue(OGIRF_LEFT,varValue);
		this->m_info.m_rcBounds.left = varValue.dblVal;
		recordset.GetFieldValue(OGIRF_RIGHT,varValue);
		this->m_info.m_rcBounds.right = varValue.dblVal;
		recordset.GetFieldValue(OGIRF_TOP,varValue);
		this->m_info.m_rcBounds.top = varValue.dblVal;
		recordset.GetFieldValue(OGIRF_BOTTOM,varValue);
		this->m_info.m_rcBounds.bottom = varValue.dblVal;
		recordset.GetFieldValue(OGIRF_WIDTH,varValue);
		this->m_info.m_nWidth = varValue.lVal;
		recordset.GetFieldValue(OGIRF_HEIGHT,varValue);
		this->m_info.m_nHeight = varValue.lVal;
		recordset.GetFieldValue(OGIRF_PIXELFORMAT,varValue);
		this->m_info.m_nWidthBytes  =  (this->m_info.m_nWidth 
			* OgdcInt(varValue.lVal) + 31) / 32 * 4 ;
		this->m_info.m_nPixelFormat = (PixelFormat)varValue.lVal;
		recordset.GetFieldValue(OGIRF_EBLOCKSIZE,varValue);
		this->m_info.m_nColBlocks  =  (OgdcInt)ceil(
			double(this->m_info.m_nWidth) / double(varValue.lVal));
		this->m_info.m_nRowBlocks  =  (OgdcInt)ceil(
			double(this->m_info.m_nHeight) / double(varValue.lVal));
		this->m_info.m_nBlockSize = (OgdcDatasetRasterInfo::IBSizeOption)varValue.lVal;
		recordset.MoveNext();
	}
	catch (CDaoException* e)
	{
		e->Delete();
		recordset.Close();
		return FALSE;	
	}

	return TRUE;	
}

OgdcBool OgdcDatasetRasterMdb::IsModified() const
{
	return m_bModified;	
}

void OgdcDatasetRasterMdb::SetModifiedFlag( OgdcBool bModified /*= TRUE*/ )
{
	m_bModified = bModified;
}

void OgdcDatasetRasterMdb::XYToImg( const OgdcRect2D& rectXY,OgdcRect& rectImg )
{
	m_rcViewGeoRef = m_info.m_rcBounds;
	//分辨率
	//水平方向地理坐标与影像映射比例.
	m_imgRatio.cx = (m_rcViewGeoRef.Width()) / (OgdcDouble)GetWidth();	
	//垂直方向地理坐标与影像映射比例.
	m_imgRatio.cy = (m_rcViewGeoRef.Height()) / (OgdcDouble)GetHeight(); 

	//地理坐标转化成像素坐标
	rectImg.left =	(OgdcInt)floor(( rectXY.left - m_rcViewGeoRef.left ) 
		/ m_imgRatio.cx);
	rectImg.right = (OgdcInt)ceil(( rectXY.right - m_rcViewGeoRef.left ) 
		/ m_imgRatio.cx);
	//翻转垂直朝向.
	rectImg.top	= (OgdcInt)floor(GetHeight() 
		- (rectXY.top - m_rcViewGeoRef.bottom) / m_imgRatio.cy);	
	//翻转垂直朝向.
	rectImg.bottom = (OgdcInt)ceil(GetHeight() 
		- (rectXY.bottom - m_rcViewGeoRef.bottom) / m_imgRatio.cy);	
}

void OgdcDatasetRasterMdb::ImgToXY( const OgdcRect& rectImg,OgdcRect2D& rectXY )
{
	m_rcViewGeoRef = m_info.m_rcBounds;
	//分辨率
	//水平方向地理坐标与影像映射比例.
	m_imgRatio.cx = (m_rcViewGeoRef.Width()) / (OgdcDouble)GetWidth();	
	//垂直方向地理坐标与影像映射比例.
	m_imgRatio.cy = (m_rcViewGeoRef.Height()) / (OgdcDouble)GetHeight(); 

	//像素坐标转化成地理坐标	
	rectXY.left = (rectImg.left * m_imgRatio.cx + m_rcViewGeoRef.left);
	rectXY.right = (rectImg.right * m_imgRatio.cx + m_rcViewGeoRef.left) ;
	//翻转垂直朝向.
	rectXY.top =  ((GetHeight() - rectImg.top) * m_imgRatio.cy 
		+ m_rcViewGeoRef.bottom);
	//翻转垂直朝向.
	rectXY.bottom = ((GetHeight() - rectImg.bottom) * m_imgRatio.cy 
		+ m_rcViewGeoRef.bottom);	
}

OgdcDatasetRasterMdb * OgdcDatasetRasterMdb::GetPyramidTier( OgdcInt nImgWidth, OgdcInt nDeviceWidth )
{
	//获取金字塔的相应的层
	OgdcInt nSize = m_pyramidDatasets.GetSize();
	if (nSize <= 0)
	{
		return this;
	}

	OgdcDouble dLevel = (OgdcDouble)log(1.0 * nImgWidth / nDeviceWidth) / log(2.0);
	if (dLevel < 1)
	{
		return this;
	}
	else
	{
		OgdcInt nLevel = OgdcInt(dLevel) - 1;
		if (nLevel >= nSize)
			nLevel = nSize - 1;
		if (nLevel < 0)
		{
			nLevel = 0;
		}	
		return m_pyramidDatasets.GetAt(nLevel);
	}
	return this;
}

OgdcDatasetRasterMdb* OgdcDatasetRasterMdb::CreateDatasetPyramid( OgdcDatasetRasterInfo& rInfo, OgdcInt nLevel )
{
	
	OgdcDatasetRasterMdb *pDatasetRaster = new OgdcDatasetRasterMdb(this->m_pDataSource);
	OgdcString strName;
	strName.Format("%sTier%d", OGDCPCTSTR(m_info.m_strName), nLevel); 
	//调色板,如果是黑白，默认加上调色板
	if (rInfo.m_nPixelFormat== IPF_MONO)
	{
		PALETTEENTRY paletteentry;
		paletteentry.peRed = 0;
		paletteentry.peGreen = 0;
		paletteentry.peBlue = 0;
		paletteentry.peFlags = 0;
		pDatasetRaster->m_colorTable.Add(paletteentry.peRed, 
			paletteentry.peGreen, paletteentry.peBlue);
		paletteentry.peRed = 255;
		paletteentry.peGreen = 255;
		paletteentry.peBlue = 255;
		paletteentry.peFlags = 255;
		pDatasetRaster->m_colorTable.Add(paletteentry.peRed,
			paletteentry.peGreen,paletteentry.peBlue);
	}

	CDaoTableDef rasterTable(&m_pDataSource->m_daoDatabase);	
	OgdcFieldInfos fieldInfos;

	fieldInfos.AddField(OGDCPCTSTR("OGRow"), OgdcFieldInfo::INT32,
		sizeof(OgdcLong), 0, FALSE, FALSE);
	fieldInfos.AddField(OGDCPCTSTR("OGColumn"), OgdcFieldInfo::INT32,
		sizeof(OgdcLong), 0, FALSE, FALSE);
	fieldInfos.AddField(OGDCPCTSTR("OGSize"), OgdcFieldInfo::INT32,
		sizeof(OgdcLong), 0, FALSE, FALSE);
	fieldInfos.AddField(OGDCPCTSTR("OGRaster"), 
		OgdcFieldInfo::LongBinary,0 , 0, FALSE, FALSE);

	OgdcLong nFields = fieldInfos.GetSize();
	CDaoFieldInfo info;
	try
	{
		rasterTable.Create(strName);
		for (OgdcLong i = 0; i < nFields; i++)
		{ 
			OgdcHelperMdb::OgdcFieldInfo2CDaoFieldInfo(fieldInfos.GetAt(OgdcInt(i)), info);
			rasterTable.CreateField(info);		
		}	
		rasterTable.Append();
		rasterTable.Close();
	}
	catch (CDaoException* e)
	{
		e->Delete();
		rasterTable.Close();
		return NULL;
	}
	
	//根据宽高度得到行列的分块数
	pDatasetRaster->m_info = rInfo;
	pDatasetRaster->m_info.m_nColBlocks = (rInfo.m_nWidth 
		+ rInfo.m_nBlockSize - 1) / rInfo.m_nBlockSize ;
	pDatasetRaster->m_info.m_nRowBlocks = (rInfo.m_nHeight 
		+ rInfo.m_nBlockSize  - 1) / rInfo.m_nBlockSize ;
	
	//下面要准备向注册表中写入信息
	OgdcInt m_blockSizes;
 	//PixelFormat是关键
 	switch (rInfo.m_nPixelFormat)
 	{
 	case IPF_MONO ://1位,单色z
 		m_blockSizes = rInfo.m_nBlockSize * rInfo.m_nBlockSize / 8;
 		break;
 	case IPF_FBIT:		//4位,16色
 		m_blockSizes = rInfo.m_nBlockSize * rInfo.m_nBlockSize / 2;
 		break;
 	case IPF_BYTE :		//8位,256色
 		m_blockSizes = rInfo.m_nBlockSize * rInfo.m_nBlockSize;
 		break;
 	case IPF_TBYTE :	//16位,彩色
 		m_blockSizes = rInfo.m_nBlockSize * rInfo.m_nBlockSize * 2;
 		break;
 	case IPF_RGB :		//24位,真彩色
 		m_blockSizes = rInfo.m_nBlockSize * rInfo.m_nBlockSize * 3;
 		break;
 	case IPF_RGBA :		//32位,增强真彩色,颠倒存储
 		m_blockSizes = rInfo.m_nBlockSize * rInfo.m_nBlockSize * 4;
 		break;
 	case IPF_LONG :		//64位,整型,不颠倒存储
 		m_blockSizes = rInfo.m_nBlockSize * rInfo.m_nBlockSize * 4;
 		break;
 	case IPF_LONGLONG :	//64位,长整型
 		m_blockSizes = rInfo.m_nBlockSize * rInfo.m_nBlockSize * 8;
 		break;
 	case IPF_FLOAT :	//32位,浮点型
 		m_blockSizes = rInfo.m_nBlockSize * rInfo.m_nBlockSize * 4;
 		break;
 	case IPF_DOUBLE :	 //64位,双精度浮点型
 		m_blockSizes = rInfo.m_nBlockSize * rInfo.m_nBlockSize * 8;
 		break;
 	default:	
 		break;
 	}
	//得到最大值
	CDaoRecordset recordset(&m_pDataSource->m_daoDatabase);
	COleVariant lRecordMaxId;
	CString strSQL;
	strSQL.Format (OGDCPCTSTR("SELECT MAX(OGDatasetID) FROM OGImgRegister"));
	try
	{
		recordset.Open (dbOpenSnapshot,strSQL);
		lRecordMaxId = recordset.GetFieldValue (0);
		recordset.Close ();
	}
	catch (CDaoException* e )
	{
		//获取最大ID，异常时为零记录
		e->Delete();
	}
	OgdcLong lMaxID;
	if (lRecordMaxId.vt == VT_NULL || lRecordMaxId.vt == VT_EMPTY)
	{
		lMaxID = 0;
	}
	else
	{
		lMaxID = lRecordMaxId.lVal;
	}
	lMaxID++;
	
	//加入注册表
	CDaoTableDef table(&m_pDataSource->m_daoDatabase);
	try
	{
		table.Open(OGDCPCTSTR(OG_IMGREGISTER_TABLE));
		recordset.Open(&table);
 		recordset.AddNew ();

 		recordset.SetFieldValue (OGRF_ID,long(lMaxID));
 		recordset.SetFieldValue (OGRF_DATASET_NAME, COleVariant(strName, VT_BSTRT));
 		recordset.SetFieldValue (OGRF_DATASET_TYPE, long(rInfo.m_nType));
 		recordset.SetFieldValue (OGIRF_PIXELFORMAT, long(rInfo.m_nPixelFormat));
 		recordset.SetFieldValue (OGIRF_WIDTH, long(rInfo.m_nWidth));	
 		recordset.SetFieldValue (OGIRF_HEIGHT, long(rInfo.m_nHeight));
 		recordset.SetFieldValue (OGIRF_EBLOCKSIZE, long(rInfo.m_nBlockSize));
		recordset.SetFieldValue (OGIRF_PYRAMID, OGDCPCTSTR(rInfo.m_strName));
		recordset.SetFieldValue (OGIRF_PYRAMIDLEVEL, (long)nLevel);
		recordset.SetFieldValue (OGIRF_BLOCKSIZES, long(m_blockSizes));
		recordset.SetFieldValue (OGIRF_LEFT, rInfo.m_rcBounds.left);
		recordset.SetFieldValue (OGIRF_TOP, rInfo.m_rcBounds.top);
		recordset.SetFieldValue (OGIRF_RIGHT, rInfo.m_rcBounds.right);
		recordset.SetFieldValue (OGIRF_BOTTOM, rInfo.m_rcBounds.bottom);
		recordset.SetFieldValue (OGIRF_MAXZ, this->m_info.m_dMaxZ);
		recordset.SetFieldValue (OGIRF_MINZ, this->m_info.m_dMinZ);

		recordset.Update ();
		recordset.Close();
		table.Close();	
	}
	catch (CDaoException* e)
	{
		recordset.Close();
		table.Close();
		e->Delete();
		return NULL;
	}

	//注册表修改完毕！
	pDatasetRaster->m_info.m_strName = strName;

	//开始填充
 	OgdcLong lWidthRemain = rInfo.m_nWidth % rInfo.m_nBlockSize;
 	OgdcLong lHeightRemain = rInfo.m_nHeight % rInfo.m_nBlockSize;
 	if ( lWidthRemain == 0 )
 	{
 		lWidthRemain = rInfo.m_nBlockSize;
 	}
 	if ( lHeightRemain == 0 )
 	{
 		lHeightRemain = rInfo.m_nBlockSize;
 	}
 	
 	strSQL.Format( "SELECT * FROM [%s]", strName.Cstr());
	CDaoRecordset newRecord(&m_pDataSource->m_daoDatabase);
 	try
 	{
 		newRecord.Open(AFX_DAO_USE_DEFAULT_TYPE, strSQL);	
 		OgdcSize szValid;
 		CByteArray btEmptyData; 
 		btEmptyData.Add((BYTE)-1);
 		for (OgdcInt i = 0; i < pDatasetRaster->m_info.m_nRowBlocks; i++)
		{
 			for (OgdcInt j = 0; j<pDatasetRaster->m_info.m_nColBlocks; j++)
 			{
 				//记录头
 				newRecord.AddNew();
 				newRecord.SetFieldValue(OG_DIRF_ROW, (long)i);
 				newRecord.SetFieldValue(OG_DIRF_COLUMN, (long)j);				
 				//有效矩形
 				szValid.cx = OgdcInt((j == pDatasetRaster->m_info.m_nColBlocks-1) 
					? lWidthRemain : pDatasetRaster->m_info.m_nBlockSize);
 				szValid.cy = OgdcInt((i == pDatasetRaster->m_info.m_nRowBlocks-1)
					? lHeightRemain : pDatasetRaster->m_info.m_nBlockSize);
 				long  lSize = (szValid.cx << 16) + szValid.cy;
 				newRecord.SetFieldValue(OG_DIRF_SIZE, lSize);	
				newRecord.SetFieldValue(OG_DIRF_RASTER, btEmptyData);
 				newRecord.Update();
 			}
		}	
 		newRecord.Close();
 	}
 	catch (CDaoException* e)
 	{
 		e->Delete();
		newRecord.Close();
 	}

	return pDatasetRaster;  	
}

OgdcBool OgdcDatasetRasterMdb::SetGridColorTable( OgdcColorTable* pColorset )
{

	if (m_info.m_dMinZ == 0 && m_info.m_dMaxZ == 0)
	{		
		if (pColorset->GetCeiling() != m_info.m_dMaxZ || pColorset->GetFloor() != m_info.m_dMinZ)
		{
			m_info.m_dMinZ = pColorset->GetFloor();
			m_info.m_dMaxZ = pColorset->GetCeiling();
			SetModifiedFlag(TRUE);
		}
	}
	else
	{
		m_colorTable.CreateDefault();	

		m_colorTable.SetCeiling(m_info.m_dMaxZ);
		m_colorTable.SetWaterLevel(m_info.m_dMinZ);
		m_colorTable.SetFloor(m_info.m_dMinZ);			
	}
	
	return TRUE;
}

OgdcDataSource* OgdcDatasetRasterMdb::GetDataSource() const
{
	return (OgdcDataSource*)m_pDataSource;
}

}