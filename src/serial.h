/*
    SmartHHC - Electronic Drums HiHat Controller Manager
    Copyright (C) 2018 Andrea Zanellato

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef SERIAL_H
#define SERIAL_H

#include "device.h"

#include <QPoint>
#include <QSerialPort>

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
    QByteArray readAll();
    void write(const QByteArray &);
    void close();
    bool open(const QString &);
    QString errorString() const;
    Type type() const {return Type::Serial;}
    void sendCommand(const QString &);

    void scan();

private:
    void onErrorOccurredRedirect(QSerialPort::SerialPortError);
    void onSocketRead();

    QSerialPort *m_qSerial = nullptr;
};
} // namespace hhc

#endif // SERIAL_H
