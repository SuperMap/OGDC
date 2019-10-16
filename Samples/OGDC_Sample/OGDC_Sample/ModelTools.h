#pragma once
#include <string>
#include "Base3D/UGModelNodeEntities.h"
#include "FileParser/UGFileParseToolkit.h"
#include "Base3D/UGModelNodeShells.h"
#include "Geometry3D/UGGeoModelPro.h"
#include "Toolkit/UGImgToolkit.h"
using namespace std;
using namespace UGC;

struct SkeletonInfo
{
	//顶点数据
	double *pVertices;
	//顶点个数(pVertices数组size的1/3)
	int nVerticesCount;
	//顶点索引数据
	int *pVerticesIndex;
	//索引个数(pVerticesIndex数组size)
	int nIndexCount;
	//法线数据
	float *pNormals;
	//法线个数(pNormals数组size的1/3)
	int nNormalCount;
	//纹理坐标
	float *pUVs;
	//纹理坐标个数(pUVS数组size的1/2)
	int nUVCount;
	//骨架名称
	string skeletonName;
};
class ModelTools
{
public:
	//创建骨架
	static UGModelSkeleton* buildSkeleton(SkeletonInfo info);
	//创建材质
	static UGModelMaterial* buildMaterial(UGColorValue3D ugc_color,vector<string>& vecTexturePaths);
	//创建纹理
	static UGModelTexture* buildTexture(string texturePath);
	//创建Geode实例
	static UGModelGeode* buildGeodeInstance(UGModelGeode*& pGeode, UGModelSkeleton*& pSkeleton, UGModelMaterial*& pMaterial, vector<UGModelTexture*>& vecTextures);
	//通过Geode实例和矩阵创建与实例共用骨架，材质，纹理的Geode,GeodeName可有可无
	static UGModelGeode* buildGeodeRef(UGModelGeode*& pGeodeInstance, UGMatrix4d matrix, UGString m_GeodeName = L"");
	//将Geode连同其LOD层一同实例化处理
	static vector<UGModelGeode*> buildGeodeRefWithLOD(vector<UGModelGeode*>& vecGeodeInstance, UGMatrix4d matrix, UGString m_geodeName = L"");
	//构建模型
	static UGGeoModelPro* buildModelPro(vector<UGModelGeode*>vecModelLod,UGPoint3D position,bool isLonLat);
};