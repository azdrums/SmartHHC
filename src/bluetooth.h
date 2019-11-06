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
#ifdef  Q_OS_ANDROID
#pragma once
#include <QBluetoothServiceInfo>
#include <QBluetoothSocket>
#include <QVector>

#include "device.h"

class QBluetoothDeviceInfo;
class QBluetoothDeviceDiscoveryAgent;
class QBluetoothServiceDiscoveryAgent;

namespace hhc {
class bluetooth : public device
{
	Q_OBJECT

public:
	explicit bluetooth(QObject *parent = nullptr);
	~bluetooth() {}

	bool open(const QString &addressString = QString());
	Type type() const {return Type::Bluetooth;}
	void scan();

signals:
	void sigDeviceDiscovered(const QStringList &);
	void sigDeviceScanFinished();

	void sigServiceDiscovered(const QStringList &);
	void sigServiceScanFinished();

private:
	void onDeviceDiscovered(const QBluetoothDeviceInfo &);
	void onDeviceScanFinished();
	void onStateChanged(QBluetoothSocket::SocketState);

	void onServiceDiscovered(const QBluetoothServiceInfo &);
	void onServiceScanFinished();

	QVector<QBluetoothServiceInfo>  services;

	QBluetoothDeviceDiscoveryAgent  *devDiscoveryAgent;
	QBluetoothServiceDiscoveryAgent *svcDiscoveryAgent;
};
} // namespace hhc
#endif // def Q_OS_ANDROID
