/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.2. It"
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
struct qt_meta_tag_ZN15ClickableSliderE_t {};
} // unnamed namespace

template <> constexpr inline auto ClickableSlider::qt_create_metaobjectdata<qt_meta_tag_ZN15ClickableSliderE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ClickableSlider"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ClickableSlider, qt_meta_tag_ZN15ClickableSliderE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ClickableSlider::staticMetaObject = { {
    QMetaObject::SuperData::link<QSlider::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15ClickableSliderE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15ClickableSliderE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN15ClickableSliderE_t>.metaTypes,
    nullptr
} };

void ClickableSlider::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ClickableSlider *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *ClickableSlider::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ClickableSlider::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15ClickableSliderE_t>.strings))
        return static_cast<void*>(this);
    return QSlider::qt_metacast(_clname);
}

int ClickableSlider::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSlider::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {
struct qt_meta_tag_ZN20ClickableVideoWidgetE_t {};
} // unnamed namespace

template <> constexpr inline auto ClickableVideoWidget::qt_create_metaobjectdata<qt_meta_tag_ZN20ClickableVideoWidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ClickableVideoWidget",
        "doubleClicked",
        ""
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'doubleClicked'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ClickableVideoWidget, qt_meta_tag_ZN20ClickableVideoWidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ClickableVideoWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QVideoWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20ClickableVideoWidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20ClickableVideoWidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN20ClickableVideoWidgetE_t>.metaTypes,
    nullptr
} };

void ClickableVideoWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ClickableVideoWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->doubleClicked(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ClickableVideoWidget::*)()>(_a, &ClickableVideoWidget::doubleClicked, 0))
            return;
    }
}

const QMetaObject *ClickableVideoWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ClickableVideoWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN20ClickableVideoWidgetE_t>.strings))
        return static_cast<void*>(this);
    return QVideoWidget::qt_metacast(_clname);
}

int ClickableVideoWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QVideoWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void ClickableVideoWidget::doubleClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
namespace {
struct qt_meta_tag_ZN10MainWindowE_t {};
} // unnamed namespace

template <> constexpr inline auto MainWindow::qt_create_metaobjectdata<qt_meta_tag_ZN10MainWindowE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MainWindow",
        "playVideo",
        "",
        "pauseVideo",
        "stopVideo",
        "setPosition",
        "position",
        "updatePosition",
        "updateDuration",
        "duration",
        "onPlaylistItemDoubleClicked",
        "QListWidgetItem*",
        "item",
        "changePlaybackRate",
        "togglePlaylist",
        "mediaStatusChanged",
        "QMediaPlayer::MediaStatus",
        "status",
        "playbackStateChanged",
        "QMediaPlayer::PlaybackState",
        "state",
        "setVolume",
        "volume",
        "playNextVideo",
        "toggleMute",
        "updateVolumeIcon",
        "openSettings",
        "onLongPressTimer",
        "seekVideo",
        "seconds",
        "startFastSeek",
        "forward",
        "onContinuousSeekTimer",
        "saveVideoPosition",
        "checkAndShowPositionDialog",
        "filePath",
        "onPositionDialogFinished",
        "getVideoHash",
        "onVideoWidgetDoubleClicked",
        "moveItemUp",
        "moveItemDown",
        "playNextVideoAuto",
        "onAudioOutputsChanged",
        "openFileOrFolder",
        "addVideoFile",
        "removeSelectedVideo"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'playVideo'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'pauseVideo'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'stopVideo'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'setPosition'
        QtMocHelpers::SlotData<void(int)>(5, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 6 },
        }}),
        // Slot 'updatePosition'
        QtMocHelpers::SlotData<void(qint64)>(7, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::LongLong, 6 },
        }}),
        // Slot 'updateDuration'
        QtMocHelpers::SlotData<void(qint64)>(8, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::LongLong, 9 },
        }}),
        // Slot 'onPlaylistItemDoubleClicked'
        QtMocHelpers::SlotData<void(QListWidgetItem *)>(10, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 11, 12 },
        }}),
        // Slot 'changePlaybackRate'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'togglePlaylist'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'mediaStatusChanged'
        QtMocHelpers::SlotData<void(QMediaPlayer::MediaStatus)>(15, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 16, 17 },
        }}),
        // Slot 'playbackStateChanged'
        QtMocHelpers::SlotData<void(QMediaPlayer::PlaybackState)>(18, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 19, 20 },
        }}),
        // Slot 'setVolume'
        QtMocHelpers::SlotData<void(int)>(21, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 22 },
        }}),
        // Slot 'playNextVideo'
        QtMocHelpers::SlotData<void()>(23, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'toggleMute'
        QtMocHelpers::SlotData<void()>(24, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'updateVolumeIcon'
        QtMocHelpers::SlotData<void()>(25, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'openSettings'
        QtMocHelpers::SlotData<void()>(26, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onLongPressTimer'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'seekVideo'
        QtMocHelpers::SlotData<void(int)>(28, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 29 },
        }}),
        // Slot 'startFastSeek'
        QtMocHelpers::SlotData<void(bool)>(30, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 31 },
        }}),
        // Slot 'onContinuousSeekTimer'
        QtMocHelpers::SlotData<void()>(32, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'saveVideoPosition'
        QtMocHelpers::SlotData<void()>(33, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'checkAndShowPositionDialog'
        QtMocHelpers::SlotData<void(const QString &)>(34, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 35 },
        }}),
        // Slot 'onPositionDialogFinished'
        QtMocHelpers::SlotData<void()>(36, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'getVideoHash'
        QtMocHelpers::SlotData<QString(const QString &)>(37, 2, QMC::AccessPrivate, QMetaType::QString, {{
            { QMetaType::QString, 35 },
        }}),
        // Slot 'onVideoWidgetDoubleClicked'
        QtMocHelpers::SlotData<void()>(38, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'moveItemUp'
        QtMocHelpers::SlotData<void()>(39, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'moveItemDown'
        QtMocHelpers::SlotData<void()>(40, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'playNextVideoAuto'
        QtMocHelpers::SlotData<void()>(41, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onAudioOutputsChanged'
        QtMocHelpers::SlotData<void()>(42, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'openFileOrFolder'
        QtMocHelpers::SlotData<void()>(43, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'addVideoFile'
        QtMocHelpers::SlotData<void(const QString &)>(44, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 35 },
        }}),
        // Slot 'removeSelectedVideo'
        QtMocHelpers::SlotData<void()>(45, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MainWindow, qt_meta_tag_ZN10MainWindowE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10MainWindowE_t>.metaTypes,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MainWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->playVideo(); break;
        case 1: _t->pauseVideo(); break;
        case 2: _t->stopVideo(); break;
        case 3: _t->setPosition((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->updatePosition((*reinterpret_cast< std::add_pointer_t<qint64>>(_a[1]))); break;
        case 5: _t->updateDuration((*reinterpret_cast< std::add_pointer_t<qint64>>(_a[1]))); break;
        case 6: _t->onPlaylistItemDoubleClicked((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 7: _t->changePlaybackRate(); break;
        case 8: _t->togglePlaylist(); break;
        case 9: _t->mediaStatusChanged((*reinterpret_cast< std::add_pointer_t<QMediaPlayer::MediaStatus>>(_a[1]))); break;
        case 10: _t->playbackStateChanged((*reinterpret_cast< std::add_pointer_t<QMediaPlayer::PlaybackState>>(_a[1]))); break;
        case 11: _t->setVolume((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 12: _t->playNextVideo(); break;
        case 13: _t->toggleMute(); break;
        case 14: _t->updateVolumeIcon(); break;
        case 15: _t->openSettings(); break;
        case 16: _t->onLongPressTimer(); break;
        case 17: _t->seekVideo((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 18: _t->startFastSeek((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 19: _t->onContinuousSeekTimer(); break;
        case 20: _t->saveVideoPosition(); break;
        case 21: _t->checkAndShowPositionDialog((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 22: _t->onPositionDialogFinished(); break;
        case 23: { QString _r = _t->getVideoHash((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 24: _t->onVideoWidgetDoubleClicked(); break;
        case 25: _t->moveItemUp(); break;
        case 26: _t->moveItemDown(); break;
        case 27: _t->playNextVideoAuto(); break;
        case 28: _t->onAudioOutputsChanged(); break;
        case 29: _t->openFileOrFolder(); break;
        case 30: _t->addVideoFile((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 31: _t->removeSelectedVideo(); break;
        default: ;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 32)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 32;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 32)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 32;
    }
    return _id;
}
QT_WARNING_POP
