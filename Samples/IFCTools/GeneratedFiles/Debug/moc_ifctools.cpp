/****************************************************************************
** Meta object code from reading C++ file 'ifctools.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.1.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ifctools.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ifctools.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.1.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_IFCTools_t {
    QByteArrayData data[10];
    char stringdata[169];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_IFCTools_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_IFCTools_t qt_meta_stringdata_IFCTools = {
    {
QT_MOC_LITERAL(0, 0, 8),
QT_MOC_LITERAL(1, 9, 23),
QT_MOC_LITERAL(2, 33, 0),
QT_MOC_LITERAL(3, 34, 25),
QT_MOC_LITERAL(4, 60, 23),
QT_MOC_LITERAL(5, 84, 23),
QT_MOC_LITERAL(6, 108, 20),
QT_MOC_LITERAL(7, 129, 14),
QT_MOC_LITERAL(8, 144, 17),
QT_MOC_LITERAL(9, 162, 5)
    },
    "IFCTools\0on_mDataSrcFile_clicked\0\0"
    "on_mExportDataset_clicked\0"
    "on_mChooseFiles_clicked\0on_mDeleteFiles_clicked\0"
    "on_mAddFiles_clicked\0GetDataSetName\0"
    "QTableWidgetItem*\0pItem\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IFCTools[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x0a,
       3,    0,   45,    2, 0x0a,
       4,    0,   46,    2, 0x0a,
       5,    0,   47,    2, 0x0a,
       6,    0,   48,    2, 0x0a,
       7,    1,   49,    2, 0x0a,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,

       0        // eod
};

void IFCTools::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        IFCTools *_t = static_cast<IFCTools *>(_o);
        switch (_id) {
        case 0: _t->on_mDataSrcFile_clicked(); break;
        case 1: _t->on_mExportDataset_clicked(); break;
        case 2: _t->on_mChooseFiles_clicked(); break;
        case 3: _t->on_mDeleteFiles_clicked(); break;
        case 4: _t->on_mAddFiles_clicked(); break;
        case 5: _t->GetDataSetName((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject IFCTools::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_IFCTools.data,
      qt_meta_data_IFCTools,  qt_static_metacall, 0, 0}
};


const QMetaObject *IFCTools::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IFCTools::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_IFCTools.stringdata))
        return static_cast<void*>(const_cast< IFCTools*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int IFCTools::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
