#ifndef IFCTOOLS_H
#define IFCTOOLS_H

#include <QtWidgets/QMainWindow>
#include "ui_ifctools.h"

class IFCTools : public QMainWindow
{
	Q_OBJECT

public:
	IFCTools(QWidget *parent = 0);
	~IFCTools();

private:
	Ui::IFCToolsClass ui;
private:
	void reset();
	void clear();
public slots:

	void on_mDataSrcFile_clicked();
	void on_mExportDataset_clicked();
	void on_mChooseFiles_clicked();
	void on_mDeleteFiles_clicked();
	void on_mAddFiles_clicked();

public slots:
	void GetDataSetName(QTableWidgetItem* pItem);

private:
	QStringList files;
	std::map<QString, QString> fileToDataSetNames;

	QTableWidgetItem * pHeaderTableWidgetItem1;
	QTableWidgetItem * pHeaderTableWidgetItem2;
	QTableWidgetItem * pHeaderTableWidgetItem3;

	std::vector<QTableWidgetItem*> vecItems;
};

#endif // IFCTOOLS_H
