/****************************************************************************
** Meta object code from reading C++ file 'CQIllustratorCreateTextMode.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "CQIllustratorCreateTextMode.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CQIllustratorCreateTextMode.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CQIllustratorCreateTextToolbar_t {
    QByteArrayData data[13];
    char stringdata[161];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQIllustratorCreateTextToolbar_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQIllustratorCreateTextToolbar_t qt_meta_stringdata_CQIllustratorCreateTextToolbar = {
    {
QT_MOC_LITERAL(0, 0, 30),
QT_MOC_LITERAL(1, 31, 11),
QT_MOC_LITERAL(2, 43, 0),
QT_MOC_LITERAL(3, 44, 13),
QT_MOC_LITERAL(4, 58, 13),
QT_MOC_LITERAL(5, 72, 8),
QT_MOC_LITERAL(6, 81, 10),
QT_MOC_LITERAL(7, 92, 10),
QT_MOC_LITERAL(8, 103, 11),
QT_MOC_LITERAL(9, 115, 10),
QT_MOC_LITERAL(10, 126, 10),
QT_MOC_LITERAL(11, 137, 11),
QT_MOC_LITERAL(12, 149, 10)
    },
    "CQIllustratorCreateTextToolbar\0"
    "updateShape\0\0fontComboSlot\0sizeComboSlot\0"
    "boldSlot\0italicSlot\0lalignSlot\0"
    "hcalignSlot\0ralignSlot\0talignSlot\0"
    "vcalignSlot\0balignSlot\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQIllustratorCreateTextToolbar[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x08,
       3,    0,   70,    2, 0x08,
       4,    0,   71,    2, 0x08,
       5,    0,   72,    2, 0x08,
       6,    0,   73,    2, 0x08,
       7,    0,   74,    2, 0x08,
       8,    0,   75,    2, 0x08,
       9,    0,   76,    2, 0x08,
      10,    0,   77,    2, 0x08,
      11,    0,   78,    2, 0x08,
      12,    0,   79,    2, 0x08,

 // slots: parameters
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
    QMetaType::Void,

       0        // eod
};

void CQIllustratorCreateTextToolbar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CQIllustratorCreateTextToolbar *_t = static_cast<CQIllustratorCreateTextToolbar *>(_o);
        switch (_id) {
        case 0: _t->updateShape(); break;
        case 1: _t->fontComboSlot(); break;
        case 2: _t->sizeComboSlot(); break;
        case 3: _t->boldSlot(); break;
        case 4: _t->italicSlot(); break;
        case 5: _t->lalignSlot(); break;
        case 6: _t->hcalignSlot(); break;
        case 7: _t->ralignSlot(); break;
        case 8: _t->talignSlot(); break;
        case 9: _t->vcalignSlot(); break;
        case 10: _t->balignSlot(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject CQIllustratorCreateTextToolbar::staticMetaObject = {
    { &CQIllustratorToolbar::staticMetaObject, qt_meta_stringdata_CQIllustratorCreateTextToolbar.data,
      qt_meta_data_CQIllustratorCreateTextToolbar,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQIllustratorCreateTextToolbar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQIllustratorCreateTextToolbar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQIllustratorCreateTextToolbar.stringdata))
        return static_cast<void*>(const_cast< CQIllustratorCreateTextToolbar*>(this));
    return CQIllustratorToolbar::qt_metacast(_clname);
}

int CQIllustratorCreateTextToolbar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CQIllustratorToolbar::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}
struct qt_meta_stringdata_CQIllustratorCreateTextMode_t {
    QByteArrayData data[1];
    char stringdata[29];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQIllustratorCreateTextMode_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQIllustratorCreateTextMode_t qt_meta_stringdata_CQIllustratorCreateTextMode = {
    {
QT_MOC_LITERAL(0, 0, 27)
    },
    "CQIllustratorCreateTextMode\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQIllustratorCreateTextMode[] = {

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

void CQIllustratorCreateTextMode::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject CQIllustratorCreateTextMode::staticMetaObject = {
    { &CQIllustratorMode::staticMetaObject, qt_meta_stringdata_CQIllustratorCreateTextMode.data,
      qt_meta_data_CQIllustratorCreateTextMode,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQIllustratorCreateTextMode::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQIllustratorCreateTextMode::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQIllustratorCreateTextMode.stringdata))
        return static_cast<void*>(const_cast< CQIllustratorCreateTextMode*>(this));
    return CQIllustratorMode::qt_metacast(_clname);
}

int CQIllustratorCreateTextMode::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CQIllustratorMode::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
