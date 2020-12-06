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
#pragma once
#include <QWidget>

namespace Ui  {class PageHome;}
namespace qsw {class SpinBox;}

class QToolButton;
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

    qsw::SpinBox *sbxOpen()   const;
    qsw::SpinBox *sbxClosed() const;
    qsw::SpinBox *sbxFixed1() const;
    qsw::SpinBox *sbxFixed2() const;
    qsw::SpinBox *sbxFixed3() const;

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
