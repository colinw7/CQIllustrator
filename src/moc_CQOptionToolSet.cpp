/****************************************************************************
** Meta object code from reading C++ file 'CQOptionToolSet.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "CQOptionToolSet.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CQOptionToolSet.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CQOptionTool_t {
    QByteArrayData data[6];
    char stringdata[57];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQOptionTool_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQOptionTool_t qt_meta_stringdata_CQOptionTool = {
    {
QT_MOC_LITERAL(0, 0, 12),
QT_MOC_LITERAL(1, 13, 9),
QT_MOC_LITERAL(2, 23, 0),
QT_MOC_LITERAL(3, 24, 8),
QT_MOC_LITERAL(4, 33, 11),
QT_MOC_LITERAL(5, 45, 10)
    },
    "CQOptionTool\0poppingUp\0\0poppedUp\0"
    "poppingDown\0poppedDown\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQOptionTool[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06,
       3,    0,   35,    2, 0x06,
       4,    0,   36,    2, 0x06,
       5,    0,   37,    2, 0x06,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CQOptionTool::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CQOptionTool *_t = static_cast<CQOptionTool *>(_o);
        switch (_id) {
        case 0: _t->poppingUp(); break;
        case 1: _t->poppedUp(); break;
        case 2: _t->poppingDown(); break;
        case 3: _t->poppedDown(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CQOptionTool::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQOptionTool::poppingUp)) {
                *result = 0;
            }
        }
        {
            typedef void (CQOptionTool::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQOptionTool::poppedUp)) {
                *result = 1;
            }
        }
        {
            typedef void (CQOptionTool::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQOptionTool::poppingDown)) {
                *result = 2;
            }
        }
        {
            typedef void (CQOptionTool::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CQOptionTool::poppedDown)) {
                *result = 3;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject CQOptionTool::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CQOptionTool.data,
      qt_meta_data_CQOptionTool,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQOptionTool::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQOptionTool::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQOptionTool.stringdata))
        return static_cast<void*>(const_cast< CQOptionTool*>(this));
    return QWidget::qt_metacast(_clname);
}

int CQOptionTool::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void CQOptionTool::poppingUp()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void CQOptionTool::poppedUp()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void CQOptionTool::poppingDown()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void CQOptionTool::poppedDown()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
struct qt_meta_stringdata_CQOptionToolDialogArea_t {
    QByteArrayData data[3];
    char stringdata[36];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQOptionToolDialogArea_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQOptionToolDialogArea_t qt_meta_stringdata_CQOptionToolDialogArea = {
    {
QT_MOC_LITERAL(0, 0, 22),
QT_MOC_LITERAL(1, 23, 10),
QT_MOC_LITERAL(2, 34, 0)
    },
    "CQOptionToolDialogArea\0closedSlot\0\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQOptionToolDialogArea[] = {

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

void CQOptionToolDialogArea::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CQOptionToolDialogArea *_t = static_cast<CQOptionToolDialogArea *>(_o);
        switch (_id) {
        case 0: _t->closedSlot(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject CQOptionToolDialogArea::staticMetaObject = {
    { &CQWinWidget::staticMetaObject, qt_meta_stringdata_CQOptionToolDialogArea.data,
      qt_meta_data_CQOptionToolDialogArea,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQOptionToolDialogArea::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQOptionToolDialogArea::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQOptionToolDialogArea.stringdata))
        return static_cast<void*>(const_cast< CQOptionToolDialogArea*>(this));
    return CQWinWidget::qt_metacast(_clname);
}

int CQOptionToolDialogArea::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CQWinWidget::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_CQOptionToolMenu_t {
    QByteArrayData data[4];
    char stringdata[40];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQOptionToolMenu_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQOptionToolMenu_t qt_meta_stringdata_CQOptionToolMenu = {
    {
QT_MOC_LITERAL(0, 0, 16),
QT_MOC_LITERAL(1, 17, 12),
QT_MOC_LITERAL(2, 30, 0),
QT_MOC_LITERAL(3, 31, 7)
    },
    "CQOptionToolMenu\0updatePixmap\0\0checked\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQOptionToolMenu[] = {

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
       1,    1,   19,    2, 0x08,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,

       0        // eod
};

void CQOptionToolMenu::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CQOptionToolMenu *_t = static_cast<CQOptionToolMenu *>(_o);
        switch (_id) {
        case 0: _t->updatePixmap((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject CQOptionToolMenu::staticMetaObject = {
    { &QToolButton::staticMetaObject, qt_meta_stringdata_CQOptionToolMenu.data,
      qt_meta_data_CQOptionToolMenu,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQOptionToolMenu::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQOptionToolMenu::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQOptionToolMenu.stringdata))
        return static_cast<void*>(const_cast< CQOptionToolMenu*>(this));
    return QToolButton::qt_metacast(_clname);
}

int CQOptionToolMenu::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QToolButton::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_CQOptionToolArea_t {
    QByteArrayData data[4];
    char stringdata[36];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQOptionToolArea_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQOptionToolArea_t qt_meta_stringdata_CQOptionToolArea = {
    {
QT_MOC_LITERAL(0, 0, 16),
QT_MOC_LITERAL(1, 17, 11),
QT_MOC_LITERAL(2, 29, 0),
QT_MOC_LITERAL(3, 30, 4)
    },
    "CQOptionToolArea\0menuPressed\0\0show\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQOptionToolArea[] = {

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
       1,    1,   19,    2, 0x08,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,

       0        // eod
};

void CQOptionToolArea::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CQOptionToolArea *_t = static_cast<CQOptionToolArea *>(_o);
        switch (_id) {
        case 0: _t->menuPressed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject CQOptionToolArea::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_CQOptionToolArea.data,
      qt_meta_data_CQOptionToolArea,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQOptionToolArea::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQOptionToolArea::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQOptionToolArea.stringdata))
        return static_cast<void*>(const_cast< CQOptionToolArea*>(this));
    return QFrame::qt_metacast(_clname);
}

int CQOptionToolArea::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_CQOptionToolSet_t {
    QByteArrayData data[3];
    char stringdata[26];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQOptionToolSet_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQOptionToolSet_t qt_meta_stringdata_CQOptionToolSet = {
    {
QT_MOC_LITERAL(0, 0, 15),
QT_MOC_LITERAL(1, 16, 7),
QT_MOC_LITERAL(2, 24, 0)
    },
    "CQOptionToolSet\0popdown\0\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQOptionToolSet[] = {

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
       1,    0,   19,    2, 0x0a,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void CQOptionToolSet::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CQOptionToolSet *_t = static_cast<CQOptionToolSet *>(_o);
        switch (_id) {
        case 0: _t->popdown(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject CQOptionToolSet::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_CQOptionToolSet.data,
      qt_meta_data_CQOptionToolSet,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQOptionToolSet::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQOptionToolSet::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQOptionToolSet.stringdata))
        return static_cast<void*>(const_cast< CQOptionToolSet*>(this));
    return QWidget::qt_metacast(_clname);
}

int CQOptionToolSet::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
