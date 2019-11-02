/*
	SmartHHC - Electronic Drums HiHat Controller Manager
	Copyright (C) 2018 Andrea Zanellato

	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#pragma once
#include <QWidget>

namespace Ui {
class PageTerminal;
}
class QLineEdit;
class PageTerminal : public QWidget
{
	Q_OBJECT

public:
	explicit PageTerminal(QWidget *parent = nullptr);
	~PageTerminal();

	void setConnected(bool);

	void write(const QString &);
	void clear();
	void setFocusCLI();
	void scrollDown();

	QLineEdit *txtCLI() const;

signals:
	void onGetData(const QByteArray &);

private:
	bool eventFilter(QObject *, QEvent *);

	Ui::PageTerminal *ui;
};
