/*
    SmartHHC - Electronic Drums HiHat Controller Manager
    Copyright (C) 2018 Andrea Zanellato

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include <QtGlobal>
#ifndef Q_OS_ANDROID
#ifndef SERIAL_H
#define SERIAL_H

#include <QSerialPort>

#include "device.h"
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

#endif // SERIAL_H
#endif // ndef Q_OS_ANDROID
