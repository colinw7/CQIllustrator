/****************************************************************************
** Meta object code from reading C++ file 'CQObjectOption.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "CQObjectOption.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CQObjectOption.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CQObjectOptionTool_t {
    QByteArrayData data[6];
    char stringdata[73];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQObjectOptionTool_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQObjectOptionTool_t qt_meta_stringdata_CQObjectOptionTool = {
    {
QT_MOC_LITERAL(0, 0, 18),
QT_MOC_LITERAL(1, 19, 12),
QT_MOC_LITERAL(2, 32, 0),
QT_MOC_LITERAL(3, 33, 4),
QT_MOC_LITERAL(4, 38, 20),
QT_MOC_LITERAL(5, 59, 12)
    },
    "CQObjectOptionTool\0valueChanged\0\0name\0"
    "selectionChangedSlot\0populateSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQObjectOptionTool[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       4,    0,   32,    2, 0x08,
       5,    0,   33,    2, 0x08,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CQObjectOptionTool::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CQObjectOptionTool *_t = static_cast<CQObjectOptionTool *>(_o);
        switch (_id) {
        case 0: _t->valueChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->selectionChangedSlot(); break;
        case 2: _t->populateSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CQObjectOptionTool::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQObjectOptionTool::valueChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject CQObjectOptionTool::staticMetaObject = {
    { &CQOptionTool::staticMetaObject, qt_meta_stringdata_CQObjectOptionTool.data,
      qt_meta_data_CQObjectOptionTool,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQObjectOptionTool::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQObjectOptionTool::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQObjectOptionTool.stringdata))
        return static_cast<void*>(const_cast< CQObjectOptionTool*>(this));
    return CQOptionTool::qt_metacast(_clname);
}

int CQObjectOptionTool::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CQOptionTool::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void CQObjectOptionTool::valueChanged(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_CQObjectOptionDialog_t {
    QByteArrayData data[8];
    char stringdata[84];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQObjectOptionDialog_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQObjectOptionDialog_t qt_meta_stringdata_CQObjectOptionDialog = {
    {
QT_MOC_LITERAL(0, 0, 20),
QT_MOC_LITERAL(1, 21, 12),
QT_MOC_LITERAL(2, 34, 0),
QT_MOC_LITERAL(3, 35, 4),
QT_MOC_LITERAL(4, 40, 16),
QT_MOC_LITERAL(5, 57, 16),
QT_MOC_LITERAL(6, 74, 4),
QT_MOC_LITERAL(7, 79, 3)
    },
    "CQObjectOptionDialog\0valueChanged\0\0"
    "name\0itemSelectedSlot\0QTreeWidgetItem*\0"
    "item\0pos\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQObjectOptionDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       4,    2,   27,    2, 0x08,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 5, QMetaType::Int,    6,    7,

       0        // eod
};

void CQObjectOptionDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CQObjectOptionDialog *_t = static_cast<CQObjectOptionDialog *>(_o);
        switch (_id) {
        case 0: _t->valueChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->itemSelectedSlot((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CQObjectOptionDialog::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQObjectOptionDialog::valueChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject CQObjectOptionDialog::staticMetaObject = {
    { &CQOptionToolDialog::staticMetaObject, qt_meta_stringdata_CQObjectOptionDialog.data,
      qt_meta_data_CQObjectOptionDialog,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQObjectOptionDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQObjectOptionDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQObjectOptionDialog.stringdata))
        return static_cast<void*>(const_cast< CQObjectOptionDialog*>(this));
    return CQOptionToolDialog::qt_metacast(_clname);
}

int CQObjectOptionDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CQOptionToolDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void CQObjectOptionDialog::valueChanged(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
