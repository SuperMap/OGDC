// OGDC_Sample.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "OGDCTools.h"
using namespace UGC;
using namespace std;
int main()
{
	//骨架信息
	SkeletonInfo info;
	//写入顶点信息
	double verts[24] =
	{
		0.1234, 0.1234, 0.1234,   //第0个顶点
		20.1234, 0.1234, 0.1234,  //第1个顶点
		20.1234, 0.1234, 20.1234, //第2个顶点
		0.1234, 0.1234, 20.1234,  //第3个顶点
		0.1234, 20.1234, 0.1234,  //第4个顶点
		20.1234, 20.1234, 0.1234, //第5个顶点
		20.1234, 20.1234, 20.1234,//第6个顶点
		0.1234, 20.1234, 20.1234  //第7个顶点
	};
	info.pVertices = new double[24];
	for (int i = 0;i < 24;i++)
	{
		info.pVertices[i] = verts[i];
	}
	info.nVerticesCount = 8;
	//写入顶点索引信息
	int VertsIndex[36] =
	{
		0, 1, 2,//前方
		0, 2, 3,
		4, 7, 6,//后方
		4, 6, 5,
		4, 0, 3,//左侧
		4, 3, 7,
		1, 5, 6,//右侧
		1, 6, 2,
		3, 2, 6,//顶部
		3, 6, 7,
		0, 4, 1,//底部
		1, 4, 5
	};
	info.pVerticesIndex = new int[36];
	for (int i = 0;i<36;i++)
	{
		info.pVerticesIndex[i] = VertsIndex[i];
	}
	info.nIndexCount = 36;
	//写入法线信息
	float normals[108] =
	{
		0, -1, 0, 0, -1, 0, 0, -1, 0,
		0, -1, 0, 0, -1, 0, 0, -1, 0,//前方
		0, 1, 0, 0, 1, 0, 0, 1, 0,
		0, 1, 0, 0, 1, 0, 0, 1, 0,//后方
		-1, 0, 0, -1, 0, 0, -1, 0, 0,
		-1, 0, 0, -1, 0, 0, -1, 0, 0,//左侧
		1, 0, 0, 1, 0, 0, 1, 0, 0,
		1, 0, 0, 1, 0, 0, 1, 0, 0,//右侧
		0, 0, 1, 0, 0, 1, 0, 0, 1,
		0, 0, 1, 0, 0, 1, 0, 0, 1,//顶部
		0, 0, -1, 0, 0, -1, 0, 0, -1,
		0, 0, -1, 0, 0, -1, 0, 0, -1//底部
	};
	info.pNormals = new float[108];
	for (int i = 0; i < 108; i++)
	{
		info.pNormals[i] = normals[i];
	}
	info.nNormalCount = 36;
	//写入UV信息
	float uvs[72] =
	{
		0, 0, 2, 0, 2, 2,
		0, 0, 2, 2, 0, 2,
		0, 0, 1, 0, 1, 1,
		0, 0, 1, 1, 0, 1,
		0, 0, 1, 0, 1, 1,
		0, 0, 1, 1, 0, 1,
		0, 0, 1, 0, 1, 1,
		0, 0, 1, 1, 0, 1,
		0, 0, 1, 0, 1, 1,
		0, 0, 1, 1, 0, 1,
		0, 0, 1, 0, 1, 1,
		0, 0, 1, 1, 0, 1
	};
	info.pUVs = new float[72];
	for (int i = 0; i < 72; i++)
	{
		info.pUVs[i] = uvs[i];
	}
	info.nUVCount = 36;
	info.skeletonName = "SkeletonName";
	UGModelSkeleton* pSkeleton = ModelTools::buildSkeleton(info);
	//材质信息
	UGColorValue3D color;
	color.SetValue(1, 1, 1, 1);
	vector<string> vecTexturePaths;
	vecTexturePaths.push_back("./modelTexture.jpg");
	UGModelMaterial* pMaterial = ModelTools::buildMaterial(color, vecTexturePaths);
	//纹理信息
	vector<UGModelTexture*>vecTexture;
	for (int i = 0; i < vecTexturePaths.size(); i++)
	{
		UGModelTexture* pTexture = ModelTools::buildTexture(vecTexturePaths[i]);
		vecTexture.push_back(pTexture);
	}
	//创建Geode实例化信息,也可给他添加矩阵并添加进记录集成为一个模型。
	vector<UGModelGeode*>vecGeode;
	UGModelGeode* pGeode = new UGModelGeode();
	//多个骨架可以循环使用此方法添加
	pGeode = ModelTools::buildGeodeInstance(pGeode, pSkeleton, pMaterial, vecTexture);
	//添加精细层
	vecGeode.push_back(pGeode);
	UGModelGeode* pGeodeLod = new UGModelGeode();
	pGeodeLod = ModelTools::buildGeodeInstance(pGeodeLod, pSkeleton, pMaterial, vecTexture);
	//添加LOD层
	vecGeode.push_back(pGeodeLod);

	//通过实例化信息实例化所有与之共用骨架的Geode（包括LOD层）
	//所有实例化模型共用实例化信息，唯独矩阵和名字可能不同
	UGMatrix4d matrix( 5,0,0,0,
		0,5,0,0,
		0,0,5,0,
		25,59,29,1 );
	vector<UGModelGeode*>vecGeodeRef = ModelTools::buildGeodeRefWithLOD(vecGeode, matrix, L"");


	/////////////////////////////////model信息/////////////////////////////////////
	//模型插入点
	UGPoint3D point;
	point.x = point.y = point.z = 0;
	//是否导出为球面
	bool isLonLat = true;
	UGGeoModelPro* pGeometryInstance = ModelTools::buildModelPro(vecGeode, point, isLonLat);
	UGGeoModelPro* pGeometry = ModelTools::buildModelPro(vecGeodeRef, point, isLonLat);



	OgdcString sever = L"./llSource.udb";
	//创建并打开数据源
	OgdcDataSource* pDs = OGDCTools::CreateAndOpenDataSource(sever);
	//创建数据集
	OgdcString strName=pDs->GetUnoccupiedDatasetName(L"OGDCSample");
	OgdcDatasetVector* datasetVector = OGDCTools::CreateOrOpenDataSetVector(pDs,strName,isLonLat);
	//创建记录集
	OgdcRecordset* recordset = OGDCTools::CreateRecordset(datasetVector);
	//记录集更新
	OGDCTools::UpdateRecordset(recordset, pGeometryInstance);
	OGDCTools::UpdateRecordset(recordset, pGeometry);
	delete  pGeometryInstance;
	//关闭记录集
	recordset->Close();
	//关闭数据源
	pDs->Close();
}
