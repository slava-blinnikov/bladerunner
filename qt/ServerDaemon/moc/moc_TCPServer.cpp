/****************************************************************************
** Meta object code from reading C++ file 'TCPServer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../TCPServer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TCPServer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_TCPServer_t {
    QByteArrayData data[17];
    char stringdata0[190];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TCPServer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TCPServer_t qt_meta_stringdata_TCPServer = {
    {
QT_MOC_LITERAL(0, 0, 9), // "TCPServer"
QT_MOC_LITERAL(1, 10, 13), // "onlineMembers"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 7), // "IntList"
QT_MOC_LITERAL(4, 33, 7), // "members"
QT_MOC_LITERAL(5, 41, 18), // "getChannelRolename"
QT_MOC_LITERAL(6, 60, 10), // "descriptor"
QT_MOC_LITERAL(7, 71, 4), // "crew"
QT_MOC_LITERAL(8, 76, 4), // "type"
QT_MOC_LITERAL(9, 81, 4), // "name"
QT_MOC_LITERAL(10, 86, 19), // "onChannelAuthorized"
QT_MOC_LITERAL(11, 106, 16), // "getMembersOnline"
QT_MOC_LITERAL(12, 123, 17), // "onChannelFinished"
QT_MOC_LITERAL(13, 141, 15), // "scheduleTimeout"
QT_MOC_LITERAL(14, 157, 15), // "scheduleMembers"
QT_MOC_LITERAL(15, 173, 7), // "started"
QT_MOC_LITERAL(16, 181, 8) // "finished"

    },
    "TCPServer\0onlineMembers\0\0IntList\0"
    "members\0getChannelRolename\0descriptor\0"
    "crew\0type\0name\0onChannelAuthorized\0"
    "getMembersOnline\0onChannelFinished\0"
    "scheduleTimeout\0scheduleMembers\0started\0"
    "finished"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TCPServer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    4,   52,    2, 0x08 /* Private */,
      10,    1,   61,    2, 0x08 /* Private */,
      11,    1,   64,    2, 0x08 /* Private */,
      12,    1,   67,    2, 0x08 /* Private */,
      13,    0,   70,    2, 0x0a /* Public */,
      14,    2,   71,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString, QMetaType::Int, QMetaType::QString,    6,    7,    8,    9,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3,   15,   16,

       0        // eod
};

void TCPServer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TCPServer *_t = static_cast<TCPServer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onlineMembers((*reinterpret_cast< const IntList(*)>(_a[1]))); break;
        case 1: _t->getChannelRolename((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 2: _t->onChannelAuthorized((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->getMembersOnline((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->onChannelFinished((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->scheduleTimeout(); break;
        case 6: _t->scheduleMembers((*reinterpret_cast< const IntList(*)>(_a[1])),(*reinterpret_cast< const IntList(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (TCPServer::*_t)(const IntList & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&TCPServer::onlineMembers)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject TCPServer::staticMetaObject = {
    { &QTcpServer::staticMetaObject, qt_meta_stringdata_TCPServer.data,
      qt_meta_data_TCPServer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *TCPServer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TCPServer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_TCPServer.stringdata0))
        return static_cast<void*>(const_cast< TCPServer*>(this));
    return QTcpServer::qt_metacast(_clname);
}

int TCPServer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpServer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void TCPServer::onlineMembers(const IntList & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
