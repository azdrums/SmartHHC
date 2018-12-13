/*
    SmartHHC - Electronic Drums HiHat Controller Manager
    Copyright (C) 2018 Andrea Zanellato

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include "device.h"
#ifdef Q_OS_ANDROID
#include <QBluetoothServiceInfo>
#endif
#include <QVector>

class QBluetoothDeviceDiscoveryAgent;
#ifdef Q_OS_ANDROID
class QBluetoothServiceDiscoveryAgent;
#endif
class QBluetoothDeviceInfo;
class QBluetoothSocket;

namespace hhc {
class bluetooth : public device
{
    Q_OBJECT

public:
    explicit bluetooth(QObject *parent = nullptr);
    ~bluetooth() {}

    QByteArray readAll();
    void write(const QByteArray &);
    void close();
    bool open(const QString &addressString = QString());
    QString errorString() const;
    Type type() const {return Type::Bluetooth;}
    void sendCommand(const QString &);

    void scan();

signals:
    void sigDeviceDiscovered(const QStringList &);
    void sigDeviceScanFinished();
#ifdef Q_OS_ANDROID
    void sigServiceDiscovered(const QStringList &);
    void sigServiceScanFinished();
#endif
private:
    void onDeviceDiscovered(const QBluetoothDeviceInfo &);
    void onDeviceScanFinished();
#ifdef Q_OS_ANDROID
    void onServiceDiscovered(const QBluetoothServiceInfo &);
    void onServiceScanFinished();
#endif
    void onSocketRead();

    QBluetoothDeviceDiscoveryAgent  *devDiscoveryAgent;
#ifdef Q_OS_ANDROID
    QBluetoothServiceDiscoveryAgent *svcDiscoveryAgent;
#endif
    QBluetoothSocket                *socket;

#ifdef Q_OS_ANDROID
    QVector<QBluetoothServiceInfo> services;
#endif
};
} // namespace hhc

#endif // BLUETOOTH_H
