/****************************************************************************
** Meta object code from reading C++ file 'CQIllustratorLayer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "CQIllustratorLayer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CQIllustratorLayer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CQIllustratorLayer_t {
    QByteArrayData data[5];
    char stringdata[69];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQIllustratorLayer_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQIllustratorLayer_t qt_meta_stringdata_CQIllustratorLayer = {
    {
QT_MOC_LITERAL(0, 0, 18),
QT_MOC_LITERAL(1, 19, 16),
QT_MOC_LITERAL(2, 36, 0),
QT_MOC_LITERAL(3, 37, 19),
QT_MOC_LITERAL(4, 57, 10)
    },
    "CQIllustratorLayer\0objectPostModify\0"
    "\0CQIllustratorShape*\0ChangeType\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQIllustratorLayer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   19,    2, 0x06,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 4,    2,    2,

       0        // eod
};

void CQIllustratorLayer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CQIllustratorLayer *_t = static_cast<CQIllustratorLayer *>(_o);
        switch (_id) {
        case 0: _t->objectPostModify((*reinterpret_cast< CQIllustratorShape*(*)>(_a[1])),(*reinterpret_cast< ChangeType(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CQIllustratorLayer::*_t)(CQIllustratorShape * , ChangeType );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQIllustratorLayer::objectPostModify)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject CQIllustratorLayer::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CQIllustratorLayer.data,
      qt_meta_data_CQIllustratorLayer,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQIllustratorLayer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQIllustratorLayer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQIllustratorLayer.stringdata))
        return static_cast<void*>(const_cast< CQIllustratorLayer*>(this));
    return QObject::qt_metacast(_clname);
}

int CQIllustratorLayer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void CQIllustratorLayer::objectPostModify(CQIllustratorShape * _t1, ChangeType _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_CQIllustratorLayerStack_t {
    QByteArrayData data[7];
    char stringdata[102];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQIllustratorLayerStack_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQIllustratorLayerStack_t qt_meta_stringdata_CQIllustratorLayerStack = {
    {
QT_MOC_LITERAL(0, 0, 23),
QT_MOC_LITERAL(1, 24, 16),
QT_MOC_LITERAL(2, 41, 0),
QT_MOC_LITERAL(3, 42, 19),
QT_MOC_LITERAL(4, 62, 10),
QT_MOC_LITERAL(5, 73, 12),
QT_MOC_LITERAL(6, 86, 14)
    },
    "CQIllustratorLayerStack\0objectPostModify\0"
    "\0CQIllustratorShape*\0ChangeType\0"
    "layerChanged\0layersModified\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQIllustratorLayerStack[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   29,    2, 0x06,
       5,    0,   34,    2, 0x06,
       6,    0,   35,    2, 0x06,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 4,    2,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CQIllustratorLayerStack::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CQIllustratorLayerStack *_t = static_cast<CQIllustratorLayerStack *>(_o);
        switch (_id) {
        case 0: _t->objectPostModify((*reinterpret_cast< CQIllustratorShape*(*)>(_a[1])),(*reinterpret_cast< ChangeType(*)>(_a[2]))); break;
        case 1: _t->layerChanged(); break;
        case 2: _t->layersModified(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CQIllustratorLayerStack::*_t)(CQIllustratorShape * , ChangeType );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQIllustratorLayerStack::objectPostModify)) {
                *result = 0;
            }
        }
        {
            typedef void (CQIllustratorLayerStack::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQIllustratorLayerStack::layerChanged)) {
                *result = 1;
            }
        }
        {
            typedef void (CQIllustratorLayerStack::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQIllustratorLayerStack::layersModified)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject CQIllustratorLayerStack::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CQIllustratorLayerStack.data,
      qt_meta_data_CQIllustratorLayerStack,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQIllustratorLayerStack::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQIllustratorLayerStack::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQIllustratorLayerStack.stringdata))
        return static_cast<void*>(const_cast< CQIllustratorLayerStack*>(this));
    return QObject::qt_metacast(_clname);
}

int CQIllustratorLayerStack::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void CQIllustratorLayerStack::objectPostModify(CQIllustratorShape * _t1, ChangeType _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CQIllustratorLayerStack::layerChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void CQIllustratorLayerStack::layersModified()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
