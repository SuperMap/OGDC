
// OGDC__XML+OSGToUDBDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OGDC__XML+OSGToUDB.h"
#include "OGDC__XML+OSGToUDBDlg.h"
#include "afxdialogex.h"
#include "Stream/UGFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// COGDC__XMLOSGToUDBDlg 对话框



COGDC__XMLOSGToUDBDlg::COGDC__XMLOSGToUDBDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COGDC__XMLOSGToUDBDlg::IDD, pParent)
	, m_UdbPath(_T(""))
	, b_IsLonLat(TRUE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COGDC__XMLOSGToUDBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FOLDER, m_Folder);
	DDX_Text(pDX, IDC_EDIT_UDBPATH, m_UdbPath);
	DDX_Check(pDX, IDC_CHECK_ISLONLAT, b_IsLonLat);
}

BEGIN_MESSAGE_MAP(COGDC__XMLOSGToUDBDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &COGDC__XMLOSGToUDBDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_SCANFOLDER, &COGDC__XMLOSGToUDBDlg::OnBnClickedButtonScanfolder)
	ON_BN_CLICKED(IDC_BUTTON_SCANUDB, &COGDC__XMLOSGToUDBDlg::OnBnClickedButtonScanudb)
END_MESSAGE_MAP()


// COGDC__XMLOSGToUDBDlg 消息处理程序

BOOL COGDC__XMLOSGToUDBDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void COGDC__XMLOSGToUDBDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR COGDC__XMLOSGToUDBDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void COGDC__XMLOSGToUDBDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//CDialogEx::OnOK();
	GetAllXMLPath();
	if (fileMap.size() < 1)
	{
		AfxMessageBox(_T("未在指定文件夹下找到任何XML文件"), NULL, NULL);
	}
	if (!CreateDatasource())//创建并打开数据源
	{
		AfxMessageBox(_T("创建数据源失败"), NULL, NULL);
	}
	map<CString, CString>::iterator iter;
	for (iter = fileMap.begin(); iter != fileMap.end(); iter++)
	{
		ReadXmlFile(iter->second);
		OgdcDatasetVector* pDatasetVector = CreateDatasetVector(iter->first, b_IsLonLat);
		bool result = WriteModelToRecorderset(pDatasetVector);
		pDatasetVector->Close();
		mapModel.clear();
	}
	pUDBDataSource->Close();
	MessageBox(_T("导出成功！", NULL, NULL));
	return;
}


void COGDC__XMLOSGToUDBDlg::OnBnClickedButtonScanfolder()
{
	// TODO: 在此添加控件通知处理程序代码
	CFolderPickerDialog dlg(NULL, 0, this, 0);
	if (dlg.DoModal() == IDOK)
	{
		m_Folder.SetString(dlg.GetFolderPath());
		UpdateData(FALSE);
	}
}

void COGDC__XMLOSGToUDBDlg::OnBnClickedButtonScanudb()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE, _T("udb"), NULL, 0, _T("数据源文件(*.udb)|*.UDB||", this));
	if (dlg.DoModal() == IDOK)
	{
		m_UdbPath = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void COGDC__XMLOSGToUDBDlg::GetAllXMLPath()
{
	CFileFind finder;
	CString filePathName;
	CString fileName;
	BOOL result = finder.FindFile(m_Folder + _T("\\*.xml"));
	while (result)
	{
		result = finder.FindNextFileW();
		filePathName = finder.GetFilePath();
		fileName = finder.GetFileTitle();
		fileMap[fileName] = filePathName;
	}
}

bool COGDC__XMLOSGToUDBDlg::CreateDatasource()
{
	OgdcString UDBSever = m_UdbPath;
	pUDBDataSource = OgdcProviderManager::CreateOgdcDataSource(OGDC::oeFile);
	pUDBDataSource->m_nEngineClass = 2;
	pUDBDataSource->m_connection.m_strServer = UDBSever;
	pUDBDataSource->Create();
	if (!((OGDC::OgdcDataSource*)pUDBDataSource)->Open())
	{
		delete pUDBDataSource;
		return FALSE;
	}
	return TRUE;
}

OgdcDatasetVector* COGDC__XMLOSGToUDBDlg::CreateDatasetVector(CString m_DatasetName, BOOL b_IsLonLat)
{
	OGDC::OgdcDatasetVectorInfo DatasetVectorInfo;
	OgdcString name = m_DatasetName;
	DatasetVectorInfo.m_strName = pUDBDataSource->GetUnoccupiedDatasetName(name);
	DatasetVectorInfo.m_nType = OGDC::OgdcDataset::Model;

	OGDC::OgdcDatasetVector* pDatasetVector = pUDBDataSource->CreateDatasetVector(DatasetVectorInfo);
	if (b_IsLonLat)
	{
		OGDC::OgdcCoordSys prjCoordSys;
		prjCoordSys.SetEarth(6378137, 0.00335281066474748, 10000);
		pDatasetVector->SetCoordSys(prjCoordSys);
	}
	else
	{
		OGDC::OgdcCoordSys prjCoordSys;
		pDatasetVector->SetCoordSys(prjCoordSys);
	}
	return pDatasetVector;
}

bool COGDC__XMLOSGToUDBDlg::ReadXmlFile(CString m_FilePath)
{
	UGC::UGMarkup markup;
	if (!markup.Load(m_FilePath))
	{
		return false;
	}
	UGString strElemName = L"ModelPoint";
	while (markup.FindChildElem(strElemName))
	{
		markup.IntoElem();
		UGString m_NameNode = L"ModelName";
		UGString m_ModelName;
		UGString m_ModelPath;
		ModelInfo modelInfo;
		modelInfo.position = new UGPoint3D();
		if (markup.FindChildElem(m_NameNode))
		{
			m_ModelName = markup.GetChildData();
			m_ModelPath = UGFile::GetDir((UGString)m_FilePath) + m_ModelName + L".osg";
		}
		UGString m_LocationNodeX = L"LocationX";
		if (markup.FindChildElem(m_LocationNodeX))
		{
			modelInfo.position->x = markup.GetChildData().ToDouble();
		}
		UGString m_LocationNodeY = L"LocationY";
		if (markup.FindChildElem(m_LocationNodeY))
		{
			modelInfo.position->y = markup.GetChildData().ToDouble();
		}
		UGString m_LocationNodeZ = L"LocationZ";
		if (markup.FindChildElem(m_LocationNodeZ))
		{
			modelInfo.position->z = markup.GetChildData().ToDouble();
		}
		UGString m_MatrixNode = L"Matrix3";
		if (markup.FindChildElem(m_MatrixNode))
		{
			UGString str_Matrix = markup.GetChildData();
			OgdcUnicodeStringArray arr_Matrix;
			if (str_Matrix.Split(arr_Matrix, L",", false) == 9)
			{
				modelInfo.matrix = new UGMatrix4d(arr_Matrix[0].ToDouble(), arr_Matrix[1].ToDouble(), arr_Matrix[2].ToDouble(),0,
					arr_Matrix[3].ToDouble(), arr_Matrix[4].ToDouble(), arr_Matrix[5].ToDouble(),0,
					arr_Matrix[6].ToDouble(), arr_Matrix[7].ToDouble(), arr_Matrix[8].ToDouble(),0,
					0, 0, 0, 1);
			}
		}
		map<UGString, vector<ModelInfo>>::iterator iter = mapModel.find(m_ModelPath);
		if (iter != mapModel.end())
		{
			iter->second.push_back(modelInfo);
		}
		else
		{
			vector<ModelInfo> infos;
			infos.push_back(modelInfo);
			mapModel[m_ModelPath] = infos;
		}
		markup.OutOfElem();
	}
	return true;
}

UGModelNode* COGDC__XMLOSGToUDBDlg::ReadOSGToModelNode(UGString modelPath)
{
	UGString strExt = UGFile::GetExt(modelPath);
	UGC::UGImportParams params;
	params.SetFilePathName(modelPath);
	UGFileParser* pFileParser;
	pFileParser = UGFileParseManager::CreateFileParser(UGFileType::ModelOSG);
	UGFileParseModel* pFileParserModel = (UGFileParseModel*)pFileParser;
	pFileParserModel->SetParseModelNode(true);
	if (pFileParser->Open(params))
	{
		UGModelNode* modelNode = pFileParserModel->GetModelNode();
		return modelNode;
	}
	return NULL;
}

bool COGDC__XMLOSGToUDBDlg::WriteModelToRecorderset(OgdcDatasetVector* pDatasetVector)
{
	//创建记录集
	OgdcQueryDef queryDef;
	pDatasetVector->Open();
	queryDef.m_nType = OgdcQueryDef::General;
	queryDef.m_nOptions = OgdcQueryDef::Both;
	//queryDef.m_nMode=OgdcQueryDef::GeneralQuery;
	queryDef.m_nCursorType = OgdcQueryDef::OpenDynamic;
	OGDC::OgdcRecordset* pRecordset = pDatasetVector->Query(queryDef);
	if (NULL == pRecordset)
	{
		return FALSE;
	}
	//创建模型并写入记录集
	map<UGString, vector<ModelInfo>> ::iterator iter;
	for (iter = mapModel.begin(); iter != mapModel.end(); iter++)
	{
		UGModelNode* pModelNode = ReadOSGToModelNode(iter->first);
		vector<ModelInfo> infos = iter->second;
		vector<ModelInfo>::iterator iter_ModelInfo;
		for (iter_ModelInfo = infos.begin(); iter_ModelInfo != infos.end(); iter_ModelInfo++)
		{
			UGGeoModelPro* pGeoModelPro = new UGGeoModelPro(pModelNode,false);
			std::vector<UGModelGeode *> geodes = pModelNode->GetGeodes();
			for (int i = 0 ;i<geodes.size();i++)
			{
				UGModelGeode * geode = geodes[i];
				geode->SetPosition(*iter_ModelInfo->matrix);
			}

			std::vector<UGModelPagedLOD*> PagedLODS = pModelNode->GetPagedLODs();
			for (int i = 0;i<PagedLODS.size();i++)
			{
				UGModelPagedLOD* pageLod = PagedLODS[i];
			    std::vector<UGModelGeode *> geodes1 = pageLod->GetGeodes();
				for (int i = 0;i< geodes1.size();i++)
				{
					UGModelGeode * geode = geodes1[i];
					geode->SetPosition(*iter_ModelInfo->matrix);
				}
			}
			pGeoModelPro->SetPosition(*iter_ModelInfo->position);
			pGeoModelPro->SetSpherePlaced(b_IsLonLat);
			pRecordset->AddNew(pGeoModelPro);
			pRecordset->Update();
		}
		delete pModelNode;
		pModelNode = NULL;
	}
	pRecordset->Close();
	return TRUE;
}
