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

class QLabel;
class QStackedWidget;
class QHBoxLayout;
class QVBoxLayout;
class CurveToolBar;
class CurveWidget;
class CurvePage : public QWidget
{
	Q_OBJECT

public:
	explicit CurvePage(QWidget *parent = nullptr);
	~CurvePage();

	CurveWidget *curveWidget() const;

	void setValue(int, int);

	void setValues(const QVector<int>);
	QVector<int> values() const;

private:
	void onKnotValueChanged(int, int);

	QVector<QLabel *> labels;

	QVBoxLayout *layout;
	QHBoxLayout *layCurveValues;
	CurveWidget *wgtCurve;
};
