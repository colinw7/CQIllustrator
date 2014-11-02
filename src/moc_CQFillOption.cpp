/****************************************************************************
** Meta object code from reading C++ file 'CQFillOption.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "CQFillOption.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CQFillOption.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CQFillOptionTool_t {
    QByteArrayData data[7];
    char stringdata[93];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQFillOptionTool_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQFillOptionTool_t qt_meta_stringdata_CQFillOptionTool = {
    {
QT_MOC_LITERAL(0, 0, 16),
QT_MOC_LITERAL(1, 17, 12),
QT_MOC_LITERAL(2, 30, 0),
QT_MOC_LITERAL(3, 31, 22),
QT_MOC_LITERAL(4, 54, 4),
QT_MOC_LITERAL(5, 59, 11),
QT_MOC_LITERAL(6, 71, 20)
    },
    "CQFillOptionTool\0valueChanged\0\0"
    "CQIllustratorShapeFill\0fill\0clipChanged\0"
    "selectionChangedSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQFillOptionTool[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06,
       5,    1,   32,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       6,    0,   35,    2, 0x08,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Bool,    2,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void CQFillOptionTool::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CQFillOptionTool *_t = static_cast<CQFillOptionTool *>(_o);
        switch (_id) {
        case 0: _t->valueChanged((*reinterpret_cast< const CQIllustratorShapeFill(*)>(_a[1]))); break;
        case 1: _t->clipChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->selectionChangedSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CQFillOptionTool::*_t)(const CQIllustratorShapeFill & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQFillOptionTool::valueChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (CQFillOptionTool::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQFillOptionTool::clipChanged)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject CQFillOptionTool::staticMetaObject = {
    { &CQOptionTool::staticMetaObject, qt_meta_stringdata_CQFillOptionTool.data,
      qt_meta_data_CQFillOptionTool,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQFillOptionTool::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQFillOptionTool::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQFillOptionTool.stringdata))
        return static_cast<void*>(const_cast< CQFillOptionTool*>(this));
    return CQOptionTool::qt_metacast(_clname);
}

int CQFillOptionTool::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void CQFillOptionTool::valueChanged(const CQIllustratorShapeFill & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CQFillOptionTool::clipChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
struct qt_meta_stringdata_CQFillOptionDialog_t {
    QByteArrayData data[28];
    char stringdata[346];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQFillOptionDialog_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQFillOptionDialog_t qt_meta_stringdata_CQFillOptionDialog = {
    {
QT_MOC_LITERAL(0, 0, 18),
QT_MOC_LITERAL(1, 19, 12),
QT_MOC_LITERAL(2, 32, 0),
QT_MOC_LITERAL(3, 33, 22),
QT_MOC_LITERAL(4, 56, 4),
QT_MOC_LITERAL(5, 61, 11),
QT_MOC_LITERAL(6, 73, 8),
QT_MOC_LITERAL(7, 82, 9),
QT_MOC_LITERAL(8, 92, 9),
QT_MOC_LITERAL(9, 102, 9),
QT_MOC_LITERAL(10, 112, 9),
QT_MOC_LITERAL(11, 122, 5),
QT_MOC_LITERAL(12, 128, 11),
QT_MOC_LITERAL(13, 140, 5),
QT_MOC_LITERAL(14, 146, 12),
QT_MOC_LITERAL(15, 159, 8),
QT_MOC_LITERAL(16, 168, 11),
QT_MOC_LITERAL(17, 180, 16),
QT_MOC_LITERAL(18, 197, 12),
QT_MOC_LITERAL(19, 210, 15),
QT_MOC_LITERAL(20, 226, 11),
QT_MOC_LITERAL(21, 238, 16),
QT_MOC_LITERAL(22, 255, 12),
QT_MOC_LITERAL(23, 268, 15),
QT_MOC_LITERAL(24, 284, 12),
QT_MOC_LITERAL(25, 297, 14),
QT_MOC_LITERAL(26, 312, 14),
QT_MOC_LITERAL(27, 327, 17)
    },
    "CQFillOptionDialog\0valueChanged\0\0"
    "CQIllustratorShapeFill\0fill\0clipChanged\0"
    "flatSlot\0lgradSlot\0rgradSlot\0imageSlot\0"
    "colorSlot\0color\0opacitySlot\0value\0"
    "fillRuleSlot\0clipSlot\0updateLGrad\0"
    "updateLGradStops\0addLGradStop\0"
    "removeLGradStop\0updateRGrad\0"
    "updateRGradStops\0addRGradStop\0"
    "removeRGradStop\0setImageSlot\0"
    "imageScaleSlot\0setDefaultSlot\0"
    "setBackgroundSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQFillOptionDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      22,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  124,    2, 0x06,
       5,    1,  127,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       6,    0,  130,    2, 0x08,
       7,    0,  131,    2, 0x08,
       8,    0,  132,    2, 0x08,
       9,    0,  133,    2, 0x08,
      10,    1,  134,    2, 0x08,
      12,    1,  137,    2, 0x08,
      14,    1,  140,    2, 0x08,
      15,    0,  143,    2, 0x08,
      16,    0,  144,    2, 0x08,
      17,    0,  145,    2, 0x08,
      18,    0,  146,    2, 0x08,
      19,    0,  147,    2, 0x08,
      20,    0,  148,    2, 0x08,
      21,    0,  149,    2, 0x08,
      22,    0,  150,    2, 0x08,
      23,    0,  151,    2, 0x08,
      24,    0,  152,    2, 0x08,
      25,    1,  153,    2, 0x08,
      26,    0,  156,    2, 0x08,
      27,    0,  157,    2, 0x08,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Bool,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QColor,   11,
    QMetaType::Void, QMetaType::Double,   13,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CQFillOptionDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CQFillOptionDialog *_t = static_cast<CQFillOptionDialog *>(_o);
        switch (_id) {
        case 0: _t->valueChanged((*reinterpret_cast< const CQIllustratorShapeFill(*)>(_a[1]))); break;
        case 1: _t->clipChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->flatSlot(); break;
        case 3: _t->lgradSlot(); break;
        case 4: _t->rgradSlot(); break;
        case 5: _t->imageSlot(); break;
        case 6: _t->colorSlot((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 7: _t->opacitySlot((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 8: _t->fillRuleSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->clipSlot(); break;
        case 10: _t->updateLGrad(); break;
        case 11: _t->updateLGradStops(); break;
        case 12: _t->addLGradStop(); break;
        case 13: _t->removeLGradStop(); break;
        case 14: _t->updateRGrad(); break;
        case 15: _t->updateRGradStops(); break;
        case 16: _t->addRGradStop(); break;
        case 17: _t->removeRGradStop(); break;
        case 18: _t->setImageSlot(); break;
        case 19: _t->imageScaleSlot((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 20: _t->setDefaultSlot(); break;
        case 21: _t->setBackgroundSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CQFillOptionDialog::*_t)(const CQIllustratorShapeFill & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQFillOptionDialog::valueChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (CQFillOptionDialog::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQFillOptionDialog::clipChanged)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject CQFillOptionDialog::staticMetaObject = {
    { &CQOptionToolDialog::staticMetaObject, qt_meta_stringdata_CQFillOptionDialog.data,
      qt_meta_data_CQFillOptionDialog,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQFillOptionDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQFillOptionDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQFillOptionDialog.stringdata))
        return static_cast<void*>(const_cast< CQFillOptionDialog*>(this));
    return CQOptionToolDialog::qt_metacast(_clname);
}

int CQFillOptionDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CQOptionToolDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 22)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 22;
    }
    return _id;
}

// SIGNAL 0
void CQFillOptionDialog::valueChanged(const CQIllustratorShapeFill & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CQFillOptionDialog::clipChanged(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
