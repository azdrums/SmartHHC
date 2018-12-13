/*
    SmartHHC - Electronic Drums HiHat Controller Manager
    Copyright (C) 2018 Andrea Zanellato

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include <QComboBox>
#include <QDebug>
#include <QToolButton>

#include "PageConnection.h"
#include "ui_PageConnection.h"

#include "spinbox.h"

PageConnection::PageConnection(QWidget *parent)
:
    QWidget(parent),
    ui(new Ui::PageConnection)
{
    ui->setupUi(this);

    ui->gbxSerialPort->setLayout(ui->laySerialPort);
    ui->gbxBluetooth ->setLayout(ui->layBluetooth);
    setLayout(ui->layout);

    showNotFound();

#ifdef Q_OS_ANDROID
    QColor enabled  = palette().color(QPalette::Active,   QPalette::ButtonText);
    QColor disabled = palette().color(QPalette::Disabled, QPalette::ButtonText);
    setStyleSheet(QString("QToolButton:enabled  {color: rgb(%1, %2, %3)}"
                          "QToolButton:disabled {color: rgb(%4, %5, %6)}")
                          .arg(enabled.red())
                          .arg(enabled.green())
                          .arg(enabled.blue())
                          .arg(disabled.red())
                          .arg(disabled.green())
                          .arg(disabled.blue()));
#else
    ui->lblScanServices->hide();
    ui->txtScanServices->hide();

    connect(ui->cbxPort, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &PageConnection::onSerialPortCurrentIndexChanged);
#endif
    connect(ui->sbxInterval, QOverload<int>::of(&SpinBox::valueChanged),
            this, &PageConnection::onIntervalChanged);

    connect(ui->tbnInterval, &QToolButton::clicked, [=]()
    {
        emit sigSettingInterval();
        ui->tbnInterval->setEnabled(false);
    });
    connect(ui->cbxDevice, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &PageConnection::onBluetoothDeviceCurrentIndexChanged);
}
PageConnection::~PageConnection()
{
    delete ui;
}
void PageConnection::setConnected(bool connected)
{
    ui->lblPort->setEnabled(!connected);
    ui->cbxPort->setEnabled(!connected);
    ui->sbxInterval->setEnabled(connected);

    if (!connected)
    {
        ui->sbxInterval->blockSignals(true);
        ui->sbxInterval->setValue(0);
        ui->sbxInterval->blockSignals(false);
        ui->sbxInterval->setEnabled(false);
        ui->tbnInterval->setEnabled(false);
    }
}
void PageConnection::showScanInProgress()
{
    ui->cbxPort  ->clear();
    ui->cbxDevice->clear();
    ui->txtScanDevice  ->setText(tr("Scanning..."));
    ui->txtScanServices->setText(tr("Scanning..."));
}
int PageConnection::interval() const
{
    return ui->sbxInterval->value();
}
void PageConnection::setInterval(int interval)
{
    ui->sbxInterval->blockSignals(true);
    ui->sbxInterval->setValue(interval);
    ui->sbxInterval->blockSignals(false);
}
void PageConnection::setOldInterval(int interval)
{
    oldIntervalValue = interval;
}
void PageConnection::setIntervalChanged(bool enable)
{
    ui->tbnInterval->setEnabled(enable);
}
void PageConnection::onIntervalChanged(int value)
{
    emit sigIntervalChanged(value);

    if (oldIntervalValue == 0 && value > 0 && value < 100)
        ui->sbxInterval->setValue(100);
    else if (oldIntervalValue > 0 && value > 0 && value < 100)
        ui->sbxInterval->setValue(0);

    oldIntervalValue = ui->sbxInterval->value();
}
#ifndef Q_OS_ANDROID
ConnectionType PageConnection::connectionType() const
{
    if (ui->gbxBluetooth->isChecked())
        return ConnectionType::Bluetooth;
    else if (ui->gbxSerialPort->isChecked())
        return ConnectionType::Serial;

    return ConnectionType::Unknown;
}
QString PageConnection::serialPortName() const
{
    return ui->cbxPort->currentText();
}
int PageConnection::serialPortCount() const
{
    return ui->cbxPort->count();
}
void PageConnection::addSerialPort(const QString &name, const QStringList &infos)
{
    ui->cbxPort->addItem(QIcon(":/icons/serial.png"), name, infos);
    ui->cbxPort->setEnabled(true);
}
void PageConnection::showSerialPortOptions()
{
    ui->lblNotFound->hide();
    ui->lblInterval->show();
    ui->sbxInterval->show();
    ui->tbnInterval->show();
    ui->gbxSerialPort->show();
}
void PageConnection::toggleSerialPort(bool checked)
{
    ui->gbxBluetooth ->setChecked(!checked);
    ui->gbxSerialPort->setChecked(checked);
}
void PageConnection::resetSerialPortInfos()
{
    ui->txtDescription->setText(tr("N/A"));
    ui->txtManufacturer->setText(tr("N/A"));
    ui->txtSN->setText(tr("N/A"));
    ui->txtLocation->setText(tr("N/A"));
    ui->txtVID->setText(tr("N/A"));
    ui->txtPID->setText(tr("N/A"));
}
void PageConnection::setSerialPortCurrentIndex(int index)
{
    ui->cbxPort->setCurrentIndex(index);
}
void PageConnection::onSerialPortCurrentIndexChanged(int index)
{
    emit sigSerialPortIndexChanged(index);
    emit sigSerialPortIndexChanged(ui->cbxPort->currentText());

    if (index == -1)
        return;

    const QStringList list = ui->cbxPort->itemData(index).toStringList();
    if (list.isEmpty())
        return;

    ui->txtDescription->setText(list.at(1));
    ui->txtManufacturer->setText(list.at(2));
    ui->txtSN->setText(list.at(3));
    ui->txtLocation->setText(list.at(4));
    ui->txtVID->setText(list.at(5));
    ui->txtPID->setText(list.at(6));
}
#endif
QString PageConnection::bluetoothAddress() const
{
    return ui->txtAddress->text();
}
void PageConnection::addBluetoothDevice(const QString &name, const QString &address)
{
    ui->cbxDevice->addItem(QIcon(":/icons/bluetooth.png"), name, address);
}
void PageConnection::showBluetoothDeviceOptions()
{
    ui->lblNotFound->hide();
    ui->lblInterval->show();
    ui->sbxInterval->show();
    ui->tbnInterval->show();
    ui->gbxBluetooth->show();
}
void PageConnection::showNotFound()
{
    ui->lblNotFound->show();
    ui->lblInterval->hide();
    ui->sbxInterval->hide();
    ui->tbnInterval->hide();
    ui->gbxBluetooth->hide();
    ui->gbxSerialPort->hide();
}
int PageConnection::bluetoothDeviceCount()
{
    return ui->cbxDevice->count();
}
void PageConnection::setBluetoothDeviceCurrentIndex(int index)
{
    ui->cbxDevice->setCurrentIndex(index);
}
void PageConnection::updateBluetoothDeviceScanProgress(const QString &message)
{
    ui->txtScanDevice->setText(message);
}
void PageConnection::bluetoothDeviceScanFinished()
{
    ui->txtScanDevice->setText(tr("Completed."));
}
void PageConnection::onBluetoothDeviceCurrentIndexChanged(int index)
{
    emit sigBluetoothDeviceIndexChanged(index);
    emit sigBluetoothDeviceIndexChanged(ui->cbxDevice->currentText());

    if (index == -1)
        return;

    const QString address = ui->cbxDevice->itemData(index).toString();

    ui->txtAddress->setText(address);
}
#ifdef Q_OS_ANDROID
void PageConnection::updateBluetoothServiceScanProgress(const QString &message)
{
    ui->txtScanServices->setText(message);
}
void PageConnection::bluetoothServiceScanFinished()
{
    ui->txtScanServices->setText(tr("Completed."));
}
#endif
