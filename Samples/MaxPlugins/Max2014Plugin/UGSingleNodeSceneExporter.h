// UGSingleNodeSceneExporter.h: plugin class for 3DMAX.
////////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file headerfile.h
//!  \brief Class 单个对象的场景类。
//!  \details 此类只包含一个节点的场景，为了进行单个模型的导出而进行封装的场景。
//!  \author sunye。
//!  \attention 
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

#ifndef UGSingleModelExport_H
#define UGSingleModelExport_H

#include "Max.h"
#include "UGMaxModelProcess.h"
#include "stdmat.h"
#ifdef MAX9_2010
#include "bmmlib.h"
#endif
#include "bitmap.h"
#include "UGMaxProcessBar.h"
#include "UGMaxToolkit.h"
#include "UGMaxLogFile.h"
#include <ilayer.h> 

#define BILLBOARDLAYER _U("BillBoard")
#define REGION3DLAYER _U("Region3D")


//! \一个节点的场景类
class SingleNodeScene:public IScene{
private: INode *node;
public:
#ifdef MAX2016
	int  EnumTree (ITreeEnumProc *proc,bool bIncludeXRef = false){
		if(proc->callback(node)==TREE_ABORT) return 1;
		else return 0;
	}
#else
	int  EnumTree (ITreeEnumProc *proc){
		if(proc->callback(node)==TREE_ABORT) return 1;
		else return 0;
	}
#endif
	void  FlagFGSelected (TimeValue t){}
	void  FlagFGAnimated (TimeValue t){}
	void  FlagFGDependent (TimeValue t, BaseObject *obj){}
	void SetNode(INode *onode){
		node=onode;
	}
};

//! \一个节点的导出接口类

class SingleNodeExpInterface: public ExpInterface{
public:
	SingleNodeExpInterface()
	{
		theScene = NULL;
	}

	void InitScene()
	{
		if (!theScene)
		{
			theScene= new SingleNodeScene();
		}
	}

	void ResetIScene(INode *onode)
	{
		((SingleNodeScene* )theScene)->SetNode(onode);
	}

	~SingleNodeExpInterface()
	{
		if (theScene != NULL)
		{
			delete theScene;
			theScene = NULL;
		}
	}
};
//! \brief 获取场景中有多少个节点，为了进度条信息
class CGetNodeNum:public ITreeEnumProc
{
public:
	CGetNodeNum()
	{
		m_nNumNode = 0;
	}
	int callback( INode *node )
	{
		m_nNumNode++;
		INode* pNode = node;
		if(pNode==NULL)return TREE_CONTINUE;
		Object *originObj = pNode->EvalWorldState(0).obj;
		if(!originObj->CanConvertToType(triObjectClassID))return TREE_CONTINUE;
		if(pNode->GetName()==NULL)return TREE_CONTINUE;
		TriObject *obj=(TriObject *)originObj->ConvertToType(0, triObjectClassID);//将object转换为TriObject指针

		Mesh mesh=obj->GetMesh();
		if(mesh.numVerts<=0)
		{
			if(originObj!=obj)
				obj->DeleteMe();
			return TREE_CONTINUE;
		}
		mesh.buildBoundingBox();
		Box3 box3 =  mesh.getBoundingBox(&pNode->GetObjectTM(0));
		m_box3 += box3;
		return TREE_CONTINUE;
	}
	int GetNumNode()
	{
		return m_nNumNode;
	}
	Box3 GetBoundingBox()
	{
		return m_box3;
	}
private:
	int m_nNumNode;
	Box3 m_box3;
};
class UGSingleNodeSceneExporter:public ITreeEnumProc
{
public:
	UGSingleNodeSceneExporter() { }
	//! \brief 单个导出类的构造函数
	//! \param pInModelProcess 指定何种批量导出的导出工作类
	//! \param inStrDest 文件生成的目标地址
	//! \param inTexSrc 纹理所在的路径
	//! \param bCopyTex 是否要拷贝纹理
	UGSingleNodeSceneExporter(UGMaxModelProcess *pInModelProcess,UGC::UGString strTexDest,UGC::UGString strTexSrc, UGC::UGbool bExportAll, UGC::UGbool bCopyTex=TRUE, UGC::UGbool bIsBIM = FALSE);

	void InitParams(UGMaxModelProcess *pInModelProcess,UGC::UGString strTexDest,UGC::UGString strTexSrc,UGC::UGbool bExportAll, UGC::UGbool bCopyTex=TRUE, UGC::UGbool bIsBIM = FALSE);
	//!\ 3DMAX进行遍历节点时所执行的操作，必须实现
	virtual int	callback( INode *pNode );

	//!\ 设置进度条
	void SetProcessBar(UGMaxProcessBar * pMaxProcessBar){m_pMaxProcessBar = pMaxProcessBar;}

	//!\ 设置导出何种模型文件的导出类
	void SetModelProcess(UGMaxModelProcess *pInModelProcess){m_pModelProcess=pInModelProcess;}

	//!\ 设置单个模型导出所在的根目录
	void SetDirPath(UGC::UGString inFilePath){m_strTexDest=inFilePath;}

	//!\ 设置整个场景移动的一个位置
	void SetModelRelative(double dx, double dy, double dz){ m_pModelProcess->SetXYZ(dx,dy,dz);}

	//!\ 获取处理后的模型信息
	UGArray<ModelProp> GetModelsInfo(){return m_pModelProcess->GetModelsInfo();}

	std::map<UGC::UGString,UGC::UGString> GetModel2SGM(){return m_pModelProcess->GetModel2SGM();}

	//!\ 执行导出操作
	void StartExport();
	//!\ BIM导出OSGB数据
	void ExportOSGB();
	//!\ BIM导出DAE数据
	void ExportDAE();
	//!\ BIM导出S3MB数据
	void ExportS3MB();
	//!\ 导出到模型数据集
	void ExportModelDataset(ExportParameter para);

	UGMaxModelProcess *m_pModelProcess;
private:
	void ProcessTexture(Mtl* mtl,UGC::UGString strTexSrc,UGC::UGString strDest);
	
	//! \ 拷贝纹理的函数
	void ReallyCopyTexture(Mtl* mtl);

	//! \ 拷贝纹理
	void CopyTexture(INode *pNode );
	
	SingleNodeExpInterface myExpInterface;
	UGC::UGString m_strTexDest;
	UGC::UGString m_strTexSrc;
	UGMaxProcessBar * m_pMaxProcessBar;
	//! \ 是否拷贝纹理
	UGC::UGbool m_bCopyTexture;
	//!\ 标示是否是BIM数据
	UGC::UGbool m_bIsBIM;
	//!\ 标示是否全部导出
	UGC::UGbool m_bExportAll;
};
#endif
