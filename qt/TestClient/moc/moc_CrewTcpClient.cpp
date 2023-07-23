/****************************************************************************
** Meta object code from reading C++ file 'CrewTcpClient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../CrewTcpClient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CrewTcpClient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CrewTcpClient_t {
    QByteArrayData data[31];
    char stringdata0[418];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CrewTcpClient_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CrewTcpClient_t qt_meta_stringdata_CrewTcpClient = {
    {
QT_MOC_LITERAL(0, 0, 13), // "CrewTcpClient"
QT_MOC_LITERAL(1, 14, 10), // "memberList"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 10), // "QList<int>"
QT_MOC_LITERAL(4, 37, 7), // "members"
QT_MOC_LITERAL(5, 45, 11), // "memberTitle"
QT_MOC_LITERAL(6, 57, 9), // "member_id"
QT_MOC_LITERAL(7, 67, 7), // "usrname"
QT_MOC_LITERAL(8, 75, 4), // "note"
QT_MOC_LITERAL(9, 80, 7), // "avatime"
QT_MOC_LITERAL(10, 88, 16), // "tracking_allowed"
QT_MOC_LITERAL(11, 105, 15), // "tracing_allowed"
QT_MOC_LITERAL(12, 121, 10), // "trctimeout"
QT_MOC_LITERAL(13, 132, 13), // "membersOnline"
QT_MOC_LITERAL(14, 146, 15), // "memberConnected"
QT_MOC_LITERAL(15, 162, 18), // "memberDisconnected"
QT_MOC_LITERAL(16, 181, 17), // "memberBeaconState"
QT_MOC_LITERAL(17, 199, 7), // "enabled"
QT_MOC_LITERAL(18, 207, 21), // "memberLocationChanged"
QT_MOC_LITERAL(19, 229, 4), // "time"
QT_MOC_LITERAL(20, 234, 8), // "latitude"
QT_MOC_LITERAL(21, 243, 9), // "longitude"
QT_MOC_LITERAL(22, 253, 5), // "speed"
QT_MOC_LITERAL(23, 259, 3), // "gps"
QT_MOC_LITERAL(24, 263, 24), // "memberTrackingPermission"
QT_MOC_LITERAL(25, 288, 25), // "memberRecordingPermission"
QT_MOC_LITERAL(26, 314, 19), // "requestMemberAvatar"
QT_MOC_LITERAL(27, 334, 13), // "requestMember"
QT_MOC_LITERAL(28, 348, 20), // "requestMembersOnline"
QT_MOC_LITERAL(29, 369, 24), // "requestStartMemberBeacon"
QT_MOC_LITERAL(30, 394, 23) // "requestStopMemberBeacon"

    },
    "CrewTcpClient\0memberList\0\0QList<int>\0"
    "members\0memberTitle\0member_id\0usrname\0"
    "note\0avatime\0tracking_allowed\0"
    "tracing_allowed\0trctimeout\0membersOnline\0"
    "memberConnected\0memberDisconnected\0"
    "memberBeaconState\0enabled\0"
    "memberLocationChanged\0time\0latitude\0"
    "longitude\0speed\0gps\0memberTrackingPermission\0"
    "memberRecordingPermission\0requestMemberAvatar\0"
    "requestMember\0requestMembersOnline\0"
    "requestStartMemberBeacon\0"
    "requestStopMemberBeacon"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CrewTcpClient[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x06 /* Public */,
       5,    7,   87,    2, 0x06 /* Public */,
      13,    1,  102,    2, 0x06 /* Public */,
      14,    1,  105,    2, 0x06 /* Public */,
      15,    1,  108,    2, 0x06 /* Public */,
      16,    2,  111,    2, 0x06 /* Public */,
      18,    6,  116,    2, 0x06 /* Public */,
      24,    2,  129,    2, 0x06 /* Public */,
      25,    2,  134,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      26,    1,  139,    2, 0x0a /* Public */,
      27,    1,  142,    2, 0x0a /* Public */,
      28,    0,  145,    2, 0x0a /* Public */,
      29,    1,  146,    2, 0x0a /* Public */,
      30,    1,  149,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::QString, QMetaType::UInt, QMetaType::Bool, QMetaType::Bool, QMetaType::Int,    6,    7,    8,    9,   10,   11,   12,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    6,   17,
    QMetaType::Void, QMetaType::Int, QMetaType::UInt, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Bool,    6,   19,   20,   21,   22,   23,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    6,   17,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    6,   17,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,

       0        // eod
};

void CrewTcpClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CrewTcpClient *_t = static_cast<CrewTcpClient *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->memberList((*reinterpret_cast< const QList<int>(*)>(_a[1]))); break;
        case 1: _t->memberTitle((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< uint(*)>(_a[4])),(*reinterpret_cast< bool(*)>(_a[5])),(*reinterpret_cast< bool(*)>(_a[6])),(*reinterpret_cast< int(*)>(_a[7]))); break;
        case 2: _t->membersOnline((*reinterpret_cast< const QList<int>(*)>(_a[1]))); break;
        case 3: _t->memberConnected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->memberDisconnected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->memberBeaconState((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 6: _t->memberLocationChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4])),(*reinterpret_cast< double(*)>(_a[5])),(*reinterpret_cast< bool(*)>(_a[6]))); break;
        case 7: _t->memberTrackingPermission((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 8: _t->memberRecordingPermission((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 9: _t->requestMemberAvatar((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->requestMember((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->requestMembersOnline(); break;
        case 12: _t->requestStartMemberBeacon((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->requestStopMemberBeacon((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<int> >(); break;
            }
            break;
        case 2:
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
            typedef void (CrewTcpClient::*_t)(const QList<int> & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CrewTcpClient::memberList)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (CrewTcpClient::*_t)(int , const QString & , const QString & , uint , bool , bool , int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CrewTcpClient::memberTitle)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (CrewTcpClient::*_t)(const QList<int> & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CrewTcpClient::membersOnline)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (CrewTcpClient::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CrewTcpClient::memberConnected)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (CrewTcpClient::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CrewTcpClient::memberDisconnected)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (CrewTcpClient::*_t)(int , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CrewTcpClient::memberBeaconState)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (CrewTcpClient::*_t)(int , uint , double , double , double , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CrewTcpClient::memberLocationChanged)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (CrewTcpClient::*_t)(int , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CrewTcpClient::memberTrackingPermission)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (CrewTcpClient::*_t)(int , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CrewTcpClient::memberRecordingPermission)) {
                *result = 8;
                return;
            }
        }
    }
}

const QMetaObject CrewTcpClient::staticMetaObject = {
    { &TcpClient::staticMetaObject, qt_meta_stringdata_CrewTcpClient.data,
      qt_meta_data_CrewTcpClient,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CrewTcpClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CrewTcpClient::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CrewTcpClient.stringdata0))
        return static_cast<void*>(const_cast< CrewTcpClient*>(this));
    return TcpClient::qt_metacast(_clname);
}

int CrewTcpClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = TcpClient::qt_metacall(_c, _id, _a);
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
void CrewTcpClient::memberList(const QList<int> & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CrewTcpClient::memberTitle(int _t1, const QString & _t2, const QString & _t3, uint _t4, bool _t5, bool _t6, int _t7)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)), const_cast<void*>(reinterpret_cast<const void*>(&_t7)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CrewTcpClient::membersOnline(const QList<int> & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CrewTcpClient::memberConnected(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CrewTcpClient::memberDisconnected(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CrewTcpClient::memberBeaconState(int _t1, bool _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void CrewTcpClient::memberLocationChanged(int _t1, uint _t2, double _t3, double _t4, double _t5, bool _t6)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)), const_cast<void*>(reinterpret_cast<const void*>(&_t5)), const_cast<void*>(reinterpret_cast<const void*>(&_t6)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void CrewTcpClient::memberTrackingPermission(int _t1, bool _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void CrewTcpClient::memberRecordingPermission(int _t1, bool _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_END_MOC_NAMESPACE
