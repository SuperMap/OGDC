// SuActions.h : action classe for menu item in 3DMAX.
////////////////////////////////////////////////////////////////////////////
//                    _   _   ____   ____                    
//   Project         | | | | / ___) / ___)                   
//                   | | | || | __ | |                       
//                   | |_| || |_\ || \___                    
//                    \____/ \____| \ ___)     6.0           
//                                                           
//!  \file headerfile.h
//!  \brief Class 菜单项动作类。
//!  \details 在此添加各类菜单项对应的动作,修改菜单配置，需要设置三个地方：
// 1.SuAction.h中定义动作响应；
// 2.在AppendOperation添加动作操作；
// 3.在MenuGUP.h添加菜单项。
//!  \author sunye。
//!  \attention 
//!   Copyright (c) 1996-2010 SuperMap Software Co., Ltd. <br> 
//!   All Rights Reserved.
//!  \version 6.0
//////////////////////////////////////////////////////////////////////////
#ifndef SuActions_H
#define SuActions_H
#undef SelectFont
#undef SelectBrush
#undef SelectPen

#include <ShlObj.h>
#include"contextids.h" 
#include <Stream/ugdefs.h>

#include "MaxPluginres.h"
#include "UGSingleNodeSceneExporter.h"

#include "OGDC/OgdcDataSource.h"
#include "OGDC/OgdcProviderManager.h"
#define SuActionContextId 0x59fd2141
#define SuActionId 0x59bd2142
//using namespace UGC;
//!  \在此添加响应菜单项动作的类，需要继承ActionItemStandin，并实现以下几个方法，其中ExecuteAction为响应函数。主要功能负责
//!  \界面响应，以及界面信息的收集。

//!  \下面是批量导出S3MB的编写。
class BatchExport:public ActionItem
{
	friend INT_PTR CALLBACK BatchExportProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
public:
	//!  \在此类已经使用过的数字不能重复使用。例如，这个方法中ActionItemStandin(1)的数字1已被使用，不能在被使用。
	BatchExport(HINSTANCE outHInstance)
	{
		hInstance=outHInstance;
	}

	BOOL IsEnabled(){return TRUE;}
	BOOL IsItemVisible(){return TRUE;}
	virtual BOOL IsChecked(){return FALSE;}
	virtual void DeleteThis() {delete this;}
	virtual int GetId()	{return m_id;}
#ifndef MAX2014 
	virtual MaxIcon* GetIcon(){return NULL;}
#endif

	void Init(int id,const TCHAR *mText, const TCHAR *bText, const TCHAR *cText, const TCHAR *dText)
	{
		m_id = id;
		m_menuText = mText;
		m_buttonText.printf(_T("%s"),bText);	
		m_descText.printf(_T("%s"),dText);	
		m_catText.printf(_T("%s"),cText);
		m_strModelFileType = _U(".S3MB");
	}

	void GetMenuText(TSTR& menuText) {menuText = this->m_menuText;}
	virtual void GetButtonText(TSTR& buttonText)	{buttonText = this->m_buttonText;}
	void GetDescriptionText(TSTR& descText) { descText = this->m_descText;}
	void GetCategoryText(TSTR& catText) {catText = this->m_catText;}

	//! \动作响应函数
	BOOL ExecuteAction();
private:
	HINSTANCE hInstance;
	bool sgmOr3ds;
	UGC::UGString filePathName;
	UGC::UGString strTexPath;
	double longitude,latitude,altitude;

	UGC::UGString m_strModelFileType;

	int m_id;
	TSTR m_menuText, m_buttonText, m_descText, m_catText;
};

//!  \界面处理函数。
static INT_PTR CALLBACK BatchExportProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam){
	static BatchExport* be;
	switch(message) {
	case WM_INITDIALOG:
		{   
			SetWindowContextHelpId(hDlg, idh_3dsexp_export);
			be = (BatchExport*) lParam;
			CenterWindow(hDlg,GetParent(hDlg));
			UGMaxLogFile* pProjectionHolder = UGMaxLogFile::GetMaxLogFile();
			UGC::UGString pathNameCurrent(GetCOREInterface()->GetCurFilePath());
			UGC::UGString pathNameCurrentDir=UGC::UGFile::GetDir(pathNameCurrent);
			be->strTexPath=pathNameCurrentDir;
			SetDlgItemText(hDlg,IDC_TEXTURE,pathNameCurrentDir.Cstr());
			SetFocus(hDlg); 
			CheckDlgButton(hDlg,IDC_EXPORTS3MB,BST_CHECKED);
			return FALSE;
		}
	case WM_DESTROY:
		return FALSE;
	case WM_COMMAND:
		switch(LOWORD(wParam)) 
		{
		case IDOK:
			{
                if (IsDlgButtonChecked(hDlg,IDC_EXPORTS3MB))
				{
					be->m_strModelFileType = _U(".s3mb");
				}

				TCHAR dispathname[MAX_PATH];
				GetDlgItemText(hDlg,IDC_EXPORTPATH,dispathname,MAX_PATH);
				UGMaxToolkit::TransStandDir(dispathname);
				be->filePathName=dispathname;
				if(be->filePathName.IsEmpty()||!UGC::UGFile::IsExist(be->filePathName)){
					UGC::UGStringEx pathWarning;
					pathWarning.LoadResString(UGS_MAX_FILE_PATH_HINT);
					UGC::UGStringEx error;
					error.LoadResString(UGS_MAX_ERROR);
					MessageBox(hDlg,pathWarning,error,MB_OK|MB_ICONERROR);
					return FALSE;
				}

				TCHAR texpathname[MAX_PATH];
				GetDlgItemText(hDlg,IDC_TEXTURE,texpathname,MAX_PATH);
				UGMaxToolkit::TransStandDir(texpathname);
				be->strTexPath=texpathname;
				EndDialog(hDlg, 1);
				return TRUE;

			}
		case IDC_EXPOLOR:
			{
				UGC::UGStringEx warning;
				warning.LoadResString(UGS_MAX_FILE_PATH_HINT);
				be->filePathName=UGMaxToolkit::GetInputPath((TCHAR*)warning.Cstr(),IDC_EXPORTPATH,hDlg,true);
				return FALSE;
			}
		case IDC_TEXEXPOLOR:
			{
				TCHAR dispathname[MAX_PATH];
				GetDlgItemText(hDlg,IDC_TEXTURE,dispathname,MAX_PATH);

				UGC::UGStringEx warning;
				warning.LoadResString(UGS_MAX_TEXTURE_PATH_HINT);
				be->strTexPath=UGMaxToolkit::GetInputPath((TCHAR*)warning.Cstr(),IDC_TEXTURE,hDlg,true,dispathname);
				return FALSE;
			}
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL BatchExport::ExecuteAction()
{
	int result = DialogBoxParam(hInstance,
		MAKEINTRESOURCE(IDD_EXPORTSingle),
		GetCOREInterface()->GetMAXHWnd(),
		BatchExportProc,
		(LPARAM)this);
	if (result <= 0) 
		return FALSE;
	UGC::UGbool bBIM = TRUE;
	UGMaxModelProcess* pExportProcessor;
	m_strModelFileType=_U(".S3MB");
	if (m_strModelFileType.CompareNoCase(_U(".s3mb")) == 0)
	{
		pExportProcessor=new UGMaxModelProcess(_U("s3mb"));
		pExportProcessor->SetIsBIM(TRUE);
	}

	UGMaxProcessBar maxProcessBar(hInstance,GetCOREInterface()->GetMAXHWnd());
	maxProcessBar.SetWeight(1.0);
	maxProcessBar.Start();
	UGSingleNodeSceneExporter exporter(pExportProcessor,filePathName,strTexPath, TRUE,bBIM);
	exporter.SetModelRelative(0.0,0.0,altitude);
	exporter.SetProcessBar(&maxProcessBar);
	if (m_strModelFileType.CompareNoCase(_U(".s3mb")) == 0)
	{
		exporter.ExportS3MB();
		return TRUE;
	}
}
static INT_PTR CALLBACK ModelDatasetProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static UGC::UGString* pDataset;
	switch(message) {
	case WM_INITDIALOG:
		{   
			SetWindowContextHelpId(hDlg, idh_3dsexp_export);
			pDataset = (UGC::UGString*) lParam;
			CenterWindow(hDlg,GetParent(hDlg));
			return FALSE;
		}
	case WM_DESTROY:
		return FALSE;
	case WM_COMMAND:
		switch(LOWORD(wParam)) {
		case IDOK:
			{
				LPTSTR pDatasetLPName=new TCHAR[20];
				GetDlgItemText(hDlg,IDC_DATASETNAME,pDatasetLPName,20); 
				if(!UGMaxToolkit::IslegalName(pDatasetLPName))
				{
					MessageBox(hDlg,_U("数据集名称不合法"),_U("注意"),MB_OK|MB_ICONERROR);
					delete[] pDatasetLPName;
					return FALSE;
				}
				*pDataset = pDatasetLPName;
				delete[] pDatasetLPName;
				EndDialog(hDlg, 1);
				return TRUE;
			}

		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;
		}
	}
	return FALSE;
}
#ifdef MAX2014 
//!  \下面是导出BIM数据到数据集
class BIMExport:public ActionItem
{
	friend INT_PTR CALLBACK BIMExportProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
public:
	BIMExport(HINSTANCE outHInstance)
	{
		hInstance = outHInstance;
		m_bPlanar = FALSE;
		m_bIsIgnore = TRUE;
		m_bIsRemoveDupVertex = FALSE;
		m_bIsExportNormal = TRUE;
	}
	~BIMExport()
	{
	}

	BOOL IsEnabled(){return TRUE;}
	BOOL IsItemVisible(){return TRUE;}
	virtual BOOL IsChecked(){return FALSE;}
	virtual void DeleteThis() {delete this;}
	virtual int GetId()	{return m_id;}
#ifndef MAX2014 
	virtual MaxIcon* GetIcon(){return NULL;}
#endif

	void Init(int id,const TCHAR *mText, const TCHAR *bText, const TCHAR *cText, const TCHAR *dText)
	{
		m_id = id;
		m_menuText = mText;
		m_buttonText.printf(_T("%s"),bText);	
		m_descText.printf(_T("%s"),dText);	
		m_catText.printf(_T("%s"),cText);

	}

	void GetMenuText(TSTR& menuText) {menuText = this->m_menuText;}
	virtual void GetButtonText(TSTR& buttonText)	{buttonText = this->m_buttonText;}
	void GetDescriptionText(TSTR& descText) { descText = this->m_descText;}
	void GetCategoryText(TSTR& catText) {catText = this->m_catText;}
	UGArray<UGC::UGString> GetDataSets(OgdcString StrPathName,OGDC::OgdcInt type)
	{
		OGDC::OgdcDataSource* pUDBDataSource;
		if (!UGC::UGFile::GetExt(StrPathName).CompareNoCase(_U(".udb")))
		{
			pUDBDataSource = OgdcProviderManager::CreateOgdcDataSource(OGDC::oeFile);
		}
		pUDBDataSource->m_nEngineClass = 2;
		pUDBDataSource->m_connection.m_strServer = StrPathName;
		UGArray<UGC::UGString> datasetNames;
		if(!((OGDC::OgdcDataSource*)pUDBDataSource)->Open())
		{
			delete pUDBDataSource;
			return datasetNames;
		}
		for (OgdcInt i=0;i < pUDBDataSource->GetDatasetCount();i++)
		{
			OgdcDataset* pDataset=pUDBDataSource->GetDataset(i);
			if (pDataset->GetType()==type)
			{
				datasetNames.Add(pDataset->GetName());
			}
		}
		
		delete pUDBDataSource;
		return datasetNames;
	}
	//! \动作响应函数
	BOOL ExecuteAction()
	{
		//创建一个进度条
		UGMaxModelProcess bimProcess(_U("udb"));
		int result = DialogBoxParam(hInstance,
			MAKEINTRESOURCE(IDD_EXPORTBIM),
			GetCOREInterface()->GetMAXHWnd(),
			BIMExportProc,
			(LPARAM)this);

		if (result <= 0) 
		{
			return FALSE;
		}

		ExportParameter exportPara;
		//获取数据源的路径
		UGC::UGString strModelDir = UGC::UGFile::GetDir(m_strDataSourceName);
		UGC::UGFile::MkDir(strModelDir);
		bimProcess.SetIsBIM(TRUE);
		//设置纹理路径
		bimProcess.SetTexPath(strModelDir);
		UGC::UGbool bIsExportMaterialColor = !m_bIsIgnore;
		bimProcess.SetIsExportMaterialColor(bIsExportMaterialColor);
		bimProcess.SetIsExportNormal(m_bIsExportNormal);
		m_BIMExporter.InitParams(&bimProcess,strModelDir,m_strTexPath,TRUE,TRUE);
		m_BIMExporter.SetModelRelative(0.0,0.0,0); 
		UGMaxProcessBar maxProcessBar(hInstance,GetCOREInterface()->GetMAXHWnd());
		maxProcessBar.Start();
		m_BIMExporter.SetProcessBar(&maxProcessBar);

		exportPara.RemoveDupVertex=m_bIsRemoveDupVertex;
		exportPara.IsPlanar=m_bPlanar;
		exportPara.IgnoreMaterialColor=m_bIsIgnore;
		exportPara.WithoutNormal=!m_bIsExportNormal;
		exportPara.X=m_dPlaneX;
		exportPara.Y=m_dPlaneY;
		exportPara.Z=m_dPlaneZ;
		exportPara.DatasourceName= m_strDataSourceName;
		exportPara.DatasetName= m_strDataSetName;
		if (!m_bPlanar)
		{
			exportPara.X=m_dLon;
			exportPara.Y =m_dLat;
			exportPara.Z= m_dAlt;
		}

		m_BIMExporter.ExportModelDataset(exportPara);

		// //导出CAD
		//if(UGMaxToolkit::m_setBillboardList.size() > 0)
		//{
		//	UGC::UGString strCADModelDir = UGMaxLogFile::GetMaxLogFile()->GetTempDir();
		//	if (UGC::UGFile::IsExist(strCADModelDir))
		//	{
		//		UGC::UGFile::RmDirEx(strCADModelDir);
		//	}
		//	UGC::UGFile::MkDir(strCADModelDir);
		//	UGMaxModelProcess modelProcessor(_U("UDB"));

		//	UGC::UGbool bIsExport = !m_bIsIgnore;
		//	modelProcessor.SetIsExportMaterialColor(bIsExport);
		//	modelProcessor.SetIsExportNormal(m_bIsExportNormal);

		//	UGC::UGbool bCopyTex = TRUE;
		//	if (m_strTexPath.IsEmpty())
		//	{
		//		bCopyTex = FALSE;
		//	}
		//	UGSingleNodeSceneExporter exporter(&modelProcessor,strCADModelDir,m_strTexPath, bCopyTex);
		//	exporter.SetModelRelative(0.0,0.0,m_dAlt);
		//	UGMaxProcessBar maxProcessBar(hInstance,GetCOREInterface()->GetMAXHWnd());
		//	maxProcessBar.Start();
		//	maxProcessBar.SetWeight(0.1f);
		//	exporter.SetProcessBar(&maxProcessBar);
		//	exporter.StartExport();
		//	UGArray<ModelProp>models = exporter.GetModelsInfo();
		//	if(models.GetSize()==0)
		//	{
		//		UGC::UGStringEx warning;
		//		warning.LoadResString(UGS_MAX_SCENE_IS_NULL);
		//		UGC::UGStringEx hint;
		//		hint.LoadResString(UGS_MAX_HINT);
		//		MessageBox(GetCOREInterface()->GetMAXHWnd(),warning,hint,MB_OK|MB_ICONWARNING);
		//		return TRUE;
		//	}

		//	maxProcessBar.SetWeight(0.9f);
		//	//UGDataSetExporter dataSetExporter;
		//	//dataSetExporter.SetIsExportByBillBoard(TRUE);
		//	//dataSetExporter.SetProcessBar(&maxProcessBar);
		//	//dataSetExporter.SetPlanarCoordinateSystem(m_bPlanar);

		//	//if (m_bPlanePRJ)
		//	//{
		//	//	dataSetExporter.SetPrjName(m_strPRJXMLPath);
		//	//}

		//	if (m_bPlanar)
		//	{
		//		m_BIMExporter.ExportCADDataset(m_strDataSourceName,m_strDataSetName+_U("CAD"),m_dPlaneX,m_dPlaneY,m_dPlaneZ,models);
		//	}
		//	else
		//	{
		//		m_BIMExporter.ExportCADDataset(m_strDataSourceName,m_strDataSetName+_U("CAD"),m_dLon,m_dLat,m_dAlt,models);
		//	}
		//	UGC::UGFile::RmDirEx(strCADModelDir);*/
		//}


		return TRUE;
	}
private:
	HINSTANCE hInstance;
public:
	//UGDataSetExporter m_dataSetExporter;
	UGC::UGdouble m_dLon,m_dLat,m_dAlt;
	UGC::UGdouble m_dPlaneX,m_dPlaneY,m_dPlaneZ;

	UGC::UGString m_strTexPath;
	UGC::UGString m_strSymLibraryPath;

	UGC::UGString m_strDataSourceName;
	UGC::UGString m_strDataSetName;
	UGC::UGString m_strPRJXMLPath;
	UGC::UGbool m_bPlanePRJ;
	UGC::UGbool m_bPlanar;
	UGSingleNodeSceneExporter m_BIMExporter;
	int m_id;
	UGC::UGbool m_isLOD;
	// 是否去除材质颜色
	UGC::UGbool m_bIsIgnore;
	// 是否去除重复顶点
	UGC::UGbool m_bIsRemoveDupVertex;
	// 是否导出法线
	UGC::UGbool m_bIsExportNormal;
	TSTR m_menuText, m_buttonText, m_descText, m_catText;
};

static INT_PTR CALLBACK BIMExportProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static BIMExport* bimExport = NULL;
	switch(message) 
	{
	case WM_INITDIALOG:
		{   
			SetWindowContextHelpId(hDlg, idh_3dsexp_export);
			bimExport = (BIMExport* ) lParam;
			CenterWindow(hDlg,GetParent(hDlg));

			UGMaxLogFile* pProjectionHolder = UGMaxLogFile::GetMaxLogFile();
			SetDlgItemText(hDlg,IDC_LONGT,pProjectionHolder->GetLontitude().Cstr());
			SetDlgItemText(hDlg,IDC_LATIt,pProjectionHolder->GetLatitude().Cstr());
			SetDlgItemText(hDlg,IDC_ALTIT,pProjectionHolder->GetAltitude().Cstr());
			//球面坐标、平面坐标分开设置
			CheckDlgButton(hDlg,IDC_EARTHPOS,BST_CHECKED);
			SetDlgItemText(hDlg,IDC_PLANEX,pProjectionHolder->GetPlaneX().Cstr());
			SetDlgItemText(hDlg,IDC_PLANEY,pProjectionHolder->GetPlaneY().Cstr());
			SetDlgItemText(hDlg,IDC_PLANEZ,pProjectionHolder->GetPlaneZ().Cstr());
			HWND hwdTemp;
			hwdTemp=GetDlgItem(hDlg,IDC_PLANEX);
			EnableWindow(hwdTemp,FALSE);
			hwdTemp=GetDlgItem(hDlg,IDC_PLANEY);
			EnableWindow(hwdTemp,FALSE);
			hwdTemp=GetDlgItem(hDlg,IDC_PLANEZ);
			EnableWindow(hwdTemp,FALSE);

			UGC::UGString pathNameCurrent(GetCOREInterface()->GetCurFilePath());
			UGC::UGString pathNameCurrentDir=UGC::UGFile::GetDir(pathNameCurrent);
			bimExport->m_strTexPath=pathNameCurrentDir;
			SetDlgItemText(hDlg,IDC_TEXTUREPATH,pathNameCurrentDir.Cstr());

			//材质颜色
			CheckDlgButton(hDlg,IDC_CHC_LOD,BST_CHECKED);
			hwdTemp=GetDlgItem(hDlg,IDC_CHC_LOD);
			EnableWindow(hwdTemp,TRUE);

			// 去除重复点
			CheckDlgButton(hDlg, IDC_CHC_REMOVE,BST_UNCHECKED);
			hwdTemp=GetDlgItem(hDlg, IDC_CHC_REMOVE);
			EnableWindow(hwdTemp,TRUE);

			// 带不带法线
			CheckDlgButton(hDlg, IDC_CHC_WITHOUTNORMAL,BST_UNCHECKED);
			hwdTemp=GetDlgItem(hDlg, IDC_CHC_WITHOUTNORMAL);
			EnableWindow(hwdTemp,TRUE);

			return FALSE;
		}
	case WM_DESTROY:
		return FALSE;
	case WM_COMMAND:
		switch(LOWORD(wParam)) {
		case IDOK:
			{
				LPTSTR longitudeC=new TCHAR[20];
				LPTSTR latitudeC=new TCHAR[20];
				LPTSTR altitudeC=new TCHAR[20];
				LPTSTR planexC=new TCHAR[20];
				LPTSTR planeyC=new TCHAR[20];
				LPTSTR planezC=new TCHAR[20];
				GetDlgItemText(hDlg,IDC_LONGT,longitudeC,20); 
				GetDlgItemText(hDlg,IDC_LATIt,latitudeC,20);
				GetDlgItemText(hDlg,IDC_ALTIT,altitudeC,20);
				GetDlgItemText(hDlg,IDC_PLANEX,planexC,20); 
				GetDlgItemText(hDlg,IDC_PLANEY,planeyC,20);
				GetDlgItemText(hDlg,IDC_PLANEZ,planezC,20);


				if (IsDlgButtonChecked(hDlg,IDC_PLANEPOS))
				{
					bimExport->m_bPlanar = TRUE; 
				}
				else
				{
					bimExport->m_bPlanar = FALSE;
				}			
				if (IsDlgButtonChecked(hDlg,IDC_CHC_LOD))
				{
					bimExport->m_bIsIgnore = TRUE;
				}
				else
				{
					bimExport->m_bIsIgnore = FALSE;
				}

				if(IsDlgButtonChecked(hDlg,IDC_CHC_REMOVE))
				{
					bimExport->m_bIsRemoveDupVertex = TRUE;
				}
				else
				{
					bimExport->m_bIsRemoveDupVertex = FALSE;
				}

				bimExport->m_bIsExportNormal = IsDlgButtonChecked(hDlg,IDC_CHC_WITHOUTNORMAL) ? FALSE: TRUE;

				UGMaxLogFile* pProjectionHolder = UGMaxLogFile::GetMaxLogFile();
				if (!bimExport->m_bPlanar)
				{
					if((!UGMaxToolkit::IsFloat(longitudeC))||(!UGMaxToolkit::IsFloat(latitudeC))||(!UGMaxToolkit::IsFloat(altitudeC)))
					{
						UGC::UGStringEx warning;
						warning.LoadResString(UGS_MAX_LATLONG_HINT);
						UGC::UGStringEx hint;
						hint.LoadResString(UGS_MAX_HINT);
						MessageBox(hDlg,warning,hint,MB_OK|MB_ICONERROR);
						delete[] longitudeC;
						delete[] latitudeC;
						delete[] altitudeC;
						return FALSE;
					}
					if(!UGMaxToolkit::IsCorrectSphericalD((char*)longitudeC,(char*)latitudeC))
					{
						UGC::UGStringEx warning;
						warning.LoadResString(UGS_MAX_LATLONG_WARNING);
						UGC::UGStringEx hint;
						hint.LoadResString(UGS_MAX_HINT); 
						MessageBox(hDlg,warning,hint,MB_OK|MB_ICONERROR);
						delete[] longitudeC;
						delete[] latitudeC;
						delete[] altitudeC;
						return FALSE;
					}

					pProjectionHolder->SetLontitude(longitudeC);
					pProjectionHolder->SetLatitude(latitudeC);
					pProjectionHolder->SetAltitude(altitudeC);
					//	Modify by lina 2014-3-4
#ifdef _UGUNICODE
					UGC::UGString lonTemp(longitudeC);
					bimExport->m_dLon=lonTemp.ToDouble();//lina
					UGC::UGString latTemp(latitudeC);
					bimExport->m_dLat=latTemp.ToDouble();
					UGC::UGString altitTemp(altitudeC);
					bimExport->m_dAlt=altitTemp.ToDouble();
#else
					batchImport->m_dLon=atof(longitudeC);
					batchImport->m_dLat=atof(latitudeC);
					batchImport->m_dAlt=atof(altitudeC);
#endif

					delete[] longitudeC;
					delete[] latitudeC;
					delete[] altitudeC;
				}	
				else
				{
					if((!UGMaxToolkit::IsFloat(planexC))||(!UGMaxToolkit::IsFloat(planeyC))||(!UGMaxToolkit::IsFloat(planezC)))
					{
						UGC::UGStringEx warning;
						warning.LoadResString(UGS_MAX_PLANEXYZ_HINT);
						UGC::UGStringEx hint;
						hint.LoadResString(UGS_MAX_HINT);
						MessageBox(hDlg,warning,hint,MB_OK|MB_ICONERROR);
						delete[] planexC;
						delete[] planeyC;
						delete[] planezC;
						return FALSE;
					}
					pProjectionHolder->SetPlaneX(planexC);
					pProjectionHolder->SetPlaneY(planeyC);
					pProjectionHolder->SetPlaneZ(planezC);
					//	Modify by lina 2014-3-4
#ifdef _UGUNICODE
					UGC::UGString lonTemp(planexC);
					bimExport->m_dPlaneX=lonTemp.ToDouble();//lina
					UGC::UGString latTemp(planeyC);
					bimExport->m_dPlaneY=latTemp.ToDouble();
					UGC::UGString altitTemp(planezC);
					bimExport->m_dPlaneZ=altitTemp.ToDouble();
#else
					batchImport->m_dPlaneX=atof(planexC);
					batchImport->m_dPlaneY=atof(planeyC);
					batchImport->m_dPlaneZ=atof(planezC);
#endif
					delete[] planexC;
					delete[] planeyC;
					delete[] planezC;
				}	
				if(!UGC::UGFile::IsExist(bimExport->m_strDataSourceName))
				{
					UGC::UGStringEx warning;
					warning.LoadResString(UGS_INVALID_DS_PATH);
					UGC::UGStringEx hint;
					hint.LoadResString(UGS_MAX_HINT);
					MessageBox(hDlg,warning,hint,MB_OK|MB_ICONERROR);
					return FALSE;
				}

				LPTSTR pMaxDir = new TCHAR[MAX_PATH];
				GetDlgItemText(hDlg,IDC_TEXTUREPATH,pMaxDir,MAX_PATH);
				bimExport->m_strTexPath = pMaxDir;
				delete[] pMaxDir;


				LPTSTR pDatasetLPName = new TCHAR[20];
				GetDlgItemText(hDlg,IDC_DATASETS,pDatasetLPName,20);
				bimExport->m_strDataSetName = pDatasetLPName;
				UGC::UGString strDatesetName = pDatasetLPName;
				if(strDatesetName.IsEmpty())
				{
					UGC::UGStringEx warning;
					warning.LoadResString(UGS_MAX_DATASET_NAME_IS_NULL);
					UGC::UGStringEx hint;
					hint.LoadResString(UGS_MAX_HINT);
					MessageBox(hDlg,warning,hint,MB_OK|MB_ICONERROR);
					delete[] pDatasetLPName;
					return FALSE;
				}
				delete pDatasetLPName;		
				EndDialog(hDlg, 1);
				return TRUE;
			}
		case IDC_EARTHPOS:
			{
				HWND hwdTemp;
				hwdTemp=GetDlgItem(hDlg,IDC_LONGT);
				EnableWindow(hwdTemp,TRUE);
				hwdTemp=GetDlgItem(hDlg,IDC_LATIt);
				EnableWindow(hwdTemp,TRUE);
				hwdTemp=GetDlgItem(hDlg,IDC_ALTIT);
				EnableWindow(hwdTemp,TRUE);

				hwdTemp=GetDlgItem(hDlg,IDC_PLANEX);
				EnableWindow(hwdTemp,FALSE);
				hwdTemp=GetDlgItem(hDlg,IDC_PLANEY);
				EnableWindow(hwdTemp,FALSE);
				hwdTemp=GetDlgItem(hDlg,IDC_PLANEZ);
				EnableWindow(hwdTemp,FALSE);
				return FALSE;
			}
		case IDC_PLANEPOS:
			{
				HWND hwdTemp;
				hwdTemp=GetDlgItem(hDlg,IDC_LONGT);
				EnableWindow(hwdTemp,FALSE);
				hwdTemp=GetDlgItem(hDlg,IDC_LATIt);
				EnableWindow(hwdTemp,FALSE);
				hwdTemp=GetDlgItem(hDlg,IDC_ALTIT);
				EnableWindow(hwdTemp,FALSE);

				hwdTemp=GetDlgItem(hDlg,IDC_PLANEX);
				EnableWindow(hwdTemp,TRUE);
				hwdTemp=GetDlgItem(hDlg,IDC_PLANEY);
				EnableWindow(hwdTemp,TRUE);
				hwdTemp=GetDlgItem(hDlg,IDC_PLANEZ);
				EnableWindow(hwdTemp,TRUE);
				return FALSE;
			}
		case IDC_DATASOURCE:
			{
				OPENFILENAME a;
				TCHAR szFile[MAX_PATH];
				ZeroMemory(&a,sizeof(a));
				a.lStructSize=sizeof(a);
				a.hwndOwner=hDlg;
				a.lpstrFile=szFile;
				a.lpstrFile[0]='\0';
				a.nMaxFile=sizeof(szFile);
				a.lpstrFilter=_U("All Support Files(*.udb)\0*.udb\0");
				a.nFilterIndex=1;
				a.lpstrFileTitle=NULL;
				a.nMaxFileTitle=0;
				a.Flags=OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST;
				GetOpenFileName(&a);;
				UGC::UGString StrPathName(szFile);
				if(!UGC::UGFile::IsExist(StrPathName))
					return FALSE;
				SetDlgItemText(hDlg,IDC_EXPORTPATH,szFile);
				bimExport->m_strDataSourceName=StrPathName;

				UGArray<UGC::UGString> existedDatasets = bimExport->GetDataSets(StrPathName,OGDC::OgdcDataset::Model);


				HWND texLevelTp=GetDlgItem(hDlg,IDC_DATASETS);
				SendMessage(texLevelTp,CB_RESETCONTENT,0,0);
				UGC::UGint numDataset = existedDatasets.GetSize();
				UGC::UGint i = 0;
				for(;i<numDataset;i++)
				{
					UGC::UGString strDatasetName = existedDatasets.GetAt(i);
					TCHAR *pDatasetName =(TCHAR *)strDatasetName.Cstr();
					SendMessage(texLevelTp,CB_ADDSTRING,0,(LPARAM)pDatasetName);
				}
				SendMessage(texLevelTp,CB_SETCURSEL,0,0);
				return FALSE;
			}
		case IDC_CREATEDATASET:
			{	
				DialogBoxParam(hInstance,
					MAKEINTRESOURCE(IDD_DATASETNAME),
					hDlg,
					ModelDatasetProc,
					(LPARAM)&(bimExport->m_strDataSetName));
				if((bimExport->m_strDataSetName).IsEmpty())
					return FALSE;
				HWND texLevelTp=GetDlgItem(hDlg,IDC_DATASETS);
				TCHAR *pDatasetName =(TCHAR *)(bimExport->m_strDataSetName).Cstr();
				SendMessage(texLevelTp,CB_INSERTSTRING,0,(LPARAM)pDatasetName);
				SendMessage(texLevelTp,CB_SETCURSEL,0,0);
				return FALSE;
			}
		case IDC_TEXPATH:
			{
				TCHAR dispathname[MAX_PATH];
				GetDlgItemText(hDlg,IDC_TEXTUREPATH,dispathname,MAX_PATH);
				UGC::UGStringEx warning;
				warning.LoadResString(UGS_MAX_TEXTURE_PATH_HINT);
				bimExport->m_strTexPath=UGMaxToolkit::GetInputPath((TCHAR*)warning.Cstr(),IDC_TEXTUREPATH,hDlg,true,dispathname);
				return FALSE;
			}
		case IDC_CHC_LOD:
			{
				HWND hwdTemp;
				hwdTemp=GetDlgItem(hDlg,IDC_CHC_LOD);
				UGC::UGint nCheck = IsDlgButtonChecked(hDlg, IDC_CHC_LOD);
				if (nCheck == BST_CHECKED)
				{	
					bimExport->m_isLOD = TRUE;
				}
				else
				{
					bimExport->m_isLOD = FALSE;
				}
			}
			break;
		case IDCANCEL:
			EndDialog(hDlg, 0);
			return TRUE;
		}
	}
	return FALSE;
}

#endif

class UGSuActions{
public:
	//!  \获取动作列表
	UGSuActions(HINSTANCE outHInstance){hInstance=outHInstance;}
	ActionTable* GetActions();
	ActionTableId GetActionsId(){return SuActionId;}
private:
	void AppendOperation(ActionTable* pTab);
	HINSTANCE hInstance;
};

ActionTable*  UGSuActions::GetActions(){
#ifdef _UGUNICODE  
	WStr name(_U("SuActions"));
#else
	CStr name("SuActions");
#endif
	ActionTable* pTab=new ActionTable(SuActionId,SuActionContextId,name);
	AppendOperation(pTab);
	GetCOREInterface()->GetActionManager()->RegisterActionContext(SuActionContextId, name.data());
	return pTab;
}

//!  \在此添加定义好的动作类
void UGSuActions::AppendOperation(ActionTable* pTab)
{
	UGC::UGStringEx name;
	UGC::UGStringEx catText = _U("SuperMapActions");

	int actionIndex=1;
	
	//!  \批量导出模型
	BatchExport* pBatchExport = new BatchExport(hInstance);
	name.LoadResString(UGS_MAX_BATCH_EXPORT);
	pBatchExport->Init(actionIndex++,name,_U(""),catText,_U("Export S3MB"));
	pTab->AppendOperation(pBatchExport);

#ifdef MAX2014
	//!  \BIM生成数据集
	BIMExport* pBIMExport = new BIMExport(hInstance);
	name.LoadResString(UGS_MAX_EXPORT_BIM);
	pBIMExport->Init(actionIndex++,name,_U(""),catText,_U("Export BIM"));
	pTab->AppendOperation(pBIMExport);
#endif
}
#endif 
