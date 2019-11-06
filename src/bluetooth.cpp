/*
	SmartHHC - Electronic Drums HiHat Controller Manager
	Copyright (C) 2018-2019 Andrea Zanellato

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#include <QtGlobal>
#ifdef Q_OS_ANDROID

#include <QBluetoothAddress>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothSocket>

#include "setup.h"
#include "bluetooth.h"

namespace hhc {

bluetooth::bluetooth(QObject *parent)
:
	device(parent),
	devDiscoveryAgent(new QBluetoothDeviceDiscoveryAgent(this)),
	svcDiscoveryAgent(new QBluetoothServiceDiscoveryAgent(this))
{
	connect(svcDiscoveryAgent, &QBluetoothServiceDiscoveryAgent::serviceDiscovered,
			this, &bluetooth::onServiceDiscovered);

	connect(svcDiscoveryAgent, &QBluetoothServiceDiscoveryAgent::finished, this,
			&bluetooth::onServiceScanFinished);

	connect(devDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
			this, &bluetooth::onDeviceDiscovered);

	connect(devDiscoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this,
			&bluetooth::onDeviceScanFinished);

	connect(qIoDevice, &QBluetoothSocket::stateChanged, this, &bluetooth::onStateChanged);

	connect(qIoDevice, &QBluetoothSocket::connected, this, &device::onConnected);

	connect(qIoDevice, &QBluetoothSocket::disconnected, this, &device::onDisconnected);

	connect(qIoDevice,
			QOverload<QBluetoothSocket::SocketError>::of(&QBluetoothSocket::error),
			[=](QBluetoothSocket::SocketError error)
	{
		emit sigError(error);
	});
	connect(qIoDevice, &QBluetoothSocket::readyRead, this,
			&hhc::bluetooth::onSocketRead);
}
/*
// TODO: This works under Linux, not on Android, see issue

bool bluetooth::open(const QString &addressString)
{
	QBluetoothAddress address(addressString);
	QBluetoothUuid	uuid(QString("00001101-0000-1000-8000-00805F9B34FB"));

	qIoDevice->connectToService(address, uuid);
	return true;
}
*/
bool bluetooth::open(const QString &addressString)
{
	if (connState != DisconnectedState)
	{
		qDebug() << "qDebug(): Trying to open an opened connection.";
		return false;
	}
	foreach(QBluetoothServiceInfo service, services)
	{
		if ((service.isValid()) &&
			(service.device().address().toString() == addressString))
		{
			qIoDevice->connectToService(service);
			qDebug() << "qDebug(): connectToService(): " << service.serviceName();
			return true;
		}
	}
	qDebug() << "qDebug(): open() failed.";
	return false;
}
void bluetooth::scan()
{
	portInfos.clear();

	if (devDiscoveryAgent->isActive())
		devDiscoveryAgent->stop();

	devDiscoveryAgent->start();
}
void bluetooth::onDeviceDiscovered(const QBluetoothDeviceInfo &deviceInfo)
{
	QString namePrefix	= hhc::NamePrefix + '-';
	QString deviceName	= deviceInfo.name();
	QString deviceAddress = deviceInfo.address().toString();

	if (deviceInfo.name().left(9) != namePrefix)
	{
		qDebug() << "qDebug(): Skipping device: " << deviceName << " at " << deviceAddress;
		return;
	}
	QStringList listInfo {deviceName, deviceAddress};

	portInfos.push_back(listInfo);

	qDebug() << "qDebug(): Discovered device: " << deviceName << " at " << deviceAddress;

	emit sigDeviceDiscovered(listInfo);
}
void bluetooth::onDeviceScanFinished()
{
	if (svcDiscoveryAgent->isActive())
		svcDiscoveryAgent->stop();
/*
	Using the quick descovery method by get devices from system cache for the
	known devices, enable FullDiscovery if needed or remove comment on start().
*/
	QBluetoothUuid uuid(QString("00001101-0000-1000-8000-00805F9B34FB"));
	svcDiscoveryAgent->setUuidFilter(uuid);
	svcDiscoveryAgent->start(/*QBluetoothServiceDiscoveryAgent::FullDiscovery*/);

	emit sigDeviceScanFinished();
}
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
		qDebug() << "qDebug(): Skipping service: " << serviceName << " at " << deviceAddress;
		return;
	}
	QStringList listInfo {serviceName, deviceAddress};

	services.push_back(serviceInfo);

	qDebug() << "qDebug(): Discovered service: " << serviceName << " at " << deviceAddress;

	emit sigServiceDiscovered(listInfo);
}
void bluetooth::onServiceScanFinished()
{
	emit sigServiceScanFinished();
}
void bluetooth::onStateChanged(QBluetoothSocket::SocketState state)
{
	if (state == QBluetoothSocket::ConnectedState)
	{
		connState = ConnectedState;
	}
	else if (state == QBluetoothSocket::ConnectingState)
	{
		connState = ConnectingState;
	}
	else if (state == QBluetoothSocket::UnconnectedState)
	{
		connState = DisconnectedState;
	}
}
} // namespace hhc

#endif // def Q_OS_ANDROID
