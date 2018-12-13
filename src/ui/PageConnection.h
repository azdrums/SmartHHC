/*
    SmartHHC - Electronic Drums HiHat Controller Manager
    Copyright (C) 2018 Andrea Zanellato

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef UICONNECTION_H
#define UICONNECTION_H

#include <QVector>
#include <QWidget>

enum ConnectionType
{
    Unknown = 0,
    Serial,
    Bluetooth
};
namespace Ui {
class PageConnection;
}
class QComboBox;
class QToolButton;
class SpinBox;
class PageConnection : public QWidget
{
    Q_OBJECT

public:
    explicit PageConnection(QWidget *parent = nullptr);
    ~PageConnection();

    void setConnected(bool);
    void showScanInProgress();

    void setInterval(int);
    void setOldInterval(int);
    void setIntervalChanged(bool);

    int  interval() const;

    void onIntervalChanged(int);

#ifndef Q_OS_ANDROID
    ConnectionType connectionType() const;
    QString        serialPortName() const;
    int            serialPortCount() const;

    void addSerialPort(const QString &, const QStringList &);
    void showSerialPortOptions();
    void toggleSerialPort(bool);
    void resetSerialPortInfos();
    void setSerialPortCurrentIndex(int);

    void onSerialPortCurrentIndexChanged(int);
#endif
    QString bluetoothAddress() const;

    void addBluetoothDevice(const QString &, const QString &);
    void showBluetoothDeviceOptions();
    void showNotFound();
    void setBluetoothDeviceCurrentIndex(int);
    void updateBluetoothDeviceScanProgress(const QString &);
    void bluetoothDeviceScanFinished();
#ifdef Q_OS_ANDROID
    void updateBluetoothServiceScanProgress(const QString &);
    void bluetoothServiceScanFinished();
#endif
    int bluetoothDeviceCount();

    void onBluetoothDeviceCurrentIndexChanged(int);

signals:
#ifndef Q_OS_ANDROID
    void sigSerialPortIndexChanged(int);
    void sigSerialPortIndexChanged(const QString &);
#endif
    void sigBluetoothDeviceIndexChanged(int);
    void sigBluetoothDeviceIndexChanged(const QString &);

    void sigIntervalChanged(int);
    void sigSettingInterval();

private:
    Ui::PageConnection *ui;

    int  oldIntervalValue = 0; // Cache old interval value.
};
#endif // UICONNECTION_H
