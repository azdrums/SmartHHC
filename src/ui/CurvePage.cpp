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
#include <QAction>
#include <QLabel>
#include <QMenu>
#include <QStackedWidget>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QVector>

#include "CurvePage.h"
#include "CurveWidget.h"

CurvePage::CurvePage(QWidget *parent)
:
	QWidget(parent),
	labels(QVector<QLabel *>(HHCurve::KnotCount)),
	layout(new QVBoxLayout(this)),
	layCurveValues(new QHBoxLayout()),
	wgtCurve(new CurveWidget(this))
{
	layout->setSpacing(0);
	layout->setSizeConstraint(QLayout::SetNoConstraint);
	layout->setContentsMargins(0, 0, 0, 0);

	wgtCurve->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	layout->addWidget(wgtCurve);

	layCurveValues->setSpacing(0);
	layCurveValues->setSizeConstraint(QLayout::SetNoConstraint);
	layCurveValues->setContentsMargins(6, 6, 6, 6);
	layout->addLayout(layCurveValues);

	for (int i = 0; i < labels.size(); i++)
	{
		labels[i] = new QLabel(this);
		labels[i]->setText(QString::number(wgtCurve->knotValues().at(i)));

		if (i < labels.size() - 1)
			labels[i]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		else
			labels[i]->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

		layCurveValues->addWidget(labels.at(i));
	}
	connect(wgtCurve, &CurveWidget::knotValueChanged, this,
			&CurvePage::onKnotValueChanged);
}
CurvePage::~CurvePage()
{
	delete layCurveValues;
}
CurveWidget *CurvePage::curveWidget() const
{
	return wgtCurve;
}
void CurvePage::onKnotValueChanged(int index, int value)
{
//  Update labels.
	labels[index]->setText(QString::number(value));
}
void CurvePage::setValue(int index, int value)
{
	Q_ASSERT(index < HHCurve::MaxValue + 1);

	wgtCurve->setCurveValueAt(index, value);
}
QVector<int> CurvePage::values() const
{
	return wgtCurve->curveValues();
}
void CurvePage::setValues(const QVector<int> values)
{
	wgtCurve->setCurveValues(values);

	for (int i = 0; i < labels.count(); ++i)
		labels[i]->setText(QString::number(wgtCurve->knotValues().at(i)));
}
