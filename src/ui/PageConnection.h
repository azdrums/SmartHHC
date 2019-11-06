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
#pragma once
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
	QString		serialPortName() const;
	int			serialPortCount() const;

	void addSerialPort(const QString &, const QStringList &);
	void showSerialPortOptions();
	void resetSerialPortInfos();
	void setSerialPortCurrentIndex(int);

	void onSerialPortCurrentIndexChanged(int);
#endif
	void showNotFound();

#ifdef Q_OS_ANDROID
	QString bluetoothAddress() const;
	void addBluetoothDevice(const QString &, const QString &);
	void showBluetoothDeviceOptions();
	void setBluetoothDeviceCurrentIndex(int);
	void updateBluetoothDeviceScanProgress(const QString &);
	void bluetoothDeviceScanFinished();
	void updateBluetoothServiceScanProgress(const QString &);
	void bluetoothServiceScanFinished();

	int bluetoothDeviceCount();

	void onBluetoothDeviceCurrentIndexChanged(int);
#endif

signals:
#ifndef Q_OS_ANDROID
	void sigSerialPortIndexChanged(int);
	void sigSerialPortIndexChanged(const QString &);
#else
	void sigBluetoothDeviceIndexChanged(int);
	void sigBluetoothDeviceIndexChanged(const QString &);
#endif
	void sigIntervalChanged(int);
	void sigSettingInterval();

	void sigRestart();

private:
	Ui::PageConnection *ui;

	int oldIntervalValue = 0; // Cache old interval value.
};
