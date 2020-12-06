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

#pragma once
#include "device.h"
#include <QtSerialPort/QSerialPort>

#ifdef Q_OS_WIN
	#pragma warning(disable : 4995)
#endif
/**
 * @brief Application specific namespace.
 */
namespace hhc {

class serial : public device
{
	Q_OBJECT

public:
	explicit serial(QObject *parent = nullptr);
	~serial() {}

	enum SerialPortError
	{
		NoError = 0,
		DeviceNotFoundError = 1,
		PermissionError = 2,
		OpenError = 3,
		ParityError = 4,
		FramingError = 5,
		BreakConditionError = 6,
		WriteError = 7,
		ReadError = 8,
		ResourceError = 9,
		UnsupportedOperationError = 10,
		UnknownError = 11,
		TimeoutError = 12,
		NotOpenError = 13
	};
	bool open(const QString &);
	Type type() const {return Type::Serial;}
	void scan();

private:
	void onErrorOccurredRedirect(QSerialPort::SerialPortError);
};
} // namespace hhc

#endif // ndef Q_OS_ANDROID
