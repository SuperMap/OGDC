/********************************************************************************
** Form generated from reading UI file 'ifctools.ui'
**
** Created by: Qt User Interface Compiler version 5.1.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IFCTOOLS_H
#define UI_IFCTOOLS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_IFCToolsClass
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QLineEdit *mXLineEdit;
    QLabel *mLabelX;
    QLineEdit *mZLineEdit;
    QLineEdit *mYLineEdit;
    QLabel *mLabelZ;
    QLabel *mLabelY;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QLineEdit *mDataSource;
    QToolButton *mDataSrcFile;
    QTableWidget *mFileWidget;
    QGridLayout *gridLayout_4;
    QGridLayout *gridLayout_3;
    QPushButton *mChooseFiles;
    QPushButton *mAddFiles;
    QPushButton *mDeleteFiles;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *mExportDataset;

    void setupUi(QMainWindow *IFCToolsClass)
    {
        if (IFCToolsClass->objectName().isEmpty())
            IFCToolsClass->setObjectName(QStringLiteral("IFCToolsClass"));
        IFCToolsClass->resize(281, 358);
        centralWidget = new QWidget(IFCToolsClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setEnabled(true);
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        mXLineEdit = new QLineEdit(groupBox);
        mXLineEdit->setObjectName(QStringLiteral("mXLineEdit"));
        mXLineEdit->setInputMethodHints(Qt::ImhDigitsOnly);

        gridLayout_2->addWidget(mXLineEdit, 0, 1, 1, 1);

        mLabelX = new QLabel(groupBox);
        mLabelX->setObjectName(QStringLiteral("mLabelX"));

        gridLayout_2->addWidget(mLabelX, 0, 0, 1, 1);

        mZLineEdit = new QLineEdit(groupBox);
        mZLineEdit->setObjectName(QStringLiteral("mZLineEdit"));
        mZLineEdit->setInputMethodHints(Qt::ImhDigitsOnly);

        gridLayout_2->addWidget(mZLineEdit, 2, 1, 1, 1);

        mYLineEdit = new QLineEdit(groupBox);
        mYLineEdit->setObjectName(QStringLiteral("mYLineEdit"));
        mYLineEdit->setInputMethodHints(Qt::ImhDigitsOnly);

        gridLayout_2->addWidget(mYLineEdit, 1, 1, 1, 1);

        mLabelZ = new QLabel(groupBox);
        mLabelZ->setObjectName(QStringLiteral("mLabelZ"));

        gridLayout_2->addWidget(mLabelZ, 2, 0, 1, 1);

        mLabelY = new QLabel(groupBox);
        mLabelY->setObjectName(QStringLiteral("mLabelY"));

        gridLayout_2->addWidget(mLabelY, 1, 0, 1, 1);


        verticalLayout->addWidget(groupBox);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 0, 0, 1, 1);

        mDataSource = new QLineEdit(centralWidget);
        mDataSource->setObjectName(QStringLiteral("mDataSource"));

        gridLayout->addWidget(mDataSource, 0, 1, 1, 1);

        mDataSrcFile = new QToolButton(centralWidget);
        mDataSrcFile->setObjectName(QStringLiteral("mDataSrcFile"));

        gridLayout->addWidget(mDataSrcFile, 0, 2, 1, 1);


        verticalLayout->addLayout(gridLayout);

        mFileWidget = new QTableWidget(centralWidget);
        mFileWidget->setObjectName(QStringLiteral("mFileWidget"));
        mFileWidget->setRowCount(0);
        mFileWidget->setColumnCount(0);

        verticalLayout->addWidget(mFileWidget);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setSpacing(6);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(6);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        mChooseFiles = new QPushButton(centralWidget);
        mChooseFiles->setObjectName(QStringLiteral("mChooseFiles"));
        mChooseFiles->setMaximumSize(QSize(32, 32));
        QIcon icon;
        icon.addFile(QStringLiteral(":/IFCTools/Resources/add.png"), QSize(), QIcon::Normal, QIcon::Off);
        mChooseFiles->setIcon(icon);
        mChooseFiles->setIconSize(QSize(16, 16));

        gridLayout_3->addWidget(mChooseFiles, 0, 0, 1, 1);

        mAddFiles = new QPushButton(centralWidget);
        mAddFiles->setObjectName(QStringLiteral("mAddFiles"));
        mAddFiles->setMaximumSize(QSize(32, 32));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/IFCTools/Resources/edit_add.png"), QSize(), QIcon::Normal, QIcon::Off);
        mAddFiles->setIcon(icon1);

        gridLayout_3->addWidget(mAddFiles, 0, 1, 1, 1);

        mDeleteFiles = new QPushButton(centralWidget);
        mDeleteFiles->setObjectName(QStringLiteral("mDeleteFiles"));
        mDeleteFiles->setMaximumSize(QSize(32, 32));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/IFCTools/Resources/edit_remove.png"), QSize(), QIcon::Normal, QIcon::Off);
        mDeleteFiles->setIcon(icon2);
        mDeleteFiles->setIconSize(QSize(16, 16));
        mDeleteFiles->setCheckable(false);
        mDeleteFiles->setChecked(false);
        mDeleteFiles->setAutoRepeat(false);

        gridLayout_3->addWidget(mDeleteFiles, 0, 2, 1, 1);


        gridLayout_4->addLayout(gridLayout_3, 0, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_2, 0, 1, 1, 1);

        mExportDataset = new QPushButton(centralWidget);
        mExportDataset->setObjectName(QStringLiteral("mExportDataset"));

        gridLayout_4->addWidget(mExportDataset, 0, 2, 1, 1);


        verticalLayout->addLayout(gridLayout_4);

        IFCToolsClass->setCentralWidget(centralWidget);

        retranslateUi(IFCToolsClass);

        QMetaObject::connectSlotsByName(IFCToolsClass);
    } // setupUi

    void retranslateUi(QMainWindow *IFCToolsClass)
    {
        IFCToolsClass->setWindowTitle(QApplication::translate("IFCToolsClass", "IFC\345\257\274\345\205\245\345\267\245\345\205\267", 0));
        groupBox->setTitle(QApplication::translate("IFCToolsClass", "\346\217\222\345\205\245\347\202\271", 0));
        mXLineEdit->setText(QApplication::translate("IFCToolsClass", "0", 0));
        mLabelX->setText(QApplication::translate("IFCToolsClass", "X", 0));
        mZLineEdit->setText(QApplication::translate("IFCToolsClass", "0", 0));
        mYLineEdit->setText(QApplication::translate("IFCToolsClass", "0", 0));
        mLabelZ->setText(QApplication::translate("IFCToolsClass", "Z", 0));
        mLabelY->setText(QApplication::translate("IFCToolsClass", "Y", 0));
        label_2->setText(QApplication::translate("IFCToolsClass", "\346\225\260\346\215\256\346\272\220", 0));
        mDataSrcFile->setText(QApplication::translate("IFCToolsClass", "...", 0));
#ifndef QT_NO_TOOLTIP
        mChooseFiles->setToolTip(QApplication::translate("IFCToolsClass", "\346\267\273\345\212\240\345\244\232\344\270\252\346\226\207\344\273\266", 0));
#endif // QT_NO_TOOLTIP
        mChooseFiles->setText(QString());
#ifndef QT_NO_TOOLTIP
        mAddFiles->setToolTip(QApplication::translate("IFCToolsClass", "\346\267\273\345\212\240\345\215\225\344\270\252\346\226\207\344\273\266", 0));
#endif // QT_NO_TOOLTIP
        mAddFiles->setText(QString());
#ifndef QT_NO_TOOLTIP
        mDeleteFiles->setToolTip(QApplication::translate("IFCToolsClass", "\347\247\273\351\231\244\351\200\211\344\270\255\346\226\207\344\273\266", 0));
#endif // QT_NO_TOOLTIP
        mDeleteFiles->setText(QString());
        mExportDataset->setText(QApplication::translate("IFCToolsClass", "\345\257\274\345\205\245\346\225\260\346\215\256\351\233\206", 0));
    } // retranslateUi

};

namespace Ui {
    class IFCToolsClass: public Ui_IFCToolsClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IFCTOOLS_H
