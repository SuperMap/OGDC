#include "max.h"
#include"UGMaxModelProcess.h"
#include "UGMaxLogFile.h"
#include "decomp.h"
#include "Base3D/UGQuaternion4d.h"
#include "UGBIMExporter.h"


#define NUMFACEOUTOFMESH 30000
#include "meshdelta.h"
typedef ClassDesc* (*LibClassDescFormDLL)(int );

UGMaxModelProcess::UGMaxModelProcess(UGC::UGString strDllName)
{
	m_strExt=GetExt(strDllName);
	if(strDllName==_U("UDB")||strDllName==_U("udb")||
		strDllName==_U("S3MB") || strDllName==_U("s3mb"))
	{
		m_pExportPlugin=(SceneExport *)GetCOREInterface()->CreateInstance(SCENE_EXPORT_CLASS_ID,Class_ID(0x69e348c, 0x4f661e5b));
		m_pExportClsDes=NULL;
		m_pHead = NULL;
		m_bIsBIM = FALSE;
	}
	else
	{
		strDllName = _U("./stdplugs/") + strDllName + _U(".dle");
		HINSTANCE baseExportDll=LoadLibrary(strDllName.Cstr());

#ifdef _UGUNICODE
		OgdcMBString strMB = "LibClassDesc";
		FARPROC libClassDesFun=GetProcAddress(baseExportDll,strMB.Cstr());
#else
		FARPROC libClassDesFun=GetProcAddress(baseExportDll,"LibClassDesc");
#endif	
		LibClassDescFormDLL getClassDes=(LibClassDescFormDLL)libClassDesFun;
	    m_pExportClsDes=getClassDes(0);
		m_pExportPlugin=(SceneExport *)m_pExportClsDes->Create();
		m_bIsBIM = FALSE;
	}
}

UGC::UGString UGMaxModelProcess::GetExt(UGC::UGString strName)
{
	if(strName==_U("S3MB")||strName==_U("s3mb"))return _U(".S3MB");
	if(strName==_U("UDB")||strName==_U("udb"))return _U(".udb");
	return _U("");
}
INode* UGMaxModelProcess::ProcessNodeByStep()
{
	if(m_bEnd)return NULL;
	m_modelInfo.nSmID=0;
	m_modelInfo.nSmUserID = m_pNode->GetGBufID();
	m_modelInfo.vScale = UGC::UGVector3d(m_pntNodeScale.x, m_pntNodeScale.y, m_pntNodeScale.z);
	m_modelInfo.vRotate = UGC::UGVector3d(m_pntNodeRotate.x, m_pntNodeRotate.y, m_pntNodeRotate.z);
	m_modelInfo.vPostion = UGC::UGVector3d(m_pntNodeCoodinate.x, m_pntNodeCoodinate.y, m_pntNodeCoodinate.z);
	
	Object *originObj = m_pNode->EvalWorldState(0).obj;
	TriObject *obj=(TriObject *)originObj->ConvertToType(0, triObjectClassID);
	ObjectEntry *pObjTemp = NULL;
	pObjTemp = Contains(obj);
	if((obj->GetMesh()).getNumFaces() < 65530 || m_strExt == _U(".SGM"))
	{
		m_bEnd=true;
		UGC::UGString strName(m_pNode->GetName());
		if (!m_bIsBIM)
		{
			m_strCurrentNodeName=m_translateFile.GetName(strName)+m_strExt;
			m_modelInfo.strName=m_strCurrentNodeName;
		}
		return m_pNode;
	}
	MeshDelta detach;
	Mesh &mesh= obj->GetMesh();
	TriObject* pNewObject= new TriObject;
	int numFaces=mesh.getNumFaces();
	BitArray detachFace(numFaces);
	
	int newObjectNum=numFaces/NUMFACEOUTOFMESH+1;
	int endNum;
	
	if(m_nEndFlagInt<newObjectNum-1)endNum=(m_nEndFlagInt+1)*NUMFACEOUTOFMESH;
	else 
	{
		endNum=numFaces;
		m_bEnd=true;
	}
	for(int i=m_nEndFlagInt*NUMFACEOUTOFMESH;i<endNum;i++)
		detachFace.Set(i);
	detach.Detach(mesh,&(pNewObject->mesh),detachFace,TRUE,FALSE,FALSE);

	INode *pTempNode = GetCOREInterface()->CreateObjectNode(pNewObject);
	Matrix3 m=pTempNode->GetNodeTM(0);
	Point3 ptemp((double)0.0,(double)0.0,(double)m_dZ+m_pntNodeCoodinate.z);
	m.Translate(ptemp);
	pTempNode->SetNodeTM(0,m);
	pTempNode->SetMtl(m_pNode->GetMtl());

	UGC::UGString str;
	str.Format(_U("%d"),m_nEndFlagInt);
	UGC::UGString strName(m_pNode->GetName());
	UGC::UGString currentNodeNameTemp=_U("Object_")+str+_U("In")+strName+m_strExt;
	m_strCurrentNodeName=m_translateFile.GetName(currentNodeNameTemp);
	m_modelInfo.strName=m_strCurrentNodeName;
	m_nEndFlagInt++;
	m_tempSmallModels.push_back(pTempNode);
	if(originObj!=obj)
		obj->DeleteMe();

	delete pNewObject;
	pNewObject = NULL;

	return pTempNode;
}


UGC::UGString UGMaxModelProcess::GetCurrentNodeName()
{
	return m_strCurrentNodeName;
}
bool UGMaxModelProcess::ProcessNode(INode *pInNode)
{
	m_pNode=pInNode;
	Object *originObj = pInNode->EvalWorldState(0).obj;
	
	if(!originObj->CanConvertToType(triObjectClassID))return false;
	TriObject *obj=(TriObject *)originObj->ConvertToType(0, triObjectClassID);
	Mesh mesh=obj->GetMesh();
	if(mesh.numVerts<=0)
	{
		if(mesh.numVerts<=0)
		{
			if(originObj!=obj)
				obj->DeleteMe();
			return false;
		}
	}
	if(originObj!=obj)
		obj->DeleteMe();

	if(m_pNode->GetName()==NULL)return false;
	m_pNodeName=m_pNode->GetName();
	m_tempSmallModels.clear();

	m_bEnd=false;
	m_nEndFlagInt=0;
	m_matrix=m_pNode->GetNodeTM(0);
	Matrix3 transM = m_matrix;
	Box3 boundingBox;
	originObj->GetDeformBBox(0,boundingBox,&(m_pNode->GetObjectTM(0)));
	m_pntNodeCoodinate.x=boundingBox.Center().x;
	m_pntNodeCoodinate.y=boundingBox.Center().y;
	m_pntNodeCoodinate.z = 0;
	Point3 ptemp(m_dX-m_pntNodeCoodinate.x,m_dY-m_pntNodeCoodinate.y,m_dZ);
	transM.Translate(ptemp);
	m_pNode->SetNodeTM(0,transM);	
	m_pntNodeRotate = Point3(0, 0, 0);
	m_pntNodeScale = Point3(1, 1, 1);
	
	return true;
}


int  UGMaxModelProcess::ExportModel(const TCHAR *name,ExpInterface *ei)
{
	if (m_bIsBIM)
	{
		std::map<UGC::UGString, ModelLOD>::iterator lodModel = m_mapNodeToLOD.find(m_strCurrentNodeName);
		if (lodModel != m_mapNodeToLOD.end())
		{
			((UGC::UGBIMExporter*)m_pExportPlugin)->SetLodInfo(lodModel->second);
		}
		std::map<UGC::UGString, std::vector<INode *>>::iterator refernceNode = m_mapRefernceNode.find(m_strCurrentNodeName);
		if (refernceNode != m_mapRefernceNode.end())
		{
			((UGC::UGBIMExporter*)m_pExportPlugin)->SetRefernceNode(refernceNode->second);
		}
	}
	else
	{
		UGC::UGString strTmpPath(name);
		m_modelInfo.strPath=strTmpPath;
		m_arryModels.Add(m_modelInfo);
	}
	return m_pExportPlugin->DoExport(name,ei,GetCOREInterface(),TRUE);	
}

void UGMaxModelProcess::ProcessNodeName(TCHAR *name)
{
	UGC::UGString strName(m_pNodeName);
	UGC::UGString newName=m_translateFile.GetName(strName)+m_strExt;
	TCHAR *pName=(TCHAR*)newName.Cstr();
	while(*pName){
		*name=*pName;
		name++;
		pName++;
	}
	*name=*pName;
}

void UGMaxModelProcess::RecoveryNode()
{
	m_pNode->SetNodeTM(0,m_matrix);
	if(m_tempSmallModels.empty())return;
	std::list<INode*>::iterator first=m_tempSmallModels.begin(),last=m_tempSmallModels.end();
	while(first!=last)
	{
		((INode*)(*first))->Delete(0,FALSE);
		first++;
	}
}


UGArray<ModelProp> UGMaxModelProcess::GetModelsInfo()
{
	return m_arryModels;
}

void UGMaxModelProcess::Clear()
{
	m_arryModels.RemoveAll();
	m_translateFile.Clear();
	m_mapModelToSGM.clear();
	m_mapInstanceNode.clear();
	m_mapRefernceNode.clear();
	m_mapNodeToLOD.clear();
	m_mapParentName.clear();
}

UGMaxModelProcess::~UGMaxModelProcess()
{
	m_pNode = NULL;

	if(m_pExportPlugin != NULL)
	{
		delete m_pExportPlugin;
		m_pExportPlugin = NULL;
	}
		
	if(m_pExportClsDes != NULL)
	{
		delete m_pExportClsDes;
		m_pExportClsDes = NULL;
	}

	if (m_pHead != NULL)
	{
		delete m_pHead;
		m_pHead = NULL;
	}

	Clear();
}

ObjectEntry *UGMaxModelProcess::Contains(Object *obj) {
	ObjectEntry *e;
	for (e=m_pHead; e!=NULL; e = e->next) {
		Object *originObj = e->node->EvalWorldState(0).obj;
		if(originObj == obj)
			return e;
	}
	return NULL;
}

void UGMaxModelProcess::SetIsBIM(UGC::UGbool bIsBIM)
{
	m_bIsBIM = bIsBIM;
}

std::map<UGC::UGString,UGC::UGString> UGMaxModelProcess::GetModel2SGM()
{
	return m_mapModelToSGM;
}

UGC::UGbool UGMaxModelProcess::AddInstanceObject(INode *pInNode)
{
	UGC::UGbool bFlag = FALSE;
	m_pNode=pInNode;
	Object *originObj = pInNode->EvalWorldState(0).obj;
	if(!originObj->CanConvertToType(triObjectClassID))
		return FALSE;

	TriObject *obj=(TriObject *)originObj->ConvertToType(0, triObjectClassID);
	Mesh mesh=obj->GetMesh();
	if(mesh.numVerts<=0)
	{
		if(mesh.numVerts<=0)
		{
			if(originObj!=obj)
				obj->DeleteMe();
			return FALSE;
		}
	}
	if(originObj!=obj)
		obj->DeleteMe();

	//判断是否和其他顶点公用obj
	ObjectEntry *pObjTemp = NULL;
	pObjTemp = Contains(originObj);

	//分解节点矩阵为平移、旋转、缩放变量
	AffineParts ap;
	Matrix3 transM=pInNode->GetNodeTM(0);
	decomp_affine(transM, &ap);
	m_pntNodeCoodinate = ap.t;
	float angle[3];
	QuatToEuler(ap.q, angle);
	m_pntNodeRotate = Point3(angle[0] * RTOD, angle[1] * RTOD, angle[2] * RTOD);
	m_pntNodeScale = Point3(ap.k.x, ap.k.y, ap.k.z);
	m_pNodeName=m_pNode->GetName();
	m_strCurrentNodeName = m_translateFile.GetName(m_pNodeName)+m_strExt;
	m_modelInfo.strName = m_strCurrentNodeName;
	m_modelInfo.nSmID=0;
	m_modelInfo.nSmUserID = m_pNode->GetGBufID();
	m_modelInfo.vScale = UGC::UGVector3d(m_pntNodeScale.x, m_pntNodeScale.y, m_pntNodeScale.z);
	m_modelInfo.vRotate = UGC::UGVector3d(m_pntNodeRotate.x, m_pntNodeRotate.y, m_pntNodeRotate.z);
	m_modelInfo.vPostion = UGC::UGVector3d(m_pntNodeCoodinate.x, m_pntNodeCoodinate.y, m_pntNodeCoodinate.z);
	INode* pParentNode = pInNode->GetParentNode();
	if (pParentNode != NULL)
	{
		m_mapParentName[pInNode->GetName()] = pParentNode->GetName();
		if (pInNode->GetParentNode()->UserPropExists(_M("id")))//某项目特殊需求
		{
		   pInNode->GetParentNode()->GetUserPropInt(_M("id"),m_mapId[pInNode->GetName()]);
		}
	}
	else
	{
		m_mapParentName[pInNode->GetName()] = _U("");
	}
	if (pObjTemp != NULL)
	{
		m_mapModelToSGM[m_modelInfo.strName] = m_strTexDest + _U("OSGB/") + pObjTemp->node->GetName() + m_strExt;
		m_arryModels.Add(m_modelInfo);
		m_mapRefernceNode[pObjTemp->node->GetName() + m_strExt].push_back(pInNode);
		bFlag = FALSE;
	}
	else
	{		
		m_mapInstanceNode[m_modelInfo.strName] = pInNode;
		UGArray<LODInfo> arrlod;
		LODInfo lod;
		lod.m_nLevel = 0;
		lod.m_dDistance = 1;
		lod.node = pInNode;
		arrlod.Add(lod);
		m_mapNodeToLOD[m_modelInfo.strName].m_arrModelLODInfo = arrlod;
		m_mapModelToSGM[m_modelInfo.strName] = m_strTexDest + _U("OSGB/") + m_strCurrentNodeName;
		m_arryModels.Add(m_modelInfo);
		UGC::UGString ugName = UGC::UGFile::GetTitle(m_strCurrentNodeName);
#ifdef _UGUNICODE
		m_pNode->SetName(ugName);
#else
		m_pNode->SetName((char*)ugName.Cstr());
#endif
		m_mapRefernceNode[m_modelInfo.strName].push_back(pInNode);
		bFlag = TRUE;

		//将节点插入到队列中
		ObjectEntry *pEntry = new ObjectEntry(m_pNode);
		if (m_pHead)
		{
			m_pHead->next = pEntry;
		}
		m_pHead = pEntry;
	}

	return bFlag;
}

void UGMaxModelProcess::SetModelInfo(UGC::UGString strName)
{
	m_strCurrentNodeName = strName;
}


void UGMaxModelProcess::SaveS3MB(UGC::UGString FilePath)
{
	((UGC::UGBIMExporter*)m_pExportPlugin)->SaveS3MB(FilePath);
}
//!\ 导出到模型数据集
void UGMaxModelProcess::ExportModelDataset(ExportParameter para)
{
	((UGC::UGBIMExporter*)m_pExportPlugin)->ExportModelDataset(para,m_mapId);
}

////!\ 导出到CAD数据集
//void UGMaxModelProcess::ExportModelDataset(ExportParameter para)
//{
//	((UGC::UGBIMExporter*)m_pExportPlugin)->ExportCADDataSet(para);
//}

void UGMaxModelProcess::SetIsExportMaterialColor(UGC::UGbool bIsExport)
{
	if(m_pExportPlugin != NULL)
	{
		((UGC::UGBIMExporter*)m_pExportPlugin)->SetIsExportMaterialColor(bIsExport);
	}
}
void UGMaxModelProcess::SetIsExportNormal(UGC::UGbool bIsExport)
{
	if(m_pExportPlugin != NULL)
	{
		((UGC::UGBIMExporter*)m_pExportPlugin)->SetIsExportNormal(bIsExport);
	}
}