/*
    SmartHHC - Electronic Drums HiHat Controller Manager
    Copyright (C) 2018 Andrea Zanellato

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "serial.h"

#include <QSerialPortInfo>

namespace hhc {

serial::serial(QObject *parent)
:
    device(parent),
    m_qSerial(new QSerialPort(this))
{
    connect(m_qSerial, &QSerialPort::errorOccurred, this, &serial::onErrorOccurredRedirect);
    connect(m_qSerial, &QSerialPort::readyRead,     this, &serial::onSocketRead);
    connect(m_qSerial, &QSerialPort::aboutToClose,  this, &serial::onDisconnected);
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
bool serial::open(const QString &portName)
{
    m_qSerial->setPortName(portName);
    m_qSerial->setBaudRate(38400);
    m_qSerial->setDataBits(QSerialPort::Data8);
    m_qSerial->setParity(QSerialPort::NoParity);
    m_qSerial->setStopBits(QSerialPort::OneStop);
    m_qSerial->setFlowControl(QSerialPort::NoFlowControl);

    if (m_qSerial->open(QIODevice::ReadWrite))
    {
        onConnected();
        return true;
    }
    return false;
}
void serial::close()
{
    if (m_qSerial->isOpen())
    {
        m_qSerial->close();
        m_fixed1    =
        m_fixed2    =
        m_fixed3    =
        m_lastIndex =
        m_min_pos   =
        m_max_pos   =
        m_position  =
        m_interval  = 0;
        m_curve     = QStringList();
    }
    onDisconnected();
}
QString serial::errorString() const
{
    return m_qSerial->errorString();
}
void serial::write(const QByteArray &data)
{
    m_qSerial->write(data);
}
QByteArray serial::readAll()
{
    const QByteArray data = m_qSerial->readAll();
    processIncomingData(data);
    return data;
}
void serial::sendCommand(const QString &cmd)
{
    QByteArray data = cmd.toUtf8() + "\r\n";
    m_qSerial->write(data);
}
void serial::onErrorOccurredRedirect(QSerialPort::SerialPortError error)
{
/*
    TODO: Check if needed or remove
    if (!m_qSerial->isOpen())
        onDisconnected();
*/
    emit sigError(error);
}
void serial::onSocketRead()
{
    if (!m_qSerial->isReadable())
        return;

    while (m_qSerial->canReadLine())
    {
        QByteArray line = m_qSerial->readLine();
        QString strLine = QString::fromUtf8(line.constData(), line.length());

        parse(strLine);
//      qDebug() << strLine;
        emit sigReadLine(strLine);
    }
}
} // namespace hhc
