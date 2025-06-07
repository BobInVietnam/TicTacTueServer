/****************************************************************************
** Meta object code from reading C++ file 'countdowntimer.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../countdowntimer.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'countdowntimer.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN14CountdownTimerE_t {};
} // unnamed namespace

template <> constexpr inline auto CountdownTimer::qt_create_metaobjectdata<qt_meta_tag_ZN14CountdownTimerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "CountdownTimer",
        "initialTimeChanged",
        "",
        "currentTimeChanged",
        "isRunningChanged",
        "isTimesUpChanged",
        "timerExpired",
        "onTimeout",
        "initialTime",
        "currentTime",
        "currentTimeText",
        "isRunning",
        "isTimesUp"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'initialTimeChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'currentTimeChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'isRunningChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'isTimesUpChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'timerExpired'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onTimeout'
        QtMocHelpers::SlotData<void()>(7, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'initialTime'
        QtMocHelpers::PropertyData<qint64>(8, QMetaType::LongLong, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet | QMC::Final, 0),
        // property 'currentTime'
        QtMocHelpers::PropertyData<qint64>(9, QMetaType::LongLong, QMC::DefaultPropertyFlags | QMC::Final, 1),
        // property 'currentTimeText'
        QtMocHelpers::PropertyData<QString>(10, QMetaType::QString, QMC::DefaultPropertyFlags, 1),
        // property 'isRunning'
        QtMocHelpers::PropertyData<bool>(11, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Final, 2),
        // property 'isTimesUp'
        QtMocHelpers::PropertyData<bool>(12, QMetaType::Bool, QMC::DefaultPropertyFlags | QMC::Final, 3),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<CountdownTimer, qt_meta_tag_ZN14CountdownTimerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject CountdownTimer::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14CountdownTimerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14CountdownTimerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN14CountdownTimerE_t>.metaTypes,
    nullptr
} };

void CountdownTimer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<CountdownTimer *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->initialTimeChanged(); break;
        case 1: _t->currentTimeChanged(); break;
        case 2: _t->isRunningChanged(); break;
        case 3: _t->isTimesUpChanged(); break;
        case 4: _t->timerExpired(); break;
        case 5: _t->onTimeout(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (CountdownTimer::*)()>(_a, &CountdownTimer::initialTimeChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (CountdownTimer::*)()>(_a, &CountdownTimer::currentTimeChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (CountdownTimer::*)()>(_a, &CountdownTimer::isRunningChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (CountdownTimer::*)()>(_a, &CountdownTimer::isTimesUpChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (CountdownTimer::*)()>(_a, &CountdownTimer::timerExpired, 4))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<qint64*>(_v) = _t->initialTime(); break;
        case 1: *reinterpret_cast<qint64*>(_v) = _t->currentTime(); break;
        case 2: *reinterpret_cast<QString*>(_v) = _t->currentTimeText(); break;
        case 3: *reinterpret_cast<bool*>(_v) = _t->isRunning(); break;
        case 4: *reinterpret_cast<bool*>(_v) = _t->isTimesUp(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setInitialTime(*reinterpret_cast<qint64*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *CountdownTimer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CountdownTimer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14CountdownTimerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CountdownTimer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 6;
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
void CountdownTimer::initialTimeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void CountdownTimer::currentTimeChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void CountdownTimer::isRunningChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void CountdownTimer::isTimesUpChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void CountdownTimer::timerExpired()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
