/****************************************************************************
** Meta object code from reading C++ file 'CQLineDash.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "CQLineDash.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CQLineDash.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CQLineDash_t {
    QByteArrayData data[8];
    char stringdata[84];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQLineDash_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQLineDash_t qt_meta_stringdata_CQLineDash = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 12),
QT_MOC_LITERAL(2, 24, 0),
QT_MOC_LITERAL(3, 25, 9),
QT_MOC_LITERAL(4, 35, 4),
QT_MOC_LITERAL(5, 40, 15),
QT_MOC_LITERAL(6, 56, 17),
QT_MOC_LITERAL(7, 74, 8)
    },
    "CQLineDash\0valueChanged\0\0CLineDash\0"
    "dash\0dashChangedSlot\0menuItemActivated\0"
    "QAction*\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQLineDash[] = {

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
       5,    0,   32,    2, 0x08,
       6,    1,   33,    2, 0x08,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    2,

       0        // eod
};

void CQLineDash::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CQLineDash *_t = static_cast<CQLineDash *>(_o);
        switch (_id) {
        case 0: _t->valueChanged((*reinterpret_cast< const CLineDash(*)>(_a[1]))); break;
        case 1: _t->dashChangedSlot(); break;
        case 2: _t->menuItemActivated((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CQLineDash::*_t)(const CLineDash & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQLineDash::valueChanged)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject CQLineDash::staticMetaObject = {
    { &QLineEdit::staticMetaObject, qt_meta_stringdata_CQLineDash.data,
      qt_meta_data_CQLineDash,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQLineDash::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQLineDash::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQLineDash.stringdata))
        return static_cast<void*>(const_cast< CQLineDash*>(this));
    return QLineEdit::qt_metacast(_clname);
}

int CQLineDash::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLineEdit::qt_metacall(_c, _id, _a);
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
void CQLineDash::valueChanged(const CLineDash & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
