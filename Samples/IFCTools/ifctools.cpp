#include "ifctools.h"
#include "ifcconvector.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTableWidgetItem>
#include "Stream\UGFile.h"

IFCTools::IFCTools(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	pHeaderTableWidgetItem1 = NULL;
	pHeaderTableWidgetItem2 = NULL;
	pHeaderTableWidgetItem3 = NULL;

	connect(ui.mFileWidget, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(GetDataSetName(QTableWidgetItem*)));

	reset();
}

IFCTools::~IFCTools()
{
	clear();
}

void IFCTools::reset()
{
	clear();

	pHeaderTableWidgetItem1 = new QTableWidgetItem(QString::fromLocal8Bit("IFC文件"));
	pHeaderTableWidgetItem2 = new QTableWidgetItem(QString::fromLocal8Bit("数据集名称"));
	pHeaderTableWidgetItem3 = new QTableWidgetItem(QString::fromLocal8Bit("结果"));

	ui.mFileWidget->setColumnCount(3);
	ui.mFileWidget->setHorizontalHeaderItem(0, pHeaderTableWidgetItem1);
	ui.mFileWidget->setHorizontalHeaderItem(1, pHeaderTableWidgetItem2);
	ui.mFileWidget->setHorizontalHeaderItem(2, pHeaderTableWidgetItem3);
}

void IFCTools::clear()
{
	ui.mFileWidget->clear();
	UGuint nSize = ui.mFileWidget->rowCount();
	while (nSize != 0)
	{
		ui.mFileWidget->removeRow(0);
		nSize = ui.mFileWidget->rowCount();
	}

	// 	if (vecItems.size() > 0)
	// 	{
	// 		UGuint nSize = vecItems.size();
	// 		for(UGuint i=0; i<nSize; i++)
	// 		{
	// 		    QTableWidgetItem* pItem = vecItems[i];
	// 			if (pItem)
	// 			{
	// 				delete pItem;
	// 				pItem = NULL;
	// 			}
	// 		}
	// 		vecItems.clear();
	// 	}
	vecItems.clear();
	// 	if (pHeaderTableWidgetItem1)
	// 	{
	// 		delete pHeaderTableWidgetItem1;
	// 		pHeaderTableWidgetItem1 = NULL;
	// 	}
	// 
	// 	if (pHeaderTableWidgetItem2)
	// 	{
	// 		delete pHeaderTableWidgetItem2;
	// 		pHeaderTableWidgetItem2 = NULL;
	// 	}

	files.clear();
	fileToDataSetNames.clear();
}

void IFCTools::on_mDataSrcFile_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Source File"), "/home", tr("IFC file (*.udb)"));

	if (!fileName.isEmpty())
	{
		ui.mDataSource->setText(fileName);
	}
}

void IFCTools::on_mExportDataset_clicked()
{
	if (ui.mDataSource->text().isEmpty() || files.size() ==0)
	{
		QMessageBox::warning(this,QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("不能为空！"));
		return;
	}

	UGC::UGPoint3D point(ui.mXLineEdit->text().toDouble(),
		ui.mYLineEdit->text().toDouble(),
		ui.mZLineEdit->text().toDouble());
	UGC::UGbool bPlaceSphere = FALSE;
	QString dataSrcName = ui.mDataSource->text();

	int nSize = files.size();
	for (int i=0; i<nSize; i++)
	{
		IFCConvector tool;
		UGbool bSuccess = tool.ParseFile(files[i].toStdWString());
		if (!bSuccess)
		{
			QTableWidgetItem* pItem = new QTableWidgetItem(QString::fromLocal8Bit("生成失败!"));
			vecItems.push_back(pItem);
			ui.mFileWidget->setItem(i, 2, pItem);
			continue;
		}
		QString dataSrcName = ui.mDataSource->text();

		tool.setPosition(point);
		tool.setIsPlaceSphere(bPlaceSphere);

		UGC::UGString strDataSrc;
		strDataSrc.FromMBString(dataSrcName.toLocal8Bit().data());

		UGC::UGString strFileName; 
		strFileName.FromMBString(fileToDataSetNames[files[i]].toLocal8Bit().data());

		if (strFileName.IsEmpty())
		{
			strFileName  = _U("ifcModel_") + UGString::From(i);
			QTableWidgetItem* pItem = ui.mFileWidget->item(i, 1);
			if (pItem)
			{
				pItem->setText(QString("ifcModel_") + QString::number(i));
			}
		}

		bSuccess = tool.ExportUDB(strDataSrc, UGFile::GetTitle(strFileName));

		if (!bSuccess)
		{
			QTableWidgetItem* pItem = new QTableWidgetItem(QString::fromLocal8Bit("生成失败!"));
			vecItems.push_back(pItem);
			ui.mFileWidget->setItem(i, 2, pItem);
			continue;
		}

		QTableWidgetItem* pItem = new QTableWidgetItem(QString::fromLocal8Bit("生成成功!"));
		vecItems.push_back(pItem);
		ui.mFileWidget->setItem(i, 2, pItem);
	}

	/*UGC::UGString strDataSrc;
	strDataSrc.FromMBString(dataSrcName.toLocal8Bit().data());
	OgdcDataSource* pUDBDataSource = OgdcProviderManager::CreateOgdcDataSource(OGDC::oeFile);
	pUDBDataSource->m_nEngineClass = 2;
	pUDBDataSource->m_connection.m_strServer = strDataSrc;
	if (!((OgdcDataSource*)pUDBDataSource)->Open())
	{
		QMessageBox::warning(NULL, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("紧缩数据集失败！"));
		delete pUDBDataSource;
		pUDBDataSource = NULL;
	}

	if(pUDBDataSource)
	{
	}
	if (pUDBDataSource)
	{
		delete pUDBDataSource;
		pUDBDataSource = NULL;
	}

	QMessageBox::warning(this, tr("Success"), tr("finished!"));*/
}

void IFCTools::on_mChooseFiles_clicked()
{

	//获取文件夹
	QStringList fileNames = QFileDialog::getOpenFileNames(
		this,
		"Select one or more files to open",
		"/home",
		"IFC Files (*.ifc)");

	if (fileNames.size() > 0)
	{
		reset();
		files = fileNames;
		int nFiles = files.size();
		for (int i = 0; i<nFiles; i++)
		{
			QString file = files[i];

			ui.mFileWidget->insertRow(i);
			QTableWidgetItem* pItem1 = new QTableWidgetItem(file);
			QTableWidgetItem* pItem2 = new QTableWidgetItem();
			vecItems.push_back(pItem1);
			vecItems.push_back(pItem2);
			ui.mFileWidget->setItem(i, 0, pItem1);
			ui.mFileWidget->setItem(i, 1, pItem2);
		}
	}
	else
	{
		return;
	}

}

void IFCTools::on_mDeleteFiles_clicked()
{
	QList<QTableWidgetItem*> itemList = ui.mFileWidget->selectedItems();

	int nSize = itemList.size();
	for (int i=0; i< nSize; i++)
	{
		files.removeOne(itemList[i]->text());
		int nRow = ui.mFileWidget->row(itemList[i]);
		ui.mFileWidget->removeRow(nRow);
	}
}

void IFCTools::on_mAddFiles_clicked()
{
	QString file = QFileDialog::getOpenFileName(this, tr("Open File"),
		"/home",
		tr("IFC Files (*.ifc)"));

	if (!file.isEmpty())
	{
		files.push_back(file);

		int i = files.size() - 1;
		ui.mFileWidget->insertRow(i);
		QTableWidgetItem* pItem1 = new QTableWidgetItem(file);
		QTableWidgetItem* pItem2 = new QTableWidgetItem();
		vecItems.push_back(pItem1);
		vecItems.push_back(pItem2);
		ui.mFileWidget->setItem(i, 0, pItem1);
		ui.mFileWidget->setItem(i, 1, pItem2);
	}
}

void IFCTools::GetDataSetName(QTableWidgetItem* pItem)
{
	if(ui.mFileWidget->column(pItem) != 1)
	{
		return;
	}

	QTableWidgetItem * pItem1 = pItem;
	if (pItem1 == NULL)
	{
		return;
	}

	int row = ui.mFileWidget->row(pItem);

	QTableWidgetItem * pItem2 = ui.mFileWidget->item(row, 0);
	if (pItem2 == NULL)
	{
		return;
	}

	QString file = pItem2->text();
	QString dataSetName = pItem1->text();
	fileToDataSetNames[file] = dataSetName;
}
