/****************************************************************************
** Meta object code from reading C++ file 'CrewClient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../CrewClient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CrewClient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CrewClient_t {
    QByteArrayData data[31];
    char stringdata0[406];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CrewClient_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CrewClient_t qt_meta_stringdata_CrewClient = {
    {
QT_MOC_LITERAL(0, 0, 10), // "CrewClient"
QT_MOC_LITERAL(1, 11, 19), // "requestMemberAvatar"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 9), // "member_id"
QT_MOC_LITERAL(4, 42, 10), // "memberList"
QT_MOC_LITERAL(5, 53, 10), // "QList<int>"
QT_MOC_LITERAL(6, 64, 7), // "members"
QT_MOC_LITERAL(7, 72, 18), // "requestMemberTitle"
QT_MOC_LITERAL(8, 91, 11), // "memberTitle"
QT_MOC_LITERAL(9, 103, 7), // "usrname"
QT_MOC_LITERAL(10, 111, 4), // "note"
QT_MOC_LITERAL(11, 116, 7), // "avatime"
QT_MOC_LITERAL(12, 124, 16), // "tracking_allowed"
QT_MOC_LITERAL(13, 141, 15), // "tracing_allowed"
QT_MOC_LITERAL(14, 157, 10), // "trctimeout"
QT_MOC_LITERAL(15, 168, 20), // "requestMembersOnline"
QT_MOC_LITERAL(16, 189, 13), // "membersOnline"
QT_MOC_LITERAL(17, 203, 15), // "memberConnected"
QT_MOC_LITERAL(18, 219, 18), // "memberDisconnected"
QT_MOC_LITERAL(19, 238, 17), // "startMemberBeacon"
QT_MOC_LITERAL(20, 256, 16), // "stopMemberBeacon"
QT_MOC_LITERAL(21, 273, 17), // "memberBeaconState"
QT_MOC_LITERAL(22, 291, 7), // "enabled"
QT_MOC_LITERAL(23, 299, 21), // "memberLocationChanged"
QT_MOC_LITERAL(24, 321, 4), // "time"
QT_MOC_LITERAL(25, 326, 8), // "latitude"
QT_MOC_LITERAL(26, 335, 9), // "longitude"
QT_MOC_LITERAL(27, 345, 5), // "speed"
QT_MOC_LITERAL(28, 351, 3), // "gps"
QT_MOC_LITERAL(29, 355, 24), // "memberTrackingPermission"
QT_MOC_LITERAL(30, 380, 25) // "memberRecordingPermission"

    },
    "CrewClient\0requestMemberAvatar\0\0"
    "member_id\0memberList\0QList<int>\0members\0"
    "requestMemberTitle\0memberTitle\0usrname\0"
    "note\0avatime\0tracking_allowed\0"
    "tracing_allowed\0trctimeout\0"
    "requestMembersOnline\0membersOnline\0"
    "memberConnected\0memberDisconnected\0"
    "startMemberBeacon\0stopMemberBeacon\0"
    "memberBeaconState\0enabled\0"
    "memberLocationChanged\0time\0latitude\0"
    "longitude\0speed\0gps\0memberTrackingPermission\0"
    "memberRecordingPermission"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CrewClient[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      14,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x06 /* Public */,
       4,    1,   87,    2, 0x06 /* Public */,
       7,    1,   90,    2, 0x06 /* Public */,
       8,    7,   93,    2, 0x06 /* Public */,
      15,    0,  108,    2, 0x06 /* Public */,
      16,    1,  109,    2, 0x06 /* Public */,
      17,    1,  112,    2, 0x06 /* Public */,
      18,    1,  115,    2, 0x06 /* Public */,
      19,    1,  118,    2, 0x06 /* Public */,
      20,    1,  121,    2, 0x06 /* Public */,
      21,    2,  124,    2, 0x06 /* Public */,
      23,    6,  129,    2, 0x06 /* Public */,
      29,    2,  142,    2, 0x06 /* Public */,
      30,    2,  147,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::QString, QMetaType::UInt, QMetaType::Bool, QMetaType::Bool, QMetaType::Int,    3,    9,   10,   11,   12,   13,   14,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    3,   22,
    QMetaType::Void, QMetaType::Int, QMetaType::UInt, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Bool,    3,   24,   25,   26,   27,   28,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    3,   22,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    3,   22,

       0        // eod
};

void CrewClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CrewClient *_t = static_cast<CrewClient *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->requestMemberAvatar((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->memberList((*reinterpret_cast< const QList<int>(*)>(_a[1]))); break;
        case 2: _t->requestMemberTitle((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->memberTitle((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< uint(*)>(_a[4])),(*reinterpret_cast< bool(*)>(_a[5])),(*reinterpret_cast< bool(*)>(_a[6])),(*reinterpret_cast< int(*)>(_a[7]))); break;
        case 4: _t->requestMembersOnline(); break;
        case 5: _t->membersOnline((*reinterpret_cast< const QList<int>(*)>(_a[1]))); break;
        case 6: _t->memberConnected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->memberDisconnected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->startMemberBeacon((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->stopMemberBeacon((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->memberBeaconState((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 11: _t->memberLocationChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4])),(*reinterpret_cast< double(*)>(_a[5])),(*reinterpret_cast< bool(*)>(_a[6]))); break;
        case 12: _t->memberTrackingPermission((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 13: _t->memberRecordingPermission((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<int> >(); break;
            }
            break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<int> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CrewClient::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CrewClient::requestMemberAvatar)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (CrewClient::*_t)(const QList<int> & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CrewClient::memberList)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (CrewClient::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CrewClient::requestMemberTitle)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (CrewClient::*_t)(int , const QString & , const QString & , uint , bool , bool , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CrewClient::memberTitle)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (CrewClient::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CrewClient::requestMembersOnline)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (CrewClient::*_t)(const QList<int> & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CrewClient::membersOnline)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (CrewClient::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CrewClient::memberConnected)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (CrewClient::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CrewClient::memberDisconnected)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (CrewClient::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CrewClient::startMemberBeacon)) {
                *result = 8;
                return;
            }
        }
        {
            typedef void (CrewClient::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CrewClient::stopMemberBeacon)) {
                *result = 9;
                return;
            }
        }
        {
            typedef void (CrewClient::*_t)(int , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CrewClient::memberBeaconState)) {
                *result = 10;
                return;
            }
        }
        {
            typedef void (CrewClient::*_t)(int , uint , double , double , double , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CrewClient::memberLocationChanged)) {
                *result = 11;
                return;
            }
        }
        {
            typedef void (CrewClient::*_t)(int , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CrewClient::memberTrackingPermission)) {
                *result = 12;
                return;
            }
        }
        {
            typedef void (CrewClient::*_t)(int , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CrewClient::memberRecordingPermission)) {
                *result = 13;
                return;
            }
        }
    }
}

const QMetaObject CrewClient::staticMetaObject = {
    { &BlrClient::staticMetaObject, qt_meta_stringdata_CrewClient.data,
      qt_meta_data_CrewClient,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CrewClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CrewClient::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CrewClient.stringdata0))
        return static_cast<void*>(const_cast< CrewClient*>(this));
    return BlrClient::qt_metacast(_clname);
}

int CrewClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BlrClient::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void CrewClient::requestMemberAvatar(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CrewClient::memberList(const QList<int> & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CrewClient::requestMemberTitle(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CrewClient::memberTitle(int _t1, const QString & _t2, const QString & _t3, uint _t4, bool _t5, bool _t6, int _t7)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)), const_cast<void*>(reinterpret_cast<const void*>(&_t7)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CrewClient::requestMembersOnline()
{
    QMetaObject::activate(this, &staticMetaObject, 4, Q_NULLPTR);
}

// SIGNAL 5
void CrewClient::membersOnline(const QList<int> & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CrewClient::memberConnected(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CrewClient::memberDisconnected(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void CrewClient::startMemberBeacon(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void CrewClient::stopMemberBeacon(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void CrewClient::memberBeaconState(int _t1, bool _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void CrewClient::memberLocationChanged(int _t1, uint _t2, double _t3, double _t4, double _t5, bool _t6)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void CrewClient::memberTrackingPermission(int _t1, bool _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void CrewClient::memberRecordingPermission(int _t1, bool _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}
QT_END_MOC_NAMESPACE
