/****************************************************************************
** Meta object code from reading C++ file 'CQWinWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "CQWinWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CQWinWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CQWinWidget_t {
    QByteArrayData data[25];
    char stringdata[290];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQWinWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQWinWidget_t qt_meta_stringdata_CQWinWidget = {
    {
QT_MOC_LITERAL(0, 0, 11),
QT_MOC_LITERAL(1, 12, 16),
QT_MOC_LITERAL(2, 29, 0),
QT_MOC_LITERAL(3, 30, 15),
QT_MOC_LITERAL(4, 46, 7),
QT_MOC_LITERAL(5, 54, 6),
QT_MOC_LITERAL(6, 61, 6),
QT_MOC_LITERAL(7, 68, 8),
QT_MOC_LITERAL(8, 77, 15),
QT_MOC_LITERAL(9, 93, 1),
QT_MOC_LITERAL(10, 95, 9),
QT_MOC_LITERAL(11, 105, 10),
QT_MOC_LITERAL(12, 116, 12),
QT_MOC_LITERAL(13, 129, 14),
QT_MOC_LITERAL(14, 144, 14),
QT_MOC_LITERAL(15, 159, 10),
QT_MOC_LITERAL(16, 170, 10),
QT_MOC_LITERAL(17, 181, 12),
QT_MOC_LITERAL(18, 194, 16),
QT_MOC_LITERAL(19, 211, 16),
QT_MOC_LITERAL(20, 228, 22),
QT_MOC_LITERAL(21, 251, 8),
QT_MOC_LITERAL(22, 260, 9),
QT_MOC_LITERAL(23, 270, 7),
QT_MOC_LITERAL(24, 278, 10)
    },
    "CQWinWidget\0geometryChanging\0\0"
    "geometryChanged\0closing\0closed\0expand\0"
    "collapse\0showContextMenu\0p\0closeSlot\0"
    "expandSlot\0collapseSlot\0decorationType\0"
    "DecorationType\0headerSide\0HeaderSide\0"
    "NoDecoration\0BorderDecoration\0"
    "HeaderDecoration\0HeaderBorderDecoration\0"
    "SideLeft\0SideRight\0SideTop\0SideBottom\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQWinWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       2,   76, // properties
       2,   82, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06,
       3,    0,   65,    2, 0x06,
       4,    0,   66,    2, 0x06,
       5,    0,   67,    2, 0x06,
       6,    0,   68,    2, 0x06,
       7,    0,   69,    2, 0x06,
       8,    1,   70,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
      10,    0,   73,    2, 0x08,
      11,    0,   74,    2, 0x08,
      12,    0,   75,    2, 0x08,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPoint,    9,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // properties: name, type, flags
      13, 0x80000000 | 14, 0x0009510b,
      15, 0x80000000 | 16, 0x0009510b,

 // enums: name, flags, count, data
      14, 0x0,    4,   90,
      16, 0x0,    4,   98,

 // enum data: key, value
      17, uint(CQWinWidget::NoDecoration),
      18, uint(CQWinWidget::BorderDecoration),
      19, uint(CQWinWidget::HeaderDecoration),
      20, uint(CQWinWidget::HeaderBorderDecoration),
      21, uint(CQWinWidget::SideLeft),
      22, uint(CQWinWidget::SideRight),
      23, uint(CQWinWidget::SideTop),
      24, uint(CQWinWidget::SideBottom),

       0        // eod
};

void CQWinWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CQWinWidget *_t = static_cast<CQWinWidget *>(_o);
        switch (_id) {
        case 0: _t->geometryChanging(); break;
        case 1: _t->geometryChanged(); break;
        case 2: _t->closing(); break;
        case 3: _t->closed(); break;
        case 4: _t->expand(); break;
        case 5: _t->collapse(); break;
        case 6: _t->showContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 7: _t->closeSlot(); break;
        case 8: _t->expandSlot(); break;
        case 9: _t->collapseSlot(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CQWinWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQWinWidget::geometryChanging)) {
                *result = 0;
            }
        }
        {
            typedef void (CQWinWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQWinWidget::geometryChanged)) {
                *result = 1;
            }
        }
        {
            typedef void (CQWinWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQWinWidget::closing)) {
                *result = 2;
            }
        }
        {
            typedef void (CQWinWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQWinWidget::closed)) {
                *result = 3;
            }
        }
        {
            typedef void (CQWinWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQWinWidget::expand)) {
                *result = 4;
            }
        }
        {
            typedef void (CQWinWidget::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQWinWidget::collapse)) {
                *result = 5;
            }
        }
        {
            typedef void (CQWinWidget::*_t)(const QPoint & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQWinWidget::showContextMenu)) {
                *result = 6;
            }
        }
    }
}

const QMetaObject CQWinWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CQWinWidget.data,
      qt_meta_data_CQWinWidget,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQWinWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQWinWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQWinWidget.stringdata))
        return static_cast<void*>(const_cast< CQWinWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int CQWinWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< DecorationType*>(_v) = decorationType(); break;
        case 1: *reinterpret_cast< HeaderSide*>(_v) = headerSide(); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setDecorationType(*reinterpret_cast< DecorationType*>(_v)); break;
        case 1: setHeaderSide(*reinterpret_cast< HeaderSide*>(_v)); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void CQWinWidget::geometryChanging()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CQWinWidget::geometryChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void CQWinWidget::closing()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void CQWinWidget::closed()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void CQWinWidget::expand()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void CQWinWidget::collapse()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void CQWinWidget::showContextMenu(const QPoint & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
struct qt_meta_stringdata_CQWinTextEdit_t {
    QByteArrayData data[5];
    char stringdata[62];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQWinTextEdit_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQWinTextEdit_t qt_meta_stringdata_CQWinTextEdit = {
    {
QT_MOC_LITERAL(0, 0, 13),
QT_MOC_LITERAL(1, 14, 12),
QT_MOC_LITERAL(2, 27, 0),
QT_MOC_LITERAL(3, 28, 16),
QT_MOC_LITERAL(4, 45, 15)
    },
    "CQWinTextEdit\0valueChanged\0\0"
    "geometryChanging\0geometryChanged\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQWinTextEdit[] = {

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
       1,    0,   29,    2, 0x06,
       3,    0,   30,    2, 0x06,
       4,    0,   31,    2, 0x06,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CQWinTextEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CQWinTextEdit *_t = static_cast<CQWinTextEdit *>(_o);
        switch (_id) {
        case 0: _t->valueChanged(); break;
        case 1: _t->geometryChanging(); break;
        case 2: _t->geometryChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CQWinTextEdit::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQWinTextEdit::valueChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (CQWinTextEdit::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQWinTextEdit::geometryChanging)) {
                *result = 1;
            }
        }
        {
            typedef void (CQWinTextEdit::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQWinTextEdit::geometryChanged)) {
                *result = 2;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject CQWinTextEdit::staticMetaObject = {
    { &QTextEdit::staticMetaObject, qt_meta_stringdata_CQWinTextEdit.data,
      qt_meta_data_CQWinTextEdit,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQWinTextEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQWinTextEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQWinTextEdit.stringdata))
        return static_cast<void*>(const_cast< CQWinTextEdit*>(this));
    if (!strcmp(_clname, "CQWinWidgetBase"))
        return static_cast< CQWinWidgetBase*>(const_cast< CQWinTextEdit*>(this));
    return QTextEdit::qt_metacast(_clname);
}

int CQWinTextEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTextEdit::qt_metacall(_c, _id, _a);
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
void CQWinTextEdit::valueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CQWinTextEdit::geometryChanging()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void CQWinTextEdit::geometryChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
struct qt_meta_stringdata_CQWinImage_t {
    QByteArrayData data[6];
    char stringdata[69];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQWinImage_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQWinImage_t qt_meta_stringdata_CQWinImage = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 12),
QT_MOC_LITERAL(2, 24, 0),
QT_MOC_LITERAL(3, 25, 16),
QT_MOC_LITERAL(4, 42, 15),
QT_MOC_LITERAL(5, 58, 9)
    },
    "CQWinImage\0valueChanged\0\0geometryChanging\0"
    "geometryChanged\0loadImage\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQWinImage[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06,
       3,    0,   35,    2, 0x06,
       4,    0,   36,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       5,    0,   37,    2, 0x0a,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void CQWinImage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CQWinImage *_t = static_cast<CQWinImage *>(_o);
        switch (_id) {
        case 0: _t->valueChanged(); break;
        case 1: _t->geometryChanging(); break;
        case 2: _t->geometryChanged(); break;
        case 3: _t->loadImage(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CQWinImage::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQWinImage::valueChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (CQWinImage::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQWinImage::geometryChanging)) {
                *result = 1;
            }
        }
        {
            typedef void (CQWinImage::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQWinImage::geometryChanged)) {
                *result = 2;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject CQWinImage::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_CQWinImage.data,
      qt_meta_data_CQWinImage,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQWinImage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQWinImage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQWinImage.stringdata))
        return static_cast<void*>(const_cast< CQWinImage*>(this));
    if (!strcmp(_clname, "CQWinWidgetBase"))
        return static_cast< CQWinWidgetBase*>(const_cast< CQWinImage*>(this));
    return QLabel::qt_metacast(_clname);
}

int CQWinImage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
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
void CQWinImage::valueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CQWinImage::geometryChanging()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void CQWinImage::geometryChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
struct qt_meta_stringdata_CQWinTable_t {
    QByteArrayData data[5];
    char stringdata[59];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQWinTable_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQWinTable_t qt_meta_stringdata_CQWinTable = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 12),
QT_MOC_LITERAL(2, 24, 0),
QT_MOC_LITERAL(3, 25, 16),
QT_MOC_LITERAL(4, 42, 15)
    },
    "CQWinTable\0valueChanged\0\0geometryChanging\0"
    "geometryChanged\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQWinTable[] = {

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
       1,    0,   29,    2, 0x06,
       3,    0,   30,    2, 0x06,
       4,    0,   31,    2, 0x06,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CQWinTable::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CQWinTable *_t = static_cast<CQWinTable *>(_o);
        switch (_id) {
        case 0: _t->valueChanged(); break;
        case 1: _t->geometryChanging(); break;
        case 2: _t->geometryChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CQWinTable::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQWinTable::valueChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (CQWinTable::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQWinTable::geometryChanging)) {
                *result = 1;
            }
        }
        {
            typedef void (CQWinTable::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQWinTable::geometryChanged)) {
                *result = 2;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject CQWinTable::staticMetaObject = {
    { &QTableWidget::staticMetaObject, qt_meta_stringdata_CQWinTable.data,
      qt_meta_data_CQWinTable,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQWinTable::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQWinTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQWinTable.stringdata))
        return static_cast<void*>(const_cast< CQWinTable*>(this));
    if (!strcmp(_clname, "CQWinWidgetBase"))
        return static_cast< CQWinWidgetBase*>(const_cast< CQWinTable*>(this));
    return QTableWidget::qt_metacast(_clname);
}

int CQWinTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTableWidget::qt_metacall(_c, _id, _a);
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
void CQWinTable::valueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CQWinTable::geometryChanging()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void CQWinTable::geometryChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
struct qt_meta_stringdata_CQWinTree_t {
    QByteArrayData data[5];
    char stringdata[58];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQWinTree_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQWinTree_t qt_meta_stringdata_CQWinTree = {
    {
QT_MOC_LITERAL(0, 0, 9),
QT_MOC_LITERAL(1, 10, 12),
QT_MOC_LITERAL(2, 23, 0),
QT_MOC_LITERAL(3, 24, 16),
QT_MOC_LITERAL(4, 41, 15)
    },
    "CQWinTree\0valueChanged\0\0geometryChanging\0"
    "geometryChanged\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQWinTree[] = {

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
       1,    0,   29,    2, 0x06,
       3,    0,   30,    2, 0x06,
       4,    0,   31,    2, 0x06,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CQWinTree::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CQWinTree *_t = static_cast<CQWinTree *>(_o);
        switch (_id) {
        case 0: _t->valueChanged(); break;
        case 1: _t->geometryChanging(); break;
        case 2: _t->geometryChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CQWinTree::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQWinTree::valueChanged)) {
                *result = 0;
            }
        }
        {
            typedef void (CQWinTree::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQWinTree::geometryChanging)) {
                *result = 1;
            }
        }
        {
            typedef void (CQWinTree::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQWinTree::geometryChanged)) {
                *result = 2;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject CQWinTree::staticMetaObject = {
    { &QTreeWidget::staticMetaObject, qt_meta_stringdata_CQWinTree.data,
      qt_meta_data_CQWinTree,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQWinTree::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQWinTree::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQWinTree.stringdata))
        return static_cast<void*>(const_cast< CQWinTree*>(this));
    if (!strcmp(_clname, "CQWinWidgetBase"))
        return static_cast< CQWinWidgetBase*>(const_cast< CQWinTree*>(this));
    return QTreeWidget::qt_metacast(_clname);
}

int CQWinTree::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeWidget::qt_metacall(_c, _id, _a);
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
void CQWinTree::valueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CQWinTree::geometryChanging()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void CQWinTree::geometryChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
