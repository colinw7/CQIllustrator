/****************************************************************************
** Meta object code from reading C++ file 'CQIllustratorCreatePathMode.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "CQIllustratorCreatePathMode.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CQIllustratorCreatePathMode.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CQIllustratorCreatePathToolbar_t {
    QByteArrayData data[15];
    char stringdata[215];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQIllustratorCreatePathToolbar_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQIllustratorCreatePathToolbar_t qt_meta_stringdata_CQIllustratorCreatePathToolbar = {
    {
QT_MOC_LITERAL(0, 0, 30),
QT_MOC_LITERAL(1, 31, 15),
QT_MOC_LITERAL(2, 47, 0),
QT_MOC_LITERAL(3, 48, 12),
QT_MOC_LITERAL(4, 61, 12),
QT_MOC_LITERAL(5, 74, 10),
QT_MOC_LITERAL(6, 85, 14),
QT_MOC_LITERAL(7, 100, 12),
QT_MOC_LITERAL(8, 113, 14),
QT_MOC_LITERAL(9, 128, 14),
QT_MOC_LITERAL(10, 143, 14),
QT_MOC_LITERAL(11, 158, 13),
QT_MOC_LITERAL(12, 172, 11),
QT_MOC_LITERAL(13, 184, 14),
QT_MOC_LITERAL(14, 199, 14)
    },
    "CQIllustratorCreatePathToolbar\0"
    "modeChangedSlot\0\0pathFreeSlot\0"
    "pathRectSlot\0path45Slot\0selChangedSlot\0"
    "lineModeSlot\0curve2ModeSlot\0curve3ModeSlot\0"
    "cornerNodeSlot\0curveNodeSlot\0addNodeSlot\0"
    "removeNodeSlot\0updateNodeSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQIllustratorCreatePathToolbar[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x08,
       3,    1,   80,    2, 0x08,
       4,    1,   83,    2, 0x08,
       5,    1,   86,    2, 0x08,
       6,    0,   89,    2, 0x08,
       7,    1,   90,    2, 0x08,
       8,    1,   93,    2, 0x08,
       9,    1,   96,    2, 0x08,
      10,    0,   99,    2, 0x08,
      11,    0,  100,    2, 0x08,
      12,    0,  101,    2, 0x08,
      13,    0,  102,    2, 0x08,
      14,    0,  103,    2, 0x08,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CQIllustratorCreatePathToolbar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CQIllustratorCreatePathToolbar *_t = static_cast<CQIllustratorCreatePathToolbar *>(_o);
        switch (_id) {
        case 0: _t->modeChangedSlot(); break;
        case 1: _t->pathFreeSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->pathRectSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->path45Slot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->selChangedSlot(); break;
        case 5: _t->lineModeSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->curve2ModeSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->curve3ModeSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->cornerNodeSlot(); break;
        case 9: _t->curveNodeSlot(); break;
        case 10: _t->addNodeSlot(); break;
        case 11: _t->removeNodeSlot(); break;
        case 12: _t->updateNodeSlot(); break;
        default: ;
        }
    }
}

const QMetaObject CQIllustratorCreatePathToolbar::staticMetaObject = {
    { &CQIllustratorToolbar::staticMetaObject, qt_meta_stringdata_CQIllustratorCreatePathToolbar.data,
      qt_meta_data_CQIllustratorCreatePathToolbar,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQIllustratorCreatePathToolbar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQIllustratorCreatePathToolbar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQIllustratorCreatePathToolbar.stringdata))
        return static_cast<void*>(const_cast< CQIllustratorCreatePathToolbar*>(this));
    return CQIllustratorToolbar::qt_metacast(_clname);
}

int CQIllustratorCreatePathToolbar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CQIllustratorToolbar::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
