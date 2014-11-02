/****************************************************************************
** Meta object code from reading C++ file 'CQStrokeOption.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "CQStrokeOption.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CQStrokeOption.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CQStrokeOptionTool_t {
    QByteArrayData data[6];
    char stringdata[87];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQStrokeOptionTool_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQStrokeOptionTool_t qt_meta_stringdata_CQStrokeOptionTool = {
    {
QT_MOC_LITERAL(0, 0, 18),
QT_MOC_LITERAL(1, 19, 12),
QT_MOC_LITERAL(2, 32, 0),
QT_MOC_LITERAL(3, 33, 24),
QT_MOC_LITERAL(4, 58, 6),
QT_MOC_LITERAL(5, 65, 20)
    },
    "CQStrokeOptionTool\0valueChanged\0\0"
    "CQIllustratorShapeStroke\0stroke\0"
    "selectionChangedSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQStrokeOptionTool[] = {

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
       5,    0,   27,    2, 0x08,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void CQStrokeOptionTool::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CQStrokeOptionTool *_t = static_cast<CQStrokeOptionTool *>(_o);
        switch (_id) {
        case 0: _t->valueChanged((*reinterpret_cast< const CQIllustratorShapeStroke(*)>(_a[1]))); break;
        case 1: _t->selectionChangedSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CQStrokeOptionTool::*_t)(const CQIllustratorShapeStroke & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQStrokeOptionTool::valueChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject CQStrokeOptionTool::staticMetaObject = {
    { &CQOptionTool::staticMetaObject, qt_meta_stringdata_CQStrokeOptionTool.data,
      qt_meta_data_CQStrokeOptionTool,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQStrokeOptionTool::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQStrokeOptionTool::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQStrokeOptionTool.stringdata))
        return static_cast<void*>(const_cast< CQStrokeOptionTool*>(this));
    return CQOptionTool::qt_metacast(_clname);
}

int CQStrokeOptionTool::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void CQStrokeOptionTool::valueChanged(const CQIllustratorShapeStroke & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_CQStrokeOptionDialog_t {
    QByteArrayData data[20];
    char stringdata[208];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQStrokeOptionDialog_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQStrokeOptionDialog_t qt_meta_stringdata_CQStrokeOptionDialog = {
    {
QT_MOC_LITERAL(0, 0, 20),
QT_MOC_LITERAL(1, 21, 12),
QT_MOC_LITERAL(2, 34, 0),
QT_MOC_LITERAL(3, 35, 24),
QT_MOC_LITERAL(4, 60, 6),
QT_MOC_LITERAL(5, 67, 9),
QT_MOC_LITERAL(6, 77, 5),
QT_MOC_LITERAL(7, 83, 9),
QT_MOC_LITERAL(8, 93, 11),
QT_MOC_LITERAL(9, 105, 8),
QT_MOC_LITERAL(10, 114, 9),
QT_MOC_LITERAL(11, 124, 4),
QT_MOC_LITERAL(12, 129, 7),
QT_MOC_LITERAL(13, 137, 12),
QT_MOC_LITERAL(14, 150, 3),
QT_MOC_LITERAL(15, 154, 8),
QT_MOC_LITERAL(16, 163, 13),
QT_MOC_LITERAL(17, 177, 4),
QT_MOC_LITERAL(18, 182, 9),
QT_MOC_LITERAL(19, 192, 14)
    },
    "CQStrokeOptionDialog\0valueChanged\0\0"
    "CQIllustratorShapeStroke\0stroke\0"
    "colorSlot\0color\0widthSlot\0opacitySlot\0"
    "dashSlot\0CLineDash\0dash\0capSlot\0"
    "CLineCapType\0cap\0joinSlot\0CLineJoinType\0"
    "join\0mitreSlot\0setDefaultSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQStrokeOptionDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       5,    1,   62,    2, 0x08,
       7,    1,   65,    2, 0x08,
       8,    1,   68,    2, 0x08,
       9,    1,   71,    2, 0x08,
      12,    1,   74,    2, 0x08,
      15,    1,   77,    2, 0x08,
      18,    1,   80,    2, 0x08,
      19,    0,   83,    2, 0x08,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::QColor,    6,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void,

       0        // eod
};

void CQStrokeOptionDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CQStrokeOptionDialog *_t = static_cast<CQStrokeOptionDialog *>(_o);
        switch (_id) {
        case 0: _t->valueChanged((*reinterpret_cast< const CQIllustratorShapeStroke(*)>(_a[1]))); break;
        case 1: _t->colorSlot((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 2: _t->widthSlot((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->opacitySlot((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->dashSlot((*reinterpret_cast< const CLineDash(*)>(_a[1]))); break;
        case 5: _t->capSlot((*reinterpret_cast< CLineCapType(*)>(_a[1]))); break;
        case 6: _t->joinSlot((*reinterpret_cast< CLineJoinType(*)>(_a[1]))); break;
        case 7: _t->mitreSlot((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 8: _t->setDefaultSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CQStrokeOptionDialog::*_t)(const CQIllustratorShapeStroke & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQStrokeOptionDialog::valueChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject CQStrokeOptionDialog::staticMetaObject = {
    { &CQOptionToolDialog::staticMetaObject, qt_meta_stringdata_CQStrokeOptionDialog.data,
      qt_meta_data_CQStrokeOptionDialog,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQStrokeOptionDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQStrokeOptionDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQStrokeOptionDialog.stringdata))
        return static_cast<void*>(const_cast< CQStrokeOptionDialog*>(this));
    return CQOptionToolDialog::qt_metacast(_clname);
}

int CQStrokeOptionDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CQOptionToolDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void CQStrokeOptionDialog::valueChanged(const CQIllustratorShapeStroke & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
