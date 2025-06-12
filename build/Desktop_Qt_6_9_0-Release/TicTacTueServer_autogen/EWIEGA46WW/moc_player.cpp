/****************************************************************************
** Meta object code from reading C++ file 'player.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../player.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'player.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN6PlayerE_t {};
} // unnamed namespace

template <> constexpr inline auto Player::qt_create_metaobjectdata<qt_meta_tag_ZN6PlayerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Player",
        "idChanged",
        "",
        "nameChanged",
        "wonTimeChanged",
        "lostTimeChanged",
        "gameIdChanged",
        "id",
        "name",
        "gameId",
        "wonTime",
        "lostTime"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'idChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'nameChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'wonTimeChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'lostTimeChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'gameIdChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'id'
        QtMocHelpers::PropertyData<QString>(7, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet | QMC::Final, 0),
        // property 'name'
        QtMocHelpers::PropertyData<QString>(8, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet | QMC::Final, 1),
        // property 'gameId'
        QtMocHelpers::PropertyData<QString>(9, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet | QMC::Final, 4),
        // property 'wonTime'
        QtMocHelpers::PropertyData<int>(10, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet | QMC::Final, 2),
        // property 'lostTime'
        QtMocHelpers::PropertyData<int>(11, QMetaType::Int, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet | QMC::Final, 3),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Player, qt_meta_tag_ZN6PlayerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Player::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6PlayerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6PlayerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN6PlayerE_t>.metaTypes,
    nullptr
} };

void Player::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Player *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->idChanged(); break;
        case 1: _t->nameChanged(); break;
        case 2: _t->wonTimeChanged(); break;
        case 3: _t->lostTimeChanged(); break;
        case 4: _t->gameIdChanged(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Player::*)()>(_a, &Player::idChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Player::*)()>(_a, &Player::nameChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (Player::*)()>(_a, &Player::wonTimeChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (Player::*)()>(_a, &Player::lostTimeChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (Player::*)()>(_a, &Player::gameIdChanged, 4))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->id(); break;
        case 1: *reinterpret_cast<QString*>(_v) = _t->name(); break;
        case 2: *reinterpret_cast<QString*>(_v) = _t->gameId(); break;
        case 3: *reinterpret_cast<int*>(_v) = _t->wonTime(); break;
        case 4: *reinterpret_cast<int*>(_v) = _t->lostTime(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setId(*reinterpret_cast<QString*>(_v)); break;
        case 1: _t->setName(*reinterpret_cast<QString*>(_v)); break;
        case 2: _t->setGameId(*reinterpret_cast<QString*>(_v)); break;
        case 3: _t->setWonTime(*reinterpret_cast<int*>(_v)); break;
        case 4: _t->setLostTime(*reinterpret_cast<int*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *Player::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Player::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6PlayerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Player::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void Player::idChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Player::nameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void Player::wonTimeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void Player::lostTimeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void Player::gameIdChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
