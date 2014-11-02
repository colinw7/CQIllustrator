/****************************************************************************
** Meta object code from reading C++ file 'CQIllustratorSelectMode.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "CQIllustratorSelectMode.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CQIllustratorSelectMode.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CQIllustratorSelectToolbar_t {
    QByteArrayData data[6];
    char stringdata[69];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQIllustratorSelectToolbar_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQIllustratorSelectToolbar_t qt_meta_stringdata_CQIllustratorSelectToolbar = {
    {
QT_MOC_LITERAL(0, 0, 26),
QT_MOC_LITERAL(1, 27, 13),
QT_MOC_LITERAL(2, 41, 0),
QT_MOC_LITERAL(3, 42, 14),
QT_MOC_LITERAL(4, 57, 7),
QT_MOC_LITERAL(5, 65, 2)
    },
    "CQIllustratorSelectToolbar\0selectAllSlot\0"
    "\0selectNoneSlot\0setMode\0id\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQIllustratorSelectToolbar[] = {

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
       1,    0,   29,    2, 0x08,
       3,    0,   30,    2, 0x08,
       4,    1,   31,    2, 0x08,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,

       0        // eod
};

void CQIllustratorSelectToolbar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CQIllustratorSelectToolbar *_t = static_cast<CQIllustratorSelectToolbar *>(_o);
        switch (_id) {
        case 0: _t->selectAllSlot(); break;
        case 1: _t->selectNoneSlot(); break;
        case 2: _t->setMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject CQIllustratorSelectToolbar::staticMetaObject = {
    { &CQIllustratorToolbar::staticMetaObject, qt_meta_stringdata_CQIllustratorSelectToolbar.data,
      qt_meta_data_CQIllustratorSelectToolbar,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQIllustratorSelectToolbar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQIllustratorSelectToolbar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQIllustratorSelectToolbar.stringdata))
        return static_cast<void*>(const_cast< CQIllustratorSelectToolbar*>(this));
    return CQIllustratorToolbar::qt_metacast(_clname);
}

int CQIllustratorSelectToolbar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
struct qt_meta_stringdata_CQIllustratorSelectMode_t {
    QByteArrayData data[1];
    char stringdata[25];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQIllustratorSelectMode_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQIllustratorSelectMode_t qt_meta_stringdata_CQIllustratorSelectMode = {
    {
QT_MOC_LITERAL(0, 0, 23)
    },
    "CQIllustratorSelectMode\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQIllustratorSelectMode[] = {

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

void CQIllustratorSelectMode::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject CQIllustratorSelectMode::staticMetaObject = {
    { &CQIllustratorMode::staticMetaObject, qt_meta_stringdata_CQIllustratorSelectMode.data,
      qt_meta_data_CQIllustratorSelectMode,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQIllustratorSelectMode::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQIllustratorSelectMode::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQIllustratorSelectMode.stringdata))
        return static_cast<void*>(const_cast< CQIllustratorSelectMode*>(this));
    return CQIllustratorMode::qt_metacast(_clname);
}

int CQIllustratorSelectMode::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CQIllustratorMode::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
