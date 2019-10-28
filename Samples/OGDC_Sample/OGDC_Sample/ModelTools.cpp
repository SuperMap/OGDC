#include "ModelTools.h"

UGC::UGModelSkeleton* ModelTools::buildSkeleton(SkeletonInfo info)
{
	UGModelSkeleton* pSkeleton = new UGModelSkeleton();
	//顶点
	UGVertexDataPackageExact* m_pVertexPackExact = new UGVertexDataPackageExact();
	m_pVertexPackExact->SetVertexNum(info.nVerticesCount);
	for (int i = 0; i < info.nVerticesCount * 3; i++)
	{
		m_pVertexPackExact->m_pVertices[i] = info.pVertices[i];
	}
	//顶点索引
	UGIndexPackage* m_pIndexPack = new UGIndexPackage();
	//如果info.nVerticesCount == info.nUVCount 则将下面的 IT_16BIT_2 与 IT_32BIT_2 分别改为IT_16BIT 和 IT_32BIT
	//然后将第二句改为 m_pIndexPack->SetIndexNum(info.nIndexCount);
	//并去掉if和else中的第二个for循环
	m_pIndexPack->m_enIndexType = info.nVerticesCount < 65536 ? IT_16BIT_2 : IT_32BIT_2;
	m_pIndexPack->SetIndexNum(info.nIndexCount * 2);
	if (m_pIndexPack->m_enIndexType == IT_16BIT_2)
	{
		for (int i = 0; i < info.nIndexCount; i++)
		{
			m_pIndexPack->m_pIndexes[i] = (UGushort)info.pVerticesIndex[i];
		}
		for (int i =0;i < info.nIndexCount ; i++)
		{
			m_pIndexPack->m_pIndexes[i+info.nIndexCount] = i;
		}
	}
	else
	{
		UGuint* pUintIndex = (UGuint*)m_pIndexPack->m_pIndexes;
		for (int i = 0; i < info.nIndexCount; i++)
		{
			pUintIndex[i] = info.pVerticesIndex[i];
		}
		for (int i =0;i < info.nIndexCount ; i++)
		{
			m_pIndexPack->m_pIndexes[i+info.nIndexCount] = i;
		}
	}
	//法线
	if (m_pVertexPackExact != NULL && info.pNormals != NULL)
	{
		if (m_pVertexPackExact->m_pNormals != NULL)
		{
			delete[] m_pVertexPackExact->m_pNormals;
		}
		m_pVertexPackExact->m_pNormals = info.pNormals;
		m_pVertexPackExact->m_nNormalCount = info.nNormalCount;
	}
	//UV
	//默认UV为二维向量
	int nDim = 2;
	if (m_pVertexPackExact != NULL && info.pUVs != NULL)
	{
		m_pVertexPackExact->m_nTexDimensions[0] = nDim;
		m_pVertexPackExact->m_TexCoordCount[0] = info.nUVCount;
		if (m_pVertexPackExact->m_pTexCoords[0] != NULL)
		{
			delete[] m_pVertexPackExact->m_pTexCoords[0];
		}
		m_pVertexPackExact->m_pTexCoords[0] = info.pUVs;
	}
	//设置骨架名称
	pSkeleton->m_strName.FromStd(info.skeletonName,UGString::Default);
	pSkeleton->SetExactDataPackRef(m_pVertexPackExact, m_pIndexPack);
	pSkeleton->ComputerBoundingBox();
	return pSkeleton;
}

UGC::UGModelMaterial* ModelTools::buildMaterial(UGColorValue3D ugc_color, vector<string>& vecTexturePaths)
{
	UGModelMaterial* pMaterial = new UGModelMaterial();
	UGString materialName;
	UGString Temp;
	vector<UGString> UGtexturePaths;
	for (int i = 0; i < vecTexturePaths.size(); i++)
	{
		Temp.FromStd(vecTexturePaths[i],UGString::Default);
		UGtexturePaths.push_back(Temp);
		materialName += Temp;
	}
	Temp.FromStd(std::to_string(ugc_color.GetAsLongRGBA()),UGString::Default);
	materialName += Temp;
	//设置材质的材质名，必须唯一
	pMaterial->m_strName = materialName;
	pMaterial->MakeDefault();
	if (pMaterial->getNumTechniques() == 0)
	{
		pMaterial->createTechnique();
	}
	UGTechnique* pTech = pMaterial->getTechnique(0);
	if (pTech->getNumPasses() == 0)
	{
		UGPass* pPassCreate = pTech->createPass();
		//通道名与材质名需一致
		pPassCreate->m_strName = pMaterial->m_strName;
	}
	UGPass* pPass = pTech->getPass(0);
	//无纹理时，才将颜色带上
	if (vecTexturePaths.size() == 0)
	{
		//除m_Diffuse外，Pass中存在各种颜色属性
		pPass->m_Diffuse = ugc_color;
	}
	return pMaterial;
}

UGC::UGModelTexture* ModelTools::buildTexture(string texturePath)
{
	UGModelTexture* pTexture = new UGModelTexture();
	UGString strFileName;
	strFileName.FromStd(texturePath,UGString::Default);
	if (pTexture->m_strName.IsEmpty())
	{
		pTexture->m_strName = UGFile::GetTitle(strFileName);
	}
	UGFileParseToolkit::GetTextureData(strFileName, pTexture->m_pTextureData);
	return pTexture;
}

UGC::UGModelGeode* ModelTools::buildGeodeInstance(UGModelGeode*& pGeode, UGModelSkeleton*& pSkeleton, UGModelMaterial*& pMaterial, vector<UGModelTexture*>& vecTextures)
{
	//骨架与材质绑定
	pSkeleton->SetMaterialName(pMaterial->m_strName);
	vector<UGString> vecTextureNames;
	for (int i = 0; i < vecTextures.size(); i++)
	{
		vecTextureNames.push_back(vecTextures[i]->m_strName);
	}
	//材质与纹理绑定
	pMaterial->SetTextureName(vecTextureNames);
	pGeode->AddSkeleton((UGModelSkeletonPtr)pSkeleton);
	pGeode->AddMaterial((UGModelMaterialPtr)pMaterial);
	if (vecTextures.size() > 0)
	{
		for (int i = 0; i < vecTextures.size(); i++)
		{
			if (vecTextures[i] != NULL)
			{
				pGeode->AddTexture((UGModelTexturePtr)vecTextures[i]);
			}
		}
	}
	return pGeode;
}

UGC::UGModelGeode* ModelTools::buildGeodeRef(UGModelGeode*& pGeodeInstance, UGMatrix4d matrix, UGString m_GeodeName)
{
	UGModelGeode* pGeodeRef = new UGModelGeode();
	pGeodeRef->SetPosition(matrix);
	pGeodeRef->ComputeBoundingBox();
	//名字可有可无
	if (!m_GeodeName.IsEmpty())
	{
		pGeodeRef->m_strName = m_GeodeName;
	}
	for (UGint nSkeleton = 0; nSkeleton < pGeodeInstance->GetEntityNames(UGModelEntity::METype::etSkeleton).size(); nSkeleton++)
	{
		//指向同一引用（实例化核心操作）
		pGeodeRef->AddSkeleton(pGeodeInstance->GetSkeleton(pGeodeInstance->GetEntityNames(UGModelEntity::METype::etSkeleton)[nSkeleton]));
	}
	for (UGint nTexture = 0; nTexture < pGeodeInstance->GetEntityNames(UGModelEntity::METype::etTexture).size(); nTexture++)
	{
		//指向同一引用（实例化核心操作）
		pGeodeRef->AddTexture(pGeodeInstance->GetTexture(pGeodeInstance->GetEntityNames(UGModelEntity::METype::etTexture)[nTexture]));
	}
	for (UGint nMaterial = 0; nMaterial < pGeodeInstance->GetEntityNames(UGModelEntity::METype::etMaterial).size(); nMaterial++)
	{
		//指向同一引用（实例化核心操作）
		pGeodeRef->AddMaterial(pGeodeInstance->GetMaterial(pGeodeInstance->GetEntityNames(UGModelEntity::METype::etMaterial)[nMaterial]));
	}
	return pGeodeRef;
}

std::vector<UGModelGeode*> ModelTools::buildGeodeRefWithLOD(vector<UGModelGeode*>& vecGeodeInstances, UGMatrix4d matrix, UGString m_geodeName /*= NULL*/)
{
	vector<UGModelGeode*> vecGeodeRefs;
	for (int i = 0;i<vecGeodeInstances.size();i++)
	{
		vecGeodeRefs.push_back(buildGeodeRef(vecGeodeInstances[i], matrix, m_geodeName));
	}
	return vecGeodeRefs;
}

UGC::UGGeoModelPro* ModelTools::buildModelPro(vector<UGModelGeode*>vecModelLod, UGPoint3D position, bool isLonLat)
{
	if (vecModelLod.size() < 1)
	{
		return NULL;
	}
	UGGeoModelPro* pGeometry = new UGGeoModelPro();
	//获得模型结点
	UGModelNode* pModelNode = pGeometry->GetModelNode();
	//精细层
	UGModelPagedPatch* pPath = pModelNode->CreatePatch();
	pPath->AddGeode(vecModelLod[0]);

	//Lod层
	if (vecModelLod.size() > 1)
	{
		for (int i = 1; i < vecModelLod.size(); i++)
		{
			UGModelPagedLOD* pPagedLod = pModelNode->AddPagedLOD();
			UGdouble dDistance = UGMIN(1024, 5 * pPath->GetBoudingSphere().GetRadius()*pow(2.0, 4 - i - 1));
			UGModelPagedPatch* pPatchLod = pPagedLod->CreatePagedPatch(dDistance);
			pPatchLod->AddGeode(vecModelLod[i]);
		}
	}
	if (pModelNode->GetSkeletonCount(-1) > 0)
	{
		//设置模型是否导出到球面场景
		pGeometry->SetSpherePlaced(isLonLat);
		pGeometry->SetPosition(position);
	}
	return pGeometry;
}
