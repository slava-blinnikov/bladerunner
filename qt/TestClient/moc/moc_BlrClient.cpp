/****************************************************************************
** Meta object code from reading C++ file 'BlrClient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../BlrClient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BlrClient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_BlrClient_t {
    QByteArrayData data[14];
    char stringdata0[149];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BlrClient_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BlrClient_t qt_meta_stringdata_BlrClient = {
    {
QT_MOC_LITERAL(0, 0, 9), // "BlrClient"
QT_MOC_LITERAL(1, 10, 9), // "connected"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 12), // "disconnected"
QT_MOC_LITERAL(4, 34, 10), // "authorized"
QT_MOC_LITERAL(5, 45, 10), // "needAvatar"
QT_MOC_LITERAL(6, 56, 13), // "avatarChanged"
QT_MOC_LITERAL(7, 70, 8), // "filename"
QT_MOC_LITERAL(8, 79, 19), // "memberAvatarChanged"
QT_MOC_LITERAL(9, 99, 9), // "member_id"
QT_MOC_LITERAL(10, 109, 13), // "threadStarted"
QT_MOC_LITERAL(11, 123, 14), // "threadFinished"
QT_MOC_LITERAL(12, 138, 5), // "start"
QT_MOC_LITERAL(13, 144, 4) // "stop"

    },
    "BlrClient\0connected\0\0disconnected\0"
    "authorized\0needAvatar\0avatarChanged\0"
    "filename\0memberAvatarChanged\0member_id\0"
    "threadStarted\0threadFinished\0start\0"
    "stop"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BlrClient[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,
       3,    0,   65,    2, 0x06 /* Public */,
       4,    0,   66,    2, 0x06 /* Public */,
       5,    0,   67,    2, 0x06 /* Public */,
       6,    1,   68,    2, 0x06 /* Public */,
       8,    2,   71,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,   76,    2, 0x08 /* Private */,
      11,    0,   77,    2, 0x08 /* Private */,
      12,    0,   78,    2, 0x0a /* Public */,
      13,    0,   79,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    9,    7,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void BlrClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BlrClient *_t = static_cast<BlrClient *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->connected(); break;
        case 1: _t->disconnected(); break;
        case 2: _t->authorized(); break;
        case 3: _t->needAvatar(); break;
        case 4: _t->avatarChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->memberAvatarChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 6: _t->threadStarted(); break;
        case 7: _t->threadFinished(); break;
        case 8: _t->start(); break;
        case 9: _t->stop(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (BlrClient::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BlrClient::connected)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (BlrClient::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BlrClient::disconnected)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (BlrClient::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BlrClient::authorized)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (BlrClient::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BlrClient::needAvatar)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (BlrClient::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BlrClient::avatarChanged)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (BlrClient::*_t)(int , const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&BlrClient::memberAvatarChanged)) {
                *result = 5;
                return;
            }
        }
    }
}

const QMetaObject BlrClient::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_BlrClient.data,
      qt_meta_data_BlrClient,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *BlrClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BlrClient::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_BlrClient.stringdata0))
        return static_cast<void*>(const_cast< BlrClient*>(this));
    return QObject::qt_metacast(_clname);
}

int BlrClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
    return _id;
}

// SIGNAL 0
void BlrClient::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void BlrClient::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void BlrClient::authorized()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void BlrClient::needAvatar()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}

// SIGNAL 4
void BlrClient::avatarChanged(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void BlrClient::memberAvatarChanged(int _t1, const QString & _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_END_MOC_NAMESPACE
