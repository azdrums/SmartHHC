/*
	SmartHHC - Electronic Drums HiHat Controller Manager
	Copyright (C) 2018 Andrea Zanellato

	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
