/*
	SmartHHC - Electronic Drums HiHat Controller Manager
	Copyright (C) 2018 Andrea Zanellato

	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
