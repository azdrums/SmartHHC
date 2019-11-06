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
#include <QObject>
#include <QVector>

#ifdef Q_OS_ANDROID
	class QBluetoothSocket;
#else
	class QSerialPort;
#endif
namespace hhc {

class device : public QObject
{
	Q_OBJECT

public:
	enum Type
	{
		Unknown   = 0,
		Bluetooth = 1,
		Serial    = 2
	};
	enum ConnectionState
	{
		DisconnectedState = 0,
		ConnectingState   = 2,
		ConnectedState    = 3
	};
	explicit device(QObject *parent = nullptr);
	virtual ~device();
/**
	Returns the connection state.
*/
	ConnectionState state() const {return connState;}
/**
	Returns the first fixed position.
*/
	uint8_t fixed1() const {return m_fixed1;}
/**
	Returns the second fixed position.
*/
	uint8_t fixed2() const {return m_fixed2;}
/**
	Returns the third fixed position.
*/
	uint8_t fixed3() const {return m_fixed3;}
	uint16_t minPosition() const {return m_min_pos;}
	uint16_t maxPosition() const {return m_max_pos;}
	uint16_t position() const {return m_position;}
	uint16_t interval() const {return m_interval;}
	QStringList curve() const {return m_curve;}
	QPoint curvePoint(uint8_t) const;
	void setFixed1(uint8_t);
	void setFixed2(uint8_t);
	void setFixed3(uint8_t);
	void setMinPosition(uint16_t);
	void setMaxPosition(uint16_t);
	void setInterval(uint16_t);
	void setCurvePoint(const QPoint &);
	void requestData();
	void writeFixed1() {sendCommand("W f1");}
	void writeFixed2() {sendCommand("W f2");}
	void writeFixed3() {sendCommand("W f3");}
	void writeMinMax() {sendCommand("W m");}
	void writeCurve()  {sendCommand("W R");}

	int portCount() const {return portInfos.count();}
	QString portName(int index) const;
	QStringList portInfosAt(int index);
	void onConnected();
	void onDisconnected();

	virtual bool open(const QString &) = 0;
	virtual Type type() const = 0;
	virtual void scan() = 0;

	virtual void close();
	virtual void onSocketRead();
	virtual void write(const QByteArray &);
	virtual QString errorString() const;
	virtual void sendCommand(const QString &);

signals:
	void sigConnected();
	void sigCurveChanged(const QStringList &);
	void sigCurvePointChanged(const QPoint &);
	void sigDisconnected();
	void sigError(uint8_t);
	void sigLastIndexChanged(const uint8_t);
	void sigFixed1ValueChanged(const uint8_t);
	void sigFixed2ValueChanged(const uint8_t);
	void sigFixed3ValueChanged(const uint8_t);
	void sigIntervalValueChanged(const uint16_t);
	void sigMaxValueChanged(const uint16_t);
	void sigMinValueChanged(const uint16_t);
	void sigPositionValueChanged(const uint16_t);
/**
	Triggered when the device is ready, when it notify the version message.
*/
	void sigReady();
	void sigReadLine(const QString &);

protected:
/**
	Called by the onSocketRead() function,
	this method processes the data incoming from the I/O device.
*/
	void parse(QString);

	uint8_t  m_fixed1	= 0, // 1st fixed pedal position value.
					 m_fixed2	= 0, // 2nd fixed pedal position value.
					 m_fixed3	= 0, // 3rd fixed pedal position value.
					 m_lastIndex = 0; // Last array index saved.
	uint16_t m_min_pos   = 0, // Minimum free pedal position value.
					 m_max_pos   = 0, // Maximum free pedal position value.
					 m_position  = 0, // Current free pedal position value.
					 m_interval  = 0;
	QStringList  m_curve;

	ConnectionState connState;

	QVector<QStringList> portInfos;

#ifdef Q_OS_ANDROID
	QBluetoothSocket *qIoDevice;
#else
	QSerialPort *qIoDevice;
#endif

private:
	bool isStarted; // TODO: Remove the data notification from firmware and then
									// this flag.
};
} // namespace hhc
