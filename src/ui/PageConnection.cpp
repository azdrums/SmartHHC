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
#include <QComboBox>
#include <QDebug>
#include <QToolButton>

#include "PageConnection.h"
#include "ui_PageConnection.h"

#include <qsw/spinbox.h>

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
    connect(ui->sbxInterval, QOverload<int>::of(&qsw::SpinBox::valueChanged),
			this, &PageConnection::onIntervalChanged);

	connect(ui->tbnInterval, &QToolButton::clicked, [=]()
	{
		emit sigSettingInterval();
		ui->tbnInterval->setEnabled(false);
	});
	connect(ui->tbnRestart, &QToolButton::clicked, [=]()
	{
		emit sigRestart();
	});
#ifdef Q_OS_ANDROID
	connect(ui->cbxDevice, QOverload<int>::of(&QComboBox::currentIndexChanged),
			this, &PageConnection::onBluetoothDeviceCurrentIndexChanged);
#endif
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
	ui->tbnRestart->setEnabled(connected);

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
void PageConnection::showNotFound()
{
	ui->lblNotFound->show();
	ui->lblInterval->hide();
	ui->sbxInterval->hide();
	ui->tbnInterval->hide();
	ui->gbxBluetooth->hide();
	ui->gbxSerialPort->hide();
	ui->tbnRestart->hide();
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
	ui->tbnRestart->show();
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
#else
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
	ui->tbnRestart->show();
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
void PageConnection::updateBluetoothServiceScanProgress(const QString &message)
{
	ui->txtScanServices->setText(message);
}
void PageConnection::bluetoothServiceScanFinished()
{
	ui->txtScanServices->setText(tr("Completed."));
}
#endif
