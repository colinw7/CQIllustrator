/****************************************************************************
** Meta object code from reading C++ file 'CQIllustratorOffsetPathMode.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "CQIllustratorOffsetPathMode.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CQIllustratorOffsetPathMode.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CQIllustratorOffsetPathToolbar_t {
    QByteArrayData data[3];
    char stringdata[45];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQIllustratorOffsetPathToolbar_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQIllustratorOffsetPathToolbar_t qt_meta_stringdata_CQIllustratorOffsetPathToolbar = {
    {
QT_MOC_LITERAL(0, 0, 30),
QT_MOC_LITERAL(1, 31, 11),
QT_MOC_LITERAL(2, 43, 0)
    },
    "CQIllustratorOffsetPathToolbar\0"
    "updateShape\0\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQIllustratorOffsetPathToolbar[] = {

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

void CQIllustratorOffsetPathToolbar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CQIllustratorOffsetPathToolbar *_t = static_cast<CQIllustratorOffsetPathToolbar *>(_o);
        switch (_id) {
        case 0: _t->updateShape(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject CQIllustratorOffsetPathToolbar::staticMetaObject = {
    { &CQIllustratorToolbar::staticMetaObject, qt_meta_stringdata_CQIllustratorOffsetPathToolbar.data,
      qt_meta_data_CQIllustratorOffsetPathToolbar,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQIllustratorOffsetPathToolbar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQIllustratorOffsetPathToolbar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQIllustratorOffsetPathToolbar.stringdata))
        return static_cast<void*>(const_cast< CQIllustratorOffsetPathToolbar*>(this));
    return CQIllustratorToolbar::qt_metacast(_clname);
}

int CQIllustratorOffsetPathToolbar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CQIllustratorToolbar::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_CQIllustratorOffsetPathMode_t {
    QByteArrayData data[1];
    char stringdata[29];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQIllustratorOffsetPathMode_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQIllustratorOffsetPathMode_t qt_meta_stringdata_CQIllustratorOffsetPathMode = {
    {
QT_MOC_LITERAL(0, 0, 27)
    },
    "CQIllustratorOffsetPathMode\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQIllustratorOffsetPathMode[] = {

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

void CQIllustratorOffsetPathMode::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject CQIllustratorOffsetPathMode::staticMetaObject = {
    { &CQIllustratorMode::staticMetaObject, qt_meta_stringdata_CQIllustratorOffsetPathMode.data,
      qt_meta_data_CQIllustratorOffsetPathMode,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQIllustratorOffsetPathMode::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQIllustratorOffsetPathMode::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQIllustratorOffsetPathMode.stringdata))
        return static_cast<void*>(const_cast< CQIllustratorOffsetPathMode*>(this));
    return CQIllustratorMode::qt_metacast(_clname);
}

int CQIllustratorOffsetPathMode::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CQIllustratorMode::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
