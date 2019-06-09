/*
    SmartHHC - Electronic Drums HiHat Controller Manager
    Copyright (C) 2018 Andrea Zanellato

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include <QApplication>
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QStackedWidget>
#include <QVBoxLayout>

#include "MainWindow.h"
#include "CurveEditor.h"
#include "CurvePage.h"
#include "CurveToolBar.h"
#include "CurveWidget.h"
#include "PageHome.h"
#include "PageConnection.h"
#include "PageTerminal.h"
#include "PageAbout.h"

#include "actionbar.h"

#include "device.h"
#ifndef Q_OS_ANDROID
    #include "serial.h"
#else
    #include "bluetooth.h"
#endif

#include "settings.h"
#include "spinbox.h"
#include "tablayout.h"

MainWindow::MainWindow() : QMainWindow(),
#ifndef Q_OS_ANDROID
    device(new hhc::serial(this)),
#else
    device(new hhc::bluetooth(this)),
#endif
    pageCurv(new CurveEditor(this)),
    pageHome(new PageHome(this)),
    pageConn(new PageConnection(this)),
    pageTerm(new PageTerminal(this)),
    pageInfo(new PageAbout(this)),
    wgtMain(new QWidget(this)),
    layout (new QVBoxLayout(wgtMain)),
    stkMain(new QStackedWidget(wgtMain)),
    tloMain(new TabLayout(stkMain)),
    actBar (new ActionBar(this)),
    actConn(new QAction(this)),
    actScan(new QAction(this)),
    actLoad(new QAction(this)),
    actSave(new QAction(this)),
    actSvAs(new QAction(this)),
    actInfo(new QAction(this)),
    actQuit(new QAction(this)),
    hasDeviceSetValue(false),
    isStarted(false),
    settings(new hhc::settings)
{
//  actConn->setIcon(QIcon(":/icons/connect.png"));
    actScan->setIcon(QIcon(":/icons/scan.png"));
    actLoad->setIcon(QIcon(":/icons/open.png"));
    actSave->setIcon(QIcon(":/icons/save.png"));
    actSvAs->setIcon(QIcon(":/icons/save-as.png"));
    actInfo->setIcon(QIcon(":/icons/about.png"));
    actQuit->setIcon(QIcon(":/icons/cancel.png"));

//  actConn->setText(tr("Connect"));
    actScan->setText(tr("Scan"));
    actLoad->setText(tr("Open... (CTRL+O)"));
    actSave->setText(tr("Save (CTRL+S)"));
    actSvAs->setText(tr("Save As..."));
    actInfo->setText(tr("About (CTRL+I)"));
    actQuit->setText(tr("Quit (CTRL+Q)"));

    actLoad->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    actSave->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    actInfo->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_I));
    actQuit->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));

    actBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    actBar->setMinimumSize(QSize(0, 48));

//  TODO: This doesn't work on Android
//  actBar->addAction(actConn);
    actBar->addAction(actConn, Action::Normal);
    actBar->addAction(actScan, Action::Normal);
    actBar->addAction(actLoad, Action::Navigation);
    actBar->addAction(actSave, Action::Navigation);
    actBar->addAction(actSvAs, Action::Navigation);
    actBar->addAction(actQuit, Action::Navigation);
    actBar->addAction(actInfo, Action::OverflowOnly, Action::Other);
    actBar->setAppIcon(QIcon(":/icons/hi-hat.png"));

    stkMain->addWidget(tloMain);
    stkMain->addWidget(pageInfo);

    tloMain->addTab(pageHome, tr("Home"));
    tloMain->addTab(pageCurv, tr("Curve"));
    tloMain->addTab(pageConn, tr("Connection"));
    tloMain->addTab(pageTerm, tr("Terminal"));

    layout->setSpacing(0);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSizeConstraint(QLayout::SetNoConstraint);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(actBar);
    layout->addWidget(stkMain);

    wgtMain->setLayout(layout);

    connect(actConn, &QAction::triggered, this, &MainWindow::doToggleConnection);
    connect(actScan, &QAction::triggered, this, &MainWindow::scanDevices);
    connect(actLoad, &QAction::triggered, this, &MainWindow::onOpenProfile);
    connect(actSave, &QAction::triggered, this, &MainWindow::onSaveProfile);
    connect(actSvAs, &QAction::triggered, this, &MainWindow::onSaveAsProfile);
    connect(actInfo, &QAction::triggered, this, &MainWindow::onAbout);
    connect(actQuit, &QAction::triggered, QCoreApplication::instance(), &QCoreApplication::quit);

    connect(actBar, &ActionBar::up, this, &MainWindow::onActionBarUp);

    connect(pageHome->sbxOpen(),   &SpinBox::valueChanged, this, &MainWindow::onSpinBoxValueChanged);
    connect(pageHome->sbxClosed(), &SpinBox::valueChanged, this, &MainWindow::onSpinBoxValueChanged);
    connect(pageHome->sbxFixed1(), &SpinBox::valueChanged, this, &MainWindow::onSpinBoxValueChanged);
    connect(pageHome->sbxFixed2(), &SpinBox::valueChanged, this, &MainWindow::onSpinBoxValueChanged);
    connect(pageHome->sbxFixed3(), &SpinBox::valueChanged, this, &MainWindow::onSpinBoxValueChanged);

    connect(pageHome->tbnSetOpen(),   &QToolButton::clicked, this, &MainWindow::onSetOpen);
    connect(pageHome->tbnSetClosed(), &QToolButton::clicked, this, &MainWindow::onSetClosed);
    connect(pageHome->tbnSetFixed1(), &QToolButton::clicked, this, &MainWindow::onSetFixed1);
    connect(pageHome->tbnSetFixed2(), &QToolButton::clicked, this, &MainWindow::onSetFixed2);
    connect(pageHome->tbnSetFixed3(), &QToolButton::clicked, this, &MainWindow::onSetFixed3);

    connect(pageHome->tbnSetAsOpen(),   &QToolButton::clicked, this, &MainWindow::onSetAsOpen);
    connect(pageHome->tbnSetAsClosed(), &QToolButton::clicked, this, &MainWindow::onSetAsClosed);
    connect(pageHome->tbnSetAll(),      &QToolButton::clicked, this, &MainWindow::onPedalPositionsSet);
    connect(pageHome->tbnSaveAll(),     &QToolButton::clicked, this, &MainWindow::onPedalPositionsSave);
    connect(pageHome->tbnRestore(),     &QToolButton::clicked, this, &MainWindow::onPedalPositionsRestore);

    connect(pageCurv->toolBar()->buttonOk(), &QToolButton::clicked, this,
            &MainWindow::onCurveOk);

    connect(pageCurv->toolBar()->buttonCancel(), &QToolButton::clicked, this,
            &MainWindow::onCurveCancel);

    connect(pageCurv->toolBar()->buttonSave(), &QToolButton::clicked, this,
            &MainWindow::onCurveSave);

    connect(pageCurv->toolBar(), &CurveToolBar::sigSlotButtonMenuItemChanged, this,
            &MainWindow::setCurrentIndex);

    connect(pageConn, &PageConnection::sigSettingInterval, this,
            &MainWindow::onSetInterval);

    connect(pageConn, &PageConnection::sigIntervalChanged, [=](int)
    {
        bool enable = (device->interval() != pageConn->interval());
        pageConn->setIntervalChanged(enable);
    });
    connect(pageConn, &PageConnection::sigRestart, [=]()
    {
        device->sendCommand("RESTART");
    });
    connect(pageTerm, &PageTerminal::onGetData, this, &MainWindow::writeData);

    connect(device, &hhc::device::sigConnected, this,
            &MainWindow::onConnected);

    connect(device, &hhc::device::sigCurveChanged, this,
            &MainWindow::onCurveChanged);

    connect(device, &hhc::device::sigCurvePointChanged, this,
            &MainWindow::onCurvePointChanged);

    connect(device, &hhc::device::sigDisconnected, this,
            &MainWindow::onDisconnected);

    connect(device, &hhc::device::sigError, this,
            &MainWindow::onError);

    connect(device, &hhc::device::sigFixed1ValueChanged, this,
            &MainWindow::onFixed1ValueChanged);

    connect(device, &hhc::device::sigFixed2ValueChanged, this,
            &MainWindow::onFixed2ValueChanged);

    connect(device, &hhc::device::sigFixed3ValueChanged, this,
            &MainWindow::onFixed3ValueChanged);

    connect(device, &hhc::device::sigIntervalValueChanged, this,
            &MainWindow::onIntervalValueChanged);

    connect(device, &hhc::device::sigMaxValueChanged, this,
            &MainWindow::onMaxValueChanged);

    connect(device, &hhc::device::sigMinValueChanged, this,
            &MainWindow::onMinValueChanged);

    connect(device, &hhc::device::sigPositionValueChanged, this,
            &MainWindow::onPositionValueChanged);
/*
    connect(device, &hhc::device::sigReadyRead, this,
            &MainWindow::onReadData);
*/
    connect(device, &hhc::device::sigReadLine, this,
            &MainWindow::onReadLine);

    connect(device, &hhc::device::sigReady, this,
            &MainWindow::onReady);

#ifdef Q_OS_ANDROID
    connect(device, &hhc::bluetooth::sigDeviceDiscovered, this,
            &MainWindow::onBluetoothDeviceDiscovered);

    connect(device, &hhc::bluetooth::sigDeviceScanFinished, this,
            &MainWindow::onBluetoothDeviceScanFinished);

    connect(device, &hhc::bluetooth::sigServiceDiscovered, this,
            &MainWindow::onBluetoothServiceDiscovered);

    connect(device, &hhc::bluetooth::sigServiceScanFinished, this,
            &MainWindow::onBluetoothServiceScanFinished);
#endif
    setStyleSheet(
#ifdef Q_OS_ANDROID
    "QWidget {background-color:white;color:black}"
    "QToolButton {border:1px solid #ababab;border-radius:3px;"
    "background-color:qlineargradient(x1:0,y1:0,x2:0,y2:1,"
                                      "stop:0 #fafafa,stop:1 #eaeaea);}"
    "QToolButton:pressed {background-color:"
                     "qlineargradient(x1:0,y1:0,x2:0,y2:1,"
                                      "stop:0 #dadada,stop:1 #fafafa);}"
#endif
    "#txtLog {background-color:black;color:lightGreen}");

    setIconSize(QSize(24, 24));
    setWindowIcon(QIcon(":/icons/icon.png"));
    setWindowTitle(QCoreApplication::applicationName());
    setCentralWidget(wgtMain);

    loadAppSettings();
    setConnected(false);
    scanDevices();
}
MainWindow::~MainWindow()
{
}
void MainWindow::closeEvent(QCloseEvent *)
{
    saveAppSettings();
}
void MainWindow::loadAppSettings()
{
    QSettings appSettings(QSettings::IniFormat, QSettings::UserScope,
                          QCoreApplication::organizationName(),
                          QCoreApplication::applicationName());

    appSettings.beginGroup("UI");
    resize(appSettings.value("size", QSize(480, 600)).toSize());
    move(appSettings.value("position", QPoint(200, 200)).toPoint());
    int index = appSettings.value("tabIndex", 0).toInt();

    if (index < tloMain->count())
        tloMain->setCurrentIndex(index);

    appSettings.endGroup();

    appSettings.beginGroup("Profile");
    QString profileName = appSettings.value("current").toString();
    if (!profileName.isEmpty())
        settings->setName(profileName);
    appSettings.endGroup();
}
void MainWindow::saveAppSettings()
{
    QSettings appSettings(QSettings::IniFormat, QSettings::UserScope,
                          QCoreApplication::organizationName(),
                          QCoreApplication::applicationName());

    appSettings.beginGroup("UI");
    appSettings.setValue("size",     this->size());
    appSettings.setValue("position", this->pos());
    appSettings.setValue("tabIndex", tloMain->currentIndex());
    appSettings.endGroup();

    appSettings.beginGroup("Profile");
    appSettings.setValue("current", settings->name());
    appSettings.endGroup();
}
void MainWindow::loadProfileToUI()
{
    // TODO: setCurve etc. from here
    pageHome->setFixed1(settings->fixed1());
    pageHome->setFixed2(settings->fixed2());
    pageHome->setFixed3(settings->fixed3());
    pageHome->setMinPosition(settings->minPosition());
    pageHome->setMaxPosition(settings->maxPosition());
}
void MainWindow::setProfileFromUI()
{
    // Implicit cast
    settings->setFixed1(pageHome->fixed1());
    settings->setFixed2(pageHome->fixed2());
    settings->setFixed3(pageHome->fixed3());
    settings->setMinPosition(pageHome->minPosition());
    settings->setMaxPosition(pageHome->maxPosition());
}
void MainWindow::setConnected(bool connected)
{
    if (!connected)
    {
        actConn->setIcon(QIcon(":/icons/connect.png"));
        actConn->setText(tr("Connect"));
    }
    else
    {
        actConn->setIcon(QIcon(":/icons/disconnect.png"));
        actConn->setText(tr("Disconnect"));
    }
    actLoad->setEnabled(connected);
    actSave->setEnabled(connected);
    actSvAs->setEnabled(connected);

    pageHome->setConnected(connected);
    pageCurv->setConnected(connected);
    pageConn->setConnected(connected);
    pageTerm->setConnected(connected);
}
void MainWindow::writeData(const QByteArray &data)
{
    device->write(data);
}
void MainWindow::scanDevices()
{
    actConn->setEnabled(true);
    pageConn->showScanInProgress();
    pageTerm->clear();

    device->scan();

    bool deviceFound =
#ifdef Q_OS_ANDROID
    pageConn->bluetoothDeviceCount() > 0;
#else
    false;
    for (int i = 0; i < device->portCount(); i++)
    {
        QStringList listInfos = device->portInfosAt(i);

        deviceFound = true;
        pageConn->showSerialPortOptions();
        pageConn->addSerialPort(device->portName(i), listInfos);
    }
#endif
    if (!deviceFound)
    {
        actConn->setEnabled(false);
        pageConn->showNotFound();
        pageTerm->write(tr("(i) Device not found.\n"
                        "Check connections and restart this application.\n"));
    }
}
void MainWindow::doToggleConnection()
{
    if (!device)
        return;

    if (device->state() == hhc::device::ConnectedState)
    {
        doDisconnect();
    }
    else
    {
        actScan->setEnabled(false);
        doConnect();
    }
}
void MainWindow::doConnect()
{
    QString portName =
#ifndef Q_OS_ANDROID
        pageConn->serialPortName();
#else
        pageConn->bluetoothAddress();
#endif
    if (!portName.isEmpty())
    {
/*
        connect(device, &hhc::device::sigConnected, this,
                &MainWindow::onConnected);

        connect(device, &hhc::device::sigCurveChanged, this,
                &MainWindow::onCurveChanged);

        connect(device, &hhc::device::sigCurvePointChanged, this,
                &MainWindow::onCurvePointChanged);

        connect(device, &hhc::device::sigDisconnected, this,
                &MainWindow::onDisconnected);

        connect(device, &hhc::device::sigError, this,
                &MainWindow::onError);

        connect(device, &hhc::device::sigFixed1ValueChanged, this,
                &MainWindow::onFixed1ValueChanged);

        connect(device, &hhc::device::sigFixed2ValueChanged, this,
                &MainWindow::onFixed2ValueChanged);

        connect(device, &hhc::device::sigFixed3ValueChanged, this,
                &MainWindow::onFixed3ValueChanged);

        connect(device, &hhc::device::sigIntervalValueChanged, this,
                &MainWindow::onIntervalValueChanged);

        connect(device, &hhc::device::sigMaxValueChanged, this,
                &MainWindow::onMaxValueChanged);

        connect(device, &hhc::device::sigMinValueChanged, this,
                &MainWindow::onMinValueChanged);

        connect(device, &hhc::device::sigPositionValueChanged, this,
                &MainWindow::onPositionValueChanged);

        connect(device, &hhc::device::sigReadLine, this,
                &MainWindow::onReadLine);
*/
        if (device->open(portName))
        {
/*
            Enable immediatly the CLI so if something goes wrong
            after connection we will be able to send commands manually.
*/
            pageTerm->setConnected(true);
            pageTerm->setFocusCLI();

            actConn->setIcon(QIcon(":/icons/disconnect.png"));
            actConn->setText(tr("Disconnect"));
#ifdef Q_OS_ANDROID
            pageTerm->write(tr("(i) Connecting...\n"));
#endif
        }
        else
        {
            pageTerm->write(tr("(!) Error: ") + device->errorString() + "\n");
            return;
        }
    }
}
void MainWindow::doDisconnect()
{
    device->close();
/*
    disconnect(device, &hhc::device::sigConnected,            this, &MainWindow::onConnected);
    disconnect(device, &hhc::device::sigCurveChanged,         this, &MainWindow::onCurveChanged);
    disconnect(device, &hhc::device::sigCurvePointChanged,    this, &MainWindow::onCurvePointChanged);
    disconnect(device, &hhc::device::sigError,                this, &MainWindow::onError);
    disconnect(device, &hhc::device::sigFixed1ValueChanged,   this, &MainWindow::onFixed1ValueChanged);
    disconnect(device, &hhc::device::sigFixed2ValueChanged,   this, &MainWindow::onFixed2ValueChanged);
    disconnect(device, &hhc::device::sigFixed3ValueChanged,   this, &MainWindow::onFixed3ValueChanged);
    disconnect(device, &hhc::device::sigIntervalValueChanged, this, &MainWindow::onIntervalValueChanged);
    disconnect(device, &hhc::device::sigMaxValueChanged,      this, &MainWindow::onMaxValueChanged);
    disconnect(device, &hhc::device::sigMinValueChanged,      this, &MainWindow::onMinValueChanged);
    disconnect(device, &hhc::device::sigPositionValueChanged, this, &MainWindow::onPositionValueChanged);
    disconnect(device, &hhc::device::sigReadLine,             this, &MainWindow::onReadLine);
*/
    isStarted = false;
}
void MainWindow::onConnected()
{
    setConnected(true);
    pageTerm->write(tr("(i) Connected.\n"));
}
void MainWindow::onDisconnected()
{
    pageTerm->clear();
    pageTerm->write(tr("(i) Disconnected.\n"));
    actScan->setEnabled(true);

    setConnected(false);
/*
    disconnect(device, &hhc::device::sigDisconnected, this,
               &MainWindow::onDisconnected);
*/
}
void MainWindow::onReady()
{
    qDebug() << "qDebug(): Got ready signal.";
}
void MainWindow::onReadLine(const QString &line)
{
    pageTerm->write(line);
    pageTerm->scrollDown();
}
void MainWindow::onError(uint8_t error)
{
    bool isCritical = false;
    QString errorString;

#ifndef Q_OS_ANDROID
    if (error == hhc::serial::ResourceError)
    {
        errorString = tr("(!) Critical Error: ") + device->errorString() + ".\n";
        isCritical = true;
    }
#endif
    if (error > 0) // TODO: Check errors in a better way for disconnection
    {
        errorString = tr("(!) Critical Error: ") + device->errorString() + ".\n";
        isCritical = true;
    }
    if (isCritical)
    {
        pageConn->setConnected(false);
        doDisconnect();
        pageTerm->write(errorString);
    }
}
void MainWindow::onCurveChanged(const QStringList &strList)
{
//  TODO: Change the read-only curve on save, not on set
    if (!isStarted)
    {
        int value, index = 0;
        QVector<int> curValues = QVector<int>(HHCurve::MaxValue + 1);

        foreach (QString strValue, strList)
        {
            value = strValue.toInt();
            curValues[index] = value;
            index++;
        }
        pageCurv->setValuesAt(0, curValues);

//      Copy the curve to slots on connect.
        for (int i = 1; i < pageCurv->count(); ++i)
            pageCurv->setValuesAt(i, curValues);

        isStarted = true;
    }
}
void MainWindow::onCurvePointChanged(const QPoint &curvePoint)
{
    pageCurv->currentPage()->setValue(curvePoint.x(), curvePoint.y());
//  qDebug() << "qDebug(): X: " << curvePoint.x() << " Y: " << curvePoint.y();
}
void MainWindow::onFixed1ValueChanged(const uint8_t value)
{
    hasDeviceSetValue = true;
    settings->setFixed1(value);
    pageHome->setFixed1(value);
}
void MainWindow::onFixed2ValueChanged(const uint8_t value)
{
    hasDeviceSetValue = true;
    settings->setFixed2(value);
    pageHome->setFixed2(value);
}
void MainWindow::onFixed3ValueChanged(const uint8_t value)
{
    hasDeviceSetValue = true;
    settings->setFixed3(value);
    pageHome->setFixed3(value);
}
void MainWindow::onMinValueChanged(const uint16_t value)
{
    hasDeviceSetValue = true;
    settings->setMinPosition(value);
    pageHome->setMinPosition(value);
}
void MainWindow::onMaxValueChanged(const uint16_t value)
{
    hasDeviceSetValue = true;
    settings->setMaxPosition(value);
    pageHome->setMaxPosition(value);

//  if (device->position() > device->maxPosition())
//      pbrHHLevel->setValue(value);
}
void MainWindow::onPositionValueChanged(const uint16_t value)
{
    pageHome->setPosition(value);
}
void MainWindow::onIntervalValueChanged(const uint16_t value)
{
    pageConn->setInterval(value);
    pageConn->setOldInterval(value);
}
void MainWindow::onOpenProfile()
{
    if(!settings->count())
    {
        QMessageBox::information(this, tr("Load Profile"),
                                 tr("There are no profiles saved yet, save one first."),
                                 QMessageBox::Ok);
        return;
    }
    bool ok;
    QStringList list = settings->list();
    QString profileName = QInputDialog::getItem(this, tr("Load Profile"),
                                                      tr("Profile name:"),
                                                      list, 0, false, &ok);
    if (ok && !profileName.isEmpty())
    {
        settings->load(profileName);
        loadProfileToUI();
    }
}
void MainWindow::onSaveProfile()
{
    settings->save();
}
void MainWindow::onSaveAsProfile()
{
    bool ok;
    QString profileName = settings->name();

    if (profileName.isEmpty())
        profileName = "Profile" + QString::number(settings->count());

    profileName = QInputDialog::getText(this, tr("Save Profile As..."),
                                              tr("Profile name:"),
                                              QLineEdit::Normal, profileName, &ok);
    if (!ok || !checkProfile(profileName))
        return;

    settings->save(profileName);
}
bool MainWindow::checkProfile(const QString &name)
{
    if (!name.isEmpty())
    {
        settings->setName(name);
        return true;
    }
    // TODO: Avoid this.
    // An error occurred or the user is an asshole deleting predefined name.
    QMessageBox::critical(this, tr("Error"), tr("An error occurred, profile was not saved."));
    return false;
}
void MainWindow::onAbout()
{
    actBar->setDisplayHomeAsUpEnabled();
    stkMain->setCurrentIndex(1);
}
void MainWindow::onActionBarUp()
{
    actBar->setDisplayHomeAsUpEnabled(false);
    actBar->setTitle(QApplication::applicationDisplayName());
    stkMain->setCurrentIndex(0);
}
void MainWindow::onSetFixed1()
{
    device->setFixed1(pageHome->fixed1());
    pageHome->disablePedalSetButton(pageHome->tbnSetFixed1());
}
void MainWindow::onSetFixed2()
{
    device->setFixed2(pageHome->fixed2());
    pageHome->disablePedalSetButton(pageHome->tbnSetFixed2());
}
void MainWindow::onSetFixed3()
{
    device->setFixed3(pageHome->fixed3());
    pageHome->disablePedalSetButton(pageHome->tbnSetFixed3());
}
void MainWindow::onSetClosed()
{
    device->setMinPosition(pageHome->minPosition());
    pageHome->disablePedalSetButton(pageHome->tbnSetClosed());
}
void MainWindow::onSetOpen()
{
    device->setMaxPosition(pageHome->maxPosition());
    pageHome->disablePedalSetButton(pageHome->tbnSetOpen());
}
void MainWindow::onSetAsClosed()
{
    uint16_t value = pageHome->position();
    device->setMinPosition(value);
    pageHome->setSetupUnsaved();
}
void MainWindow::onSetAsOpen()
{
    uint16_t value = pageHome->position();
    device->setMaxPosition(value);
    pageHome->setSetupUnsaved();
}
void MainWindow::onPedalPositionsSet()
{
    onSetFixed1();
    onSetFixed2();
    onSetFixed3();
    onSetClosed();
    onSetOpen();
    pageHome->setSetupUnsaved();
}
void MainWindow::onPedalPositionsSave()
{
    device->writeFixed1();
    device->writeFixed2();
    device->writeFixed3();
    device->writeMinMax();
//  device->writeCurve();
    pageHome->setSetupClean();;
}
void MainWindow::onPedalPositionsRestore()
{
    loadProfileToUI();
    pageHome->setSetupClean();
}
void MainWindow::onSetInterval()
{
    device->setInterval(pageConn->interval());
}
void MainWindow::onSpinBoxValueChanged(int value)
{
    if (hasDeviceSetValue)
    {
        hasDeviceSetValue = false;
        return;
    }
    bool changed = true;

    SpinBox *spinBox = dynamic_cast<SpinBox *>(sender());
    if (spinBox == pageHome->sbxClosed())
    {
        if (value == settings->minPosition())
            changed = false;

        pageHome->tbnSetClosed()->setEnabled(changed);
    }
    else if (spinBox == pageHome->sbxOpen())
    {
        if (value == settings->maxPosition())
            changed = false;

        pageHome->tbnSetOpen()->setEnabled(changed);
    }
    else if (spinBox == pageHome->sbxFixed1())
    {
        if (value == settings->fixed1())
            changed = false;

        pageHome->tbnSetFixed1()->setEnabled(changed);
    }
    else if (spinBox == pageHome->sbxFixed2())
    {
        if (value == settings->fixed2())
            changed = false;

        pageHome->tbnSetFixed2()->setEnabled(changed);
    }
    else // if (spinBox == pageHome->sbxFixed3())
    {
        if (value == settings->fixed3())
            changed = false;

        pageHome->tbnSetFixed3()->setEnabled(changed);
    }
    pageHome->setSetupChanged(changed);
}
void MainWindow::setCurrentIndex(int)
{
    onCurveOk();
}
void MainWindow::onCurveCancel()
{
    QStringList  curveList = device->curve();
    QVector<int> values(QVector<int>(HHCurve::MaxValue + 1));

    for (int i = 0; i < values.size(); ++i)
    {
        values[i] = curveList.at(i).toInt();
    }
    pageCurv->currentPage()->setValues(values);
}
void MainWindow::onCurveOk()
{
    QVector<int> values = pageCurv->currentPage()->values();
    for (int i = 0; i < values.size(); ++i)
    {
        QPoint point(i, values.at(i));
        device->setCurvePoint(point);
    }
}
void MainWindow::onCurveSave()
{
    QVector<int> values = pageCurv->currentPage()->values();

    pageCurv->setValuesAt(0, values); // Update the read-only curve slot
    device->writeCurve();
}
#ifdef Q_OS_ANDROID
void MainWindow::onBluetoothDeviceDiscovered(const QStringList &listInfos)
{
    QString deviceName    = listInfos.at(0);
    QString deviceAddress = listInfos.at(1);

    qDebug() << "qDebug(): Device found: " << deviceName << " at " << deviceAddress;

    if (pageConn->bluetoothDeviceCount() == 0)
    {
        actConn->setEnabled(true);
        pageConn->showBluetoothDeviceOptions();
        pageTerm->clear();
    }
    pageConn->updateBluetoothDeviceScanProgress(QString("%1").arg(deviceName));
    pageConn->addBluetoothDevice(deviceName, deviceAddress);
}
void MainWindow::onBluetoothDeviceScanFinished()
{
    pageConn->bluetoothDeviceScanFinished();
}
void MainWindow::onBluetoothServiceDiscovered(const QStringList &listInfos)
{
    QString serviceName   = listInfos.at(0);
    QString deviceAddress = listInfos.at(1);

    qDebug() << "qDebug(): Service found: " << serviceName << " at " << deviceAddress;

    pageConn->updateBluetoothServiceScanProgress(QString("%1").arg(serviceName));
}
void MainWindow::onBluetoothServiceScanFinished()
{
    pageConn->bluetoothServiceScanFinished();
}
#endif
