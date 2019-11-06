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
#ifndef Q_OS_ANDROID

#include "serial.h"

#include <QSerialPortInfo>

namespace hhc {

serial::serial(QObject *parent)
:
device(parent)
{
	connect(qIoDevice, &QSerialPort::errorOccurred, this, &serial::onErrorOccurredRedirect);
	connect(qIoDevice, &QSerialPort::readyRead,     this, &serial::onSocketRead);
	connect(qIoDevice, &QSerialPort::aboutToClose,  this, &serial::onDisconnected);
}
bool serial::open(const QString &portName)
{
	qIoDevice->setPortName(portName);
	qIoDevice->setBaudRate(38400);
	qIoDevice->setDataBits(QSerialPort::Data8);
	qIoDevice->setParity(QSerialPort::NoParity);
	qIoDevice->setStopBits(QSerialPort::OneStop);
	qIoDevice->setFlowControl(QSerialPort::NoFlowControl);

	if (qIoDevice->open(QIODevice::ReadWrite))
	{
		onConnected();
		return true;
	}
	return false;
}
void serial::scan()
{
	QString description, manufacturer, serialNumber;
	portInfos.clear();

	const auto infos = QSerialPortInfo::availablePorts();
	for (const QSerialPortInfo &info : infos)
	{
#ifdef Q_OS_LINUX
		if (info.portName() == "ttyS0")
			continue;
#endif
		QStringList list;
		description  = info.description();
		manufacturer = info.manufacturer();
		serialNumber = info.serialNumber();
		list << info.portName()
			 << (!description.isEmpty() ?  description  : tr("N/A"))
			 << (!manufacturer.isEmpty() ? manufacturer : tr("N/A"))
			 << (!serialNumber.isEmpty() ? serialNumber : tr("N/A"))
			 << info.systemLocation()
			 << (info.vendorIdentifier()  ?
				QString::number(info.vendorIdentifier(), 16) : tr("N/A"))
			 << (info.productIdentifier() ?
				QString::number(info.productIdentifier(), 16) : tr("N/A"));

		portInfos.push_back(list);
	}
}
void serial::onErrorOccurredRedirect(QSerialPort::SerialPortError error)
{
	emit sigError(error);
}
} // namespace hhc

#endif // ndef Q_OS_ANDROID
