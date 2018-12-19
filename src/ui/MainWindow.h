/*
    SmartHHC - Electronic Drums HiHat Controller Manager
    Copyright (C) 2018 Andrea Zanellato

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef UIMAIN_H
#define UIMAIN_H

#include <QMainWindow>
#include <QSharedPointer>

namespace hhc {

class device;
#ifdef Q_OS_ANDROID
    class bluetooth;
#else
    class serial;
#endif
class settings;
}
class QStackedWidget;
class QVBoxLayout;
class ActionBar;
class TabLayout;
class CurveEditor;
class PageHome;
class PageConnection;
class PageTerminal;
class PageAbout;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow();

private:
    void closeEvent(QCloseEvent *);

    void loadAppSettings();
    void saveAppSettings();
    void loadProfileToUI();
    void setProfileFromUI();

    void setConnected(bool);

    void writeData(const QByteArray &);

    void scanDevices();
    void doToggleConnection();
    void doConnect();
    void doDisconnect();

    // Device Signal Slots

    void onConnected();
    void onCurveChanged(const QStringList &);
    void onCurvePointChanged(const QPoint &);
    void onDisconnected();
    void onError(uint8_t);
    void onFixed1ValueChanged(const uint8_t);
    void onFixed2ValueChanged(const uint8_t);
    void onFixed3ValueChanged(const uint8_t);
    void onIntervalValueChanged(const uint16_t);
    void onMinValueChanged(const uint16_t);
    void onMaxValueChanged(const uint16_t);
    void onPositionValueChanged(const uint16_t);
    void onReadLine(const QString &);
    void onReady();

    // Profile Settings Signal Slots

    void onOpenProfile();
    void onSaveProfile();
    void onSaveAsProfile();
    bool checkProfile(const QString &);

    // About Informations Signal Slots

    void onAbout();
    void onActionBarUp();

    // Device Settings Signal Slots

    void onSetFixed1();
    void onSetFixed2();
    void onSetFixed3();
    void onSetClosed();
    void onSetOpen();
    void onSetAsClosed();
    void onSetAsOpen();
    void onPedalPositionsRestore();
    void onPedalPositionsSet();
    void onPedalPositionsSave();
    void onSetInterval();

    void setCurrentIndex(int);
    void onCurveOk();
    void onCurveCancel();
    void onCurveRestore();
    void onCurveSave();

    void onSpinBoxValueChanged(int);

#ifdef Q_OS_ANDROID
    void onBluetoothDeviceDiscovered(const QStringList &);
    void onBluetoothDeviceScanFinished();
    void onBluetoothServiceDiscovered(const QStringList &);
    void onBluetoothServiceScanFinished();

    hhc::bluetooth *device;
#else
    hhc::serial *device;
#endif

    CurveEditor    *pageCurv;
    PageHome       *pageHome;
    PageConnection *pageConn;
    PageTerminal   *pageTerm;
    PageAbout      *pageInfo;

    QWidget        *wgtMain;
    QVBoxLayout    *layout;
    QStackedWidget *stkMain;
    TabLayout      *tloMain;
    ActionBar      *actBar;

    QAction *actConn;
    QAction *actScan;
    QAction *actLoad;
    QAction *actSave;
    QAction *actSvAs;
    QAction *actInfo;
    QAction *actQuit;

    bool hasDeviceSetValue; // Flag to avoid enabling buttons when
                            // device changed values, not user.
    bool isStarted;         // onFirstDeviceDataSent() flag

    typedef QSharedPointer<hhc::settings> settingsPtr;
    settingsPtr settings;
};
#endif // UIMAIN_H
