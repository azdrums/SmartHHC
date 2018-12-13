/*
    SmartHHC - Electronic Drums HiHat Controller Manager
    Copyright (C) 2018 Andrea Zanellato

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include <QBluetoothAddress>
#include <QBluetoothDeviceDiscoveryAgent>
#ifdef Q_OS_ANDROID
#include <QBluetoothServiceDiscoveryAgent>
#endif
#include <QBluetoothSocket>

#include "setup.h"
#include "bluetooth.h"

namespace hhc {

bluetooth::bluetooth(QObject *parent)
:
    device(parent),
    devDiscoveryAgent(new QBluetoothDeviceDiscoveryAgent(this)),
#ifdef Q_OS_ANDROID
    svcDiscoveryAgent(new QBluetoothServiceDiscoveryAgent(this)),
#endif
    socket(new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this))
{
    connect(devDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &bluetooth::onDeviceDiscovered);

    connect(devDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this,
            &bluetooth::onDeviceScanFinished);
#ifdef Q_OS_ANDROID
    connect(svcDiscoveryAgent, &QBluetoothServiceDiscoveryAgent::serviceDiscovered,
            this, &bluetooth::onServiceDiscovered);

    connect(svcDiscoveryAgent, &QBluetoothServiceDiscoveryAgent::finished, this,
            &bluetooth::onServiceScanFinished);
#endif
    connect(socket, &QBluetoothSocket::connected, this, &device::onConnected);

    connect(socket, &QBluetoothSocket::disconnected, this, &device::onDisconnected);

    connect(socket,
            QOverload<QBluetoothSocket::SocketError>::of(&QBluetoothSocket::error),
            [=](QBluetoothSocket::SocketError error)
    {
        emit sigError(error);
    });
    connect(socket, &QBluetoothSocket::readyRead, this,
            &hhc::bluetooth::onSocketRead);
}
#ifndef Q_OS_ANDROID
bool bluetooth::open(const QString &addressString)
{
    QBluetoothAddress address(addressString);
    QBluetoothUuid    uuid(QString("00001101-0000-1000-8000-00805F9B34FB"));

    socket->connectToService(address, uuid);
    return true;
}
#else
bool bluetooth::open(const QString &addressString)
{
    if (isConnected_)
    {
        qDebug() << "Trying to open an opened connection.";
        return false;
    }
    foreach(QBluetoothServiceInfo service, services)
    {
        if ((service.isValid()) &&
            (service.device().address().toString() == addressString))
        {
            socket->connectToService(service);
            qDebug() << "connectToService(): " << service.serviceName();
            return true;
        }
    }
    qDebug() << "open() failed.";
    return false;
}
#endif
void bluetooth::close()
{
    if (socket->isOpen())
        socket->close();
}
QString bluetooth::errorString() const
{
    if (!socket)
        return QString();

    return socket->errorString();
}
void bluetooth::write(const QByteArray &data)
{
    socket->write(data);
}
QByteArray bluetooth::readAll()
{
    const QByteArray data = socket->readAll();
    processIncomingData(data);
    return data;
}
void bluetooth::sendCommand(const QString &cmd)
{
    QByteArray data = cmd.toUtf8() + "\r\n";
    socket->write(data);
}
void bluetooth::scan()
{
    portInfos.clear();

    if (devDiscoveryAgent->isActive())
        devDiscoveryAgent->stop();

    devDiscoveryAgent->start();

#ifdef Q_OS_ANDROID
    if (svcDiscoveryAgent->isActive())
        svcDiscoveryAgent->stop();
/*
    Using the quick descovery method by get devices from system cache for the
    known devices, enable FullDiscovery if needed or remove comment on start().
*/
    QBluetoothUuid uuid(QString("00001101-0000-1000-8000-00805F9B34FB"));
    svcDiscoveryAgent->setUuidFilter(uuid);
    svcDiscoveryAgent->start(/*QBluetoothServiceDiscoveryAgent::FullDiscovery*/);
#endif
}
void bluetooth::onDeviceDiscovered(const QBluetoothDeviceInfo &deviceInfo)
{
    QString namePrefix    = hhc::NamePrefix + '-';
    QString deviceName    = deviceInfo.name();
    QString deviceAddress = deviceInfo.address().toString();

    if (deviceInfo.name().left(4) != namePrefix)
    {
        qDebug() << "Skipping: " << deviceName << " at " << deviceAddress;
        return;
    }
    QStringList listInfo {deviceName, deviceAddress};

    portInfos.push_back(listInfo);

    qDebug() << "Discovered: " << deviceName << " at " << deviceAddress;

    emit sigDeviceDiscovered(listInfo);
}
void bluetooth::onDeviceScanFinished()
{
    emit sigDeviceScanFinished();
}
#ifdef Q_OS_ANDROID
void bluetooth::onServiceDiscovered(const QBluetoothServiceInfo &serviceInfo)
{
    QString serviceName   = serviceInfo.serviceName();
    QString deviceAddress = serviceInfo.device().address().toString();

//  Check if service is one of devices registered.
    bool found = false;
    for (int i = 0; i < portInfos.size(); ++i)
    {
        if (portInfos.at(i).at(1) == deviceAddress)
            found = true;
    }
    if (!found)
    {
        qDebug() << "Skipping: " << serviceName << " at " << deviceAddress;
        return;
    }
    QStringList listInfo {serviceName, deviceAddress};

    services.push_back(serviceInfo);

    qDebug() << "Discovered: " << serviceName << " at " << deviceAddress;

    emit sigServiceDiscovered(listInfo);
}
void bluetooth::onServiceScanFinished()
{
    emit sigServiceScanFinished();
}
#endif
void bluetooth::onSocketRead()
{
    if (!socket->isReadable())
        return;

    while (socket->canReadLine())
    {
        QByteArray line = socket->readLine();
        QString strLine = QString::fromUtf8(line.constData(), line.length());

        parse(strLine);
//      qDebug() << strLine;
        emit sigReadLine(strLine);
    }
}
} // namespace hhc
