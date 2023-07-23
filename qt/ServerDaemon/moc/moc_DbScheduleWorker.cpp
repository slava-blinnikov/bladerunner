/****************************************************************************
** Meta object code from reading C++ file 'DbScheduleWorker.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../DbScheduleWorker.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DbScheduleWorker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_DbScheduleWorker_t {
    QByteArrayData data[10];
    char stringdata0[95];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DbScheduleWorker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DbScheduleWorker_t qt_meta_stringdata_DbScheduleWorker = {
    {
QT_MOC_LITERAL(0, 0, 16), // "DbScheduleWorker"
QT_MOC_LITERAL(1, 17, 7), // "timeout"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 15), // "scheduleMembers"
QT_MOC_LITERAL(4, 42, 7), // "IntList"
QT_MOC_LITERAL(5, 50, 7), // "started"
QT_MOC_LITERAL(6, 58, 8), // "finished"
QT_MOC_LITERAL(7, 67, 5), // "start"
QT_MOC_LITERAL(8, 73, 13), // "onlineMembers"
QT_MOC_LITERAL(9, 87, 7) // "members"

    },
    "DbScheduleWorker\0timeout\0\0scheduleMembers\0"
    "IntList\0started\0finished\0start\0"
    "onlineMembers\0members"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DbScheduleWorker[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,
       3,    2,   40,    2, 0x06 /* Public */,
       6,    0,   45,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   46,    2, 0x0a /* Public */,
       8,    1,   47,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4, 0x80000000 | 4,    5,    6,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    9,

       0        // eod
};

void DbScheduleWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DbScheduleWorker *_t = static_cast<DbScheduleWorker *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->timeout(); break;
        case 1: _t->scheduleMembers((*reinterpret_cast< const IntList(*)>(_a[1])),(*reinterpret_cast< const IntList(*)>(_a[2]))); break;
        case 2: _t->finished(); break;
        case 3: _t->start(); break;
        case 4: _t->onlineMembers((*reinterpret_cast< const IntList(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (DbScheduleWorker::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DbScheduleWorker::timeout)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (DbScheduleWorker::*_t)(const IntList & , const IntList & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DbScheduleWorker::scheduleMembers)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (DbScheduleWorker::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DbScheduleWorker::finished)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject DbScheduleWorker::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DbScheduleWorker.data,
      qt_meta_data_DbScheduleWorker,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *DbScheduleWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DbScheduleWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_DbScheduleWorker.stringdata0))
        return static_cast<void*>(const_cast< DbScheduleWorker*>(this));
    return QObject::qt_metacast(_clname);
}

int DbScheduleWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void DbScheduleWorker::timeout()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void DbScheduleWorker::scheduleMembers(const IntList & _t1, const IntList & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DbScheduleWorker::finished()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
