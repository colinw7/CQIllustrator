/****************************************************************************
** Meta object code from reading C++ file 'CQPropertiesOption.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "CQPropertiesOption.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CQPropertiesOption.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CQPropertiesOptionTool_t {
    QByteArrayData data[4];
    char stringdata[59];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQPropertiesOptionTool_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQPropertiesOptionTool_t qt_meta_stringdata_CQPropertiesOptionTool = {
    {
QT_MOC_LITERAL(0, 0, 22),
QT_MOC_LITERAL(1, 23, 12),
QT_MOC_LITERAL(2, 36, 0),
QT_MOC_LITERAL(3, 37, 20)
    },
    "CQPropertiesOptionTool\0valueChanged\0"
    "\0selectionChangedSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQPropertiesOptionTool[] = {

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
       1,    0,   24,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       3,    0,   25,    2, 0x08,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void CQPropertiesOptionTool::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CQPropertiesOptionTool *_t = static_cast<CQPropertiesOptionTool *>(_o);
        switch (_id) {
        case 0: _t->valueChanged(); break;
        case 1: _t->selectionChangedSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CQPropertiesOptionTool::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQPropertiesOptionTool::valueChanged)) {
                *result = 0;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject CQPropertiesOptionTool::staticMetaObject = {
    { &CQOptionTool::staticMetaObject, qt_meta_stringdata_CQPropertiesOptionTool.data,
      qt_meta_data_CQPropertiesOptionTool,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQPropertiesOptionTool::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQPropertiesOptionTool::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQPropertiesOptionTool.stringdata))
        return static_cast<void*>(const_cast< CQPropertiesOptionTool*>(this));
    return CQOptionTool::qt_metacast(_clname);
}

int CQPropertiesOptionTool::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CQOptionTool::qt_metacall(_c, _id, _a);
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
void CQPropertiesOptionTool::valueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
struct qt_meta_stringdata_CQPropertiesOptionDialog_t {
    QByteArrayData data[6];
    char stringdata[90];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQPropertiesOptionDialog_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQPropertiesOptionDialog_t qt_meta_stringdata_CQPropertiesOptionDialog = {
    {
QT_MOC_LITERAL(0, 0, 24),
QT_MOC_LITERAL(1, 25, 11),
QT_MOC_LITERAL(2, 37, 0),
QT_MOC_LITERAL(3, 38, 17),
QT_MOC_LITERAL(4, 56, 16),
QT_MOC_LITERAL(5, 73, 15)
    },
    "CQPropertiesOptionDialog\0updateShape\0"
    "\0replaceMatrixSlot\0appendMatrixSlot\0"
    "resetMatrixSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQPropertiesOptionDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x08,
       3,    0,   35,    2, 0x08,
       4,    0,   36,    2, 0x08,
       5,    0,   37,    2, 0x08,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CQPropertiesOptionDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CQPropertiesOptionDialog *_t = static_cast<CQPropertiesOptionDialog *>(_o);
        switch (_id) {
        case 0: _t->updateShape(); break;
        case 1: _t->replaceMatrixSlot(); break;
        case 2: _t->appendMatrixSlot(); break;
        case 3: _t->resetMatrixSlot(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject CQPropertiesOptionDialog::staticMetaObject = {
    { &CQOptionToolDialog::staticMetaObject, qt_meta_stringdata_CQPropertiesOptionDialog.data,
      qt_meta_data_CQPropertiesOptionDialog,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQPropertiesOptionDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQPropertiesOptionDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQPropertiesOptionDialog.stringdata))
        return static_cast<void*>(const_cast< CQPropertiesOptionDialog*>(this));
    return CQOptionToolDialog::qt_metacast(_clname);
}

int CQPropertiesOptionDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CQOptionToolDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
struct qt_meta_stringdata_CQControlPointsTable_t {
    QByteArrayData data[3];
    char stringdata[40];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQControlPointsTable_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQControlPointsTable_t qt_meta_stringdata_CQControlPointsTable = {
    {
QT_MOC_LITERAL(0, 0, 20),
QT_MOC_LITERAL(1, 21, 16),
QT_MOC_LITERAL(2, 38, 0)
    },
    "CQControlPointsTable\0updatePointsSlot\0"
    "\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQControlPointsTable[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x08,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void CQControlPointsTable::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CQControlPointsTable *_t = static_cast<CQControlPointsTable *>(_o);
        switch (_id) {
        case 0: _t->updatePointsSlot(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject CQControlPointsTable::staticMetaObject = {
    { &CQTableWidget::staticMetaObject, qt_meta_stringdata_CQControlPointsTable.data,
      qt_meta_data_CQControlPointsTable,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQControlPointsTable::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQControlPointsTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQControlPointsTable.stringdata))
        return static_cast<void*>(const_cast< CQControlPointsTable*>(this));
    return CQTableWidget::qt_metacast(_clname);
}

int CQControlPointsTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CQTableWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
struct qt_meta_stringdata_CQTransformPanel_t {
    QByteArrayData data[1];
    char stringdata[18];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQTransformPanel_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQTransformPanel_t qt_meta_stringdata_CQTransformPanel = {
    {
QT_MOC_LITERAL(0, 0, 16)
    },
    "CQTransformPanel\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQTransformPanel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void CQTransformPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject CQTransformPanel::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CQTransformPanel.data,
      qt_meta_data_CQTransformPanel,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQTransformPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQTransformPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQTransformPanel.stringdata))
        return static_cast<void*>(const_cast< CQTransformPanel*>(this));
    return QWidget::qt_metacast(_clname);
}

int CQTransformPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
