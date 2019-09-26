
// OGDC__XML+OSGToUDBDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "Base3D/UGModelNodeEntities.h"
#include "FileParser/UGFileParseToolkit.h"
#include "Base3D/UGModelNodeShells.h"
#include "Geometry3D/UGGeoModelPro.h"
#include "OGDC/OgdcDataSource.h"
#include "OGDC/OgdcProviderManager.h"
#include <vector>
#include "Toolkit/UGMarkup.h"
#include "FileParser/UGFileParseManager.h"
#include "FileParser/UGFileParseModel.h"
#include "FileParser/UGExchangeParamsManager.h"

using namespace std;
using namespace UGC;
// COGDC__XMLOSGToUDBDlg 对话框
class COGDC__XMLOSGToUDBDlg : public CDialogEx
{
// 构造
public:
	COGDC__XMLOSGToUDBDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_OGDC__XMLOSGTOUDB_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonScanfolder();
	afx_msg void OnBnClickedButtonScanudb();
	CString m_Folder;
	CString m_UdbPath;
	BOOL b_IsLonLat;
public:
	void GetAllXMLPath();
	bool CreateDatasource();
	OgdcDatasetVector* CreateDatasetVector(CString m_DatasetName,BOOL b_IsLonLat);
	bool ReadXmlFile(CString m_FilePath);
	UGModelNode* ReadOSGToModelNode(UGString modelPath);
	bool WriteModelToRecorderset(OgdcDatasetVector* pDatasetVector);
public:
	struct ModelInfo
	{
		UGPoint3D* position;
		UGMatrix4d* matrix;
	};
	map<CString, CString> fileMap;
	OgdcDataSource* pUDBDataSource;
	map<UGString,vector<ModelInfo>> mapModel;
};
