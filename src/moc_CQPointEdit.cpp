/****************************************************************************
** Meta object code from reading C++ file 'CQPointEdit.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "CQPointEdit.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CQPointEdit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CQPointEdit_t {
    QByteArrayData data[6];
    char stringdata[73];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQPointEdit_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQPointEdit_t qt_meta_stringdata_CQPointEdit = {
    {
QT_MOC_LITERAL(0, 0, 11),
QT_MOC_LITERAL(1, 12, 12),
QT_MOC_LITERAL(2, 25, 0),
QT_MOC_LITERAL(3, 26, 13),
QT_MOC_LITERAL(4, 40, 16),
QT_MOC_LITERAL(5, 57, 14)
    },
    "CQPointEdit\0valueChanged\0\0valueAccepted\0"
    "editingFinishedI\0returnPressedI\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQPointEdit[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06,
       3,    0,   35,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       4,    0,   36,    2, 0x08,
       5,    0,   37,    2, 0x08,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CQPointEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CQPointEdit *_t = static_cast<CQPointEdit *>(_o);
        switch (_id) {
        case 0: _t->valueChanged(); break;
        case 1: _t->valueAccepted(); break;
        case 2: _t->editingFinishedI(); break;
        case 3: _t->returnPressedI(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CQPointEdit::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQPointEdit::valueChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (CQPointEdit::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQPointEdit::valueAccepted)) {
                *result = 1;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject CQPointEdit::staticMetaObject = {
    { &QLineEdit::staticMetaObject, qt_meta_stringdata_CQPointEdit.data,
      qt_meta_data_CQPointEdit,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQPointEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQPointEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQPointEdit.stringdata))
        return static_cast<void*>(const_cast< CQPointEdit*>(this));
    return QLineEdit::qt_metacast(_clname);
}

int CQPointEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLineEdit::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void CQPointEdit::valueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CQPointEdit::valueAccepted()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
