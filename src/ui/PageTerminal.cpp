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
#include <QKeyEvent>
#include <QScrollBar>

#include "PageTerminal.h"
#include "ui_PageTerminal.h"

PageTerminal::PageTerminal(QWidget *parent)
:
	QWidget(parent),
	ui(new Ui::PageTerminal)
{
	ui->setupUi(this);

	setLayout(ui->layout);

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
	ui->txtCLI->installEventFilter(this);

	connect(ui->tbnClear, &QToolButton::clicked, ui->txtLog,
			&QPlainTextEdit::clear);
}
PageTerminal::~PageTerminal()
{
	delete ui;
}
bool PageTerminal::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() == QEvent::KeyRelease && object == ui->txtCLI)
	{
		QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

		if ((keyEvent->key() == Qt::Key_Enter) || (keyEvent->key() == Qt::Key_Return))
		{
			QString text = ui->txtCLI->text() + "\r\n";

			emit onGetData(text.toUtf8());
			ui->txtCLI->clear();

			return true;
		}
	}
	return false;

}
void PageTerminal::setConnected(bool connected)
{
	ui->txtCLI->setEnabled(connected);
	ui->tbnClear->setEnabled(connected);
}
void PageTerminal::write(const QString &text)
{
	ui->txtLog->insertPlainText(text);
}
void PageTerminal::clear()
{
	ui->txtLog->clear();
}
void PageTerminal::setFocusCLI()
{
	ui->txtLog->setFocus();
}
void PageTerminal::scrollDown()
{
	QScrollBar *bar = ui->txtLog->verticalScrollBar();
	bar->setValue(bar->maximum());
}
QLineEdit *PageTerminal::txtCLI() const
{
	return ui->txtCLI;
}
