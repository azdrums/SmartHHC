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
#include <QResizeEvent>
#include <QToolButton>

#include "PageHome.h"
#include "ui_PageHome.h"

PageHome::PageHome(QWidget *parent)
:
	QWidget(parent),
	ui(new Ui::PageHome)
{
	ui->setupUi(this);

	ui->pbrHHLevel->addLevel(0, Qt::green);
	ui->pbrHHLevel->addLevel(0, Qt::red);

	ui->pageBottomRight->setLayout(ui->layBottomRight);
	ui->pageTopLeft->setLayout(ui->layTopLeft);

	setLayout(ui->layout);
	setSetupClean();

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
#endif
}
PageHome::~PageHome()
{
	delete ui;
}
void PageHome::setFixed1(int value)
{
	// Don't emit events programmatically
	ui->sbxFixed1->blockSignals(true);
	ui->sbxFixed1->setValue(value);
	ui->sbxFixed1->blockSignals(false);
}
void PageHome::setFixed2(int value)
{
	ui->sbxFixed2->blockSignals(true);
	ui->sbxFixed2->setValue(value);
	ui->sbxFixed2->blockSignals(false);
}
void PageHome::setFixed3(int value)
{
	ui->sbxFixed3->blockSignals(true);
	ui->sbxFixed3->setValue(value);
	ui->sbxFixed3->blockSignals(false);
}
void PageHome::setMinPosition(int value)
{
	ui->sbxClosed->blockSignals(true);
	ui->sbxClosed->setValue(value);
	ui->sbxClosed->blockSignals(false);

	ui->pbrHHLevel->blockSignals(true);
	ui->pbrHHLevel->setLevel(0, value);
	ui->pbrHHLevel->blockSignals(false);
}
void PageHome::setMaxPosition(int value)
{
	ui->sbxOpen->blockSignals(true);
	ui->sbxOpen->setValue(value);
	ui->sbxOpen->blockSignals(false);

	ui->pbrHHLevel->blockSignals(true);
	ui->pbrHHLevel->setLevel(1, value);
	ui->pbrHHLevel->blockSignals(false);
}
void PageHome::setPosition(int position)
{
	ui->pbrHHLevel->blockSignals(true);
	ui->lblHHLevel->setText(QString::number(position));
	ui->pbrHHLevel->setValue(position);
	ui->pbrHHLevel->blockSignals(false);
}
int PageHome::fixed1() const
{
	return ui->sbxFixed1->value();
}
int PageHome::fixed2() const
{
	return ui->sbxFixed2->value();
}
int PageHome::fixed3() const
{
	return ui->sbxFixed3->value();
}
int PageHome::minPosition() const
{
	return ui->sbxClosed->value();
}
int PageHome::maxPosition() const
{
	return ui->sbxOpen->value();
}
int PageHome::position() const
{
	return ui->lblHHLevel->text().toInt();
}
void PageHome::disablePedalSetButton(QToolButton *button)
{
	if (!button)
		return;

	button->setEnabled(false);

	if (isSetupChanged())
		setSetupChanged(true);
	else
		setSetupUnsaved();
}
QToolButton *PageHome::tbnSetFixed1() const
{
	return ui->tbnSetFixed1;
}
QToolButton *PageHome::tbnSetFixed2() const
{
	return ui->tbnSetFixed2;
}
QToolButton *PageHome::tbnSetFixed3() const
{
	return ui->tbnSetFixed3;
}
QToolButton *PageHome::tbnSetClosed() const
{
	return ui->tbnSetClosed;
}
QToolButton *PageHome::tbnSetOpen() const
{
	return ui->tbnSetOpen;
}
qsw::SpinBox *PageHome::sbxOpen() const
{
	return ui->sbxOpen;
}
qsw::SpinBox *PageHome::sbxClosed() const
{
	return ui->sbxClosed;
}
qsw::SpinBox *PageHome::sbxFixed1() const
{
	return ui->sbxFixed1;
}
qsw::SpinBox *PageHome::sbxFixed2() const
{
	return ui->sbxFixed2;
}
qsw::SpinBox *PageHome::sbxFixed3() const
{
	return ui->sbxFixed3;
}
QToolButton *PageHome::tbnRestore() const
{
	return ui->tbnRestore;
}
QToolButton *PageHome::tbnSetAsOpen() const
{
	return ui->tbnSetAsOpen;
}
QToolButton *PageHome::tbnSetAsClosed() const
{
	return ui->tbnSetAsClosed;
}
QToolButton *PageHome::tbnSetAll() const
{
	return ui->tbnSetAll;
}
QToolButton *PageHome::tbnSaveAll() const
{
	return ui->tbnSaveAll;
}
void PageHome::setSetupClean()
{
	ui->tbnSetClosed->setEnabled(false);
	ui->tbnSetOpen  ->setEnabled(false);
	ui->tbnSetFixed1->setEnabled(false);
	ui->tbnSetFixed2->setEnabled(false);
	ui->tbnSetFixed3->setEnabled(false);
	setSetupChanged(false);
}
void PageHome::setSetupChanged(bool changed)
{
	ui->tbnRestore->setEnabled(changed);
	ui->tbnSaveAll->setEnabled(changed);
	ui->tbnSetAll ->setEnabled(changed);
}
void PageHome::setSetupUnsaved()
{
	ui->tbnRestore->setEnabled(false);
	ui->tbnSetAll ->setEnabled(false);
	ui->tbnSaveAll->setEnabled(true);
}
bool PageHome::isSetupChanged()
{
	return ui->tbnSetClosed->isEnabled() ||
			 ui->tbnSetOpen  ->isEnabled() ||
			 ui->tbnSetFixed1->isEnabled() ||
			 ui->tbnSetFixed2->isEnabled() ||
			 ui->tbnSetFixed3->isEnabled();
}
void PageHome::setConnected(bool connected)
{
	setEnabled(connected);

	if (!connected)
	{
		ui->sbxFixed1->blockSignals(true);
		ui->sbxFixed1->setValue(0);
		ui->sbxFixed1->blockSignals(false);

		ui->sbxFixed2->blockSignals(true);
		ui->sbxFixed2->setValue(0);
		ui->sbxFixed2->blockSignals(false);

		ui->sbxFixed3->blockSignals(true);
		ui->sbxFixed3->setValue(0);
		ui->sbxFixed3->blockSignals(false);

		ui->sbxOpen->blockSignals(true);
		ui->sbxOpen->setValue(0);
		ui->sbxOpen->blockSignals(false);

		ui->sbxClosed->blockSignals(true);
		ui->sbxClosed->setValue(0);
		ui->sbxClosed->blockSignals(false);

		ui->pbrHHLevel->blockSignals(true);
		ui->pbrHHLevel->setValue(0);
		ui->pbrHHLevel->blockSignals(false);

		ui->lblHHLevel->setText("0");
	}
}
#ifdef Q_OS_ANDROID
void PageHome::resizeEvent(QResizeEvent *event)
{
	ui->pageBottomRight->hide();
	ui->pageTopLeft->hide();

	if (event->size().width() > event->size().height())
	{
		ui->layPanels->addWidget(ui->pageTopLeft, 0, 0, 1, 1);
		ui->layPanels->addWidget(ui->pageBottomRight, 0, 1, 1, 1);
	}
	else
	{
		ui->layPanels->addWidget(ui->pageTopLeft, 0, 0, 1, 1);
		ui->layPanels->addWidget(ui->pageBottomRight, 1, 0, 1, 1);
	}

	ui->pageBottomRight->show();
	ui->pageTopLeft->show();
}
#endif
