/****************************************************************************
** Meta object code from reading C++ file 'CQIllustratorCreateStarMode.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "CQIllustratorCreateStarMode.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CQIllustratorCreateStarMode.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CQIllustratorCreateStarToolbar_t {
    QByteArrayData data[5];
    char stringdata[71];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQIllustratorCreateStarToolbar_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQIllustratorCreateStarToolbar_t qt_meta_stringdata_CQIllustratorCreateStarToolbar = {
    {
QT_MOC_LITERAL(0, 0, 30),
QT_MOC_LITERAL(1, 31, 12),
QT_MOC_LITERAL(2, 44, 0),
QT_MOC_LITERAL(3, 45, 12),
QT_MOC_LITERAL(4, 58, 11)
    },
    "CQIllustratorCreateStarToolbar\0"
    "starModeSlot\0\0polyModeSlot\0updateShape\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQIllustratorCreateStarToolbar[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x08,
       3,    1,   32,    2, 0x08,
       4,    0,   35,    2, 0x08,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,

       0        // eod
};

void CQIllustratorCreateStarToolbar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CQIllustratorCreateStarToolbar *_t = static_cast<CQIllustratorCreateStarToolbar *>(_o);
        switch (_id) {
        case 0: _t->starModeSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->polyModeSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->updateShape(); break;
        default: ;
        }
    }
}

const QMetaObject CQIllustratorCreateStarToolbar::staticMetaObject = {
    { &CQIllustratorToolbar::staticMetaObject, qt_meta_stringdata_CQIllustratorCreateStarToolbar.data,
      qt_meta_data_CQIllustratorCreateStarToolbar,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQIllustratorCreateStarToolbar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQIllustratorCreateStarToolbar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQIllustratorCreateStarToolbar.stringdata))
        return static_cast<void*>(const_cast< CQIllustratorCreateStarToolbar*>(this));
    return CQIllustratorToolbar::qt_metacast(_clname);
}

int CQIllustratorCreateStarToolbar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CQIllustratorToolbar::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_CQIllustratorCreateStarMode_t {
    QByteArrayData data[1];
    char stringdata[29];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQIllustratorCreateStarMode_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQIllustratorCreateStarMode_t qt_meta_stringdata_CQIllustratorCreateStarMode = {
    {
QT_MOC_LITERAL(0, 0, 27)
    },
    "CQIllustratorCreateStarMode\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQIllustratorCreateStarMode[] = {

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

void CQIllustratorCreateStarMode::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject CQIllustratorCreateStarMode::staticMetaObject = {
    { &CQIllustratorMode::staticMetaObject, qt_meta_stringdata_CQIllustratorCreateStarMode.data,
      qt_meta_data_CQIllustratorCreateStarMode,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQIllustratorCreateStarMode::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQIllustratorCreateStarMode::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQIllustratorCreateStarMode.stringdata))
        return static_cast<void*>(const_cast< CQIllustratorCreateStarMode*>(this));
    return CQIllustratorMode::qt_metacast(_clname);
}

int CQIllustratorCreateStarMode::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CQIllustratorMode::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
