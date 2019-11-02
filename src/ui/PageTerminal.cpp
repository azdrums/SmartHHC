/*
	SmartHHC - Electronic Drums HiHat Controller Manager
	Copyright (C) 2018 Andrea Zanellato

	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
