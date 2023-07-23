/****************************************************************************
** Meta object code from reading C++ file 'Channel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Channel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Channel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Channel_t {
    QByteArrayData data[28];
    char stringdata0[329];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Channel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Channel_t qt_meta_stringdata_Channel = {
    {
QT_MOC_LITERAL(0, 0, 7), // "Channel"
QT_MOC_LITERAL(1, 8, 22), // "requestChannelRolename"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 10), // "descriptor"
QT_MOC_LITERAL(4, 43, 4), // "crew"
QT_MOC_LITERAL(5, 48, 4), // "type"
QT_MOC_LITERAL(6, 53, 4), // "name"
QT_MOC_LITERAL(7, 58, 10), // "authorized"
QT_MOC_LITERAL(8, 69, 20), // "requestMembersOnline"
QT_MOC_LITERAL(9, 90, 8), // "finished"
QT_MOC_LITERAL(10, 99, 13), // "replyRolename"
QT_MOC_LITERAL(11, 113, 8), // "rolename"
QT_MOC_LITERAL(12, 122, 7), // "crew_id"
QT_MOC_LITERAL(13, 130, 2), // "id"
QT_MOC_LITERAL(14, 133, 18), // "replyMembersOnline"
QT_MOC_LITERAL(15, 152, 7), // "IntList"
QT_MOC_LITERAL(16, 160, 7), // "members"
QT_MOC_LITERAL(17, 168, 15), // "memberConnected"
QT_MOC_LITERAL(18, 184, 9), // "member_id"
QT_MOC_LITERAL(19, 194, 18), // "memberDisconnected"
QT_MOC_LITERAL(20, 213, 13), // "crewConnected"
QT_MOC_LITERAL(21, 227, 8), // "crew_dsc"
QT_MOC_LITERAL(22, 236, 12), // "watcher_name"
QT_MOC_LITERAL(23, 249, 16), // "crewDisconnected"
QT_MOC_LITERAL(24, 266, 13), // "scheduleStart"
QT_MOC_LITERAL(25, 280, 12), // "scheduleStop"
QT_MOC_LITERAL(26, 293, 20), // "workerThreadFinished"
QT_MOC_LITERAL(27, 314, 14) // "setWatcherName"

    },
    "Channel\0requestChannelRolename\0\0"
    "descriptor\0crew\0type\0name\0authorized\0"
    "requestMembersOnline\0finished\0"
    "replyRolename\0rolename\0crew_id\0id\0"
    "replyMembersOnline\0IntList\0members\0"
    "memberConnected\0member_id\0memberDisconnected\0"
    "crewConnected\0crew_dsc\0watcher_name\0"
    "crewDisconnected\0scheduleStart\0"
    "scheduleStop\0workerThreadFinished\0"
    "setWatcherName"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Channel[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      12,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   84,    2, 0x06 /* Public */,
       7,    1,   93,    2, 0x06 /* Public */,
       8,    1,   96,    2, 0x06 /* Public */,
       9,    1,   99,    2, 0x06 /* Public */,
      10,    4,  102,    2, 0x06 /* Public */,
      14,    1,  111,    2, 0x06 /* Public */,
      17,    1,  114,    2, 0x06 /* Public */,
      19,    1,  117,    2, 0x06 /* Public */,
      20,    2,  120,    2, 0x06 /* Public */,
      23,    2,  125,    2, 0x06 /* Public */,
      24,    0,  130,    2, 0x06 /* Public */,
      25,    0,  131,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      26,    0,  132,    2, 0x08 /* Private */,
      27,    1,  133,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::Int, QMetaType::QString,    3,    4,    5,    6,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::Int, QMetaType::Int,    5,   11,   12,   13,
    QMetaType::Void, 0x80000000 | 15,   16,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   21,   22,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,   21,   22,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   22,

       0        // eod
};

void Channel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Channel *_t = static_cast<Channel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->requestChannelRolename((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 1: _t->authorized((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->requestMembersOnline((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->finished((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->replyRolename((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 5: _t->replyMembersOnline((*reinterpret_cast< const IntList(*)>(_a[1]))); break;
        case 6: _t->memberConnected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->memberDisconnected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->crewConnected((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 9: _t->crewDisconnected((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 10: _t->scheduleStart(); break;
        case 11: _t->scheduleStop(); break;
        case 12: _t->workerThreadFinished(); break;
        case 13: _t->setWatcherName((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Channel::*_t)(int , const QString & , int , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Channel::requestChannelRolename)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Channel::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Channel::authorized)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (Channel::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Channel::requestMembersOnline)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (Channel::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Channel::finished)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (Channel::*_t)(int , const QString & , int , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Channel::replyRolename)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (Channel::*_t)(const IntList & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Channel::replyMembersOnline)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (Channel::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Channel::memberConnected)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (Channel::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Channel::memberDisconnected)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (Channel::*_t)(int , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Channel::crewConnected)) {
                *result = 8;
                return;
            }
        }
        {
            typedef void (Channel::*_t)(int , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Channel::crewDisconnected)) {
                *result = 9;
                return;
            }
        }
        {
            typedef void (Channel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Channel::scheduleStart)) {
                *result = 10;
                return;
            }
        }
        {
            typedef void (Channel::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Channel::scheduleStop)) {
                *result = 11;
                return;
            }
        }
    }
}

const QMetaObject Channel::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Channel.data,
      qt_meta_data_Channel,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Channel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Channel::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Channel.stringdata0))
        return static_cast<void*>(const_cast< Channel*>(this));
    return QObject::qt_metacast(_clname);
}

int Channel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void Channel::requestChannelRolename(int _t1, const QString & _t2, int _t3, const QString & _t4)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Channel::authorized(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Channel::requestMembersOnline(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Channel::finished(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void Channel::replyRolename(int _t1, const QString & _t2, int _t3, int _t4)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void Channel::replyMembersOnline(const IntList & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void Channel::memberConnected(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void Channel::memberDisconnected(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void Channel::crewConnected(int _t1, const QString & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void Channel::crewDisconnected(int _t1, const QString & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void Channel::scheduleStart()
{
    QMetaObject::activate(this, &staticMetaObject, 10, Q_NULLPTR);
}

// SIGNAL 11
void Channel::scheduleStop()
{
    QMetaObject::activate(this, &staticMetaObject, 11, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
