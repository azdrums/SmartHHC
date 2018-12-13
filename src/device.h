/*
    SmartHHC - Electronic Drums HiHat Controller Manager
    Copyright (C) 2018 Andrea Zanellato

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef DEVICE_H
#define DEVICE_H

#include <QCoreApplication>
#include <QObject>
#include <QPoint>
#include <QTextCodec>
#include <QVector>

namespace hhc {
class device : public QObject
{
    Q_OBJECT

public:
    explicit device(QObject *parent = nullptr)
    :
    QObject(parent),
    isConnected_(false)
    {
        for (int i=0; i<128; i++)
            m_curve.append(QString());

         connect(QCoreApplication::instance(),
                &QCoreApplication::aboutToQuit, this, &device::onDisconnected);
    }
    virtual ~device() {}

    enum Type
    {
        Unknown = 0,
        Bluetooth = 1,
        Serial = 2
    };
    bool isConnected() const {return isConnected_;}

    uint8_t  fixed1()      const {return m_fixed1;}
    uint8_t  fixed2()      const {return m_fixed2;}
    uint8_t  fixed3()      const {return m_fixed3;}
    uint16_t minPosition() const {return m_min_pos;}
    uint16_t maxPosition() const {return m_max_pos;}
    uint16_t position()    const {return m_position;}
    uint16_t interval()    const {return m_interval;}
    QStringList curve()    const {return m_curve;}
    QPoint   curvePoint(uint8_t x) const
    {
        Q_ASSERT(x < 128);

        uint8_t y = m_curve.at(x).toUShort();
        return QPoint(x, y);
    }
    void setFixed1(uint8_t value)
    {
        QString cmd = "S f1 " + QString::number(value);
        sendCommand(cmd);
    }
    void setFixed2(uint8_t value)
    {
        QString cmd = "S f2 " + QString::number(value);
        sendCommand(cmd);
    }
    void setFixed3(uint8_t value)
    {
        QString cmd = "S f3 " + QString::number(value);
        sendCommand(cmd);
    }
    void setMinPosition(uint16_t value)
    {
        QString cmd = "S m " + QString::number(value);
        sendCommand(cmd);
    }
    void setMaxPosition(uint16_t value)
    {
        QString cmd = "S M " + QString::number(value);
        sendCommand(cmd);
    }
    void setInterval(uint16_t interval)
    {
        QString cmd = "C " + QString::number(interval);
        sendCommand(cmd);
    }
    void setCurvePoint(const QPoint &point)
    {
        QString sX  = QString::number(point.x()),
                sY  = QString::number(point.y()),
                cmd = "S r " + sX + " " + sY;
        sendCommand(cmd);
    }
    void requestData()
    {
        sendCommand("C 0");
        sendCommand("?");
        sendCommand("G m");
        sendCommand("G M");
        sendCommand("G f1");
        sendCommand("G f2");
        sendCommand("G f3");
        sendCommand("G R");
    }
    void writeFixed1() {sendCommand("W f1");}
    void writeFixed2() {sendCommand("W f2");}
    void writeFixed3() {sendCommand("W f3");}
    void writeMinMax() {sendCommand("W m");}
    void writeCurve()  {sendCommand("W R");}

    int portCount() const
    {
        return portInfos.count();
    }
    QString portName(int index) const
    {
        if (index < portInfos.count())
            return portInfos.at(index).at(0);

        return QString();
    }
    QStringList portInfosAt(int index)
    {
        if (index < portInfos.count())
            return portInfos.at(index);

        return QStringList();
    }
    void onConnected()
    {
        isConnected_= true;
        emit sigConnected();
    }
    void onDisconnected()
    {
        if (isConnected_)
        {
            isConnected_= false;
            sendCommand("C 0"); // clean up
            emit sigDisconnected();
        }
    }

//  @deprecated

    void onReadyReadRedirect()
    {
        emit sigReadyRead();
    }
    virtual QByteArray readAll() = 0;
    virtual void write(const QByteArray &) = 0;
    virtual void close() = 0;
    virtual bool open(const QString &) = 0;
    virtual QString errorString() const = 0;
    virtual Type type() const = 0;
    virtual void sendCommand(const QString &) = 0;

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
    void sigReadLine(const QString &);

//  @deprecated

    void sigReadyRead();

protected:
/**
    @deprecated
    @brief Processes the incoming data from the I/O device.
    @param data The data to process.

    Called by the @ref readAll() function, this method processes the data
    incoming from the I/O device.

    The data is sent at different times, so it happens that an information
    can be trunkated one time and continuing on the next one, so we cache
    last information saved using the m_cacheData QString member, so
    the next time we prepend the missing information to the new data.
    We ensure that the information value is complete and not the same
    as set previously, so we emit the related signals to notify the application
    about changed values.

    @see onSockedRead
 */
    void processIncomingData(const QByteArray &data)
    {
        QString strData = QTextCodec::codecForMib(106)->toUnicode(data); // 106 is UTF-8
        if (!m_cacheData.isEmpty())
            strData.prepend(m_cacheData);

        m_cacheData = strData.section("\r\n", -1);
        strData.remove(m_cacheData);
        QStringList lines = strData.split("\r\n");
        lines.removeAll(QString(""));

        foreach (QString line, lines)
            parse(line);
    }
/**
    @brief Processes the incoming data from the I/O device.
    @param data The data to process.

    Called by the @ref processIncomingData() or @ref onSocketRead() functions,
    this method processes the data incoming from the I/O device.
*/
    void parse(QString line)
    {
        QStringList tokens = line.split(':');

        if (!tokens.count())
        {
            return; // No data?
        }
        else if (tokens.at(0).left(3) == "HHC")
        {
        //  Ready to get commands, enable command logging at 0.1s
            if (m_interval == 0)
            {
                sendCommand("C 100");
                m_interval = 100;
                emit sigIntervalValueChanged(m_interval);
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
    }
    uint8_t      m_fixed1    = 0, // 1st fixed pedal position value.
                 m_fixed2    = 0, // 2nd fixed pedal position value.
                 m_fixed3    = 0, // 3rd fixed pedal position value.
                 m_lastIndex = 0; // Last array index saved.
    uint16_t     m_min_pos   = 0, // Minimum free pedal position value.
                 m_max_pos   = 0, // Maximum free pedal position value.
                 m_position  = 0, // Current free pedal position value.
                 m_interval  = 0;
    QStringList  m_curve;
    QString      m_cacheData;
    bool         isConnected_;

    QVector<QStringList> portInfos;
};
} // namespace hhc

#endif // DEVICE_H
