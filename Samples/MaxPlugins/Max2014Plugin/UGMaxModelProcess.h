// ModelProcess.h: plugin class for 3DMAX.
////////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file headerfile.h
//!  \brief Class 此类提供详细的模型处理，此类是个状态类，内部状态会保持。
//!  \details 包括如何处理模型，导出模型，处理模型名称和清除已经处理过的模型信息。
//!  \author sunye。
//!  \attention 
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

#ifndef _UGMaxModelProcess_H_
#define _UGMaxModelProcess_H_

#include "Stream/ugdefs.h"
#include "Stream/UGFile.h"
#include "UGMaxToolkit.h"
using namespace UGC;

class INode;
class SceneExport;
class Object;
class TriObject;
class ExpInterface;

class ObjectEntry {
public:
	INode *node;
	ObjectEntry *next;
	ObjectEntry(INode *e) 
	{ 
		node = e; 
		next = NULL;
	}

	~ObjectEntry()
	{
		node = NULL;

		if (next != NULL)
		{
			delete next;
			next = NULL;
		}
	}
};

struct UserField 
{
	//! \brief 自定义标签名
	UGString strName;
	//! \brief 值
	UGString strValue;
};
struct  ModelProp
{
	UGint nSmID;
	UGint nSmUserID;
	UGString strName;
	UGString strPath;
	UGVector3d vPostion;
	UGPoint3D innerPoint3D;
	UGVector3d vScale;
	UGVector3d vRotate;
	UGVector3d vMin;
	UGVector3d vMax;
	UGRect2D rcBounds;
	//与m_arrUserField对应,不过arrUserField.strValue代表具体的值
	UGArray<UserField> arrUserField;

};

struct LODInfo
{
	UGC::UGint m_nLevel;
	UGC::UGdouble m_dDistance;
	UGC::UGdouble m_dRecude;
	INode *node;
};

struct ModelLOD
{
	//! \brief 模型符号动态LOD层级信息
	UGArray<LODInfo> m_arrModelLODInfo;

	ModelLOD()
	{
		
	}

	~ModelLOD()
	{
		m_arrModelLODInfo.RemoveAll();
	}
};

class CtranslateFileName{
private:
	std::map<UGC::UGString,UGC::UGint> fileNameMap;
public:	
	void Add(UGC::UGString name){
		++fileNameMap[name];
	}
	UGC::UGString GetName(UGC::UGString name){
		TCHAR* pName=(TCHAR*)name.Cstr();
		UGC::UGint nameNumC=name.GetLength();
		TCHAR* newNameSta=new TCHAR[nameNumC+1];
		TCHAR* newName=newNameSta;
		memset(newName,0,nameNumC+1);
		while(*pName)
		{
			TCHAR charElement=*pName;
			if(charElement!='\\'&&charElement!='/'&&charElement!=':'&&charElement!='*'&&charElement!='\?'&&charElement!='\''&&charElement!='<'&&charElement!='>'&&charElement!='|')
			{
				*newName=*pName;
				newName++;
				pName++;
			}

			else	
				pName++;

		}
		*newName=0;
		UGC::UGString SnewName(newNameSta);
		if(SnewName.IsEmpty())
		{
			SnewName = _U("noname");//   Modify by lina 2014-3-4 字符转换
		}
		UGC::UGString strRealName = SnewName;
		SnewName.MakeUpper();
		delete[] newNameSta;
		++fileNameMap[SnewName];
		if(fileNameMap[SnewName]==1)
		{
			return strRealName;
		}
		UGC::UGString str;
		str.Format(_U("%d"),fileNameMap[SnewName]-1);
		UGC::UGString tempName=strRealName+_U("_")+str;
		return tempName;
	}
	std::map<UGC::UGString,UGC::UGint> SameNameList(){
		std::map<UGC::UGString,UGC::UGint>::const_iterator map_it=fileNameMap.begin();
		std::map<UGC::UGString,UGC::UGint> nameDict;
		while(map_it!=fileNameMap.end()){
			if(map_it->second>=2)
				nameDict[map_it->first]=map_it->second;
			map_it++;
		}
		return nameDict;
	}
	bool IsEmpty(){
		return fileNameMap.empty();
	} 
	void Clear(){
		fileNameMap.clear();
	}
};

class UGMaxModelProcess{
public:
//!  \利用所使用的dllName来加载。
	UGMaxModelProcess(UGC::UGString strDllName=_U("3dsexp"));

//!  \利用所使用的dllName来加载。
	~UGMaxModelProcess();

//!  \对模型进行一些加工，比如归原点等。
	bool ProcessNode(INode *pInNode);

//!  \还原ProcessNode方法所做的操作，此类操作针对上一次ProcessNode操作
	void RecoveryNode();

//!  \将场景EI中包含的节点以name的名称保存到硬盘上
	int  ExportModel(const TCHAR *name,ExpInterface *ei);

//!  \根据类的内部已经处理过的操作来对name进行命名
	void ProcessNodeName(TCHAR *name);

//!  \返回处理后的模型信息

	UGArray<ModelProp> GetModelsInfo();

//!  \复位处理类的所有信息
	void Clear();

//!  \获取当前处理节点的名称
	UGC::UGString GetCurrentNodeName();

//!  \获取当前处理的节点
	INode* ProcessNodeByStep();

//!  \复位处理类的所有信息
	void SetXYZ(double dInx,double dIny,double dInz){m_dX=dInx;m_dY=dIny;m_dZ=dInz;}

	ObjectEntry *Contains(Object *obj);

	void SetIsBIM(UGC::UGbool bIsBIM);

	std::map<UGC::UGString,UGC::UGString> GetModel2SGM();

	void SetTexPath(UGC::UGString strPath){m_strTexDest = strPath;}

	//是否需要增加实例对象
	UGC::UGbool AddInstanceObject(INode *pInNode);

	void SetModelInfo(UGC::UGString strName);
	//保存S3MB
	void SaveS3MB(UGC::UGString filePath);

	//!\ 导出到模型数据集
	void ExportModelDataset(ExportParameter para);


	UGC::UGString GetOSGBPath();

	//实例化节点
	std::map<UGC::UGString, INode *> m_mapInstanceNode;
	//实例化节点对应的引用列表
	std::map<UGC::UGString, std::vector<INode *>> m_mapRefernceNode;

	std::map<UGC::UGString, ModelLOD> m_mapNodeToLOD;

	//父节点名称数组
	std::map<UGC::UGString, UGC::UGString> m_mapParentName;

	//用户自定义id需求
	std::map<UGC::UGString, int> m_mapId;

	//设置是否导出材质颜色
	void SetIsExportMaterialColor(UGC::UGbool bIsExport);

	//设置是否导出法线
	void SetIsExportNormal(UGC::UGbool bIsExport);

private:

//! \ 通过传递的名称来获取后缀名
	UGC::UGString GetExt(UGC::UGString strName);

//! \ 处理过的模型信息
	UGArray<ModelProp> m_arryModels;

//! \  要使用的导出插件类
	SceneExport* m_pExportPlugin;

//！\   模型本身的转换矩阵
	Matrix3 m_matrix;

//！\   模型名称加工器
	CtranslateFileName m_translateFile;

//！\   正在处理的NODE的名称
#ifdef _UGUNICODE
	const TCHAR *m_pNodeName;
#else
	TCHAR *m_pNodeName;
#endif
	

//！\   正在处理的NODE的名称
	INode *m_pNode;

//！\  导出时的设置参数，设置每个模型经过居中（z轴不居中，以原来的值加上z值）后，然后统一移动的相对值。
	double m_dX;
	double m_dY;
	double m_dZ;

//！\   标识对节点的操作是否完毕。
	bool m_bEnd;
	
//！\   当前节点的名称。
	UGC::UGString m_strCurrentNodeName;

//！\   保留处理流程信息。
	int m_nEndFlagInt;

//！\   正在处理的NODE的世界坐标
	Point3 m_pntNodeCoodinate;

	//！\   模型的旋转量
	Point3 m_pntNodeRotate;

	//！\   模型的所放量
	Point3 m_pntNodeScale;

//！\   文件后缀名
	UGC::UGString m_strExt;

//！\   由于模型大而生成的临时小模型
	std::list <INode*> m_tempSmallModels;

//！\   当前模型信息
	ModelProp m_modelInfo;

//！\   导出描述类
	ClassDesc* m_pExportClsDes;

	UGC::UGbool m_bIsBIM;

	ObjectEntry * m_pHead;

	std::map<UGC::UGString,UGC::UGString> m_mapModelToSGM;

	UGC::UGString m_strTexDest;
};

#endif
