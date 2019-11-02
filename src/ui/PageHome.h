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
class PageHome;
}
class QToolButton;
class SpinBox;
class PageHome : public QWidget
{
	Q_OBJECT

public:
	explicit PageHome(QWidget *parent = nullptr);
	~PageHome();

	void setFixed1(int);
	void setFixed2(int);
	void setFixed3(int);
	void setMinPosition(int);
	void setMaxPosition(int);
	void setPosition(int);

	int fixed1() const;
	int fixed2() const;
	int fixed3() const;
	int minPosition() const;
	int maxPosition() const;
	int position() const;

	void disablePedalSetButton(QToolButton *);

	QToolButton *tbnSetFixed1() const;
	QToolButton *tbnSetFixed2() const;
	QToolButton *tbnSetFixed3() const;
	QToolButton *tbnSetClosed() const;
	QToolButton *tbnSetOpen()   const;

	SpinBox *sbxOpen()   const;
	SpinBox *sbxClosed() const;
	SpinBox *sbxFixed1() const;
	SpinBox *sbxFixed2() const;
	SpinBox *sbxFixed3() const;

	QToolButton *tbnRestore()	 const;
	QToolButton *tbnSetAsOpen()   const;
	QToolButton *tbnSetAsClosed() const;
	QToolButton *tbnSetAll()	  const;
	QToolButton *tbnSaveAll()	 const;

	bool isSetupChanged();
	void setSetupClean();
	void setSetupUnsaved();
	void setSetupChanged(bool);

	void setConnected(bool);

private:
#ifdef Q_OS_ANDROID
	void resizeEvent(QResizeEvent *);
#endif
	Ui::PageHome *ui;
};
