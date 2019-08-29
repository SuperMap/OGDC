
// UGBIMExporter.h: plugin class for 3DMAX.
////////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file headerfile.h
//!  \brief Class 导出成BIM的插件类。
//!  \details 从3DMAX中提取数据，生成公司支持的三维模型，此类为插件类，DoExport方法是最重要的函数，在进行导出时使用的MAX所调用的接口。
//!  \author sunye。
//!  \attention 3DMax导出插件中mapChannel中默认的1必须为Diff的通道，这是因为3DMAX中SDK介绍的关于多重纹理和使用显示的多重纹理存在一些矛盾
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#include "max.h"
#include "stdmat.h"
#include "Stream/UGFileStream.h"
#include <map>
#include <list>
#include "Base3D/UGNode.h"
#include "UGMaxModelProcess.h "
#include "UGMtlToNode.h"
#include "Base3D/UGRenderOperationGroup.h"
#include "Base3D/UGModelNodeShells.h"
#include "UGMaxProcessBar.h"
#include "OGDC/OgdcDataSource.h"
#include "OGDC/OgdcProviderManager.h"
namespace UGC
{
	using namespace std;
	struct MeshParamInfos
	{
		UGString strID;
		UGint mtlID;
		std::vector<UGuint> indexs;
		std::vector<UGfloat> verts;
		std::vector<UGfloat> colors;
		std::vector<UGfloat> texCoords;
		UGbool bTwoSided;
	};
	class UGBIMExporter : public SceneExport 
	{
		//friend INT_PTR CALLBACK BIMExporterProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	public:
		UGBIMExporter();
		~UGBIMExporter();	
		int				ExtCount();					// Number of extensions supported                     // TRUE if generating extra verts for mismatched UV coords
		const TCHAR *	Ext(int n);					// Extension #n (i.e. "3DS")
		const TCHAR *	LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
		const TCHAR *	ShortDesc();				// Short ASCII description (i.e. "3D Studio")
		const TCHAR *	AuthorName();				// ASCII Author name
		const TCHAR *	CopyrightMessage();			// ASCII Copyright message
		const TCHAR *	OtherMessage1();			// Other message #1
		const TCHAR *	OtherMessage2();			// Other message #2
		unsigned int	Version();					// Version number * 100 (i.e. v3.01 = 301)
		void			ShowAbout(HWND hWnd);		// Show DLL's "About..." box
		void			BeforeExport(Interface *i);
		void			WriteNodes(UGNode& rootNode ,MtlToNode& mtlToNode);
		void			WriteAnimation(std::list<INode*>& nodeInScene,UGFileStream& fStream,std::map<INode*,UGString>& nodeToString,Interface *ip);
		int				DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options);	// Export file
		BOOL			SupportsOptions(int ext, DWORD options);
		//以上方法都是SceneExport集成而来的函数

		//------------------------------------------------------------------------------------
		//将Max中材质转换成UGMaterial，并写入流
		void			WriteMtl(Mtl *mtl,UGFileStream& fStream);
		//！ \生成一个UGMESH
		//！ \nodeTex 处理max中的一个节点
		//！ \mtlID UGMesh使用的材质ID
		//！ \mtlToNode 以材质组织的数据结构node列表
		//！ \返回一个UGMESH列表
		MeshParamInfos WriteMesh(NodeMultiTex*  nodeTex,UGint mtlID,MtlToNode& mtlToNode,Mtl* std = NULL);
		//！ \内部优化使用，用来处理顶点共享是否，是否要生存一个顶点放入顶点数组
		//！ \返回一个索引值，如果返回-1则说明要插入该顶点，否则就直接放入索引值即可
		UGint			NeedAddVertAttribute(UGint faceIndex,UGint vertIndex,Mesh& mesh,std::map<UGint,UGint>&oldToNewPos,std::map<UGint,UGint>&oldToNewTex,std::map<UGint,UGint>&oldToNewMultiTex,int nSecondID,UGint size);
		//！ \放入新的顶点所需要处理的操作
		void			WriteIndex(std::vector<UGuint>&indexs,std::vector<UGfloat>&verts,std::vector<UGfloat>&colors,std::vector<UGfloat>&texCoords,UGint faceIndex,UGint vertIndex,Mesh &mesh,INode* pNode,std::list<int>& mapIDList,Mtl* std = NULL);
		//！ \导出动画信息
		//！ \pMapNode UGC类库的节点，需要进行构建的树的根
		//！ \pNode max中的节点，是源数据，需要进行拷贝转换的节点
		//！ \nodeToString 节点到名称的映射，动画信息需要mesh对应于名称，绑定在哪个节点上
		void            BuildNodeTree(UGNode* pMapNode,INode* pNode,std::map<INode*,UGString>& nodeToString);
	
		//! \brief 导出S3MB数据
		int ExportS3MB(const TCHAR *name, ExpInterface *ei, Interface *i);

		//! \brief 导出S3MB数据
		int ExportUDB(const TCHAR *name, ExpInterface *ei, Interface *i);

#pragma region 数据层导出模型数据
		UGbool UGBIMExporter::ReadModelData(ExpInterface *ei, Interface *inf, UGC::UGModelGeode* pGeode);
		UGModelSkeleton* UGBIMExporter::BuildSkeleton(MeshParamInfos& meshInfo,vector<UGint> &isDDS);
		void ProcessRefernceNode(UGModelGeode* pGeode);
#pragma endregion

		UGModelMaterial* BuildMaterial(Mtl *mtl, UGbool bIsTwoSide,std::vector<UGint> &isDDS);

		//! \brief 获取纹理数据
		UGTextureData* GetTextureData(UGString& strTexPath);

		//! \brief 设置模型LOD信息
		void SetLodInfo(ModelLOD& lodInfo);

		//! \brief 设置引用Node
		void SetRefernceNode(std::vector<INode *>& vecNode);

		UGbool SaveS3MB(UGString strFilePath);

		//! \brief 导出到模型数据集
		UGbool ExportModelDataset(ExportParameter para,std::map<UGC::UGString, int> m_mapId);

		UGbool ExportCADDataSet(ExportParameter para);

		//! \brief 创建或获取模型数据集
		OGDC::OgdcDatasetVector* GetModelDatasetVector(OGDC::OgdcDataSource* pDatasource,ExportParameter para);

		UGString MaterialHashFunc(UGPass* pPass);

		//! \brief 设置是否导出材质颜色
		void SetIsExportMaterialColor(UGbool bIsExport);

		//! \brief 设置是否导出法线
		void SetIsExportNormal(UGbool bIsExport);

		//! \brief 设置进度条
		void SetMaxProgressBar(UGMaxProcessBar* pMaxProcessBar);

	private:
		//! \两个字段名称
		UGString m_strModelName;
		UGString m_strParentName;
		UGString m_strId;

		//！ \是否导出动画
		BOOL m_bExportAnimation;

		//！ \当前导出的网格材质ID
		UGint m_nCurrentMeshID;

		std::map<INode*,UGString> m_NodeIndexMap;
		UGint timeStart;

		//！ \源纹理路径
		UGString m_strTexPath;

		//！ \目标纹理路径
		UGString m_strDestTexPath;

		//！ \包围盒值
		UGVector3d vecMax,vecMin;

		BOOL m_bShowPrompts;

		//材质ID号对应的通道ID，通道ID对应于在UGMesh中对应的纹理坐标存储顺序
		std::map<int,std::list<int>> m_mtlIDToChannels;

		//正在导出的材质ID号
		int m_mtlIDNow;

		//标志是否按照原样Index输出wangxinhe 为了使导出面数据集正确 20120607
		UGbool m_bIsExportVertexNormal;

		//! \brief 纹理列表
		std::map<int,std::vector<UGfloat>> m_multiTexList;

		//! \brief LOD信息
		ModelLOD m_LodInfo;

		//! \brief 是否导出材质颜色
		UGbool m_bIsExportMaterialColor;

		//! \brief 是否导出法线
		UGbool m_bIsExportNormal;

		std::vector<INode*> m_vecNode;

		//! \brief 数据层指针
		UGModelGeode* m_Geode;

		//! \brief 存储需要导出的模型
		std::vector<UGModelGeode*> m_ModelGroup;

		//! \brief 进度条
		UGMaxProcessBar* m_pMaxProcessBar;
	};
}

