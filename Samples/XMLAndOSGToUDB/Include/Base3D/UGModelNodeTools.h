#pragma once

#include "Base3D/UGMesh.h"
#include "Base3D/UGModelNode.h"

namespace UGC
{
class UGModelPagedLOD;
class UGModelNode;

class BASE3D_API UGModelNodeTools
{
public:
	//! \brief 构造函数
	UGModelNodeTools();

	~UGModelNodeTools();

public:
	//! \brief 从老的模型结构构造实体(不带Animation)
	//! \brief 仅负责构造实体
	//! \brief arryMeshes[in]
	//! \brief arrMaterials[in]
	//! \brief pModelNode[in/out]
	//! \brief nGeoID[in] 传入的ID，为了给骨架命名用
	static UGbool MakeModelNodeFromModelDataInfo(UGArray<UGMesh*>& arryMeshes, 
		UGArray<UGMaterial*>& arrMaterials, UGModelNode* pModelNode, const UGint nGeoID=0);

	//! \brief 根据m_pntScale m_pntRotate m_pntPos构造矩阵；
	//! \brief 该 矩阵+ModelNode=UGGeoModelPro
	static UGMatrix4d GetLocalMatrix(const UGPoint3D& pntPos, const UGPoint3D& pntScale, \
		const UGPoint3D& pntRotate, const UGbool bSpherePlaced);

	//! \brief 比较两个骨架是否一样
	//! \brief 判断材质名、顶点、纹理、法线等内容
	static UGbool IsSame(UGModelSkeleton* pSkeleton1, UGModelSkeleton* pSkeleton2);

	//! \brief 给ModelNode设置LOD层
	//! \brief arrModelNodes：0->N 由精细到粗糙
	//! \brief 取arrModelNodes[i]的最精细层数据作为pModelModel的LOD
	static UGbool SetModelLODs(UGModelNode* pModelNode, \
		UGArray<UGModelNode*> arrModelNodes, UGArray<UGdouble> arrLODScales);

	//! \brief 给ModelNode 设置LOD 的壳儿
	static void BuildLODShell(UGModelNode* pModelNode, const UGint nLODCount, UGModelEM* pEM=NULL);
	//! \brief Patch未加载的情况下，从EM获取范围
	static UGBoundingSphere GetPatchBounds(UGModelEM* pEM, UGModelPagedPatch* pPatch);

	//! \brief 骨架移除重复点，生成带属性的索引包（组件层调用）
	static UGint RemoveDuplicateVertex2(UGModelNode* pModelNodeSrc, UGModelNode* pModelNodeDes);

	//! \brief 骨架移除重复点，生成带属性的索引包（Revit插件调用）
	static UGint RemoveDuplicateVertex2(UGModelSkeleton* pSkeleton);

	//! \brief 球面坐标转局部坐标
	static void VertexSphere2Local(UGVertexDataPackageExact* pVertexPack, UGPoint3D &pntCenter);

	//! \brief vec球面坐标转以pntCenter为中心的局部坐标
	static UGVector3d VertexSphere2Local(const UGVector3d& vec, const UGPoint3D& pntCenter);

	//! \brief vec球面坐标转以pntCenter为中心的局部坐标
	static UGVector3d VertexSphere2Local(const UGVector3d& vec, const UGMatrix4d& matDest);

	//! \brief 清空Patch的FileName
	static void ClearPatchFileName(UGModelNode* pModelNode);

	//! \brief 计算ModelNode按mat放置后的BBox，逐个点计算后比较
	static UGBoundingBox ComputeExtremZ(UGModelNode* pModelNode, UGMatrix4d& mat, UGbool b2Shpere);
	static UGBoundingBox ComputeExtremZ(UGModelPagedPatch* pPatch, UGMatrix4d& mat, UGbool b2Shpere);
	static UGBoundingBox ComputeExtremZ(UGModelGeode* pGeode, UGMatrix4d& mat, UGbool b2Shpere);

private:
	//! \brief Mesh转Skeleton
	static UGModelSkeleton* MakeSkeletonFrom(UGMesh* pMesh, const UGString strSkeletonTag);

	//! \brief 转Material3D
	static void MakeMaterialFrom(UGMaterial* pMaterial, PolygonFrontFace pff, UGint nDiffuseColorCount, \
		UGModelMaterial*& pMaterial3D, std::vector<UGModelTexture*>& vecTexture);

	//! \brief 比较两个顶点包内容是否一致
	template<typename VertexPackage, typename TVertex>
	static UGbool IsSame(VertexPackage* pVertexPack1, \
		VertexPackage* pVertexPack2, UGdouble dbTolerance)
	{
		if(pVertexPack1->m_nVertexDimension != pVertexPack2->m_nVertexDimension ||
			pVertexPack1->m_nVerticesCount != pVertexPack2->m_nVerticesCount ||
			pVertexPack1->m_nNormalCount != pVertexPack2->m_nNormalCount ||
			pVertexPack1->m_nNormalDimension != pVertexPack2->m_nNormalDimension ||
			pVertexPack1->m_nVertexColorCount != pVertexPack2->m_nVertexColorCount ||
			pVertexPack1->m_nSecondVertexColorCount != pVertexPack2->m_nSecondVertexColorCount)
		{
			return FALSE;
		}
		for(UGint i = 0; i < SMSCN_MAX_TEXTURE_COORD_SETS; i++)
		{
			if(pVertexPack1->m_TexCoordCount[i] != pVertexPack2->m_TexCoordCount[i])
			{
				return FALSE;
			}
		}

		//顶点
		if(! UGModelNodeTools::CompareValue<TVertex>(pVertexPack1->m_pVertices, \
			pVertexPack2->m_pVertices, pVertexPack1->m_nVerticesCount, \
			pVertexPack1->m_nVertexDimension, dbTolerance))
		{
			return FALSE;
		}

		//法线
		if(! UGModelNodeTools::CompareValue<UGfloat>(pVertexPack1->m_pNormals, \
			pVertexPack2->m_pNormals, pVertexPack1->m_nNormalCount, \
			pVertexPack1->m_nNormalDimension, dbTolerance))
		{
			return FALSE;
		}	

		//顶点颜色
		if(! UGModelNodeTools::CompareValue<UGuint>(pVertexPack1->m_pVertexColor, \
			pVertexPack2->m_pVertexColor, pVertexPack1->m_nVertexColorCount))
		{
			return FALSE;
		}
		if(! UGModelNodeTools::CompareValue<UGuint>(pVertexPack1->m_pSecondVertexColor, \
			pVertexPack2->m_pSecondVertexColor, pVertexPack1->m_nSecondVertexColorCount))
		{
			return FALSE;
		}

		//纹理坐标
		for(UGint i = 0; i < SMSCN_MAX_TEXTURE_COORD_SETS; i++)
		{
			UGfloat* pTexCoords1 = pVertexPack1->m_pTexCoords[i];
			UGfloat* pTexCoords2 = pVertexPack2->m_pTexCoords[i];
			if(pTexCoords1 == NULL)
			{
				continue;
			}

			UGint nDim = pVertexPack1->m_nTexDimensions[i];
			if(! UGModelNodeTools::CompareValue<UGfloat>(pTexCoords1, pTexCoords2, \
				pVertexPack1->m_TexCoordCount[i], nDim, dbTolerance))
			{
				return FALSE;
			}
		}
		return TRUE;
	}

	//! \brief 比较两个索引
	static UGbool IsSame(UGIndexPackage* pIndexPack1, UGIndexPackage* pIndexPack2);

	template<typename TType>
	static UGbool CompareValue(TType* pValue1, TType* pValue2, \
		UGint nCount, UGint nDim=1, UGdouble dbTolerance=0.0)
	{
		if(typeid(TType) == typeid(float) ||
			typeid(TType) == typeid(double))
		{
			for(UGint i=0; i<nCount; i++)
			{
				for(UGint j=0; j<nDim; j++)
				{
					if(!UGEQ(pValue1[nDim*i+j], pValue2[nDim*i+j], dbTolerance))
					{
						return FALSE;
					}
				}
			}
		}
		else
		{
			for(UGint i=0; i<nCount; i++)
			{
				for(UGint j=0; j<nDim; j++)
				{
					if(pValue1[nDim*i+j] != pValue2[nDim*i+j])
					{
						return FALSE;
					}
				}
			}
		}
		return TRUE;
	}
};

}
