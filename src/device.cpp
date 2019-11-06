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
#include <QCoreApplication>
#include <QPoint>
#include <QTextCodec>

#ifdef Q_OS_ANDROID
	#include <QBluetoothSocket>
	#include <QBluetoothServiceInfo>
#else
	#include <QSerialPort>
#endif

#include "device.h"

namespace hhc
{
device::device(QObject *parent)
:
QObject(parent),
connState(DisconnectedState),
#ifdef Q_OS_ANDROID
	qIoDevice(new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol, this)),
#else
	qIoDevice(new QSerialPort(this)),
#endif
isStarted(false)
{
	for (int i=0; i<128; i++)
		m_curve.append(QString());

	 connect(QCoreApplication::instance(),
			&QCoreApplication::aboutToQuit, this, &device::onDisconnected);
}
device::~device()
{
}
void device::close()
{
	if (qIoDevice && qIoDevice->isOpen())
	{
		qIoDevice->close();
		m_fixed1	=
		m_fixed2	=
		m_fixed3	=
		m_lastIndex =
		m_min_pos   =
		m_max_pos   =
		m_position  =
		m_interval  = 0;
		m_curve	 = QStringList();
		onDisconnected();
	}
}
void device::onSocketRead()
{
	if (!qIoDevice->isReadable())
		return;

	while (qIoDevice->canReadLine())
	{
		QByteArray line = qIoDevice->readLine();
		QString strLine = QString::fromUtf8(line.constData(), line.length());

		parse(strLine);
//	  qDebug() << "qDebug(): " << strLine;
		emit sigReadLine(strLine);
	}
}
void device::write(const QByteArray &data)
{
	qIoDevice->write(data);
}
QString device::errorString() const
{
	if (!qIoDevice)
		return QString();

	return qIoDevice->errorString();
}
void device::sendCommand(const QString &cmd)
{
	QByteArray data = cmd.toUtf8() + "\r\n";
	qIoDevice->write(data);
}
QPoint device::curvePoint(uint8_t x) const
{
	Q_ASSERT(x < 128);

	uint8_t y = m_curve.at(x).toUShort();
	return QPoint(x, y);
}
void device::setFixed1(uint8_t value)
{
	QString cmd = "S f1 " + QString::number(value);
	sendCommand(cmd);
}
void device::setFixed2(uint8_t value)
{
	QString cmd = "S f2 " + QString::number(value);
	sendCommand(cmd);
}
void device::setFixed3(uint8_t value)
{
	QString cmd = "S f3 " + QString::number(value);
	sendCommand(cmd);
}
void device::setMinPosition(uint16_t value)
{
	QString cmd = "S m " + QString::number(value);
	sendCommand(cmd);
}
void device::setMaxPosition(uint16_t value)
{
	QString cmd = "S M " + QString::number(value);
	sendCommand(cmd);
}
void device::setInterval(uint16_t interval)
{
	QString cmd = "C " + QString::number(interval);
	sendCommand(cmd);
}
void device::setCurvePoint(const QPoint &point)
{
	QString sX  = QString::number(point.x()),
			sY  = QString::number(point.y()),
			cmd = "S r " + sX + " " + sY;
	sendCommand(cmd);
}
void device::requestData()
{
	sendCommand("G m");
	sendCommand("G M");
	sendCommand("G f1");
	sendCommand("G f2");
	sendCommand("G f3");
	sendCommand("G R");
	sendCommand("C 100");
}
QString device::portName(int index) const
{
	if (index < portInfos.count())
		return portInfos.at(index).at(0);

	return QString();
}
QStringList device::portInfosAt(int index)
{
	if (index < portInfos.count())
		return portInfos.at(index);

	return QStringList();
}
void device::onConnected()
{
	connState = ConnectedState;
	emit sigConnected();
}
void device::onDisconnected()
{
	if (connState != DisconnectedState)
	{
		connState = DisconnectedState;
//	  TODO: This should be the correct thing to do but stops the device
//			sending data after reconnection. Add a requestDisconnection().

//	  sendCommand("C 0"); // clean up
		emit sigDisconnected();
		isStarted = false;
	}
}
void device::parse(QString line)
{
	QStringList tokens = line.split(':');

	if (!tokens.count())
	{
		return; // No data?
	}
	else if (tokens.at(0).left(8) == "SmartHHC")
	{
		if (!isStarted)
		{
			emit sigReady();
			requestData();
			isStarted = true;
		}
	}
	else if (tokens.at(0) == "C")
	{
		uint16_t value = tokens.at(1).toUInt();
		if (m_interval == value)
			return;

		m_interval = value;
		emit sigIntervalValueChanged(m_interval);
	}
	else if (tokens.at(0) == "f1")
	{
		uint8_t value = tokens.at(1).toUShort();
		if (m_fixed1 == value)
			return;

		m_fixed1 = value;
		emit sigFixed1ValueChanged(m_fixed1);
	}
	else if (tokens.at(0) == "f2")
	{
		uint8_t value = tokens.at(1).toUShort();
		if (m_fixed2 == value)
			return;

		m_fixed2 = value;
		emit sigFixed2ValueChanged(m_fixed2);
	}
	else if (tokens.at(0) == "f3")
	{
		uint8_t value = tokens.at(1).toUShort();
		if (m_fixed3 == value)
			return;

		m_fixed3 = value;
		emit sigFixed3ValueChanged(m_fixed3);
	}
	else if (tokens.at(0) == "i")
	{
		uint8_t value = tokens.at(1).toUShort();
		if (m_lastIndex == value)
			return;

		m_lastIndex = value;
		emit sigLastIndexChanged(m_lastIndex);
	}
	else if (tokens.at(0) == "m")
	{
		quint16 value = tokens.at(1).toUInt();
		if (m_min_pos == value)
			return;

		m_min_pos = value;
		emit sigMinValueChanged(m_min_pos);
	}
	else if (tokens.at(0) == "M")
	{
		uint16_t value = tokens.at(1).toUInt();
		if (m_max_pos == value)
			return;

		m_max_pos = value;
		emit sigMaxValueChanged(m_max_pos);
	}
	else if (tokens.at(0) == "P")
	{
		uint16_t value = tokens.at(1).toUInt();
		if (m_position == value)
			return;

		m_position = value;
		emit sigPositionValueChanged(m_position);
	}
	else if (tokens.at(0) == "r")
	{
		uint8_t x = tokens.at(1).toUShort();
		QString sY = tokens.at(2);
		if (m_curve.at(x) == sY)
			return;

		m_curve[x] = sY;
		uint8_t y = tokens.at(2).toUShort();
		emit sigCurvePointChanged(QPoint(x, y));
		emit sigCurveChanged(m_curve);
	}
	else if (tokens.at(0) == "R")
	{
		line.replace("R:", "");
		QStringList array = line.split('|');
		if (m_curve == array)
			return;

		m_curve = array;
		if (m_curve.size() == 128)
			emit sigCurveChanged(m_curve);
	}
	else if (tokens.at(0) == "RESTART")
	{
		isStarted = false;
	}
}
} // namespace hhc
