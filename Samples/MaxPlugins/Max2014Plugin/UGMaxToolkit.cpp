
#include "UGMaxToolkit.h"
#include "UGMaxLogFile.h"
#include "Base3D/UGMathEngine.h"
#include "Toolkit/UGImgToolkit.h"
#include "FileParser/UGFileParseFactory.h"
#include "FileParser/UGExchangeFileType.h"
#include "FileParser/UGFileParseManager.h"
#include "FileParser/UGExchangeParamsManager.h"
#include "FileParser/UGFileParseToolkit.h"

#include <osgDB/ReadFile>
#include <osg/Image>
#include <osg/ref_ptr>
#include "Toolkit/UGImageOperator.h"

USE_OSGPLUGIN(dds)

#define RI 6378137
#define MPI 3.1415926535897932384626433833
	using namespace UGC;

std::set<UGString> UGMaxToolkit::m_setRegionZ;
std::set<UGString> UGMaxToolkit::m_setBillboardList;

UGC::UGRenderOperationNode::TextureDataInfo* UGMaxToolkit::ImageTOTextureDataInfo(UGC::UGString strTex)
{
	// 通过OSGB加载dds
#ifdef _UGUNICODE
	UGMBString strTemp;
	strTex.ToMBString(strTemp);
#else
	UGString strTemp = strTexPath;
#endif

	osg::ref_ptr<osg::Image> image = osgDB::readImageFile(strTemp.Cstr());
	// image 转TextureData

	if(image)
	{
		GLenum          osgPixelFormat = image->getPixelFormat();
		UG3DPixelFormat ugcPixelFormat = GlPixelFormat2UG3DFormat(osgPixelFormat);
		UGint           nByteSize;
		if(image->getNumMipmapLevels() > 1)
		{
			nByteSize = image->getTotalSizeInBytesIncludingMipmaps();
		}

		if(nByteSize < 1)
		{
			nByteSize = image->s() * image->t() * 4;//image->getTotalDataSize();
		}

		UGuchar * pUchar = (UGuchar*)image->getDataPointer();
		if (NULL == pUchar)
		{
			return NULL;
		}

		UGuchar * pData = new UGuchar[nByteSize];
		if (NULL == pData)
		{
			return NULL;
		}

		if (pUchar && pData)
		{
			UGTextureData * osgTextureData = new UGTextureData;
			osgTextureData->m_pBuffer	= pData;
			osgTextureData->m_nWidth	= image->s();
			osgTextureData->m_nHeight	= image->t();
			osgTextureData->m_nDepth = image->r();
			osgTextureData->m_nSize	=  nByteSize;
			osgTextureData->m_enFormat = ugcPixelFormat;

			UGint nCompressType = 0;
			if(osgPixelFormat == GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG || osgPixelFormat == GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG)
			{
				nCompressType = UGDataCodec::enrPVRTPF_PVRTC;
			}
			else if(osgPixelFormat == GL_ETC1_RGB8_OES)
			{
				nCompressType = UGDataCodec::enrPVRTPF_ETC1;
			}

			if(nCompressType != 0)
			{
#ifndef OPENGL_ES_VERSION2
				delete[] pData;

				UGbyte* pTempData = NULL;
				UGint nPerPixelBytes =(ugcPixelFormat == PF_BYTE_RGB) ? 3 : 4;
				osgTextureData->m_nSize = UGImageOperator::Decode(nPerPixelBytes, osgTextureData->m_nWidth, osgTextureData->m_nHeight, 
					&pTempData, pUchar, nCompressType);

				osgTextureData->m_pBuffer = pTempData;
				pData = pTempData;
#endif
				osgTextureData->m_enFormat = PF_BYTE_RGBA;
			}
			else
			{
				UGint i = 0,j = 0, k = 0;
				if (ugcPixelFormat ==PF_BYTE_RGB)
				{
					for (i=0;i<osgTextureData->m_nWidth*osgTextureData->m_nHeight;i++,j+=3,k+=3)
					{
						pData[j]   = pUchar[k+2]; //r
						pData[j+1] = pUchar[k+1]; //g
						pData[j+2] = pUchar[k];	//b
					}
					osgTextureData->m_enFormat = PF_BYTE_BGR;
				}
				else
				{
					switch (ugcPixelFormat)
					{
					case PF_BYTE_RGBA:
						osgTextureData->m_enFormat = PF_BYTE_RGBA;
						break;

					case PF_DXT1:
						osgTextureData->m_enFormat = PF_BYTE_RGB;
						osgTextureData->m_CompressType = UGDataCodec::enrS3TCDXTN;
						break;

					case PF_DXT5:
						osgTextureData->m_enFormat = PF_BYTE_RGBA;
						osgTextureData->m_CompressType = UGDataCodec::enrS3TCDXTN;
						break;

					case PF_L8:
						osgTextureData->m_enFormat = PF_L8;
						break;

					default:
						break;
					}

					memcpy(pData, pUchar, nByteSize * sizeof(UGuchar));
				}
			}

			UGRenderOperationNode::TextureDataInfo* pTextureDataInfo = new UGRenderOperationNode::TextureDataInfo;
			pTextureDataInfo->m_pTextureData = osgTextureData;
			if(image->getNumMipmapLevels() > 1 && nCompressType == 0)
			{
				pTextureDataInfo->m_bMipmap = TRUE;
				pTextureDataInfo->m_nLevel = image->getNumMipmapLevels();
			}
			pData = NULL;
			return pTextureDataInfo;
		}
	}

	return NULL;
}

UGC::UG3DPixelFormat UGMaxToolkit::GlPixelFormat2UG3DFormat(UGuint osgPixelFormat)
{
	UG3DPixelFormat pixelFormat;
	switch (osgPixelFormat)
	{
	case GL_RGB:
		pixelFormat = PF_BYTE_RGB;
		break;
	case GL_RGBA:
		pixelFormat = PF_BYTE_RGBA;
		break;
	case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
	case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
		pixelFormat = PF_DXT1;
		break;
	case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
	case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
		pixelFormat = PF_DXT5;
		break;
	case GL_LUMINANCE:
		pixelFormat = PF_L8;
		break;
	case GL_LUMINANCE_ALPHA:
		pixelFormat = PF_BYTE_LA;
		break;
	case GL_LUMINANCE32F_ARB:
		pixelFormat = PF_FLOAT32_R;
		break;
	case GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG:
		pixelFormat = PF_BYTE_RGB;
		break;
	case GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG:
		pixelFormat = PF_BYTE_RGBA;
		break;
	case GL_ETC1_RGB8_OES:
		pixelFormat = PF_BYTE_RGBA;
		break;
	case GL_ALPHA:
		pixelFormat = PF_A8;
		break;
	default:
		pixelFormat = PF_UNKNOWN;
		break;
	}

	return pixelFormat;
}

int CALLBACK BrowseCallbackProc( HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData)
{
	switch (uMsg)
	{
	case BFFM_INITIALIZED:
		SendMessage(hwnd,BFFM_SETSELECTION,TRUE,lpData);
		break;
	}
	return 0;
}
UGString UGMaxToolkit::GetInputPath(TCHAR* dialogName,int IDC,HWND hDlg,bool setText,TCHAR* pInitialPath )
{
	TCHAR* pDespath = new TCHAR[MAX_PATH];
	BROWSEINFO bi;
	TCHAR dispname[MAX_PATH];
	ITEMIDLIST* pidl;
	bi.hwndOwner=hDlg;
	bi.pidlRoot=0;
	bi.pszDisplayName=dispname;
	bi.lpszTitle= dialogName;
	bi.ulFlags=BIF_RETURNONLYFSDIRS|BIF_NEWDIALOGSTYLE;
	bi.lpfn=BrowseCallbackProc;
	bi.lParam=(LPARAM)pInitialPath;
	bi.iImage=0;

	UGString pathName;
	if (pidl=SHBrowseForFolder(&bi)) 
	{
		SHGetPathFromIDList(pidl,pDespath);
		TCHAR* pc=pDespath;
		while(*pc)pc++;
		if(*(pc-1)!='\\')
		{
			*pc='\\';
			pc++;
			*pc=0;
		}

		if(setText)
		{
			SetDlgItemText(hDlg,IDC,pDespath);
		}

		UGString pathNameTemp(pDespath);
		pathName=pathNameTemp;
	}

	if (pDespath != NULL)
	{
		delete[] pDespath;
		pDespath = NULL;
	}
	return pathName;
}

void UGMaxToolkit::TransStandDir(TCHAR* pDespath)
{
	while(*pDespath)
	{
		pDespath++;
	}

	if(*(pDespath-1)!='\\')
	{
		*pDespath='\\';
		pDespath++;
		*pDespath=0;
	}
}

bool UGMaxToolkit::IsInt(LPTSTR intStr)
{
	LPTSTR ptemp=intStr;
	while(*ptemp)
	{
		if((*ptemp)<'0'||(*ptemp)>'9')
			return false;
		ptemp++;
	}
	return true;
}
bool UGMaxToolkit::IsFloat(LPTSTR floStr)
{
	LPTSTR ptemp=floStr;
	if(*ptemp=='-')
	{
		ptemp++;
	}
	int dotNum=0;
	while(*ptemp)
	{
		if(((*ptemp)<'0'||(*ptemp)>'9')&&(*ptemp)!='.')
			return false;

		if((*ptemp)=='.')
		{
			dotNum++;
			if(dotNum>1)return false;
		}
		ptemp++;
	}
	return true;
}

//! \cmpathName文件生成路径，cfileName不带后缀名的文件名，m_arrmodelProp模型信息表，如果存在文件则视为追加数据，若不存在则创建
//void UGMaxToolkit::CreateUDB(UGString cmpathName,UGString cfileName,UGArray<ModelProp>m_arrmodelProp,
//							 double longitude,double latitude,double altitude)
//{
//	UGC::UGDataSource* pUDBDataSource = UGC::UGDataSourceManager::CreateDataSource(UGC::UDB);
//	pUDBDataSource->m_nEngineClass = 2;
//	pUDBDataSource->GetConnectionInfo().m_strServer = cmpathName+cfileName;
//	UGString UDBFileName=cmpathName+cfileName+_U(".udb");//   Modify by lina 2014-3-4 字符转换
//	UGString strModelName = _U("ModelName");
//	UGString strLongitudeColumn = _U("Longitude");
//	UGString strLatitudeColumn= _U("Latitude");
//	UGString strAltitudeColumn = _U("Altitude");
//	if (NULL == pUDBDataSource)
//	{
//		return ;
//	}
//	UGC::UGDatasetVector* pTempVector=NULL;
//	UGC::UGRecordset* pRecordset=NULL;
//	if(!UGC::UGFile::IsExist(UDBFileName))
//	{
//		if(!((UGC::UGDataSource*)pUDBDataSource)->Create())
//		{
//			delete pUDBDataSource;
//			return ;
//		}
//		UGC::UGDatasetVectorInfo tempDatasetVectorInfo;
//		UGString tempDatasetName= cfileName;	
//		tempDatasetVectorInfo.m_strName = tempDatasetName;
//		tempDatasetVectorInfo.m_nType	= UGC::UGDataset::Tabular;
//		/*tempDatasetVectorInfo.m_nType	= UGDataset::CAD;*/
//		pTempVector =  pUDBDataSource->CreateDatasetVector(tempDatasetVectorInfo);
//		if(pTempVector == NULL)
//		{
//			delete pUDBDataSource;
//			return ;
//		}
//
//		pTempVector->CreateField(strModelName,UGC::UGFieldInfo::Text ,30);
//
//		pTempVector->CreateField(strLongitudeColumn,UGC::UGFieldInfo::Double ,8);
//
//		pTempVector->CreateField(strLatitudeColumn,UGC::UGFieldInfo::Double,8);
//
//		pTempVector->CreateField(strAltitudeColumn,UGC::UGFieldInfo::Double,8);
//		UGC::UGQueryDef queryDef;
//		queryDef.m_nOptions = UGC::UGQueryDef::Attribute;	
//		pRecordset = pTempVector->Query(queryDef);
//		if(pRecordset == NULL)
//		{
//			delete pUDBDataSource;
//			return ;
//		}
//		pRecordset->MoveFirst();
//
//	}
//	else
//	{
//		if(!((UGC::UGDataSource*)pUDBDataSource)->Open())
//		{
//			delete pUDBDataSource;
//			return ;
//		}
//		//打开已有数据集
//		UGC::UGDataset* p =pUDBDataSource->GetDataset(cfileName);
//		if(!p)
//		{
//			return;
//		}
//		pTempVector=(UGC::UGDatasetVector*)p;//是否可以强制转换?
//		UGC::UGQueryDef queryDef;
//		queryDef.m_nOptions = UGC::UGQueryDef::Attribute;	
//		pRecordset = pTempVector->Query(queryDef);
//
//
//		pRecordset->MoveLast();
//	}
//	UGC::UGint i =0;
//	UGC::UGGeometry* pGeometryNULL = NULL;
//	UGC::UGint count=m_arrmodelProp.GetSize();
//	for(i=0;i<count;i++)
//	{
//		ModelProp modelProp = m_arrmodelProp.GetAt(i);
//
//		pRecordset->AddNew(pGeometryNULL);
//		pRecordset->SetFieldValue(strModelName, UGC::UGFile::GetTitle(modelProp.strName));
//		UGC::UGPoint3D sphCoordinate(modelProp.vPostion.x,modelProp.vPostion.y,modelProp.vPostion.z); 
//		CartesianToSphericalD(longitude,latitude,altitude,sphCoordinate);
//		pRecordset->SetFieldValue(strLongitudeColumn,sphCoordinate.x);
//		pRecordset->SetFieldValue(strLatitudeColumn,sphCoordinate.y);
//		pRecordset->SetFieldValue(strAltitudeColumn,sphCoordinate.z);				
//		pRecordset->Update();
//	}
//	pTempVector->ReleaseRecordset(pRecordset);
//	pRecordset = NULL;
//	pTempVector->SaveInfo();
//	pUDBDataSource->Close();
//	delete pUDBDataSource;
//}
#define ISALPHA(a)  ((a<='z'&&a>='a')||(a<='Z'&&a>='A'))
#define ISNUM(a)  ((a<='9'&&a>='0'))

bool UGMaxToolkit::IslegalName(UGString strName)
{
	TCHAR* pName = (TCHAR* )strName.Cstr();
	if (!ISALPHA(*pName))
	{
		return false;
	}
	pName++;
	while(*pName)
	{
		if ((!ISALPHA(*pName))&& (!ISNUM(*pName)))
		{
			return false;
		}
		pName++;
	}
	return true;
}

//void UGMaxToolkit::CreateUDBWithFilePath(UGString cmpathName,UGString cfileName,
//										 UGArray<ModelProp>m_arrmodelProp,double longitude,double latitude,double altitude)
//{
//	UGC::UGDataSource* pUDBDataSource = UGC::UGDataSourceManager::CreateDataSource(UGC::UDB);
//	pUDBDataSource->m_nEngineClass = 2;
//	pUDBDataSource->GetConnectionInfo().m_strServer = cmpathName+cfileName;
//	UGString UDBFileName=cmpathName+cfileName+_U(".udb");//   Modify by lina 2014-3-4 字符转换
//	UGString strModelName = _U("ModelName");
//	UGString strLongitudeColumn = _U("Longitude");
//	UGString strLatitudeColumn= _U("Latitude");
//	UGString strAltitudeColumn = _U("Altitude");
//	UGString strModelPath = _U("ModelPath");
//	if (NULL == pUDBDataSource)
//	{
//		return ;
//	}
//	UGC::UGDatasetVector* pTempVector=NULL;
//	UGC::UGRecordset* pRecordset=NULL;
//	if(!UGC::UGFile::IsExist(UDBFileName)){
//		if(!((UGC::UGDataSource*)pUDBDataSource)->Create())
//		{
//			delete pUDBDataSource;
//			return ;
//		}
//		UGC::UGDatasetVectorInfo tempDatasetVectorInfo;
//		UGString tempDatasetName= cfileName;	
//		tempDatasetVectorInfo.m_strName = tempDatasetName;
//		tempDatasetVectorInfo.m_nType	= UGC::UGDataset::Tabular;
//		/*tempDatasetVectorInfo.m_nType	= UGDataset::CAD;*/
//		pTempVector =  pUDBDataSource->CreateDatasetVector(tempDatasetVectorInfo);
//		if(pTempVector == NULL)
//		{
//			delete pUDBDataSource;
//			return ;
//		}
//
//		pTempVector->CreateField(strModelName,UGC::UGFieldInfo::Text ,30);
//
//		pTempVector->CreateField(strLongitudeColumn,UGC::UGFieldInfo::Double ,8);
//
//		pTempVector->CreateField(strLatitudeColumn,UGC::UGFieldInfo::Double,8);
//
//		pTempVector->CreateField(strAltitudeColumn,UGC::UGFieldInfo::Double,8);
//
//		pTempVector->CreateField(strModelPath,UGC::UGFieldInfo::Text ,MAX_PATH);
//		UGC::UGQueryDef queryDef;
//		queryDef.m_nOptions = UGC::UGQueryDef::Attribute;	
//		pRecordset = pTempVector->Query(queryDef);
//		if(pRecordset == NULL)
//		{
//			delete pUDBDataSource;
//			return ;
//		}
//		pRecordset->MoveFirst();
//
//	}
//	else
//	{
//		if(!((UGC::UGDataSource*)pUDBDataSource)->Open())
//		{
//			delete pUDBDataSource;
//			return ;
//		}
//		//打开已有数据集
//		UGC::UGDataset* p =pUDBDataSource->GetDataset(cfileName);
//		if(!p)
//		{
//			return;
//		}
//		pTempVector=(UGC::UGDatasetVector*)p;//是否可以强制转换?
//		UGC::UGQueryDef queryDef;
//		queryDef.m_nOptions = UGC::UGQueryDef::Attribute;	
//		pRecordset = pTempVector->Query(queryDef);
//		pRecordset->MoveLast();
//	}
//
//	UGC::UGint i =0;
//	UGC::UGGeometry* pGeometryNULL = NULL;
//
//	UGC::UGint count=m_arrmodelProp.GetSize();
//
//	UGdouble fFirstX,fFirstY,fFirstZ;
//	{
//		ModelProp modelProp = m_arrmodelProp.GetAt(0);
//		fFirstX = -modelProp.vPostion.x;
//		fFirstY = -modelProp.vPostion.y;
//		fFirstZ = -modelProp.vPostion.z;
//		UGC::UGPoint3D sphCoordinate(modelProp.vPostion.x,modelProp.vPostion.y,modelProp.vPostion.z); 
//		UGMaxToolkit::CartesianToSphericalD(longitude,latitude,altitude,sphCoordinate);
//		longitude = sphCoordinate.x;
//		latitude = sphCoordinate.y;
//		altitude = sphCoordinate.z;
//	}
//
//	for(i=0;i<count;i++)
//	{
//		ModelProp modelProp = m_arrmodelProp.GetAt(i);
//		pRecordset->AddNew(pGeometryNULL);
//		pRecordset->SetFieldValue(strModelName, UGC::UGFile::GetTitle(modelProp.strName));
//		UGC::UGPoint3D sphCoordinate(modelProp.vPostion.x+fFirstX,modelProp.vPostion.y+fFirstY,modelProp.vPostion.z+fFirstZ); 
//		CartesianToSphericalD(longitude,latitude,altitude,sphCoordinate);
//		pRecordset->SetFieldValue(strLongitudeColumn,sphCoordinate.x);
//		pRecordset->SetFieldValue(strLatitudeColumn,sphCoordinate.y);
//		pRecordset->SetFieldValue(strAltitudeColumn,sphCoordinate.z);	
//		pRecordset->SetFieldValue(strModelPath,modelProp.strPath);
//		pRecordset->Update();
//	}
//	pTempVector->ReleaseRecordset(pRecordset);
//	pRecordset = NULL;
//	pTempVector->SaveInfo();
//	pUDBDataSource->Close();
//	delete pUDBDataSource;
//}
void UGMaxToolkit::DeleteAllMyDirFiles(UGString strDeleteDir)
{
	UGString strFileName;
	UGArray<UGString> strFileArray;
	UGFile::ListFiles(strFileArray, strDeleteDir);
	UGint nFileCount = strFileArray.GetSize();
	for(UGint i = 0; i < nFileCount; i++)
	{
		DeleteAllMyDirFiles(strDeleteDir+_U("\\")+strFileArray[i]);//   Modify by lina 2014-3-4 字符转换
	}
	UGFile::RmDir(strDeleteDir);
	UGFile::Delete(strDeleteDir);
}

void UGMaxToolkit::CartesianToSphericalD(double dLongitude,double dLatitude,double dAltitude,UGPoint3D& p)
{
	double x=p.x,y=p.y;
	double dLatitudeObject=dLatitude+y*180/(MPI*RI);
	if(dLatitudeObject > 90.0)
	{
		dLatitudeObject = 180 - dLatitudeObject;
	}
	if(dLatitudeObject<-90.0)
	{
		dLatitudeObject=-180-dLatitudeObject;
	}
	double dLongitudeObject=dLongitude+180*x/(MPI*RI*cos(dLatitudeObject*MPI/180));
	if(dLongitudeObject>180)
	{
		dLongitudeObject=-360+dLongitudeObject;
	}
	if(dLongitudeObject<-180)
	{
		dLongitudeObject=360+dLongitudeObject;
	}
	//这里不应该更改高度，造成高度重复，前边已经在UGMaxModelProcess中设置高度了 wangxinhe 20120607
	//double dAltitudeObject=/*dAltitude+*/p.z;
	p.x=dLongitudeObject;
	p.y=dLatitudeObject;
	//p.z=dAltitudeObject;
}


UGbool UGMaxToolkit::IsCorrectSphericalD(char* pStrLonti,char* pLati)
{
	UGfloat fLonti = atof(pStrLonti);
	UGfloat fLati = atof(pLati);
	if(fLonti<=180.0&&fLonti>=-180.0&&fLati>=-90.0&&fLati<=90.0)
		return TRUE;
	else
		return FALSE;

}

void UGMaxToolkit::CopyTexture(const UGString& strSrcTexPath,const UGString& strDesTexPath)
{
	UGString strExt = UGFile::GetExt(strSrcTexPath);	
	UGint nFileType = UGFileType::ExtNameToFileType(strExt);
	UGuint nTextureScaling = UGMaxLogFile::GetMaxLogFile()->GetTextureScaling();
	UGuint nMaxTextureSize = UGMaxLogFile::GetMaxLogFile()->GetMaxTextureSize();

	if((nFileType == UGFileType::GIF ||
		(nTextureScaling == 0 && nMaxTextureSize == 8192)))
	{
		UGFile::Copy(strSrcTexPath,strDesTexPath,true);
		return;
	}

	UGImportParams* pParamImport = UGExchangeParamsManager::MakeImportParams(nFileType);
	if(pParamImport == NULL)
	{
		return;
	}	
	pParamImport->SetFilePathName(strSrcTexPath);

	UGArray<UGImageData*> aryImageData;
	if(UGFileParseToolkit::GetImageData(pParamImport, aryImageData,TRUE) && 
		aryImageData.GetSize() > 0)
	{
		UGint nWidthPowerOfTwo = aryImageData[0]->nWidth;
		UGint nHeightPowerOfTwo = aryImageData[0]->nHeight;

		nWidthPowerOfTwo >>= nTextureScaling;
		nHeightPowerOfTwo >>= nTextureScaling;
		// 模型纹理最大支持2048
		if (nWidthPowerOfTwo >= nMaxTextureSize)
		{
			nWidthPowerOfTwo = nMaxTextureSize;
		}
		if(nHeightPowerOfTwo >= nMaxTextureSize)
		{
			nHeightPowerOfTwo = nMaxTextureSize;
		}

		UGImageData tempImageData;
		tempImageData.btBitsPixel = aryImageData[0]->btBitsPixel;

		UGint nWidthPower2 = UGMathEngine::NextP2(nWidthPowerOfTwo);
		UGint nHeightPower2 = UGMathEngine::NextP2(nHeightPowerOfTwo);
		UGMathEngine::ScaleImage(*aryImageData[0],tempImageData,nWidthPower2,nHeightPower2);

		UGString strNewTexPath = strDesTexPath;
		UGExportParams* pParamExport = NULL;
		switch (nFileType)
		{
		case UGFileType::GTiff:
			{
				UGString strMapName = UGFile::GetName(strSrcTexPath);
				UGMaxLogFile::GetMaxLogFile()->GetMapName().SetAt(strMapName,aryImageData[0]->btBitsPixel);
				if (aryImageData[0]->btBitsPixel == IPF_RGBA)
				{
					pParamExport = UGExchangeParamsManager::MakeExportParams(UGFileType::PNG);
					strNewTexPath = UGFile::ChangeExt(strDesTexPath,_U(".png"));//   Modify by lina 2014-3-4 字符转换
				}
				else
				{
					pParamExport = UGExchangeParamsManager::MakeExportParams(UGFileType::JPG);
					strNewTexPath = UGFile::ChangeExt(strNewTexPath,_U(".jpg"));
				}
			}
			break;
		default:
			{
				pParamExport = UGExchangeParamsManager::MakeExportParams(nFileType);
			}
			break;
		}
		pParamExport->SetFilePathName(strNewTexPath);
		UGFileParseToolkit::SaveImageData(pParamExport, tempImageData,true);//设置纹理不反向

		delete pParamExport;
		pParamExport = NULL;

		delete[] tempImageData.pBits;
		tempImageData.pBits = NULL;
	}		

	for(UGint j = 0;j < aryImageData.GetSize();j++)
	{
		if (NULL!= aryImageData[j])
		{
			if (NULL!=aryImageData[j]->pBits)
			{
				delete[] aryImageData[j]->pBits;
			}
			delete aryImageData[j];
		}
		aryImageData.RemoveAll();
	}

	delete pParamImport;
	pParamImport = NULL;	
}
